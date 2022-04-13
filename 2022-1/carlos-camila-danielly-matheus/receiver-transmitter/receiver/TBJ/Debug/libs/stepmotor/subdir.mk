################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../libs/stepmotor/step.c 

OBJS += \
./libs/stepmotor/step.o 

C_DEPS += \
./libs/stepmotor/step.d 


# Each subdirectory must supply rules for building sources it contributes
libs/stepmotor/%.o: ../libs/stepmotor/%.c libs/stepmotor/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G0B1xx -c -I../Core/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -I"/home/gabrielly/Downloads/projeto final/TBJ/libs/nrf24" -I"/home/gabrielly/Downloads/projeto final/TBJ/aplication" -I"/home/gabrielly/Downloads/projeto final/TBJ/libs/stepmotor" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-libs-2f-stepmotor

clean-libs-2f-stepmotor:
	-$(RM) ./libs/stepmotor/step.d ./libs/stepmotor/step.o

.PHONY: clean-libs-2f-stepmotor

