################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Core/Startup/startup_stm32h750vbtx.s 

OBJS += \
./Core/Startup/startup_stm32h750vbtx.o 

S_DEPS += \
./Core/Startup/startup_stm32h750vbtx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Startup/%.o: ../Core/Startup/%.s Core/Startup/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m7 -g3 -DDEBUG -c -I"C:/Users/Yassine Bakkali/STM32CubeIDE/workspace_1.7.0/EPFL_XPLORE_AVIONICS_MCU/Drivers/VL53L1x/Core/Inc" -I"C:/Users/Yassine Bakkali/STM32CubeIDE/workspace_1.7.0/EPFL_XPLORE_AVIONICS_MCU/Drivers/VL53L1x/Platform/Inc" -I"C:/Users/Yassine Bakkali/STM32CubeIDE/workspace_1.7.0/EPFL_XPLORE_AVIONICS_MCU/Drivers/HX711" -I"C:/Users/Yassine Bakkali/STM32CubeIDE/workspace_1.7.0/EPFL_XPLORE_AVIONICS_MCU/Drivers/HX711/Core/Inc" -I"C:/Users/Yassine Bakkali/STM32CubeIDE/workspace_1.7.0/EPFL_XPLORE_AVIONICS_MCU/Drivers/BNO055/Core/Inc" -I"C:/Users/Yassine Bakkali/STM32CubeIDE/workspace_1.7.0/EPFL_XPLORE_AVIONICS_MCU/Drivers/Stemma/Core/inc" -I"C:/Users/Yassine Bakkali/STM32CubeIDE/workspace_1.7.0/EPFL_XPLORE_AVIONICS_MCU/Drivers/U087/Core/Inc" -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

