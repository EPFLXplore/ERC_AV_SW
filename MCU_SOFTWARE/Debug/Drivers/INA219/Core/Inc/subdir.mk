################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Drivers/INA219/Core/Inc/ina219.cpp 

OBJS += \
./Drivers/INA219/Core/Inc/ina219.o 

CPP_DEPS += \
./Drivers/INA219/Core/Inc/ina219.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/INA219/Core/Inc/%.o Drivers/INA219/Core/Inc/%.su: ../Drivers/INA219/Core/Inc/%.cpp Drivers/INA219/Core/Inc/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H750xx -c -I../Core/Inc -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/MCU_SOFTWARE/Drivers/ADC1115/Core/Src" -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/MCU_SOFTWARE/Drivers/ADC1115/Core/Inc" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/MCU_SOFTWARE/Drivers/BNO055/Core/Inc" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/MCU_SOFTWARE/Drivers/HX711/Core/Inc" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/MCU_SOFTWARE/Drivers/Stemma/Core/inc" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/MCU_SOFTWARE/Drivers/VL53L1x/Core/Inc" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/MCU_SOFTWARE/Drivers/VL53L1x/Platform/Inc" -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/MCU_SOFTWARE/System/Sensors" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/MCU_SOFTWARE/System" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/MCU_SOFTWARE/System/RoCo/Src/Build" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/MCU_SOFTWARE/System/RoCo/Src" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/MCU_SOFTWARE/System/RoCo/Src/Protocol" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/MCU_SOFTWARE/Drivers/BNO055" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/MCU_SOFTWARE/Drivers/ADC1115" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/MCU_SOFTWARE/Drivers/Stemma" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/MCU_SOFTWARE/Drivers/VL53L1x" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/MCU_SOFTWARE/Drivers" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/MCU_SOFTWARE/System/Peripherals" -I"H:/My Drive/EPFL/EPFL Xplore/AV_Soft/MCU_SOFTWARE/Drivers/INA219/Core/Src" -O1 -ffunction-sections -fdata-sections -fno-exceptions -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-INA219-2f-Core-2f-Inc

clean-Drivers-2f-INA219-2f-Core-2f-Inc:
	-$(RM) ./Drivers/INA219/Core/Inc/ina219.d ./Drivers/INA219/Core/Inc/ina219.o ./Drivers/INA219/Core/Inc/ina219.su

.PHONY: clean-Drivers-2f-INA219-2f-Core-2f-Inc

