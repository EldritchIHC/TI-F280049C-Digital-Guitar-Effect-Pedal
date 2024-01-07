/*
 * TIMER.c
 *
 *  Created on: 14 oct. 2023
 *      Author: Paul
 */




/* Project Headers */
#include "F28x_Project.h"

/* System Headerfiles*/
#include <stdlib.h>
#include "SYSPARAM.h"
/* Own Headerfiles */
#include "TIMER.h"
#include "GPIO.h"
//#include "ADC_C.h"
#include "DAC.h"
#include "EFFECTS.h"
#include "INTERFACE.h"

/* Extern Headerfiles */


/* Function Prototypes */
void TIMER_vCPUTimerSecondsInit(tstCPUTimerSecondsConfig CPU);
void TIMER_vCPUTimerHertzInit(tstCPUTimerHertzConfig CPU);
__interrupt void cputimer0_isr();
__interrupt void cputimer1_isr();
__interrupt void cputimer2_isr();

/* Global Variables */
tstCPUTimerSecondsConfig stCPU1 = TIMER1_SEC_INIT;
tstCPUTimerSecondsConfig stCPU2 = TIMER2_SEC_INIT;
tstCPUTimerSecondsConfig stCPU3 = TIMER3_SEC_INIT;
tstCPUTimerHertzConfig stCPU4 = TIMER1_HEZ_INIT;
tstCPUTimerHertzConfig stCPU5 = TIMER2_HEZ_INIT;
tstCPUTimerHertzConfig stCPU6 = TIMER3_HEZ_INIT;
/*Notes*/
/*
INT1.7 - Timer0(Through PIE)
INT13 - Timer1(Directly connected)
INT14 - Timer2(Directly connected)
*/
void TIMER_vCPUTimerSecondsInit(tstCPUTimerSecondsConfig CPU)
{
    EALLOW;
    Uint64 u64TBPRD;
    Uint16 u16PRESCALER = 0;
    Uint16 u16LPRESCALER = 0;//Low bits of divider
    Uint16 u16HPRESCALER = 0;//High bits of divider
    u64TBPRD = ( CPU.u32Period_ms * MS_TO_NS ) / 2 / SYS_PERIOD_NS;
    while(u64TBPRD > UINT32MAX)
    {
         if(u16PRESCALER == 0)
             u16PRESCALER = 1;
         else
             u16PRESCALER = u16PRESCALER + 1;
         u64TBPRD = ( CPU.u32Period_ms * MS_TO_NS ) / 2 / ( SYS_PERIOD_NS * u16PRESCALER );
    }

    u16LPRESCALER = u16PRESCALER & 0x00FF;
    u16HPRESCALER = (u16PRESCALER & 0xFF00)>>8;

    switch(CPU.u16CPUTimer)
        {
            case CPUTIMER0:
                CpuTimer0Regs.TCR.bit.TSS = 1; // Stop Timer
                CpuTimer0Regs.PRD.all = u64TBPRD;     // Set the Period Register
                CpuTimer0Regs.TPR.bit.TDDR = u16LPRESCALER; // Set prescaler registers to 0
                CpuTimer0Regs.TPRH.bit.TDDRH = u16HPRESCALER ; // Set prescaler high registers to 0
                CpuTimer0Regs.TCR.bit.TRB = 1; // Reload Timer
                CpuTimer0Regs.TCR.bit.SOFT = 1;
                CpuTimer0Regs.TCR.bit.FREE = 1;
                CpuTimer0Regs.TCR.bit.TIF = 1; // Clear Interrupt Flag
                CpuTimer0Regs.TCR.bit.TIE = CPU.u16CPUTimerInterruptEnable; // Enable/Disable Timer Interrupts
                if(CPU.u16CPUTimerInterruptEnable)
                {
                    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
                    PieVectTable.TIMER0_INT = &cputimer0_isr;
                }
                CpuTimer0Regs.TCR.bit.TSS = 0; // Start Timer
            break;

            case CPUTIMER1:
                CpuTimer1Regs.TCR.bit.TSS = 1; // Stop Timer
                CpuTimer1Regs.PRD.all = u64TBPRD;     // Set the Period Register
                CpuTimer1Regs.TPR.bit.TDDR = u16LPRESCALER; // Set prescaler registers to 0
                CpuTimer1Regs.TPRH.bit.TDDRH = u16HPRESCALER; // Set prescaler high registers to 0
                CpuTimer1Regs.TCR.bit.TRB = 1; // Reload Timer
                CpuTimer1Regs.TCR.bit.SOFT = 1;
                CpuTimer1Regs.TCR.bit.FREE = 1;
                CpuTimer1Regs.TCR.bit.TIF = 1; // Clear Interrupt Flag
                CpuTimer1Regs.TCR.bit.TIE = CPU.u16CPUTimerInterruptEnable; // Disable Timer Interrupts
                if(CPU.u16CPUTimerInterruptEnable)PieVectTable.TIMER1_INT = &cputimer1_isr;
                CpuTimer1Regs.TCR.bit.TSS = 0; // Start Timer
            break;

            case CPUTIMER2:
                CpuTimer2Regs.TCR.bit.TSS = 1; // Stop Timer
                CpuTimer2Regs.PRD.all = u64TBPRD;     // Set the Period Register
                CpuTimer2Regs.TPR.bit.TDDR= u16LPRESCALER; // Set prescaler registers to 0
                CpuTimer2Regs.TPRH.bit.TDDRH = u16HPRESCALER; // Set prescaler high registers to 0
                CpuTimer2Regs.TCR.bit.TRB = 1; // Reload Timer
                CpuTimer2Regs.TCR.bit.SOFT = 1;
                CpuTimer2Regs.TCR.bit.FREE = 1;
                CpuTimer2Regs.TCR.bit.TIF = 1; // Clear Interrupt Flag
                CpuTimer2Regs.TCR.bit.TIE = CPU.u16CPUTimerInterruptEnable; // Disable Timer Interrupts
                if(CPU.u16CPUTimerInterruptEnable)PieVectTable.TIMER2_INT = &cputimer2_isr;
                CpuTimer2Regs.TCR.bit.TSS = 0; // Start Timer
            break;


        }
    EDIS;
}

