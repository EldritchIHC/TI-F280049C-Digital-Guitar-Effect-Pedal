/*
 * ADC_A.c
 *
 *  Created on: 21 oct. 2023
 *      Author: Paul
 */

/* Project Headers */
#include "F28x_Project.h"

/* System Headers*/
#include <stdlib.h>
#include <ADC_A.h>

/* Own Headers */
#include "SYSPARAM.h"
#include "GPIO.h"
/* External Headers */

/* Function Prototypes */

void ADC_A_vADCA_Group_Init();
void ADC_A_vADCA_SOC0Config(tstADCAConfig ADC);
void ADC_A_vADCA_SOC1Config(tstADCAConfig ADC);
void ADC_C_vADCA_SOC0Disable();
void ADC_C_vADCA_SOC1Disable();
Uint16 ADC_A_u16ADCAgetRawValue0();
Uint16 ADC_A_u16ADCAgetRawValue1();
__interrupt void adca1_isr(void);
__interrupt void adca2_isr(void);
__interrupt void adca3_isr(void);
__interrupt void adca4_isr(void);
/* Global Variables */
tstADCAConfig stADCA1 = ADCA1_INIT;
tstADCAConfig stADCA2 = ADCA2_INIT;
tstADCAConfig stADCA3 = ADCA3_INIT;
tstADCAConfig stADCA4 = ADCA4_INIT;




/*Initialize ADC Group */
void ADC_A_vADCA_Group_Init()
{
    SetVREF(ADC_ADCA, ADC_INTERNAL, ADC_VREF3P3);
    EALLOW;
    AdcaRegs.ADCCTL2.bit.PRESCALE = 0;//Divide sysclk by 1
    AdcaRegs.ADCCTL1.bit.INTPULSEPOS = 1;
    AdcaRegs.ADCCTL1.bit.ADCPWDNZ = 1;
    PieVectTable.ADCA1_INT = &adca1_isr;
    PieVectTable.ADCA2_INT = &adca2_isr;
    PieVectTable.ADCA3_INT = &adca3_isr;
    PieVectTable.ADCA4_INT = &adca4_isr;
    EDIS;
    DELAY_US(1000);
}

/*Initialize ADC by SOC*/
void ADC_A_vADCA_SOC0Config(tstADCAConfig ADC)
{
        EALLOW;
        AdcaRegs.ADCSOC0CTL.bit.CHSEL = ADC.u16ADCChannel; //Read pin
        AdcaRegs.ADCSOC0CTL.bit.ACQPS = ADC.u16AqWindow; //Sample window
        AdcaRegs.ADCSOC0CTL.bit.TRIGSEL = ADC.u16TriggerSRC;
        if(ADC.u16Interrupt==1)
        {
            AdcaRegs.ADCINTSEL1N2.bit.INT1SEL = EOC0; //end of SOC0 will set INT1 flag
            AdcaRegs.ADCINTSEL1N2.bit.INT1E = ENABLE;   //enable INT1 flag
            AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //make sure INT1 flag is cleared
            PieCtrlRegs.PIEIER1.bit.INTx1 = 1; //ADCA1 Interrupt
        }
        if(ADC.u16Interrupt==2)
        {
            AdcaRegs.ADCINTSEL1N2.bit.INT2SEL = EOC0; //end of SOC0 will set INT2 flag
            AdcaRegs.ADCINTSEL1N2.bit.INT2E = ENABLE;   //enable INT2 flag
            AdcaRegs.ADCINTFLGCLR.bit.ADCINT2 = 1; //make sure INT2 flag is cleared
            PieCtrlRegs.PIEIER10.bit.INTx2 = 1; //ADCA2 Interrupt
        }
        if(ADC.u16Interrupt==3)
        {
            AdcaRegs.ADCINTSEL3N4.bit.INT3SEL = EOC0; //end of SOC0 will set INT3 flag
            AdcaRegs.ADCINTSEL3N4.bit.INT3E = ENABLE;   //enable INT3 flag
            AdcaRegs.ADCINTFLGCLR.bit.ADCINT3 = 1; //make sure INT3 flag is cleared
            PieCtrlRegs.PIEIER10.bit.INTx3 = 1; //ADCA3 Interrupt
        }
        if(ADC.u16Interrupt==4)
        {
            AdcaRegs.ADCINTSEL3N4.bit.INT4SEL = EOC0; //end of SOC0 will set INT4 flag
            AdcaRegs.ADCINTSEL3N4.bit.INT4E = ENABLE;   //enable INT4 flag
            AdcaRegs.ADCINTFLGCLR.bit.ADCINT4 = 1; //make sure INT4 flag is cleared
            PieCtrlRegs.PIEIER10.bit.INTx4 = 1; //ADCA4 Interrupt
        }

        EDIS;
}

/*Disable the Trigger of the SOC*/
void ADC_C_vADCA_SOC0Disable()
{
    EALLOW;
    AdcaRegs.ADCSOC0CTL.bit.TRIGSEL = DISABLE;
    EDIS;
}

