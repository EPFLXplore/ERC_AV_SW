################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../System/BRoCo/Src/BufferedIODriver.cpp \
../System/BRoCo/Src/EspressifUARTDriver.cpp \
../System/BRoCo/Src/I2CBus.cpp \
../System/BRoCo/Src/I2CDevice.cpp \
../System/BRoCo/Src/I2CDriver.cpp \
../System/BRoCo/Src/IOBus.cpp \
../System/BRoCo/Src/LWIPClientIO.cpp \
../System/BRoCo/Src/LoopbackDriver.cpp \
../System/BRoCo/Src/MessageBus.cpp \
../System/BRoCo/Src/NetworkBus.cpp \
../System/BRoCo/Src/NetworkClientIO.cpp \
../System/BRoCo/Src/NetworkServerIO.cpp \
../System/BRoCo/Src/RadioBus.cpp \
../System/BRoCo/Src/RoCanDriver.cpp \
../System/BRoCo/Src/STMUARTDriver.cpp \
../System/BRoCo/Src/UDevDriver.cpp \
../System/BRoCo/Src/ZephyrUARTDriver.cpp 

OBJS += \
./System/BRoCo/Src/BufferedIODriver.o \
./System/BRoCo/Src/EspressifUARTDriver.o \
./System/BRoCo/Src/I2CBus.o \
./System/BRoCo/Src/I2CDevice.o \
./System/BRoCo/Src/I2CDriver.o \
./System/BRoCo/Src/IOBus.o \
./System/BRoCo/Src/LWIPClientIO.o \
./System/BRoCo/Src/LoopbackDriver.o \
./System/BRoCo/Src/MessageBus.o \
./System/BRoCo/Src/NetworkBus.o \
./System/BRoCo/Src/NetworkClientIO.o \
./System/BRoCo/Src/NetworkServerIO.o \
./System/BRoCo/Src/RadioBus.o \
./System/BRoCo/Src/RoCanDriver.o \
./System/BRoCo/Src/STMUARTDriver.o \
./System/BRoCo/Src/UDevDriver.o \
./System/BRoCo/Src/ZephyrUARTDriver.o 

CPP_DEPS += \
./System/BRoCo/Src/BufferedIODriver.d \
./System/BRoCo/Src/EspressifUARTDriver.d \
./System/BRoCo/Src/I2CBus.d \
./System/BRoCo/Src/I2CDevice.d \
./System/BRoCo/Src/I2CDriver.d \
./System/BRoCo/Src/IOBus.d \
./System/BRoCo/Src/LWIPClientIO.d \
./System/BRoCo/Src/LoopbackDriver.d \
./System/BRoCo/Src/MessageBus.d \
./System/BRoCo/Src/NetworkBus.d \
./System/BRoCo/Src/NetworkClientIO.d \
./System/BRoCo/Src/NetworkServerIO.d \
./System/BRoCo/Src/RadioBus.d \
./System/BRoCo/Src/RoCanDriver.d \
./System/BRoCo/Src/STMUARTDriver.d \
./System/BRoCo/Src/UDevDriver.d \
./System/BRoCo/Src/ZephyrUARTDriver.d 


# Each subdirectory must supply rules for building sources it contributes
System/BRoCo/Src/%.o System/BRoCo/Src/%.su System/BRoCo/Src/%.cyclo: ../System/BRoCo/Src/%.cpp System/BRoCo/Src/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H750xx -c -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"C:/Users/ybakk/OneDrive/Documents/Xplore/AV_SW_workspace/Orion_FW/Core/Inc" -I"C:/Users/ybakk/OneDrive/Documents/Xplore/AV_SW_workspace/Orion_FW/System" -I"C:/Users/ybakk/OneDrive/Documents/Xplore/AV_SW_workspace/Orion_FW/System/BRoCo" -I"C:/Users/ybakk/OneDrive/Documents/Xplore/AV_SW_workspace/Orion_FW/System/Sensors" -I"C:/Users/ybakk/OneDrive/Documents/Xplore/AV_SW_workspace/Orion_FW/Drivers/ADC1115/Core/Inc" -I../Core/Inc -I"C:/Users/ybakk/OneDrive/Documents/Xplore/AV_SW_workspace/Orion_FW/Drivers/BMP280/Core/Inc" -I"C:/Users/ybakk/OneDrive/Documents/Xplore/AV_SW_workspace/Orion_FW/System/BRoCo/Src/Build" -I"C:/Users/ybakk/OneDrive/Documents/Xplore/AV_SW_workspace/Orion_FW/Drivers/MAX11615" -I"C:/Users/ybakk/OneDrive/Documents/Xplore/AV_SW_workspace/Orion_FW/Drivers/MAX11615/Core" -I"C:/Users/ybakk/OneDrive/Documents/Xplore/AV_SW_workspace/Orion_FW/Drivers/MAX11615/Core/Inc" -I"C:/Users/ybakk/OneDrive/Documents/Xplore/AV_SW_workspace/Orion_FW/System/Modbus_Lib/Inc" -I"C:/Users/ybakk/OneDrive/Documents/Xplore/AV_SW_workspace/Orion_FW/Drivers/ALLINONE/Core/Inc" -I"C:/Users/ybakk/OneDrive/Documents/Xplore/AV_SW_workspace/Orion_FW/System/Modbus_Lib/Src" -I"C:/Users/ybakk/OneDrive/Documents/Xplore/AV_SW_workspace/Orion_FW/System/Modbus_Lib/Config" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-System-2f-BRoCo-2f-Src

