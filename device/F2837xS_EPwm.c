//###########################################################################
//
// FILE:   F2837xS_EPwm.c
//
// TITLE:  F2837xS EPwm Initialization & Support Functions.
//
//###########################################################################
// $TI Release: F2837xS Support Library v3.03.00.00 $
// $Release Date: Thu Dec  7 18:53:06 CST 2017 $
// $Copyright:
// Copyright (C) 2014-2017 Texas Instruments Incorporated - http://www.ti.com/
//
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions 
// are met:
// 
//   Redistributions of source code must retain the above copyright 
//   notice, this list of conditions and the following disclaimer.
// 
//   Redistributions in binary form must reproduce the above copyright
//   notice, this list of conditions and the following disclaimer in the 
//   documentation and/or other materials provided with the   
//   distribution.
// 
//   Neither the name of Texas Instruments Incorporated nor the names of
//   its contributors may be used to endorse or promote products derived
//   from this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// $
//###########################################################################

//
// Included Files
//
#include "F2837xS_device.h"
#include "F2837xS_Examples.h"
//#include "device.h"
#include "variables.h"

#define PWM_PRD 1176                                                                     //250126 add //250702 deactivate  //250730 deactivate //250905 activate
//#define PWM_PRD 1111
//#define PWM_PRD 1163
//#define PWM_PRD 1136
//#define PWM_PRD 1063                                                                                                         //250730 add
//#define PWM_PRD 1018                                                       //250826 deactivate
//#define PWM_PRD 1018                                                         //250826 add                  //250905 deactivate
//#define PWM_PRD 1075
//#define PWM_PRD 1205

void InitEPwmRegs(void);
void initPWM(volatile struct EPWM_REGS *EPwmRegs, Uint16 periodCount, Uint16 deadbandCount, Uint16 InModeSetup, Uint16 PolSelSetup, Uint16 AQCTLBSetup, Uint16 intterruptEnable);
//void initPWMDABP(volatile struct EPWM_REGS *EPwmRegs, Uint16 periodCount, Uint16 deadbandCount, Uint16 InModeSetup, Uint16 PolSelSetup);
void initPWMDABS(volatile struct EPWM_REGS *EPwmRegs, Uint16 periodCount, Uint16 deadbandCount);

