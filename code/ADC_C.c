/*
 * ADC_C.c
 *
 *  Created on: 20 oct. 2023
 *      Author: Paul
 */

/* Project Headers */
#include "F28x_Project.h"

/* System Headers*/
#include <stdlib.h>
#include <ADC_C.h>
/* Own Headers */
#include "SYSPARAM.h"
#include "GPIO.h"
/* External Headers */

/* Function Prototypes */
void ADC_C_vADCC_Group_Init();
void ADC_C_vADCC_SOC0Config(tstADCCConfig ADC);
void ADC_C_vADCC_SOC1Config(tstADCCConfig ADC);
void ADC_C_vADCC_SOC2Config(tstADCCConfig ADC);
void ADC_C_vADCC_SOC3Config(tstADCCConfig ADC);
void ADC_C_vADCC_SOC0Disable();
void ADC_C_vADCC_SOC1Disable();
void ADC_C_vADCC_SOC2Disable();
void ADC_C_vADCC_SOC3Disable();
Uint16 ADC_C_u16ADCCgetRawValue0();
Uint16 ADC_C_u16ADCCgetRawValue1();
Uint16 ADC_C_u16ADCCgetRawValue2();
Uint16 ADC_C_u16ADCCgetRawValue3();
__interrupt void adcc1_isr(void);
__interrupt void adcc2_isr(void);
__interrupt void adcc3_isr(void);
__interrupt void adcc4_isr(void);
/* Global Variables */
tstADCCConfig stADCC1 = ADCC1_INIT;
tstADCCConfig stADCC2 = ADCC2_INIT;
tstADCCConfig stADCC3 = ADCC3_INIT;
tstADCCConfig stADCC4 = ADCC4_INIT;

void SetVREF(int module, int mode, int ref)
{
    Uint16 *offset, offval;

    //
    // Define offset locations from OTP
    //
    offset = (Uint16 *)(0x70594 + (module * 6));

    if((mode == ADC_INTERNAL) && (ref == ADC_VREF3P3))
    {
        offval = (*offset) >> 8;    // Internal / 1.65v mode offset
    }
    else
    {
        offval = (*offset) & 0xFF;  // All other modes
    }

    //
    // Write offset trim values and configure reference modes
    //
    EALLOW;
    switch(module)
    {
        case 0:
            AdcaRegs.ADCOFFTRIM.bit.OFFTRIM = offval;
            AnalogSubsysRegs.ANAREFCTL.bit.ANAREFASEL = mode;
            AnalogSubsysRegs.ANAREFCTL.bit.ANAREFA2P5SEL = ref;
            break;
        case 1:
            AdcbRegs.ADCOFFTRIM.bit.OFFTRIM = offval;
            AnalogSubsysRegs.ANAREFCTL.bit.ANAREFBSEL = mode;
            AnalogSubsysRegs.ANAREFCTL.bit.ANAREFB2P5SEL = ref;
            break;
        case 2:
            AdccRegs.ADCOFFTRIM.bit.OFFTRIM = offval;
            AnalogSubsysRegs.ANAREFCTL.bit.ANAREFCSEL = mode;
            AnalogSubsysRegs.ANAREFCTL.bit.ANAREFC2P5SEL = ref;
            break;
        default:
            break;
    }
    EDIS;
}

/*Initialize ADC Group*/
void ADC_C_vADCC_Group_Init()
{

    SetVREF(ADC_ADCC, ADC_INTERNAL, ADC_VREF3P3);
    SetVREF(ADC_ADCB, ADC_INTERNAL, ADC_VREF3P3);
    EALLOW;
    AdccRegs.ADCCTL2.bit.PRESCALE = 6;//Divide sysclk by 4
    AdccRegs.ADCCTL1.bit.INTPULSEPOS = 1;
    AdccRegs.ADCCTL1.bit.ADCPWDNZ = 1;
    PieVectTable.ADCC1_INT = &adcc1_isr;
    PieVectTable.ADCC2_INT = &adcc2_isr;
    PieVectTable.ADCC3_INT = &adcc3_isr;
    PieVectTable.ADCC4_INT = &adcc4_isr;
    EDIS;
    DELAY_US(1000);

}

