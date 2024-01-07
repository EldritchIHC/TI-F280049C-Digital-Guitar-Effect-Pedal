/*
 * ADC_B.h
 *
 *  Created on: 21 oct. 2023
 *      Author: Paul
 */

#ifndef ADC_B_H_
#define ADC_B_H_

/*Defines*/
/*ADC Triggers*/
#define SOFTWARE 0
#define TIMER0 1
#define TIMER1 2
#define TIMER2 3
#define GPIO_EXT 4
#define EPWM1SOCA 5
#define EPWM1SOCB 6
#define EPWM2SOCA 7
#define EPWM2SOCB 8
#define EPWM3SOCA 9
#define EPWM3SOCB 10
#define EPWM4SOCA 11
#define EPWM4SOCB 12
#define EPWM5SOCA 13
#define EPWM5SOCB 14

/*Select End of Conversion*/
#define EOC0 0
#define EOC1 1
#define EOC2 2
#define EOC3 3
/*Structures*/
typedef struct{
    Uint16 u16ADCChannel;//Input Pin
    Uint16 u16AqWindow;//
    Uint16 u16Interrupt;//Interrupt number 1 - 4
    Uint16 u16TriggerSRC;//Trigger Source(EPWM/CPU)
}tstADCBConfig;

/*Structure Initializations*/
#define ADCB1_INIT {1, 47, 1, TIMER0}
#define ADCB2_INIT {4, 14, 1, EPWM4SOCA}
#define ADCB3_INIT {3, 14, 1, TIMER0}
#define ADCB4_INIT {4, 14, 1, EPWM4SOCA}

void ADC_B_vADCB_Group_Init();
void ADC_B_vADCB_SOC0Config(tstADCBConfig ADC);
void ADC_B_vADCB_SOC1Config(tstADCBConfig ADC);
void ADC_B_vADCB_SOC0Disable();
void ADC_B_vADCB_SOC1Disable();
Uint16 ADC_B_u16ADCBgetRawValue0();
Uint16 ADC_B_u16ADCBgetRawValue1();
/*Variables*/
extern tstADCBConfig stADCB1;
extern tstADCBConfig stADCB2;
extern tstADCBConfig stADCB3;
extern tstADCBConfig stADCB4;



#endif /* ADC_B_H_ */
