////////////////////////////////////
//// variables.h                ////
//// Created on: 2020. 2. 5.    ////
//// Author: Dong Yeup Han      ////
////////////////////////////////////
#include "F28x_Project.h"
#include "device.h"

#ifndef APPLICATION_VARIABLES_H_
#define APPLICATION_VARIABLES_H_

#define SYS_CLK                 ((float)200e6)
#define SYS_CLK_PRD             ((float)5e-9)
//#define PWM_PRD 1063
//#define PWM_PRD 1018                                                       //250826 deactivate
//#define PWM_PRD 1018                                                         //250826 add   //250905 deactivate
#define PWM_PRD 1176                                                                          //250905 add
//#define PWM_PRD 1111
//#define PWM_PRD 1163
//#define PWM_PRD 1205
//#define PWM_PRD 1136

// // 10kHz switching setting
// #define PWMMaxCount             ((Uint32)10000)    // To change switching frequency, determine PWMMaxCount.
// #define PWMMaxCountHalf         ((Uint32)5000)     // And, for controller, recalculate PWMMaxCountHalf, Tsmap, Tsw
// #define PWMMaxCountQuarter      ((Uint32)2500)     // And, for controller, recalculate PWMMaxCountHalf, Tsmap, Tsw
// #define Tsamp                   ((double)100e-6)   // Single sampling / Double sampling is determined by register 'EPwmRegs->ETSEL.bit.INTSEL' in EPWM.c.
// #define Tsw                     ((double)100e-6)   // Tsamp = 0.5*Tsw (Double sampling) / Tsamp = Tsw (Single sampling)

// 100kHz switching setting
//#define PWMMaxCount             ((Uint32)1176)    // To change switching frequency, determine PWMMaxCount.          //250306 deactivate  //250730 deactivate
//#define PWMMaxCountHalf         ((Uint32)588)     // And, for controller, recalculate PWMMaxCountHalf, Tsmap, Tsw   //250306 deactivate
//#define PWMMaxCountQuarter      ((Uint32)294)     // And, for controller, recalculate PWMMaxCountHalf, Tsmap, Tsw   //250306 deactivate
//#define PWMMaxCount             ((Uint32)1063)                                                                                            //250805 add
//#define PWMMaxCountHalf         ((Uint32)531)                                                                                             //250805 add
//#define PWMMaxCountQuarter      ((Uint32)265)                                                                                             //250805 add
#define PWMMaxCount             ((Uint32)PWM_PRD)
#define PWMMaxCountHalf         ((Uint32)PWM_PRD*0.5)
#define PWMMaxCountQuarter      ((Uint32)PWM_PRD*0.25)
//#define PWMMaxCount             ((Uint32)1075)                                                                                            //250805 add
//#define PWMMaxCountHalf         ((Uint32)537)                                                                                             //250805 add
//#define PWMMaxCountQuarter      ((Uint32)268)                                                                                             //250805 add

#define Tsamp                   ((double)10e-6)   // Single sampling / Double sampling is determined by register 'EPwmRegs->ETSEL.bit.INTSEL' in EPWM.c.
#define Tsw                     ((double)10e-6)   // Tsamp = 0.5*Tsw (Double sampling) / Tsamp = Tsw (Single sampling)
#define Tsamp1                  ((double)0.1e-6)                                                                    //250313 add
//#define EPWM_DB                 ((Uint16)30)                // 1us-> 200cnt / max: 5us. 1000cnt  //250306 deactivate                     //250904 deactivate
#define EPWM_DB                 ((Uint16)60)                // 1us-> 200cnt / max: 5us. 1000cnt    //250306 add       //250312 deactivate  //250904 activate

//Can
#define MSG_DATA_LENGTH           4
#define CAM_TX_MSG_OBJ            1
#define CAM_RX_MSG_OBJ            15

// Macro Functions
#define BOUND_PI(x)     ((x) - TWOPI*floor(((x) + PI) * INV_2PI))