//
// InitEPwmGpio - Initialize all EPWM modules' GPIOs
//
void InitEPwmGpio(void)
{
    EALLOW;

    // Pins for GateDriverBuffer(EPWM, GD1~4)   ////////////////////////////////
    // Configure GateDriverBuffer 1 and 2
    GpioDataRegs.GPCSET.bit.GPIO69 = 1;
    GpioCtrlRegs.GPCMUX1.bit.GPIO69 = 0;    // GPIO69 = GD1_OEn
    GpioCtrlRegs.GPCDIR.bit.GPIO69 = 1;     // GPIO69 = output
    GpioCtrlRegs.GPCPUD.bit.GPIO69 = 0;

    GpioDataRegs.GPCSET.bit.GPIO70 = 1;
    GpioCtrlRegs.GPCMUX1.bit.GPIO70 = 0;    // GPIO70 = GD2_OEn
    GpioCtrlRegs.GPCDIR.bit.GPIO70 = 1;     // GPIO70 = output
    GpioCtrlRegs.GPCPUD.bit.GPIO70 = 0;

    // Configure GD1_D[7..0] and GD2_D[7..0]
    GpioCtrlRegs.GPEMUX1.all = 0x0000;      // GPIO[135..128] = GD1_D[7..0], GPIO[143..136] = GD2_D[7..0]
    GpioCtrlRegs.GPEDIR.all = (GpioCtrlRegs.GPEDIR.all & 0xffff0000) | 0x00000303;  // GPIO[129:128] = output, GPIO[137:136] = output
    GpioCtrlRegs.GPEPUD.all = (GpioCtrlRegs.GPEPUD.all | 0x0000ffff);

    // Configure GateDriverBuffer 3 and 4
    GpioDataRegs.GPCSET.bit.GPIO74 = 1;
    GpioCtrlRegs.GPCMUX1.bit.GPIO74 = 0;    // GPIO74 = GD3_OEn
    GpioCtrlRegs.GPCDIR.bit.GPIO74 = 1;     // GPIO74 = output
    GpioCtrlRegs.GPCPUD.bit.GPIO74 = 0;

    GpioDataRegs.GPCSET.bit.GPIO75 = 1;
    GpioCtrlRegs.GPCMUX1.bit.GPIO75 = 0;    // GPIO75 = GD4_OEn
    GpioCtrlRegs.GPCDIR.bit.GPIO75 = 1;     // GPIO75 = output
    GpioCtrlRegs.GPCPUD.bit.GPIO75 = 0;

    // Configure GD3_D[7..0] and GD4_D[7..0]
    GpioCtrlRegs.GPBMUX1.all = 0x0000;      // GPIO[39..32] = GD3_D[7..0], GPIO[47..40] = GD4_D[7..0]
    GpioCtrlRegs.GPBDIR.all = (GpioCtrlRegs.GPBDIR.all & 0xffff0000) | 0x00000303;  // GPIO[33:32] = output, GPIO[41:40] = output
    GpioCtrlRegs.GPBPUD.all = (GpioCtrlRegs.GPBPUD.all | 0x0000ffff);


    GpioCtrlRegs.GPEPUD.bit.GPIO145 = 1;    // Disable pull-up on GPIO145 (EPWM1A)
    GpioCtrlRegs.GPEPUD.bit.GPIO146 = 1;    // Disable pull-up on GPIO146 (EPWM1B)
    GpioCtrlRegs.GPEPUD.bit.GPIO147 = 1;    // Disable pull-up on GPIO147 (EPWM2A)
    GpioCtrlRegs.GPEPUD.bit.GPIO148 = 1;    // Disable pull-up on GPIO148 (EPWM2B)
    GpioCtrlRegs.GPEPUD.bit.GPIO149 = 1;    // Disable pull-up on GPIO149 (EPWM3A)
    GpioCtrlRegs.GPEPUD.bit.GPIO150 = 1;    // Disable pull-up on GPIO150 (EPWM3B)

    GpioCtrlRegs.GPEMUX2.bit.GPIO145 = 1;   // Configure GPIO145 as EPWM1A
    GpioCtrlRegs.GPEMUX2.bit.GPIO146 = 1;   // Configure GPIO146 as EPWM1B
    GpioCtrlRegs.GPEMUX2.bit.GPIO147 = 1;   // Configure GPIO147 as EPWM2A
    GpioCtrlRegs.GPEMUX2.bit.GPIO148 = 1;   // Configure GPIO148 as EPWM2B
    GpioCtrlRegs.GPEMUX2.bit.GPIO149 = 1;   // Configure GPIO149 as EPWM3A
    GpioCtrlRegs.GPEMUX2.bit.GPIO150 = 1;   // Configure GPIO150 as EPWM3B

    GpioCtrlRegs.GPEDIR.bit.GPIO151 = 1;                                                //250326 add(gpio)    //250716 deactivate
//    GpioCtrlRegs.GPEPUD.bit.GPIO151 = 1;    // Disable pull-up on GPIO151 (EPWM4A)    //250326 deactivate       //250716 activate
    GpioCtrlRegs.GPEPUD.bit.GPIO151 = 0;                                                //250326 add(gpio)    //250716 deactivate
//    GpioCtrlRegs.GPEPUD.bit.GPIO152 = 1;    // Disable pull-up on GPIO152 (EPWM4B)       //251027 deactivate
    GpioCtrlRegs.GPEDIR.bit.GPIO152 = 1;                                                   //251027 add
    GpioCtrlRegs.GPEPUD.bit.GPIO152 = 0;                                                   //251027 add
    GpioCtrlRegs.GPEPUD.bit.GPIO153 = 1;    // Disable pull-up on GPIO153 (EPWM5A)
    GpioCtrlRegs.GPEPUD.bit.GPIO154 = 1;    // Disable pull-up on GPIO154 (EPWM5B)
    GpioCtrlRegs.GPEPUD.bit.GPIO155 = 1;    // Disable pull-up on GPIO155 (EPWM6A)
    GpioCtrlRegs.GPEPUD.bit.GPIO156 = 1;    // Disable pull-up on GPIO156 (EPWM6B)

//    GpioCtrlRegs.GPEMUX2.bit.GPIO151 = 1;   // Configure GPIO151 as EPWM4A                //250325 deactivate //250716 activate
    GpioCtrlRegs.GPEMUX2.bit.GPIO151 = 0;   // Configure GPIO151 as EPWM4A              //250326 add(gpio)    //250716 deactivate
    GpioCtrlRegs.GPEMUX2.bit.GPIO152 = 1;   // Configure GPIO152 as EPWM4B
    GpioCtrlRegs.GPEMUX2.bit.GPIO153 = 1;   // Configure GPIO153 as EPWM5A
    GpioCtrlRegs.GPEMUX2.bit.GPIO154 = 1;   // Configure GPIO154 as EPWM5B
    GpioCtrlRegs.GPEMUX2.bit.GPIO155 = 1;   // Configure GPIO155 as EPWM6A
    GpioCtrlRegs.GPEMUX2.bit.GPIO156 = 1;   // Configure GPIO156 as EPWM6B


    GpioCtrlRegs.GPEPUD.bit.GPIO157 = 1;    // Disable pull-up on GPIO157 (EPWM7A)
    GpioCtrlRegs.GPEPUD.bit.GPIO158 = 1;    // Disable pull-up on GPIO158 (EPWM7B)
    GpioCtrlRegs.GPEPUD.bit.GPIO159 = 1;    // Disable pull-up on GPIO159 (EPWM8A)
    GpioCtrlRegs.GPFPUD.bit.GPIO160 = 1;    // Disable pull-up on GPIO160 (EPWM8B)
    GpioCtrlRegs.GPFPUD.bit.GPIO161 = 1;    // Disable pull-up on GPIO161 (EPWM9A)
    GpioCtrlRegs.GPFPUD.bit.GPIO162 = 1;    // Disable pull-up on GPIO162 (EPWM9B)

    GpioCtrlRegs.GPEMUX2.bit.GPIO157 = 1;   // Configure GPIO157 as EPWM7A
    GpioCtrlRegs.GPEMUX2.bit.GPIO158 = 1;   // Configure GPIO158 as EPWM7B
    GpioCtrlRegs.GPEMUX2.bit.GPIO159 = 1;   // Configure GPIO159 as EPWM8A
    GpioCtrlRegs.GPFMUX1.bit.GPIO160 = 1;   // Configure GPIO160 as EPWM8B
    GpioCtrlRegs.GPFMUX1.bit.GPIO161 = 1;   // Configure GPIO161 as EPWM9A
    GpioCtrlRegs.GPFMUX1.bit.GPIO162 = 1;   // Configure GPIO162 as EPWM9B


    GpioCtrlRegs.GPFPUD.bit.GPIO163 = 1;    // Disable pull-up on GPIO163 (EPWM10A)
    GpioCtrlRegs.GPFPUD.bit.GPIO164 = 1;    // Disable pull-up on GPIO164 (EPWM10B)
    GpioCtrlRegs.GPFPUD.bit.GPIO165 = 1;    // Disable pull-up on GPIO165 (EPWM11A)
    GpioCtrlRegs.GPFPUD.bit.GPIO166 = 1;    // Disable pull-up on GPIO166 (EPWM11B)
    GpioCtrlRegs.GPFPUD.bit.GPIO167 = 1;    // Disable pull-up on GPIO167 (EPWM12A)
    GpioCtrlRegs.GPFPUD.bit.GPIO168 = 1;    // Disable pull-up on GPIO168 (EPWM12B)

    GpioCtrlRegs.GPFMUX1.bit.GPIO163 = 1;   // Configure GPIO163 as EPWM10A
    GpioCtrlRegs.GPFMUX1.bit.GPIO164 = 1;   // Configure GPIO164 as EPWM10B
    GpioCtrlRegs.GPFMUX1.bit.GPIO165 = 1;   // Configure GPIO165 as EPWM11A
    GpioCtrlRegs.GPFMUX1.bit.GPIO166 = 1;   // Configure GPIO166 as EPWM11B
    GpioCtrlRegs.GPFMUX1.bit.GPIO167 = 1;   // Configure GPIO167 as EPWM12A
    GpioCtrlRegs.GPFMUX1.bit.GPIO168 = 1;   // Configure GPIO168 as EPWM12B

    // Configure PWM SYNC in setting INput
    GpioCtrlRegs.GPAGMUX1.bit.GPIO3 = 0;
    GpioCtrlRegs.GPAMUX1.bit.GPIO3  = 0;    // GPIO3 = Sync in
    GpioCtrlRegs.GPADIR.bit.GPIO3   = 0;    // GPIO3 =  input
    GpioCtrlRegs.GPAPUD.bit.GPIO3   = 0;
    InputXbarRegs.INPUT5SELECT = 3;    // 1. Input X bar ����  // GPIO053

    // Configure PWM SYNC Output setting Output
    GpioCtrlRegs.GPAGMUX1.bit.GPIO6 = 0;    // GPIO6 = Sync out
    GpioCtrlRegs.GPAMUX1.bit.GPIO6  = 3;
    GpioCtrlRegs.GPADIR.bit.GPIO6   = 1;      // GPIO6 =  output

    EDIS;

    InitEPwmRegs();
}

