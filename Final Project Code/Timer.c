#include <stdio.h>
#include "PWMRegs.h"
#include "types.h"
#include "Timer.h"
#include "Utils.h"
#include "GPIORegs.h"
#include "INTRRegs.h"
#include "INTRDefs.h"


extern bool IsServoMotor; // defined in testButtons.c
#define	ESC_KEY		0x1b

  
static U32 PCLK = 50000000;  
U32 CyclesPerSeconds = 50;
U32 DutyCycle = GRAD_0;		// rotation direction in servo motor
U32 CounterTreshold = 100; 	// delay between moving steps in servo motor


// ISR for servo motor PWM
static void TimerServoMotorISR (void) __attribute__ ((interrupt ("IRQ")));

// step motor phases functions
void ClearGPDat();
void Phase_1();
void Phase_2();
void Phase_3();
void Phase_4();
void Phase_5();
void Phase_6();
void Phase_7();
void Phase_8();

typedef void (*phase_func) (); // define pointer to phase function

#define STEP_DELAY 	6	// default delay between phases

U32 StepMotorCurrentDirection  = ACW_DIRECTION; // step motor direction: anticlockwise, clockwise
U32 StepMotorSpeed = STEP_DELAY; 				// set default delay between phases


// step motor rotation phases, array of pointers to relevant functions
phase_func Phases[] =
{
	Phase_1, Phase_2, Phase_3, Phase_4, Phase_5, Phase_6, Phase_7, Phase_8, (phase_func)NULL
};


/***** Servo motor ********/

// decrement delay - encrease servo motor rotation speed
void DecServoSpeedDelay()
{
	CounterTreshold -= 10;
	if(CounterTreshold < 10)
		CounterTreshold = 10;
	
	Util_Printf("   Servo speed delay - %d\n", CounterTreshold);
}

// increment delay - decrease servo motor rotation speed
void IncServoSpeedDelay()
{
	CounterTreshold += 10; 
	if(CounterTreshold > 100)
		CounterTreshold = 100;
		
	Util_Printf("   Servo speed delay - %d\n", CounterTreshold);
}

// set servo motor rotation direction
void SetServoDirection(U32 direction)
{
	DutyCycle = direction;	
}

// initializing of servo motor, use PWM and start its rotation
void StartServoMotor()
{
	Util_Printf("   Start servo motor\n");
	U32 StepsInTooth = 50000 / CyclesPerSeconds;
	
	/* Disable any sperious interrupts from timer 1*/
	*INTMSK	 |= (BIT_TIMER1);
	
	/* Place interrupt routine in interrupt vector */
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

// stop servo motor rotation
void StopServoMotor()
{
 	*TCON &= ~(1<<11); 				//disable auto reload for timer 1
 	*TCON &= ~(1<<8); 				//Timer 1 is stoped

	*GPBCON &= ~(3 << 2);			//set GPB1 as output
	*GPBCON |= 1 << 2;	
	Util_Printf("   Stop servo motor\n");
}

// ISR for servo motor PWM
static void TimerServoMotorISR(void)
{
	*SRCPND = BIT_TIMER1;
	*INTPND = BIT_TIMER1;
	
	static unsigned char counter = 0;
	if(counter == (unsigned char)2)
	{
		// stop servo motor
		StopServoMotor();
		counter = 0;
		// delay between steps
		Timer_Bdelay_Milli (CounterTreshold);
		// start new step
		StartServoMotor();
	}
	 counter++;
}


/***** Step motor ********/

// create delay in milliseconds
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

// clear GPF data 
void ClearGPDat()
{
	*GPFDAT = 0x0;
}


// step motor rotation phases functions

void Phase_1() // orange
{
	ClearGPDat();
	*GPFDAT = 0x1 << 1;
}

void Phase_2() // orange, yellow
{
	ClearGPDat();
	*GPFDAT = 0x1 << 1;
	*GPFDAT |= 0x1 << 3;
}

void Phase_3() // yellow
{
	ClearGPDat();
	*GPFDAT = 0x1 << 3;
}

void Phase_4() // yellow, pink
{
	ClearGPDat();
	*GPFDAT = 0x1 << 3;
	*GPFDAT = 0x1 << 5;
}

void Phase_5() //  pink
{
	ClearGPDat();
	*GPFDAT = 0x1 << 5;
}

void Phase_6() //  pink, blue
{
	ClearGPDat();
	*GPFDAT = 0x1 << 5;
	*GPFDAT = 0x1 << 6;
}

void Phase_7() //  blue
{
	ClearGPDat();
	*GPFDAT = 0x1 << 6;
}

void Phase_8() //  orange, blue
{
	ClearGPDat();
	*GPFDAT = 0x1 << 1;
	*GPFDAT = 0x1 << 6;
}

// execute next step of step motor rotation
void StepMotorMove()
{
	static int ind = 7;
	
	// perform next step
	Phases[ind]();
	Timer_Bdelay_Milli(StepMotorSpeed);
	
	if(StepMotorCurrentDirection == CW_DIRECTION)
	{
		ind--;		
		if(ind < 0)
			ind = 7;
	}
	else
	{
		ind++;		
		if(ind >= 8)
			ind = 0;
	}
}

// initialize and start step motor rotation
void StartStepMotor()
{	
	Util_Printf("   Start step motor\n");
	*GPFCON = 0x1 << 2;
	*GPFCON |= 0x1 << 6;
	*GPFCON |= 0x1 << 10;
	*GPFCON |= 0x1 << 12;
	
	*GPFUP = 0x1 << 1;
	*GPFUP |= 0x1 << 3;
	*GPFUP |= 0x1 << 5;
	*GPFUP |= 0x1 << 6;
	
	*GPFDAT = 0x0;
}

// stop step motor
void StopStepMotor()
{	
	*GPFCON = 0x0;
	*GPFUP  = 0x0;
	*GPGDAT = 0x0;
	Util_Printf("   Stop step motor\n");
}

// deccrement step motor rotation delay - decrease speed
void DecStepSpeedDelay()
{
	StepMotorSpeed -= 2;
	if(StepMotorSpeed < 2)
		StepMotorSpeed = 2;
	
	Util_Printf("   Step speed delay - %d\n", StepMotorSpeed);
}

// increment step motor rotation delay - increase speed
void IncStepSpeedDelay()
{
	StepMotorSpeed += 2;
	if(StepMotorSpeed > 8)
		StepMotorSpeed = 8;
	
	Util_Printf("   Step speed delay - %d\n", StepMotorSpeed);
}

// set step motor direction
void SetStepsDirection(U32 direction)
{
	StepMotorCurrentDirection = direction;
}

// start motors routine
void TestMotors()
{
	// start with step motor
	Util_Printf("Start motors rotation \n");
	Util_Printf("   Step motor selected \n");
	IsServoMotor = false;
	TestButtons ();
}

