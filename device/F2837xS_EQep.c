
// Included Files
//
#include "F2837xS_device.h"
#include "F2837xS_Examples.h"
#include "variables.h"
//

void InitENCOBJ_(void);
// InitEQep - This function initializes the eQEP(s) to a known state.
void InitEQep()
{
    InitENCOBJ_();

    // <-- Initialize eQEP2
    EQep2Regs.QUPRD=2000000;            // QUPRD = Unit Period Register (Unit Timer for 100Hz at 200 MHz SYSCLKOUT)
    EQep2Regs.QDECCTL.bit.QSRC=0;       // 0: QEP quadrature count mode, 1 : Direction Count mode, 2: UP-Count mode, 3: Down-count mode
    EQep2Regs.QEPCTL.bit.FREE_SOFT=2;   // For Emulation mode
    EQep2Regs.QEPCTL.bit.PCRM=01;       // QPOSCNT reset on Maximum Position
    EQep2Regs.QEPCTL.bit.UTE=1;         // Unit Timeout Enable
    EQep2Regs.QEPCTL.bit.QCLM=1;        // QEP Capture latch mode(0 : Latch on CPU, 1 : Latch on unit time out)
    EQep2Regs.QPOSMAX=encData.QPOSMAX;  // Max Count Setting = 8192 ppr: 0x1fff  4096 ppr: 0x0fff
    EQep2Regs.QEPCTL.bit.QPEN=1;        // eQep position counter enable

    EQep2Regs.QCAPCTL.bit.UPPS=5;       // Unit position event prescaler : 1/32 for unit position ( 2 power n)
    EQep2Regs.QCAPCTL.bit.CCPS=7;       // eQep capture timer clock prescaler : 1/128 for CAP clock ( 2 power n)
    EQep2Regs.QCAPCTL.bit.CEN=1;        // QEP Capture Enable
    // Initialize eQEP2 -->

    InitEQep2Gpio();

}

//
// InitEQepGpio - This function initializes GPIO pins to function as eQEP pins
//                Each GPIO pin can be configured as a GPIO pin or up to 3
//                different peripheral functional pins. By default all pins
//                come up as GPIO inputs after reset.
//                Caution:
//                For each eQEP peripheral
//                Only one GPIO pin should be enabled for EQEPxA operation.
//                Only one GPIO pin should be enabled for EQEPxB operation.
//                Only one GPIO pin should be enabled for EQEPxS operation.
//                Only one GPIO pin should be enabled for EQEPxI operation.
//                Comment out other unwanted lines.
//

// InitEQep1Gpio - Initialize EQEP-1 GPIOs
//                 Caution:
//                 For each eQEP peripheral
//                 Only one GPIO pin should be enabled for EQEPxA operation.
//                 Only one GPIO pin should be enabled for EQEPxB operation.
//                 Only one GPIO pin should be enabled for EQEPxS operation.
//                 Only one GPIO pin should be enabled for EQEPxI operation.
//                 Comment out other unwanted lines.

// <-- InitEQep2Gpio - Initialize EQEP-2 GPIOs
void InitEQep2Gpio(void)
{
    EALLOW;

    //
    // Disable internal pull-up for the selected output pins
    // for reduced power consumption
    // Pull-ups can be enabled or disabled by the user.
    // Comment out other unwanted lines.
    //
    // <-- Configure EQEP-1 pins using GPIO regs
      GpioCtrlRegs.GPCGMUX1.bit.GPIO78 = 1;    // Configure GPIO24 as EQEP2A
      GpioCtrlRegs.GPCGMUX1.bit.GPIO79 = 1;    // Configure GPIO25 as EQEP2B
      GpioCtrlRegs.GPCGMUX2.bit.GPIO81 = 1;    // Configure GPIO27 as EQEP2I

      GpioCtrlRegs.GPCMUX1.bit.GPIO78 = 2;     // Configure GPIO24 as EQEP2A
      GpioCtrlRegs.GPCMUX1.bit.GPIO79 = 2;     // Configure GPIO25 as EQEP2B
      GpioCtrlRegs.GPCMUX2.bit.GPIO81 = 2;     // Configure GPIO27 as EQEP2I
      // Configure EQEP-1 pins using GPIO regs -->


      GpioCtrlRegs.GPCPUD.bit.GPIO78 = 1;    // Disable pull-up on GPIO78 (EQEP2A)
      GpioCtrlRegs.GPCPUD.bit.GPIO79 = 1;    // Disable pull-up on GPIO79 (EQEP2B)
      GpioCtrlRegs.GPCPUD.bit.GPIO81 = 1;    // Disable pull-up on GPIO81 (EQEP2I)

    //
    // <-- Synchronize inputs to SYSCLK
    // Synchronization can be enabled or disabled by the user.
    // Comment out other unwanted lines.

      GpioCtrlRegs.GPCQSEL1.bit.GPIO78 = 0;   // Sync GPIO24 to SYSCLK  (EQEP2A)
      GpioCtrlRegs.GPCQSEL1.bit.GPIO79 = 0;   // Sync GPIO25 to SYSCLK  (EQEP2B)
      GpioCtrlRegs.GPCQSEL2.bit.GPIO81 = 0;   // Sync GPIO27 to SYSCLK  (EQEP2I)

    EDIS;
}
// InitEQep2Gpio - Initialize EQEP-2 GPIOs -->
