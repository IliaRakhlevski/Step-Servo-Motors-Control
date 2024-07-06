#include "MMU_Table.h"

extern int _stext_init;

void MMU_Table_Init_MMU ()
{
    int i,j;
    //========================== IMPORTANT NOTE =========================
    //The current stack and code area can't be re-mapped in this routine.
    //If you want memory map mapped freely, your own sophiscated MMU
    //initialization code is needed.
    //===================================================================

    HAL_MMU_DISABLE_DCACHE ();
    HAL_MMU_DISABLE_ICACHE ();

    //If write-back is used,the DCache should be cleared.

    /* There are 64 ways with 8 cache lines each. Each cache line is 32 bytes */
    /* Total cache size is 16KBytes for D */
    for (i=0; i<64; i++)
    	for (j=0; j<8; j++)
    	    HAL_MMU_CLEAN_INVALIDATE_DCACHE_INDEX ((i<<26) | (j<<5));
    HAL_MMU_INVALIDATE_ICACHE ();
    
    HAL_MMU_DISABLE ();
    HAL_MMU_INVALIDATE_TLB ();
    
    
    MMU_Table_SetMTT (0x00000000,0x00100000,(int)&_stext_init,RW_NCNB);  //For interrupt vector mapping    
//    MMU_Table_SetMTT (0x04000000,0x07f00000,0,RW_NCNB);  	  //bank0
    MMU_Table_SetMTT (0x04000000,0x07f00000,0,RW_FAULT);  	  //bank0 - currently not accessed             
    MMU_Table_SetMTT (0x08000000,0x0ff00000,0x08000000,RW_NCNB);  //bank1
    MMU_Table_SetMTT (0x10000000,0x17f00000,0x10000000,RW_NCNB); //bank2
    MMU_Table_SetMTT (0x18000000,0x1ff00000,0x18000000,RW_NCNB); //bank3
    MMU_Table_SetMTT (0x20000000,0x27f00000,0x20000000,RW_NCNB);  //bank4 for STRATA Flash
    MMU_Table_SetMTT (0x28000000,0x2ff00000,0x28000000,RW_NCNB); //bank5
    MMU_Table_SetMTT (0x30000000,0x30100000,0x30000000,RW_CB);	 //bank6-1
    MMU_Table_SetMTT (0x30200000,0x33e00000,0x30200000,RW_CB);   //bank6-2
    MMU_Table_SetMTT (0x33f00000,0x33f00000,0x33f00000,RW_CB);   //bank6-3
    MMU_Table_SetMTT (0x34000000,0x37f00000,0x34000000,RW_CB);   //bank6-4 - Added by Yoav
    MMU_Table_SetMTT (0x38000000,0x3ff00000,0x38000000,RW_CB);   //bank7
    
    MMU_Table_SetMTT (0x40000000,0x47f00000,0x40000000,RW_NCNB); //SFR
    MMU_Table_SetMTT (0x48000000,0x5af00000,0x48000000,RW_NCNB); //SFR
    MMU_Table_SetMTT (0x5b000000,0x5b000000,0x5b000000,RW_NCNB); //SFR
    MMU_Table_SetMTT (0x5b100000,0x5b200000,0x20000000,RW_CB);   //bank4 for STRATA Flash - Cached buffer access    
    MMU_Table_SetMTT (0x5b300000,0x5ba00000,0x34000000,RW_NCNB); //Non cached, Non buffered alias to 7 megas of bank6-4, Added by Yoav
          
    MMU_Table_SetMTT (0x5bb00000,0xfff00000,0x5bb00000,RW_FAULT);//not used - Modified by Yoav
    MMU_Table_SetMTT (0x70000000,0x70400000,0x34a00000,RW_CB);// Modified by Yoav - usage example
    MMU_Table_SetMTT (0x70500000,0x70500000,0x20000000,RW_NCNB);// Used for NC access to bank4 and DM9000          
    
    HAL_MMU_SetTTBase (_MMUTT_STARTADDRESS);
    HAL_MMU_SetDomain (0x55555550|DOMAIN1_ATTR|DOMAIN0_ATTR);
    
    HAL_MMU_SetProcessId (0x0);
    HAL_MMU_EnableAlignFault ();

    HAL_MMU_EnableMMU ();
    HAL_MMU_EnableICache ();
    HAL_MMU_EnableDCache (); //DCache should be turned on after MMU is turned on.
}

void MMU_Table_SetMTT (int vaddrStart, int vaddrEnd, int paddrStart, int attr)
{
    volatile unsigned long *pTT;
    volatile int i,nSec;
    
    pTT = (unsigned long *)_MMUTT_STARTADDRESS + (vaddrStart>>20);
    nSec = (vaddrEnd>>20) - (vaddrStart>>20);
    for(i = 0; i <= nSec; i++)
    {
        *pTT++ = attr | (((paddrStart>>20) + i) << 20);
    }
}