clean-System-2f-BRoCo-2f-Src:
	-$(RM) ./System/BRoCo/Src/BufferedIODriver.cyclo ./System/BRoCo/Src/BufferedIODriver.d ./System/BRoCo/Src/BufferedIODriver.o ./System/BRoCo/Src/BufferedIODriver.su ./System/BRoCo/Src/EspressifUARTDriver.cyclo ./System/BRoCo/Src/EspressifUARTDriver.d ./System/BRoCo/Src/EspressifUARTDriver.o ./System/BRoCo/Src/EspressifUARTDriver.su ./System/BRoCo/Src/I2CBus.cyclo ./System/BRoCo/Src/I2CBus.d ./System/BRoCo/Src/I2CBus.o ./System/BRoCo/Src/I2CBus.su ./System/BRoCo/Src/I2CDevice.cyclo ./System/BRoCo/Src/I2CDevice.d ./System/BRoCo/Src/I2CDevice.o ./System/BRoCo/Src/I2CDevice.su ./System/BRoCo/Src/I2CDriver.cyclo ./System/BRoCo/Src/I2CDriver.d ./System/BRoCo/Src/I2CDriver.o ./System/BRoCo/Src/I2CDriver.su ./System/BRoCo/Src/IOBus.cyclo ./System/BRoCo/Src/IOBus.d ./System/BRoCo/Src/IOBus.o ./System/BRoCo/Src/IOBus.su ./System/BRoCo/Src/LWIPClientIO.cyclo ./System/BRoCo/Src/LWIPClientIO.d ./System/BRoCo/Src/LWIPClientIO.o ./System/BRoCo/Src/LWIPClientIO.su ./System/BRoCo/Src/LoopbackDriver.cyclo ./System/BRoCo/Src/LoopbackDriver.d ./System/BRoCo/Src/LoopbackDriver.o ./System/BRoCo/Src/LoopbackDriver.su ./System/BRoCo/Src/MessageBus.cyclo ./System/BRoCo/Src/MessageBus.d ./System/BRoCo/Src/MessageBus.o ./System/BRoCo/Src/MessageBus.su ./System/BRoCo/Src/NetworkBus.cyclo ./System/BRoCo/Src/NetworkBus.d ./System/BRoCo/Src/NetworkBus.o ./System/BRoCo/Src/NetworkBus.su ./System/BRoCo/Src/NetworkClientIO.cyclo ./System/BRoCo/Src/NetworkClientIO.d ./System/BRoCo/Src/NetworkClientIO.o ./System/BRoCo/Src/NetworkClientIO.su ./System/BRoCo/Src/NetworkServerIO.cyclo ./System/BRoCo/Src/NetworkServerIO.d ./System/BRoCo/Src/NetworkServerIO.o ./System/BRoCo/Src/NetworkServerIO.su ./System/BRoCo/Src/RadioBus.cyclo ./System/BRoCo/Src/RadioBus.d ./System/BRoCo/Src/RadioBus.o ./System/BRoCo/Src/RadioBus.su ./System/BRoCo/Src/RoCanDriver.cyclo ./System/BRoCo/Src/RoCanDriver.d ./System/BRoCo/Src/RoCanDriver.o ./System/BRoCo/Src/RoCanDriver.su ./System/BRoCo/Src/STMUARTDriver.cyclo ./System/BRoCo/Src/STMUARTDriver.d ./System/BRoCo/Src/STMUARTDriver.o ./System/BRoCo/Src/STMUARTDriver.su ./System/BRoCo/Src/UDevDriver.cyclo ./System/BRoCo/Src/UDevDriver.d ./System/BRoCo/Src/UDevDriver.o ./System/BRoCo/Src/UDevDriver.su ./System/BRoCo/Src/ZephyrUARTDriver.cyclo ./System/BRoCo/Src/ZephyrUARTDriver.d ./System/BRoCo/Src/ZephyrUARTDriver.o ./System/BRoCo/Src/ZephyrUARTDriver.su

.PHONY: clean-System-2f-BRoCo-2f-Src

