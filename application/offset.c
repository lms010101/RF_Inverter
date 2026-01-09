/*
 * offset.c
 *
 *  Created on: 2019. 7. 19.
 *      Author: powerelab1
 */
#include <math.h>
#include "F28x_Project.h"
#include "device.h"
#include "variables.h"
#include "cc.h"
#include "dac.h"
#include "fault.h"
#include "filter.h"

int offsetLoopCnt = 0, offsetMaxCnt = 10000;    // power of 2, maximum = 2^18

double AdcOffsetAccum[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

interrupt void offset(void)
{

    int i=0;

    // <-- fault check for booting
//    checkHWFault();
//    if(Faults.HW_Prot.bit.nAll != 0b1) {
//        Flags.Fault = 1;
//        faultManage();
//    }

    // fault check for booting -->

    daOut();  // daOut() takes about 10[us].

//    AdcOffsetAccum[0] += AdcaResultRegs.ADCRESULT0;     // ADC01              //250208 deactivate
//    AdcOffsetAccum[1] += AdcaResultRegs.ADCRESULT1;     // ADC02              //250208 deactivate
//    AdcOffsetAccum[2] += AdcaResultRegs.ADCRESULT2;     // ADC03              //250208 deactivate
//    AdcOffsetAccum[3] += AdcaResultRegs.ADCRESULT3;     // ADC04              //250208 deactivate
//    AdcOffsetAccum[4] += AdcaResultRegs.ADCRESULT4;     // ADC04              //250208 deactivate
//    AdcOffsetAccum[5] += AdcaResultRegs.ADCRESULT5;     // ADC05              //250208 deactivate
//    AdcOffsetAccum[6] += AdccResultRegs.ADCRESULT0;     // ADC06              //250208 deactivate
//    AdcOffsetAccum[7] += AdccResultRegs.ADCRESULT1;     // ADC07              //250125 deactivate
    AdcOffsetAccum[7] += AdcdResultRegs.ADCRESULT0;     // ADC07                //250125 add
//    AdcOffsetAccum[8] += AdccResultRegs.ADCRESULT2;     // ADC08              //250208 deactivate
//    AdcOffsetAccum[9] += AdccResultRegs.ADCRESULT3;     // ADC09              //250208 deactivate

    // Offset value calculation
    offsetLoopCnt++;
    if(offsetMaxCnt <= offsetLoopCnt) {
        for (i = 0; i < 10; i++) {
            AdcOffset[i] = (double)AdcOffsetAccum[i] / (double)offsetMaxCnt;
        }
         
        // For revised setup
//        AdcOffset[0]=2041.5;
//        AdcOffset[1]=2044.2;
//        AdcOffset[2]=2053.1;
//        AdcOffset[3]=2054.5;
//        AdcOffset[4]=2054.5;

        // For test setup
        // AdcOffset[0]=2038.717163;
        // AdcOffset[1]=2038.986084;
        // AdcOffset[2]=2043.898315;
        // AdcOffset[3]=2046.607178;


        EALLOW;
        PieVectTable.EPWM1_INT = &cc_isr;
        EDIS;
    }

        EPwm1Regs.ETCLR.bit.INT = 1; //clear INT flag
        PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;

}
/*------------------------------------------------------------------*/
/*         End of OFFSET.C                                          */
/*------------------------------------------------------------------*/

