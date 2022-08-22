################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/BMO055/Core/Src/bno055.c \
../Drivers/BMO055/Core/Src/bno055_stm32.c 

OBJS += \
./Drivers/BMO055/Core/Src/bno055.o \
./Drivers/BMO055/Core/Src/bno055_stm32.o 

C_DEPS += \
./Drivers/BMO055/Core/Src/bno055.d \
./Drivers/BMO055/Core/Src/bno055_stm32.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BMO055/Core/Src/%.o: ../Drivers/BMO055/Core/Src/%.c Drivers/BMO055/Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H750xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Yassine Bakkali/STM32CubeIDE/workspace_1.7.0/EPFL_XPLORE_AVIONICS_MCU/Drivers/VL53L1x/Core/Inc" -I"C:/Users/Yassine Bakkali/STM32CubeIDE/workspace_1.7.0/EPFL_XPLORE_AVIONICS_MCU/Drivers/VL53L1x/Platform/Inc" -I"C:/Users/Yassine Bakkali/STM32CubeIDE/workspace_1.7.0/EPFL_XPLORE_AVIONICS_MCU/Drivers/HX711" -I"C:/Users/Yassine Bakkali/STM32CubeIDE/workspace_1.7.0/EPFL_XPLORE_AVIONICS_MCU/Drivers/HX711/Core/Inc" -I"C:/Users/Yassine Bakkali/STM32CubeIDE/workspace_1.7.0/EPFL_XPLORE_AVIONICS_MCU/Drivers/BMO055/Core/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

