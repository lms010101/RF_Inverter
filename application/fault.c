////////////////////////////////////
//// fault.c                    ////
//// Created on: 2020. 2. 5.    ////
//// Author: Dong Yeup Han      ////
////////////////////////////////////
#include "F28x_Project.h"
#include "device.h"
#include "variables.h"
#include "fault.h"

FAULT Faults;

interrupt void fault(void) {
    faultManage();
    checkHWFault();
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}
void checkHWFault(void) {
    Faults.HW_Prot.bit.nPROT1 = (GpioDataRegs.GPCDAT.all & (1 << 4))>>4;                      // ADC_1_ FAULT (bit:14)
    Faults.HW_Prot.bit.nPROT2 = (GpioDataRegs.GPCDAT.all & (1 << 3))>>3;                      // ADC_2_ FAULT (bit:13)
    Faults.HW_Prot.bit.nPROT3 = (GpioDataRegs.GPCDAT.all & (1 << 2))>>2;                      // ADC_3_ FAULT (bit:12)
    Faults.HW_Prot.bit.nPROT4 = (GpioDataRegs.GPCDAT.all & (1 << 1))>>1;                      // ADC_4_ FAULT (bit:11)
    Faults.HW_Prot.bit.nPROT5 = (GpioDataRegs.GPCDAT.all & (1 << 0))>>0;                      // ADC_5_ FAULT (bit:10)
//    Faults.HW_Prot.bit.nPROT6 = (GpioDataRegs.GPBDAT.all & ((Uint32)1 << 31))>>31;            // ADC_6_ FAULT (bit:9)  //250403 deacitvate
//    Faults.HW_Prot.bit.nPROT7 = (GpioDataRegs.GPBDAT.all & ((Uint32)1 << 30))>>30;            // ADC_7_ FAULT (bit:8)  //250403 deacitvate
//    Faults.HW_Prot.bit.nPROT8 = (GpioDataRegs.GPBDAT.all & ((Uint32)1 << 29))>>29;            // ADC_8_ FAULT (bit:7)  //250403 deacitvate
//    Faults.HW_Prot.bit.nPROT9 = (GpioDataRegs.GPBDAT.all & ((Uint32)1 << 28))>>28;            // ADC_9_ FAULT (bit:6)  //250403 deacitvate
//    Faults.HW_Prot.bit.nPROT10 = (GpioDataRegs.GPBDAT.all & ((Uint32)1 << 27))>>27;           // ADC_10_ FAULT (bit:5) //250403 deacitvate

    Faults.HW_Prot.bit.nPWM1 = (GpioDataRegs.GPBDAT.all & ((Uint32)1 << 26))>>26;              // PWM_1ch_ FAULT (bit:4)
    Faults.HW_Prot.bit.nPWM2 = (GpioDataRegs.GPBDAT.all & ((Uint32)1 << 25))>>25;              // PWM_2ch_ FAULT (bit:3)
    Faults.HW_Prot.bit.nPWM3 = (GpioDataRegs.GPBDAT.all & ((Uint32)1 << 23))>>23;              // PWM_3ch_ FAULT (bit:2)
    Faults.HW_Prot.bit.nPWM4 = (GpioDataRegs.GPBDAT.all & ((Uint32)1 << 22))>>22;              // PWM_4cn_ FAULT (bit:1)
    Faults.HW_Prot.bit.nAll = (GpioDataRegs.GPBDAT.all & ((Uint32)1 << 21))>>21;               // ALL (bit:0)

    Faults.Gating_Prot1.bit.GFA = (((GpioDataRegs.GPEDAT.all & 0x00fc) >>  2) & 0x3f);         //(LSB) GFA1 : GFA6  (bit:6)
    Faults.Gating_Prot1.bit.GFB = (((GpioDataRegs.GPEDAT.all & 0xfc00) >> 10) & 0x3f);         //(LSB) GFB1 : GFA6  (bit:6)
    Faults.Gating_Prot2.bit.GFC = (((GpioDataRegs.GPBDAT.all & 0x00fc) >>  2) & 0x3f);         //(LSB) GFC1 : GFA6  (bit:6)
    Faults.Gating_Prot2.bit.GFD = (((GpioDataRegs.GPBDAT.all & 0xfc00) >> 10) & 0x3f);         //(LSB) GFD1 : GFA6  (bit:6)
}

void faultManage(void) {
      GpioDataRegs.GPCSET.bit.GPIO69 = 1;  // Gate Set #1 Disable
      GpioDataRegs.GPCSET.bit.GPIO70 = 1;  // Gate Set #2 Disable
      GpioDataRegs.GPCSET.bit.GPIO74 = 1;  // Gate Set #3 Disable
      GpioDataRegs.GPCSET.bit.GPIO75 = 1;  // Gate Set #4 Disable
      Flags.EnableGD1 = 0;
      Flags.EnableGD2 = 0;
      Flags.EnableGD3 = 0;
      Flags.EnableGD4 = 0;
      Flags.EnableStart = 0;
      Flags.EnableStart_Old = 0;
      Flags.Fault = 1;
      Flags.Fault_Old = 1;
      //DABDutySet = 0;
      EnableDABP = 0;
      EnableDABS = 0;
      Flag_VdcPCont = 0;
}


void faultCheck(void)
{
    // Software Fault
       if(fabs(Idse) > Ig_rated || fabs(Iqse) > Ig_rated)
      {
           faultManage();
       }

       ///// <-- Gating Signal Enable
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
           }
       } else {
           faultManage();
       }

       if(Flags.Reset == 1) {
           //checkHWFault();
               Faults.SW_Prot.all = 0;
               Flags.Fault = 0;
               Flags.Fault_Old = 0;
               Flags.Reset = 0;
       }
}