/*Initialize ADC by SOC*/
void ADC_A_vADCA_SOC1Config(tstADCAConfig ADC)
{
        EALLOW;
        AdcaRegs.ADCSOC1CTL.bit.CHSEL = ADC.u16ADCChannel;  //SOC will convert pin A0
        AdcaRegs.ADCSOC1CTL.bit.ACQPS = ADC.u16AqWindow; //sample window is 100 SYSCLK cycles

        if(ADC.u16Interrupt==1)
        {
            AdcaRegs.ADCINTSEL1N2.bit.INT1SEL = EOC1; //end of SOC will set INT1 flag
            AdcaRegs.ADCINTSEL1N2.bit.INT1E = ENABLE;  //enable INT1 flag
            AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //make sure INT1 flag is cleared
            PieCtrlRegs.PIEIER1.bit.INTx1 = 1; //ADCA1 Interrupt
        }
        if(ADC.u16Interrupt==2)
        {
            AdcaRegs.ADCINTSEL1N2.bit.INT2SEL = EOC1; //end of SOC0 will set INT1 flag
            AdcaRegs.ADCINTSEL1N2.bit.INT2E = ENABLE;  //enable INT1 flag
            AdcaRegs.ADCINTFLGCLR.bit.ADCINT2 = 1; //make sure INT1 flag is cleared
            PieCtrlRegs.PIEIER10.bit.INTx2 = 1; //ADCA2 Interrupt
        }
        if(ADC.u16Interrupt==3)
        {
            AdcaRegs.ADCINTSEL3N4.bit.INT3SEL = EOC1; //end of SOC0 will set INT1 flag
            AdcaRegs.ADCINTSEL3N4.bit.INT3E = ENABLE;;   //enable INT1 flag
            AdcaRegs.ADCINTFLGCLR.bit.ADCINT3 = 1; //make sure INT1 flag is cleared
            PieCtrlRegs.PIEIER10.bit.INTx3 = 1; //ADCA3 Interrupt
        }
        if(ADC.u16Interrupt==4)
        {
            AdcaRegs.ADCINTSEL3N4.bit.INT4SEL = EOC1; //end of SOC0 will set INT1 flag
            AdcaRegs.ADCINTSEL3N4.bit.INT4E = ENABLE;   //enable INT1 flag
            AdcaRegs.ADCINTFLGCLR.bit.ADCINT4 = 1; //make sure INT1 flag is cleared
            PieCtrlRegs.PIEIER10.bit.INTx4 = 1; //ADCA4 Interrupt
        }
        AdcaRegs.ADCSOC1CTL.bit.TRIGSEL = ADC.u16TriggerSRC;
        //AdcaRegs.ADCCTL1.bit.ADCPWDNZ = 1;
        EDIS;
}

/*Disable the Trigger of the SOC*/
void ADC_C_vADCA_SOC1Disable()
{
    EALLOW;
    AdcaRegs.ADCSOC1CTL.bit.TRIGSEL = DISABLE;
    EDIS;
}

Uint16 ADC_A_u16ADCAgetRawValue0()
{
    return AdcaResultRegs.ADCRESULT0;
}

Uint16 ADC_A_u16ADCAgetRawValue1()
{
    return AdcaResultRegs.ADCRESULT1;
}

//ADCA Interrupt 1
__interrupt void adca1_isr(void)
{
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //clear INT1 flag
    if(1 == AdcaRegs.ADCINTOVF.bit.ADCINT1)
    {
        AdcaRegs.ADCINTOVFCLR.bit.ADCINT1 = 1; //clear INT1 overflow flag
        AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //clear INT1 flag
    }
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

//ADCA Interrupt 2
__interrupt void adca2_isr(void)
{
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT2 = 1; //clear INT2 flag
    if(1 == AdcaRegs.ADCINTOVF.bit.ADCINT2)
    {
        AdcaRegs.ADCINTOVFCLR.bit.ADCINT2 = 1; //clear INT1 overflow flag
        AdcaRegs.ADCINTFLGCLR.bit.ADCINT2 = 1; //clear INT1 flag
    }
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP10;
}

//ADCA Interrupt 3
__interrupt void adca3_isr(void)
{
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT3 = 1; //clear INT3 flag
    if(1 == AdcaRegs.ADCINTOVF.bit.ADCINT3)
    {
        AdcaRegs.ADCINTOVFCLR.bit.ADCINT3 = 1; //clear INT1 overflow flag
        AdcaRegs.ADCINTFLGCLR.bit.ADCINT3 = 1; //clear INT1 flag
    }
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP10;

}

//ADCA Interrupt 4
__interrupt void adca4_isr(void)
{
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT4 = 1; //clear INT4 flag
    if(1 == AdcaRegs.ADCINTOVF.bit.ADCINT4)
    {
        AdcaRegs.ADCINTOVFCLR.bit.ADCINT4 = 1; //clear INT1 overflow flag
        AdcaRegs.ADCINTFLGCLR.bit.ADCINT4 = 1; //clear INT1 flag
    }
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP10;
}