//Constant Value
#define PI          ((float)3.141592653589793)
#define INV_PI      ((float)0.318309886183791)
#define PI_2        ((float)1.57079632679)
#define PI_3        ((float)1.047197551196598)
#define PI_2over3   ((float)2.094395102393195)
#define TWOPI       ((float)6.283185307179586)
#define SQRT2       ((float)1.414213562373095)
#define SQRT3       ((float)1.732050807568877)
#define INV_SQRT3   ((float)0.577350269189626)
#define INV_SQRT2   ((float)0.707106781186547)
#define INV_3       ((float)0.333333333333333)
#define INV_2PI     ((float)0.159154943091895)

void InitFlags(void);
void InitVariables(void);
void UpdateVariables(void);
void InitEQep();
void InitEPwmRegs(void);
void initPWMDABS(volatile struct EPWM_REGS *EPwmRegs, Uint16 periodCount, Uint16 deadbandCount);
void InitAdc(void);
void BalancingController(void);
void EnergyController(void);
//void PowerController (double Pdc_ref , double Idc, double Vdc_ref);
///Variable Switching frequency //

//extern Uint16 EPWM_DB;
//extern Uint32 PWMMax;
//extern Uint32 PWMMaxCount;    // To change switching frequency, determine PWMMaxCount.
//extern Uint32 PWMMaxCountHalf;     // And, for controller, recalculate PWMMaxCountHalf, Tsmap, Tsw
//extern Uint32 PWMMaxCountQuarter;     // And, for controller, recalculate PWMMaxCountHalf, Tsmap, Tsw
//extern double Tsamp;   // Single sampling / Double sampling is determined by register 'EPwmRegs->ETSEL.bit.INTSEL' in EPWM.c.
//extern double Tsw;   // Tsamp = 0.5*Tsw (Double sampling) / Tsamp = Tsw (Single sampling)

extern float Pmeasure;                                         //251103 add
//
////*************  Flags  *************************/////
typedef struct  _FLAGS_
{
    Uint16 Reset;
    Uint16 Fault, Fault_Old;
    Uint16 Run;
    Uint16 EnableStart, EnableStart_Old;
    Uint16 EnableGD1;
    Uint16 EnableGD2;
    Uint16 EnableGD3;
    Uint16 EnableGD4;
    int Can_Error;
} FLAG;
extern FLAG Flags;

////*************  Encoder  *************************/////
typedef struct _ENCOBJ_
{
    int PolePair;       // Pulse Per Revolution
    long Thetarm;      // Multiplication Factor
    long Thetae;   // PPR*mode - 1
    double Counter;
    double Thetae_init;
    double QPOSMAX;
} ENCOBJ;
extern ENCOBJ encData;

struct P_FB_out
{
    double P_FB_out_1;
    double P_FB_out_2;
    double P_FB_out_3;
};

extern double turn;
////*************  ADC variables  *************************/////
//Gain 관련
extern double Ivar1;
extern double Ivar2;
extern double gain;
extern Uint16 AdcResults[12];
extern double AdcValues[12];
extern double AdcOffset[12];
extern double SetOffset[12];
extern double AdcScale[12];
extern double SRStopflag;                                                         //251104 add
////*************  CC timer variables  *************************/////
extern uint32_t ccTimeInit, ccTimePresent;
extern float ccTime_us;

////*************  Din, Dout variables  *************************/////
extern Uint16 variable_dout;
extern Uint16 variable_din;

////*************  PWM  ***************************************////
extern Uint32 tmpDuty;
extern double ScaleGating;
extern double HalfTsw;
extern double pwmTa, pwmTb, pwmTc;
extern Uint32 PWM01_Count_Ref, PWM02_Count_Ref, PWM03_Count_Ref;
extern Uint32 PWM04_Count_Ref, PWM05_Count_Ref, PWM06_Count_Ref;
extern Uint32 PWM07_Count_Ref, PWM08_Count_Ref, PWM09_Count_Ref;
extern Uint32 PWM10_Count_Ref, PWM11_Count_Ref, PWM12_Count_Ref;
extern float PhSft, PhaseShift;                                               //251022 add
#define LIMIT(A, min,max)      A=A<(min)?(min):A>(max)?(max):A                //251022 add
// <-- user variables
////************* System operation variables ***************/////
extern Uint16 Status, Cmd_DAB;
extern Uint16 Operation_Mode;

