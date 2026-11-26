/*
=========================================
NAME: INIT.S
DESC: Assembly start up codes
      Configure memory, ISR ,stacks
	  Jump to C code.

*/
.include "ClkCfg.inc"
.include "DRAMCfg.inc"
.include "Addresses.inc"

BIT_SELFREFRESH =	(1<<22)

/* Pre-defined constants */
USERMODE   =  	0x10
FIQMODE    =  	0x11
IRQMODE    =  	0x12
SVCMODE    =  	0x13
ABORTMODE  =  	0x17
UNDEFMODE  =  	0x1b
SYSMODE    =    0x1f
MODEMASK   =  	0x1f
NOINT      =  	0xc0


UPLLCON_VAL = ((U_MDIV<<12)+(U_PDIV<<4)+U_SDIV)
MPLLCON_VAL = ((M_MDIV<<12)+(M_PDIV<<4)+M_SDIV)

/*
The location of stacks
*/
UserStack =	(_STACK_BASEADDRESS-0x3800)	/*0x33ff4800~*/
SysStack  =	(_STACK_BASEADDRESS-0x3800)	/*0x33ff4800~ - similar to user stack */
SVCStack  =	(_STACK_BASEADDRESS-0x2800)	/*0x33ff5800 ~*/
UndefStack=	(_STACK_BASEADDRESS-0x2400)	/*0x33ff5c00 ~*/
AbortStack=	(_STACK_BASEADDRESS-0x2000)	/*0x33ff6000 ~*/
IRQStack  =	(_STACK_BASEADDRESS-0x1000)	/*0x33ff7000 ~*/
FIQStack  =	(_STACK_BASEADDRESS-0x0)	/*0x33ff8000 ~*/


.macro HALT
HaltProc:
	ldr	r0,=GPBDAT
	ldr	r1,=0x079e
	str	r1,[r0]
    b HaltProc
.endm


.text
.extern user_main
.extern halt_on_und
.extern halt_on_pabrt
.extern halt_on_dabrt
.extern GPOS_Init
.extern Nand_Minimal_CopyProgramFromNand
.extern _Uart_Init

__start: .global __start

__start:
    b	ResetHandler
	ldr pc, =halt_on_und /*b	.	*//*handler for Undefined mode*/
    ldr pc, _IsrSWI /*handler for software interrupts*/
	ldr pc, =halt_on_pabrt	/*handler for PAbort*/
	ldr pc, =halt_on_dabrt /*ldr pc, _IsrSWI	*//*handler for DAbort*/
	b	.	/*reserved*/
	ldr pc, _IsrIRQ /*handler for IRQ interrupt*/
	b	.	/*handler for FIQ interrupt*/

