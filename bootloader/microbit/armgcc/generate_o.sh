#!/bin/sh
arm-none-eabi-objcopy -I ihex --output-target=binary _build/nrf52833_xxaa_s140.hex bootloader.bin
arm-none-eabi-objcopy --rename-section .data=.bootloader -I binary -O elf32-littlearm -B arm bootloader.bin bootloader.o

