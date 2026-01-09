////////////////////////////////////
//// cc.c                       ////
//// Created on: 2020. 2. 5.    ////
//// Author: Dong Yeup Han      ////
////////////////////////////////////
#include <math.h>
#include "F28x_Project.h"
#include "device.h"
#include "variables.h"
#include "cc.h"
#include "dac.h"
#include "fault.h"
#include "filter.h"
#include "variables.h"

double ia= 0, ib = 0;                                        //250228 add

#pragma CODE_SECTION(cc, "ramfuncs");

//double f_peak = 85000.0;                                      //250304 add      //250321 deactivate
//double bandwidth = 17000.0;                                   //250304 add      //250321 deactivate
//double sampling_rate = 200000.0;                              //250304 add      //250321 deactivate
//double Duty_ref_PLL_P2 = 0, Duty_ref_PLL_N2 = 0;              //250304 add      //250321 deactivate
//double Duty_ref_PLL_P = 2, Duty_ref_PLL_N = -2;               //250304 add      //250321 deactivate
//double Wo, Q, H, H_0;                                         //250304 add      //250321 deactivate
//double b0, b1, b2, a1, a2;                                    //250304 add      //250321 deactivate
//double prev_input_1 = 0.0, prev_input_2 = 0.0;                //250304 add      //250321 deactivate
//double prev_output_1 = 0.0, prev_output_2 = 0.0;              //250304 add      //250321 deactivate
/*
double apply_bandpass_filter(double input_signal) {           //250304 add
    Wo = 2.0 * PI * f_peak / sampling_rate;                   //250304 add
    Q = f_peak / bandwidth;                                   //250304 add
    H = 1.0 / Wo;                                            //250304 add
    H_0 = H / Q;                                             //250304 add

    double alpha = sin(Wo) / (2.0 * Q);                      //250304 add

    b0 = H_0 * Wo * Wo;                                     //250304 add
    b1 = 0.0;                                                //250304 add
    b2 = -H_0 * Wo * Wo;                                     //250304 add
    double a0 = Wo * Wo + (Wo / Q);                          //250304 add
    a1 = -2.0 * Wo * Wo;                                     //250304 add
    a2 = Wo * Wo - (Wo / Q);                                 //250304 add

    b0 /= a0;                                                //250304 add
    b1 /= a0;                                               //250304 add
    b2 /= a0;                                               //250304 add
    a1 /= a0;                                               //250304 add
    a2 /= a0;                                               //250304 add

    double output_signal = b0 * input_signal                //250304 add
                         + b1 * prev_input_1                //250304 add
                         + b2 * prev_input_2                //250304 add
                         - a1 * prev_output_1               //250304 add
                         - a2 * prev_output_2;              //250304 add

    prev_input_2 = prev_input_1;                            //250304 add
    prev_input_1 = input_signal;                            //250304 add
    prev_output_2 = prev_output_1;                          //250304 add
    prev_output_1 = output_signal;                          //250304 add

    return output_signal;                                   //250304 add
}                                                           //250304 add*/      //250321 deactivate

void dout(unsigned int variable_dout);