/*Initialize ADC by SOC*/
void ADC_C_vADCC_SOC0Config(tstADCCConfig ADC)
{
        EALLOW;
        AdccRegs.ADCSOC0CTL.bit.CHSEL = ADC.u16ADCChannel; //Read pin
        AdccRegs.ADCSOC0CTL.bit.ACQPS = ADC.u16AqWindow; //Sample window
        AdccRegs.ADCSOC0CTL.bit.TRIGSEL = ADC.u16TriggerSRC;
        if(ADC.u16Interrupt == 1)
        {
            AdccRegs.ADCINTSEL1N2.bit.INT1SEL = EOC0; //end of SOC0 will set INT1 flag
            AdccRegs.ADCINTSEL1N2.bit.INT1E = ENABLE;   //enable INT1 flag
            AdccRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //make sure INT1 flag is cleared
            PieCtrlRegs.PIEIER1.bit.INTx3 = 1; //ADCC1 Interrupt
        }
        if(ADC.u16Interrupt == 2)
        {
            AdccRegs.ADCINTSEL1N2.bit.INT2SEL = EOC0; //end of SOC0 will set INT2 flag
            AdccRegs.ADCINTSEL1N2.bit.INT2E = ENABLE;   //enable INT2 flag
            AdccRegs.ADCINTFLGCLR.bit.ADCINT2 = 1; //make sure INT2 flag is cleared
            PieCtrlRegs.PIEIER10.bit.INTx10 = 1; //ADCC2 Interrupt
        }
        if(ADC.u16Interrupt == 3)
        {
            AdccRegs.ADCINTSEL3N4.bit.INT3SEL = EOC0; //end of SOC0 will set INT3 flag
            AdccRegs.ADCINTSEL3N4.bit.INT3E = ENABLE;   //enable INT3 flag
            AdccRegs.ADCINTFLGCLR.bit.ADCINT3 = 1; //make sure INT3 flag is cleared
            PieCtrlRegs.PIEIER10.bit.INTx11 = 1; //ADCC3 Interrupt
        }
        if(ADC.u16Interrupt == 4)
        {
            AdccRegs.ADCINTSEL3N4.bit.INT4SEL = EOC0; //end of SOC0 will set INT4 flag
            AdccRegs.ADCINTSEL3N4.bit.INT4E = ENABLE;   //enable INT4 flag
            AdccRegs.ADCINTFLGCLR.bit.ADCINT4 = 1; //make sure INT4 flag is cleared
            PieCtrlRegs.PIEIER10.bit.INTx12 = 1; //ADCC4 Interrupt
        }

        EDIS;
}

//Disable the Trigger of the SOC
void ADC_C_vADCC_SOC0Disable()
{
    EALLOW;
    AdccRegs.ADCSOC0CTL.bit.TRIGSEL = DISABLE;//0x00 - Software only trigger
    EDIS;
}

//Initialize ADC by SOC
void ADC_C_vADCC_SOC1Config(tstADCCConfig ADC)
{
        EALLOW;
        AdccRegs.ADCSOC1CTL.bit.CHSEL = ADC.u16ADCChannel;  //SOC will convert pin A0
        AdccRegs.ADCSOC1CTL.bit.ACQPS = ADC.u16AqWindow; //sample window is 100 SYSCLK cycles

        if(ADC.u16Interrupt == 1)
        {
            AdccRegs.ADCINTSEL1N2.bit.INT1SEL = EOC1; //end of SOC will set INT1 flag
            AdccRegs.ADCINTSEL1N2.bit.INT1E = ENABLE;  //enable INT1 flag
            AdccRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //make sure INT1 flag is cleared
            PieCtrlRegs.PIEIER1.bit.INTx3 = 1; //ADCC1 Interrupt
        }
        if(ADC.u16Interrupt == 2)
        {
            AdccRegs.ADCINTSEL1N2.bit.INT2SEL = EOC1; //end of SOC0 will set INT1 flag
            AdccRegs.ADCINTSEL1N2.bit.INT2E = ENABLE;  //enable INT1 flag
            AdccRegs.ADCINTFLGCLR.bit.ADCINT2 = 1; //make sure INT1 flag is cleared
            PieCtrlRegs.PIEIER10.bit.INTx10 = 1; //ADCC2 Interrupt
        }
        if(ADC.u16Interrupt == 3)
        {
            AdccRegs.ADCINTSEL3N4.bit.INT3SEL = EOC1; //end of SOC0 will set INT1 flag
            AdccRegs.ADCINTSEL3N4.bit.INT3E = ENABLE;;   //enable INT1 flag
            AdccRegs.ADCINTFLGCLR.bit.ADCINT3 = 1; //make sure INT1 flag is cleared
            PieCtrlRegs.PIEIER10.bit.INTx11 = 1; //ADCC3 Interrupt
        }
        if(ADC.u16Interrupt == 4)
        {
            AdccRegs.ADCINTSEL3N4.bit.INT4SEL = EOC1; //end of SOC0 will set INT1 flag
            AdccRegs.ADCINTSEL3N4.bit.INT4E = ENABLE;   //enable INT1 flag
            AdccRegs.ADCINTFLGCLR.bit.ADCINT4 = 1; //make sure INT1 flag is cleared
            PieCtrlRegs.PIEIER10.bit.INTx12 = 1; //ADCC4 Interrupt
        }
        AdccRegs.ADCSOC1CTL.bit.TRIGSEL = ADC.u16TriggerSRC;

        EDIS;
}

