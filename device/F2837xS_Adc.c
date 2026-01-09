//###########################################################################
//
// FILE:   F2837xS_Adc.c
//
// TITLE:  F2837xS Adc Support Functions.
//
//###########################################################################
// $TI Release: F2837xS Support Library v210 $
// $Release Date: Tue Nov  1 15:35:23 CDT 2016 $
// $Copyright: Copyright (C) 2014-2016 Texas Instruments Incorporated -
//             http://www.ti.com/ ALL RIGHTS RESERVED $
//###########################################################################

//
// Included Files
//
#include "F2837xS_device.h"
#include "F2837xS_Examples.h"


//#pragma DATA_SECTION(AdcBuffer, "RAMLS4");                                                        //250320 add  //250321 deactivate //250327 deactivate
#define ADC_BUFFER_SIZE 64                                                                       //250319 add  //250321 deactivate //250326 activate
Uint32 AdcBuffer[ADC_BUFFER_SIZE];                                                                //250319 add  //250321 deactivate //250326 activate

void InitAdc(void)
{
    Uint16 acqps = 19, tempsensor_acqps = 139; //temperature sensor needs at least 700ns



    EALLOW;
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 0;                                                         //250201 add

    AdcaRegs.ADCCTL2.bit.PRESCALE = 6; //set ADCCLK divider to /4
    AdcSetMode(ADC_ADCA, ADC_BITRESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);
    AdcaRegs.ADCCTL1.bit.INTPULSEPOS = 1;   // Set pulse positions to late
    AdcaRegs.ADCCTL1.bit.ADCPWDNZ = 1;      //power up the ADC


    AdcbRegs.ADCCTL2.bit.PRESCALE = 6; //set ADCCLK divider to /4
    AdcSetMode(ADC_ADCB, ADC_BITRESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);
    AdcbRegs.ADCCTL1.bit.INTPULSEPOS = 1;   // Set pulse positions to late
    AdcbRegs.ADCCTL1.bit.ADCPWDNZ = 1;    //power up the ADC


    AdccRegs.ADCCTL2.bit.PRESCALE = 6; //set ADCCLK divider to /4
    AdcSetMode(ADC_ADCC, ADC_BITRESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);
    AdccRegs.ADCCTL1.bit.INTPULSEPOS = 1;   // Set pulse positions to late
    AdccRegs.ADCCTL1.bit.ADCPWDNZ = 1;    //power up the ADC


    AdcdRegs.ADCCTL2.bit.PRESCALE = 6; //set ADCCLK divider to /4
    AdcSetMode(ADC_ADCD, ADC_BITRESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);
    AdcdRegs.ADCCTL1.bit.INTPULSEPOS = 1;   // Set pulse positions to late
    AdcdRegs.ADCCTL1.bit.ADCPWDNZ = 1;    //power up the ADC

    AdcaRegs.ADCSOCPRICTL.bit.SOCPRIORITY = 0x0f;
    acqps = 14 + AdcaRegs.ADCCTL2.bit.RESOLUTION * 49; // 75ns for 12bit mode, 320ns for 16bit mode

    //Select the channels to convert and end of conversion flag
    AdcaRegs.ADCSOC0CTL.bit.CHSEL = 0;      //SOC0 will convert pin A0
    AdcaRegs.ADCSOC0CTL.bit.ACQPS = acqps;  //sample window is 14 SYSCLK cycles         //250207 deactivate   //250913 activate
//    AdcaRegs.ADCSOC0CTL.bit.ACQPS = 14;                                                //250207 add //250314 deactivate //250913 deactivate
//    AdcaRegs.ADCSOC0CTL.bit.ACQPS = 14;                                                   //250314 add

    AdcaRegs.ADCSOC0CTL.bit.TRIGSEL = 5;    //trigger on ePWM1 SOCA/C

    AdcaRegs.ADCSOC1CTL.bit.CHSEL = 1;      //SOC1 will convert pin A1
    AdcaRegs.ADCSOC1CTL.bit.ACQPS = acqps;  //sample window is 14 SYSCLK cycles           //250207 deactivate //250913 activate
//    AdcaRegs.ADCSOC1CTL.bit.ACQPS = 14;                                                    //250314 add     //250913 deactivate
    AdcaRegs.ADCSOC1CTL.bit.TRIGSEL = 5;    //trigger on ePWM1 SOCA/C

    AdcaRegs.ADCSOC2CTL.bit.CHSEL = 2;      //SOC2 will convert pin A2
    AdcaRegs.ADCSOC2CTL.bit.ACQPS = acqps;  //sample window is 14 SYSCLK cycles        //250207 deactivate   //250913 activate
//    AdcaRegs.ADCSOC2CTL.bit.ACQPS = 14;                                                   //250314 add     //250913 deactivate
    AdcaRegs.ADCSOC2CTL.bit.TRIGSEL = 5;    //trigger on ePWM1 SOCA/C

    AdcaRegs.ADCSOC3CTL.bit.CHSEL = 3;      //SOC3 will convert pin A3
    AdcaRegs.ADCSOC3CTL.bit.ACQPS = acqps;  //sample window is 14 SYSCLK cycles        //250207 deactivate   //250913 activate
//    AdcaRegs.ADCSOC3CTL.bit.ACQPS = 14;  //sample window is 14 SYSCLK cycles              //250207 add     //250913 deactivate
    AdcaRegs.ADCSOC3CTL.bit.TRIGSEL = 5;    //trigger on ePWM1 SOCA/C

    AdcaRegs.ADCSOC4CTL.bit.CHSEL = 4;      //SOC4 will convert pin A4
    AdcaRegs.ADCSOC4CTL.bit.ACQPS = acqps;  //sample window is 14 SYSCLK cycles        //250207 deactivate   //250913 activate
//    AdcaRegs.ADCSOC4CTL.bit.ACQPS = 14;                                                                    //250913 deactivate
    AdcaRegs.ADCSOC4CTL.bit.TRIGSEL = 5;    //trigger on ePWM1 SOCA/C

    AdcaRegs.ADCSOC5CTL.bit.CHSEL = 5;      //SOC5 will convert pin A5
    AdcaRegs.ADCSOC5CTL.bit.ACQPS = acqps;  //sample window is 14 SYSCLK cycles        //250207 deactivate   //250913 activate
//    AdcaRegs.ADCSOC5CTL.bit.ACQPS = 14;                                                  //250207 add      //250913 deactivate
    AdcaRegs.ADCSOC5CTL.bit.TRIGSEL = 5;    //trigger on ePWM1 SOCA/C

    AdcaRegs.ADCSOC6CTL.bit.CHSEL = 13;      //SOC6 will convert pin TEMP SENSOR
    AdcaRegs.ADCSOC6CTL.bit.ACQPS = tempsensor_acqps;  //sample window is 100 SYSCLK cycles                  //250913 activate
//    AdcaRegs.ADCSOC6CTL.bit.ACQPS = 14;                                                  //250314 add      //250913 deactivate
    AdcaRegs.ADCSOC6CTL.bit.TRIGSEL = 5;    //trigger on ePWM1 SOCA/C

        AdcaRegs.ADCINTSEL1N2.bit.INT1SEL = 0; //end of SOC0 will set INT1 flag         //250912 activate
        AdcaRegs.ADCINTSEL1N2.bit.INT1E = 1;   //enable INT1 flag                       //250912 activate
        AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //make sure INT1 flag is cleared         //250912 activate
//    AdcaRegs.ADCINTSEL1N2.bit.INT1SEL = 3;                                                //250201 add //250320 deactivate
//    AdcaRegs.ADCINTSEL1N2.bit.INT1E = 1;                                                  //250201 add //250320 deactivate

    // Adc B module :
    //Select the channels to convert and end of conversion flag
    AdcbRegs.ADCSOC0CTL.bit.CHSEL = 0;      //SOC0 will convert pin B0
//    AdcbRegs.ADCSOC0CTL.bit.ACQPS = acqps;  //sample window is 14 SYSCLK cycles        //250207 deactivate
    AdcbRegs.ADCSOC0CTL.bit.ACQPS = 14;                                                //250207 add
    AdcbRegs.ADCSOC0CTL.bit.TRIGSEL = 5;    //trigger on ePWM1 SOCA/C             //250126 deactivate   //250127 activate
//    AdcbRegs.ADCSOC0CTL.bit.TRIGSEL = 9;    //trigger on ePWM3 SOCA/C            //250126 add   //250127 deactivate

    AdcbRegs.ADCSOC1CTL.bit.CHSEL = 1;      //SOC1 will convert pin B1
//    AdcbRegs.ADCSOC1CTL.bit.ACQPS = acqps;  //sample window is 14 SYSCLK cycles        //250207 deactivate
    AdcbRegs.ADCSOC1CTL.bit.ACQPS = 14;                                                //250207 add
    AdcbRegs.ADCSOC1CTL.bit.TRIGSEL = 5;    //trigger on ePWM1 SOCA/C               //250126 deactivate  //250127 activate
//    AdcbRegs.ADCSOC1CTL.bit.TRIGSEL = 9;    //trigger on ePWM3 SOCA/C            //250126 add   //250127 deactivate

    AdcbRegs.ADCSOC2CTL.bit.CHSEL = 2;      //SOC2 will convert pin B2
//    AdcbRegs.ADCSOC2CTL.bit.ACQPS = acqps;  //sample window is 14 SYSCLK cycles        //250207 deactivate
    AdcbRegs.ADCSOC2CTL.bit.ACQPS = 14;                                                //250207 add
    AdcbRegs.ADCSOC2CTL.bit.TRIGSEL = 5;    //trigger on ePWM1 SOCA/C             //250126 deactivate   //250127 activate
//    AdcbRegs.ADCSOC2CTL.bit.TRIGSEL = 9;    //trigger on ePWM3 SOCA/C            //250126 add   //250127 deactivate

    AdcbRegs.ADCSOC3CTL.bit.CHSEL = 3;      //SOC3 will convert pin B3
//    AdcbRegs.ADCSOC3CTL.bit.ACQPS = acqps;  //sample window is 14 SYSCLK cycles        //250207 deactivate
    AdcbRegs.ADCSOC3CTL.bit.ACQPS = 14;                                                //250207 add
    AdcbRegs.ADCSOC3CTL.bit.TRIGSEL = 5;    //trigger on ePWM1 SOCA/C            //250126 deactivate   //250127 activate
//    AdcbRegs.ADCSOC3CTL.bit.TRIGSEL = 9;    //trigger on ePWM3 SOCA/C            //250126 add   //250127 deactivate

    AdcbRegs.ADCSOC4CTL.bit.CHSEL = 4;      //SOC4 will convert pin B4
//    AdcbRegs.ADCSOC4CTL.bit.ACQPS = acqps;  //sample window is 14 SYSCLK cycles        //250207 deactivate
    AdcbRegs.ADCSOC4CTL.bit.ACQPS = 14;                                                //250207 add
    AdcbRegs.ADCSOC4CTL.bit.TRIGSEL = 5;    //trigger on ePWM1 SOCA/C            //250126 deactivate   //250127 activate
//    AdcbRegs.ADCSOC4CTL.bit.TRIGSEL = 9;    //trigger on ePWM3 SOCA/C            //250126 add   //250127 deactivate

    AdcbRegs.ADCSOC5CTL.bit.CHSEL = 5;      //SOC5 will convert pin B5
//    AdcbRegs.ADCSOC5CTL.bit.ACQPS = acqps;  //sample window is 14 SYSCLK cycles           //250207 deactivate
    AdcbRegs.ADCSOC5CTL.bit.ACQPS = 14;                                                //250207 add
    AdcbRegs.ADCSOC5CTL.bit.TRIGSEL = 5;    //trigger on ePWM1 SOCA/C            //250126 deactivate   //250127 activate
//    AdcbRegs.ADCSOC5CTL.bit.TRIGSEL = 9;    //trigger on ePWM3 SOCA/C            //250126 add   //250127 deactivate

    // Adc C module :
    // Select the channels to convert and end of conversion flag
    AdccRegs.ADCSOC0CTL.bit.CHSEL = 14;      //SOC0 will convert pin 14
//    AdccRegs.ADCSOC0CTL.bit.ACQPS = acqps;  //sample window is 14 SYSCLK cycles        //250207 deactivate
    AdccRegs.ADCSOC0CTL.bit.ACQPS = 14;                                                //250207 add
    AdccRegs.ADCSOC0CTL.bit.TRIGSEL = 5;    //trigger on ePWM1 SOCA/C

    AdccRegs.ADCSOC1CTL.bit.CHSEL = 15;      //SOC1 will convert pin 15
//    AdccRegs.ADCSOC1CTL.bit.ACQPS = acqps;  //sample window is 14 SYSCLK cycles      //250207 deactivate
    AdccRegs.ADCSOC1CTL.bit.ACQPS = 14;                                                //250207 add
    AdccRegs.ADCSOC1CTL.bit.TRIGSEL = 5;    //trigger on ePWM1 SOCA/C

    AdccRegs.ADCSOC2CTL.bit.CHSEL = 2;      //SOC2 will convert pin C2
//    AdccRegs.ADCSOC2CTL.bit.ACQPS = acqps;  //sample window is 14 SYSCLK cycles        //250207 deactivate
    AdccRegs.ADCSOC2CTL.bit.ACQPS = 14;                                                //250207 add
    AdccRegs.ADCSOC2CTL.bit.TRIGSEL = 5;    //trigger on ePWM1 SOCA/C

    AdccRegs.ADCSOC3CTL.bit.CHSEL = 3;      //SOC3 will convert pin C3
//    AdccRegs.ADCSOC3CTL.bit.ACQPS = acqps;  //sample window is 14 SYSCLK cycles      //250207 deactivate
    AdccRegs.ADCSOC3CTL.bit.ACQPS = 14;                                                //250207 add
    AdccRegs.ADCSOC3CTL.bit.TRIGSEL = 5;    //trigger on ePWM1 SOCA/C

    AdccRegs.ADCSOC4CTL.bit.CHSEL = 4;      //SOC4 will convert pin C4
//    AdccRegs.ADCSOC4CTL.bit.ACQPS = acqps;  //sample window is 14 SYSCLK cycles        //250207 deactivate
    AdccRegs.ADCSOC4CTL.bit.ACQPS = 14;                                                //250207 add
    AdccRegs.ADCSOC4CTL.bit.TRIGSEL = 5;    //trigger on ePWM1 SOCA/C

    AdccRegs.ADCSOC5CTL.bit.CHSEL = 5;      //SOC5 will convert pin C5
//    AdccRegs.ADCSOC5CTL.bit.ACQPS = acqps;  //sample window is 14 SYSCLK cycles      //250207 deactivate
    AdccRegs.ADCSOC5CTL.bit.ACQPS = 14;                                                //250207 add
    AdccRegs.ADCSOC5CTL.bit.TRIGSEL = 5;    //trigger on ePWM1 SOCA/C

    // Adc D module :
    // Select the channels to convert and end of conversion flag
    AdcdRegs.ADCSOC0CTL.bit.CHSEL = 0;      //SOC0 will convert pin D0
//    AdcdRegs.ADCSOC0CTL.bit.ACQPS = acqps*2;  //sample window is 14 SYSCLK cycles //250126 deactivate //250301 activate
//   AdcdRegs.ADCSOC0CTL.bit.ACQPS = 14;                                         //250314 add      //250321 deactivate
//   AdcdRegs.ADCSOC0CTL.bit.ACQPS = 30;                                                              //250322 add //250914 deactivate
    AdcdRegs.ADCSOC0CTL.bit.ACQPS = acqps;                                                                         //250914 add
    //    AdcdRegs.ADCSOC0CTL.bit.TRIGSEL = 5;    //trigger on ePWM1 SOCA/C          //250126 deactivate //250301 activate //250315 deactivate
//    AdcdRegs.ADCSOC0CTL.bit.TRIGSEL = 2;                                              //250315 add //250319 deactivate(CPU1 Timer)
    AdcdRegs.ADCSOC0CTL.bit.TRIGSEL = 9;                                              //250319 add
//    AdcdRegs.ADCSOC0CTL.bit.ACQPS = 14;                                          //250126 add //250301 deactivate
//    AdcdRegs.ADCSOC0CTL.bit.TRIGSEL = 11;    //trigger on ePWM7 SOCA/C           //250126 add //250301 deactivate
    AdcdRegs.ADCSOC0CTL.bit.TRIGSEL = 5;                                                                           //250914 add

    AdcdRegs.ADCSOC1CTL.bit.CHSEL = 1;      //SOC1 will convert pin D1         //250131 deactivate //250301 activate
//    AdcdRegs.ADCSOC1CTL.bit.ACQPS = acqps;  //sample window is 14 SYSCLK cycles//250126 deactivate //250301 activate
//    AdcdRegs.ADCSOC1CTL.bit.ACQPS = 14;                                         //250314 add      //250321 deactivate
//    AdcdRegs.ADCSOC1CTL.bit.ACQPS = 30;                                                             //250322 add//250914 deactivate
    AdcdRegs.ADCSOC1CTL.bit.ACQPS = acqps;                                                                         //250914 add
    AdcdRegs.ADCSOC1CTL.bit.TRIGSEL = 5;    //trigger on ePWM1 SOCA/C          //250126 deactivate //250301 activate
//    AdcdRegs.ADCSOC1CTL.bit.ACQPS = 14;                                          //250126 add //250131 deactivate
//    AdcdRegs.ADCSOC1CTL.bit.TRIGSEL = 11;    //trigger on ePWM7 SOCA/C           //250126 add //250131 deactivate

    AdcdRegs.ADCSOC2CTL.bit.CHSEL = 2;      //SOC2 will convert pin D2         //250131 deactivate //250301 activate
    AdcdRegs.ADCSOC2CTL.bit.ACQPS = acqps;  //sample window is 14 SYSCLK cycles//250126 deactivate //250301 activate                   //250914 activate
//    AdcdRegs.ADCSOC2CTL.bit.ACQPS = 14;                                         //250314 add //250321 deactivate //250322 deactivate
//    AdcdRegs.ADCSOC2CTL.bit.ACQPS = 30;                                         //250322 add                                         //250914 deactivate
    AdcdRegs.ADCSOC2CTL.bit.TRIGSEL = 5;    //trigger on ePWM1 SOCA/C          //250126 deactivate //250301 activate
//    AdcdRegs.ADCSOC2CTL.bit.ACQPS = 14;  //sample window is 14 SYSCLK cycles     //250126 add //250131 deactivate
//    AdcdRegs.ADCSOC2CTL.bit.TRIGSEL = 11;    //trigger on ePWM7 SOCA/C           //250126 add //250131 deactivate

    AdcdRegs.ADCSOC3CTL.bit.CHSEL = 3;      //SOC3 will convert pin D3         //250131 deactivate //250301 activate
    AdcdRegs.ADCSOC3CTL.bit.ACQPS = acqps;  //sample window is 14 SYSCLK cycles//250126 deactivate //250301 activate      //250914 activate
//    AdcdRegs.ADCSOC3CTL.bit.ACQPS = 14;                                         //250314 add        //250321 deactivate //250322 deactivate
//    AdcdRegs.ADCSOC3CTL.bit.ACQPS = 30;                                                            //250322 add         //250914 deactivate
    AdcdRegs.ADCSOC3CTL.bit.TRIGSEL = 5;    //trigger on ePWM1 SOCA/C          //250126 deactivate //250301 activate
//    AdcdRegs.ADCSOC3CTL.bit.ACQPS = 14;  //sample window is 14 SYSCLK cycles     //250126 add //250131 deactivate
//    AdcdRegs.ADCSOC3CTL.bit.TRIGSEL = 11;    //trigger on ePWM7 SOCA/C           //250126 add //250131 deactivate

    AdcdRegs.ADCSOC4CTL.bit.CHSEL = 4;      //SOC4 will convert pin D4         //250131 deactivate //250301 activate
    AdcdRegs.ADCSOC4CTL.bit.ACQPS = acqps;  //sample window is 14 SYSCLK cycles//250126 deactivate //250301 activate             //250914 activate
//    AdcdRegs.ADCSOC4CTL.bit.ACQPS = 14;                                         //250314 add        //250321 deactivate //250322 deactivate
//    AdcdRegs.ADCSOC4CTL.bit.ACQPS = 30;                                         //250322 add                                   //250914 deactivate
    AdcdRegs.ADCSOC4CTL.bit.TRIGSEL = 5;    //trigger on ePWM1 SOCA/C          //250126 deactivate //250301 activate
//    AdcdRegs.ADCSOC4CTL.bit.ACQPS = 14;  //sample window is 14 SYSCLK cycles     //250126 add //250131 deactivate
//    AdcdRegs.ADCSOC4CTL.bit.TRIGSEL = 11;    //trigger on ePWM7 SOCA/C           //250126 add //250131 deactivate

    AdcdRegs.ADCSOC5CTL.bit.CHSEL = 5;      //SOC5 will convert pin D5         //250131 deactivate //250301 activate
    AdcdRegs.ADCSOC5CTL.bit.ACQPS = acqps;  //sample window is 14 SYSCLK cycles//250126 deactivate //250301 activate     //250914 activate
//    AdcdRegs.ADCSOC5CTL.bit.ACQPS = 14;                                         //250314 add        //250321 deactivate //250322 deactivate
//    AdcdRegs.ADCSOC5CTL.bit.ACQPS = 30;                                         //250322 add                           //250914 deactivate
    AdcdRegs.ADCSOC5CTL.bit.TRIGSEL = 5;    //trigger on ePWM1 SOCA/C          //250126 deactivate //250301 activate
//    AdcdRegs.ADCSOC5CTL.bit.ACQPS = 14;  //sample window is 14 SYSCLK cycles     //250126 add //250131 deactivate
//    AdcdRegs.ADCSOC5CTL.bit.TRIGSEL = 11;    //trigger on ePWM7 SOCA/C           //250126 add //250131 deactivate

//    AdcdRegs.ADCCTL1.bit.INTPULSEPOS= 1;                                                  //250314 add //250314 deactivate   //250327 activate   //250404 deactivate
//    AdcdRegs.ADCINTSEL1N2.bit.INT1E = 1;                                                    //250314 add //250314 deactivate //250319 activate //250319 deactivate //250321 activate //250326 deactivate //250327 activate //250404 deactivate
//    AdcdRegs.ADCINTSEL1N2.bit.INT1CONT = 0;                                               //250314 add //250314 deactivate   //250321 activate   //250404 deactivate
//    AdcdRegs.ADCINTSEL1N2.bit.INT1SEL = 0;                                                  //250314 add //250314 deactivate //250319 activate   //250404 deactivate
//    AdcdRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;                                                  //250315 add //250314 deactivate //250319 activate   //250404 deactivate
//    AdcdRegs.ADCINTSEL1N2.bit.INT1E = 0;                                                    //250319 add   //250321 deactivate //250326 activate  //250327 deactivate
    AdcdRegs.ADCINTSEL1N2.bit.INT1SEL = 0; //end of SOC0 will set INT1 flag         //250914 add
    AdcdRegs.ADCINTSEL1N2.bit.INT1E = 1;   //enable INT1 flag                       //250914 add
    AdcdRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //make sure INT1 flag is cleared         //250914 add
    EDIS;

    DELAY_US(1000);    //delay for 1ms to allow ADC time to power up

/*    EALLOW;                                                                                                       //250126 add
    DacaRegs.DACCTL.bit.DACREFSEL = 1;  // Comp의 전원설정 : 3.3V로 설정                                               //250126 add
    DacaRegs.DACCTL.bit.LOADMODE  = 0;  // load on next SYSCLK                                                    //250126 add
    DacaRegs.DACCTL.bit.SYNCSEL = 6;    // PWMSYNC7                                                               //250126 add
//    DacaRegs.DACCTL.bit.SYNCSEL = 0;    // PWMSYNC7                                                               //250127 add  //250131 deactivate
    DacaRegs.DACOUTEN.bit.DACOUTEN=1;                                                                             //250126 add
    DacbRegs.DACCTL.bit.DACREFSEL = 1;  // VDAC ref                                                               //250126 add
    DacbRegs.DACCTL.bit.LOADMODE  = 0;  // load on next SYSCLK                                                    //250126 add
    DacbRegs.DACCTL.bit.SYNCSEL = 6;    // PWMSYNC7                                                               //250126 add
//    DacaRegs.DACCTL.bit.SYNCSEL = 1;    // PWMSYNC7                                                               //250127 add  //250131 deactivate
    DacbRegs.DACOUTEN.bit.DACOUTEN=1;                                                                             //250126 add
    DaccRegs.DACCTL.bit.DACREFSEL = 1;  // Comp의 전원설정 : 3.3V로 설정                                               //250126 add
    DaccRegs.DACCTL.bit.LOADMODE  = 0;  // load on next SYSCLK                                                    //250126 add
    DaccRegs.DACCTL.bit.SYNCSEL = 2;    // PWMSYNC3                                                               //250126 add
    DaccRegs.DACOUTEN.bit.DACOUTEN=1;                                                                             //250126 add
    Cmpss7Regs.COMPDACCTL.bit.SWLOADSEL = 0;    // 동기를 SYSCLK으로 할지 PWMCLK로 할지 설정 : SYSCLK로 하도록 설정          //250126 add
    Cmpss7Regs.COMPDACCTL.bit.DACSOURCE = 0;    // Comp의 (-)치(비교치)를 설정하는 Reg : DACVAL에 기록한 값이 제한치가 되도록 설정//250126 add
    Cmpss7Regs.COMPCTL.bit.COMPDACE = 1;        // 내부 Comp (-)단용 DAC Converter Enable                           //250126 add
    Cmpss7Regs.COMPCTL.bit.COMPLSOURCE = 1; // Select external pin for comparator input                           //250126 add
    Cmpss7Regs.COMPCTL.bit.COMPLINV = 0;    // Do not invert the comparator output                                //250126 add
    Cmpss7Regs.COMPSTS.bit.COMPLSTS = 1;    // Clear status                                                        //250126 add
    Cmpss7Regs.COMPCTL.bit.CTRIPLSEL = 0;   // Select ASYNCL for trip low signal                                  //250126 add
    Cmpss7Regs.COMPCTL.bit.CTRIPOUTHSEL = 0;                                                                     //250126 add
    Cmpss7Regs.DACHVALS.bit.DACVAL = 2150;                                                                        //250126 add
    Cmpss7Regs.COMPCTL.bit.COMPLSOURCE = 0;     // Comp의 (-)치(비교치)를 설정하는 Reg : DACVAL에 기록한 값이 제한치가 되도록 설정//250126 add
    Cmpss7Regs.COMPCTL.bit.COMPLINV = 1;        // Comp출력의 반전관련 Reg : 반전으로 설정(제한치보다 낮으면 Protection 되도록)  //250126 add
    Cmpss7Regs.COMPCTL.bit.CTRIPLSEL = 0;       // 동기화 관련 Reg : 비동기로 설정 Epwm X-Bar                            //250126 add
    Cmpss7Regs.COMPCTL.bit.CTRIPOUTLSEL = 0;    // 동기화 관련 Reg : 비동기로 설정 Output X-Bar                         //250126 add
    Cmpss7Regs.DACLVALS.bit.DACVAL = 0;                                                                          //250126 add
    Cmpss7Regs.CTRIPHFILCLKCTL.bit.CLKPRESCALE = 2; // Set clock prescaler for filter                            //250126 add
    Cmpss7Regs.CTRIPHFILCTL.bit.THRESH = 19;        // Set threshold                                             //250126 add
    Cmpss7Regs.CTRIPHFILCTL.bit.SAMPWIN = 20;       // Set sample window                                         //250126 add
    Cmpss7Regs.CTRIPHFILCTL.bit.FILINIT = 1;        // Initialize filter                                         //250126 add
    EDIS;                                                                                                        //250126 add*/   //250301 deactivate

}


