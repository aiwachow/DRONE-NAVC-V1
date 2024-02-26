################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Drivers/ADXL345.c \
../Core/Src/Drivers/BMI088.c \
../Core/Src/Drivers/ICM_42688_P.c \
../Core/Src/Drivers/LPS22HB.c \
../Core/Src/Drivers/LPS25HB_I2C.c \
../Core/Src/Drivers/QMC5883L.c \
../Core/Src/Drivers/SPL06.c 

OBJS += \
./Core/Src/Drivers/ADXL345.o \
./Core/Src/Drivers/BMI088.o \
./Core/Src/Drivers/ICM_42688_P.o \
./Core/Src/Drivers/LPS22HB.o \
./Core/Src/Drivers/LPS25HB_I2C.o \
./Core/Src/Drivers/QMC5883L.o \
./Core/Src/Drivers/SPL06.o 

C_DEPS += \
./Core/Src/Drivers/ADXL345.d \
./Core/Src/Drivers/BMI088.d \
./Core/Src/Drivers/ICM_42688_P.d \
./Core/Src/Drivers/LPS22HB.d \
./Core/Src/Drivers/LPS25HB_I2C.d \
./Core/Src/Drivers/QMC5883L.d \
./Core/Src/Drivers/SPL06.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Drivers/%.o Core/Src/Drivers/%.su Core/Src/Drivers/%.cyclo: ../Core/Src/Drivers/%.c Core/Src/Drivers/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L476xx -c -I../Core/Inc -I"E:/STM32/workspace_1.12.1_Projects/Attitude_Estimation/Core/Inc/Estimation" -I"E:/STM32/workspace_1.12.1_Projects/Attitude_Estimation/Core/Inc/Drivers" -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Drivers

clean-Core-2f-Src-2f-Drivers:
	-$(RM) ./Core/Src/Drivers/ADXL345.cyclo ./Core/Src/Drivers/ADXL345.d ./Core/Src/Drivers/ADXL345.o ./Core/Src/Drivers/ADXL345.su ./Core/Src/Drivers/BMI088.cyclo ./Core/Src/Drivers/BMI088.d ./Core/Src/Drivers/BMI088.o ./Core/Src/Drivers/BMI088.su ./Core/Src/Drivers/ICM_42688_P.cyclo ./Core/Src/Drivers/ICM_42688_P.d ./Core/Src/Drivers/ICM_42688_P.o ./Core/Src/Drivers/ICM_42688_P.su ./Core/Src/Drivers/LPS22HB.cyclo ./Core/Src/Drivers/LPS22HB.d ./Core/Src/Drivers/LPS22HB.o ./Core/Src/Drivers/LPS22HB.su ./Core/Src/Drivers/LPS25HB_I2C.cyclo ./Core/Src/Drivers/LPS25HB_I2C.d ./Core/Src/Drivers/LPS25HB_I2C.o ./Core/Src/Drivers/LPS25HB_I2C.su ./Core/Src/Drivers/QMC5883L.cyclo ./Core/Src/Drivers/QMC5883L.d ./Core/Src/Drivers/QMC5883L.o ./Core/Src/Drivers/QMC5883L.su ./Core/Src/Drivers/SPL06.cyclo ./Core/Src/Drivers/SPL06.d ./Core/Src/Drivers/SPL06.o ./Core/Src/Drivers/SPL06.su

.PHONY: clean-Core-2f-Src-2f-Drivers

