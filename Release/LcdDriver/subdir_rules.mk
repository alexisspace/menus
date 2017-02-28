################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
LcdDriver/HAL_MSP_EXP430F5529LP_KITRONIX320X240_SSD2119_SPI.obj: ../LcdDriver/HAL_MSP_EXP430F5529LP_KITRONIX320X240_SSD2119_SPI.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv6/tools/compiler/msp430_15.12.3.LTS/bin/cl430" -vmspx --data_model=restricted -O3 --use_hw_mpy=F5 --include_path="C:/ti/ccsv6/ccs_base/msp430/include" --include_path="C:/Users/alexis/workspace_v6_2/MSP-EXP430F5529LP_ILI9341/driverlib/MSP430F5xx_6xx" --include_path="C:/Users/alexis/workspace_v6_2/MSP-EXP430F5529LP_ILI9341" --include_path="C:/Users/alexis/workspace_v6_2/MSP-EXP430F5529LP_ILI9341/GrLib/grlib" --include_path="C:/Users/alexis/workspace_v6_2/MSP-EXP430F5529LP_ILI9341/GrLib/fonts" --include_path="C:/ti/ccsv6/tools/compiler/msp430_15.12.3.LTS/include" --advice:power="none" --gcc --define=__MSP430F5529__ --diag_warning=225 --diag_wrap=off --display_error_number --large_memory_model --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --printf_support=minimal --preproc_with_compile --preproc_dependency="LcdDriver/HAL_MSP_EXP430F5529LP_KITRONIX320X240_SSD2119_SPI.d" --obj_directory="LcdDriver" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

LcdDriver/ILI9341_Driver.obj: ../LcdDriver/ILI9341_Driver.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv6/tools/compiler/msp430_15.12.3.LTS/bin/cl430" -vmspx --data_model=restricted -O3 --use_hw_mpy=F5 --include_path="C:/ti/ccsv6/ccs_base/msp430/include" --include_path="C:/Users/alexis/workspace_v6_2/MSP-EXP430F5529LP_ILI9341/driverlib/MSP430F5xx_6xx" --include_path="C:/Users/alexis/workspace_v6_2/MSP-EXP430F5529LP_ILI9341" --include_path="C:/Users/alexis/workspace_v6_2/MSP-EXP430F5529LP_ILI9341/GrLib/grlib" --include_path="C:/Users/alexis/workspace_v6_2/MSP-EXP430F5529LP_ILI9341/GrLib/fonts" --include_path="C:/ti/ccsv6/tools/compiler/msp430_15.12.3.LTS/include" --advice:power="none" --gcc --define=__MSP430F5529__ --diag_warning=225 --diag_wrap=off --display_error_number --large_memory_model --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --printf_support=minimal --preproc_with_compile --preproc_dependency="LcdDriver/ILI9341_Driver.d" --obj_directory="LcdDriver" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

LcdDriver/kitronix320x240x16_ssd2119_spi.obj: ../LcdDriver/kitronix320x240x16_ssd2119_spi.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv6/tools/compiler/msp430_15.12.3.LTS/bin/cl430" -vmspx --data_model=restricted -O3 --use_hw_mpy=F5 --include_path="C:/ti/ccsv6/ccs_base/msp430/include" --include_path="C:/Users/alexis/workspace_v6_2/MSP-EXP430F5529LP_ILI9341/driverlib/MSP430F5xx_6xx" --include_path="C:/Users/alexis/workspace_v6_2/MSP-EXP430F5529LP_ILI9341" --include_path="C:/Users/alexis/workspace_v6_2/MSP-EXP430F5529LP_ILI9341/GrLib/grlib" --include_path="C:/Users/alexis/workspace_v6_2/MSP-EXP430F5529LP_ILI9341/GrLib/fonts" --include_path="C:/ti/ccsv6/tools/compiler/msp430_15.12.3.LTS/include" --advice:power="none" --gcc --define=__MSP430F5529__ --diag_warning=225 --diag_wrap=off --display_error_number --large_memory_model --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --printf_support=minimal --preproc_with_compile --preproc_dependency="LcdDriver/kitronix320x240x16_ssd2119_spi.d" --obj_directory="LcdDriver" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


