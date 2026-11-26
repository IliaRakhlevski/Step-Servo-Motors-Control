#include "types.h"
#include "Utils.h"
#include "ADCRegs.h"
#include "Timer.h"


#define ADC_FREQ 2500000
#define PCLK 50000000

#define	ESC_KEY		0x1b

#define GRAD_0   	2
#define GRAD_180   13
#define STOP	   	0


U32 CurrentDirection = STOP;


void JoystickToServo(U32 joystick, U32 *direction, U32 *speed_timer_delay )
{
	if((joystick > 480 && joystick < 543) || joystick > 1023)
	{
		*speed_timer_delay = 0;
		*direction = STOP;
	}
	else if(joystick >= 0 && joystick <= 480)
	{
		*speed_timer_delay = joystick;
		*direction = GRAD_0;
	}
	else
	{
		*speed_timer_delay = 480 - (joystick - 543);
		*direction = GRAD_180;
	}
	
	*speed_timer_delay /= (U32)100;
	*speed_timer_delay *= (U32)100;
	
	return;
}

//==================================================================================		
int ReadAdc0()
{
	/*
		Now - issue the start command.
	*/
    *ADCCON|=0x1;   //start ADC


detect_conversion:

    while(*ADCCON & 0x1);	//Wait until Enable_start bit has returned to low
    
    while(!(*ADCCON & 0x8000));	//Wait until EC(End of Conversion) flag is high

    return ( (U32)*ADCDAT0 & 0x3ff ); // Read the data
}

//==================================================================================
void TestJoystick(void) 
{
    volatile U32 a0;
	volatile U32 preScaler;     
    U32 rADCCON_save = *ADCCON;
    
   //	Util_Printf( "ADC INPUT Test, press ESC key to exit !\n" ) ;

    /*
     * Set prescaler value to reflect smapling frequency of 500000 SPS
    */ 
    preScaler = (PCLK/ADC_FREQ);               
    
   // Util_Printf("Sampling rate is set to %d SPS\n",(PCLK/preScaler)/5);
    
    /*
    * Setup channel 0
    */
	*ADCCON = (1<<14)|(preScaler<<6);
	
	*ADCCON |= (0x2<<3);
	    
    while( Util_Getkey() != ESC_KEY )
    {
		U32 direction;
		U32 speed_timer_delay;
		
	    a0=ReadAdc0();
	    JoystickToServo(a0, &direction, &speed_timer_delay);
	    
	    SetPWMValues(direction, speed_timer_delay);
	    
	    if(CurrentDirection == STOP && direction != STOP) // start moving
	    {	
			StartServoMotor();
		}
		else if(CurrentDirection != STOP && direction == STOP) // stop moving
		{
			StopServoMotor();
		}
	    
	    CurrentDirection = direction;

	    Util_Printf( "AIN1: %04d, direction - %02d, delay - %03d\n", a0, direction, speed_timer_delay );
    }
   
    *ADCCON = rADCCON_save;
   // Util_Printf("\nrADCCON = 0x%x\n", *ADCCON);
}
