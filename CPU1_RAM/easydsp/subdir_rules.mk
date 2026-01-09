################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
easydsp/%.obj: ../easydsp/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/c2000/ti-cgt-c2000_15.12.7.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --cla_support=cla1 --tmu_support=tmu0 --vcu_support=vcu2 -Ooff --include_path="E:/CCS_Workspace/RF_Inverter" --include_path="E:/CCS_Workspace/RF_Inverter/device" --include_path="E:/CCS_Workspace/RF_Inverter/application" --include_path="C:/ti/c2000/C2000Ware_4_00_00_00/device_support/f2837xs/common/include" --include_path="C:/ti/c2000/C2000Ware_4_00_00_00/device_support/f2837xs/headers/include" --include_path="C:/ti/c2000/C2000Ware_4_00_00_00/driverlib/f2837xs/driverlib" --include_path="C:/ti/c2000/ti-cgt-c2000_15.12.7.LTS/include" --advice:performance=all --symdebug:coff --define=DEBUG --define=_DUAL_HEADERS --define=CPU1 --display_error_number --diag_suppress=10063 --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="easydsp/$(basename $(<F)).d_raw" --obj_directory="easydsp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


