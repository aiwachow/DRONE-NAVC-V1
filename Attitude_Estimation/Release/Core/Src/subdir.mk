################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/ADXL345.c \
../Core/Src/Altimeter.c \
../Core/Src/BMI088.c \
../Core/Src/ICM_42688_P.c \
../Core/Src/LPS22HB.c \
../Core/Src/LPS25HB_I2C.c \
../Core/Src/RollPitch.c \
../Core/Src/SPL06.c \
../Core/Src/gpio.c \
../Core/Src/i2c.c \
../Core/Src/main.c \
../Core/Src/stm32l4xx_hal_msp.c \
../Core/Src/stm32l4xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32l4xx.c \
../Core/Src/tim.c \
../Core/Src/usart.c 

OBJS += \
./Core/Src/ADXL345.o \
./Core/Src/Altimeter.o \
./Core/Src/BMI088.o \
./Core/Src/ICM_42688_P.o \
./Core/Src/LPS22HB.o \
./Core/Src/LPS25HB_I2C.o \
./Core/Src/RollPitch.o \
./Core/Src/SPL06.o \
./Core/Src/gpio.o \
./Core/Src/i2c.o \
./Core/Src/main.o \
./Core/Src/stm32l4xx_hal_msp.o \
./Core/Src/stm32l4xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32l4xx.o \
./Core/Src/tim.o \
./Core/Src/usart.o 

C_DEPS += \
./Core/Src/ADXL345.d \
./Core/Src/Altimeter.d \
./Core/Src/BMI088.d \
./Core/Src/ICM_42688_P.d \
./Core/Src/LPS22HB.d \
./Core/Src/LPS25HB_I2C.d \
./Core/Src/RollPitch.d \
./Core/Src/SPL06.d \
./Core/Src/gpio.d \
./Core/Src/i2c.d \
./Core/Src/main.d \
./Core/Src/stm32l4xx_hal_msp.d \
./Core/Src/stm32l4xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32l4xx.d \
./Core/Src/tim.d \
./Core/Src/usart.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -c -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/ADXL345.cyclo ./Core/Src/ADXL345.d ./Core/Src/ADXL345.o ./Core/Src/ADXL345.su ./Core/Src/Altimeter.cyclo ./Core/Src/Altimeter.d ./Core/Src/Altimeter.o ./Core/Src/Altimeter.su ./Core/Src/BMI088.cyclo ./Core/Src/BMI088.d ./Core/Src/BMI088.o ./Core/Src/BMI088.su ./Core/Src/ICM_42688_P.cyclo ./Core/Src/ICM_42688_P.d ./Core/Src/ICM_42688_P.o ./Core/Src/ICM_42688_P.su ./Core/Src/LPS22HB.cyclo ./Core/Src/LPS22HB.d ./Core/Src/LPS22HB.o ./Core/Src/LPS22HB.su ./Core/Src/LPS25HB_I2C.cyclo ./Core/Src/LPS25HB_I2C.d ./Core/Src/LPS25HB_I2C.o ./Core/Src/LPS25HB_I2C.su ./Core/Src/RollPitch.cyclo ./Core/Src/RollPitch.d ./Core/Src/RollPitch.o ./Core/Src/RollPitch.su ./Core/Src/SPL06.cyclo ./Core/Src/SPL06.d ./Core/Src/SPL06.o ./Core/Src/SPL06.su ./Core/Src/gpio.cyclo ./Core/Src/gpio.d ./Core/Src/gpio.o ./Core/Src/gpio.su ./Core/Src/i2c.cyclo ./Core/Src/i2c.d ./Core/Src/i2c.o ./Core/Src/i2c.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/stm32l4xx_hal_msp.cyclo ./Core/Src/stm32l4xx_hal_msp.d ./Core/Src/stm32l4xx_hal_msp.o ./Core/Src/stm32l4xx_hal_msp.su ./Core/Src/stm32l4xx_it.cyclo ./Core/Src/stm32l4xx_it.d ./Core/Src/stm32l4xx_it.o ./Core/Src/stm32l4xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32l4xx.cyclo ./Core/Src/system_stm32l4xx.d ./Core/Src/system_stm32l4xx.o ./Core/Src/system_stm32l4xx.su ./Core/Src/tim.cyclo ./Core/Src/tim.d ./Core/Src/tim.o ./Core/Src/tim.su ./Core/Src/usart.cyclo ./Core/Src/usart.d ./Core/Src/usart.o ./Core/Src/usart.su

.PHONY: clean-Core-2f-Src

