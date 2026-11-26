#include "PWMRegs.h"
#include "types.h"
#include "Timer.h"
#include "Utils.h"
#include "GPIORegs.h"
#include "INTRRegs.h"
#include "INTRDefs.h"


  
static U32 PCLK = 50000000;  
U32 CyclesPerSeconds = 50;
U32 DutyCycle = 0;	
U32 CounterTreshold = 0;



static void TimerServoMotorISR (void) __attribute__ ((interrupt ("IRQ")));

void SetPWMValues(U32 direction, U32 speed_timer_delay)
{
	DutyCycle = direction;	
	CounterTreshold = speed_timer_delay;
}

void StartServoMotor()
{
	U32 StepsInTooth = 50000 / CyclesPerSeconds;
	
	/* Disable any sperious interrupts from timer 1*/
	*INTMSK	 |= (BIT_TIMER1);
	
	/* Place interrupt routine in interrupt vestor */
	pISR_TIMER1=(U32)TimerServoMotorISR;
	
	// Set TOUT1
	*GPBCON &= (~(3 << 2));			//set GPB1 as tout1, pwm output
	*GPBCON |= 2 << 2;
	
	*TCFG1	&= ~(0xf << 4);
	*TCFG0 &= ~0xff;	
		
	*TCFG1 |= 1 << 4;			
	*TCFG0 = 249;	
	
	*TCNTB1 = (short)StepsInTooth; /* Must not exceed 16 bit */
	*TCMPB1 = (StepsInTooth * DutyCycle)/100;
		
	*TCON |= 0xb << 8;			//disable deadzone, auto-reload, inv-off, update TCNTB0&TCMPB0, start timer 1
	*TCON &= ~(2 << 8);			//clear manual update bit
	
	*INTMSK	 &= ~(BIT_TIMER1);
}


void StopServoMotor()
{
	// Now stop it.
 	*TCON &= ~(1<<11); //disable auto reload for timer 1
 	*TCON &= ~(1<<8); //Timer 1 is stoped

	*GPBCON &= ~(3 << 2);			//set GPB1 as output
	*GPBCON |= 1 << 2;
	//*GPBDAT &= ~2;	
}

static void TimerServoMotorISR(void)
{
	*SRCPND = BIT_TIMER1;
	*INTPND = BIT_TIMER1;
	
	static unsigned char counter = 0;
	if(counter == (unsigned char)2)
	{
		StopServoMotor();
		counter = 0;
		Timer_Bdelay_Milli (CounterTreshold);
		StartServoMotor();
	}
	 counter++;
}


void TestServoMotor ()
{	
	Util_Printf ("Enter the duty cycle percentage (decimal 0-100) ");	
	DutyCycle = Util_GetIntNum ();
	Util_Printf ("Enter counter treshold: ");	
	CounterTreshold = Util_GetIntNum ();
	
	TestButtons ();
}



void Timer_Bdelay_Milli (unsigned long Millis)
{
	U32 Prev;
	U32 Current;
	U32 ElapsedSteps = 0;
	U32 StepsInOneMilli = (PCLK>>3)/1000;
	U32 TotalSteps = StepsInOneMilli * Millis;	

	/*
	 * Set timer step period T for Timer 4 - Divide PCLK by 8
	*/	
	*TCFG0 &= ~(0xff<<8);
	*TCFG0 |= 3<<8;			        //prescaler = 3+1
	*TCFG1 &= ~(0xf<<16);
	*TCFG1 |= TIMER4_MUX_1_2;		//mux = 1/2

	/*
	 * Tooth period cT is set by multiplying T by the number of countown steps.
	*/	
	*TCNTB4 = StepsInOneMilli;
	
	/*
	 * Activate timer 4.
	*/	
	
	*TCON |= (TIMER4_START | TIMER4_MAN_UPDATE | TIMER4_AUTO_RELOAD);	//autoload, update TCNTB4, start timer 4
	*TCON &= ~(TIMER4_MAN_UPDATE);		//clear manual update bit
	
	Prev = *TCNTO4;

	while (ElapsedSteps < TotalSteps)
	{
		Current = *TCNTO4;
		if (Current <= Prev)
		{
			ElapsedSteps += (Prev - Current);
		}
		else
		{
			ElapsedSteps += (StepsInOneMilli - (Current - Prev));
		}
		
		Prev = Current;
	}

	/* Stop the timer */	
	*TCON &= ~(TIMER4_START | TIMER4_AUTO_RELOAD);	
}


void Timer_Bdelay_Sec (unsigned long Sec)
{
	U32 Prev;
	U32 Current;
	U32 ElapsedSteps = 0;
	U32 StepsInOneSec = 50000;
	U32 TotalSteps = StepsInOneSec * Sec;

			
	*TCFG0 &= ~(0xff<<8);
	*TCFG0 |= 249<<8;			        //prescaler = 249+1
	*TCFG1 &= ~(0xf<<16);
	*TCFG1 |= TIMER4_MUX_1_4;		//mux = 1/4

	/*
	 * Tooth period cT is set by multiplying T by the number of countown steps.
	*/	
	*TCNTB4 = StepsInOneSec;
	

	/*
	 * Activate timer 4.
	*/	
	
	*TCON |= (TIMER4_START | TIMER4_MAN_UPDATE | TIMER4_AUTO_RELOAD);	//autoload, update TCNTB4, start timer 4
	*TCON &= ~(TIMER4_MAN_UPDATE);		//clear manual update bit
	
	Prev = *TCNTO4;

	Util_Printf ("Start delay ...\n");

	while (ElapsedSteps < TotalSteps)
	{
		Current = *TCNTO4;
		if (Current <= Prev)
		{
			ElapsedSteps += (Prev - Current);
		}
		else
		{
			ElapsedSteps += (StepsInOneSec - (Current - Prev));
		}
		
		Prev = Current;
	}

	/* Stop the timer */	
	*TCON &= ~(TIMER4_START | TIMER4_AUTO_RELOAD);	

	Util_Printf ("End delay ...\n");
}

