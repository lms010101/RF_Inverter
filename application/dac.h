/*
 * dac.h
 *
 *  Created on: 2018. 8. 3.
 *      Author: PowerElabLAPTOP1
 */


#ifndef APPLICATION_DAC_H_
#define APPLICATION_DAC_H_
// /* <-- previous code

extern int *da[4];
extern int da_type[4];
extern float da_scale[4];
extern float da_mid[4];

void daInit(void);
void daOut(void);
// previous code --> */
/*
#include "device.h"

extern void InitDa(void);
extern void UpdateDa(void);

extern uint16_t da_out[4];
extern int *da_data[4];
extern int da_type[4];
extern float da_scale[4];
extern float da_offset[4];
extern uint32_t da_done[2];
*/
#endif /* APPLICATION_DAC_H_ */
