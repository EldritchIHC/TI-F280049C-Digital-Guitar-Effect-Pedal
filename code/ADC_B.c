/*
 * ADC_B.c
 *
 *  Created on: 21 oct. 2023
 *      Author: Paul
 */

/* Project Headers */
#include "F28x_Project.h"

/* System Headers*/
#include <stdlib.h>
#include <ADC_B.h>
/* Own Headers */
#include "SYSPARAM.h"
#include "GPIO.h"
#include "EFFECTS.h"

/* External Headers */

/* Function Prototypes */
void ADC_B_vADCB_Group_Init();
void ADC_B_vADCB_SOC0Config(tstADCBConfig ADC);
void ADC_B_vADCB_SOC1Config(tstADCBConfig ADC);
Uint16 ADC_B_u16ADCBgetRawValue0();
Uint16 ADC_B_u16ADCBgetRawValue1();
__interrupt void adcb1_isr(void);
__interrupt void adcb2_isr(void);
__interrupt void adcb3_isr(void);
__interrupt void adcb4_isr(void);
/* Global Variables */
tstADCBConfig stADCB1 = ADCB1_INIT;
tstADCBConfig stADCB2 = ADCB2_INIT;
tstADCBConfig stADCB3 = ADCB3_INIT;
tstADCBConfig stADCB4 = ADCB4_INIT;

/*Initialize ADC Group*/
void ADC_B_vADCB_Group_Init()
{
    SetVREF(ADC_ADCB, ADC_INTERNAL, ADC_VREF3P3);
    SetVREF(ADC_ADCC, ADC_INTERNAL, ADC_VREF3P3);
    EALLOW;
    AdcbRegs.ADCCTL2.bit.PRESCALE = 0;//Divide sysclk by 1
    AdcbRegs.ADCCTL1.bit.INTPULSEPOS = 1;
    AdcbRegs.ADCCTL1.bit.ADCPWDNZ = 1;
    PieVectTable.ADCB1_INT = &adcb1_isr;
    PieVectTable.ADCB2_INT = &adcb2_isr;
    PieVectTable.ADCB3_INT = &adcb3_isr;
    PieVectTable.ADCB4_INT = &adcb4_isr;
    EDIS;
    DELAY_US(1000);
}
/*Initialize ADC by SOC*/
void ADC_B_vADCB_SOC0Config(tstADCBConfig ADC)
{
        EALLOW;
        AdcbRegs.ADCSOC0CTL.bit.CHSEL = ADC.u16ADCChannel;  //Read pin
        AdcbRegs.ADCSOC0CTL.bit.ACQPS = ADC.u16AqWindow; //Sample window
        AdcbRegs.ADCSOC0CTL.bit.TRIGSEL = ADC.u16TriggerSRC;
        if(ADC.u16Interrupt == 1)
        {
            AdcbRegs.ADCINTSEL1N2.bit.INT1SEL = EOC0; //end of SOC0 will set INT1 flag
            AdcbRegs.ADCINTSEL1N2.bit.INT1E = ENABLE;   //enable INT1 flag
            AdcbRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //make sure INT1 flag is cleared
            PieCtrlRegs.PIEIER1.bit.INTx2 = 1; //ADCB1 Interrupt
        }
        if(ADC.u16Interrupt == 2)
        {
            AdcbRegs.ADCINTSEL1N2.bit.INT2SEL = EOC0; //end of SOC0 will set INT2 flag
            AdcbRegs.ADCINTSEL1N2.bit.INT2E = ENABLE;   //enable INT2 flag
            AdcbRegs.ADCINTFLGCLR.bit.ADCINT2 = 1; //make sure INT2 flag is cleared
            PieCtrlRegs.PIEIER10.bit.INTx6 = 1; //ADCB2 Interrupt
        }
        if(ADC.u16Interrupt == 3)
        {
            AdcbRegs.ADCINTSEL3N4.bit.INT3SEL = EOC0; //end of SOC0 will set INT3 flag
            AdcbRegs.ADCINTSEL3N4.bit.INT3E = ENABLE;   //enable INT3 flag
            AdcbRegs.ADCINTFLGCLR.bit.ADCINT3 = 1; //make sure INT3 flag is cleared
            PieCtrlRegs.PIEIER10.bit.INTx7 = 1; //ADCB3 Interrupt
        }
        if(ADC.u16Interrupt == 4)
        {
            AdcbRegs.ADCINTSEL3N4.bit.INT4SEL = EOC0; //end of SOC0 will set INT4 flag
            AdcbRegs.ADCINTSEL3N4.bit.INT4E = ENABLE;   //enable INT4 flag
            AdcbRegs.ADCINTFLGCLR.bit.ADCINT4 = 1; //make sure INT4 flag is cleared
            PieCtrlRegs.PIEIER10.bit.INTx8 = 1; //ADCB3 Interrupt
        }

        EDIS;
}

/*Disable the Trigger of the SOC*/
void ADC_B_vADCB_SOC0Disable()
{
    EALLOW;
    AdcbRegs.ADCSOC0CTL.bit.TRIGSEL = DISABLE;
    EDIS;
}

