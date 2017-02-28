################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
main.obj: ../main.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv6/tools/compiler/msp430_15.12.3.LTS/bin/cl430" -vmspx --data_model=restricted -O3 --use_hw_mpy=F5 --include_path="C:/ti/ccsv6/ccs_base/msp430/include" --include_path="C:/Users/alexis/workspace_v6_2/MSP-EXP430F5529LP_ILI9341/driverlib/MSP430F5xx_6xx" --include_path="C:/Users/alexis/workspace_v6_2/MSP-EXP430F5529LP_ILI9341" --include_path="C:/Users/alexis/workspace_v6_2/MSP-EXP430F5529LP_ILI9341/GrLib/grlib" --include_path="C:/Users/alexis/workspace_v6_2/MSP-EXP430F5529LP_ILI9341/GrLib/fonts" --include_path="C:/ti/ccsv6/tools/compiler/msp430_15.12.3.LTS/include" --advice:power="none" --gcc --define=__MSP430F5529__ --diag_warning=225 --diag_wrap=off --display_error_number --large_memory_model --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --printf_support=minimal --preproc_with_compile --preproc_dependency="main.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

system_pre_init.obj: ../system_pre_init.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv6/tools/compiler/msp430_15.12.3.LTS/bin/cl430" -vmspx --data_model=restricted -O3 --use_hw_mpy=F5 --include_path="C:/ti/ccsv6/ccs_base/msp430/include" --include_path="C:/Users/alexis/workspace_v6_2/MSP-EXP430F5529LP_ILI9341/driverlib/MSP430F5xx_6xx" --include_path="C:/Users/alexis/workspace_v6_2/MSP-EXP430F5529LP_ILI9341" --include_path="C:/Users/alexis/workspace_v6_2/MSP-EXP430F5529LP_ILI9341/GrLib/grlib" --include_path="C:/Users/alexis/workspace_v6_2/MSP-EXP430F5529LP_ILI9341/GrLib/fonts" --include_path="C:/ti/ccsv6/tools/compiler/msp430_15.12.3.LTS/include" --advice:power="none" --gcc --define=__MSP430F5529__ --diag_warning=225 --diag_wrap=off --display_error_number --large_memory_model --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --printf_support=minimal --preproc_with_compile --preproc_dependency="system_pre_init.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

touch_F5529LP.obj: ../touch_F5529LP.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv6/tools/compiler/msp430_15.12.3.LTS/bin/cl430" -vmspx --data_model=restricted -O3 --use_hw_mpy=F5 --include_path="C:/ti/ccsv6/ccs_base/msp430/include" --include_path="C:/Users/alexis/workspace_v6_2/MSP-EXP430F5529LP_ILI9341/driverlib/MSP430F5xx_6xx" --include_path="C:/Users/alexis/workspace_v6_2/MSP-EXP430F5529LP_ILI9341" --include_path="C:/Users/alexis/workspace_v6_2/MSP-EXP430F5529LP_ILI9341/GrLib/grlib" --include_path="C:/Users/alexis/workspace_v6_2/MSP-EXP430F5529LP_ILI9341/GrLib/fonts" --include_path="C:/ti/ccsv6/tools/compiler/msp430_15.12.3.LTS/include" --advice:power="none" --gcc --define=__MSP430F5529__ --diag_warning=225 --diag_wrap=off --display_error_number --large_memory_model --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --printf_support=minimal --preproc_with_compile --preproc_dependency="touch_F5529LP.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


