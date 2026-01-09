//#############################################################################
//
// FILE:   main.c
//
// TITLE:  Main
//
// Modification from Empty Bit-Field & Driverlib Example by rnr
//
// This example is an empty project setup for Bit-Field and Driverlib development.
//
//#############################################################################
// $TI Release: F2837xS Support Library v3.03.00.00 $
// $Release Date: Thu Dec  7 18:53:06 CST 2017 $
// $Copyright:
// Copyright (C) 2014-2017 Texas Instruments Incorporated - http://www.ti.com/
// 
//#############################################################################
// Compiler ver.15.12.5.LTS
// 
//
// Included Files
//
#include "F28x_Project.h"
#include "device.h"

#include "easydsp\easy2837xS_sci_v8.5.h"
#include "easydsp\RingBuff.h"
#include "stdlib.h"                            //250914 add
#include "variables.h"
#include "cc.h"
#include "filter.h"
#include "fault.h"
#include "dido.h"
//#include "CANlibrary.h"


/*typedef enum {                                                            //251103 add
    PI_AWU_NONE = 0,         // 안티-와인드업 비활성화                          //251103 add
    PI_AWU_CLAMP_HOLD_INT,   // 포화 방향에서 적분 정지 (clamping)              //251103 add
    PI_AWU_BACK_CALC         // 백-계산: integrator += Kaw*(u - u_unsat)    //251103 add
} PI_AWU_Mode;                                                            //251103 add


typedef struct {
    // gains & timing
    float kp;     // 비례 게인
    float ki;     // 연속계 적분 게인 [1/s]
    float Ts;     // 샘플링 주기 [s]                                         //251103 add

    // state                                                              //251103 add
    float integrator; // 적분 상태                                             //251103 add
    float prev_err;   // 필요시 사용 (현재는 미사용)                               //251103 add

    // limits                                                             //251103 add
    float u_min; // 출력 하한                                                 //251103 add
    float u_max; // 출력 상한                                                 //251103 add

    // anti-windup                                                       //251103 add
    PI_AWU_Mode awu_mode;                                                    //251103 add
    float kaw;   // back-calculation 게인 (PI_AWU_BACK_CALC에서만 사용)         //251103 add
} PIcontrol;                                                                       //251103 add

static inline void PI_Init(PIcontrol* c, float kp, float ki, float Ts,             //251103 add
                           float u_min, float u_max,                               //251103 add
                           PI_AWU_Mode mode, float kaw)                            //251103 add
{                                                                                  //251103 add
    c->kp = kp;                                                                    //251103 add
    c->ki = ki;                                                                    //251103 add
    c->Ts = Ts;                                                                    //251103 add
    c->integrator = 0.0f;                                                          //251103 add
    c->prev_err = 0.0f;                                                            //251103 add
    c->u_min = u_min;                                                              //251103 add
    c->u_max = u_max;                                                              //251103 add
    c->awu_mode = mode;                                                            //251103 add
    c->kaw = (kaw < 0.0f) ? 0.0f : kaw;                                            //251103 add
}                                                                                  //251103 add

// 적분/상태 리셋                                                                    //251103 add
static inline void PI_Reset(PIcontrol* c, float integrator_init)                   //251103 add
{                                                                                  //251103 add
    c->integrator = integrator_init;                                               //251103 add
    c->prev_err = 0.0f;                                                            //251103 add
}                                                                                  //251103 add

// 런타임 게인 변경                                                                  //251103 add
static inline void PI_SetGains(PIcontrol* c, float kp, float ki)                         //251103 add
{                                                                                 //251103 add
    c->kp = kp;                                                                   //251103 add
    c->ki = ki;                                                                   //251103 add
}                                                                                 //251103 add

// 한 스텝 업데이트: ref-측정 -> 출력(포화 적용)                                          //251103 add
static inline float PI_Update(PIcontrol* c, float ref, float meas)                //251103 add
{                                                                                 //251103 add
    // 1) 오차                                                                   //251103 add
    float e = ref - meas;                                                         //251103 add

    // 2) P, I 후보 계산                                                          //251103 add
    float up = c->kp * e;                                                         //251103 add
    float ui_next = c->integrator + (c->ki * c->Ts) * e;                          //251103 add

    // 3) 포화 전 출력                                                             //251103 add
    float u_unsat = up + ui_next;                                                 //251103 add

    // 4) 포화 적용                                                                //251103 add
    float u = u_unsat;                                                            //251103 add
    if (u > c->u_max) u = c->u_max;                                               //251103 add
    if (u < c->u_min) u = c->u_min;                                               //251103 add

        // 5) 안티-와인드업                                                            //251103 add
    if (c->awu_mode == PI_AWU_CLAMP_HOLD_INT) {                                  //251103 add
        // 포화 방향으로 더 밀어붙일 때 적분 업데이트 정지                                    //251103 add
        int sat_hi = (u >= c->u_max - 1e-12f);                                   //251103 add
        int sat_lo = (u <= c->u_min + 1e-12f);                                   //251103 add
        int pushing_hi = sat_hi && (u_unsat > c->u_max);                         //251103 add
        int pushing_lo = sat_lo && (u_unsat < c->u_min);                         //251103 add

        if (!(pushing_hi || pushing_lo)) {                                       //251103 add
            c->integrator = ui_next;  // 포화가 아니거나 반대방향이면 적분 허용             //251103 add
        }                                                                        //251103 add
        // 그렇지 않으면 integrator 유지                                            //251103 add
    } else if (c->awu_mode == PI_AWU_BACK_CALC) {                                //251103 add
        // 백-계산: 포화 오차로 integrator를 되돌림                                    //251103 add
        float aw = c->kaw * (u - u_unsat);       // (<= 0 또는 >= 0)              //251103 add
        c->integrator = ui_next + aw;            // windup 빠르게 해소              //251103 add
    } else {                                                                     //251103 add
        // 안티-와인드업 비활성화                                                     //251103 add
        c->integrator = ui_next;                                                 //251103 add
    }                                                                            //251103 add

    c->prev_err = e;                                                             //251103 add
    return u;                                                                    //251103 add
}                                                                                //251103 add

static PIcontrol pi;                                                             //251103 add
float reference = 200;                                       //251103 add*/
/*float PIvalue = 0;                                         //251103 add*/

