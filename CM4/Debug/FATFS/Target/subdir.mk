################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../FATFS/Target/user_diskio.c 

C_DEPS += \
./FATFS/Target/user_diskio.d 

OBJS += \
./FATFS/Target/user_diskio.o 


# Each subdirectory must supply rules for building sources it contributes
FATFS/Target/user_diskio.o: ../FATFS/Target/user_diskio.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DCORE_CM4 -DDEBUG -DSTM32H745xx -c -I/Users/arion/git/XploreAvionics/System -I/Users/arion/git/XploreAvionics/CM4/Core/Inc -I/Users/arion/git/XploreAvionics/CM4/FATFS/Target -I/Users/arion/git/XploreAvionics/CM4/FATFS/App -I/Users/arion/git/XploreAvionics/Drivers/STM32H7xx_HAL_Driver/Inc -I/Users/arion/git/XploreAvionics/Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I/Users/arion/git/XploreAvionics/Middlewares/Third_Party/FatFs/src -I/Users/arion/git/XploreAvionics/Middlewares/Third_Party/FreeRTOS/Source/include -I/Users/arion/git/XploreAvionics/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I/Users/arion/git/XploreAvionics/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I/Users/arion/git/XploreAvionics/Drivers/CMSIS/Device/ST/STM32H7xx/Include -I/Users/arion/git/XploreAvionics/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"FATFS/Target/user_diskio.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