//
// AdcSetMode - Set the resolution and signalmode for a given ADC. This will
//              ensure that the correct trim is loaded.
//
void AdcSetMode(Uint16 adc, Uint16 resolution, Uint16 signalmode)
{
    Uint16 adcOffsetTrimOTPIndex; //index into OTP table of ADC offset trims
    Uint16 adcOffsetTrim;         //temporary ADC offset trim

    //
    //re-populate INL trim
    //
    CalAdcINL(adc);

    if(0xFFFF != *((Uint16*)GetAdcOffsetTrimOTP))
    {
        //
        //offset trim function is programmed into OTP, so call it
        //

        //
        //calculate the index into OTP table of offset trims and call
        //function to return the correct offset trim
        //
        adcOffsetTrimOTPIndex = 4*adc + 2*resolution + 1*signalmode;
        adcOffsetTrim = (*GetAdcOffsetTrimOTP)(adcOffsetTrimOTPIndex);
    }
    else
    {
        //
        //offset trim function is not populated, so set offset trim to 0
        //
        adcOffsetTrim = 0;
    }

    //
    //Apply the resolution and signalmode to the specified ADC.
    //Also apply the offset trim and, if needed, linearity trim correction.
    //
    switch(adc)
    {
        case ADC_ADCA:
            AdcaRegs.ADCCTL2.bit.RESOLUTION = resolution;
            AdcaRegs.ADCCTL2.bit.SIGNALMODE = signalmode;
            AdcaRegs.ADCOFFTRIM.all = adcOffsetTrim;
            if(ADC_BITRESOLUTION_12BIT == resolution)
            {
                //
                //12-bit linearity trim workaround
                //
                AdcaRegs.ADCINLTRIM1 &= 0xFFFF0000;
                AdcaRegs.ADCINLTRIM2 &= 0xFFFF0000;
                AdcaRegs.ADCINLTRIM4 &= 0xFFFF0000;
                AdcaRegs.ADCINLTRIM5 &= 0xFFFF0000;
            }
        break;
        case ADC_ADCB:
            AdcbRegs.ADCCTL2.bit.RESOLUTION = resolution;
            AdcbRegs.ADCCTL2.bit.SIGNALMODE = signalmode;
            AdcbRegs.ADCOFFTRIM.all = adcOffsetTrim;
            if(ADC_BITRESOLUTION_12BIT == resolution)
            {
                //
                //12-bit linearity trim workaround
                //
                AdcbRegs.ADCINLTRIM1 &= 0xFFFF0000;
                AdcbRegs.ADCINLTRIM2 &= 0xFFFF0000;
                AdcbRegs.ADCINLTRIM4 &= 0xFFFF0000;
                AdcbRegs.ADCINLTRIM5 &= 0xFFFF0000;
            }
        break;
        case ADC_ADCC:
            AdccRegs.ADCCTL2.bit.RESOLUTION = resolution;
            AdccRegs.ADCCTL2.bit.SIGNALMODE = signalmode;
            AdccRegs.ADCOFFTRIM.all = adcOffsetTrim;
            if(ADC_BITRESOLUTION_12BIT == resolution)
            {
                //
                //12-bit linearity trim workaround
                //
                AdccRegs.ADCINLTRIM1 &= 0xFFFF0000;
                AdccRegs.ADCINLTRIM2 &= 0xFFFF0000;
                AdccRegs.ADCINLTRIM4 &= 0xFFFF0000;
                AdccRegs.ADCINLTRIM5 &= 0xFFFF0000;
            }
        break;
        case ADC_ADCD:
            AdcdRegs.ADCCTL2.bit.RESOLUTION = resolution;
            AdcdRegs.ADCCTL2.bit.SIGNALMODE = signalmode;
            AdcdRegs.ADCOFFTRIM.all = adcOffsetTrim;
            if(ADC_BITRESOLUTION_12BIT == resolution)
            {
                //
                //12-bit linearity trim workaround
                //
                AdcdRegs.ADCINLTRIM1 &= 0xFFFF0000;
                AdcdRegs.ADCINLTRIM2 &= 0xFFFF0000;
                AdcdRegs.ADCINLTRIM4 &= 0xFFFF0000;
                AdcdRegs.ADCINLTRIM5 &= 0xFFFF0000;
            }
        break;
    }
}

