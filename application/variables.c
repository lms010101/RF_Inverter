////////////////////////////////////
//// variables.c                ////
//// Created on: 2020. 2. 5.    ////
//// Author: Dong Yeup Han      ////
////////////////////////////////////
#include "variables.h"
///Variable Switching frequency //

//Uint32 PWMMax = 1666; //60kHz, 52kHz = 1923
//Uint32 PWMMaxCount =  1666;    // To change switching frequency, determine PWMMaxCount.
//Uint32 PWMMaxCountHalf   =  1666 * 0.5;     // And, for controller, recalculate PWMMaxCountHalf, Tsmap, Tsw
//Uint32 PWMMaxCountQuarter  = 1666 * 0.25;  // And, for controller, recalculate PWMMaxCountHalf, Tsmap, Tsw
//double Tsamp                =  0.00001666666666;   // Single sampling / Double sampling is determined by register 'EPwmRegs->ETSEL.bit.INTSEL' in EPWM.c.
//double Tsw                  =  0;   // Tsamp = 0.5*Tsw (Double sampling) / Tsamp = Tsw (Single sampling)
//Uint16 EPWM_DB              =  90;
//
////*************  Flags  *************************/////
FLAG Flags;

////*************  Encoder  *************************/////
ENCOBJ encData;
void InitENCOBJ_(void) {
    encData.PolePair =1;       // Pulse Per Revolution
    encData.Thetarm= 0;      // Multiplication Factor
    encData.Thetae=0;   // PPR*mode - 1
    encData.Counter=0;
    encData.Thetae_init=0;
    encData.QPOSMAX=4095;
}
double turn =0;
////*************  ADC variables  *************************/////

