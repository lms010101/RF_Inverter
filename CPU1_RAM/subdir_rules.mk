################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs2020/ti-cgt-c2000_20.2.7.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -Ooff --include_path="C:/Users/spec1/CCS/RF_Inverter" --include_path="C:/Users/spec1/CCS/RF_Inverter/device" --include_path="C:/Users/spec1/CCS/RF_Inverter/application" --include_path="C:/ti/c2000/C2000Ware_4_00_00_00/device_support/f2837xs/common/include" --include_path="C:/ti/c2000/C2000Ware_4_00_00_00/device_support/f2837xs/headers/include" --include_path="C:/ti/c2000/C2000Ware_4_00_00_00/driverlib/f2837xs/driverlib" --include_path="C:/ti/ccs2020/ti-cgt-c2000_20.2.7.LTS/include" --advice:performance=all --define=DEBUG --define=_DUAL_HEADERS --define=CPU1 -g --diag_suppress=10063 --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" --include_path="C:/Users/spec1/CCS/RF_Inverter/CPU1_RAM/syscfg" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


