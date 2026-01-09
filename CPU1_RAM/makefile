################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccs2020/ti-cgt-c2000_20.2.7.LTS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./main.obj" \
"./application/cc.obj" \
"./application/dac.obj" \
"./application/dido.obj" \
"./application/fault.obj" \
"./application/offset.obj" \
"./application/variables.obj" \
"./device/F2837xS_Adc.obj" \
"./device/F2837xS_CodeStartBranch.obj" \
"./device/F2837xS_CpuTimers.obj" \
"./device/F2837xS_DBGIER.obj" \
"./device/F2837xS_DefaultISR.obj" \
"./device/F2837xS_EPwm.obj" \
"./device/F2837xS_EQep.obj" \
"./device/F2837xS_GlobalVariableDefs.obj" \
"./device/F2837xS_Gpio.obj" \
"./device/F2837xS_PieCtrl.obj" \
"./device/F2837xS_PieVect.obj" \
"./device/F2837xS_SysCtrl.obj" \
"./device/F2837xS_Upp.obj" \
"./device/F2837xS_usDelay.obj" \
"./device/device.obj" \
"./easydsp/RingBuff.obj" \
"./easydsp/easy2837xS_sci_v8.5.obj" \
"../2837xS_Generic_RAM_lnk.cmd" \
"../F2837xS_Headers_nonBIOS.cmd" \
"../driverlib.lib" \
$(GEN_CMDS__FLAG) \
-llibc.a \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include application/subdir_vars.mk
-include device/subdir_vars.mk
-include easydsp/subdir_vars.mk
-include subdir_rules.mk
-include application/subdir_rules.mk
-include device/subdir_rules.mk
-include easydsp/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
DAB_Control.out 

EXE_OUTPUTS__QUOTED += \
"DAB_Control.out" 


# All Target
all: $(OBJS) $(CMD_SRCS) $(LIB_SRCS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "DAB_Control.out"

# Tool invocations
DAB_Control.out: $(OBJS) $(CMD_SRCS) $(LIB_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: C2000 Linker'
	"C:/ti/ccs2020/ti-cgt-c2000_20.2.7.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -Ooff --advice:performance=all --define=DEBUG --define=_DUAL_HEADERS --define=CPU1 -g --diag_suppress=10063 --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi -z -m"DAB_Control.map" --stack_size=0x100 --warn_sections -i"C:/ti/ccs2020/ti-cgt-c2000_20.2.7.LTS/lib" -i"C:/ti/ccs2020/ti-cgt-c2000_20.2.7.LTS/include" --reread_libs --diag_wrap=off --display_error_number --xml_link_info="DAB_Control_linkInfo.xml" --rom_model -o "DAB_Control.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXE_OUTPUTS__QUOTED)
	-$(RM) "main.obj" "application\cc.obj" "application\dac.obj" "application\dido.obj" "application\fault.obj" "application\offset.obj" "application\variables.obj" "device\F2837xS_Adc.obj" "device\F2837xS_CodeStartBranch.obj" "device\F2837xS_CpuTimers.obj" "device\F2837xS_DBGIER.obj" "device\F2837xS_DefaultISR.obj" "device\F2837xS_EPwm.obj" "device\F2837xS_EQep.obj" "device\F2837xS_GlobalVariableDefs.obj" "device\F2837xS_Gpio.obj" "device\F2837xS_PieCtrl.obj" "device\F2837xS_PieVect.obj" "device\F2837xS_SysCtrl.obj" "device\F2837xS_Upp.obj" "device\F2837xS_usDelay.obj" "device\device.obj" "easydsp\RingBuff.obj" "easydsp\easy2837xS_sci_v8.5.obj" 
	-$(RM) "main.d" "application\cc.d" "application\dac.d" "application\dido.d" "application\fault.d" "application\offset.d" "application\variables.d" "device\F2837xS_Adc.d" "device\F2837xS_CpuTimers.d" "device\F2837xS_DefaultISR.d" "device\F2837xS_EPwm.d" "device\F2837xS_EQep.d" "device\F2837xS_GlobalVariableDefs.d" "device\F2837xS_Gpio.d" "device\F2837xS_PieCtrl.d" "device\F2837xS_PieVect.d" "device\F2837xS_SysCtrl.d" "device\F2837xS_Upp.d" "device\device.d" "easydsp\RingBuff.d" "easydsp\easy2837xS_sci_v8.5.d" 
	-$(RM) "device\F2837xS_CodeStartBranch.d" "device\F2837xS_DBGIER.d" "device\F2837xS_usDelay.d" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

