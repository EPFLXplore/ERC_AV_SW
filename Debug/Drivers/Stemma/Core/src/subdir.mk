################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/Stemma/Core/src/stemma.c 

C_DEPS += \
./Drivers/Stemma/Core/src/stemma.d 

OBJS += \
./Drivers/Stemma/Core/src/stemma.o 


# Each subdirectory must supply rules for building sources it contributes
Drivers/Stemma/Core/src/%.o Drivers/Stemma/Core/src/%.su: ../Drivers/Stemma/Core/src/%.c Drivers/Stemma/Core/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H750xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/Drivers/ADC1115/Core/Inc" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/Drivers/BNO055/Core/Inc" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/Drivers/HX711/Core/Inc" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/Drivers/Stemma/Core/inc" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/Drivers/VL53L1x/Core/Inc" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/Drivers/VL53L1x/Platform/Inc" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/System" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/System/RoCo/Src" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/System/Lang" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/System/RoCo/Src/Build" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/System/RoCo/Src/Protocol" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/System/Sensors" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/System/Misc" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-Stemma-2f-Core-2f-src

clean-Drivers-2f-Stemma-2f-Core-2f-src:
	-$(RM) ./Drivers/Stemma/Core/src/stemma.d ./Drivers/Stemma/Core/src/stemma.o ./Drivers/Stemma/Core/src/stemma.su

.PHONY: clean-Drivers-2f-Stemma-2f-Core-2f-src

