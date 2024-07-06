#ifndef INTRREGS_H_
#define INTRREGS_H_

#define SRCPND     ((volatile unsigned *)0x4a000000)	//Interrupt request status
#define INTMOD     ((volatile unsigned *)0x4a000004)	//Interrupt mode control
#define INTMSK     ((volatile unsigned *)0x4a000008)	//Interrupt mask control
#define PRIORITY   ((volatile unsigned *)0x4a00000c)	//IRQ priority control
#define INTPND     ((volatile unsigned *)0x4a000010)	//Interrupt request status
#define INTOFFSET  ((volatile unsigned *)0x4a000014)	//Interruot request source offset
#define SUBSRCPND  ((volatile unsigned *)0x4a000018)	//Sub source pending
#define INTSUBMSK  ((volatile unsigned *)0x4a00001c)	//Interrupt sub mask


#define BIT_TIMER0		(0x1<<10)
#define BIT_TIMER1		(0x1<<11)
#define BIT_TIMER4		(0x1<<14)
#define BIT_UART1		(0x1<<23)
#define BIT_UART2		(0x1<<15)
#define BIT_SUB_ERR2	(0x1<<8)
#define BIT_SUB_TXD2	(0x1<<7)
#define BIT_SUB_RXD2	(0x1<<6)
#define BIT_SUB_ERR1	(0x1<<5)
#define BIT_SUB_TXD1	(0x1<<4)
#define BIT_SUB_RXD1	(0x1<<3)
#define BIT_SUB_ERR0	(0x1<<2)
#define BIT_SUB_TXD0	(0x1<<1)
#define BIT_SUB_RXD0	(0x1<<0)

#define BIT_DMA0		(0x1<<17)
#define BIT_DMA1		(0x1<<18)
#define BIT_DMA2		(0x1<<19)
#define BIT_DMA3		(0x1<<20)



#endif /*INTRREGS_H_*/