/*Initialize ADC by SOC*/
void ADC_B_vADCB_SOC1Config(tstADCBConfig ADC)
{
        EALLOW;
        AdcbRegs.ADCSOC1CTL.bit.CHSEL = ADC.u16ADCChannel;  //SOC will convert pin A0
        AdcbRegs.ADCSOC1CTL.bit.ACQPS = ADC.u16AqWindow; //sample window is 100 SYSCLK cycles
        AdcbRegs.ADCSOC1CTL.bit.TRIGSEL = ADC.u16TriggerSRC;
        if(ADC.u16Interrupt==1)
        {
            AdcbRegs.ADCINTSEL1N2.bit.INT1SEL = EOC1; //end of SOC will set INT1 flag
            AdcbRegs.ADCINTSEL1N2.bit.INT1E = ENABLE;  //enable INT1 flag
            AdcbRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //make sure INT1 flag is cleared
            PieCtrlRegs.PIEIER1.bit.INTx2 = 1; //ADCB1 Interrupt
        }
        if(ADC.u16Interrupt==2)
        {
            AdcbRegs.ADCINTSEL1N2.bit.INT2SEL = EOC1; //end of SOC0 will set INT1 flag
            AdcbRegs.ADCINTSEL1N2.bit.INT2E = ENABLE;  //enable INT1 flag
            AdcbRegs.ADCINTFLGCLR.bit.ADCINT2 = 1; //make sure INT1 flag is cleared
            PieCtrlRegs.PIEIER10.bit.INTx6 = 1; //ADCB2 Interrupt
        }
        if(ADC.u16Interrupt==3)
        {
            AdcbRegs.ADCINTSEL3N4.bit.INT3SEL = EOC1; //end of SOC0 will set INT1 flag
            AdcbRegs.ADCINTSEL3N4.bit.INT3E = ENABLE;;   //enable INT1 flag
            AdcbRegs.ADCINTFLGCLR.bit.ADCINT3 = 1; //make sure INT1 flag is cleared
            PieCtrlRegs.PIEIER10.bit.INTx7 = 1; //ADCB3 Interrupt
        }
        if(ADC.u16Interrupt==4)
        {
            AdcbRegs.ADCINTSEL3N4.bit.INT4SEL = EOC1; //end of SOC0 will set INT1 flag
            AdcbRegs.ADCINTSEL3N4.bit.INT4E = ENABLE;   //enable INT1 flag
            AdcbRegs.ADCINTFLGCLR.bit.ADCINT4 = 1; //make sure INT1 flag is cleared
            PieCtrlRegs.PIEIER10.bit.INTx8 = 1; //ADCB3 Interrupt
        }

        EDIS;
}

/*Disable the Trigger of the SOC*/
void ADC_B_vADCB_SOC1Disable()
{
    EALLOW;
    AdcbRegs.ADCSOC1CTL.bit.TRIGSEL = DISABLE;
    EDIS;
}

Uint16 ADC_B_u16ADCBgetRawValue0()
{
    return AdcbResultRegs.ADCRESULT0;
}

Uint16 ADC_B_u16ADCBgetRawValue1()
{
    return AdcbResultRegs.ADCRESULT1;
}

//ADCB Interrupt 1
__interrupt void adcb1_isr(void)
{

    #ifdef DEBUG
    GPIO_vSet(DEBUG_PIN3);
    #endif

    fBuffer1 = ( ( (float)ADC_B_u16ADCBgetRawValue0() ) * UINT16_TO_FLOAT_SLOPE ) + UINT16_TO_FLOAT_OFFSET;
    AdcbRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //clear INT1 flag
    if(1 == AdcbRegs.ADCINTOVF.bit.ADCINT1)
    {
        AdcbRegs.ADCINTOVFCLR.bit.ADCINT1 = 1; //clear INT1 overflow flag
        AdcbRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //clear INT1 flag
    }
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;

    #ifdef DEBUG
    GPIO_vClear(DEBUG_PIN3);
    #endif

}

//ADCB Interrupt 2
__interrupt void adcb2_isr(void)
{
    AdcbRegs.ADCINTFLGCLR.bit.ADCINT2 = 1; //clear INT1 flag
    if(1 == AdcbRegs.ADCINTOVF.bit.ADCINT2)
    {
        AdcbRegs.ADCINTOVFCLR.bit.ADCINT2 = 1; //clear INT1 overflow flag
        AdcbRegs.ADCINTFLGCLR.bit.ADCINT2 = 1; //clear INT1 flag
    }
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP10;
}

//ADCB Interrupt 3
__interrupt void adcb3_isr(void)
{
    AdcbRegs.ADCINTFLGCLR.bit.ADCINT3 = 1; //clear INT1 flag
    if(1 == AdcbRegs.ADCINTOVF.bit.ADCINT3)
    {
        AdcbRegs.ADCINTOVFCLR.bit.ADCINT3 = 1; //clear INT1 overflow flag
        AdcbRegs.ADCINTFLGCLR.bit.ADCINT3 = 1; //clear INT1 flag
    }
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP10;
}

//ADCB Interrupt 4
__interrupt void adcb4_isr(void)
{

    AdcbRegs.ADCINTFLGCLR.bit.ADCINT4 = 1; //clear INT1 flag
    if(1 == AdcbRegs.ADCINTOVF.bit.ADCINT4)
        {
            AdcbRegs.ADCINTOVFCLR.bit.ADCINT4 = 1; //clear INT1 overflow flag
            AdcbRegs.ADCINTFLGCLR.bit.ADCINT4 = 1; //clear INT1 flag
        }
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP10;
}
