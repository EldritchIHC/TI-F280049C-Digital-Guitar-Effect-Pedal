/*
 * DAC.h
 *
 *  Created on: 21 oct. 2023
 *      Author: Paul
 */

#ifndef DAC_H_
#define DAC_H_

/*Defines*/

#define REFERENCE_VDAC  0
#define REFERENCE_VREF  1

/*Which EPWM Period will update the DAC*/
#define EPWM1SYNCPER 0
#define EPWM2SYNCPER 1
#define EPWM3SYNCPER 2
#define EPWM4SYNCPER 3
#define EPWM5SYNCPER 4
#define EPWM6SYNCPER 5
/*Update on SYSCLK or on SYNCSEL*/
#define SYSCLK 0
#define SYNCSELCLK 1

#define GAIN1 0
#define GAIN2 1

#define MAX12BIT 4095U

/*Structures*/
typedef struct{
    Uint16 u16Update;
    Uint16 u16UpdateSrc;
    Uint16 u16Value;
    float fSlope;
    int16 i16Offset;
}tstDACData;

/*Structure Initializations*/
#define DAC1_INIT {SYSCLK, 0, 0, 1, 0}
#define DAC2_INIT {SYSCLK, 0, 0, 1, 0}

/*Function prototypes*/
void DAC_vDACAInit(tstDACData DACData);
void DAC_vDACBInit(tstDACData DACData);
void DAC_vDACAWriteValue(tstDACData DACData);
void DAC_vDACBWriteValue(tstDACData DACData);
void DAC_vDACAWriteFloatValue(float Value);
void DAC_vDACBWriteFloatValue(float Value);

/*Variables*/
extern tstDACData stDACData1;
extern tstDACData stDACData2;

#endif /* DAC_H_ */
