/*
 * filter.c
 *
 *  Created on: 2018. 10. 16.
 *      Author: PowerElabLAPTOP1
 */

#include "filter.h"

// How to use filter
// 1. Define filter in 'filter.h.'
// 2. Define filter in 'filter.c.'
// 3. Initialize filter in 'main.c.'
// 4. Update the filter where you want.

// filter
//IIR2 LPF2_10Hz = IIR2DEFINE(K_LPF,10, 1);
IIR1 LPF1_VdcP_A = IIR1DEFINE(K_LPF,100.);
IIR1 LPF1_VdcP_B = IIR1DEFINE(K_LPF,100.);
IIR1 LPF1_VdcP_C = IIR1DEFINE(K_LPF,100.);
IIR1 LPF1_VdcS_A = IIR1DEFINE(K_LPF,100.);
IIR1 LPF1_VdcS_B = IIR1DEFINE(K_LPF,100.);
IIR1 LPF1_VdcS_C = IIR1DEFINE(K_LPF,100.);
IIR1 LPF1_Ias = IIR1DEFINE(K_LPF,600.);
IIR1 LPF1_Ibs = IIR1DEFINE(K_LPF,600.);
IIR1 LPF1_Ics = IIR1DEFINE(K_LPF,600.);
void IIR1CoeffInit(IIR1 *p_gIIR, const float dt) {
    float a0, b0, b1;
    float w0, INV_alpha;
    int type;

    // Continuous-time Filter Coefficients
    w0 = p_gIIR->w0;
    type = p_gIIR->type;

    a0 = w0;
    switch(type)
    {
        case K_LPF:
            b0 = w0;
            b1 = 0;
            break;
        case K_HPF:
            b0 = 0;
            b1 = (float)1;
            break;
        default:
        case K_ALLPASS:
            b0 = -w0;
            b1 = (float)1;
    }

    // Discrete-time Filter Coefficients
    INV_alpha = (float)1./((float)2 + dt*a0);
    p_gIIR->coeff[0] = ((float)2*b1 + dt*b0)*INV_alpha;
    p_gIIR->coeff[1] = (-(float)2*b1 + dt*b0)*INV_alpha;
    p_gIIR->coeff[2] = -(-(float)2 + dt*a0)*INV_alpha;

    return;
}

void IIR1Init(IIR1 *p_gIIR, const float dt) {
    // Initialize Filter Coefficients
    IIR1CoeffInit(p_gIIR, dt);

    // Initialize Storage Elements
    p_gIIR->reg = 0;

    return;
}

float IIR1Update(IIR1 *p_gIIR, const float x) {
    float y;

    y = p_gIIR->reg + p_gIIR->coeff[0]*x;
    p_gIIR->reg = p_gIIR->coeff[1]*x + p_gIIR->coeff[2]*y;

    return(y);
}

void IIR2CoeffInit(IIR2 *p_gIIR, const float dt) {
    float a0, a1, b0, b1, b2;
    float w0, zeta, INV_alpha;
    int type;

    // Continuous-time Filter Coefficients
    w0 = p_gIIR->w0;
    zeta = p_gIIR->zeta;
    type = p_gIIR->type;

    a0 = w0*w0;
    a1 = 2*zeta*w0;
    switch(type)
    {
        case K_LPF:
            b0 = w0*w0;
            b1 = 0;
            b2 = 0;
            break;
        case K_HPF:
            b0 = 0;
            b1 = 0;
            b2 = (float)1;
            break;
        case K_BPF:
            b0 = 0;
            b1 = (float)2*zeta*w0;
            b2 = 0;
            break;
        case K_NOTCH:
            b0 = w0*w0;
            b1 = 0;
            b2 = (float)1;
            break;
        case K_ALLPASS:
        default:
            b0 = w0*w0;
            b1 = -(float)2*zeta*w0;
            b2 = (float)1;
    }

    // Discrete-time Filter Coefficients
    INV_alpha = (float)1./((float)4 + (float)2*dt*a1 + dt*dt*a0);
    p_gIIR->coeff[0] = ((float)4*b2 + (float)2*dt*b1 + dt*dt*b0)*INV_alpha;
    p_gIIR->coeff[1] = ((float)2*dt*dt*b0 - (float)8*b2)*INV_alpha;
    p_gIIR->coeff[2] = -((float)2*dt*dt*a0 - (float)8)*INV_alpha;
    p_gIIR->coeff[3] = ((float)4*b2 - (float)2*dt*b1 + dt*dt*b0)*INV_alpha;
    p_gIIR->coeff[4] = -((float)4 - (float)2*dt*a1 + dt*dt*a0)*INV_alpha;


    return;
}

void IIR2Init(IIR2 *p_gIIR, const float dt) {
    // Initialize Filter Coefficients
    IIR2CoeffInit(p_gIIR, dt);

    // Initialize Storage Elements
    p_gIIR->reg[0] = 0;
    p_gIIR->reg[1] = 0;

    return;
}

float IIR2Update(IIR2 *p_gIIR, const float x) {
    float y;

    y = p_gIIR->reg[0] + p_gIIR->coeff[0]*x;
    p_gIIR->reg[0] = p_gIIR->reg[1] + p_gIIR->coeff[1]*x + p_gIIR->coeff[2]*y;
    p_gIIR->reg[1] = p_gIIR->coeff[3]*x + p_gIIR->coeff[4]*y;

    return(y);
}


