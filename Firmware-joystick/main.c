#include <stdio.h>
#include "types.h"
#include "Utils.h"
#include "tests.h"
#include "Timer.h"

#include "GPIORegs.h"

static void _PrintMenu ();


typedef struct _USER_MENU_ITEM
{
	char *Descr;
	void (*pfunc) ();
} USER_MENU_ITEM, *PUSER_MENU_ITEM;


USER_MENU_ITEM UserMenu[] =
{
	{"0 - Test servo motor\n", TestServoMotor},
	{"1 - Test joystick\n", TestJoystick},
	{NULL, NULL}
};

static void _PrintMenu ()
{
	PUSER_MENU_ITEM pMenuItem = UserMenu; 	
	while (pMenuItem->Descr != NULL)
	{
		Util_SendString (pMenuItem->Descr);
		pMenuItem ++;
	}
}


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
	
	int i = 0;
	while (1)
	{
		_PrintMenu ();		
		i = Util_GetIntNum ();
		if (UserMenu[i].pfunc)
		{
			UserMenu[i].pfunc ();
		}
	}
	
	
	// example of GPB1
	/* *GPBCON = 0x4;
	*GPBUP = 0x2;
	*GPBDAT = 0x0;
		
	while(1)
	{
		char ch = Util_Getch();
		*GPBDAT = 0x2;
		ch = Util_Getch();
		*GPBDAT = 0x0;
	} */
}

