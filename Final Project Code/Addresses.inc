/*;====================================================================
; File Name : 2440addr.a
; Function  : S3C2440 Define Address Register (Assembly)
; Date      : March 27, 2002
; Revision	: Programming start (February 18,2002) -> SOP
; Revision	: 03.11.2003 ver 0.0	Attatched for 2440
;====================================================================
*/
/*
        GBLL   BIG_ENDIAN__
BIG_ENDIAN__   SETL   {FALSE}
*/
/*
;=================
; Memory control
;=================
*/
 

BWSCON  	=     0x48000000     /*Bus width & wait status*/
BANKCON0	=     0x48000004     /*Boot ROM control*/
BANKCON1	=     0x48000008     /*BANK1 control*/
BANKCON2	=     0x4800000c     /*BANK2 control*/
BANKCON3	=     0x48000010     /*BANK3 control*/
BANKCON4	=     0x48000014     /*BANK4 control*/
BANKCON5	=     0x48000018     /*BANK5 control*/
BANKCON6	=     0x4800001c     /*BANK6 control*/
BANKCON7	=     0x48000020     /*BANK7 control*/
REFRESH 	=     0x48000024     /*DRAM/SDRAM refresh*/
BANKSIZE	=     0x48000028     /*Flexible Bank Size*/
MRSRB6  	=     0x4800002c     /*Mode register set for SDRAM Bank6*/
MRSRB7  	=     0x48000030     /*Mode register set for SDRAM Bank7*/

/*
;==========================
; CLOCK & POWER MANAGEMENT
;==========================
*/
LOCKTIME	=     0x4c000000     /*PLL lock time counter*/
MPLLCON 	=     0x4c000004     /*MPLL Control*/
UPLLCON 	=     0x4c000008     /*UPLL Control*/
CLKCON  	=     0x4c00000c     /*Clock generator control*/
CLKSLOW 	=     0x4c000010     /*Slow clock control*/
CLKDIVN 	=     0x4c000014     /*Clock divider control*/

/*
;=================
; PWM TIMER
;=================
*/
TCFG0		=	0x51000000		/*Timer configuration register0*/
TCFG1		=	0x51000004		/*Timer configuration register1*/
TCON		=	0x51000008		/*Timer control register*/
TCNTB0		=	0x5100000C		/*Timer 0 count buffer register*/
TCMPB0		=	0x51000010		/*Timer 0 compare buffer register*/
TCNTO0		=	0x51000014		/*Timer 0 observation register*/
TCNTB1		=	0x51000018		/*Timer 1 count buffer register*/
TCMPB1		=	0x5100001C		/*Timer 1 compare buffer register*/
TCNTO1		=	0x51000020		/*Timer 1 observation register*/
TCNTB2		=	0x51000024		/*Timer 2 count buffer register*/
TCMPB2		=	0x51000028		/*Timer 2 compare buffer register*/
TCNTO2		=	0x5100002C		/*Timer 2 observation register*/
TCNTB3		=	0x51000030		/*Timer 3 count buffer register*/
TCMPB3		=	0x51000034		/*Timer 3 compare buffer register*/
TCNTO3		=	0x51000038		/*Timer 3 observation register*/
TCNTB4		=	0x5100003C		/*Timer 4 count buffer register*/
TCNTO4		=	0x51000040		/*Timer 4 observation register*/



WTCNT 	    =     0x53000008       /*Eatch-dog timer count*/

/*
;=================
; INTERRUPT
;=================
*/
SRCPND   	=     0x4a000000    /*Interrupt request status*/
INTMOD   	=     0x4a000004    /*Interrupt mode control*/
INTMSK   	=     0x4a000008    /*Interrupt mask control*/
PRIORITY 	=     0x4a00000c    /*IRQ priority control           <-- May 06, 2002 SOP*/
INTPND   	=     0x4a000010    /*Interrupt request status*/
INTOFFSET	=     0x4a000014    /*Interruot request source offset*/
SUSSRCPND	=     0x4a000018    /*Sub source pending*/
INTSUBMSK	=     0x4a00001c    /*Interrupt sub mask*/

/*
;=================
; I/O PORT for LED
;=================
*/
GPBCON  	=     0x56000010     /*Port B control*/
GPBDAT  	=     0x56000014     /*Port B data*/
GPBUP  		=     0x56000018     /*Pull-up control B*/

GPFCON  	=     0x56000050     /*Port F control*/
GPFDAT  	=     0x56000054     /*Port F data*/
GPFUP   	=     0x56000058     /*Pull-up control F*/

GPGCON  	=     0x56000060     /*Port G control*/
GPGDAT  	=     0x56000064     /*Port G data*/
GPGUP  		=     0x56000068     /*Pull-up control G*/

/*Miscellaneous register */
MISCCR  	=     0x56000080     /*Miscellaneous control*/
DCKCON  	=     0x56000084     /*DCLK0/1 control*/
EXTINT0 	=     0x56000088     /*External interrupt control register 0*/
EXTINT1 	=     0x5600008c     /*External interrupt control register 1*/
EXTINT2 	=     0x56000090     /*External interrupt control register 2*/
EINTFLT0	=     0x56000094     /*Reserved*/
EINTFLT1	=     0x56000098     /*Reserved*/
EINTFLT2	=     0x5600009c     /*External interrupt filter control register 2*/
EINTFLT3	=     0x560000a0     /*External interrupt filter control register 3*/
EINTMASK	=     0x560000a4     /*External interrupt mask*/
EINTPEND	=     0x560000a8     /*External interrupt pending*/
GSTATUS0	=     0x560000ac     /*External pin status*/
GSTATUS1	=     0x560000b0     /*Chip ID(0x32440000)*/
GSTATUS2	=     0x560000b4     /*Reset type*/
GSTATUS3	=     0x560000b8     /*Saved data0(32-bit) before entering POWER_OFF mode*/
GSTATUS4	=     0x560000bc     /*Saved data1(32-bit) before entering POWER_OFF mode*/

/* Added for 2440					; DonGo */
MSLCON		=     0x560000cc     /*Memory sleep control register*/

/*
;=================
; WATCH DOG TIMER
;=================
*/
WTCON       =     0x53000000       /*Watch-dog timer mode*/
WTDAT 	    =     0x53000004       /*Watch-dog timer data*/
WTCNT 	    =     0x53000008       /*Eatch-dog timer count*/

/*
;=================
; Nand Flash
;=================
*/
NFCONF		=  		0x4E000000	/*NAND Flash configuration*/
NFCONT		=   	0x4E000004      /*NAND Flash control*/
NFCMD		=   	0x4E000008      /*NAND Flash command*/
NFADDR		=   	0x4E00000C      /*NAND Flash address*/
NFDATA		=   	0x4E000010      /*NAND Flash data*/
NFDATA8		=   	0x4E000010      /*NAND Flash data*/
NFMECCD0	=   	0x4E000014      /*NAND Flash ECC for Main Area*/
NFMECCD1	=   	0x4E000018
NFSECCD		=   	0x4E00001C	/*NAND Flash ECC for Spare Area*/
NFSTAT		=   	0x4E000020	/*NAND Flash operation status*/
NFESTAT0	=   	0x4E000024
NFESTAT1	=   	0x4E000028
NFMECC0		=   	0x4E00002C
NFMECC1		=   	0x4E000030
NFSECC		=   	0x4E000034
NFSBLK		=   	0x4E000038	/*NAND Flash Start block address*/
NFEBLK		=   	0x4E00003C	/*NAND Flash End block address*/

/*        END */