void TIMER_vCPUTimerHertzInit(tstCPUTimerHertzConfig CPU)
{
    EALLOW;
    Uint64 u64TBPRD;
    Uint16 u16PRESCALER = 0;
    Uint16 u16LPRESCALER = 0;//Low bits of divider
    Uint16 u16HPRESCALER = 0;//High bits of divider
    u64TBPRD =  SYS_CLK_HZ  / CPU.u32Frequency_Hz;
    while(u64TBPRD > UINT32MAX)
    {
         if(u16PRESCALER == 0)
             u16PRESCALER = 1;
         else
             u16PRESCALER = u16PRESCALER + 1;
         u64TBPRD =  SYS_CLK_HZ  / ( u16PRESCALER * CPU.u32Frequency_Hz );
    }

    u16LPRESCALER = u16PRESCALER & 0x00FF;
    u16HPRESCALER = (u16PRESCALER & 0xFF00)>>8;

    switch(CPU.u16CPUTimer)
        {
            case CPUTIMER0:
                CpuTimer0Regs.TCR.bit.TSS = 1; // Stop Timer
                CpuTimer0Regs.PRD.all = u64TBPRD;     // Set the Period Register
                CpuTimer0Regs.TPR.bit.TDDR = u16LPRESCALER; // Set prescaler registers to 0
                CpuTimer0Regs.TPRH.bit.TDDRH = u16HPRESCALER ; // Set prescaler high registers to 0
                CpuTimer0Regs.TCR.bit.TRB = 1; // Reload Timer
                CpuTimer0Regs.TCR.bit.SOFT = 1;
                CpuTimer0Regs.TCR.bit.FREE = 1;
                CpuTimer0Regs.TCR.bit.TIF = 1; // Clear Interrupt Flag
                CpuTimer0Regs.TCR.bit.TIE = CPU.u16CPUTimerInterruptEnable; // Enable/Disable Timer Interrupts
                if(CPU.u16CPUTimerInterruptEnable)
                {
                    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
                    PieVectTable.TIMER0_INT = &cputimer0_isr;
                }
                CpuTimer0Regs.TCR.bit.TSS = 0; // Start Timer
            break;

            case CPUTIMER1:
                CpuTimer1Regs.TCR.bit.TSS = 1; // Stop Timer
                CpuTimer1Regs.PRD.all = u64TBPRD;     // Set the Period Register
                CpuTimer1Regs.TPR.bit.TDDR = u16LPRESCALER; // Set prescaler registers to 0
                CpuTimer1Regs.TPRH.bit.TDDRH = u16HPRESCALER; // Set prescaler high registers to 0
                CpuTimer1Regs.TCR.bit.TRB = 1; // Reload Timer
                CpuTimer1Regs.TCR.bit.SOFT = 1;
                CpuTimer1Regs.TCR.bit.FREE = 1;
                CpuTimer1Regs.TCR.bit.TIF = 1; // Clear Interrupt Flag
                CpuTimer1Regs.TCR.bit.TIE = CPU.u16CPUTimerInterruptEnable; // Disable Timer Interrupts
                if(CPU.u16CPUTimerInterruptEnable)PieVectTable.TIMER1_INT = &cputimer1_isr;
                CpuTimer1Regs.TCR.bit.TSS = 0; // Start Timer
            break;

            case CPUTIMER2:
                CpuTimer2Regs.TCR.bit.TSS = 1; // Stop Timer
                CpuTimer2Regs.PRD.all = u64TBPRD;     // Set the Period Register
                CpuTimer2Regs.TPR.bit.TDDR= u16LPRESCALER; // Set prescaler registers to 0
                CpuTimer2Regs.TPRH.bit.TDDRH = u16HPRESCALER; // Set prescaler high registers to 0
                CpuTimer2Regs.TCR.bit.TRB = 1; // Reload Timer
                CpuTimer2Regs.TCR.bit.SOFT = 1;
                CpuTimer2Regs.TCR.bit.FREE = 1;
                CpuTimer2Regs.TCR.bit.TIF = 1; // Clear Interrupt Flag
                CpuTimer2Regs.TCR.bit.TIE = CPU.u16CPUTimerInterruptEnable; // Disable Timer Interrupts
                if(CPU.u16CPUTimerInterruptEnable)PieVectTable.TIMER2_INT = &cputimer2_isr;
                CpuTimer2Regs.TCR.bit.TSS = 0; // Start Timer
            break;


        }
    EDIS;
}

