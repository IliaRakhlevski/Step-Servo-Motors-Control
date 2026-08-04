#include "GPIORegs.h"
#include "types.h"
#include "Utils.h"
#include "tests.h"
#include "Timer.h"


#define GPG0	0
#define GPG3	3
#define GPG5	5
#define GPG6	6
#define GPG7	7
#define GPG11	11


bool IsServoMotor = false;  // true - servo motor selected, false - step motor selected
bool IsStepMove = false;	// if step motor rotates

void Button_State_Changed(int button, int new_state);


// check if some board buttons pressed/released and perform 
// relevant action
void TestButtons ()
{	
	U32 ConRegVal;
	U32 ValToData;	
	U32 CurrentData;
	U32 NewData;
	bool out_state=0;
	
	/* 0xC0FCC3 */
	U32 ConMask = (3 << (2 * GPG0)) | 
				  (3 << (2 * GPG3)) | 
				  (3 << (2 * GPG5)) | 
				  (3 << (2 * GPG6)) | 
				  (3 << (2 * GPG7)) |
				  (3 << (2 * GPG11));
	
	/* 0x8E9 */	
	U32 UpDataMask =  (1 << GPG0) | 
					(1 << GPG3) | 
					(1 << GPG5) | 
					(1 << GPG6) | 
					(1 << GPG7) | 
					(1 << GPG11);
				  
	//Util_Printf(" ConMask = 0x%x\n", ConMask);
	//Util_Printf(" UpDataMask = 0x%x\n", UpDataMask);
				  	
	if ((ConRegVal = ((U32)(*GPGCON) & ConMask )) != 0x0)
	{
		*GPGCON &= ~ConMask; 
		*GPGCON |= 0x0;
		*GPGUP  &= ~UpDataMask; 
		*GPGUP  |= UpDataMask;				
	} 	
	
	CurrentData = UpDataMask;
		
	while(1)
	{
		/* Buttons */
		ValToData = (U32)(*GPGDAT);
		NewData = (U32)(ValToData & UpDataMask);

		if(	CurrentData != NewData) /* no button state changed */
		{	
			if(	(NewData & (1 << GPG0)) != (CurrentData & (1 << GPG0)))			
				Button_State_Changed(GPG0, (CurrentData & (1 << GPG0)));
	
			if(	(NewData & (1 << GPG3)) != (CurrentData & (1 << GPG3)))	
			{		
				Button_State_Changed(GPG3, (CurrentData & (1 << GPG3)));
				break; // exit to the main menu
			}
			
			if(	(NewData & (1 << GPG5)) != (CurrentData & (1 << GPG5)))			
				Button_State_Changed(GPG5, (CurrentData & (1 << GPG5)));
			
			if(	(NewData & (1 << GPG6)) != (CurrentData & (1 << GPG6)))			
				Button_State_Changed(GPG6, (CurrentData & (1 << GPG6)));
			
			if(	(NewData & (1 << GPG7)) != (CurrentData & (1 << GPG7)))			
				Button_State_Changed(GPG7, (CurrentData & (1 << GPG7)));
			
			if(	(NewData & (1 << GPG11)) != (CurrentData & (1 << GPG11)))			
				Button_State_Changed(GPG11, (CurrentData & (1 << GPG11)));
			
			CurrentData = NewData;	
		}	
		
		if(IsStepMove) // if step motor must rotate
		{
			StepMotorMove(); // rotate step motor to next phase
		}
		
	} // while
}

// perform relevant action according to button new state (pressed/released)
void Button_State_Changed(int button, int new_state)
{
	switch(button)
	{
		case GPG0:
		{
			if(new_state)
			{
				Util_Printf("--Button 1, pressed \n");
				if(IsServoMotor)
				{
					SetServoDirection(GRAD_0);
					StartServoMotor();			
				}
				else
				{
					SetStepsDirection(ACW_DIRECTION);
					IsStepMove = true;
					StartStepMotor();
				}
			}
			else
			{
				Util_Printf("--Button 1, released \n");
				if(IsServoMotor)
					StopServoMotor();
				else
				{
					IsStepMove = false;
					StopStepMotor();
				}
			}
			break;
		}
		case GPG3:
		{
			if(new_state)
			{
				Util_Printf("--Button 2, pressed \n");
				Util_Printf("   Exit to main menu\n");
			}
			else
				Util_Printf("--Button 2, released \n");
			break;
		}
		case GPG5:
		{
			if(new_state)
			{
				Util_Printf("--Button 3, pressed \n");
				if(IsServoMotor)
					DecServoSpeedDelay();
				else
					DecStepSpeedDelay();
			}
			else
			{
				Util_Printf("--Button 3, released \n");
			}
			break;
		}
		case GPG6:
		{
			if(new_state)
			{
				Util_Printf("--Button 4, pressed \n");
				if(IsServoMotor)
				{
					IsServoMotor = false;
					Util_Printf("   Step motor selected \n");
				}
				else
				{
					IsServoMotor = true;
					Util_Printf("   Servo motor selected \n");
				}
			}
			else
			{
				Util_Printf("--Button 4, released \n");
			}
			break;
		}
		case GPG7:
		{
			if(new_state)
			{
				Util_Printf("--Button 5, pressed \n");
				if(IsServoMotor)
				{
					SetServoDirection(GRAD_180);
					StartServoMotor();
				}
				else
				{
					SetStepsDirection(CW_DIRECTION);
					IsStepMove = true;
					StartStepMotor();
				}
			}
			else
			{
				Util_Printf("--Button 5, released \n");	
				if(IsServoMotor)		
					StopServoMotor();
				else
				{
					IsStepMove = false;
					StopStepMotor();
				}
			}
			break;
		}
		case GPG11:
		{
			if(new_state)
			{
				Util_Printf("--Button 6, pressed \n");	
				if(IsServoMotor)			
					IncServoSpeedDelay();
				else
					IncStepSpeedDelay();
			}
			else
			{
				Util_Printf("--Button 6, released \n");
			}
			break;
		}
	};
}

