/*
 * TIMER.h
 *
 *  Created on: 14 oct. 2023
 *      Author: Paul
 */

#ifndef TIMER_H_
#define TIMER_H_

/*Defines*/
#define CPUTIMER0 0
#define CPUTIMER1 1
#define CPUTIMER2 2


/*Structures*/
typedef struct{
    Uint32 u32Period_ms;
    Uint16 u16CPUTimer;
    Uint16 u16CPUTimerInterruptEnable;
}tstCPUTimerSecondsConfig;

typedef struct{
    Uint32 u32Frequency_Hz;
    Uint16 u16CPUTimer;
    Uint16 u16CPUTimerInterruptEnable;
}tstCPUTimerHertzConfig;

/*Structure Initializations*/
//CPUTIMER0
#define TIMER1_SEC_INIT {  300, CPUTIMER1, ENABLE }
#define TIMER1_HEZ_INIT {  96000, CPUTIMER0, ENABLE }
//CPUTIMER1
#define TIMER2_SEC_INIT {  3000, CPUTIMER1, ENABLE }
#define TIMER2_HEZ_INIT {    1, CPUTIMER1, ENABLE }
//CPUTIMER2
#define TIMER3_SEC_INIT { 100000, CPUTIMER2, DISABLE}
#define TIMER3_HEZ_INIT { 100000, CPUTIMER2, DISABLE}

/*Function prototypes*/
void TIMER_vCPUTimerSecondsInit(tstCPUTimerSecondsConfig CPU);
void TIMER_vCPUTimerHertzInit(tstCPUTimerHertzConfig CPU);
/*Variables*/
extern tstCPUTimerSecondsConfig stCPU1;
extern tstCPUTimerSecondsConfig stCPU2;
extern tstCPUTimerSecondsConfig stCPU3;
extern tstCPUTimerHertzConfig stCPU4;
extern tstCPUTimerHertzConfig stCPU5;
extern tstCPUTimerHertzConfig stCPU6;

#endif /* TIMER_H_ */
