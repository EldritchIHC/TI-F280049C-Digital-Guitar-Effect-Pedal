/*
 * ADC_A.h
 *
 *  Created on: 21 oct. 2023
 *      Author: Paul
 */

#ifndef ADC_A_H_
#define ADC_A_H_

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
}tstADCAConfig;

/*Structure Initializations*/
#define ADCA1_INIT {0, 47, 1, TIMER0}
#define ADCA2_INIT {4, 14, 1, EPWM4SOCA}
#define ADCA3_INIT {3, 14, 1, EPWM4SOCA}
#define ADCA4_INIT {4, 14, 1, EPWM4SOCA}

/*Function prototypes*/
void ADC_A_vADCA_Group_Init();
void ADC_A_vADCA_SOC0Config(tstADCAConfig ADC);
void ADC_A_vADCA_SOC1Config(tstADCAConfig ADC);
void ADC_C_vADCA_SOC0Disable();
void ADC_C_vADCA_SOC1Disable();
Uint16 ADC_A_u16ADCAgetRawValue0();
Uint16 ADC_A_u16ADCAgetRawValue1();
/*Variables*/
extern tstADCAConfig stADCA1;
extern tstADCAConfig stADCA2;
extern tstADCAConfig stADCA3;
extern tstADCAConfig stADCA4;

#endif /* ADC_A_H_ */
