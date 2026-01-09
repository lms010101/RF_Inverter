/*
 * filter.h
 *
 *  Created on: 2018. 10. 16.
 *      Author: PowerElabLAPTOP1
 */

#ifndef APPLICATION_FILTER_H_
#define APPLICATION_FILTER_H_
// filter
#define K_ALLPASS   0
#define K_LPF         1
#define K_HPF         2
#define K_BPF         3
#define K_NOTCH     4
#define IIR1DEFINE(type, w0)        {(int)(type), (float)(w0), 0, 0, 0, 0}
#define IIR2DEFINE(type, w0, zeta)  {(int)(type), (float)(w0), (float)(zeta), 0, 0, 0, 0, 0, 0, 0}



// filter
typedef struct  {
    int type;
    float w0;
    float coeff[3], reg;
}   IIR1;

typedef struct  {
    int type;
    float w0, zeta;
    float coeff[5], reg[2];
}   IIR2;

extern void IIR1CoeffInit(IIR1 *p_gIIR, const float dt);
extern void IIR1Init(IIR1 *p_gIIR, const float dt);
extern float IIR1Update(IIR1 *p_gIIR, const float x);

extern void IIR2CoeffInit(IIR2 *p_gIIR, const float dt);
extern void IIR2Init(IIR2 *p_gIIR, const float dt);
extern float IIR2Update(IIR2 *p_gIIR, const float x);

//////////////////////////
// user filter define  ///
//////////////////////////
extern IIR1 LPF1_VdcP_A, LPF1_VdcP_B, LPF1_VdcP_C;
extern IIR1 LPF1_VdcS_A, LPF1_VdcS_B, LPF1_VdcS_C;
extern IIR1 LPF1_Ias, LPF1_Ibs, LPF1_Ics;

#endif /* APPLICATION_FILTER_H_ */