interrupt void cc_isr(void)
{
    // <-- Timer0 initiation
    ccTimeInit = CPUTimer_getTimerCount(CPUTIMER0_BASE);
    CPUTimer_startTimer(CPUTIMER0_BASE);
    // Timer0 initiation -->

//    ADC_Update();         // For ADC update                                    //250228 add     //250321 deactivate
    daOut();  // daOut() takes about 10[us].
    faultCheck();         // S/W fault check                                   //250228 add
//    SOGI_PLL();           // For PLL Seta                                      //250228 add

//    Current_Control();    // For control code                                  //250228 add //250301 deactivate

    // <-- ADC Sensing Code
  //  AdcResults[0] = AdcaResultRegs.ADCRESULT0;     // ADC01
 // AdcResults[1] = AdcaResultRegs.ADCRESULT1;     // ADC02
 // AdcResults[2] = AdcaResultRegs.ADCRESULT2;     // ADC03
//    AdcResults[3] = AdcaResultRegs.ADCRESULT3;     // ADC04              //250208 deactivate

//    AdcResults[4] = AdcaResultRegs.ADCRESULT4;     // ADC05              //250208 deactivate
//    AdcResults[5] = AdcaResultRegs.ADCRESULT5;     // ADC06
   // AdcResults[6] = AdccResultRegs.ADCRESULT0;     // ADC07
   // AdcResults[7] = AdccResultRegs.ADCRESULT1;     // ADC08
//    AdcResults[7] = AdcdResultRegs.ADCRESULT0;                                           //250125 add //250301 deactivate
    // AdcResults[8] = AdccResultRegs.ADCRESULT2;     // ADC09
   // AdcResults[9] = AdccResultRegs.ADCRESULT3;     // ADC010
    //<--ADC Extension Board
   // AdcResults[10] = AdccResultRegs.ADCRESULT4;     // ADC11
   // AdcResults[11] = AdccResultRegs.ADCRESULT5;     // ADC12



   // AdcValues[0] = ((((double)AdcResults[0] - (double)AdcOffset[0] * SetOffset[0])/ 4096. * 3.3 ) - (1.65 * (1-SetOffset[0])) ) * 6;  // ADC01
  //  AdcValues[1] = ((((double)AdcResults[1] - (double)AdcOffset[1] * SetOffset[1])/ 4096. * 3.3 ) - (1.65 * (1-SetOffset[1])) ) * 6;  // ADC02
  //  AdcValues[2] = ((((double)AdcResults[2] - (double)AdcOffset[2] * SetOffset[2])/ 4096. * 3.3 ) - (1.65 * (1-SetOffset[2])) ) * 6;  // ADC03
 //   AdcValues[3] = ((((double)AdcResults[3] - (double)AdcOffset[3] * SetOffset[3])/ 4096. * 3.3 ) - (1.65 * (1-SetOffset[3])) ) * 6;  // ADC04              //250208 deactivate
 //   AdcValues[4] = ((((double)AdcResults[4] - (double)AdcOffset[4] * SetOffset[4])/ 4096. * 3.3 ) - (1.65 * (1-SetOffset[4])) ) * 6;  // ADC04              //250208 deactivate
//    AdcValues[5] = ((((double)AdcResults[5] - (double)AdcOffset[5] * SetOffset[5])/ 4096. * 3.3 ) - (1.65 * (1-SetOffset[5])) ) * 6;  // ADC05
  //  AdcValues[6] = ((((double)AdcResults[6] - (double)AdcOffset[6] * SetOffset[6])/ 4096. * 3.3 ) - (1.65 * (1-SetOffset[6])) ) * 6;  // ADC06
  //  AdcValues[7] = ((((double)AdcResults[7] - (double)AdcOffset[7] * SetOffset[7])/ 4096. * 3.3 ) - (1.65 * (1-SetOffset[7])) ) * 6;  // ADC07        //250125 activate //250301 deactivate
  //  AdcValues[8] = ((((double)AdcResults[8] - (double)AdcOffset[8] * SetOffset[8])/ 4096. * 3.3 ) - (1.65 * (1-SetOffset[8])) ) * 6;  // ADC08

    // ADC Sensing Code -->

    // <-- ADC variable calculation
  Idc_2 = AdcValues[0] *  AdcScale[0];
  Vdc_2 = AdcValues[1] *  AdcScale[1];
  Idc_1 = AdcValues[3] *  AdcScale[3];
  Vdc_1 = AdcValues[4] *  AdcScale[4];
//  Isec = AdcValues[7] *  AdcScale[7];                                              //250125 add   //250301 deactivate

  LPF(Vdc_2_LPF, 20, Vdc_2, CONTROL_PERIOD);
  LPF(Vdc_1_LPF, 20, Vdc_1, CONTROL_PERIOD);
  LPF(Idc_2_LPF, 20, Idc_2, CONTROL_PERIOD);
  LPF(Idc_1_LPF, 20, Idc_1, CONTROL_PERIOD);

  Vdc_2 = Vdc_2_LPF;
  Vdc_1 = Vdc_1_LPF;
  Idc_2 = Idc_2_LPF;
  Idc_1 = Idc_1_LPF;

    // Software Fault
    if(fabs(Vdc_1) > OV_Set_Vdc_1)    Faults.SW_Prot.bit.OV_Vdc_1 = 1;
    if(fabs(Vdc_2) > OV_Set_Vdc_2)    Faults.SW_Prot.bit.OV_Vdc_2 = 1;
    if(fabs(Vdc_3) > OV_Set_Vdc_3)    Faults.SW_Prot.bit.OV_Vdc_3 = 1;
    if(fabs(Vdc_4) > OV_Set_Vdc_4)    Faults.SW_Prot.bit.OV_Vdc_4 = 1;

    if(fabs(Idc_1) > OC_Set_Idc1)    Faults.SW_Prot.bit.OC_Idc_1 = 1;
    if(fabs(Idc_2) > OC_Set_Idc2)    Faults.SW_Prot.bit.OC_Idc_2 = 1;
    if(fabs(Idc_3) > OC_Set_Idc3)    Faults.SW_Prot.bit.OC_Idc_3 = 1;
    if(fabs(Idc_4) > OC_Set_Idc4)    Faults.SW_Prot.bit.OC_Idc_4 = 1;

    if(Faults.SW_Prot.all){
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
   
    
    //***********TAB port power Calculation***********//
    Pout_1 = Vdc_1 * Idc_1;     //direction: converter to DC-source
    Pout_2 = Vdc_2 * Idc_2;     //direction: converter to DC-source
    //Pout_3 = Vdc_3 * Idc_3;     //direction: converter to DC-source
    //Pout_4 = Vdc_4 * Idc_4;     //direction: converter to DC-source



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
    {

        if(Duty_ref_PLL_P<Theta2)                                       //250304 add
        {                                                               //250304 add
//           EPwm7Regs.CMPA.bit.CMPA = EPwm3Regs.TBPRD;                             //250322 deactivate
//           EPwm7Regs.CMPA.bit.CMPA = EPwm7Regs.TBPRD;                             //250322 deactivate
             EPwm7Regs.AQCSFRC.bit.CSFA = AQ_SET;
             EPwm7Regs.AQCSFRC.bit.CSFB = AQ_SET;
        }                                                               //250304 add
        else{                                                           //250304 add
//           EPwm7Regs.CMPA.bit.CMPA = 0;                                           //250322 deactivate
             EPwm7Regs.AQCSFRC.bit.CSFA = AQ_CLEAR;                                     //250322 add
             EPwm7Regs.AQCSFRC.bit.CSFB = AQ_CLEAR;                                     //250322 add
        }                                                               //250304 add
        if(DABDuty >= PWMMaxCountHalf)
        {
            DABDuty = PWMMaxCountHalf;
        }

/*        if(Duty_ref_PLL_P<Theta2)                                       //250304 add
        {                                                               //250304 add
//            EPwm7Regs.AQSFRC.all = 0;                                   //250304 add    //250304 deactivate
//            EPwm7Regs.AQCSFRC.bit.CSFA = 0x2;                           //250304 add    //250304 deactivate
//            GpioDataRegs.GPESET.bit.GPIO157 = 1;                                          //240304 add
            EPwm7Regs.CMPA.bit.CMPA = EPwm3Regs.TBPRD;
        }                                                               //250304 add
        else{                                                           //250304 add
//            EPwm7Regs.AQSFRC.all = 0;                                   //250304 add    //250304 deactivate
//            EPwm7Regs.AQCSFRC.bit.CSFA = 0x1;                           //250304 add    //250304 deactivate
//            GpioDataRegs.GPESET.bit.GPIO157 = 0;                                          //240304 add
            EPwm7Regs.CMPA.bit.CMPA = 0;
        }                                                               //250304 add
//        if(Duty_ref_PLL_N>theta2)                                     //250304 add
//        {                                                             //250304 add
//                                                                      //250304 add
//        }                                                             //250304 add
//        else{                                                         //250304 add
//                                                                      //250304 add
//       }                                                              //250304 add
        if(Duty_ref_PLL_P2>Theta2)                                      //250304 add
        {                                                               //250304 add
//            EPwm7Regs.AQSFRC.all = 0;                                   //250304 add    //250304 deactivate
//            EPwm7Regs.AQCSFRC.bit.CSFB = 0x2;                           //250304 add    //250304 deactivate
        }                                                               //250304 add
        else{                                                           //250304 add
//            EPwm7Regs.AQSFRC.all = 0;                                   //250304 add    //250304 deactivate
//            EPwm7Regs.AQCSFRC.bit.CSFB = 0x2;                           //250304 add    //250304 deactivate
        }*/                                                             //250304 add                        */ 250321 deactivate
//        if(Duty_ref_PLL_N2<theta2)                                    //250304 add
//        {                                                             //250304 add
//                                                                      //250304 add
//        }                                                             //250304 add
//        else                                                          //250304 add
//        {                                                             //250304 add
//                                                                      //250304 add
//        }                                                             //250304 add

       if(PWMMaxCountHalf > DABDutyPre){ DABDutyPre += DABDutyDiff;}
       else if(PWMMaxCountHalf < DABDutyPre){DABDutyPre = PWMMaxCountHalf;}
       else{DABDutyPre = DABDutyPre;}
        DABDuty = (int)DABDutyPre;


    }


    // <-- DAB Phase value calculation(int)
    tempInt = (int)PWMMaxCountHalf;    

    TAB_Phase_21 = (TAB_Phase_21>=tempInt) ? tempInt : ( (TAB_Phase_21<=(-1*tempInt)) ? (-1*tempInt) : TAB_Phase_21 );
    if(TAB_Phase_21 >= 0)         {
        TAB_Phase_21_legA = TAB_Phase_21;                      TAB_Phase_21_legA_Dir = 0;
        TAB_Phase_21_legB = PWMMaxCount - TAB_Phase_21;            TAB_Phase_21_legB_Dir = 1;
    } else {
        TAB_Phase_21_legA = -1*TAB_Phase_21;                   TAB_Phase_21_legA_Dir = 1;
        TAB_Phase_21_legB = PWMMaxCount + TAB_Phase_21;            TAB_Phase_21_legB_Dir = 0;
    }


    // DAB Phase value calculation  -->

    // <-- Gating Counter Value Update
    // Phase A
    if(DABDuty < PWMMaxCountHalf){
    EPwm1Regs.TBPHS.bit.TBPHS = 0;                        //TAB port-2 leg-A (Port-2 is phase reference)
    EPwm2Regs.TBPHS.bit.TBPHS = PWMMaxCount;                  //TAB port-2 leg-B

//    EPwm7Regs.TBPHS.bit.TBPHS = 0;                         //TAB port-1 leg-A                 //250304 deactivate
//    EPwm8Regs.TBPHS.bit.TBPHS = PWMMaxCount;               //TAB port-1 leg-B                 //250304 deactivate

    EPwm1Regs.TBCTL.bit.PHSDIR = 0;                       //TAB port-2 leg-A  (Port-2 is phase reference)
    EPwm2Regs.TBCTL.bit.PHSDIR = 1;                       //TAB port-2 leg-B

//    EPwm7Regs.TBCTL.bit.PHSDIR = 0;                       //TAB port-1 leg-A                 //250304 deactivate
//    EPwm8Regs.TBCTL.bit.PHSDIR = 1;                       //TAB port-1 leg-B                 //250304 deactivate

    EPwm1Regs.CMPA.bit.CMPA = DABDuty;                        //TAB port-2 leg-A
    EPwm2Regs.CMPA.bit.CMPA = DABDuty;                        //TAB port-2 leg-B

//    EPwm7Regs.CMPA.bit.CMPA = DABDuty;                        //TAB port-1 leg-A                 //250304 deactivate
//    EPwm8Regs.CMPA.bit.CMPA = DABDuty;                        //TAB port-1 leg-B                 //250304 deactivate
    }
    else{
    EPwm1Regs.TBPHS.bit.TBPHS = 0;                        //TAB port-2 leg-A (Port-2 is phase reference)
    EPwm2Regs.TBPHS.bit.TBPHS = PWMMaxCount;                  //TAB port-2 leg-B

//    EPwm7Regs.TBPHS.bit.TBPHS = TAB_Phase_21_legA;        //TAB port-1 leg-A                 //250304 deactivate
//    EPwm8Regs.TBPHS.bit.TBPHS = TAB_Phase_21_legB;        //TAB port-1 leg-B                 //250304 deactivate

    EPwm1Regs.TBCTL.bit.PHSDIR = 0;                       //TAB port-2 leg-A  (Port-2 is phase reference)
    EPwm2Regs.TBCTL.bit.PHSDIR = 1;                       //TAB port-2 leg-B

//    EPwm7Regs.TBCTL.bit.PHSDIR = TAB_Phase_21_legA_Dir;   //TAB port-1 leg-A                 //250304 deactivate
//    EPwm8Regs.TBCTL.bit.PHSDIR = TAB_Phase_21_legB_Dir;   //TAB port-1 leg-B                 //250304 deactivate

    EPwm1Regs.CMPA.bit.CMPA = PWMMaxCountHalf;            //TAB port-2 leg-A
    EPwm2Regs.CMPA.bit.CMPA = PWMMaxCountHalf;            //TAB port-2 leg-B

//    EPwm7Regs.CMPA.bit.CMPA = PWMMaxCountHalf;            //TAB port-1 leg-A                 //250304 deactivate
//    EPwm8Regs.CMPA.bit.CMPA = PWMMaxCountHalf;            //TAB port-1 leg-B                 //250304 deactivate
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
    }
    //Flags.Reset = 0;
    // Fault reset check -->



    // <-- LED code
    if(cc_cnt >= 5000U) {

        cc_cnt = 0;
        GpioDataRegs.GPBTOGGLE.bit.GPIO50 = 1;       // LED2 - Yellow
    }else {
        cc_cnt++;

    }
    if(Flags.Fault == 1) {
        GpioDataRegs.GPBCLEAR.bit.GPIO48 = 1;      // LED0(Red) Turn on
    } else {
        GpioDataRegs.GPBSET.bit.GPIO48 = 1;      // LED0(Red) Turn off
    }
    // LEC code -->

    // <-- Timer stop
    CPUTimer_stopTimer(CPUTIMER0_BASE);
    ccTimePresent = CPUTimer_getTimerCount(CPUTIMER0_BASE);
    ccTime_us = 1000000.*(float)SYS_CLK_PRD*(ccTimeInit - ccTimePresent);
    CPUTimer_reloadTimerCounter(CPUTIMER0_BASE);
    // Timer stop -->

 //   EPwm1Regs.ETCLR.bit.SOCA = 1; //clear SOCA flag
    EPwm1Regs.ETCLR.bit.INT = 1; //clear INT flag
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;

    //UpdateVariables();

}

//----------------------------------------------------250228 add-----------------------------------------------//
void ADC_Update()
{

    // <-- ADC Sensing Code
//    AdcResults[0] = AdcaResultRegs.ADCRESULT0;     // ADC01     : Ia  for Grid current           //250301 deactivate
//    AdcResults[1] = AdcaResultRegs.ADCRESULT1;     // ADC02     : Ib  for Grid current           //250301 deactivate
//    AdcResults[2] = AdcaResultRegs.ADCRESULT2;     // ADC03     : Ic  for Grid current           //250301 deactivate
//    AdcResults[3] = AdcaResultRegs.ADCRESULT3;     // ADC04     : Vab  for Grid voltage          //250301 deactivate
//    AdcResults[4] = AdcaResultRegs.ADCRESULT4;     // ADC05     : Vbc  for Grid voltage          //250301 deactivate
//    AdcResults[5] = AdcaResultRegs.ADCRESULT5;     // ADC06     : Vca  for Grid voltage          //250301 deactivate
    //AdcResults[6] = AdccResultRegs.ADCRESULT2;     // ADC14     : Vdc  for DC voltage
///    AdcResults[7] = AdcdResultRegs.ADCRESULT0;                                                  //250301 add     //250314 deactivate

    //    AdcValues[0] = ((((double)AdcResults[0] - (double)AdcOffset[0] * SetOffset[0])/ 4096. * 3.3 ) - (1.65 * (1-SetOffset[0])) ) * 6;  // ADC01          //250301 deactivate
//    AdcValues[1] = ((((double)AdcResults[1] - (double)AdcOffset[1] * SetOffset[1])/ 4096. * 3.3 ) - (1.65 * (1-SetOffset[1])) ) * 6;  // ADC02          //250301 deactivate
//    AdcValues[2] = ((((double)AdcResults[2] - (double)AdcOffset[2] * SetOffset[2])/ 4096. * 3.3 ) - (1.65 * (1-SetOffset[2])) ) * 6;  // ADC03          //250301 deactivate
//    AdcValues[3] = ((((double)AdcResults[3] - (double)AdcOffset[3] * SetOffset[3])/ 4096. * 3.3 ) - (1.65 * (1-SetOffset[3])) ) * 6;  // ADC03          //250301 deactivate
//    AdcValues[4] = ((((double)AdcResults[4] - (double)AdcOffset[4] * SetOffset[4])/ 4096. * 3.3 ) - (1.65 * (1-SetOffset[4])) ) * 6;  // ADC03          //250301 deactivate
//    AdcValues[5] = ((((double)AdcResults[5] - (double)AdcOffset[5] * SetOffset[5])/ 4096. * 3.3 ) - (1.65 * (1-SetOffset[5])) ) * 6;  // ADC03          //250301 deactivate
    //AdcValues[6] = ((((double)AdcResults[6] - (double)AdcOffset[6] * SetOffset[6])/ 4096. * 3.3 ) - (1.65 * (1-SetOffset[6])) ) * 6;  // ADC03
//    AdcValues[7] = ((((double)AdcResults[7] - (double)AdcOffset[7] * SetOffset[7])/ 4096. * 3.3 ) - (1.65 * (1-SetOffset[7])) ) * 6; //250301 add   //250314 deactivate
//    AdcValues[7] = (double)AdcResults[7]/4096*3.3;                                  //250314 add //250314 deactivate


    // INV1 +Ig
//    Ig = AdcValues[0] *  AdcScale[0];          //250301 deactivate
    // INV1 Eg
//    Eg = AdcValues[3] *  AdcScale[3];          //250301 deactivate
//    Eg = AdcValues[7] *  AdcScale[7];                                                //250321 deactivate
    // For VDC
    Vdc_measure_old = Vdc_measure;
    Vdc_measure = AdcValues[5] * AdcScale[5];
    Vdc = 0.5*(Vdc_measure + Vdc_measure_old);
    LPF(Vdc_LPF, 10., Vdc, Tsamp);
    Vdc = Vdc_LPF;
}

//void SOGI_PLL()                                                                                                         //250321 deactivate
//{                                                                                                                       //250321 deactivate
    ////****************************For PLL Code(parameter : Kp_PLL, Ki_PLL)*******************************************// //250321 deactivate
                                                                                                                          //250321 deactivate
//    Duty_ref_PLL_P2 = Duty_ref_PLL_P * 1;               //250304 add                                                      //250321 deactivate
//    Duty_ref_PLL_N2 = Duty_ref_PLL_N * 1;               //250304 add                                                      //250321 deactivate
//    Efilt = apply_bandpass_filter(Eg);                     //250309 add                                                   //250321 deactivate
                 //SOGI PLL/////////////////////////////////////////////////////////////
/*                 Edss += Tsamp * (2.*PI*60.*((Kp_SOGI*(Eg - Edss)) - Eqss));          //250309 deactivate
                 Edss += Tsamp1 * (2.*PI*60.*((Kp_SOGI*(Efilt - Edss)) - Eqss));         //250309 add
                 iterm_eds += Tsamp1*Edss;
                 Eqss = 2.*PI*60.*iterm_eds;

                 //DQs to DQe///////////////////////////////////////////////////////////

                 Edse = CosTheta * Edss + SinTheta * Eqss;
                 Eqse = -SinTheta * Edss + CosTheta * Eqss;

                 Emag = sqrt( Edse*Edse + Eqse * Eqse );
//                 Err_Edse_pre = Err_Edse;
//                 Err_Edse = 0 - Edse;

//                 Integ_Err_Edse += Ki_PLL * Tsamp1 *0.5 * (Err_Edse + Err_Edse_pre);
//                 We_PLL_Fb = Integ_Err_Edse + Kp_PLL * Err_Edse;

//                 We_hat = We_PLL_Fb + We_norm;
//                 freq_pre = freq;
//                 freq = We_hat*INV_2PI;

//                 Theta += Tsamp1 * We_hat;
//                 Theta = BOUND_PI(Theta);                                      //250309 deactivate

                 if (Theta > PI)                                                 //250309 add
                    Theta -= 2. * PI;                                            //250309 add
                 else if (Theta < -PI)                                           //250309 add
                    Theta += 2. * PI;                                            //250309 add

                 CosTheta = cos(Theta);
                 SinTheta = sin(Theta);

                 double Theta_shifted = Theta - (PI / 2);

                 if (Theta_shifted < -PI)
                 {
                     Theta_shifted += 2 * PI;
                 }
                 else if (Theta_shifted > PI)
                 {
                     Theta_shifted -= 2 * PI;
                 }

                 if (Theta_shifted < 0)
                 {
                     Theta2 = -(-PI + 2 * (PI + Theta_shifted));
                 }
                 else
                 {
                     Theta2 = -(PI - 2 * Theta_shifted);
                 }
} */                                                                                               //250321 deactivate
/*
void Current_Control()
{

                 //Vdc setting
                     if(Vdc > 10.) {
                       INV_Vdc = 1./Vdc;
                     } else {
                       INV_Vdc = 0.1;
                     }

                 Idss = -Ig;
                 Idss_all = Idss_all + 2.*PI*60.*Tsamp*Iqss;
                 Iqss_all = Iqss_all + 2.*PI*60.*Tsamp*Idss;
                 Iqss = -1. * Idss - Idss_all + Iqss_all;

                 Idse = CosTheta * Idss + SinTheta * Iqss;
                 Iqse = -SinTheta * Idss + CosTheta * Iqss;
                 Imag = sqrt(Idse*Idse + Iqse * Iqse);

                 LPF(Idse_LPF, 40., Idse, Tsamp);
                 LPF(Iqse_LPF, 40., Iqse, Tsamp);

                 // Current control
                 if(Flags.Run == 1)
                 {
                     Flags.EnableStart = 1;
                     Flags.EnableGD1 = 1;

                     Idse_Ref = Idse_Ref_Set; // For reactive power
                     Iqse_Ref = Iqse_Ref_Set; // For active power synchronized by Eg

                     Idss_Ref = CosTheta * Idse_Ref - SinTheta * Iqse_Ref; // for reference
                     Iqss_Ref = SinTheta * Idse_Ref + CosTheta * Iqse_Ref; // for reference

                     //d-axis PI Controller
                     //Err_Idse_pre = Err_Idse;
                     Err_Idse = Idse_Ref - Idse;// - Ka_cc * Vdse_Sat;
                     Integ_Err_Idse += Ki_cc_d * Tsamp * Err_Idse;
                     Vdse_Ref_fb = Integ_Err_Idse + Kp_cc_d * Err_Idse;

                     //q-axis PI Controller
                     //Err_Iqse_pre = Err_Iqse;
                     Err_Iqse = Iqse_Ref - Iqse;// - Ka_cc * Vqse_Sat;
                     Integ_Err_Iqse += Ki_cc_q * Tsamp * Err_Iqse;
                     Vqse_Ref_fb = Integ_Err_Iqse + Kp_cc_q * Err_Iqse;

                     Vdse_Ref = Vdse_Ref_fb + 1 * We_norm * Ls * Iqse_Ref + Edse;
                     Vqse_Ref = Vqse_Ref_fb - 1 * We_norm * Ls * Idse_Ref + Eqse;

                     Vdse_Ref_Sat = (Vdse_Ref > Vdc) ? Vdc : ((Vdse_Ref < -Vdc) ? -Vdc : Vdse_Ref);
                     Vqse_Ref_Sat = (Vqse_Ref > Vdc) ? Vdc : ((Vqse_Ref < -Vdc) ? -Vdc : Vqse_Ref);

                     Vdss_Sat = cos(Theta) * Vdse_Ref_Sat + -sin(Theta) * Vqse_Ref_Sat;
                     Vqss_Sat = sin(Theta) * Vdse_Ref_Sat + cos(Theta) * Vqse_Ref_Sat;

                     // For Current control
                     Vas_Ref = Vdss_Sat;

                     // For Test by MI
                     //Van_Ref = Edss * MI;
                     Van_Ref = Vas_Ref;

                     pwmTa = HalfTsw * (Van_Ref * INV_Vdc + 0.5);
                     pwmTb = HalfTsw * (-Van_Ref * INV_Vdc + 0.5);

                    //  FLOAT to INT conversion
                     ia = (int)(ScaleGating * pwmTa + 0.5);
                     ia = (ia > (PWMMaxCount-EPWM_DB)) ? (PWMMaxCount-EPWM_DB) : ((ia < 0) ? 0 : ia);
                     EPwm1Regs.CMPA.bit.CMPA = ia;

                     ib = (int)(ScaleGating * pwmTb + 0.5);
                     ib = (ib > (PWMMaxCount-EPWM_DB)) ? (PWMMaxCount-EPWM_DB) : ((ib < 0) ? 0 : ib);
                     EPwm2Regs.CMPA.bit.CMPA = ib;
                 }
                 else{
                     Flags.EnableStart = 0;
                     Flags.EnableGD1 = 0;
                     Err_Idse = 0, Integ_Err_Idse = 0, Vdse_Ref_fb = 0, Vdse_Ref=0, Vdse_Ref_Sat=0, Vdss_Sat=0, Vdss_Sat=0;
                     Err_Iqse = 0, Integ_Err_Iqse = 0, Vqse_Ref_fb = 0, Vqse_Ref=0, Vdse_Ref_Sat=0, Vdss_Sat=0;
                     Van_Ref =0;
                 }
}*/                                                                                            //250301 deactivate
//----------------------------------------------------250228 add-----------------------------------------------//

/////////////////////////////////CC END/////////////////////////////////////////
