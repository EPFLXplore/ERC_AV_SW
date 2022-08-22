################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/ThreadSafe/newlib_lock_glue.c 

C_DEPS += \
./Core/ThreadSafe/newlib_lock_glue.d 

OBJS += \
./Core/ThreadSafe/newlib_lock_glue.o 


# Each subdirectory must supply rules for building sources it contributes
Core/ThreadSafe/%.o Core/ThreadSafe/%.su: ../Core/ThreadSafe/%.c Core/ThreadSafe/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H750xx -DSTM32_THREAD_SAFE_STRATEGY=4 -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Vincent/Documents/Xplore/GitHub/AV_workspace/Drivers/ADC1115/Core/Inc" -I"C:/Users/Vincent/Documents/Xplore/GitHub/AV_workspace/Drivers/BNO055/Core/Inc" -I"C:/Users/Vincent/Documents/Xplore/GitHub/AV_workspace/Drivers/HX711/Core/Inc" -I"C:/Users/Vincent/Documents/Xplore/GitHub/AV_workspace/Drivers/Stemma/Core/inc" -I"C:/Users/Vincent/Documents/Xplore/GitHub/AV_workspace/Drivers/VL53L1x/Core/Inc" -I"C:/Users/Vincent/Documents/Xplore/GitHub/AV_workspace/Drivers/VL53L1x/Platform/Inc" -I"C:/Users/Vincent/Documents/Xplore/GitHub/AV_workspace/System" -I"C:/Users/Vincent/Documents/Xplore/GitHub/AV_workspace/System/RoCo/Src" -I"C:/Users/Vincent/Documents/Xplore/GitHub/AV_workspace/System/Lang" -I"C:/Users/Vincent/Documents/Xplore/GitHub/AV_workspace/System/RoCo/Src/Build" -I"C:/Users/Vincent/Documents/Xplore/GitHub/AV_workspace/System/RoCo/Src/Protocol" -I"C:/Users/Vincent/Documents/Xplore/GitHub/AV_workspace/System/Sensors" -I"C:/Users/Vincent/Documents/Xplore/GitHub/AV_workspace/System/Misc" -I../Core/ThreadSafe -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -O1 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-ThreadSafe

clean-Core-2f-ThreadSafe:
	-$(RM) ./Core/ThreadSafe/newlib_lock_glue.d ./Core/ThreadSafe/newlib_lock_glue.o ./Core/ThreadSafe/newlib_lock_glue.su

.PHONY: clean-Core-2f-ThreadSafe