int adcintnum = 0;
void dout(variable_dout);
unsigned int din(void);
int num1 = 0;
int flag1 = 0;                                                //251022 add

//float babo = 0;
//unsigned int babo2 = 0;
unsigned int SetLed = 0;
unsigned int gpbData = 0;
//
// Main
//

extern interrupt void offset(void);
extern interrupt void adc_isr(void){
    AdcdRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}                                                                   //250314 activate

void PWM_Trip(void){
    EALLOW;
    EPwm1Regs.TZFRC.bit.OST = 1;                                                                              //250913 add
    EPwm2Regs.TZFRC.bit.OST = 1;                                                                              //250913 add
    EPwm8Regs.TZFRC.bit.OST = 1;                                                                              //250913 add
    EPwm9Regs.TZFRC.bit.OST = 1;                                                                              //250913 add
    EPwm1Regs.CMPA.bit.CMPA = 0;                                                                              //250913 add
    EPwm1Regs.CMPB.bit.CMPB = 0;                                                                              //250913 add
    EPwm2Regs.CMPA.bit.CMPA = 0;                                                                              //250913 add
    EPwm2Regs.CMPB.bit.CMPB = 0;                                                                              //250913 add
    EPwm8Regs.CMPA.bit.CMPA = 0;                                                                              //250913 add
    EPwm8Regs.CMPB.bit.CMPB = 0;                                                                              //250913 add
    EPwm9Regs.CMPA.bit.CMPA = 0;                                                                              //250913 add
    EPwm9Regs.CMPB.bit.CMPB = 0;                                                                              //250913 add
    EDIS;                                                                                                     //250913 add
}

    void main(void)
{
//    Device_init();
    //
    // Step 1. Initialize System Control:
    // PLL, WatchDog, enable Peripheral Clocks
    // This example function is found in the F2837xS_SysCtrl.c file.
    //
    InitSysCtrl();
    InitCpuTimers();
    EALLOW;                                                                  //250315 add
//    CpuTimer1Regs.PRD.all = 19;                                            //250315 add  //250315 deactivate
//    CpuTimer1Regs.TCR.bit.TSS = 0;                                         //250315 add  //250315 deactivate
//    CpuTimer1Regs.TCR.bit.TIE = 1;                                         //250315 add  //250315 deactivate
    EDIS;
//
// Step 2. Initialize GPIO:
// This example function is found in the F2837xS_Gpio.c file and
// illustrates how to set the GPIO to it's default state.
//
    InitGpio();
    InitAdc();
    InitEPwmGpio();
//    InitEQep();                                                             //250403 deactivate
//    InitDMA();                                                              //250319 add  //250321 deactivate //250326 activate //250327 deactivate

    // <--Initialize the CAN controllers CANInit(사용하는  CAN port)
    //CANInit(CANB_BASE);
    // Initialize the CAN controllers-->

    // <-- Setup CAN to be clocked off the PLL output clock : default = 0
    //CANClkSourceSelect(CANB_BASE, 0);
    // Setup CAN to be clocked off the PLL output clock -->


    //<-- Set up the CAN bus bit rate to 500kHz for each module
    // This function sets up the CAN bus timing for a nominal configuration.
    // You can achieve more control over the CAN bus timing by using the
    // function CANBitTimingSet() instead of this one, if needed.
    // Additionally, consult the device data sheet for more information about
    // the CAN module clocking.

   // CANBitRateSet(CANB_BASE, 200000000, 500000);      // 500kHz CAN-Clock(CAN Max : 1MHz)
    // Set up the CAN bus bit rate to 500kHz for each module -->
    HWREG(CANB_BASE + CAN_O_CTL) = 0;                     // CAN 기능 설정 ( CAN_O_CTL -> 기본)
//          HWREG(CANB_BASE + CAN_O_CTL) |= CAN_CTL_TEST;                     // CAN 기능 설정 ( CAN_O_CTL -> 기본)


    // Step 3. Clear all interrupts and initialize PIE vector table:
    // Disable CPU interrupts
    //
    DINT;

    //
    // Initialize the PIE control registers to their default state.
    // The default state is all PIE interrupts disabled and flags
    // are cleared.
    // This function is found in the F2837xS_PieCtrl.c file.
    //
    InitPieCtrl();

    //
    // Disable CPU interrupts and clear all CPU interrupt flags:
    //
    IER = 0x0000; //Interrupt Enalbe Register
    IFR = 0x0000; // Interrupt Flag Register

    // Initialize the PIE vector table with pointers to the shell Interrupt
    // Service Routines (ISR).
    // This will populate the entire table, even if the interrupt
    // is not used in this example.  This is useful for debug purposes.
    // The shell ISR routines are found in F2837xS_DefaultIsr.c.
    // This function is found in F2837xS_PieVect.c.
    //
    InitPieVectTable();

    //
    // Map ISR functions
    //

    EALLOW;
    PieVectTable.ADCA1_INT = &ADCA1_ISR;                                      //250912 add      //250915 deactivate
    PieVectTable.ADCD1_INT = &ADCD1_ISR;                                      //250912 add      //250915 deactivate
    PieVectTable.ECAP2_INT = &ECAP2_ISR;                                     //250403 add //250412 deactivate //250417 activate //deactivate         //250426 activate //250912 deactivate
//    PieVectTable.TIMER1_INT = &TIMER1_ISR;                                              //250412 add        //250417 deactivate //250417 activate  //250426 deactivate
//    PieVectTable.TIMER2_INT = &TIMER2_ISR;
    // offset interrupt setting
//    PieVectTable.TIMER1_INT = &TIMER1_ISR;                                               //250315 add  //250315 deactivate
//    PieVectTable.EPWM1_INT = &offset;                                                    //250127 deactivate  //250301 activate //250315 deactivate    //250322 deactivate
//    PieVectTable.DMA_CH1_INT = &DMA_CH1_ISR;                                                 //250319 add  //250321 deactivate    //250326 activate    //250327 deactivate
//    PieVectTable.ADCD1_INT = &adc_isr;                                                 //250314 add       //250315 deactivate
    // <-- Fault interrupt setting
    InputXbarRegs.INPUT4SELECT = 53;    // 1. Input X bar 설정  // GPIO053                //250127 deactivate  //250301 activate
    XintRegs.XINT1CR.bit.POLARITY=0x2;  // 2. 외부 인터럽트 발생 조건 설정  //입력 신호의 하강엣지    //250127 deactivate  //250301 activate
    XintRegs.XINT1CR.bit.ENABLE=0x1;    // 3 외부 인터럽트 기능 활성화 설정                      //250127 deactivate  //250301 activate
    PieVectTable.XINT1_INT = &fault;                                                     //250127 deactivate  //250301 activate
    InputXbarRegs.INPUT8SELECT = 25;                                                                          //250405 add //250405 deactivate
    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;                                                            //250412 add
    // Fault interrupt setting -->
//    PieVectTable.EPWM7_TZ_INT = &EPWM7_TZ_ISR;                                          //250127 add     //250301 deactivate
//    PieVectTable.ADCD1_INT = &adc_isr;                                             //250301 add //250301 deactivate
//    PieCtrlRegs.PIEIER1.bit.INTx6 = 1;                                             //250301 add //250301 deactivate
    CpuSysRegs.PCLKCR2.bit.EPWM7 = 1;                                                                                  //250503 add
    EDIS;

//    //
//    // Configure the ADC and power it up
//    //
//        ConfigureADC();
//    //
//    // Configure the ePWM
//    //
//        ConfigureEPWM();
//
//    //
//    // Setup the ADC for ePWM triggered conversions on channel 0
//    //
//        SetupADCEpwm(0);

//        EALLOW;
//        GpioCtrlRegs.GPCMUX1.bit.GPIO73 = 3;  // GPIO73 = XCLKOUT
//        EDIS;

//    easyDSP_SCI_Init();                                                                              //250207 deactivate
//    EALLOW;                                                                                          //250207 deactivate

    EALLOW;                                                                                          //250411 add
    ECap2Regs.ECEINT.all = 0x0000;         // 인터럽트 disable                                        //250403 add
    ECap2Regs.ECCLR.all = 0xFFFF;          // clear flags                                          //250403 add
    ECap2Regs.ECCTL1.all=0x0000;                                                                   //250405 add
    ECap2Regs.ECEINT.bit.CEVT1 = 1;        // enable CEVT1 (rising)                                //250403 add
    ECap2Regs.ECEINT.bit.CEVT2 = 1;        // enable CEVT2 (falling)                               //250403 add
    ECap2Regs.ECCTL1.bit.CAP1POL = 0;      // rising edge                                          //250403 add
    ECap2Regs.ECCTL1.bit.CAP2POL = 1;      // falling edge                                         //250403 add
    ECap2Regs.ECCTL1.bit.CTRRST1 = 1;                                                              //250403 add
//    ECap2Regs.ECCTL1.bit.CTRRST2 = 1;                                                              //250403 add   //250409 deactivate
//    ECap2Regs.ECCTL1.bit.CTRRST2 = 0;                                                 //250409 add //250410 deactivate
    ECap2Regs.ECCTL1.bit.CTRRST2 = 1;                                                                //250410 add
//    ECap2Regs.ECCTL2.bit.SYNCI_EN = 1;                                                           //250410 add //250411 deactivate
    ECap2Regs.ECCTL2.bit.SYNCI_EN = 0;                                                                          //250411 add
    ECap2Regs.ECCTL1.bit.CAPLDEN = 1;                                                              //250403 add
//    ECap1Regs.ECCTL2.bit.STOP_WRAP = 1;                                                                           //250409 activate
    ECap2Regs.ECCTL2.bit.STOP_WRAP = 1;                                                                           //250409 add
//    ECap2Regs.ECCTL2.bit.STOP_WRAP = 0;    // wrap after 2 events                                  //250403 add   //250404 deactivate //250404 activate //250409 deactivate
    ECap2Regs.ECCTL2.bit.CONT_ONESHT = 0;  // continuous                                           //250403 add
    //    ECap2Regs.ECCTL2.bit.TSCTRSTOP = 1;                                                            //250403 add                               //250417 deactivate
    ECap2Regs.ECCTL2.bit.TSCTRSTOP = 0;                                                                                                         //250417 add
    ECap2Regs.ECCTL2.bit.CAP_APWM= 0;                                                                            //250404 add
    ECap2Regs.ECCTL1.bit.PRESCALE = 0;                                                                           //250404 add //250409 activate
//    ECap1Regs.ECCTL2.bit.SYNCO_SEL = 2;                                                                          //250404 add                   //250417 activate //250417 deactivate
    ECap2Regs.ECCTL2.bit.SYNCO_SEL = 2;                                      //250417 add
    //    ECap1Regs.ECCTL2.bit.SYNCI_EN = 0;                                                                           //250404 add
    ECap2Regs.ECCTL2.bit.REARM = 1;                                                                //250403 add   //250404 deactivate //250404 activate //250412 deactivate //250417 activate

    EDIS;                                                                                     //250411 add


//    IER |= M_INT4;                                                                                     //250403 add          //251104 deactivate
//    IER |= M_INT3;                     //Enable CPU interrupts for EPWM1_INT                         //250404 deactivate
    IER |= M_INT1;                     //Enable CPU interrupts for XINT1_INT
    IER |= M_INT4;                                                                                                             //251104 add
    IER |= M_INT2;                                                                                   //250127 add
    IER |= M_INT13;                                                                                  //250315 add
//    IER |= M_INT7;                                                                                   //250319 add  //250321 deactivate //250326 activate //250327 deactivate

//
// Enable global Interrupts and higher priority real-time debug events:
//

    EALLOW;                                                                                          //250126 add
//    PieCtrlRegs.PIEIER3.bit.INTx1 = 1;        //PIE 인터럽트 (EPWM1) Enable                            //250127 deactivate //250301 activate //250322 deactivate
//    PieCtrlRegs.PIEIER4.bit.INTx2 = 1;          // PIE ECAP2 enable                                               //250405 add               //251104 deactivate
    PieCtrlRegs.PIEIER1.bit.INTx4 = 1;        //PIE 인터럽트 (XINT1) Enable                            //250127 deactivate //250301 activate
    PieCtrlRegs.PIEIER1.bit.INTx1 = 1;         //ADCA1 Int Enable                                    //250912 add      //250914 deactivate //250914 activate       //250915 deactivate
    PieCtrlRegs.PIEIER1.bit.INTx6 = 1;         //ADCD1 Int Enable                                    //250914 add                                                        //250915 deactivate
    PieCtrlRegs.PIEIER4.bit.INTx2 = 1;          // PIE ECAP2 enable                                                                           //251104 add
    //    PieCtrlRegs.PIEIER1.bit.INTx6 = 1;                                                              //250314 add      //250403 deactivate
//    EDIS;                                                                                            //250207 deactivate
//    PieCtrlRegs.PIEIER7.bit.INTx1 = 1;                                                                 //250326 add //250327 deactivate
//    PieCtrlRegs.PIECTRL.bit.ENPIE = 1;                                                               //250319 add  //250321 deactivate
//    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;                                                               //250315 add    //250404 deactivate
//    PieCtrlRegs.PIEIER2.bit.INTx7 = 1;        //Enable EPWM7_TZ: Group 2,INT7                        //250126 add      //250301 deactivate
//    PieCtrlRegs.PIEIER3.bit.INTx7 = 1;        //PIE 인터럽트 (EPWM7) Enable                             //250126 add      //250301 deactivate
    EDIS;                                                                                            //250126 add
    EINT;                                     // Enable Global interrupt INTM
//      ERTM;                                     // Enable Global realtime interrupt DBGM
    ERTM;                                     // Enable Global realtime interrupt DBGM               //250126 add


    // <-- SPI for DAC Initiation
    //Must put SPI into reset before configuring it
//    SPI_disableModule(SPIA_BASE);                                                                    //250207 deactivate //250327 activate  //250405 deactivate //250405 activate //250409 deactivate
    // SPI configuration. Use a 10MHz SPICLK and 16-bit word size
    // SPI_PROT_POL1PHA0: Mode 2. Polarity 1, phase 0. Falling edge without delay.
//    SPI_setConfig(SPIA_BASE, DEVICE_LSPCLK_FREQ, SPI_PROT_POL1PHA0, SPI_MODE_MASTER, 10000000, 16);  //250207 deactivate //250327 activate  //250405 deactivate //250405 activate //250409 deactivate
//    SPI_enableLoopback(SPIA_BASE);                                                                   //250207 deactivate //250327 activate  //250405 deactivate //250405 activate //250409 deactivate
//    SPI_setEmulationMode(SPIA_BASE, SPI_EMULATION_FREE_RUN);                                         //250207 deactivate //250327 activate  //250405 deactivate //250405 activate //250409 deactivate
    // Configuration complete. Enable the module.
//    SPI_enableModule(SPIA_BASE);                                                                     //250207 deactivate //250327 activate  //250405 deactivate //250405 activate //250409 deactivate
    // SPI for DAC initiation -->

    // custom initiation
//    daInit();                                                                                        //250207 deactivate //250327 activate //250405 deactivate //250405 activate //250409 deactivate
   // myIinitCAN();

    // <-- User Code Start!!!
    InitFlags();
    InitVariables();
    // User Code End!!! -->

//    EPwm7Regs.ETCLR.bit.INT = 1;                                                                     //250126 add      //250301 deactivate
//    EPwm7Regs.ETCLR.bit.SOCA = 1;                                                                    //250126 add      //250301 deactivate

    WdRegs.SCSR.all = 0x00;                                                                          //250131 add
    WdRegs.WDCR.all = 0x2F;                                                                          //250131 add

/*    PI_Init(&pi,                                                                            //251103 add
                0.8f,  //kp                                                              //251103 add
                50.0f, //ki                                                              //251103 add
                0.001f,//Ts                                                              //251103 add
                0.0f,  //u_min                                                             //251103 add
                100.0f,//u_max                                                           //251103 add
                PI_AWU_BACK_CALC,//mode                                                  //251103 add
                0.2f);  //kaw                                                             //251103 add*/   //251103 deactivate

    while(1)
    {
       Dt = duty*0.01;
       AdcValues[0] = ((double)AdcResults[0] - 2020.)*2.32;
//       AdcValues[1] = (abs(2020-(double)AdcResults[1]))*0.189;
//       AdcValues[1] = (abs(2020-(double)AdcResults[1]))*0.1819;
//       AdcValues[1] = (abs(2020-(double)AdcResults[1]))*0.02686;
       AdcValues[1] = (abs(2020-(double)AdcResults[1]))*0.08058*2.6918;      //x3
       AdcValues[2] = ((double)AdcResults[2] - 2035.)*2;
//       AdcValues[3] = ((double)AdcResults[3]-2029)*0.1156;
       AdcValues[3] = ((double)AdcResults[3]-2029)*0.3468;           //x3
       Vdc_2 = AdcValues[0] *  AdcScale[0];                                                                                                       //250912 add
       Idc_2 = AdcValues[1] *  AdcScale[1];                                                                                                       //250912 add
       LPF(Vdc_2_LPF, 20, Vdc_2, CONTROL_PERIOD);                                                                                                 //250912 add
       LPF(Idc_2_LPF, 20, Idc_2, CONTROL_PERIOD);                                                                                                 //250912 add
       Vdc_2 = Vdc_2_LPF;                                                                                                                         //250912 add
       Idc_2 = Idc_2_LPF;                                                                                                                         //250912 add

       Vdc_1 = AdcValues[2] *  AdcScale[2]*1.21;                                                                                                       //250912 add
       Idc_1 = AdcValues[3] *  AdcScale[3];                                                                                                       //250912 add
       LPF(Vdc_1_LPF, 20, Vdc_1, CONTROL_PERIOD);                                                                                                 //250912 add
       LPF(Idc_1_LPF, 20, Idc_1, CONTROL_PERIOD);                                                                                                 //250912 add
       Vdc_1 = Vdc_1_LPF;                                                                                                                         //250912 add
       Idc_1 = Idc_1_LPF;                                                                                                                         //250912 add


/*       reference  = 100;                                               //251103 add
       Pmeasure = Vdc_2*Idc_2;                                          //251103 add
       PIvalue = PI_Update(&pi, reference, measure);                   //251103 add
       if(reference>measure) duty-=0.1;                                //251103 add
       else if(reference<=measure);                                    //251103 add*/              //251103 deactivate


       if(fabs(Vdc_1) > OV_Set_Vdc_1){ PWM_Trip();        faultnum = 1;}                                                                            //250914 add
       if(fabs(Idc_1) > OC_Set_Idc1) { PWM_Trip();        faultnum = 2;}                                                                           //250914 add
       if(fabs(Vdc_2) > OV_Set_Vdc_2){ PWM_Trip();        faultnum = 3;}                                                                            //250914 add
       if(fabs(Idc_2) > OC_Set_Idc2) { PWM_Trip();        faultnum = 4;}                                                                            //250914 add


       UpdateVariables();                                                                               //250207 deactivate //250301 activate
        //InitEPwmRegs();
        // <--Digital Input/Output
        dout(variable_dout);    // If variable_dout = 1010b : Turn on => Dout2, Dout4 / 7:0 -> Dout8 ... Dout1          //250207 deactivate //250301 activate
        variable_din = din();   // If variable_din = 1010b : Din4 = low, Din3 = high, Din2 = low, Din1 = high / 3:0 -> Din4 ... Din1  //250207 deactivate //250301 activate

/*        ECap2Regs.ECCTL2.bit.TSCTRSTOP = 1;

//        daOut();                                                                                               //250327 add //250405 deactivate //250405 activate //250409 deactivate
//        da[0] = EPwm7Regs.TBCTR;                    //250405 add                             //250409 deactivate
        // Digital Input/Output  -->
/*        if(DmaRegs.CH1.CONTROL.bit.RUN == 0){
            DmaRegs.CH1.CONTROL.bit.RUN = 1;
        }
        if(EPwm3Regs.ETFLG.bit.SOCA == 1){
            EPwm3Regs.ETCLR.bit.SOCA= 1;
        }                                        */  //250321 deactivate //250326 activate //250327 deactivate

/*        if(AdcdRegs.ADCINTFLG.bit.ADCINT1 == 1)
        {
            AdcdRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;
        }
        else{

        }                                                   */ //250326 deactivate //250327 activate //250327 deactivate

        GpioDataRegs.GPCCLEAR.bit.GPIO69 = 1;               // GPIO69 = GD1_OEn
        GpioDataRegs.GPCCLEAR.bit.GPIO70 = 1;               // GPIO70 = GD2_OEn
        GpioDataRegs.GPCCLEAR.bit.GPIO74 = 1;               // GPIO74 = GD3_OEn
        GpioDataRegs.GPCCLEAR.bit.GPIO75 = 1;               // GPIO75 = GD4_OEn

        if(DABDuty >= PWMMaxCountHalf)
        {
            DABDuty = PWMMaxCountHalf;
        }

       if(PWMMaxCountHalf > DABDutyPre){ DABDutyPre += DABDutyDiff;}
       else if(PWMMaxCountHalf < DABDutyPre){DABDutyPre = PWMMaxCountHalf;}
       else{DABDutyPre = DABDutyPre;}
        DABDuty = (int)DABDutyPre;

        if(DABDuty < PWMMaxCountHalf){
        EPwm1Regs.TBPHS.bit.TBPHS = 0;                        //TAB port-2 leg-A (Port-2 is phase reference)
        EPwm2Regs.TBPHS.bit.TBPHS = PWMMaxCount;                  //TAB port-2 leg-B


        EPwm1Regs.TBCTL.bit.PHSDIR = 0;                       //TAB port-2 leg-A  (Port-2 is phase reference)
        EPwm2Regs.TBCTL.bit.PHSDIR = 1;                       //TAB port-2 leg-B


        EPwm1Regs.CMPA.bit.CMPA = DABDuty;                        //TAB port-2 leg-A
        EPwm2Regs.CMPA.bit.CMPA = DABDuty;                        //TAB port-2 leg-B
        flag1 = 0;                                                                              //251022 add
        }
        else{
        EPwm1Regs.TBPHS.bit.TBPHS = 0;                        //TAB port-2 leg-A (Port-2 is phase reference)            //251029 deactivate  //251107 activate
        EPwm2Regs.TBPHS.bit.TBPHS = PWMMaxCount;                  //TAB port-2 leg-B                                    //251029 deactivate  //251107 activate

        EPwm1Regs.TBCTL.bit.PHSDIR = 0;                       //TAB port-2 leg-A  (Port-2 is phase reference)           //251029 deactivate  //251107 activate
        EPwm2Regs.TBCTL.bit.PHSDIR = 1;                       //TAB port-2 leg-B                                        //251029 deactivate  //251107 activate
        EPwm1Regs.CMPA.bit.CMPA = PWMMaxCountHalf;            //TAB port-2 leg-A                                        //251029 deactivate  //251107 activate
        EPwm2Regs.CMPA.bit.CMPA = PWMMaxCountHalf;            //TAB port-2 leg-B                                        //251029 deactivate  //251107 activate

//            LIMIT(PhSft, -50, 50);                                                                                        //251022 add //251022 deactivate
/*            PhaseShift = (PhSft / 180) * PWM_PRD;                                                                         //251022 add   for phase shift inverter
            if(PhaseShift>=0){                                                                                            //251022 add
                EPwm1Regs.TBCTL.bit.PHSDIR = TB_DOWN;                                                                     //251022 add
                EPwm2Regs.TBCTL.bit.PHSDIR = TB_UP;                                                                       //251022 add
                EPwm1Regs.TBPHS.bit.TBPHS = PhaseShift;                                                                   //251022 add
                EPwm2Regs.TBPHS.bit.TBPHS = PWM_PRD - PhaseShift;                                                         //251022 add
            }
            else{                                                                                                         //251022 add
                EPwm1Regs.TBCTL.bit.PHSDIR = TB_UP;                                                                       //251022 add
                EPwm2Regs.TBCTL.bit.PHSDIR = TB_DOWN;                                                                     //251022 add
                EPwm1Regs.TBPHS.bit.TBPHS = -PhaseShift;                                                                  //251022 add
                EPwm2Regs.TBPHS.bit.TBPHS = PWM_PRD + PhaseShift;                                                         //251022 add
            }
            EPwm1Regs.CMPA.bit.CMPA = PWMMaxCountHalf;            //TAB port-2 leg-A                                      //251022 add
            EPwm2Regs.CMPA.bit.CMPA = PWMMaxCountHalf;            //TAB port-2 leg-B                                      //251022 add
            flag1 = 1;                                                                           */   //251022 add for inverter phase shift //251107 deactivate
        }
//-----------------------------------------------------------250127------------------------------------------------------------------// add
/*        if(Faults.SW_Prot.all){
            faultManage();
            checkHWFault();
            //return;
        }

        //Status report
        if(Flags.Fault == 1) {                                  // Fault - Status = 3
            Status = 3;
        } else if((Flags.Fault == 0) && (EnableDABP == 0) && (EnableDABS == 0)) {     // Ready - Status = 1
            Status = 1;
        } else if((Flags.Fault == 0) && ((EnableDABP == 1) || (EnableDABS == 1))) {     // Run - Status = 2
            Status = 2;
        } else {                                                // Cable out - Status = 0
            Status = 0;
        }
        variable_dout = Status;
        dout(variable_dout);    // If variable_dout = 1010b : Turn on => Dout2, Dout4 / 7:0 -> Dout8 ... Dout1

        if(EnableDABP == 0 && EnableDABS == 0) //Mode 1
        {
                Pdc_2 = 0., Pdc_3 = 0., Pdc_4 = 0.;
                Pdc_2_Integ = 0., Pdc_3_Integ = 0., Pdc_4_Integ = 0.;
                Phase_12 = 0., Phase_13 = 0., Phase_14 = 0.;
                DABDuty =0., DABDutyPre =0.;
                EnableDABP = 0., EnableDABS = 0.;

                Pout_2_ref = 0, Pout_3_ref = 0, Pout_4_ref = 0;
                Idc_2_ref = 0, Idc_3_ref = 0, Idc_4_ref = 0;
                Vdc_2_cc_ref =0., Vdc_3_cc_ref=0., Vdc_4_cc_ref=0.;
                Pdc_2_ref = 0, Pdc_3_ref = 0, Pdc_4_ref = 0;
                Err_Idc_2 = 0, Err_Idc_3 = 0, Err_Idc_4 = 0;
                Err_Ecap_2=0., Err_Ecap_3=0., Err_Ecap_4=0.;
                Idc_2_Integ = 0, Idc_3_Integ = 0, Idc_4_Integ = 0;
         }

        else if(EnableDABP == 1 && EnableDABS == 0) // GD1: port-1 and port-2 one module, GD2: port-4, GD3: port-2 one module and port-3
        {
                Pdc_2 = 0., Pdc_3 = 0., Pdc_4 = 0.;
                Pdc_2_Integ = 0., Pdc_3_Integ = 0., Pdc_4_Integ = 0.;
                Phase_12 = 0., Phase_13 = 0., Phase_14 = 0.;
                DABDuty =0., DABDutyPre =0.;

                Pout_2_ref = 0, Pout_3_ref = 0, Pout_4_ref = 0;
                Idc_2_ref = 0, Idc_3_ref = 0, Idc_4_ref = 0;
                Vdc_2_cc_ref =0., Vdc_3_cc_ref=0., Vdc_4_cc_ref=0.;
                Pdc_2_ref = 0, Pdc_3_ref = 0, Pdc_4_ref = 0;
                Err_Idc_2 = 0, Err_Idc_3 = 0, Err_Idc_4 = 0;
                Err_Ecap_2=0., Err_Ecap_3=0., Err_Ecap_4=0.;
                Idc_2_Integ = 0, Idc_3_Integ = 0, Idc_4_Integ = 0;

                EnableDABS = 1;   // If EnableDABP is triggered, EableDABS is also triggered.

         }
        else if (EnableDABP == 0 && EnableDABS == 1) // Mode 3
        {
                Pdc_2 = 0., Pdc_3 = 0., Pdc_4 = 0.;
                Pdc_2_Integ = 0., Pdc_3_Integ = 0., Pdc_4_Integ = 0.;
                Phase_12 = 0., Phase_13 = 0., Phase_14 = 0.;
                DABDuty =0., DABDutyPre =0.;
                EnableDABP = 0., EnableDABS = 0.;

                Pout_2_ref = 0, Pout_3_ref = 0, Pout_4_ref = 0;
                Idc_2_ref = 0, Idc_3_ref = 0, Idc_4_ref = 0;
                Vdc_2_cc_ref =0., Vdc_3_cc_ref=0., Vdc_4_cc_ref=0.;
                Pdc_2_ref = 0, Pdc_3_ref = 0, Pdc_4_ref = 0;
                Err_Idc_2 = 0, Err_Idc_3 = 0, Err_Idc_4 = 0;
                Err_Ecap_2=0., Err_Ecap_3=0., Err_Ecap_4=0.;
                Idc_2_Integ = 0, Idc_3_Integ = 0, Idc_4_Integ = 0;
        }
        else  //Mode 4
        {*/                     //250417 deactivate

/*            if(Duty_ref_PLL_P<Theta2)                                       //250304 add
            {                                                               //250304 add
//                EPwm7Regs.CMPA.bit.CMPA = EPwm3Regs.TBPRD;                             //250322 deactivate
    //          EPwm7Regs.CMPA.bit.CMPA = EPwm7Regs.TBPRD;
                EPwm7Regs.AQCSFRC.bit.CSFA = AQ_SET;
                EPwm7Regs.AQCSFRC.bit.CSFB = AQ_SET;                                                                     //250326 add

            }                                                               //250304 add
            else{                                                           //250304 add
//                EPwm7Regs.CMPA.bit.CMPA = 0;
                EPwm7Regs.AQCSFRC.bit.CSFA = AQ_CLEAR;                                     //250322 add
                EPwm7Regs.AQCSFRC.bit.CSFB = AQ_CLEAR;                                     //250322 add
            }                                                               //250304 add*/                       //250403 deactivate

/*            if(DABDuty >= PWMMaxCountHalf)
            {
                DABDuty = PWMMaxCountHalf;
            }

           if(PWMMaxCountHalf > DABDutyPre){ DABDutyPre += DABDutyDiff;}
           else if(PWMMaxCountHalf < DABDutyPre){DABDutyPre = PWMMaxCountHalf;}
           else{DABDutyPre = DABDutyPre;}
            DABDuty = (int)DABDutyPre;
        }*/                                        //250417 deactivate

/*        if(DABDuty < PWMMaxCountHalf){
        EPwm1Regs.TBPHS.bit.TBPHS = 0;                        //TAB port-2 leg-A (Port-2 is phase reference)
        EPwm2Regs.TBPHS.bit.TBPHS = PWMMaxCount;                  //TAB port-2 leg-B

//        EPwm7Regs.TBPHS.bit.TBPHS = 0;                         //TAB port-1 leg-A               //250201 deactivate
//        EPwm8Regs.TBPHS.bit.TBPHS = PWMMaxCount;               //TAB port-1 leg-B               //250201 deactivate

        EPwm1Regs.TBCTL.bit.PHSDIR = 0;                       //TAB port-2 leg-A  (Port-2 is phase reference)
        EPwm2Regs.TBCTL.bit.PHSDIR = 1;                       //TAB port-2 leg-B

//        EPwm7Regs.TBCTL.bit.PHSDIR = 0;                       //TAB port-1 leg-A               //250201 deactivate
//        EPwm8Regs.TBCTL.bit.PHSDIR = 1;                       //TAB port-1 leg-B               //250201 deactivate

        EPwm1Regs.CMPA.bit.CMPA = DABDuty;                        //TAB port-2 leg-A
        EPwm2Regs.CMPA.bit.CMPA = DABDuty;                        //TAB port-2 leg-B

//        EPwm7Regs.CMPA.bit.CMPA = DABDuty;                        //TAB port-1 leg-A               //250201 deactivate
//        EPwm8Regs.CMPA.bit.CMPA = DABDuty;                        //TAB port-1 leg-B               //250201 deactivate
        }
        else{
        EPwm1Regs.TBPHS.bit.TBPHS = 0;                        //TAB port-2 leg-A (Port-2 is phase reference)
        EPwm2Regs.TBPHS.bit.TBPHS = PWMMaxCount;                  //TAB port-2 leg-B

//        EPwm7Regs.TBPHS.bit.TBPHS = TAB_Phase_21_legA;        //TAB port-1 leg-A               //250201 deactivate
//        EPwm8Regs.TBPHS.bit.TBPHS = TAB_Phase_21_legB;        //TAB port-1 leg-B               //250201 deactivate

        EPwm1Regs.TBCTL.bit.PHSDIR = 0;                       //TAB port-2 leg-A  (Port-2 is phase reference)
        EPwm2Regs.TBCTL.bit.PHSDIR = 1;                       //TAB port-2 leg-B

//        EPwm7Regs.TBCTL.bit.PHSDIR = TAB_Phase_21_legA_Dir;   //TAB port-1 leg-A               //250201 deactivate
//        EPwm8Regs.TBCTL.bit.PHSDIR = TAB_Phase_21_legB_Dir;   //TAB port-1 leg-B               //250201 deactivate

        EPwm1Regs.CMPA.bit.CMPA = PWMMaxCountHalf;            //TAB port-2 leg-A
        EPwm2Regs.CMPA.bit.CMPA = PWMMaxCountHalf;            //TAB port-2 leg-B

//        EPwm7Regs.CMPA.bit.CMPA = PWMMaxCountHalf;            //TAB port-1 leg-A               //250201 deactivate
//        EPwm8Regs.CMPA.bit.CMPA = PWMMaxCountHalf;            //TAB port-1 leg-B               //250201 deactivate
        }
        // User Code End!!! -->

        // <-- Enable Signal
        if(EnableDABP == 1) {   Flags.EnableGD1 = 1,    Flags.EnableGD2 = 1;
        } else {                Flags.EnableGD1 = 0,    Flags.EnableGD2 = 0;}



        if(EnableDABS == 1) {   Flags.EnableGD3 = 1;    Flags.EnableGD4 = 1;
        } else {                Flags.EnableGD3 = 0;    Flags.EnableGD4 = 0;}
        // Enable Signal -->


        ///// <-- Gating Signal Enable
    //    checkHWFault();
        if(Flags.Fault == 0 && Flags.Fault_Old == 0) {
            if(Flags.EnableStart == 1) {


                if(Flags.EnableGD1 == 1)    GpioDataRegs.GPCCLEAR.bit.GPIO69 = 1;               // GPIO69 = GD1_OEn
                else                GpioDataRegs.GPCSET.bit.GPIO69 = 1;
                if(Flags.EnableGD2 == 1)    GpioDataRegs.GPCCLEAR.bit.GPIO70 = 1;               // GPIO70 = GD2_OEn
                else                GpioDataRegs.GPCSET.bit.GPIO70 = 1;
                if(Flags.EnableGD3 == 1)    GpioDataRegs.GPCCLEAR.bit.GPIO74 = 1;               // GPIO74 = GD3_OEn
                else                GpioDataRegs.GPCSET.bit.GPIO74 = 1;
                if(Flags.EnableGD4 == 1)    GpioDataRegs.GPCCLEAR.bit.GPIO75 = 1;               // GPIO75 = GD4_OEn
                else                GpioDataRegs.GPCSET.bit.GPIO75 = 1;


            }
            if(Flags.EnableStart == 0) {
                Flags.EnableGD1 = 0, GpioDataRegs.GPCSET.bit.GPIO69 = 1;
                Flags.EnableGD2 = 0, GpioDataRegs.GPCSET.bit.GPIO70 = 1;
                Flags.EnableGD3 = 0, GpioDataRegs.GPCSET.bit.GPIO74 = 1;
                Flags.EnableGD4 = 0, GpioDataRegs.GPCSET.bit.GPIO75 = 1;
                Flags.EnableStart_Old = 0;

                EnableDABP = 0;
                EnableDABS = 0;

                Err_Ecap_13 = 0.;
                Err_Ecap_2 = 0.;
                Pdc_13_Integ = 0.;
                Pdc_2_Integ = 0.;
                Pdc_13 = 0.;
                Pdc_2 = 0.;
                DABPhase_A = 0;
                DABPhase_B = 0;
                DABPhase_C = 0;


            }
        } else {
            faultManage();
        }
        ///// Gating Signal Enable -->

        // <-- Fault reset check
        if(Flags.Reset == 1) {
            checkHWFault();
            if(Faults.HW_Prot.bit.nAll == 0x0) {
                Faults.SW_Prot.all = 0;
                Flags.Fault = 0;
                Flags.Fault_Old = 0;
                Flags.Reset = 0;
            }
        }*/                      //250417 deactivate
//-----------------------------------------------------------250127-------------------------------------------// add

    }
}

//
// End of File
//
