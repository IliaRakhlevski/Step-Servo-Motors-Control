#include <stdio.h>
#include "types.h"
#include "Utils.h"
#include "tests.h"
#include "Timer.h"
#include "GPIORegs.h"


void halt_on_und();
void halt_on_pabrt();

void halt_on_und()
{
	Util_Printf ("Halt on und\n");
	while (1);	
}

void halt_on_pabrt()
{
	Util_Printf ("Halt on pabrt\n");
	while (1);	
}

void halt_on_dabrt()
{
	Util_Printf ("Halt on dabrt\n");
	while (1);	
}


int main ()
{
	Util_Printf ("\n\n *** Start Application ***\n\n");

	MMU_Table_Init_MMU ();
	
	TestMotors();
	
	Util_Printf ("\n\n *** Stop Application ***\n\n");
}

