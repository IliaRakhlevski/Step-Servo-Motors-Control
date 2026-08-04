#ifndef ADCREGS_H_
#define ADCREGS_H_

#define ADCCON    ((volatile unsigned *)0x58000000)	//ADC control
#define ADCTSC    ((volatile unsigned *)0x58000004)	//ADC touch screen control
#define ADCDLY    ((volatile unsigned *)0x58000008)	//ADC start or Interval Delay
#define ADCDAT0   ((volatile unsigned *)0x5800000c)	//ADC conversion data 0
#define ADCDAT1   ((volatile unsigned *)0x58000010)	//ADC conversion data 1
#define ADCUPDN   ((volatile unsigned *)0x58000014)	//Stylus Up/Down interrupt status


#endif /*ADCREGS_H_*/