//
// CalAdcINL - Loads INL trim values from OTP into the trim registers of the
//             specified ADC. Use only as part of AdcSetMode function, since
//             linearity trim correction is needed for some modes.
//
void CalAdcINL(Uint16 adc)
{
    switch(adc)
    {
        case ADC_ADCA:
            if(0xFFFF != *((Uint16*)CalAdcaINL))
            {
                //
                //trim function is programmed into OTP, so call it
                //
                (*CalAdcaINL)();
            }
            else
            {
                //
                //do nothing, no INL trim function populated
                //
            }
            break;
        case ADC_ADCB:
            if(0xFFFF != *((Uint16*)CalAdcbINL))
            {
                //
                //trim function is programmed into OTP, so call it
                //
                (*CalAdcbINL)();
            }
            else
            {
                //
                //do nothing, no INL trim function populated
                //
            }
            break;
        case ADC_ADCC:
            if(0xFFFF != *((Uint16*)CalAdccINL))
            {
                //
                //trim function is programmed into OTP, so call it
                //
                (*CalAdccINL)();
            }
            else
            {
                //
                //do nothing, no INL trim function populated
                //
            }
            break;
        case ADC_ADCD:
            if(0xFFFF != *((Uint16*)CalAdcdINL))
            {
                //
                //trim function is programmed into OTP, so call it
                //
                (*CalAdcdINL)();
            }
            else
            {
                //
                //do nothing, no INL trim function populated
                //
            }
            break;
    }
}

//
// End of file
//
