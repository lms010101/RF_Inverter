////////////////////////////////////
//// fault.h                    ////
//// Created on: 2020. 2. 5.    ////
//// Author: Dong Yeup Han      ////
////////////////////////////////////

#ifndef APPLICATION_FAULT_H_
#define APPLICATION_FAULT_H_

void faultManage(void);
void checkHWFault(void);
extern interrupt void fault(void);



typedef union
{
    Uint16 all;
    struct
    {
        unsigned int nAll            :1; // 0 bit - ALL PROT
        unsigned int reserved        :1; // 1 bit - reserved
        unsigned int nPWM4           :1; // 2 bit - PWM4
        unsigned int nPWM3           :1; // 3 bit - PWM3
        unsigned int nPWM2           :1; // 4 bit - PWM2
        unsigned int nPWM1           :1; // 5 bit - PWM1
        unsigned int nPROT10        :1; // 6 bit - nPROT10
        unsigned int nPROT9         :1; // 7 bit - nPROT9
        unsigned int nPROT8         :1; // 8 bit - nPROT8
        unsigned int nPROT7         :1; // 9 bit - nPROT7
        unsigned int nPROT6         :1; // 10 bit - nPROT6
        unsigned int nPROT5         :1; // 11 bit - nPROT5
        unsigned int nPROT4         :1; // 12 bit - nPROT4
        unsigned int nPROT3         :1; // 13 bit - nPROT3
        unsigned int nPROT2         :1; // 14 bit - nPROT2
        unsigned int nPROT1         :1; // 15 bit - nPROT1

    }bit;
}HW_Fault;

typedef union
{
    Uint16 all;
    struct
    {
        unsigned int GFA           :6; //  5:0 bit - GFA fault
        unsigned int GFB           :6; //  11:6 bit - GFB fault

        unsigned int               :4; // 31 bit - reserved

    }bit;
}Gating_Fault1;


typedef union
{
    Uint16 all;
    struct
    {
        unsigned int GFC           :6; //  5:0 bit - GFA fault
        unsigned int GFD           :6; //  11:6 bit - GFB fault

        unsigned int               :4; // 31 bit - reserved

    }bit;
}Gating_Fault2;


typedef union
{
    Uint16 all;
    struct
    {
        unsigned int OC_Idc_1         :1;
        unsigned int OC_Idc_2         :1;
        unsigned int OC_Idc_3         :1;
        unsigned int OC_Idc_4         :1;
        unsigned int OV_Vdc_1       :1;
        unsigned int OV_Vdc_2       :1;
        unsigned int OV_Vdc_3       :1;
        unsigned int OV_Vdc_4       :1;
        unsigned int OV_Eab         :1;
        unsigned int OV_Ebc         :1;
        unsigned int OV_Vdc         :1;
        unsigned int OV_VdcS         :1;
        unsigned int OV_VdcP_A            :1;
        unsigned int OV_VdcP_B            :1;
        unsigned int OV_VdcP_C        :1;
        unsigned int UV_VdcS        :1;
        unsigned int OV_Vab_grid        :1;
        unsigned int OV_Vbc_grid        :1;
        unsigned int OV_Vab_STS         :1;
        unsigned int OV_Vbc_STS         :1;
        unsigned int DV_dcHL            :1;
    }bit;
}SW_Fault;


typedef struct _FAULT_para_
{
    HW_Fault         HW_Prot;
    Gating_Fault1    Gating_Prot1;
    Gating_Fault2    Gating_Prot2;
    SW_Fault         SW_Prot;

} FAULT;

extern FAULT Faults;

#endif /* APPLICATION_FAULT_H_ */
