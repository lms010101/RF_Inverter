/*
 * dac.c
 *
 *  Created on: 2018. 3. 21.
 *      Author: RNR
 */

#include "dac.h"
#include "variables.h"

// /* <-- Previous code
int *da[4] = {0, 0, 0, 0};
int da_type[4] = {0, 0, 0, 0};
float da_scale[4] = {0., 0., 0., 0.};
float da_mid[4] = {0., 0., 0., 0.};

unsigned int sData = 0; // Send data
unsigned int rData = 0; // Receive data
unsigned int DAC_addr0 = 0;
unsigned int DAC_addr1 = 1;
unsigned int DAC_addr2 = 2;
unsigned int DAC_addr3 = 3;

void daInit(void)
{
    // Ch0
    sData = ((DAC_addr0&0x03)<<14) | (0x800 & 0x0fff);
    // Transmit data
    SPI_writeDataNonBlocking(SPIA_BASE, sData);
    DELAY_US(1.8);
    GpioDataRegs.GPACLEAR.bit.GPIO17 = 1;   // DAC_nLD low
    DELAY_US(0.01);
    GpioDataRegs.GPASET.bit.GPIO17 = 1; // DAC_nLD high

    // Ch1
    sData = ((DAC_addr1&0x03)<<14) | (0x800 & 0x0fff);
    // Transmit data
    SPI_writeDataNonBlocking(SPIA_BASE, sData);
    DELAY_US(1.8);
    GpioDataRegs.GPACLEAR.bit.GPIO17 = 1;   // DAC_nLD low
    DELAY_US(0.01);
    GpioDataRegs.GPASET.bit.GPIO17 = 1; // DAC_nLD high

    // Ch2
    sData = ((DAC_addr2&0x03)<<14) | (0x800 & 0x0fff);
    // Transmit data
    SPI_writeDataNonBlocking(SPIA_BASE, sData);
    DELAY_US(1.8);
    GpioDataRegs.GPACLEAR.bit.GPIO17 = 1;   // DAC_nLD low
    DELAY_US(0.01);
    GpioDataRegs.GPASET.bit.GPIO17 = 1; // DAC_nLD high

    // Ch3
    sData = ((DAC_addr3&0x03)<<14) | (0x800 & 0x0fff);
    // Transmit data
    SPI_writeDataNonBlocking(SPIA_BASE, sData);
    DELAY_US(1.8);
    GpioDataRegs.GPACLEAR.bit.GPIO17 = 1;   // DAC_nLD low
    DELAY_US(0.01);
    GpioDataRegs.GPASET.bit.GPIO17 = 1; // DAC_nLD high
}

