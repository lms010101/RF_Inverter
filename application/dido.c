#include "F28x_Project.h"
#include "variables.h"
#include "device.h"
#include "dido.h"



void dout(unsigned int variable_dout)
{
  if(variable_dout & 1) {
     GpioDataRegs.GPCCLEAR.bit.GPIO89 = 1;
  } else {
     GpioDataRegs.GPCSET.bit.GPIO89 = 1;
  }
  if((variable_dout >>1) &1) {
     GpioDataRegs.GPCCLEAR.bit.GPIO90 = 1;
  } else {
     GpioDataRegs.GPCSET.bit.GPIO90 = 1;
  }
  if((variable_dout >>2) &1) {
     GpioDataRegs.GPACLEAR.bit.GPIO28 = 1;
  } else {
     GpioDataRegs.GPASET.bit.GPIO28 = 1;
  }
  if((variable_dout >>3) &1) {
     GpioDataRegs.GPACLEAR.bit.GPIO29 = 1;
  } else {
     GpioDataRegs.GPASET.bit.GPIO29 = 1;
  }
  if((variable_dout >>4) &1) {
     GpioDataRegs.GPACLEAR.bit.GPIO30 = 1;
  } else {
     GpioDataRegs.GPASET.bit.GPIO30 = 1;
  }
  if((variable_dout >>5) &1) {
     GpioDataRegs.GPACLEAR.bit.GPIO31 = 1;
  } else {
     GpioDataRegs.GPASET.bit.GPIO31 = 1;
  }
  if((variable_dout >>6) &1)  {
     GpioDataRegs.GPDCLEAR.bit.GPIO124 = 1;
  } else {
     GpioDataRegs.GPDSET.bit.GPIO124 = 1;
  }
  if((variable_dout >>7) &1) {
     GpioDataRegs.GPDCLEAR.bit.GPIO125 = 1;
  } else {
     GpioDataRegs.GPDSET.bit.GPIO125 = 1;
  }
}

unsigned int din(void) {                  // Input 5V -> 0      Input 0 V -> 1

  unsigned int tmp_din=1;
  
  variable_din = 0;

  tmp_din = ~ GpioDataRegs.GPCDAT.bit.GPIO77; //Din0
  variable_din |= tmp_din & 1;
  tmp_din = ~ GpioDataRegs.GPCDAT.bit.GPIO91; //Din1
  variable_din |= ((tmp_din & 1) << 1);
  tmp_din = ~ GpioDataRegs.GPADAT.bit.GPIO14; //Din2
  variable_din |= ((tmp_din & 1) << 2);
  tmp_din = ~ GpioDataRegs.GPADAT.bit.GPIO12; //Din3
  variable_din |= ((tmp_din & 1) << 3);

/*
    DIN_0=GpioDataRegs.GPCDAT.bit.GPIO77; // DIN0
    DIN_1=GpioDataRegs.GPCDAT.bit.GPIO91; // DIN1
    DIN_2=GpioDataRegs.GPADAT.bit.GPIO14; // DIN2
    DIN_3=GpioDataRegs.GPADAT.bit.GPIO12; // DIN3
  */
  return variable_din;
}

/*------------------------------------------------------------------*/
/*         End of dido.c                                            */
/*------------------------------------------------------------------*/