Uint16 AdcResults[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
double AdcValues[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
double AdcOffset[12] = {1, 1, 1, 2, 2, 1, 1, 1, 1, 1, 1, 1};
double SetOffset[12] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
double AdcScale[12] = {//22,                   // Idc2                             //250914
                       1,                                      //250914 add Vdc2
//                       1050,               // Vdc2           //250912 deactivate     //250913 Idc1
                       1,                                      //250912 add Idc2
                       1,                  // None             //250914 add Vdc1
//                       22,              // Idc1              //250914 deactivate
                       1,                                      //250914 add Idc1
                       630,               // Vdc1
                       2000./(250./10.),                  // Ain1_6 : Idc3      2000:1, 270ohm/2
//                       124000./(2.5*270.)*1,              // Ain1_7 : Vdc1                       //250125 deactivate
                       630,                  // Ain1_7 : Isec                         //250125 add
                       124000./(2.5*200.)*1.,             // Ain1_8 : Vdc2
                       124000./(2.5*200.)*1.,             // Ain1_9 : Vdc3
                       124000./(2.5*200.)*1.,             // Ain1_10 :Vdc4
                       124000./(2.5*200.)*1.,             // Ain1_11 :
                       124000./(2.5*200.)*1.,             // Ain1_12 :
                       };

extern float Pmeasure=0;                                                                     //251103 add

////*************  CC timer variables  *************************/////
uint32_t ccTimeInit = 0, ccTimePresent = 0;
float ccTime_us = 0.;
////*************  Din, Dout variables  *************************/////
Uint16 variable_dout =0;
Uint16 variable_din =0;
extern double Ivar1=6;
extern double Ivar2=6;
extern double gain=2000.;

//extern double duty = 50.0;
extern double duty = 50.0;
extern double Dt = 0;
////***************************** PWM*************************************////
Uint32 tmpDuty=0;
double ScaleGating = SYS_CLK;
double HalfTsw = 0.0;
double pwmTa = 0., pwmTb = 0., pwmTc = 0.;
Uint32 PWM01_Count_Ref = 0, PWM02_Count_Ref = 0, PWM03_Count_Ref = 0;
Uint32 PWM04_Count_Ref = 0, PWM05_Count_Ref = 0, PWM06_Count_Ref = 0;
Uint32 PWM07_Count_Ref = 0, PWM08_Count_Ref = 0, PWM09_Count_Ref = 0;
Uint32 PWM10_Count_Ref = 0, PWM11_Count_Ref = 0, PWM12_Count_Ref = 0;
float PhSft = 0.0, PhaseShift = 0.0;                                                          //251022 add
// <-- user variables
////************* System operation variables ***************/////
Uint16 Status = 3, Cmd_DAB = 0;
Uint16 Operation_Mode = 0;     // 0: self control   1: Rectifier control

double Ias = 0., Ibs = 0., Ics = 0.;
double Eab = 0., Ebc = 0., Eca = 0.;
double Eas = 0., Ebs = 0., Ecs = 0.;
double Ig = 0, Ig1 = 0, Ig_LPF =0., Ig1_LPF =0.;                   //250228 add
double Eg = 0., Eg_LPF = 0.;
double Is = 0.;                                                         //250309 add
double Vdc = 0., Vdc_Set = 0., INV_Vdc = 0.;
double Vdc_measure = 0., Vdc_measure_old = 0.;


////*************GRID : Parameter *************************/////
double Edss = 0., Eqss = 0., Edse = 0., Eqse = 0., Emag = 0., Emag_norm = 0.;
double Err_Edse = 0., Err_Eqse = 0., Err_Eqse_pre = 0., Err_Edse_pre = 0.;
double freq= 0.;
double Ls= 0.;
double Rs= 0.;
double Theta = 0., Theta2 =0., Theta_shifted = 0.;

////*************PLL : Parameter *************************/////
double We_PLL_Integ=0, We_PLL_Fb=0;
//double Fc_PLL = 0. , Wc_PLL = 0., Kp_PLL = 0., Ki_PLL = 0.;           //250309 deactivate
double Fc_PLL = 0. , Wc_PLL = 0.;                                       //250309 add
//double Kp_PLL = 5., Ki_PLL = 100.;                                       //250309 add//250322 deactivate
double Kp_PLL = 5., Ki_PLL = 10.;                                          //250322 add
//double We_hat = 0. , We_norm = 0. ;                                   //250309 deactivate
double We_hat = 0.;
double We_norm = 85000*2*PI;                                          //250314 add
double Thetae = 0. , CosThetae = 0. , SinThetae = 0. ;
double zeta_PLL = 0.;                                           //250228 add
double Integ_Err_Eqse = 0., Integ_Err_Edse = 0.;
extern double freq_pre = 0.;
//extern double Kp_SOGI = 1.;                                    //250228 add    //250309 deactivate
//extern double Kp_SOGI = 50000;                                   //250309 add    //250322 deactivate
extern double Kp_SOGI = 5;                                                      //250322 add
////*************Current Control : Parameter *************************/////
double Is_rated = 0. ;
double Ig_rated = 20. ;                                        //250228 add
double Err_Idse_pre = 0. ,Err_Iqse_pre = 0. ;
double Err_Idse = 0. ,Err_Iqse = 0. ;
double Idss_Ref,Idss = 0. ,Iqss_Ref,Iqss = 0. ;
double Idse = 0. ,Iqse = 0. ;
double Idse_LPF = 0. ,Iqse_LPF = 0. ;                         //250228 add
double Idse_Ref = 0. ,Iqse_Ref = 0. ;
double Idse_Ref_Set = 0., Iqse_Ref_Set = 0. ;
double Vdss_Ref_Sat = 0. ,Vqss_Ref_Sat = 0. ,Vdss_Sat,Vqss_Sat = 0. ,Vqse_Sat,Vdse_Sat = 0. ;
double Vdse_Ref_Sat = 0., Vqse_Ref_Sat = 0.;                                                           //250228 add
double Integ_Err_Idse = 0. ,Integ_Err_Iqse = 0. ;
double Imag = 0. ;
double Idss_all = 0., Iqss_all = 0., iterm_eds =0., iterm_Ids;                                         //250228 add

double Vas_Ref = 0. , Vbs_Ref = 0. , Vcs_Ref = 0. ;


double Vdse_Ref_fb = 0.;
double Vqse_Ref_fb = 0.;
double Vdse_Ref = 0., Vdse_Integ = 0., Vdse_Ref_Ff = 0. , Vdss_Ref = 0.;
double Vqse_Ref = 0., Vqse_Integ = 0., Vqse_Ref_Ff = 0. , Vqss_Ref = 0.;
double Van_Ref = 0., Vbn_Ref = 0., Vcn_Ref = 0.;
double Wcc = 4000.;                                                                                    //250228 add

double Ki_cc_d = 0. ,Kp_cc_d = 0. ,Ki_cc_q = 0. ,Kp_cc_q = 0. ,Ka_cc = 0. ;


////占쏙옙타 占쏙옙占�////// <--占쏙옙占쏙옙 占쌀곤옙
unsigned int GD_All=0;
int cc_cnt =0;
double CosTheta = 0., SinTheta = 0.;


////************* Faults variables *************************/////
double OV_Set_VdcP = 0., OV_Set_VdcS = 0.;
double OV_Set_Vdc_1 = 0., OV_Set_Vdc_2 = 0., OV_Set_Vdc_3 = 0., OV_Set_Vdc_4 = 0.;
double OC_Set_Idc1 =0., OC_Set_Idc2 =0., OC_Set_Idc3 =0., OC_Set_Idc4 =0.;
////************* Converter variables *************************/////Flag_VdcPCont
double Vdc_1 = 0., Vdc_2 = 0., Vdc_3 = 0., Vdc_4 = 0.;
double Vdc_2_LPF = 0., Vdc_1_LPF = 0., Idc_2_LPF = 0., Idc_1_LPF = 0.;
//double Idc_1 =0., Idc_2 = 0., Idc_3 = 0., Idc_4 =0.;                     //250125 deactivate
double Idc_1 =0., Idc_2 = 0., Idc_3 = 0., Idc_4 =0., Isec =0.;             //250125 add
double Pout_1 = 0., Pout_2 = 0., Pout_3 = 0., Pout_4 = 0.;
double Pout_2_ref = 0., Pout_3_ref = 0., Pout_4_ref = 0.;
double Efficiency =0., Ploss=0.;
double SRStopflag = 0;                                                         //251104 add
////************* MAB Control *************************/////
Uint16 EnableRec = 0, EnableRec_Old = 0, EnableDABP = 0, EnableDABS = 0, EnableInverter = 0;
int16 Flag_VdcSCont = 0, Flag_VdcPCont = 0, Flag_CCont = 0, Flag_BalCont = 0, Flag_DAB_Ready = 0, Flag_VdcS_Fault_Disable = 0;

double VdcP_A_LPF = 0., VdcP_B_LPF = 0., VdcP_C_LPF = 0., VdcS_LPF = 0.;
double VdcP_avg = 0.;
double Efilt = 0.;
double Ecap_1 = 0., Ecap_2 = 0., Ecap_3 = 0., Ecap_4 = 0.;
double Err_Ecap_13=0., Err_Ecap_1= 0., Err_Ecap_2= 0., Err_Ecap_3= 0., Err_Ecap_4= 0.;
double Err_Ecap_13_ref= 0., Ecap_1_ref= 0., Ecap_2_ref= 0., Ecap_3_ref= 0., Ecap_4_ref= 0.;
double Pdc_13=0., Pdc_1= 0, Pdc_2= 0, Pdc_3= 0, Pdc_4= 0;
double Pdc_13_Integ=0., Pdc_1_Integ= 0, Pdc_2_Integ= 0, Pdc_3_Integ= 0, Pdc_4_Integ= 0;
double Ecap_1_cc_ref =0., Ecap_2_cc_ref=0., Ecap_3_cc_ref=0., Ecap_4_cc_ref=0.;

double Duty_ref_PLL_P2 = 0, Duty_ref_PLL_N2 = 0;
double Duty_ref_PLL_P = 2, Duty_ref_PLL_N = -2;                //250322 deactivate
//double Duty_ref_PLL_P = 1, Duty_ref_PLL_N = -1;                  //250322 add



double a =0., b =0., c =0., d =0., e =0., f =0.;
double a_11 =0., a_12 =0., a_13 =0., a_21 =0., a_22 =0., a_23 =0., a_31 =0., a_32 =0., a_33 =0.;
double alpha =0., beta =0., gamma =0.;
double det_G =0.;
int DABPhase = 0.;
double DABDutySet = 0, DABDutyDiff = 0., DABDutyPre = 0;
double DABPhase_ratio_A =0.;
int DABDuty = 0;
double TAB_Phase_12_ratio =0., TAB_Phase_13_ratio =0., TAB_Phase_21_ratio =0., TAB_Phase_23_ratio =0.;
int TAB_Phase_12 =0., TAB_Phase_13 =0., TAB_Phase_23 =0., TAB_Phase_21 =0.;
int TAB_Phase_12_legA =0., TAB_Phase_12_legB =0., TAB_Phase_13_legA =0., TAB_Phase_13_legB =0.;
int TAB_Phase_21_legA =0., TAB_Phase_21_legB =0., TAB_Phase_23_legA =0., TAB_Phase_23_legB =0.;
int TAB_Phase_12_legA_Dir =0., TAB_Phase_12_legB_Dir =0., TAB_Phase_13_legA_Dir =0., TAB_Phase_13_legB_Dir =0.;
int TAB_Phase_21_legA_Dir =0., TAB_Phase_21_legB_Dir =0., TAB_Phase_23_legA_Dir =0., TAB_Phase_23_legB_Dir =0.;

double Phase_12_ratio =0., Phase_13_ratio =0., Phase_14_ratio =0.;
double Phase_12 =0., Phase_13 =0., Phase_14 =0.;
int Phase_12_count =0., Phase_13_count =0., Phase_14_count =0.;
int Phase_12_legA =0., Phase_12_legB =0., Phase_13_legA =0., Phase_13_legB =0., Phase_14_legA =0., Phase_14_legB =0.;
int Phase_11_legA_Dir =0., Phase_11_legB_Dir =0.,Phase_12_legA_Dir =0., Phase_12_legB_Dir =0., Phase_13_legA_Dir =0., Phase_13_legB_Dir =0., Phase_14_legA_Dir =0., Phase_14_legB_Dir =0.;

Uint16 GaDABPAUCount_A = 0, GaDABPALCount_A = 0, GaDABPBUCount_A = 0, GaDABPBLCount_A = 0;
Uint16 GaDABSAUCount_A = 0, GaDABSALCount_A = 0, GaDABSBUCount_A = 0, GaDABSBLCount_A = 0;
int DABPCount_A = 0, DABSCount_A = 0, DABPhase_A = 0;
int DABP_Phase_A = 0, DABS_Phase_A = 0, DABP_Phase_Dir_A = 0, DABS_Phase_Dir_A = 0;

Uint16 GaDABPAUCount_B = 0, GaDABPALCount_B = 0, GaDABPBUCount_B = 0, GaDABPBLCount_B = 0;
Uint16 GaDABSAUCount_B = 0, GaDABSALCount_B = 0, GaDABSBUCount_B = 0, GaDABSBLCount_B = 0;
int DABPCount_B = 0, DABSCount_B = 0, DABPhase_B = 0;
int DABP_Phase_B = 0, DABS_Phase_B = 0, DABP_Phase_Dir_B = 0, DABS_Phase_Dir_B = 0;

Uint16 GaDABPAUCount_C = 0, GaDABPALCount_C = 0, GaDABPBUCount_C = 0, GaDABPBLCount_C = 0;
Uint16 GaDABSAUCount_C = 0, GaDABSALCount_C = 0, GaDABSBUCount_C = 0, GaDABSBLCount_C = 0;
int DABPCount_C = 0, DABSCount_C = 0, DABPhase_C = 0;
int DABP_Phase_C = 0, DABS_Phase_C = 0, DABP_Phase_Dir_C = 0, DABS_Phase_Dir_C = 0;


// DAB control gain and control parameters
double Ldab = 0., DABPhaseScale = 0.;
double Err_EcapS = 0.;
double PdcS_Integ = 0., PdcS = 0.;
double Fc_VdcP = 0., Wc_VdcP = 0., Ki_VdcPT = 0., Kp_VdcP = 0., zeta_VdcP = 0.;
double Fc_VdcS = 0., Wc_VdcS = 0., Ki_VdcST = 0., Kp_VdcS = 0., zeta_VdcS = 0.;
double Fc_cc = 0., Wc_cc = 0.;
double Kp_Pdc = 0., Ki_Pdc = 0.;
double Kp_Idc=0., Ki_IdcT = 0.;
double VdcS_Ref =0.;


double n2 =0., n3=0., n4=0;
double L1=0., L2=0., L3=0., L4=0., L2_dot=0., L3_dot=0.,
       L4_dot=0., L12=0., L13=0., L14=0., L23=0., L24=0., L34=0., Ldc =0.;
double TABPhaseScale_12 =0., TABPhaseScale_13=0.;
double Cdc_1 = 0., Cdc_2 = 0., Cdc_3 = 0., Cdc_4 = 0.;
double Vdc_1_ref = 0., Vdc_2_ref = 0., Vdc_3_ref = 0., Vdc_4_ref = 0.;
double Pdc_1_ref = 0., Pdc_2_ref = 0., Pdc_3_ref = 0., Pdc_4_ref = 0.;
double Idc_1_ref =0., Idc_2_ref =0., Idc_3_ref =0., Idc_4_ref =0.;
double Vdc_1_cc_ref =0., Vdc_2_cc_ref =0., Vdc_3_cc_ref =0., Vdc_4_cc_ref =0.;
double Idc_1_Integ =0., Idc_2_Integ =0., Idc_3_Integ =0., Idc_4_Integ =0.;
double Err_Idc_1 =0., Err_Idc_2 =0., Err_Idc_3 =0., Err_Idc_4 =0.;
double ff1 = 0., ff2 = 0., Sin_ff = 0., theta_ff = 0., G1 = 0., G2 = 0.;

// user variables -->
double VacP_norm = 0., VacS_norm = 0.;
double Rdab = 0., Mdab = 0.;
int DABPhase_A_test = 0, Flag_control = 0 ;
int16 tempInt = 0;
double Vdc_LPF = 0;                                          //250228 add

Uint32 cap1Value = 0;                               //250412 add
Uint32 cap2Value = 0;                               //250412 add
Uint16 update_pwm_flag=0;                  //250412 add

Uint32 faultnum = 0;                             //251029 add

Uint32 cnt=0;                                                 //250417 add

Uint32 cnt_test = 0;

Uint16 direction = 0;                                                          //250411 add
Uint32 ECap1IntCount = 0;                                                      //250411 add
Uint32 ECap1PassCount = 0;                                                     //250411 add

int flag = 0;                                                                                 //250805 add

void InitFlags(void) {
    Flags.Reset = 0;
    Flags.Fault = 0;
    Flags.EnableStart = 0, Flags.EnableStart_Old = 0;  // Enable PWM Gates which are enabled.
    Flags.EnableGD1 = 0;  //PWM Gate 1 enable
    Flags.EnableGD2 = 0;  //PWM Gate 2 enable
    Flags.EnableGD3 = 0;  //PWM Gate 3 enable
    Flags.EnableGD4 = 0;  //PWM Gate 4 enable
    Flags.Can_Error = 0;
}

void InitVariables(void) {
    // TAB System parameters
    OV_Set_Vdc_1 = 500., OV_Set_Vdc_2 = 500., OV_Set_Vdc_3 = 500., OV_Set_Vdc_4 = 500.;
    OC_Set_Idc1 = 50., OC_Set_Idc2 = 50., OC_Set_Idc3 = 50., OC_Set_Idc4 = 50.;
    Operation_Mode = 1;

//    We_norm = TWOPI*60;                                             //250228 add    //250313 deactivate
//    Fc_PLL = 0.1;                                                   //250228 add
//    Kp_SOGI = 0.4;                                                  //250228 add    //250312 deactivate
//    zeta_PLL = 0.707;                                               //250228 add
//    Wc_PLL = TWOPI * Fc_PLL;                                        //250228 add
//    Kp_PLL = 2*zeta_PLL*Wc_PLL;                                     //250228 add
//    Ki_PLL = Wc_PLL*Wc_PLL;                                         //250228 add
//   Wcc = TWOPI * Fc_cc;                                            //250228 add
    //    Ki_cc_d = Rs * Wcc ;                                            //250228 add
    //    Kp_cc_d = Ls * Wcc ;                                            //250228 add
    //    Ki_cc_q = Rs * Wcc ;                                            //250228 add
    //    Kp_cc_q = Ls * Wcc ;                                            //250228 add
    //    Ka_cc = (float)1/Kp_cc_d;                                       //250228 add

    // DAB variables
    DABDutySet = 0.;
    DABDutyDiff = 0.2;
    Ldab = 40e-6;
    Mdab = 1/(2*PI*PI*60e3*Ldab)*8./PI;
    
    // Energy Controller Gain
    Fc_VdcS = 50.;
    zeta_VdcS = 1.414;
    Wc_VdcS = TWOPI * Fc_VdcS;
    Ki_VdcST = Wc_VdcS*Wc_VdcS * Tsamp;
    Kp_VdcS = 2*zeta_VdcS*Wc_VdcS;
}

void UpdateVariables(void)
{
    Mdab = 1/(2*PI*PI*60e3*Ldab)*8./PI;

    // Energy Controller Gain
    Wc_VdcS = TWOPI * Fc_VdcS;
    Ki_VdcST = Wc_VdcS*Wc_VdcS * Tsamp;
    Kp_VdcS = 2*zeta_VdcS*Wc_VdcS;

    HalfTsw = 0.5*Tsw;

    PWM01_Count_Ref = PWMMaxCountHalf, PWM02_Count_Ref = PWMMaxCountHalf, PWM03_Count_Ref = PWMMaxCountHalf;
    PWM04_Count_Ref = PWMMaxCountHalf, PWM05_Count_Ref = PWMMaxCountHalf, PWM06_Count_Ref = PWMMaxCountHalf;
    PWM07_Count_Ref = PWMMaxCountHalf, PWM08_Count_Ref = PWMMaxCountHalf, PWM09_Count_Ref = PWMMaxCountHalf;
    PWM10_Count_Ref = PWMMaxCountHalf, PWM11_Count_Ref = PWMMaxCountHalf, PWM12_Count_Ref = PWMMaxCountHalf;

//    Wc_PLL = TWOPI * Fc_PLL;                                               //250228 add
//    Kp_PLL = 2*zeta_PLL*Wc_PLL;                                            //250228 add
//    Ki_PLL = Wc_PLL*Wc_PLL;                                                //250228 add
//    Wcc = TWOPI * Fc_cc;                                                   //250228 add
//    Ki_cc_d = Rs * Wcc ;                                                   //250228 add
//    Kp_cc_d = Ls * Wcc ;                                                   //250228 add
//    Ki_cc_q = Rs * Wcc ;                                                   //250228 add
//    Kp_cc_q = Ls * Wcc ;                                                   //250228 add
//    Ka_cc = (float)1/Kp_cc_d;                                              //250228 add
}


int test_A = 0, test_B = 0, test_C = 0;
double test_DABDutySet = 0.;
int Flags_Freq = 0;
double DAB_Duty_test = 5000.;//PWMmaxcount
////////////////////////////// done -->
