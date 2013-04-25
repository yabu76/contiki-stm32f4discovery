CONTIKI_PROJECT = hello-world
all: $(CONTIKI_PROJECT) hex.hex

hex.hex:
	arm-none-eabi-objcopy.exe -O ihex hello-world-nosyms.stm32f4discovery hex.hex

flash:
	ST-LINK_CLI.exe -P hex.hex -V -Rst

#UIP_CONF_IPV6=1

CONTIKI = ../..
include $(CONTIKI)/Makefile.include
