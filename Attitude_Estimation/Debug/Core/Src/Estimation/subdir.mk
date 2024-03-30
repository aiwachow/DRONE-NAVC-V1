################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Estimation/Altimeter.c \
../Core/Src/Estimation/RollPitchYaw.c 

OBJS += \
./Core/Src/Estimation/Altimeter.o \
./Core/Src/Estimation/RollPitchYaw.o 

C_DEPS += \
./Core/Src/Estimation/Altimeter.d \
./Core/Src/Estimation/RollPitchYaw.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Estimation/%.o Core/Src/Estimation/%.su Core/Src/Estimation/%.cyclo: ../Core/Src/Estimation/%.c Core/Src/Estimation/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L476xx -c -I../Core/Inc -I"E:/STM32/workspace_1.12.1_Projects/Attitude_Estimation/Core/Inc/Estimation" -I"E:/STM32/workspace_1.12.1_Projects/Attitude_Estimation/Core/Inc/Drivers" -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Estimation

clean-Core-2f-Src-2f-Estimation:
	-$(RM) ./Core/Src/Estimation/Altimeter.cyclo ./Core/Src/Estimation/Altimeter.d ./Core/Src/Estimation/Altimeter.o ./Core/Src/Estimation/Altimeter.su ./Core/Src/Estimation/RollPitchYaw.cyclo ./Core/Src/Estimation/RollPitchYaw.d ./Core/Src/Estimation/RollPitchYaw.o ./Core/Src/Estimation/RollPitchYaw.su

.PHONY: clean-Core-2f-Src-2f-Estimation