void daOut(void)
{
    int j;
    float a;

    GpioDataRegs.GPASET.bit.GPIO17 = 1; // DAC_nLD high

    // Ch0
    a = da_type[0] ? (float)(*da[0]) : *(float *)da[0];
    j = (int)((a - da_mid[0])*da_scale[0]) + 0x800;
    sData = ((DAC_addr0&0x03)<<14) | (j & 0x0fff);
    // Transmit data
    SPI_writeDataNonBlocking(SPIA_BASE, sData);
    DELAY_US(1.8);
    GpioDataRegs.GPACLEAR.bit.GPIO17 = 1;   // DAC_nLD low
    DELAY_US(0.01);
    GpioDataRegs.GPASET.bit.GPIO17 = 1; // DAC_nLD high

    // Ch1
    a = da_type[1] ? (float)(*da[1]) : *(float *)da[1];
    j = (int)((a - da_mid[1])*da_scale[1]) + 0x800;
    sData = ((DAC_addr1&0x03)<<14) | (j & 0x0fff);
    // Transmit data
    SPI_writeDataNonBlocking(SPIA_BASE, sData);
    DELAY_US(1.8);
    GpioDataRegs.GPACLEAR.bit.GPIO17 = 1;   // DAC_nLD low
    DELAY_US(0.01);
    GpioDataRegs.GPASET.bit.GPIO17 = 1; // DAC_nLD high

    // Ch2
    a = da_type[2] ? (float)(*da[2]) : *(float *)da[2];
    j = (int)((a - da_mid[2])*da_scale[2]) + 0x800;
    sData = ((DAC_addr2&0x03)<<14) | (j & 0x0fff);
    // Transmit data
    SPI_writeDataNonBlocking(SPIA_BASE, sData);
    DELAY_US(1.8);
    GpioDataRegs.GPACLEAR.bit.GPIO17 = 1;   // DAC_nLD low
    DELAY_US(0.01);
    GpioDataRegs.GPASET.bit.GPIO17 = 1; // DAC_nLD high

    // Ch3
    a = da_type[3] ? (float)(*da[3]) : *(float *)da[3];
    j = (int)((a - da_mid[3])*da_scale[3]) + 0x800;
    sData = ((DAC_addr3&0x03)<<14) | (j & 0x0fff);
    // Transmit data
    SPI_writeDataNonBlocking(SPIA_BASE, sData);
    DELAY_US(1.8);
    GpioDataRegs.GPACLEAR.bit.GPIO17 = 1;   // DAC_nLD low
    DELAY_US(0.01);
    GpioDataRegs.GPASET.bit.GPIO17 = 1; // DAC_nLD high

    // Block until data is received and then return it
    //rData = SPI_readDataBlockingNonFIFO(SPIA_BASE);
}
//  previous code --> */
/*
uint16_t da_out[4] = {0, 0, 0, 0};
int *da_data[4] = {0, 0, 0, 0};
int da_type[4] = {0, 0, 0, 0};
float da_scale[4] = {2048./10., 2048./10., 2048./10., 2048./10.};
float da_offset[4] = {0., 0., 0., 0.};
uint32_t da_done[2] = {0, 0};
//float test_da=0,test_da2=0;

void InitDa(void)
{
    uint16_t destStep;
    uint16_t burstSize;
    volatile uint32_t *gpioDataReg;

    gpioDataReg = (uint32_t *)GPIODATA_BASE;

    DMA_initController();

    const void *destAddr;
    const void *srcAddr;
    srcAddr = (const void *)da_done;
    destAddr = (const void *)gpioDataReg[GPIO_GPxCLEAR_INDEX];
    destStep = GPIO_GPxSET_INDEX - GPIO_GPxCLEAR_INDEX;

    burstSize = 10;

    //
    // configure DMA CH1
    //
    DMA_configAddresses(DMA_CH1_BASE, destAddr, srcAddr);
    DMA_configBurst(DMA_CH1_BASE, burstSize, 0, 0);     // repeat for burstSize
    DMA_configTransfer(DMA_CH1_BASE, 2, 1, destStep);
    DMA_configMode(DMA_CH1_BASE, DMA_TRIGGER_SPIATX, (DMA_MODE_DATASIZE | DMA_MODE_CONTINUOUS | DMA_MODE_ONESHOT));
    DMA_setInterruptMode(DMA_CH1_BASE, DMA_INT_AT_END);
    DMA_enableTrigger(DMA_CH1_BASE);    // enable SPIATX ¡æ DMA start
    DMA_disableInterrupt(DMA_CH1_BASE); // disable DMA ¡æ CPU interrupt

    da_done[0] = GPIO_GPACLEAR_GPIO17; // DAC_nLD = low
    da_done[1] = (GPIO_GPASET_GPIO17 | GPIO_GPASET_GPIO19);  // DAC_nLD = high, DAC_nCS = high

    da_out[0] = 0x800;
    da_out[1] = 0x800;
    da_out[2] = 0x800;
    da_out[3] = 0x800;
    GPIO_setPortPins(GPIO_PORT_A, GPIO_GPASET_GPIO17 | GPIO_GPASET_GPIO19);  // DAC_nLD = high, DAC_nCS = high

}

void UpdateDa(void)
{
    unsigned int j;
    float a;

    GPIO_setPortPins(GPIO_PORT_A, GPIO_GPASET_GPIO17);      // DAC_nLD high
    GPIO_clearPortPins(GPIO_PORT_A, GPIO_GPACLEAR_GPIO19);  // DAC_nCS low

    a = da_type[0] ? (float)(*da_data[0]) : *(float *)da_data[0];  // da_data[0]=> a
    da_out[0] = ((int)((a - da_offset[0])*da_scale[0]) + 0x800) & 0x0FFF;
    j = (unsigned int)(0x0<<14) | da_out[0];
    SPI_writeDataNonBlocking(SPIA_BASE, j);

    a = da_type[1] ? (float)(*da_data[1]) : *(float *)da_data[1];
    da_out[1] = ((int)((a - da_offset[1])*da_scale[1]) + 0x800) & 0x0FFF;
    j = (unsigned int)(0x1<<14) | da_out[1];
    SPI_writeDataNonBlocking(SPIA_BASE, j);

    a = da_type[2] ? (float)(*da_data[2]) : *(float *)da_data[2];
    da_out[2] = ((int)((a - da_offset[2])*da_scale[2]) + 0x800) & 0x0FFF;
    j = (unsigned int)(0x2<<14) | da_out[2];
    SPI_writeDataNonBlocking(SPIA_BASE, j);

    a = da_type[3] ? (float)(*da_data[3]) : *(float *)da_data[3];
    da_out[3] = ((int)((a - da_offset[3])*da_scale[3]) + 0x800) & 0x0FFF;
    j = (unsigned int)(0x3<<14) | da_out[3];
    SPI_writeDataNonBlocking(SPIA_BASE, j);

    // (DAC_nCS high) and (DAC_nLD low) : done by DMA after SPI transmit
}
*/