_IsrIRQ: .word IsrIRQ
IsrIRQ:

	sub	sp,sp,#4       @reserved for PC
	stmfd	sp!,{r8-r9}

	ldr	r9,=INTOFFSET
	ldr	r9,[r9]
	ldr	r8,=HandleEINT0
	add	r8,r8,r9,lsl #2
	ldr	r8,[r8]
	str	r8,[sp,#8]
	ldmfd	sp!,{r8-r9,pc}

_IsrSWI: .word IsrSWI
IsrSWI:
	stmfd	 sp!,{r0-r2, lr}

	LDR		 r2,[lr,#-4]
	BIC		 r2,r2,#0xff000000
	MOV      r0,#0x56000000
	LDR      r1,[r0,#0x10]
	BIC      r1,r1,#0xc00
	STR      r1,[r0,#0x10]
	LDR      r1,[r0,#0x10]
	ORR      r1,r1,#0x400
	STR      r1,[r0,#0x10]
	LDR      r1,Lable_IsrSwi_1
	STR      r1,[r0,#0x18]
	LDR      r1,[r0,#0x14]
	BIC      r1,r1,#0x20
	STR      r1,[r0,#0x14]
SwiReturnToSVCMode:
/*
	What should we write here ?
*/
	ldmfd	sp!,{r0-r2, pc}^

Lable_IsrSwi_1:
	.word    2047

IsrUndef:
	stmfd	sp!,{r0-r12, lr}
	ldmfd	sp!,{r0-r12,pc}^

ResetHandler:

	ldr	r0,=WTCON       /*watch dog disable*/
	ldr	r1,=0x0
	str	r1,[r0]

	ldr	r0,=INTMSK
	ldr	r1,=0xffffffff  /*all interrupt disable*/
	str	r1,[r0]

	ldr	r0,=INTSUBMSK
	ldr	r1,=0x7fff		/*all sub interrupt disable*/
	str	r1,[r0]


/*
TestModeSwitch:
	@Switch into user mode
	mrs	r0,cpsr
	bic	r0,r0,#MODEMASK

SwitchToUser:
	orr	r1,r0,#USERMODE
	msr	cpsr_cxsf,r1		@UserMode


BadSwitchBackToSVCMode:
	@Switch back into SVC mode and disable interrupts- should not work
	orr	r1,r0,#SVCMODE|NOINT
	msr	cpsr_cxsf,r1		@FIQMode


SwitchBackToSVCModeViaSWI:
	swi 0x12
*/

cpu_init_crit:
	/*
	 * flush v4 I/D caches
	 */
	mov	r0, #0
	mcr	p15, 0, r0, c7, c7, 0	/* flush v3/v4 cache */
	mcr	p15, 0, r0, c8, c7, 0	/* flush v4 TLB */

	/*
	 * disable MMU stuff and caches
	 */
	mrc	p15, 0, r0, c1, c0, 0
	bic	r0, r0, #0x00002300	@ clear bits 13, 9:8 (--V- --RS)
	bic	r0, r0, #0x00000087	@ clear bits 7, 2:0 (B--- -CAM)
	orr	r0, r0, #0x00000002	@ set bit 2 (A) Align
	orr	r0, r0, #0x00001000	@ set bit 12 (I) I-Cache
	mcr	p15, 0, r0, c1, c0, 0


	/*rGPFDAT = (rGPFDAT & ~(0xf<<4)) | ((~data & 0xf)<<4)*/
	/* Led_Display */

	ldr	r0,=GPBCON
	ldr	r1,=0x00555555
	str	r1,[r0]

	ldr	r0,=GPBUP
	ldr	r1,=0x07ff
	str	r1,[r0]

	ldr	r0,=GPBDAT
	ldr	r1,=0x061e
	str	r1,[r0]

	/*swi 10*/

	/* Added for confirm clock divide. for 2440.
	 Setting value Fclk:Hclk:Pclk
	*/
	ldr	r0,=CLKDIVN
	ldr	r1,=CLKDIV_VAL		/* 0=1:1:1, 1=1:1:2, 2=1:2:2, 3=1:2:4, 4=1:4:4, 5=1:4:8, 6=1:3:3, 7=1:3:6. */
	str	r1,[r0]

	/* CLKDIV_VAL = 5 	 means Fclk:Hclk is not 1:1. */
	@??? MMU_SetAsyncBusMode - not supported for s3c2440 ??? page. 243
	mrc p15,0,r0,c1,c0,0
	orr r0,r0,#0xc0000000 	/*R1_nF:OR:R1_iA*/
	mcr p15,0,r0,c1,c0,0

	/* Configure UPLL */
	ldr	r0,=UPLLCON
	ldr	r1,=UPLLCON_VAL
	str	r1,[r0]
	nop	/* Caution: After UPLL setting, at least 7-clocks delay must be inserted for setting hardware be completed. */
	nop
	nop
	nop
	nop
	nop
	nop
	/* Configure MPLL */
	ldr	r0,=MPLLCON
	ldr	r1,=MPLLCON_VAL  /* Fin=16.9344MHz */
	str	r1,[r0]


InitializeMemoryController:
	/* Set memory control registers */
 	adrl	r0, SMRDATA	/* be careful! */
	ldr	r1,=BWSCON	/*BWSCON Address */
	add	r2, r0, #52	/*End address of SMRDATA */

Lable_0.0:
	ldr	r3, [r0], #4
	str	r3, [r1], #4
	cmp	r2, r0
	bne	Lable_0.0


Lable_0.1:
 	@Initialize stacks
	bl	InitStacks


/*===========================================================*/
_secondary_boot_loader: .word 0

	ldr	r0, =BWSCON
	ldr	r0, [r0]
	ands	r0, r0, #6		@OM[1:0] != 0, NOR FLash boot
	bne	copy_proc_beg		@do not read nand flash

	adr	r0, __start		    @OM[1:0] == 0, NAND FLash boot
	cmp	r0, #0				@if use Multi-ice, then again addres is not 0.
	beq nand_boot_beg

	ldr r1, =_secondary_boot_loader
	mov r2,#1
	str	r2,[r1]
relocate_boot_vector:
	ldr r3, __start
	mov	r4, #0
fill_one_word_in_boot_vector:	
	ldmia	r0!, {r3}	
	stmia	r4!, {r3}
	cmp r4,#40
	bcc fill_one_word_in_boot_vector
	
	b copy_proc_beg		@do not read nand flash for boot

/*===========================================================*/
nand_boot_beg:
	bl Nand_Minimal_CopyProgramFromNand
copy_proc_beg:
	adr	r0, __start
	ldr	r2, BaseOfROM
	cmp	r0, r2
    beq Lable_2.0
	ldr r3, TopOfROM /* If not running from address 0 */
Lable_1.0:
	ldmia	r0!, {r4-r7}
	stmia	r2!, {r4-r7}
	cmp	r2, r3
	bcc	Lable_1.0

	ldr	pc, =Lable_2.0		@goto compiler address

Lable_2.0:
/*===========================================================*/
  	@ Setup IRQ handler
	ldr	r0,=HandleIRQ
	ldr	r1,=IsrIRQ
	str	r1,[r0]

	@ Setup SWI handler
	ldr	r0,=HandleSWI
	ldr	r1,=IsrSWI
	str	r1,[r0]
    /*Write the initial value into TCNTBn*/
	ldr	r0,=HandleUndef
	ldr	r1,=IsrUndef
	str	r1,[r0]


/*===========================================================*/
    @ Initialize CLOCK - 400 MGHZ, PCLK=50000000, HCLK=FCLK/4 PCLK=HCLK/2
    mov r0,#0
    mov r1,#92
    orr r0, r0, r1, lsl #12
    mov r1,#1
    orr r0, r0, r1, lsl #4
    mov r1,#1
    orr r0, r0, r1
    ldr r1,=MPLLCON
    str r0,[r1]

    mov r0,#0
    mov r1,#2
    orr r0, r0, r1, lsl #1
    mov r1,#1
    orr r0, r0, r1
    ldr r1,=CLKDIVN
    str r0,[r1]

    @ Initialize Timer4 for RT interrupt each 10ms

    @ Initialize UART 0 for console IO
	/* gpio UART0 init */
	ldr	r0, =0x56000070
	mov	r1, #0xaa
	str	r1, [r0]

    ldr r0,=50000000
    ldr r1,=115200
    ldr r2,=0
	bl _Uart_Init


	/* Initialize MMU if needed */
	/* 
	   This is a seconday boot loader only if switch is set NAND but this image 
	       is loaded by an existing first level boot loader.
	*/     
	ldr r0, =_secondary_boot_loader
	ldr r0,[r0]
	cmp r0,#0
	bleq MMU_Table_Init_MMU /*Set MMU only if NOR mode */
	


/* Switch to user mode */
/*
	mrs	r0,cpsr
	bic	r0,r0,#MODEMASK
	orr	r1,r0,#USERMODE
	msr	cpsr_cxsf,r1		@UserMode
*/

/* Switch to system mode */
/*
	mrs	r0,cpsr
	bic	r0,r0,#MODEMASK
	orr	r1,r0,#SYSMODE
	msr	cpsr_cxsf,r1		@SysMode
*/	
	
/* Show that in user-mode, we can't switch back to other mode
   nor can we play with interrupts */
/*   	
	mrs	r0,cpsr
	bic	r0,r0,#MODEMASK
	orr	r1,r0,#SVCMODE|NOINT
	msr	cpsr_cxsf,r1		@SvsMode
*/	
    /*Jump from here to main C routine.*/
	b main

/*function initializing stacks*/
InitStacks:
	@Do not use DRAM,such as stmfd,ldmfd......
	@SVCstack is initialized before
	@Under toolkit ver 2.5, 'msr cpsr,r1' can be used instead of 'msr cpsr_cxsf,r1'
	mrs	r0,cpsr
	bic	r0,r0,#MODEMASK
	orr	r1,r0,#UNDEFMODE|NOINT
	msr	cpsr_cxsf,r1		@UndefMode
	ldr	sp,=UndefStack		@ UndefStack=0x33FF_5C00

	orr	r1,r0,#ABORTMODE|NOINT
	msr	cpsr_cxsf,r1		@AbortMode
	ldr	sp,=AbortStack		@ AbortStack=0x33FF_6000

	orr	r1,r0,#IRQMODE|NOINT
	msr	cpsr_cxsf,r1		@IRQMode
	ldr	sp,=IRQStack		@ IRQStack=0x33FF_7000

	orr	r1,r0,#FIQMODE|NOINT
	msr	cpsr_cxsf,r1		@FIQMode
	ldr	sp,=FIQStack		@ FIQStack=0x33FF_8000

	orr	r1,r0,#SYSMODE|NOINT
	msr	cpsr_cxsf,r1		@SysMode
	ldr	sp,=SysStack		@ SysStack=0x33ff4800


	bic	r0,r0,#MODEMASK|NOINT
	orr	r1,r0,#SVCMODE
	msr	cpsr_cxsf,r1		@SVCMode
	ldr	sp,=SVCStack		@ SVCStack=0x33FF_5800

	@USER mode has not be initialized.

	mov	pc,lr
	@The LR register will not be valid if the current mode is not SVC mode.

.text
HaltFunc: .global HaltFunc
HALT

end: .global end
b .

.ltorg
SMRDATA:
/*
; Memory configuration should be optimized for best performance
; The following parameter is not optimized.
; Memory access cycle parameter strategy
; 1) The memory settings is  safe parameters even at HCLK=75Mhz.
; 2) SDRAM refresh period is for HCLK<=75Mhz.
*/

	.word (0+(B1_BWSCON<<4)+(B2_BWSCON<<8)+(B3_BWSCON<<12)+(B4_BWSCON<<16)+(B5_BWSCON<<20)+(B6_BWSCON<<24)+(B7_BWSCON<<28))
	.word ((B0_Tacs<<13)+(B0_Tcos<<11)+(B0_Tacc<<8)+(B0_Tcoh<<6)+(B0_Tah<<4)+(B0_Tacp<<2)+(B0_PMC))   @GCS0
	.word ((B1_Tacs<<13)+(B1_Tcos<<11)+(B1_Tacc<<8)+(B1_Tcoh<<6)+(B1_Tah<<4)+(B1_Tacp<<2)+(B1_PMC))   @GCS1
	.word ((B2_Tacs<<13)+(B2_Tcos<<11)+(B2_Tacc<<8)+(B2_Tcoh<<6)+(B2_Tah<<4)+(B2_Tacp<<2)+(B2_PMC))   @GCS2
	.word ((B3_Tacs<<13)+(B3_Tcos<<11)+(B3_Tacc<<8)+(B3_Tcoh<<6)+(B3_Tah<<4)+(B3_Tacp<<2)+(B3_PMC))   @GCS3
	.word ((B4_Tacs<<13)+(B4_Tcos<<11)+(B4_Tacc<<8)+(B4_Tcoh<<6)+(B4_Tah<<4)+(B4_Tacp<<2)+(B4_PMC))   @GCS4
	.word ((B5_Tacs<<13)+(B5_Tcos<<11)+(B5_Tacc<<8)+(B5_Tcoh<<6)+(B5_Tah<<4)+(B5_Tacp<<2)+(B5_PMC))   @GCS5
	.word ((B6_MT<<15)+(B6_Trcd<<2)+(B6_SCAN))    @GCS6
	.word ((B7_MT<<15)+(B7_Trcd<<2)+(B7_SCAN))    @GCS7
	.word ((REFEN<<23)+(TREFMD<<22)+(Trp<<20)+(Tsrc<<18)+(Tchr<<16)+REFCNT)

	.word 0x32	    @SCLK power saving mode, BANKSIZE 128M/128M

	.word 0x30	    @MRSR6 CL=3clk
	.word 0x30	    @MRSR7 CL=3clk


BaseOfROM:	.word __start
ROMSize:	.word _rom_init_area_size /* Size of Stepping stone actual code+data+bss */
TopOfROM:	.word _ebss_init /* End of Stepping stone actual code+data+bss */

.data
HandleReset    		= 0x33FFFF00
HandleUndef    		= 0x33FFFF04
HandleSWI    		= 0x33FFFF08
HandlePabort    	= 0x33FFFF0C
HandleDabort    	= 0x33FFFF10
HandleReserved    	= 0x33FFFF14
HandleIRQ    		= 0x33FFFF18
HandleFIQ    		= 0x33FFFF1C


@Do not use the label 'IntVectorTable',
@The value of IntVectorTable is different with the address you think it may be.
@IntVectorTable
@0x33FF_FF20

HandleEINT0    		= 0x33FFFF20
HandleEINT1    		= 0x33FFFF24
HandleEINT2    		= 0x33FFFF28
HandleEINT3    		= 0x33FFFF2C
HandleEINT4_7    	= 0x33FFFF30
HandleEINT8_23    	= 0x33FFFF34
HandleCAM    		= 0x33FFFF38		@ Added for 2440.
HandleBATFLT    	= 0x33FFFF3C
HandleTICK    		= 0x33FFFF40
HandleWDT    		= 0x33FFFF44
HandleTIMER0    	= 0x33FFFF48
HandleTIMER1    	= 0x33FFFF4C
HandleTIMER2    	= 0x33FFFF50
HandleTIMER3    	= 0x33FFFF54
HandleTIMER4    	= 0x33FFFF58
HandleUART2    		= 0x33FFFF5C
@0x33FF_FF60
HandleLCD    		= 0x33FFFF60
HandleDMA0    		= 0x33FFFF64
HandleDMA1    		= 0x33FFFF68
HandleDMA2    		= 0x33FFFF6C
HandleDMA3    		= 0x33FFFF70
HandleMMC    		= 0x33FFFF74
HandleSPI0    		= 0x33FFFF78
HandleUART1    		= 0x33FFFF7C
HandleNFCON    		= 0x33FFFF80		@ Added for 2440.
HandleUSBD    		= 0x33FFFF84
HandleUSBH    		= 0x33FFFF88
HandleIIC    		= 0x33FFFF8C
HandleUART0    		= 0x33FFFF90
HandleSPI1    		= 0x33FFFF94
HandleRTC    		= 0x33FFFF98
HandleADC    		= 0x33FFFF9C


.end