void InitEPwmRegs(void)
{

//    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 0;                                                              //250201 deactivate

    EALLOW;                                                                                              //250126 add
    EPwm8Regs.TBCTL.bit.FREE_SOFT=2;                                 //250424 add
    EPwm8Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;                                                     //250412 add     //250503 deactivate
//    EPwm8Regs.TBPRD = 1176;                                                                            //250412 add  //250503 deactivate //250730 deactivate
    EPwm8Regs.TBPRD = PWM_PRD;                                                                                            //250730 add
    EPwm8Regs.TBCTR = 0;                                                                                 //250412 add
    EPwm8Regs.CMPA.bit.CMPA = PWM_PRD/2;                                                                 //250412 add
    EPwm8Regs.CMPB.bit.CMPB = PWM_PRD/2;                                                                 //250412 add
    EPwm8Regs.TBPHS.bit.TBPHS = 0x0000;             // Phase is 0                                        //250412 add
    EPwm8Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;  // Count up and douwn                                //250412 add
    EPwm8Regs.TBCTL.bit.PHSEN = TB_DISABLE;         // Disable phase loading                             //250412 add
    EPwm8Regs.TBCTL.bit.PRDLD = TB_SHADOW;                                                               //250412 add
    EPwm8Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;        // Clock ratio to SYSCLKOUT                          //250412 add
    EPwm8Regs.TBCTL.bit.CLKDIV = TB_DIV1;                                                                //250412 add
//    EPwm8Regs.TBCTL.bit.SYNCOSEL = 0;                                                                    //250412 add
//    EPwm8Regs.TBCTL.bit.SYNCOSEL = 1;                                                                    //250412 add  //250417 deactivate
    EPwm8Regs.TBCTL.bit.SYNCOSEL = 2;                                                                    //250417 add

    // Setup shadowing                                                                                   //250412 add
    EPwm8Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;                                                          //250412 add
    EPwm8Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;                                                          //250412 add
    EPwm8Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;   // Load on Zero                                      //250412 add
    EPwm8Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;                                                        //250412 add

    // Set actions                                                                                       //250412 add
    EPwm8Regs.AQCTLA.bit.CAU = AQ_SET;              // Set PWM7A on event A, up count                    //250412 add
    EPwm8Regs.AQCTLA.bit.CAD = AQ_CLEAR;            // Clear PWM7A on event A, down count                //250412 add
//    EPwm8Regs.AQCTLB.bit.CBU = AQ_CLEAR;            // Set PWM7B on event B, up count                    //250412 add  //250515 deactivate
//    EPwm8Regs.AQCTLB.bit.CBD = AQ_SET;              // Clear PWM7B on event B, down count                //250412 add  //250515 deactivate

    //Setup Deadband                                                                                     //250412 add
    EPwm8Regs.DBCTL.bit.OUT_MODE = 0x3;                                                                  //250412 add
    EPwm8Regs.DBCTL.bit.POLSEL = 0x2;                                                                    //250412 add //250509 deactivate
//    EPwm8Regs.DBCTL.bit.POLSEL = 0x3;                                                                                   //250509 add
//    EPwm8Regs.DBCTL.bit.IN_MODE = 0x3;                                                                   //250412 add   //250515 deactivate
    EPwm8Regs.DBCTL.bit.IN_MODE = 0x0;                                                                                    //250515 add
    EPwm8Regs.DBCTL.bit.OUTSWAP = 0x0;                                                                   //250412 add
    EPwm8Regs.DBCTL.bit.DEDB_MODE  = 0x0;                                                                //250412 add
//    EPwm7Regs.DBRED.all = 100;                                                                           //250412 add
//    EPwm7Regs.DBFED.all = 100;                                                                           //250412 add
//    EPwm8Regs.DBRED.all = 0;                                                                                           //250509 add //250515 deactivate
//    EPwm8Regs.DBFED.all = 0;                                                                                           //250509 add //250515 deactivate
    EPwm8Regs.DBCTL.bit.SHDWDBFEDMODE = 1;                                                                               //250515 add
    EPwm8Regs.DBCTL.bit.SHDWDBREDMODE = 1;                                                                               //250515 add
    EPwm8Regs.DBCTL.bit.LOADFEDMODE = 2;                                                                               //250515 add
    EPwm8Regs.DBCTL.bit.LOADREDMODE = 2;                                                                               //250515 add
//    EPwm8Regs.DBRED.all = 80;                                                                                             //250515 add //250714 deactivate
//    EPwm8Regs.DBFED.all = 80;                                                                                             //250515 add //250714 deactivate
//    EPwm8Regs.DBRED.all = 150;                            //250811 deactivate                                                                      //250515 add //250714 deactivate
    EPwm8Regs.DBRED.all = 110;                              //250811 add          //250821 deactivate
//    EPwm8Regs.DBRED.all = 50;
//    EPwm8Regs.DBFED.all = 150;                            //250811 deactivate
    EPwm8Regs.DBFED.all = 110;                              //250811 add          //250821 deactivate
//    EPwm8Regs.DBFED.all = 50;
//    EPwm8Regs.GLDCTL.bit.GLD = 1;                             //250703 add
//    EPwm8Regs.GLDCTL.bit.OSHTMODE = 1;                        //250703 add
//    EPwm8Regs.GLDCTL2.bit.OSHTLD = 1;                         //250703 add

    EPwm9Regs.TBCTL.bit.FREE_SOFT=2;                                                                      //250511 add
    EPwm9Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;                                                        //250511 add
//    EPwm9Regs.TBPRD = 1176;                                                                               //250511 add        //250730 deactivate
    EPwm9Regs.TBPRD = PWM_PRD;                                                                                                     //250730 add
    EPwm9Regs.TBCTR = 0;                                                                                  //250511 add
    EPwm9Regs.CMPA.bit.CMPA = PWM_PRD/2;                                                                  //250511 add
    EPwm9Regs.CMPB.bit.CMPB = PWM_PRD/2;                                                                  //250511 add
    EPwm9Regs.TBPHS.bit.TBPHS = 0x0000;                                                                   //250511 add      //251019 deactivate
    EPwm9Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;                                                        //250511 add
    EPwm9Regs.TBCTL.bit.PHSEN = TB_DISABLE;                                                               //250511 add
    EPwm9Regs.TBCTL.bit.PRDLD = TB_SHADOW;                                                                //250511 add
    EPwm9Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;                                                              //250511 add
    EPwm9Regs.TBCTL.bit.CLKDIV = TB_DIV1;                                                                 //250511 add
    EPwm9Regs.TBCTL.bit.SYNCOSEL = 2;                                                                     //250511 add

    // Setup shadowing
    EPwm9Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;                                                           //250511 add
    EPwm9Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;                                                           //250511 add
    EPwm9Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;   // Load on Zero                                       //250511 add
    EPwm9Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;                                                         //250511 add

    // Set actions
    EPwm9Regs.AQCTLA.bit.CAU = AQ_SET;                                                                    //250511 add
    EPwm9Regs.AQCTLA.bit.CAD = AQ_CLEAR;                                                                  //250511 add
    EPwm9Regs.AQCTLB.bit.CBU = AQ_CLEAR;                                                                  //250511 add  //250515 deactivate
    EPwm9Regs.AQCTLB.bit.CBD = AQ_SET;                                                                    //250511 add  //250515 deactivate

    //Setup Deadband
    EPwm9Regs.DBCTL.bit.OUT_MODE = 0x3;                                                                   //250511 add
    EPwm9Regs.DBCTL.bit.POLSEL = 0x2;                                                                     //250511 add
//    EPwm9Regs.DBCTL.bit.IN_MODE = 0x3;                                                                    //250511 add
    EPwm9Regs.DBCTL.bit.IN_MODE = 0x0;                                                                                      //250515 add
    EPwm9Regs.DBCTL.bit.OUTSWAP = 0x0;                                                                    //250511 add
    EPwm9Regs.DBCTL.bit.DEDB_MODE  = 0x0;                                                                 //250511 add
//    EPwm9Regs.DBRED.all = 0;                                                                              //250511 add //250515 deactivate
//    EPwm9Regs.DBFED.all = 0;                                                                              //250511 add //250515 deactivate
    EPwm9Regs.DBCTL.bit.SHDWDBFEDMODE = 1;                                                                               //250515 add
    EPwm9Regs.DBCTL.bit.SHDWDBREDMODE = 1;                                                                               //250515 add
    EPwm9Regs.DBCTL.bit.LOADFEDMODE = 2;                                                                                 //250515 add
    EPwm9Regs.DBCTL.bit.LOADREDMODE = 2;                                                                                 //250515 add
//    EPwm9Regs.DBRED.all = 80;                                                                              //250515 add  //250714 deactivate
//    EPwm9Regs.DBFED.all = 80;                                                                              //250515 add  //250714 deactivate
//    EPwm9Regs.DBRED.all = 150;             //250811 deactivate
    EPwm9Regs.DBRED.all = 110;               //250811 add                //250821 deactivate
//    EPwm9Regs.DBRED.all = 50;                                              //250821 add
//    EPwm9Regs.DBFED.all = 150;             //250811 deactivate
    EPwm9Regs.DBFED.all = 110;               //250811 add                //250821 deactivate
//    EPwm9Regs.DBFED.all = 50;                                              //250821 add
//    EPwm9Regs.GLDCTL.bit.GLD = 1;                             //250703 add
//    EPwm9Regs.GLDCTL.bit.OSHTMODE = 1;                        //250703 add
//    EPwm9Regs.GLDCTL2.bit.OSHTLD = 1;                         //250703 add

//    initPWM(&EPwm1Regs, PWMMaxCount, EPWM_DB, DBA_ALL, DB_ACTV_HIC, 1, 1);
//    initPWM(&EPwm2Regs, PWMMaxCount, EPWM_DB, DBA_ALL, DB_ACTV_HIC, 1, 0);
//    initPWM(&EPwm3Regs, PWMMaxCount, EPWM_DB, DBA_ALL, DB_ACTV_HIC, 1, 0);
//    initPWM(&EPwm4Regs, PWMMaxCount, EPWM_DB, DBA_ALL, DB_ACTV_HIC, 1, 0);
//    initPWM(&EPwm5Regs, PWMMaxCount, EPWM_DB, DBA_ALL, DB_ACTV_HIC, 1, 0);
//    initPWM(&EPwm6Regs, PWMMaxCount, EPWM_DB, DBA_ALL, DB_ACTV_HIC, 1, 0);
//    initPWM(&EPwm7Regs, PWMMaxCount, EPWM_DB, DBA_ALL, DB_ACTV_HIC, 1, 0);
//    initPWM(&EPwm8Regs, PWMMaxCount, EPWM_DB, DBA_ALL, DB_ACTV_HIC, 1, 0);
//    initPWM(&EPwm9Regs, PWMMaxCount, EPWM_DB, DBA_ALL, DB_ACTV_HIC, 1, 0);
//    initPWM(&EPwm10Regs, PWMMaxCount, EPWM_DB, DBA_ALL, DB_ACTV_HIC, 1, 0);
//    initPWM(&EPwm11Regs, PWMMaxCount, EPWM_DB, DBA_ALL, DB_ACTV_HIC, 1, 0);
//    initPWM(&EPwm12Regs, PWMMaxCount, EPWM_DB, DBA_ALL, DB_ACTV_HIC, 1, 0);

    initPWMDABS(&EPwm1Regs, PWMMaxCount, EPWM_DB);
    initPWMDABS(&EPwm2Regs, PWMMaxCount, EPWM_DB);


//    initPWMDABS(&EPwm3Regs, PWMMaxCount, EPWM_DB);                             //250126 deactivate   //250321 activate  //250322 deactivate
//    initPWMDABS(&EPwm4Regs, PWMMaxCount, EPWM_DB);                             //250126 deactivate
//    initPWMDABS(&EPwm5Regs, PWMMaxCount, EPWM_DB);                             //250126 deactivate
//    initPWMDABS(&EPwm6Regs, PWMMaxCount, EPWM_DB);                             //250126 deactivate
//    initPWMDABS(&EPwm7Regs, PWMMaxCount, EPWM_DB);                             //250126 deactivate    //250304 activate  //250304 deactivate
//    initPWMDABS(&EPwm8Regs, PWMMaxCount, EPWM_DB);                             //250126 deactivate
//    initPWMDABS(&EPwm9Regs, PWMMaxCount, EPWM_DB);                             //250126 deactivate
//    initPWMDABS(&EPwm10Regs, PWMMaxCount, EPWM_DB);                            //250126 deactivate
//    initPWMDABS(&EPwm11Regs, PWMMaxCount, EPWM_DB);                            //250126 deactivate
//    initPWMDABS(&EPwm12Regs, PWMMaxCount, EPWM_DB);                            //250126 deactivate

    EALLOW;
    EPwm1Regs.TBCTL.bit.PHSEN = TB_ENABLE;        // Enable phase loading
    EPwm1Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO;    //  Sync out source : time base counter equals to zero  (TBCTR == 0x0000)
    //EPwm1Regs.TBCTL.bit.SWFSYNC = 0x0;

    EPwm1Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;            // Select INT on Period                <-- Single sampling
    EPwm1Regs.ETSEL.bit.INTEN = 1;       // Enable INT
    EPwm1Regs.ETPS.bit.INTPRD = ET_3RD;                 // Generate INT on every event

    // <-- Sync Setting
    SyncSocRegs.SYNCSELECT.bit.SYNCOUT = 0x00;     //Select SyncOut Source : EPWM1SYNCOUT Selected
    SyncSocRegs.SYNCSELECT.bit.EPWM4SYNCIN = 0x00; //Select EPWM4 SyncIN Source :  EPWM1SYNCOUT Selected
//    SyncSocRegs.SYNCSELECT.bit.EPWM7SYNCIN = 0x00; //Select EPWM7 SyncIN Source :  EPWM1SYNCOUT Selected     //250126 deactivate
    SyncSocRegs.SYNCSELECT.bit.EPWM10SYNCIN = 0x00; //Select EPWM10 SyncIN Source :  EPWM1SYNCOUT Selected
    // Sync Setting -->

    EPwm1Regs.ETSEL.bit.SOCASEL = ET_CTR_PRD; // SOCA on PRD or ZERO
    EPwm1Regs.ETPS.bit.SOCAPRD = ET_1ST;          // Generate SOCA on every event
    EPwm1Regs.ETPS.bit.SOCPSSEL = 0;
    EPwm1Regs.ETSEL.bit.SOCAEN = 1;  //enable SOCA

//    EALLOW;
    ClkCfgRegs.PERCLKDIVSEL.bit.EPWMCLKDIV = 0;
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 1;
    EDIS;
}