//Time0 ISR
__interrupt void cputimer0_isr(void)
{

    #ifdef DEBUG
    GPIO_vSet(DEBUG_PIN1);
    #endif

    #ifdef SOFTWARE_FILTERS
    fBuffer2 = EFFECTS_fHighPassFilterCalculate(&stHPF1, fBuffer1) ;
    fBuffer3 = EFFECTS_fPeakFilterCalculate(&stBass, fBuffer2);
    fBuffer4 = EFFECTS_fPeakFilterCalculate(&stMid, fBuffer3);
    fBuffer5 = EFFECTS_fPeakFilterCalculate(&stTreble, fBuffer4);
    fBuffer6 = EFFECTS_fOverdriveCalculate(fOverdriveGain, fBuffer5, u16DistortionSelect);
    fBuffer7 = EFFECTS_fLowPassFilterButterCalculate(&stLPF1, fBuffer6) * FLOAT_TO_UINT16_SLOPE + FLOAT_TO_UINT16_OFFSET;
    DAC_vDACBWriteFloatValue(fBuffer7);
    #else
    fBuffer2 = EFFECTS_fPeakFilterCalculate(&stBass, fBuffer1);
    fBuffer3 = EFFECTS_fPeakFilterCalculate(&stMid, fBuffer2);
    fBuffer4 = EFFECTS_fPeakFilterCalculate(&stTreble, fBuffer3);
    fBuffer5 = EFFECTS_fOverdriveCalculate(fOverdriveGain, fBuffer4, u16DistortionSelect);
    DAC_vDACBWriteFloatValue( ( fBuffer5 * FLOAT_TO_UINT16_SLOPE ) + FLOAT_TO_UINT16_OFFSET );
    #endif

    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1; // Acknowledge Interrupt Group
    CpuTimer0Regs.TCR.bit.TIF = 1; // Clear Interrupt Flag

    #ifdef DEBUG
    GPIO_vClear(DEBUG_PIN1);
    #endif
}

//Timer1 ISR
__interrupt void cputimer1_isr(void)
{

    #ifdef DEBUG
    GPIO_vSet(DEBUG_PIN2);
    #endif

    INTERFACE_vButtonActions();
    INTERFACE_vLEDActions();
    CpuTimer1Regs.TCR.bit.TIF = 1; // Clear Interrupt Flag

    #ifdef DEBUG
    GPIO_vClear(DEBUG_PIN2);
    #endif
}

//Timer2 ISR
__interrupt void cputimer2_isr(void)
{
   CpuTimer2Regs.TCR.bit.TIF = 1; // Clear Interrupt Flag
}