//Disable the Trigger of the SOC
void ADC_C_vADCC_SOC1Disable()
{
    EALLOW;
    AdccRegs.ADCSOC1CTL.bit.TRIGSEL = DISABLE;//0x00 - Software only trigger
    EDIS;
}

//Initialize ADC by SOC
void ADC_C_vADCC_SOC2Config(tstADCCConfig ADC)
{
        EALLOW;
        AdccRegs.ADCSOC2CTL.bit.CHSEL = ADC.u16ADCChannel;  //SOC will convert pin A0
        AdccRegs.ADCSOC2CTL.bit.ACQPS = ADC.u16AqWindow; //sample window is 100 SYSCLK cycles

        if(ADC.u16Interrupt==1)
        {
            AdccRegs.ADCINTSEL1N2.bit.INT1SEL = EOC2; //end of SOC will set INT1 flag
            AdccRegs.ADCINTSEL1N2.bit.INT1E = ENABLE;  //enable INT1 flag
            AdccRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //make sure INT1 flag is cleared
            PieCtrlRegs.PIEIER1.bit.INTx3 = 1; //ADCC1 Interrupt
        }
        if(ADC.u16Interrupt==2)
        {
            AdccRegs.ADCINTSEL1N2.bit.INT2SEL = EOC2; //end of SOC0 will set INT1 flag
            AdccRegs.ADCINTSEL1N2.bit.INT2E = ENABLE;  //enable INT1 flag
            AdccRegs.ADCINTFLGCLR.bit.ADCINT2 = 1; //make sure INT1 flag is cleared
            PieCtrlRegs.PIEIER10.bit.INTx10 = 1; //ADCC2 Interrupt
        }
        if(ADC.u16Interrupt==3)
        {
            AdccRegs.ADCINTSEL3N4.bit.INT3SEL = EOC2; //end of SOC0 will set INT1 flag
            AdccRegs.ADCINTSEL3N4.bit.INT3E = ENABLE;;   //enable INT1 flag
            AdccRegs.ADCINTFLGCLR.bit.ADCINT3 = 1; //make sure INT1 flag is cleared
            PieCtrlRegs.PIEIER10.bit.INTx11 = 1; //ADCC3 Interrupt
        }
        if(ADC.u16Interrupt==4)
        {
            AdccRegs.ADCINTSEL3N4.bit.INT4SEL = EOC2; //end of SOC0 will set INT1 flag
            AdccRegs.ADCINTSEL3N4.bit.INT4E = ENABLE;   //enable INT1 flag
            AdccRegs.ADCINTFLGCLR.bit.ADCINT4 = 1; //make sure INT1 flag is cleared
            PieCtrlRegs.PIEIER10.bit.INTx12 = 1; //ADCC4 Interrupt
        }
        AdccRegs.ADCSOC2CTL.bit.TRIGSEL = ADC.u16TriggerSRC;

        EDIS;
}

//Disable the Trigger of the SOC
void ADC_C_vADCC_SOC2Disable()
{
    EALLOW;
    AdccRegs.ADCSOC2CTL.bit.TRIGSEL = DISABLE;//0x00 - Software only trigger
    EDIS;
}