// initEPWM - Configure ePWM
void initPWM(volatile struct EPWM_REGS *EPwmRegs, Uint16 periodCount, Uint16 deadbandCount, Uint16 InModeSetup, Uint16 PolSelSetup, Uint16 AQCTLBSetup, Uint16 intterruptEnable)
{
    EPwmRegs->TBPRD = periodCount;                // Set timer period
    EPwmRegs->TBPHS.bit.TBPHS = 0x0000;           // Phase is 0
    EPwmRegs->TBCTR = 0x0000;                     // Clear counter

    //
    // Setup TBCLK
    //
    EPwmRegs->TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Count up-down
    EPwmRegs->TBCTL.bit.PHSEN = TB_ENABLE;        // Enable phase loading
    EPwmRegs->TBPHS.bit.TBPHS = 0x00;
    EPwmRegs->TBCTL.bit.HSPCLKDIV = TB_DIV1;       // Clock ratio to SYSCLKOUT
    EPwmRegs->TBCTL.bit.CLKDIV = TB_DIV1;

    EPwmRegs->CMPCTL.bit.SHDWAMODE = CC_CTR_ZERO;    // Load registers every ZERO
    EPwmRegs->CMPCTL.bit.SHDWBMODE = CC_CTR_ZERO;
    EPwmRegs->CMPCTL.bit.LOADAMODE = CC_CTR_ZERO_PRD;
    EPwmRegs->CMPCTL.bit.LOADBMODE = CC_CTR_ZERO_PRD;

    //
    // Setup compare
    //
    EPwmRegs->CMPA.bit.CMPA = periodCount>>1;   // Half-duty

    //
    // Set actions
    //
    EPwmRegs->AQCTL.bit.LDAQAMODE = 0x02;
    EPwmRegs->AQCTL.bit.LDAQBMODE = 0x02;
    EPwmRegs->AQCTL.bit.SHDWAQAMODE = 0x01;
    EPwmRegs->AQCTL.bit.SHDWAQBMODE = 0x01;


    EPwmRegs->AQCTLA.bit.CAU = AQ_CLEAR; // Action When TBCTR = CMPA on Up Count
    EPwmRegs->AQCTLA.bit.CAD = AQ_SET;

//    if(AQCTLBSetup == 1) {
//        EPwmRegs->AQCTLB.bit.CBU = AQ_CLEAR;
//        EPwmRegs->AQCTLB.bit.CBD = AQ_SET;
//    } else {
//        EPwmRegs->AQCTLB.bit.CAU = AQ_CLEAR;
//        EPwmRegs->AQCTLB.bit.CAD = AQ_SET;
//    }

    //
    // Active High Complementary - Setup Deadband
    //
    EPwmRegs->DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
    EPwmRegs->DBCTL.bit.POLSEL = PolSelSetup;
    EPwmRegs->DBCTL.bit.IN_MODE = InModeSetup;
    EPwmRegs->DBRED.bit.DBRED = deadbandCount;
    EPwmRegs->DBFED.bit.DBFED = deadbandCount;

    //
    // Set interrupt
    //
    EPwmRegs->ETSEL.bit.INTSEL = ET_CTR_ZERO;      // Select INT on Period and Zero event <-- Double sampling
//    EPwmRegs->ETSEL.bit.INTSEL = ET_CTR_PRD;            // Select INT on Period                <-- Single sampling
    EPwmRegs->ETSEL.bit.INTEN = intterruptEnable;       // Enable INT
    EPwmRegs->ETPS.bit.INTPRD = ET_1ST;                 // Generate INT on every event

}