extern Uint32 faultnum;                               //251029 add

extern double Eg, Eg_LPF;
extern double Efilt;                                                 //250309 add                                                       //250228 add
extern double Ig, Ig1, Ig_LPF, Ig1_LPF;                           //250228 add
extern double Ia, Ib, Ic, Ia_LPF, Ib_LPF, Ic_LPF;                 //250228 add
extern double Ias, Ibs, Ics;
extern double Eab, Ebc, Eca;
extern double Eas, Ebs, Ecs;
extern double Vdc, Vdc_Set, INV_Vdc;
extern double Vdc_LPF;                                        //250228 add
extern double Vdc_measure, Vdc_measure_old;
extern Uint16 update_pwm_flag;                                           //250412 add
////*************GRID : Parameter *************************/////
extern double Edss, Eqss, Edse, Eqse, Emag, Emag_norm;
extern double Err_Edse, Err_Eqse,Err_Eqse_pre,Err_Edse_pre;
extern double freq;
extern double Ls;
extern double Rs;
extern double Theta, Theta2, Theta_shifted;
////*************PLL : Parameter *************************/////
extern double We_PLL_Integ, We_PLL_Fb;
extern double Fc_PLL, Wc_PLL,Kp_PLL, Ki_PLL;
extern double zeta_PLL;                                                          //250228 add
extern double We_hat, We_norm;
extern double Thetae, CosThetae, SinThetae;
extern double Integ_Err_Eqse, Integ_Err_Edse;
extern double freq_pre, Kp_SOGI;


#define PWM7_TIMER_MIN     10
#define PWM7_TIMER_MAX     8000
// To keep track of which way the timer value is moving
#define EPWM_TIMER_UP   1
#define EPWM_TIMER_DOWN 0

extern Uint32  ECap2IntCount;                           //250411 add
extern Uint32  ECap2PassCount;                          //250411 add
extern Uint32  EPwm7TimerDirection;                     //250411 add
extern Uint32 cnt;                                                          //250417 add

////*************Current Control : Parameter *************************/////
extern double Is_rated;
extern double Ig_rated;                                                          //250228 add
extern double Err_Idse_pre,Err_Iqse_pre;
extern double Err_Idse,Err_Iqse;
extern double Idss_Ref,Idss,Iqss_Ref,Iqss;
extern double Idse,Iqse;
extern double Idse_Ref,Iqse_Ref;
extern double Idse_LPF, Iqse_LPF;                         //250228 add
//extern double Idse_LPF = 0,Iqse_LPF = 0;                //250228 deactivate
extern double Idse_Ref_Set,Idse_Ref_Set,Iqse_Ref_Set,Iqse_Ref_Set;
extern double Vdss_Ref_Sat,Vqss_Ref_Sat,Vdss_Sat,Vqss_Sat,Vqse_Sat,Vdse_Sat;
extern double Integ_Err_Idse,Integ_Err_Iqse;
extern double Imag,iterm_eds;
extern double Idss_all, Iqss_all, iterm_Ids;                            //250228 add
extern double Wcc;                                                      //250228 add
extern double Vdse_Ref_Sat, Vqse_Ref_Sat;                               //250228 add

extern double Vas_Ref, Vbs_Ref, Vcs_Ref;

extern double Vdse_Ref_fb;
extern double Vqse_Ref_fb;
extern double Vdse_Ref, Vdse_Integ, Vdse_Ref_Ff, Vdss_Ref;
extern double Vqse_Ref, Vqse_Integ, Vqse_Ref_Ff, Vqss_Ref;
extern double Van_Ref, Vbn_Ref, Vcn_Ref;

extern double Ki_cc_d,Kp_cc_d,Ki_cc_q,Kp_cc_q,Ka_cc;


////占쏙옙타 占쏙옙占�//////
extern unsigned int GD_All;
extern int cc_cnt;
extern double CosTheta, SinTheta;

