################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Drivers/VL53L1x/Platform/Src/subdir.mk
-include Drivers/VL53L1x/Core/src/subdir.mk
-include Drivers/U087/Core/Src/subdir.mk
-include Drivers/Stemma/Core/src/subdir.mk
-include Drivers/STM32H7xx_HAL_Driver/Src/subdir.mk
-include Drivers/HX711/Core/Src/subdir.mk
-include Drivers/BNO055/Core/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := EPFL_XPLORE_AVIONICS_MCU
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
EPFL_XPLORE_AVIONICS_MCU.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
EPFL_XPLORE_AVIONICS_MCU.list \

OBJCOPY_BIN += \
EPFL_XPLORE_AVIONICS_MCU.bin \


# All Target
all: main-build

# Main-build Target
main-build: EPFL_XPLORE_AVIONICS_MCU.elf secondary-outputs

# Tool invocations
EPFL_XPLORE_AVIONICS_MCU.elf: $(OBJS) $(USER_OBJS) C:\Users\Yassine\ Bakkali\STM32CubeIDE\workspace_1.7.0\EPFL_XPLORE_AVIONICS_MCU\STM32H750VBTX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "EPFL_XPLORE_AVIONICS_MCU.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m7 -T"C:\Users\Yassine Bakkali\STM32CubeIDE\workspace_1.7.0\EPFL_XPLORE_AVIONICS_MCU\STM32H750VBTX_FLASH.ld" --specs=nosys.specs -Wl,-Map="EPFL_XPLORE_AVIONICS_MCU.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -u _printf_float -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

EPFL_XPLORE_AVIONICS_MCU.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "EPFL_XPLORE_AVIONICS_MCU.list"
	@echo 'Finished building: $@'
	@echo ' '

EPFL_XPLORE_AVIONICS_MCU.bin: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "EPFL_XPLORE_AVIONICS_MCU.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(SIZE_OUTPUT)$(OBJDUMP_LIST)$(EXECUTABLES)$(OBJS)$(S_DEPS)$(S_UPPER_DEPS)$(C_DEPS)$(OBJCOPY_BIN) EPFL_XPLORE_AVIONICS_MCU.elf
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
