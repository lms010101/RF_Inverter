/*
 * cc.h
 *
 *  Created on: 2018. 3. 25.
 *      Author: eunso
 */

#ifndef CC_H_
#define CC_H_

#include "variables.h"
#include <math.h>                                      //250304 add
#define CONTROL_PERIOD          1.0/CONTROL_FREQUENCY   /* 30usec */
#define CONTROL_FREQUENCY       100000

extern void SOGI_PLL(void);                                               //250228 add
extern void ADC_Update(void);                                             //250228 add
extern void Current_Control(void);                                        //250228 add

extern interrupt void cc_isr(void);
// Prototype statements for functions found within this file.
//extern void PLL(GRID *tmpGrid);
//extern void CurrCtrl(SYSTEM_3PH *tmp3PH, CC_3PH *tmpC);
//extern void PwmUpdate(SYSTEM_3PH *tmp3PH, CC_3PH *tmpC , volatile struct EPWM_REGS *phA, volatile struct EPWM_REGS *phB, volatile struct EPWM_REGS *phC);
//extern void ResetCC(CC_3PH *tmpC);
//extern float RefSlope(float Ref, float Ref_set, float Slope);

#define LPF(out, cutF, in, tsamp)\
    {\
        double out1;\
        out1 = out;\
        out = out1 + 2*3.141592*cutF*tsamp*(in - out1);\
    }


#endif /* CC_H_ */