////************* Faults variables *************************/////
extern double OV_Set_VdcP, OV_Set_VdcS;
extern double OV_Set_Vdc_1, OV_Set_Vdc_2, OV_Set_Vdc_3, OV_Set_Vdc_4;
extern double OC_Set_Idc1, OC_Set_Idc2, OC_Set_Idc3, OC_Set_Idc4;
////************* Converter variables *************************/////
extern double Vdc_1, Vdc_2, Vdc_3, Vdc_4;
extern double Vdc_2_LPF, Vdc_1_LPF, Idc_2_LPF, Idc_1_LPF;
//extern double Idc_1, Idc_2, Idc_3, Idc_4;                            //250124 deactivate
extern double Idc_1, Idc_2, Idc_3, Idc_4, Isec;                        //250125 add
extern double Pout_1, Pout_2, Pout_3, Pout_4;
extern double Pout_2_ref, Pout_3_ref, Pout_4_ref;
extern double Efficiency, Ploss;
////************* MAB Control *************************/////
extern Uint16 EnableRec, EnableRec_Old, EnableDABP, EnableDABS, EnableInverter;
extern int16 Flag_VdcSCont, Flag_VdcPCont, Flag_CCont, Flag_BalCont, Flag_DAB_Ready, Flag_VdcS_Fault_Disable;

extern double VdcP_A_LPF, VdcP_B_LPF, VdcP_C_LPF, VdcS_LPF;
extern double VdcP_avg;
extern double Is;
extern double Ecap_1, Ecap_2, Ecap_3, Ecap_4;
extern double Err_Ecap_13, Err_Ecap_1, Err_Ecap_2, Err_Ecap_3, Err_Ecap_4;
extern double Err_Ecap_13_ref, Ecap_1_ref, Ecap_2_ref, Ecap_3_ref, Ecap_4_ref;
extern double Pdc_13, Pdc_1, Pdc_2, Pdc_3, Pdc_4;
extern double Pdc_13_Integ, Pdc_1_Integ, Pdc_2_Integ, Pdc_3_Integ, Pdc_4_Integ;
extern double Ecap_1_cc_ref, Ecap_2_cc_ref, Ecap_3_cc_ref, Ecap_4_cc_ref;

extern double a, b, c, d, e, f;
extern double a_11, a_12, a_13, a_21, a_22, a_23, a_31, a_32, a_33;
extern double alpha, beta, gamma;
extern double det_G;
extern int DABPhase;
extern double DABDutySet, DABDutyDiff, DABDutyPre;
extern double DABPhase_ratio_A;
extern int DABDuty;
extern double TAB_Phase_12_ratio, TAB_Phase_13_ratio, TAB_Phase_21_ratio, TAB_Phase_23_ratio;
extern int TAB_Phase_12, TAB_Phase_13, TAB_Phase_23, TAB_Phase_21;
extern int TAB_Phase_12_legA, TAB_Phase_12_legB, TAB_Phase_13_legA, TAB_Phase_13_legB;
extern int TAB_Phase_21_legA, TAB_Phase_21_legB, TAB_Phase_23_legA, TAB_Phase_23_legB;
extern int TAB_Phase_12_legA_Dir, TAB_Phase_12_legB_Dir, TAB_Phase_13_legA_Dir, TAB_Phase_13_legB_Dir;
extern int TAB_Phase_21_legA_Dir, TAB_Phase_21_legB_Dir, TAB_Phase_23_legA_Dir, TAB_Phase_23_legB_Dir;

extern double Phase_12_ratio, Phase_13_ratio, Phase_14_ratio;
extern double Phase_12, Phase_13, Phase_14;
extern int Phase_12_count, Phase_13_count, Phase_14_count;
extern int Phase_12_legA, Phase_12_legB, Phase_13_legA, Phase_13_legB, Phase_14_legA, Phase_14_legB;
extern int Phase_11_legA_Dir, Phase_11_legB_Dir, Phase_12_legA_Dir, Phase_12_legB_Dir, Phase_13_legA_Dir, Phase_13_legB_Dir, Phase_14_legA_Dir, Phase_14_legB_Dir;

