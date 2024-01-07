/*
 * DAC.c
 *
 *  Created on: 21 oct. 2023
 *      Author: Paul
 */

#include "F28x_Project.h"

/* System Headefiles*/
#include <stdlib.h>

/* Own Headerfiles */
#include "SYSPARAM.h"
#include "DAC.h"

/* External Headers */

/* Function Prototypes */
void DAC_vDACAInit(tstDACData DACData);
void DAC_vDACBInit(tstDACData DACData);
void DAC_vDACAWriteValue(tstDACData DACData);
void DAC_vDACBWriteValue(tstDACData DACData);
void DAC_vDACAWriteFloatValue(float Value);
void DAC_vDACBWriteFloatValue(float Value);
/* Global Variables */
tstDACData stDACData1 = DAC1_INIT;
tstDACData stDACData2 = DAC2_INIT;

void DAC_vDACAInit(tstDACData DACData)
{
    EALLOW;
    /*DACA*/
    CpuSysRegs.PCLKCR16.bit.DAC_A = ENABLE;
    AnalogSubsysRegs.ANAREFCTL.bit.ANAREFASEL = ADC_INTERNAL;
    AnalogSubsysRegs.ANAREFCTL.bit.ANAREFA2P5SEL = ADC_VREF3P3;
    DacaRegs.DACCTL.bit.LOADMODE = DACData.u16Update;
    DacaRegs.DACCTL.bit.SYNCSEL = DACData.u16UpdateSrc;
    DacaRegs.DACCTL.bit.MODE = GAIN2;
    DacaRegs.DACCTL.bit.DACREFSEL = REFERENCE_VREF;
    DacaRegs.DACOUTEN.bit.DACOUTEN = ENABLE;
    DELAY_US(100);
    EDIS;
}

void DAC_vDACBInit(tstDACData DACData)
{
    EALLOW;
    /*DACB*/
    CpuSysRegs.PCLKCR16.bit.DAC_B = ENABLE;
    AnalogSubsysRegs.ANAREFCTL.bit.ANAREFBSEL = ADC_INTERNAL;
    AnalogSubsysRegs.ANAREFCTL.bit.ANAREFB2P5SEL = ADC_VREF3P3;
    /*B and C References bonded together!*/
    AnalogSubsysRegs.ANAREFCTL.bit.ANAREFCSEL = ADC_INTERNAL;
    AnalogSubsysRegs.ANAREFCTL.bit.ANAREFC2P5SEL = ADC_VREF3P3;
    DacbRegs.DACCTL.bit.LOADMODE = DACData.u16Update;
    DacbRegs.DACCTL.bit.SYNCSEL = DACData.u16UpdateSrc;
    DacbRegs.DACCTL.bit.MODE = GAIN2;
    DacbRegs.DACCTL.bit.DACREFSEL = REFERENCE_VREF;
    DacbRegs.DACOUTEN.bit.DACOUTEN = ENABLE;
    DELAY_US(100);
    EDIS;
}

void DAC_vDACAWriteValue(tstDACData DACData)
{
    DacaRegs.DACVALS.bit.DACVALS = DACData.u16Value * DACData.fSlope + DACData.i16Offset;
}

void DAC_vDACBWriteValue(tstDACData DACData)
{
    DacbRegs.DACVALS.bit.DACVALS = DACData.u16Value * DACData.fSlope + DACData.i16Offset;
}

void DAC_vDACAWriteFloatValue(float Value)
{
    DacaRegs.DACVALS.bit.DACVALS = (Uint16)Value;
}
void DAC_vDACBWriteFloatValue(float Value)
{
    DacbRegs.DACVALS.bit.DACVALS = (Uint16)Value;
}