//Initialize ADC by SOC
void ADC_C_vADCC_SOC3Config(tstADCCConfig ADC)
{
        EALLOW;
        AdccRegs.ADCSOC3CTL.bit.CHSEL = ADC.u16ADCChannel;  //SOC will convert pin A0
        AdccRegs.ADCSOC3CTL.bit.ACQPS = ADC.u16AqWindow; //sample window is 100 SYSCLK cycles

        if(ADC.u16Interrupt==1)
        {
            AdccRegs.ADCINTSEL1N2.bit.INT1SEL = EOC3; //end of SOC will set INT1 flag
            AdccRegs.ADCINTSEL1N2.bit.INT1E = ENABLE;  //enable INT1 flag
            AdccRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //make sure INT1 flag is cleared
        }
        if(ADC.u16Interrupt==2)
        {
            AdccRegs.ADCINTSEL1N2.bit.INT2SEL = EOC3; //end of SOC0 will set INT1 flag
            AdccRegs.ADCINTSEL1N2.bit.INT2E = ENABLE;  //enable INT1 flag
            AdccRegs.ADCINTFLGCLR.bit.ADCINT2 = 1; //make sure INT1 flag is cleared
        }
        if(ADC.u16Interrupt==3)
        {
            AdccRegs.ADCINTSEL3N4.bit.INT3SEL = EOC3; //end of SOC0 will set INT1 flag
            AdccRegs.ADCINTSEL3N4.bit.INT3E = ENABLE;;   //enable INT1 flag
            AdccRegs.ADCINTFLGCLR.bit.ADCINT3 = 1; //make sure INT1 flag is cleared
        }
        if(ADC.u16Interrupt==4)
        {
            AdccRegs.ADCINTSEL3N4.bit.INT4SEL = EOC3; //end of SOC0 will set INT1 flag
            AdccRegs.ADCINTSEL3N4.bit.INT4E = ENABLE;   //enable INT1 flag
            AdccRegs.ADCINTFLGCLR.bit.ADCINT4 = 1; //make sure INT1 flag is cleared
        }
        AdccRegs.ADCSOC3CTL.bit.TRIGSEL = ADC.u16TriggerSRC;
        //AdcaRegs.ADCCTL1.bit.ADCPWDNZ = 1;
        EDIS;
}

//Disable the Trigger of the SOC
void ADC_C_vADCC_SOC3Disable()
{
    EALLOW;
    AdccRegs.ADCSOC3CTL.bit.TRIGSEL = DISABLE;//0x00 - Software only trigger
    EDIS;
}

Uint16 ADC_C_u16ADCCgetRawValue0()
{
    return AdccResultRegs.ADCRESULT0;
}

Uint16 ADC_C_u16ADCCgetRawValue1()
{
    return AdccResultRegs.ADCRESULT1;
}

Uint16 ADC_C_u16ADCCgetRawValue2()
{
    return AdccResultRegs.ADCRESULT2;
}

Uint16 ADC_C_u16ADCCgetRawValue3()
{
    return AdccResultRegs.ADCRESULT3;
}

//ADCC Interrupt 1
__interrupt void adcc1_isr(void)
{
    GPIO_vToggle(13);
    AdccRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //clear INT1 flag
    if(1 == AdccRegs.ADCINTOVF.bit.ADCINT1)
    {
        AdccRegs.ADCINTOVFCLR.bit.ADCINT1 = 1; //clear INT1 overflow flag
        AdccRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //clear INT1 flag
    }
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;

}
//ADCC Interrupt 2
__interrupt void adcc2_isr(void)
{
    AdccRegs.ADCINTFLGCLR.bit.ADCINT2 = 1; //clear INT2 flag
    if(1 == AdccRegs.ADCINTOVF.bit.ADCINT2)
    {
        AdccRegs.ADCINTOVFCLR.bit.ADCINT2 = 1; //clear INT1 overflow flag
        AdccRegs.ADCINTFLGCLR.bit.ADCINT2 = 1; //clear INT1 flag
    }
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP10;

}
//ADCC Interrupt 3
__interrupt void adcc3_isr(void)
{
    AdccRegs.ADCINTFLGCLR.bit.ADCINT3 = 1; //clear INT3 flag
    if(1 == AdccRegs.ADCINTOVF.bit.ADCINT3)
    {
        AdccRegs.ADCINTOVFCLR.bit.ADCINT3 = 1; //clear INT1 overflow flag
        AdccRegs.ADCINTFLGCLR.bit.ADCINT3 = 1; //clear INT1 flag
    }
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP10;
}
//ADCC Interrupt 4
__interrupt void adcc4_isr(void)
{
    AdccRegs.ADCINTFLGCLR.bit.ADCINT4 = 1; //clear INT4 flag
    if(1 == AdccRegs.ADCINTOVF.bit.ADCINT4)
    {
        AdccRegs.ADCINTOVFCLR.bit.ADCINT4 = 1; //clear INT1 overflow flag
        AdccRegs.ADCINTFLGCLR.bit.ADCINT4 = 1; //clear INT1 flag
    }
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP10;
}



