@************************************************
@ NAME    : MEMCFG.A
@ DESC	  : Memory bank configuration file
@ Revision: 02.28.2002 ver 0.0
@ Revision: 03.11.2003 ver 0.0	Attatched for 2440
@************************************************

@Memory Area
@GCS6 32bit(64MB) SDRAM(0x3000_0000-0x33ff_ffff)


@BWSCON
DW8			=	(0x0)
DW16		=	(0x1)
DW32		=	(0x2)
WAIT		=	(0x1<<2)
UBLB		=	(0x1<<3)

B1_BWSCON	= (DW16)	@ AMD flash(AM29LV800B), 16-bit,  for nCS1
B2_BWSCON	= (DW16)	@ PCMCIA(PD6710), 16-bit
B3_BWSCON	= (DW16)	@ Ethernet(CS8900), 16-bit
B4_BWSCON	= (DW32)	@ Intel Strata(28F128), 32-bit, for nCS4
B5_BWSCON	= (DW16)	@ A400/A410 Ext, 16-bit
B6_BWSCON	= (DW32)	@ SDRAM(K4S561632C) 32MBx2, 32-bit
B7_BWSCON	= (DW32)	@ N.C.

@BANK0CON

B0_Tacs		=	0x3	@0clk
B0_Tcos		=	0x3	@0clk
B0_Tacc		=	0x7	@14clk
B0_Tcoh		=	0x3	@0clk
B0_Tah		=	0x3	@0clk
B0_Tacp		=	0x1
B0_PMC		=	0x0	@normal

@BANK1CON
B1_Tacs		=	1@0x0	@0clk
B1_Tcos		=	1@0x0	@0clk
B1_Tacc		=	6@0x7	@14clk
B1_Tcoh		=	1@0x0	@0clk
B1_Tah		=	1@0x0	@0clk
B1_Tacp		=	0x0
B1_PMC		=	0x0	@normal

@Bank 2 parameter
B2_Tacs		=	1@0x0	@0clk
B2_Tcos		=	1@0x0	@0clk
B2_Tacc		=	6@0x7	@14clk
B2_Tcoh		=	1@0x0	@0clk
B2_Tah		=	1@0x0	@0clk
B2_Tacp		=	0x0
B2_PMC		=	0x0	@normal

@Bank 3 parameter
B3_Tacs		=	0x1@0	@0clk
B3_Tcos		=	0x1@0	@0clk
B3_Tacc		=	0x6@7	@14clk
B3_Tcoh		=	0x1@0	@0clk
B3_Tah		=	0x1@0	@0clk
B3_Tacp		=	0x0
B3_PMC		=	0x0	@normal

@Bank 4 parameter
B4_Tacs		=	0x1@0	@0clk
B4_Tcos		=	0x1@0	@0clk
B4_Tacc		=	0x6@7	@14clk
B4_Tcoh		=	0x1@0	@0clk
B4_Tah		=	0x1@0	@0clk
B4_Tacp		=	0x0
B4_PMC		=	0x0	@normal

@Bank 5 parameter
B5_Tacs		=	0x1@0	@0clk
B5_Tcos		=	0x1@0	@0clk
B5_Tacc		=	0x6@7	@14clk
B5_Tcoh		=	0x1@0	@0clk
B5_Tah		=	0x1@0	@0clk
B5_Tacp		=	0x0
B5_PMC		=	0x0	@normal

@ When 100MHz HCLK is used.

@Bank 6 parameter
B6_MT		=	0x3	@SDRAM
B6_Trcd		=	0x1	@3clk
B6_SCAN		=	0x1	@9bit

@Bank 7 parameter
B7_MT		=	0x3	@SDRAM
B7_Trcd		=	0x1	@3clk
B7_SCAN		=	0x1	@9bit

@REFRESH parameter
REFEN		=	0x1	@Refresh enable
TREFMD		=	0x0	@CBR(CAS before RAS)/Auto refresh
Trp			=	0x1	@3clk
Tsrc		=	0x1	@5clk	Trc= Trp(3)+Tsrc(5) = 8clock
Tchr		=	0x2	@3clk
REFCNT		=	1268@1463@1268	@HCLK=105Mhz, (2048+1-7.81*100)@75M->1463

