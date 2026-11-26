AL=arm-elf-ld
AS=arm-elf-as
AR=arm-elf-ar
NM=arm-elf-nm
OB=arm-elf-objcopy
CC=arm-elf-gcc
DMP=arm-elf-objdump

OFLAGS  = -O binary
TOOLCHAIN 	=	/usr/cross/arm-elf
TCHAIN_DIRS	=	$(TOOLCHAIN)/arm-elf/lib/ \
				$(TOOLCHAIN)/lib/gcc/arm-elf/4.1.1/


all: 
	$(MAKE) Demo.bin
	
clean:
	rm -f libclass.a init.o MMU.o _Uart.o ConsoleIf.o main.o ADC.o Timer.o TestButtons.o *.elf *.bin  *.map

init.o: init.s DRAMCfg.inc Addresses.inc ClkCfg.inc makefile
	$(AS) $< -mfpu=softfpa --gdwarf-2 -mcpu=arm920t -o $@
	
HAL.o: HAL.s makefile
	$(AS) $< -mfpu=softfpa --gdwarf-2 -mcpu=arm920t -o $@
		
main.o: main.c tests.h
	$(CC) -g -march=armv4 -c $< -o $@ 

TestButtons.o: TestButtons.c GPIORegs.h types.h Utils.h 
	$(CC) -g -march=armv4 -c $< -o $@

Timer.o: Timer.c Timer.h PWMRegs.h INTRRegs.h INTRDefs.h
	$(CC) -g -march=armv4 -c $< -o $@ 

ADC.o: ADC.c ADCRegs.h
	$(CC) -g -march=armv4 -c $< -o $@ 

MMU.o: MMU_Table.c MMU_Table.h
	$(CC) -g -march=armv4 -c $< -o $@ 	
			
libclass.a: ConsoleIf.o _Uart.o HAL.o
	arm-elf-ar crv  libclass.a  _Uart.o ConsoleIf.o 
	
Demo.elf: Demo.lds MMU.o init.o main.o libclass.a ADC.o Timer.o TestButtons.o makefile
	$(AL) -T $< -L$(TOOLCHAIN)/arm-elf/lib/ -L$(TOOLCHAIN)/lib/gcc/arm-elf/4.1.1/   -Map Demo.map -o $@
	
Demo.bin: Demo.elf
	$(OB) $< $(OFLAGS) $@
#	$(DMP) -S $< >Demo.dmp	
	@cp $@ u-boot.bin
	
	
