################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/VL53L1x/Platform/Src/vl53l1_platform.c \
../Drivers/VL53L1x/Platform/Src/vl53l1_platform_log.c 

C_DEPS += \
./Drivers/VL53L1x/Platform/Src/vl53l1_platform.d \
./Drivers/VL53L1x/Platform/Src/vl53l1_platform_log.d 

OBJS += \
./Drivers/VL53L1x/Platform/Src/vl53l1_platform.o \
./Drivers/VL53L1x/Platform/Src/vl53l1_platform_log.o 


# Each subdirectory must supply rules for building sources it contributes
Drivers/VL53L1x/Platform/Src/%.o Drivers/VL53L1x/Platform/Src/%.su: ../Drivers/VL53L1x/Platform/Src/%.c Drivers/VL53L1x/Platform/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H750xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Vincent/Documents/Xplore/GitHub/AV_workspace/Drivers/VL53L1x/Core/Inc" -I"C:/Users/Vincent/Documents/Xplore/GitHub/AV_workspace/Drivers/VL53L1x/Platform/Inc" -I"C:/Users/Vincent/Documents/Xplore/GitHub/AV_workspace/Drivers/HX711" -I"C:/Users/Vincent/Documents/Xplore/GitHub/AV_workspace/Drivers/HX711/Core/Inc" -I"C:/Users/Vincent/Documents/Xplore/GitHub/AV_workspace/Drivers/BNO055/Core/Inc" -I"C:/Users/Vincent/Documents/Xplore/GitHub/AV_workspace/Drivers/Stemma/Core/inc" -I"C:/Users/Vincent/Documents/Xplore/GitHub/AV_workspace/Drivers/ADC1115/Core/Inc" -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"C:/Users/Vincent/Documents/Xplore/GitHub/AV_workspace/System/Sensors" -I"C:/Users/Vincent/Documents/Xplore/GitHub/AV_workspace/System" -I"C:/Users/Vincent/Documents/Xplore/GitHub/AV_workspace/System/RoCo/Src/Build" -I"C:/Users/Vincent/Documents/Xplore/GitHub/AV_workspace/System/RoCo/Src" -I"C:/Users/Vincent/Documents/Xplore/GitHub/AV_workspace/System/RoCo/Src/Protocol" -O1 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-VL53L1x-2f-Platform-2f-Src

clean-Drivers-2f-VL53L1x-2f-Platform-2f-Src:
	-$(RM) ./Drivers/VL53L1x/Platform/Src/vl53l1_platform.d ./Drivers/VL53L1x/Platform/Src/vl53l1_platform.o ./Drivers/VL53L1x/Platform/Src/vl53l1_platform.su ./Drivers/VL53L1x/Platform/Src/vl53l1_platform_log.d ./Drivers/VL53L1x/Platform/Src/vl53l1_platform_log.o ./Drivers/VL53L1x/Platform/Src/vl53l1_platform_log.su

.PHONY: clean-Drivers-2f-VL53L1x-2f-Platform-2f-Src
