################################################################################
# MRS Version: 1.9.1
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Debug/debug.c 

OBJS += \
./Debug/debug.o 

C_DEPS += \
./Debug/debug.d 


# Each subdirectory must supply rules for building sources it contributes
Debug/%.o: ../Debug/%.c
	@	@	riscv-none-embed-gcc -march=rv32ecxw -mabi=ilp32e -msmall-data-limit=0 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"C:\share\MounRiver(WCH)\CH32V003F4P6\CH32V003F4P6_kioku_game_2024\Debug" -I"C:\share\MounRiver(WCH)\CH32V003F4P6\CH32V003F4P6_kioku_game_2024\Core" -I"C:\share\MounRiver(WCH)\CH32V003F4P6\CH32V003F4P6_kioku_game_2024\User" -I"C:\share\MounRiver(WCH)\CH32V003F4P6\CH32V003F4P6_kioku_game_2024\Peripheral\inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