void initPWMDABS(volatile struct EPWM_REGS *EPwmRegs, Uint16 periodCount, Uint16 deadbandCount)
{
    EPwmRegs->TBPRD = periodCount;                // Set timer period
    EPwmRegs->TBPHS.bit.TBPHS = 0x0000;           // Phase is 0
    EPwmRegs->TBCTR = 0x0000;                     // Clear counter

    //
    // Setup TBCLK
    //
    EPwmRegs->TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Count up-down
    EPwmRegs->TBCTL.bit.PHSEN = TB_ENABLE;        // Enable phase loading
    EPwmRegs->TBPHS.bit.TBPHS = 0x00;
    EPwmRegs->TBCTL.bit.HSPCLKDIV = TB_DIV1;       // Clock ratio to SYSCLKOUT
    EPwmRegs->TBCTL.bit.CLKDIV = TB_DIV1;

    EPwmRegs->CMPCTL.bit.SHDWAMODE = CC_SHADOW;    // Load registers every ZERO
    EPwmRegs->CMPCTL.bit.SHDWBMODE = CC_SHADOW;
    EPwmRegs->CMPCTL.bit.LOADAMODE = CC_CTR_ZERO_PRD;
    EPwmRegs->CMPCTL.bit.LOADBMODE = CC_CTR_ZERO_PRD;

    //
    // Setup compare
    //
    EPwmRegs->CMPA.bit.CMPA = periodCount>>1;   // Half-duty

    //
    // Set actions
    //
    EPwmRegs->AQCTL.bit.LDAQAMODE = 0x02;
    EPwmRegs->AQCTL.bit.LDAQBMODE = 0x02;
    EPwmRegs->AQCTL.bit.SHDWAQAMODE = 0x01;
    EPwmRegs->AQCTL.bit.SHDWAQBMODE = 0x01;

    EPwmRegs->AQCTLA.bit.CAU = AQ_CLEAR; // Action When TBCTR = CMPA on Up Count
    EPwmRegs->AQCTLA.bit.CAD = AQ_SET;

    // EPwmRegs->AQCTLA.bit.CBU = AQ_CLEAR; // Action When TBCTR = CMPA on Up Count
    // EPwmRegs->AQCTLA.bit.CAD = AQ_SET;

    //EPwmRegs->AQCTLB.bit.CAU = AQ_CLEAR;
    //EPwmRegs->AQCTLB.bit.CBD = AQ_SET;

    // Active High Complementary - Setup Deadband
    EPwmRegs->DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
    EPwmRegs->DBCTL.bit.POLSEL = DB_ACTV_HIC;
    EPwmRegs->DBCTL.bit.IN_MODE = DBA_ALL;
    EPwmRegs->DBRED.bit.DBRED = deadbandCount;
    EPwmRegs->DBFED.bit.DBFED = deadbandCount;

    //
    // Set interrupt
    //
 //   EPwmRegs->ETSEL.bit.INTSEL = ET_CTR_ZERO;      // Select INT on Period and Zero event <-- Double sampling
//        EPwmRegs->ETSEL.bit.INTSEL = ET_CTR_PRD;            // Select INT on Period                <-- Single sampling
//    EPwmRegs->ETSEL.bit.INTEN = 1;       // Enable INT
 //  EPwmRegs->ETPS.bit.INTPRD = ET_2ND;
}
/*
//
// InitEPwm1Gpio - Initialize EPWM1 GPIOs
//
void InitEPwm1Gpio(void)
{
    EALLOW;

    //
    // Disable internal pull-up for the selected output pins
    // for reduced power consumption
    // Pull-ups can be enabled or disabled by the user.
    // Comment out other unwanted lines.
    //
    GpioCtrlRegs.GPEPUD.bit.GPIO145 = 1;    // Disable pull-up on GPIO145 (EPWM1A)
    GpioCtrlRegs.GPEPUD.bit.GPIO146 = 1;    // Disable pull-up on GPIO146 (EPWM1B)

    //
    // Configure EPWM-1 pins using GPIO regs
    // This specifies which of the possible GPIO pins will be EPWM1 functional
    // pins.
    // Comment out other unwanted lines.
    //
   GpioCtrlRegs.GPEMUX2.bit.GPIO145 = 1;   // Configure GPIO145 as EPWM1A
    GpioCtrlRegs.GPEMUX2.bit.GPIO146 = 1;   // Configure GPIO146 as EPWM1B

    EDIS;
}

//
// InitEPwm2Gpio - Initialize EPWM2 GPIOs
//
void InitEPwm2Gpio(void)
{
    EALLOW;

    //
    // Disable internal pull-up for the selected output pins
    // for reduced power consumption
    // Pull-ups can be enabled or disabled by the user.
    // This will enable the pullups for the specified pins.
    // Comment out other unwanted lines.
    //
     GpioCtrlRegs.GPEPUD.bit.GPIO147 = 1;    // Disable pull-up on GPIO147 (EPWM2A)
    GpioCtrlRegs.GPEPUD.bit.GPIO148 = 1;    // Disable pull-up on GPIO148 (EPWM2B)

    //
    // Configure EPwm-2 pins using GPIO regs
    // This specifies which of the possible GPIO pins will be EPWM2 functional pins.
    // Comment out other unwanted lines.
    //
    GpioCtrlRegs.GPEMUX2.bit.GPIO147 = 1;   // Configure GPIO147 as EPWM2A
    GpioCtrlRegs.GPEMUX2.bit.GPIO148 = 1;   // Configure GPIO148 as EPWM2B

    EDIS;
}

//
// InitEPwm3Gpio - Initialize EPWM3 GPIOs
//
void InitEPwm3Gpio(void)
{
    EALLOW;

    //
    // Disable internal pull-up for the selected output pins
    //   for reduced power consumption
    // Pull-ups can be enabled or disabled by the user.
    // This will enable the pullups for the specified pins.
    // Comment out other unwanted lines.
    //
    GpioCtrlRegs.GPEPUD.bit.GPIO149 = 1;    // Disable pull-up on GPIO149 (EPWM3A)
    GpioCtrlRegs.GPEPUD.bit.GPIO150 = 1;    // Disable pull-up on GPIO150 (EPWM3B)

    //
    // Configure EPwm-3 pins using GPIO regs
    // This specifies which of the possible GPIO pins will be EPWM3 functional pins.
    // Comment out other unwanted lines.
    //
    GpioCtrlRegs.GPEMUX2.bit.GPIO149 = 1;   // Configure GPIO149 as EPWM3A
    GpioCtrlRegs.GPEMUX2.bit.GPIO150 = 1;   // Configure GPIO150 as EPWM3B

    EDIS;
}

//
// InitEPwm4Gpio - Initialize EPWM4 GPIOs
//
void InitEPwm4Gpio(void)
{
    EALLOW;

    //
    // Disable internal pull-up for the selected output pins
    //   for reduced power consumption
    // Pull-ups can be enabled or disabled by the user.
    // This will enable the pullups for the specified pins.
    // Comment out other unwanted lines.
    //
    GpioCtrlRegs.GPEPUD.bit.GPIO151 = 1;    // Disable pull-up on GPIO151 (EPWM4A)
    GpioCtrlRegs.GPEPUD.bit.GPIO152 = 1;    // Disable pull-up on GPIO152 (EPWM4B)

    //
    // Configure EPWM-4 pins using GPIO regs
    // This specifies which of the possible GPIO pins will be EPWM4 functional
    // pins.
    // Comment out other unwanted lines.
    //
    GpioCtrlRegs.GPEMUX2.bit.GPIO151 = 1;   // Configure GPIO151 as EPWM4A
    GpioCtrlRegs.GPEMUX2.bit.GPIO152 = 1;   // Configure GPIO152 as EPWM4B

    EDIS;
}

//
// InitEPwm5Gpio - Initialize EPWM5 GPIOs
//
void InitEPwm5Gpio(void)
{
    EALLOW;
    //
    // Disable internal pull-up for the selected output pins
    //   for reduced power consumption
    // Pull-ups can be enabled or disabled by the user.
    // This will enable the pullups for the specified pins.
    // Comment out other unwanted lines.
    //
    GpioCtrlRegs.GPEPUD.bit.GPIO153 = 1;    // Disable pull-up on GPIO153 (EPWM5A)
    GpioCtrlRegs.GPEPUD.bit.GPIO154 = 1;    // Disable pull-up on GPIO154 (EPWM5B)

    //
    // Configure EPWM-5 pins using GPIO regs
    // This specifies which of the possible GPIO pins will be EPWM5 functional
    // pins.
    // Comment out other unwanted lines.
    //
   GpioCtrlRegs.GPEMUX2.bit.GPIO153 = 1;   // Configure GPIO153 as EPWM5A
    GpioCtrlRegs.GPEMUX2.bit.GPIO154 = 1;   // Configure GPIO0154 as EPWM5B

    EDIS;
}

//
// InitEPwm6Gpio - Initialize EPWM6 GPIOs
//
void InitEPwm6Gpio(void)
{
    EALLOW;
    //
    // Disable internal pull-up for the selected output pins
    // for reduced power consumption
    // Pull-ups can be enabled or disabled by the user.
    // This will enable the pullups for the specified pins.
    // Comment out other unwanted lines.
    //
     GpioCtrlRegs.GPEPUD.bit.GPIO155 = 1;    // Disable pull-up on GPIO155 (EPWM6A)
    GpioCtrlRegs.GPEPUD.bit.GPIO156 = 1;    // Disable pull-up on GPIO156 (EPWM6B)

    //
    // Configure EPWM-6 pins using GPIO regs
    // This specifies which of the possible GPIO pins will be EPWM6 functional
    // pins.
    // Comment out other unwanted lines.
    //
    // GpioCtrlRegs.GPAMUX1.bit.GPIO10 = 1;   // Configure GPIO10 as EPWM6A
    // GpioCtrlRegs.GPAMUX1.bit.GPIO11 = 1;   // Configure GPIO11 as EPWM6B
    GpioCtrlRegs.GPEMUX2.bit.GPIO155 = 1;   // Configure GPIO155 as EPWM6A
    GpioCtrlRegs.GPEMUX2.bit.GPIO156 = 1;   // Configure GPIO156 as EPWM6B

    EDIS;
}

//
// InitEPwm7Gpio - Initialize EPWM7 GPIOs
//
void InitEPwm7Gpio(void)
{
    EALLOW;

    //
    // Disable internal pull-up for the selected output pins
    // for reduced power consumption
    // Pull-ups can be enabled or disabled by the user.
    // This will enable the pullups for the specified pins.
    // Comment out other unwanted lines.
    //
    GpioCtrlRegs.GPEPUD.bit.GPIO157 = 1;    // Disable pull-up on GPIO157 (EPWM7A)
    GpioCtrlRegs.GPEPUD.bit.GPIO158 = 1;    // Disable pull-up on GPIO158 (EPWM7B)

    //
    // Configure EPWM-6 pins using GPIO regs
    // This specifies which of the possible GPIO pins will be EPWM6 functional
    // pins.
    // Comment out other unwanted lines.
    //
    GpioCtrlRegs.GPEMUX2.bit.GPIO157 = 1;   // Configure GPIO157 as EPWM7A
    GpioCtrlRegs.GPEMUX2.bit.GPIO158 = 1;   // Configure GPIO158 as EPWM7B

    EDIS;
}

//
// InitEPwm8Gpio - Initialize EPWM8 GPIOs
//
void InitEPwm8Gpio(void)
{
    EALLOW;
    //
    // Disable internal pull-up for the selected output pins
    // for reduced power consumption
    // Pull-ups can be enabled or disabled by the user.
    // This will enable the pullups for the specified pins.
    // Comment out other unwanted lines.
    //
    GpioCtrlRegs.GPEPUD.bit.GPIO159 = 1;    // Disable pull-up on GPIO159 (EPWM8A)
    GpioCtrlRegs.GPFPUD.bit.GPIO160 = 1;    // Disable pull-up on GPIO160 (EPWM8B)

    //
    // Configure EPWM-6 pins using GPIO regs
    // This specifies which of the possible GPIO pins will be EPWM6 functional
    // pins.
    // Comment out other unwanted lines.
    //
   GpioCtrlRegs.GPEMUX2.bit.GPIO159 = 1;   // Configure GPIO159 as EPWM8A
    GpioCtrlRegs.GPFMUX1.bit.GPIO160 = 1;   // Configure GPIO160 as EPWM8B

    EDIS;
}

//
// InitEPwm9Gpio - Initialize EPWM9 GPIOs
//
void InitEPwm9Gpio(void)
{
    EALLOW;
    //
    // Disable internal pull-up for the selected output pins
    // for reduced power consumption
    // Pull-ups can be enabled or disabled by the user.
    // This will enable the pullups for the specified pins.
    // Comment out other unwanted lines.
    //
    GpioCtrlRegs.GPFPUD.bit.GPIO161 = 1;    // Disable pull-up on GPIO161 (EPWM9A)
    GpioCtrlRegs.GPFPUD.bit.GPIO162 = 1;    // Disable pull-up on GPIO162 (EPWM9B)

    //
    // Configure EPWM-6 pins using GPIO regs
    // This specifies which of the possible GPIO pins will be EPWM6 functional
    // pins.
    // Comment out other unwanted lines.
    //
    GpioCtrlRegs.GPFMUX1.bit.GPIO161 = 1;   // Configure GPIO161 as EPWM9A
    GpioCtrlRegs.GPFMUX1.bit.GPIO162 = 1;   // Configure GPIO162 as EPWM9B

    EDIS;
}

//
// InitEPwm10Gpio - Initialize EPWM10 GPIOs
//
void InitEPwm10Gpio(void)
{
    EALLOW;
    //
    // Disable internal pull-up for the selected output pins
    // for reduced power consumption
    // Pull-ups can be enabled or disabled by the user.
    // This will enable the pullups for the specified pins.
    // Comment out other unwanted lines.
    //
    GpioCtrlRegs.GPFPUD.bit.GPIO163 = 1;    // Disable pull-up on GPIO163 (EPWM10A)
    GpioCtrlRegs.GPFPUD.bit.GPIO164 = 1;    // Disable pull-up on GPIO164 (EPWM10B)

    //
    // Configure EPWM-6 pins using GPIO regs
    // This specifies which of the possible GPIO pins will be EPWM6 functional
    // pins.
    // Comment out other unwanted lines.
    //
    GpioCtrlRegs.GPFMUX1.bit.GPIO163 = 1;   // Configure GPIO163 as EPWM10A
    GpioCtrlRegs.GPFMUX1.bit.GPIO164 = 1;   // Configure GPIO164 as EPWM10B

    EDIS;
}

//
// InitEPwm11Gpio - Initialize EPWM11 GPIOs
//
void InitEPwm11Gpio(void)
{
    EALLOW;
    //
    // Disable internal pull-up for the selected output pins
    // for reduced power consumption
    // Pull-ups can be enabled or disabled by the user.
    // This will enable the pullups for the specified pins.
    // Comment out other unwanted lines.
    //
    GpioCtrlRegs.GPFPUD.bit.GPIO165 = 1;    // Disable pull-up on GPIO165 (EPWM11A)
    GpioCtrlRegs.GPFPUD.bit.GPIO166 = 1;    // Disable pull-up on GPIO166 (EPWM11B)

    //
    // Configure EPWM-6 pins using GPIO regs
    // This specifies which of the possible GPIO pins will be EPWM6 functional
    // pins.
    // Comment out other unwanted lines.
    //
    GpioCtrlRegs.GPFMUX1.bit.GPIO165 = 1;   // Configure GPIO165 as EPWM11A
    GpioCtrlRegs.GPFMUX1.bit.GPIO166 = 1;   // Configure GPIO166 as EPWM11B

    EDIS;
}

//
// InitEPwm12Gpio - Initialize EPWM12 GPIOs
//
void InitEPwm12Gpio(void)
{
    EALLOW;
    //
    // Disable internal pull-up for the selected output pins
    // for reduced power consumption
    // Pull-ups can be enabled or disabled by the user.
    // This will enable the pullups for the specified pins.
    // Comment out other unwanted lines.
    //
    GpioCtrlRegs.GPFPUD.bit.GPIO167 = 1;    // Disable pull-up on GPIO167 (EPWM12A)
    GpioCtrlRegs.GPFPUD.bit.GPIO168 = 1;    // Disable pull-up on GPIO168 (EPWM12B)

    //
    // Configure EPWM-6 pins using GPIO regs
    // This specifies which of the possible GPIO pins will be EPWM6 functional
    // pins.
    // Comment out other unwanted lines.
    //
    GpioCtrlRegs.GPFMUX1.bit.GPIO167 = 1;   // Configure GPIO167 as EPWM12A
    GpioCtrlRegs.GPFMUX1.bit.GPIO168 = 1;   // Configure GPIO168 as EPWM12B

    EDIS;
}
*/
