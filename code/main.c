
#include "F28x_Project.h"
#include "SYSPARAM.h"
#include "GPIO.h"
#include "TIMER.h"
#include "ADC_B.h"
#include "DAC.h"
#include "EFFECTS.h"
#include "INTERFACE.h"
void main(void)

{

    InitSysCtrl();
    //
    // Initialize GPIO:
    // This example function is found in the F2837xS_Gpio.c file and
    // illustrates how to set the GPIO to it's default state.
    //
    InitGpio();

    //
    // Clear all interrupts and initialize PIE vector table:
    // Disable CPU interrupts
    //
    DINT;

    //
    // Initialize the PIE control registers to their default state.
    // The default state is all PIE interrupts disabled and flags
    // are cleared.
    // This function is found in the F2837xS_PieCtrl.c file.
    //
    InitPieCtrl();

    //
    // Disable CPU interrupts and clear all CPU interrupt flags:
    //
    IER = 0x0000;
    IFR = 0x0000;

    //
    // Initialize the PIE vector table with pointers to the shell Interrupt
    // Service Routines (ISR).
    // This will populate the entire table, even if the interrupt
    // is not used in this example.  This is useful for debug purposes.
    // The shell ISR routines are found in F2837xS_DefaultIsr.c.
    // This function is found in F2837xS_PieVect.c.
    //
    InitPieVectTable();

    EALLOW;

    EFFECTS_vPeakFilterInit(&stBass, BASS_CENTER_FREQUENCY, BASS_BANDWIDTH, fBassGain);

    EFFECTS_vPeakFilterInit(&stMid, MID_CENTER_FREQUENCY, MID_BANDWIDTH, fMidGain);

    EFFECTS_vPeakFilterInit(&stTreble, TREBLE_CENTER_FREQUENCY, TREBLE_BANDWIDTH, fTrebleGain);

    ADC_B_vADCB_Group_Init();

    TIMER_vCPUTimerHertzInit(stCPU4);//ADCB SOC Interrupt

    TIMER_vCPUTimerSecondsInit(stCPU1);//Interface Interrupt

    ADC_B_vADCB_SOC0Config(stADCB1);

    DAC_vDACBInit(stDACData1);

    #ifdef DEBUG
    const Uint16 u16DebugPinList[] = {DEBUG_PIN1, DEBUG_PIN2, DEBUG_PIN3};
    GPIO_vConfigOutput(u16DebugPinList, DEBUG_PINS);
    #endif

    INTERFACE_vConfig();

    IER |= M_INT1; //Enable group 1 interrupts, TIMER0 Interrupt
    IER |= M_INT13;// TIMER1 Interrupt
    EINT;
    ERTM;
    EDIS;
    for(;;)
    {
        if(u16UpdateBass)
            {
                EFFECTS_vPeakFilterUpdate(&stBass, BASS_CENTER_FREQUENCY, BASS_BANDWIDTH, fBassGain);
                u16UpdateBass = 0;
            }

        if(u16UpdateMid)
            {
                EFFECTS_vPeakFilterUpdate(&stMid, MID_CENTER_FREQUENCY, MID_BANDWIDTH, fMidGain);
                u16UpdateMid = 0;
            }

        if(u16UpdateTreble)
            {
                EFFECTS_vPeakFilterUpdate(&stTreble, TREBLE_CENTER_FREQUENCY, BASS_BANDWIDTH, fTrebleGain);
                u16UpdateTreble = 0;
            }
    }
}


