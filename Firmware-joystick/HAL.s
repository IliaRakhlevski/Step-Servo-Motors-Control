NOINT      =  	0xc0

.text

.macro MOV_PC_LR
	mov pc,lr
.endm

.macro EQU
	=
.endm

HAL_CLEAR_PENDING_BIT: .global HAL_CLEAR_PENDING_BIT
HAL_CLEAR_PENDING_BIT:
	SRCPND = 0x4a000000
	INTRPND = 0x4a000010	
	ldr	r1,=SRCPND
	str	r0,[r1]
	ldr	r1,=INTRPND
	str	r0,[r1]
	mov pc,lr
	
HAL_CLEAR_SUBPENDING_BIT: .global HAL_CLEAR_SUBPENDING_BIT
HAL_CLEAR_SUBPENDING_BIT:
	SUBSRCPND = 0x4a000018
	ldr	r1,=SUBSRCPND
	str	r0,[r1]
	mov pc,lr

HAL_CLEAR_EPENDING_BIT: .global HAL_CLEAR_EPENDING_BIT
HAL_CLEAR_EPENDING_BIT:
	EINTPEND = 0x560000a8
	ldr	r1,=EINTPEND	
	str	r0,[r1]
	mov pc,lr

HAL_MASK_INTR: .global HAL_MASK_INTR
HAL_MASK_INTR:
	mrs	r1, cpsr
	str	r1, [r0]
	orr	r1, r1, #NOINT
	msr	cpsr_cxsf, r1
	mov pc,lr


HAL_RESTORE_INTR: .global HAL_RESTORE_INTR
HAL_RESTORE_INTR:
	ldr	r1, [r0]
	msr	cpsr_cxsf, r1
	mov pc,lr


/*
==============
 CPSR I,F bit
==============
int HAL_SET_IF(void);
*/

HAL_SET_IF:  .global HAL_SET_IF
HAL_SET_IF:
   /* This function works only if the processor is in previliged mode.*/
   mrs r0,cpsr
   mov r1,r0
   orr r1,r1,#NOINT
   msr cpsr_cxsf,r1
   mov pc,lr

/*void HAL_WR_IF(int cpsrValue);*/
HAL_WR_IF:  .global HAL_WR_IF
HAL_WR_IF:
   /* This function works only if the processor is in previliged mode.*/
   msr cpsr_cxsf,r0
   MOV_PC_LR
/*
====================================
 MMU Cache/TLB/etc on/off functions
====================================
*/
R1_I	=	(1<<12)
R1_C	=	(1<<2)
R1_A	=	(1<<1)
R1_M    =	(1)
R1_iA	=	(1<<31)
R1_nF   =	(1<<30)


/* void MMU_EnableICache(void) */
HAL_MMU_ENABLE_ICACHE:  .global HAL_MMU_ENABLE_ICACHE
HAL_MMU_ENABLE_ICACHE:
   mrc p15,0,r0,c1,c0,0
   orr r0,r0,#R1_I
   mcr p15,0,r0,c1,c0,0
   MOV_PC_LR
   
HAL_MMU_DISABLE_ICACHE:  .global HAL_MMU_DISABLE_ICACHE
HAL_MMU_DISABLE_ICACHE:
   mrc p15,0,r0,c1,c0,0
   bic r0,r0,#R1_I
   mcr p15,0,r0,c1,c0,0
   MOV_PC_LR

HAL_MMU_INVALIDATE_ICACHE:  .global HAL_MMU_INVALIDATE_ICACHE
HAL_MMU_INVALIDATE_ICACHE:
   mcr p15,0,r0,c7,c5,0
   MOV_PC_LR
   
   
   
HAL_MMU_DISABLE_DCACHE:  .global HAL_MMU_DISABLE_DCACHE
HAL_MMU_DISABLE_DCACHE:
   mrc p15,0,r0,c1,c0,0
   bic r0,r0,#R1_C
   mcr p15,0,r0,c1,c0,0
   MOV_PC_LR

HAL_MMU_CLEAN_INVALIDATE_DCACHE_INDEX:  .global HAL_MMU_CLEAN_INVALIDATE_DCACHE_INDEX
HAL_MMU_CLEAN_INVALIDATE_DCACHE_INDEX:
   ;r0=index
   mcr p15,0,r0,c7,c10,2
   MOV_PC_LR

HAL_MMU_DISABLE:  .global HAL_MMU_DISABLE
HAL_MMU_DISABLE:
   mrc p15,0,r0,c1,c0,0
   bic r0,r0,#R1_M
   mcr p15,0,r0,c1,c0,0
   MOV_PC_LR

HAL_MMU_INVALIDATE_TLB:  .global HAL_MMU_INVALIDATE_TLB
HAL_MMU_INVALIDATE_TLB:
   mcr p15,0,r0,c8,c7,0
   MOV_PC_LR

HAL_MMU_SetTTBase:		 .global HAL_MMU_SetTTBase
HAL_MMU_SetTTBase:
   ;ro=TTBase
   mcr p15,0,r0,c2,c0,0
   MOV_PC_LR

HAL_MMU_SetDomain:		 .global HAL_MMU_SetDomain
HAL_MMU_SetDomain:
   ;ro=domain
   mcr p15,0,r0,c3,c0,0
   MOV_PC_LR

HAL_MMU_SetProcessId:	 .global HAL_MMU_SetProcessId
HAL_MMU_SetProcessId:
   ;r0= pid
   mcr p15,0,r0,c13,c0,0
   MOV_PC_LR

HAL_MMU_EnableAlignFault: .global HAL_MMU_EnableAlignFault
HAL_MMU_EnableAlignFault:
   mrc p15,0,r0,c1,c0,0
   orr r0,r0,#R1_A
   mcr p15,0,r0,c1,c0,0
   MOV_PC_LR

HAL_MMU_EnableMMU:		  .global HAL_MMU_EnableMMU
HAL_MMU_EnableMMU:
   mrc p15,0,r0,c1,c0,0
   orr r0,r0,#R1_M
   mcr p15,0,r0,c1,c0,0
   MOV_PC_LR

HAL_MMU_EnableICache:	  .global HAL_MMU_EnableICache
HAL_MMU_EnableICache:
   mrc p15,0,r0,c1,c0,0
   orr r0,r0,#R1_I
   mcr p15,0,r0,c1,c0,0
   MOV_PC_LR

HAL_MMU_EnableDCache:	 .global HAL_MMU_EnableDCache
HAL_MMU_EnableDCache:
   mrc p15,0,r0,c1,c0,0
   orr r0,r0,#R1_C
   mcr p15,0,r0,c1,c0,0
   MOV_PC_LR

HAL_MMU_InvalidateDCache:	 .global HAL_MMU_InvalidateDCache
HAL_MMU_InvalidateDCache:
   mcr p15,0,r0,c7,c6,0
   MOV_PC_LR

HAL_MMU_InvalidateDCacheMVA: .global HAL_MMU_InvalidateDCacheMVA
HAL_MMU_InvalidateDCacheMVA:
   mcr p15,0,r0,c7,c6,1
   MOV_PC_LR

HAL_MMU_CleanDCacheMVA: .global HAL_MMU_CleanDCacheMVA
HAL_MMU_CleanDCacheMVA:
   mcr p15,0,r0,c7,c10,1
   MOV_PC_LR

HAL_MMU_InvalidateICacheMVA: .global HAL_MMU_InvalidateICacheMVA
HAL_MMU_InvalidateICacheMVA:
   mcr p15,0,r0,c7,c5,1
   MOV_PC_LR



.ltorg

.end
