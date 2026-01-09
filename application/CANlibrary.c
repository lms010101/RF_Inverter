//////////////////////////////////////////////////
//// cc.c                                     ////
//// Created on: 2020. 2. 5.                  ////
//// Author: Junsik Hwang, DongYeop Han      ////
//////////////////////////////////////////////////

#include "F28x_Project.h"
#include "CANlibrary.h"


/* <-- CAN TX/RX Setting
unsigned char TXMsgData[8], RXMsgData[8];
//tCANMsgObject sTXCANMessage;  //CAN Transfer
//tCANMsgObject sRXCANMessage;  //CAN Receive
//  CAN TX/RX Setting -->

int i=0;
Uint16 myRXMsgData=0;
Uint16 myTXMsgData=0;

static const uint16_t g_ui16CANBitValues[] =
{
    0x1100, // TSEG2 2, TSEG1 2, SJW 1, Divide 5
    0x1200, // TSEG2 2, TSEG1 3, SJW 1, Divide 6
    0x2240, // TSEG2 3, TSEG1 3, SJW 2, Divide 7
    0x2340, // TSEG2 3, TSEG1 4, SJW 2, Divide 8
    0x3340, // TSEG2 4, TSEG1 4, SJW 2, Divide 9
    0x3440, // TSEG2 4, TSEG1 5, SJW 2, Divide 10
    0x3540, // TSEG2 4, TSEG1 6, SJW 2, Divide 11
    0x3640, // TSEG2 4, TSEG1 7, SJW 2, Divide 12
    0x3740  // TSEG2 4, TSEG1 8, SJW 2, Divide 13
};

static bool
CANBaseValid(uint32_t ui32Base)
{
    return((ui32Base == CANA_BASE) || (ui32Base == CANB_BASE));
}

static void
CANDataRegRead(unsigned char *pucData, uint32_t *pui32Register, int16_t iSize)
{
    int16_t iIdx;
    unsigned char * pucRegister = (unsigned char *) pui32Register;

    // Loop always copies 1 or 2 bytes per iteration.
    for(iIdx = 0; iIdx < iSize; iIdx++ )
    {
        // Read out the data
        pucData[iIdx] = HWREGB(pucRegister++);
    }
}

static void
CANDataRegWrite(unsigned char *pucData, uint32_t *pui32Register, int16_t iSize)
{
    int16_t iIdx;
    unsigned char * pucRegister = (unsigned char *) pui32Register;

    // Loop always copies 1 or 2 bytes per iteration.
    for(iIdx = 0; iIdx < iSize; iIdx++ )
    {
        // Write out the data 8 bits at a time.
        HWREGB(pucRegister++) = pucData[iIdx];
    }
}

uint32_t
CANBitRateSet(uint32_t ui32Base, uint32_t ui32SourceClock, uint32_t ui32BitRate)
{
    uint32_t ui32DesiredRatio;
    uint32_t ui32CANBits;
    uint32_t ui32PreDivide;
    uint32_t ui32RegValue;
    uint16_t ui16CANCTL;

    ASSERT(ui32BitRate != 0);

    // Calculate the desired clock rate.
    ui32DesiredRatio = ui32SourceClock / ui32BitRate;

    // If the ratio of CAN bit rate to processor clock is too small or too
    // large then return 0 indicating that no bit rate was set.
    ASSERT(ui32DesiredRatio <= (CAN_MAX_PRE_DIVISOR * CAN_MAX_BIT_DIVISOR));
    ASSERT(ui32DesiredRatio >= (CAN_MIN_PRE_DIVISOR * CAN_MIN_BIT_DIVISOR));

    // Make sure that the Desired Ratio is not too large.  This enforces the
    // requirement that the bit rate is larger than requested.
    if((ui32SourceClock / ui32DesiredRatio) > ui32BitRate)
    {
        ui32DesiredRatio += 1;
    }

    // Check all possible values to find a matching value.
    while(ui32DesiredRatio <= CAN_MAX_PRE_DIVISOR * CAN_MAX_BIT_DIVISOR)
    {
        // Loop through all possible CAN bit divisors.
        for(ui32CANBits = CAN_MAX_BIT_DIVISOR;
            ui32CANBits >= CAN_MIN_BIT_DIVISOR;
            ui32CANBits--)
        {
            // For a given CAN bit divisor save the pre divisor.
            ui32PreDivide = ui32DesiredRatio / ui32CANBits;

            // If the calculated divisors match the desired clock ratio then
            // return these bit rate and set the CAN bit timing.
            if((ui32PreDivide * ui32CANBits) == ui32DesiredRatio)
            {
                // Start building the bit timing value by adding the bit timing
                // in time quanta.
                ui32RegValue =
                    g_ui16CANBitValues[ui32CANBits - CAN_MIN_BIT_DIVISOR];

                // To set the bit timing register, the controller must be
                // placed
                // in init mode (if not already), and also configuration change
                // bit enabled.  The state of the register should be saved
                // so it can be restored.

                ui16CANCTL = HWREGH(ui32Base + CAN_O_CTL);
                HWREGH(ui32Base + CAN_O_CTL) = ui16CANCTL | CAN_CTL_INIT |
                                               CAN_CTL_CCE;

                // Now add in the pre-scalar on the bit rate.
                ui32RegValue |= ((ui32PreDivide - 1) & CAN_BTR_BRP_M) |
                                (((ui32PreDivide - 1) << 10) & CAN_BTR_BRPE_M);

                // Set the clock bits in the and the bits of the
                // pre-scalar.
                //HWREGH(ui32Base + CAN_O_BTR) = (ui32RegValue &
                                                //CAN_REG_WORD_MASK);
                //HWREGH(ui32Base + CAN_O_BTR + 2) = (ui32RegValue >> 16);

                // Restore the saved CAN Control register.
                HWREGH(ui32Base + CAN_O_CTL) = ui16CANCTL;

                // Return the computed bit rate.
                return(ui32SourceClock / ( ui32PreDivide * ui32CANBits));
            }
        }

        // Move the divisor up one and look again.  Only in rare cases are
        // more than 2 loops required to find the value.
        ui32DesiredRatio++;
    }
    return(0);
}

void CANClkSourceSelect(uint32_t ui32Base, uint16_t ui16Source)
{
    EALLOW;
    switch(ui32Base)
    {
        case CANA_BASE:
        {
            ClkCfgRegs.CLKSRCCTL2.bit.CANABCLKSEL = ui16Source;
        }

        case CANB_BASE:
        {
            ClkCfgRegs.CLKSRCCTL2.bit.CANBBCLKSEL = ui16Source;
        }

        default:
            break;
    }
    EDIS;
}

void
CANEnable(uint32_t ui32Base)
{
    // Check the arguments.
    ASSERT(CANBaseValid(ui32Base));

    // Clear the init bit in the control register.
    HWREGH(ui32Base + CAN_O_CTL) = HWREGH(ui32Base + CAN_O_CTL) &
                                   ~CAN_CTL_INIT;
}

void
CANInit(uint32_t ui32Base)
{
    int16_t iMsg;

    // Check the arguments.
    ASSERT(CANBaseValid(ui32Base));

    // Place CAN controller in init state, regardless of previous state.  This
    // will put controller in idle, and allow the message object RAM to be
    // programmed.

    HWREGH(ui32Base + CAN_O_CTL) = CAN_CTL_INIT;
    HWREGH(ui32Base + CAN_O_CTL) = CAN_CTL_SWR;

    // Wait for busy bit to clear
    while(HWREGH(ui32Base + CAN_O_IF1CMD) & CAN_IF1CMD_BUSY)
    {
    }

    // Clear the message value bit in the arbitration register.  This indicates
    // the message is not valid and is a "safe" condition to leave the message
    // object.  The same arb reg is used to program all the message objects.
    HWREGH(ui32Base + CAN_O_IF1CMD + 2) = (CAN_IF1CMD_DIR | CAN_IF1CMD_ARB |
                                           CAN_IF1CMD_CONTROL) >> 16;
    HWREGH(ui32Base + CAN_O_IF1ARB) = 0;
    HWREGH(ui32Base + CAN_O_IF1ARB + 2) = 0;

    HWREGH(ui32Base + CAN_O_IF1MCTL) = 0;
    HWREGH(ui32Base + CAN_O_IF1MCTL + 2) = 0;

    HWREGH(ui32Base + CAN_O_IF2CMD + 2) = (CAN_IF2CMD_DIR | CAN_IF2CMD_ARB |
                                           CAN_IF2CMD_CONTROL) >> 16;
    HWREGH(ui32Base + CAN_O_IF2ARB) = 0;
    HWREGH(ui32Base + CAN_O_IF2ARB + 2) = 0;

    HWREGH(ui32Base + CAN_O_IF2MCTL) = 0;
    HWREGH(ui32Base + CAN_O_IF2MCTL + 2) = 0;

    // Loop through to program all 32 message objects
    for(iMsg = 1; iMsg <= 32; iMsg+=2)
    {
        // Wait for busy bit to clear
        while(HWREGH(ui32Base + CAN_O_IF1CMD) & CAN_IF1CMD_BUSY)
        {
        }

        // Initiate programming the message object
        HWREGH(ui32Base + CAN_O_IF1CMD) = iMsg;

        // Wait for busy bit to clear
        while(HWREGH(ui32Base + CAN_O_IF2CMD) & CAN_IF2CMD_BUSY)
        {
        }

        // Initiate programming the message object
        HWREGH(ui32Base + CAN_O_IF2CMD) = iMsg + 1;
    }

    // Make sure that the interrupt and new data flags are updated for the
    // message objects.
    HWREGH(ui32Base + CAN_O_IF1CMD + 2) = (CAN_IF1CMD_TXRQST |
                                           CAN_IF1CMD_CLRINTPND) >> 16;
    HWREGH(ui32Base + CAN_O_IF2CMD + 2) = (CAN_IF2CMD_TXRQST |
                                           CAN_IF2CMD_CLRINTPND) >> 16;

    // Loop through to program all 32 message objects
    for(iMsg = 1; iMsg <= 32; iMsg+=2)
    {
        // Wait for busy bit to clear.
        while(HWREGH(ui32Base + CAN_O_IF1CMD) & CAN_IF1CMD_BUSY)
        {
        }

        // Initiate programming the message object
        HWREGH(ui32Base + CAN_O_IF1CMD) = iMsg;

        // Wait for busy bit to clear.
        while(HWREGH(ui32Base + CAN_O_IF2CMD) & CAN_IF2CMD_BUSY)
        {
        }

        // Initiate programming the message object
        HWREGH(ui32Base + CAN_O_IF2CMD) = iMsg + 1;
    }

    // Acknowledge any pending status interrupts.
    HWREG(ui32Base + CAN_O_ES);
}

void
CANIntClear(uint32_t ui32Base, uint32_t ui32IntClr)
{
    // Check the arguments.
    ASSERT(CANBaseValid(ui32Base));
    ASSERT((ui32IntClr == CAN_INT_INT0ID_STATUS) ||
           ((ui32IntClr>=1) && (ui32IntClr <=32)));

    if(ui32IntClr == CAN_INT_INT0ID_STATUS)
    {
        // Simply read and discard the status to clear the interrupt.
        HWREG(ui32Base + CAN_O_ES);
    }
    else
    {
        // Wait to be sure that this interface is not busy.
        while(HWREGH(ui32Base + CAN_O_IF1CMD) & CAN_IF1CMD_BUSY)
        {
        }

        // Only change the interrupt pending state by setting only the
        // CAN_IF1CMD_CLRINTPND bit.
        HWREGH(ui32Base + CAN_O_IF1CMD + 2) = CAN_IF1CMD_CLRINTPND >> 16;

        // Send the clear pending interrupt command to the CAN controller.
        HWREGH(ui32Base + CAN_O_IF1CMD) = ui32IntClr & CAN_IF1CMD_MSG_NUM_M;

        // Wait to be sure that this interface is not busy.
        while(HWREGH(ui32Base + CAN_O_IF1CMD) & CAN_IF1CMD_BUSY)
        {
        }
    }
}

void
//CANMessageGet(uint32_t ui32Base, uint32_t ui32ObjID, tCANMsgObject *pMsgObject,
              bool bClrPendingInt)
{
    uint32_t ui32CmdMaskReg;
    uint32_t ui32MaskReg;
    uint32_t ui32ArbReg;
    uint32_t ui32MsgCtrl;

    // Check the arguments.
    ASSERT(CANBaseValid(ui32Base));
    ASSERT((ui32ObjID <= 32) && (ui32ObjID != 0));

    // This is always a read to the Message object as this call is setting a
    // message object.
    ui32CmdMaskReg = (CAN_IF2CMD_DATA_A | CAN_IF2CMD_DATA_B |
                      CAN_IF2CMD_CONTROL | CAN_IF2CMD_MASK | CAN_IF2CMD_ARB);

    // Clear a pending interrupt and new data in a message object.
    if(bClrPendingInt)
    {
        ui32CmdMaskReg |= CAN_IF2CMD_CLRINTPND | CAN_IF2CMD_TXRQST;
    }

    // Set up the request for data from the message object.
    HWREGH(ui32Base + CAN_O_IF2CMD + 2) =  ui32CmdMaskReg >> 16;

    // Transfer the message object to the message object specified by ui32ObjID.
    HWREGH(ui32Base + CAN_O_IF2CMD) = ui32ObjID & CAN_IF2CMD_MSG_NUM_M;

    // Wait for busy bit to clear
    while(HWREGH(ui32Base + CAN_O_IF2CMD) & CAN_IF2CMD_BUSY)
    {
    }

    // Read out the IF Registers.
    ui32MaskReg = HWREG(ui32Base + CAN_O_IF2MSK);
    ui32ArbReg = HWREG(ui32Base + CAN_O_IF2ARB);
    ui32MsgCtrl = HWREG(ui32Base + CAN_O_IF2MCTL);
    //pMsgObject->ui32Flags = MSG_OBJ_NO_FLAGS;

    // Determine if this is a remote frame by checking the TXRQST and DIR bits.
    if((!(ui32MsgCtrl & CAN_IF2MCTL_TXRQST) && (ui32ArbReg & CAN_IF2ARB_DIR)) ||
       ((ui32MsgCtrl & CAN_IF2MCTL_TXRQST) && (!(ui32ArbReg & CAN_IF2ARB_DIR))))
    {
        //pMsgObject->ui32Flags |= MSG_OBJ_REMOTE_FRAME;
    }

    // Get the identifier out of the register, the format depends on size of
    // the mask.
    if(ui32ArbReg & CAN_IF2ARB_XTD)
    {
        // Set the 29 bit version of the Identifier for this message object.
        pMsgObject->ui32MsgID = ui32ArbReg & CAN_IF2ARB_ID_M;

        //pMsgObject->ui32Flags |= MSG_OBJ_EXTENDED_ID;
    }
    else
    {
        // The Identifier is an 11 bit value.
        pMsgObject->ui32MsgID = (ui32ArbReg &
                                 CAN_IF2ARB_STD_ID_M) >> CAN_IF2ARB_STD_ID_S;
    }

    // Indicate that we lost some data.
    if(ui32MsgCtrl & CAN_IF2MCTL_MSGLST)
    {
       // pMsgObject->ui32Flags |= MSG_OBJ_DATA_LOST;
    }

    // Set the flag to indicate if ID masking was used.
    if(ui32MsgCtrl & CAN_IF2MCTL_UMASK)
    {
        if(ui32ArbReg & CAN_IF2ARB_XTD)
        {
            // The Identifier Mask is assumed to also be a 29 bit value.
            pMsgObject->ui32MsgIDMask = (ui32MaskReg & CAN_IF2MSK_MSK_M);

            // If this is a fully specified Mask and a remote frame then don't
            // set the MSG_OBJ_USE_ID_FILTER because the ID was not really
            // filtered.
            if((pMsgObject->ui32MsgIDMask != 0x1fffffff) ||
        //       ((pMsgObject->ui32Flags & MSG_OBJ_REMOTE_FRAME) == 0))
            {
      //          pMsgObject->ui32Flags |= MSG_OBJ_USE_ID_FILTER;
            }
        }
        else
        {
            // The Identifier Mask is assumed to also be an 11 bit value.
            pMsgObject->ui32MsgIDMask = ((ui32MaskReg & CAN_IF2MSK_MSK_M) >>
                                         18);

            // If this is a fully specified Mask and a remote frame then don't
            // set the MSG_OBJ_USE_ID_FILTER because the ID was not really
            // filtered.
            if((pMsgObject->ui32MsgIDMask != 0x7ff) ||
        //       ((pMsgObject->ui32Flags & MSG_OBJ_REMOTE_FRAME) == 0))
            {
                pMsgObject->ui32Flags |= MSG_OBJ_USE_ID_FILTER;
            }
        }

        // Indicate if the extended bit was used in filtering.
        if(ui32MaskReg & CAN_IF2MSK_MXTD)
        {
       //     pMsgObject->ui32Flags |= MSG_OBJ_USE_EXT_FILTER;
        }

        // Indicate if direction filtering was enabled.
        if(ui32MaskReg & CAN_IF2MSK_MDIR)
        {
      //      pMsgObject->ui32Flags |= MSG_OBJ_USE_DIR_FILTER;
        }
    }

    // Set the interrupt flags.
    if(ui32MsgCtrl & CAN_IF2MCTL_TXIE)
    {
   //     pMsgObject->ui32Flags |= MSG_OBJ_TX_INT_ENABLE;
    }
    if(ui32MsgCtrl & CAN_IF2MCTL_RXIE)
    {
    //    pMsgObject->ui32Flags |= MSG_OBJ_RX_INT_ENABLE;
    }

    // See if there is new data available.
    if(ui32MsgCtrl & CAN_IF2MCTL_NEWDAT)
    {
        // Get the amount of data needed to be read.
        pMsgObject->ui32MsgLen = (ui32MsgCtrl & CAN_IF2MCTL_DLC_M);

        // Don't read any data for a remote frame, there is nothing valid in
        // that buffer anyway.
      //  if((pMsgObject->ui32Flags & MSG_OBJ_REMOTE_FRAME) == 0)
        {
            // Read out the data from the CAN registers.
            CANDataRegRead(pMsgObject->pucMsgData,
                           (uint32_t *)(ui32Base + CAN_O_IF2DATA),
                           pMsgObject->ui32MsgLen);
        }

        // Now clear out the new data flag.
        HWREGH(ui32Base + CAN_O_IF2CMD + 2) = CAN_IF2CMD_TXRQST >> 16;

        // Transfer the message object to the message object specified by
        // ui32ObjID.
        HWREGH(ui32Base + CAN_O_IF2CMD) = ui32ObjID & CAN_IF2CMD_MSG_NUM_M;

        // Wait for busy bit to clear
        while(HWREGH(ui32Base + CAN_O_IF2CMD) & CAN_IF2CMD_BUSY)
        {
        }

        // Indicate that there is new data in this message.
       // pMsgObject->ui32Flags |= MSG_OBJ_NEW_DATA;
    }
    else
    {
        // Along with the MSG_OBJ_NEW_DATA not being set the amount of data
        // needs to be set to zero if none was available.
        pMsgObject->ui32MsgLen = 0;
    }
}

void
//CANMessageSet(uint32_t ui32Base, uint32_t ui32ObjID, tCANMsgObject *pMsgObject,
  //            tMsgObjType eMsgType)
{
    uint32_t ui32CmdMaskReg;
    uint32_t ui32MaskReg;
    uint32_t ui32ArbReg;
    uint32_t ui32MsgCtrl;
    bool bTransferData;
    bool bUseExtendedID;

    bTransferData = 0;

    // Check the arguments.
    ASSERT(CANBaseValid(ui32Base));
    ASSERT((ui32ObjID <= 32) && (ui32ObjID != 0));
   // ASSERT((eMsgType == MSG_OBJ_TYPE_TX) ||
           (eMsgType == MSG_OBJ_TYPE_TX_REMOTE) ||
           (eMsgType == MSG_OBJ_TYPE_RX) ||
           (eMsgType == MSG_OBJ_TYPE_RX_REMOTE) ||
           (eMsgType == MSG_OBJ_TYPE_TX_REMOTE) ||
           (eMsgType == MSG_OBJ_TYPE_RXTX_REMOTE));

    // Wait for busy bit to clear
    while(HWREGH(ui32Base + CAN_O_IF1CMD) & CAN_IF1CMD_BUSY)
    {
    }

    // See if we need to use an extended identifier or not.
    if((pMsgObject->ui32MsgID > CAN_MAX_11BIT_MSG_ID) ||
    //   (pMsgObject->ui32Flags & MSG_OBJ_EXTENDED_ID))
    {
        bUseExtendedID = 1;
    }
    else
    {
        bUseExtendedID = 0;
    }

    // This is always a write to the Message object as this call is setting a
    // message object.  This call will also always set all size bits so it sets
    // both data bits.  The call will use the CONTROL register to set control
    // bits so this bit needs to be set as well.
    ui32CmdMaskReg = (CAN_IF1CMD_DIR | CAN_IF1CMD_DATA_A | CAN_IF1CMD_DATA_B |
                      CAN_IF1CMD_CONTROL);

    // Initialize the values to a known state before filling them in based on
    // the type of message object that is being configured.
    ui32ArbReg = 0;
    ui32MsgCtrl = 0;
    ui32MaskReg = 0;

    switch(eMsgType)
    {
    // Transmit message object.
   // case MSG_OBJ_TYPE_TX:
    {
        // Set the TXRQST bit and the reset the rest of the register.
        ui32MsgCtrl |= CAN_IF1MCTL_TXRQST;
        ui32ArbReg = CAN_IF1ARB_DIR;
        bTransferData = 1;
        break;
    }

    // Transmit remote request message object
   // case MSG_OBJ_TYPE_TX_REMOTE:
    {
        // Set the TXRQST bit and the reset the rest of the register.
        ui32MsgCtrl |= CAN_IF1MCTL_TXRQST;
        ui32ArbReg = 0;
        break;
    }

    // Receive message object.
 //   case MSG_OBJ_TYPE_RX:
    {
        // This clears the DIR bit along with everything else.  The TXRQST
        // bit was cleared by defaulting ui32MsgCtrl to 0.
        ui32ArbReg = 0;
        break;
    }

    // Receive remote request message object.
 //   case MSG_OBJ_TYPE_RX_REMOTE:
    {
        // The DIR bit is set to one for remote receivers.  The TXRQST bit
        // was cleared by defaulting ui32MsgCtrl to 0.
        ui32ArbReg = CAN_IF1ARB_DIR;

        // Set this object so that it only indicates that a remote frame
        // was received and allow for software to handle it by sending back
        // a data frame.
        ui32MsgCtrl = CAN_IF1MCTL_UMASK;

        // Use the full Identifier by default.
        ui32MaskReg = CAN_IF1MSK_MSK_M;

        // Make sure to send the mask to the message object.
        ui32CmdMaskReg |= CAN_IF1CMD_MASK;
        break;
    }

    // Remote frame receive remote, with auto-transmit message object.
  //  case MSG_OBJ_TYPE_RXTX_REMOTE:
    {
        // Oddly the DIR bit is set to one for remote receivers.
        ui32ArbReg = CAN_IF1ARB_DIR;

        // Set this object to auto answer if a matching identifier is seen.
        ui32MsgCtrl = CAN_IF1MCTL_RMTEN | CAN_IF1MCTL_UMASK;

        // The data to be returned needs to be filled in.
        bTransferData = 1;
        break;
    }

    // This case should never happen due to the ASSERT statement at the
    // beginning of this function.
    default:
    {
        return;
    }
    }

    // Configure the Mask Registers.
   // if(pMsgObject->ui32Flags & MSG_OBJ_USE_ID_FILTER)
    {
        if(bUseExtendedID)
        {
            // Set the 29 bits of Identifier mask that were requested.
            ui32MaskReg = pMsgObject->ui32MsgIDMask & CAN_IF1MSK_MSK_M;
        }
        else
        {

            // Put the 11 bit Mask Identifier into the upper bits of the field
            // in the register.
            ui32MaskReg = ((pMsgObject->ui32MsgIDMask << CAN_IF1ARB_STD_ID_S) &
                           CAN_IF1ARB_STD_ID_M);
        }
    }

    // If the caller wants to filter on the extended ID bit then set it.
 //   if((pMsgObject->ui32Flags & MSG_OBJ_USE_EXT_FILTER) ==
       MSG_OBJ_USE_EXT_FILTER)
    {
        ui32MaskReg |= CAN_IF1MSK_MXTD;
    }

    // The caller wants to filter on the message direction field.
   // if((pMsgObject->ui32Flags & MSG_OBJ_USE_DIR_FILTER) ==
       MSG_OBJ_USE_DIR_FILTER)
    {
        ui32MaskReg |= CAN_IF1MSK_MDIR;
    }

    if(pMsgObject->ui32Flags & (MSG_OBJ_USE_ID_FILTER | MSG_OBJ_USE_DIR_FILTER |
                              MSG_OBJ_USE_EXT_FILTER))
    {
        // Set the UMASK bit to enable using the mask register.
        ui32MsgCtrl |= CAN_IF1MCTL_UMASK;

        // Set the MASK bit so that this gets transferred to the Message
        // Object.
        ui32CmdMaskReg |= CAN_IF1CMD_MASK;
    }

    // Set the Arb bit so that this gets transferred to the Message object.
    ui32CmdMaskReg |= CAN_IF1CMD_ARB;

    // Configure the Arbitration registers.
    if(bUseExtendedID)
    {
        // Set the 29 bit version of the Identifier for this message object.
        // Mark the message as valid and set the extended ID bit.
        ui32ArbReg |= (pMsgObject->ui32MsgID & CAN_IF1ARB_ID_M) |
                      CAN_IF1ARB_MSGVAL | CAN_IF1ARB_XTD;
    }
    else
    {
        // Set the 11 bit version of the Identifier for this message object.
        // The lower 18 bits are set to zero.
        // Mark the message as valid.
        ui32ArbReg |= ((pMsgObject->ui32MsgID << CAN_IF1ARB_STD_ID_S) &
                       CAN_IF1ARB_STD_ID_M) | CAN_IF1ARB_MSGVAL;
    }

    // Set the data length since this is set for all transfers.  This is also a
    // single transfer and not a FIFO transfer so set EOB bit.
    ui32MsgCtrl |= (pMsgObject->ui32MsgLen & CAN_IF1MCTL_DLC_M);

    // Mark this as the last entry if this is not the last entry in a FIFO.
   // if((pMsgObject->ui32Flags & MSG_OBJ_FIFO) == 0)
    {
        ui32MsgCtrl |= CAN_IF1MCTL_EOB;
    }

    // Enable transmit interrupts if they should be enabled.
   // if(pMsgObject->ui32Flags & MSG_OBJ_TX_INT_ENABLE)
    {
        ui32MsgCtrl |= CAN_IF1MCTL_TXIE;
    }

    // Enable receive interrupts if they should be enabled.
   // if(pMsgObject->ui32Flags & MSG_OBJ_RX_INT_ENABLE)
    {
        ui32MsgCtrl |= CAN_IF1MCTL_RXIE;
    }

    // Write the data out to the CAN Data registers if needed.
    if(bTransferData)
    {
        CANDataRegWrite(pMsgObject->pucMsgData,
                        (uint32_t *)(ui32Base + CAN_O_IF1DATA),
                        pMsgObject->ui32MsgLen);
    }

    // Write out the registers to program the message object.
    HWREGH(ui32Base + CAN_O_IF1CMD + 2) = ui32CmdMaskReg >> 16;

   // HWREGH(ui32Base + CAN_O_IF1MSK) = ui32MaskReg & CAN_REG_WORD_MASK;
    HWREGH(ui32Base + CAN_O_IF1MSK + 2) = ui32MaskReg >> 16;

    HWREGH(ui32Base + CAN_O_IF1ARB) = ui32ArbReg & CAN_REG_WORD_MASK;
    HWREGH(ui32Base + CAN_O_IF1ARB + 2) = ui32ArbReg >> 16;

    HWREGH(ui32Base + CAN_O_IF1MCTL) = ui32MsgCtrl & CAN_REG_WORD_MASK;

    // Transfer the message object to the message object specific by ui32ObjID.
    HWREGH(ui32Base + CAN_O_IF1CMD) = ui32ObjID & CAN_IF1CMD_MSG_NUM_M;

    return;
}

void myIinitCAN(){
    // <-- Initialize the transmit message object used for sending CAN messages (Message Object Parameters)
    sTXCANMessage.ui32MsgID = 44;                    //Message Identifier Setting
    sTXCANMessage.ui32MsgIDMask = 0x0;               // Message ID Mask: 0x0
   // sTXCANMessage.ui32Flags = MSG_OBJ_NO_FLAGS;      //  Message Object Flags: None
    sTXCANMessage.ui32MsgLen = sizeof(TXMsgData);    // Message Data Length
    sTXCANMessage.pucMsgData = TXMsgData;            // Message Transmit data: TXMsgData
    // Initialize the transmit message object used for sending CAN messages (Message Object Parameters) -->

    // <-- Initialize the receive message object used for receiving CAN messages (Message Object Parameters)
    sRXCANMessage.ui32MsgID = 54;                                                                   // Message Identifier Setting
    sRXCANMessage.ui32MsgIDMask = 0x37;                                                                // Message ID Mask: 0x0
   // sRXCANMessage.ui32Flags = MSG_OBJ_USE_ID_FILTER;                                                     // Message Object Flags
    sRXCANMessage.ui32MsgLen = sizeof(RXMsgData);                                                   // Message Data Length: RXMsgData
    sRXCANMessage.pucMsgData = RXMsgData;                                                           // Message Receive data: RXMsgData

    // Initialize the receive message object used for receiving CAN messages (Message Object Parameters) -->

    // <-- 32의 메일 박스 중 0번 메일 박스에 있는 데이터를 받을 거라고 MCU한테 알림
   // CANMessageSet(CANB_BASE, 0, &sRXCANMessage, MSG_OBJ_TYPE_RX);
    // 32의 메일 박스 중 0번 메일 박스 있는 데이터를 받을 거라고 MCU한테 알림 -->

}


void myGettingCANMSG(void){

    // <-- CAN 송수신 오브젝트 버퍼에 저장된 메시지를 읽어옴
       CANMessageGet(CANB_BASE, 0, &sRXCANMessage, true);               // 해당 메일박스(0)에  ID 구분해서 데이터를 받음
     //  CANMessageSet(CANB_BASE, 1, &sTXCANMessage, MSG_OBJ_TYPE_TX);    //해당 메일박스(1)번에다 데이터를 올려서 보냄
       CANIntClear(CANB_BASE, 0);                                       // 메세지 오브젝트 입터럽트 초기화
       // CAN 송수신 오브젝트 버퍼에 저장된 메시지를 읽어옴 -->


    // <--CAN 메시지 배열에 다시 넣음
//    for (i=0;i<7;sizeof(TXMsgData))
//    {
//        myRXMsgData=((RXMsgData[i]>>i)&1);
//        TXMsgData[i]=((myTXMsgData>>i)&1);
//    }
    //  CAN 메시지 배열에 다시 넣음-->

}*/