extern double duty;                                                //250805 add
extern double Dt;                                                         //250805 add

extern Uint32 cap1Value,cap2Value;                               //250412 add

extern Uint16 direction;
extern Uint16 GaDABPAUCount_A, GaDABPALCount_A, GaDABPBUCount_A, GaDABPBLCount_A;
extern Uint16 GaDABSAUCount_A, GaDABSALCount_A, GaDABSBUCount_A, GaDABSBLCount_A;
extern int DABPCount_A, DABSCount_A, DABPhase_A;
extern int DABP_Phase_A, DABS_Phase_A, DABP_Phase_Dir_A, DABS_Phase_Dir_A;

extern Uint16 GaDABPAUCount_B, GaDABPALCount_B, GaDABPBUCount_B, GaDABPBLCount_B;
extern Uint16 GaDABSAUCount_B, GaDABSALCount_B, GaDABSBUCount_B, GaDABSBLCount_B;
extern int DABPCount_B, DABSCount_B, DABPhase_B;
extern int DABP_Phase_B, DABS_Phase_B, DABP_Phase_Dir_B, DABS_Phase_Dir_B;

extern Uint16 GaDABPAUCount_C, GaDABPALCount_C, GaDABPBUCount_C, GaDABPBLCount_C;
extern Uint16 GaDABSAUCount_C, GaDABSALCount_C, GaDABSBUCount_C, GaDABSBLCount_C;
extern int DABPCount_C, DABSCount_C, DABPhase_C;
extern int DABP_Phase_C, DABS_Phase_C, DABP_Phase_Dir_C, DABS_Phase_Dir_C;
extern int flag;                                                                            //250805 add
extern double Duty_ref_PLL_P2, Duty_ref_PLL_N2;                         //250321 add
extern double Duty_ref_PLL_P, Duty_ref_PLL_N;                           //250321 add

// DAB control gain and control parameters
extern double Ldab, DABPhaseScale;
extern double Err_EcapS;
extern double PdcS_Integ, PdcS;
extern double Fc_VdcS, Wc_VdcS, Ki_VdcST, Kp_VdcS, zeta_VdcS;
extern double Fc_VdcP, Wc_VdcP, Ki_VdcPT, Kp_VdcP, zeta_VdcP;
extern double Fc_cc, Wc_cc;
extern double Kp_Pdc, Ki_Pdc;
extern double Kp_Idc, Ki_IdcT;

extern double n2, n3, n4;
extern double L1, L2, L3, L4, L2_dot, L3_dot, L4_dot, L12, L13, L14, L23, L24, L34, Ldc;
extern double TABPhaseScale_12, TABPhaseScale_13;
extern double Cdc_1, Cdc_2, Cdc_3, Cdc_4;
extern double Vdc_1_ref, Vdc_2_ref, Vdc_3_ref, Vdc_4_ref;
extern double Pdc_1_ref, Pdc_2_ref, Pdc_3_ref, Pdc_4_ref;
extern double Idc_1_Ref, Idc_2_ref, Idc_3_ref, Idc_4_ref;
extern double Vdc_1_cc_ref, Vdc_2_cc_ref, Vdc_3_cc_ref, Vdc_4_cc_ref;
extern double Idc_1_Integ, Idc_2_Integ, Idc_3_Integ, Idc_4_Integ;
extern double Err_Idc_1, Err_Idc_2, Err_Idc_3, Err_Idc_4;
extern double ff1, ff2, Sin_ff, theta_ff, G1, G2;
// user variables -->
////////////////////////////// done -->

extern int test_A, test_B, test_C;
extern int DABPhase_A_test, Flag_control;
extern int Flags_Freq;
extern double DAB_Duty_test;

extern double VacP_norm, VacS_norm;
extern double Rdab, Mdab;
extern int16 tempInt;
extern int ItmpDuty1;

extern Uint32 cnt_test;
#endif /* APPLICATION_VARIABLES_H_ */


