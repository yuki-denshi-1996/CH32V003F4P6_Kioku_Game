################################################################################
# MRS Version: 1.9.1
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/CH32V003F4P6_Mylib.c \
../User/Kioku_game_program.c \
../User/ch32v00x_it.c \
../User/main.c \
../User/system_ch32v00x.c 

OBJS += \
./User/CH32V003F4P6_Mylib.o \
./User/Kioku_game_program.o \
./User/ch32v00x_it.o \
./User/main.o \
./User/system_ch32v00x.o 

C_DEPS += \
./User/CH32V003F4P6_Mylib.d \
./User/Kioku_game_program.d \
./User/ch32v00x_it.d \
./User/main.d \
./User/system_ch32v00x.d 


# Each subdirectory must supply rules for building sources it contributes
User/%.o: ../User/%.c
	@	@	riscv-none-embed-gcc -march=rv32ecxw -mabi=ilp32e -msmall-data-limit=0 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"C:\share\MounRiver(WCH)\CH32V003F4P6\CH32V003F4P6_kioku_game_2024\Debug" -I"C:\share\MounRiver(WCH)\CH32V003F4P6\CH32V003F4P6_kioku_game_2024\Core" -I"C:\share\MounRiver(WCH)\CH32V003F4P6\CH32V003F4P6_kioku_game_2024\User" -I"C:\share\MounRiver(WCH)\CH32V003F4P6\CH32V003F4P6_kioku_game_2024\Peripheral\inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

