/*
 * INTERFACE.c
 *
 *  Created on: 19 nov. 2023
 *      Author: Paul
 */

/* Project Headers */
#include "F28x_Project.h"

/* System Headerfiles*/
#include <stdlib.h>
#include "SYSPARAM.h"
/* Own Headerfiles */
#include "INTERFACE.h"
#include "GPIO.h"
#include "EFFECTS.h"

/* Extern Headerfiles */

/* Function Prototypes */
void INTERFACE_vConfig();
void INTERFACE_vNavigateLeft();
void INTERFACE_vNavigateRight();
void INTERFACE_vSelect();
void INTERFACE_vBack();
void INTERFACE_vIncrease();
void INTERFACE_vDecease();
void INTERFACE_vButtonActions();
void INTERFACE_vLEDActions();
/* Global Variables */
const Uint32  u32MaskButtonLeft = 1UL << ( BUTTON_LEFT % 32 );
const Uint32 u32MaskButtonRight = 1UL << ( BUTTON_RIGHT % 32 );
const Uint32 u32MaskButtonSelect = 1UL << ( BUTTON_SELECT % 32 );
const Uint32 u32MaskButtonBack = 1UL << ( BUTTON_BACK % 32 );
const Uint32 u32MaskButtonIncrease = 1UL << ( BUTTON_INCREASE % 32 );
const Uint32 u32MaskButtonDecrease = 1UL << ( BUTTON_DECREAE % 32 );
const Uint32 u32MaskButtons = 1UL << ( BUTTON_LEFT % 32 ) | 1UL << ( BUTTON_RIGHT % 32 ) | 1UL << ( BUTTON_SELECT % 32 ) | 1UL << ( BUTTON_BACK % 32 ) | 1UL << ( BUTTON_INCREASE % 32 ) | 1UL << ( BUTTON_DECREAE % 32 );
Uint16 u16InputPinList[] = {BUTTON_LEFT, BUTTON_RIGHT, BUTTON_SELECT, BUTTON_BACK, BUTTON_INCREASE, BUTTON_DECREAE };
Uint16 u16OutputPinList[] = {BASS, MID, TREBLE, NO_DIST, SOFT_DIST, SYM_SOFT_DIST, HARD_DIST, LED_RED };
Uint16 u16LEDPanel = NO_DIST;
Uint16 u16ButtonPanel = LED_NO_DIST;
Uint16 u16UpdateBass = 0;
Uint16 u16UpdateMid = 0;
Uint16 u16UpdateTreble = 0;

void INTERFACE_vConfig()
{
    GPIO_vConfigInput(u16InputPinList, INPUTS);
    GPIO_vConfigOutput(u16OutputPinList, OUTPUTS);
}

void INTERFACE_vNavigateLeft()
{
    if((u16ButtonPanel > LED_BASS) && (u16ButtonPanel <= LED_HARD_DIST))
        u16ButtonPanel = u16ButtonPanel >> 1UL;
    else
        u16ButtonPanel = LED_BASS;
}

void INTERFACE_vNavigateRight()
{
    if((u16ButtonPanel >= LED_BASS) && (u16ButtonPanel < LED_HARD_DIST))
        u16ButtonPanel = u16ButtonPanel << 1UL;
    else
        u16ButtonPanel = LED_HARD_DIST;
}

void INTERFACE_vSelect()
{
    if(u16ButtonPanel == LED_NO_DIST)
        u16DistortionSelect = NO_DISTORTION;
    else if(u16ButtonPanel == LED_SOFT_DIST)
        u16DistortionSelect = SOFT_CLIP;
    else if(u16ButtonPanel == LED_SYM_SOFT_DIST)
        u16DistortionSelect = SYM_SOFT_CLIP;
    else if(u16ButtonPanel == LED_HARD_DIST)
        u16DistortionSelect = HARD_CLIP;
}

void INTERFACE_vBack()
{

}

void INTERFACE_vIncrease()
{
    if( (u16ButtonPanel == LED_NO_DIST) || (u16ButtonPanel == LED_SOFT_DIST) || (u16ButtonPanel == LED_SYM_SOFT_DIST) || (u16ButtonPanel == LED_HARD_DIST))
    {
        if(fOverdriveGain < DIST_GAIN_MAX)
            fOverdriveGain = fOverdriveGain + DIST_GAIN_STEP;
        else fOverdriveGain = DIST_GAIN_MAX;
    }
    else if(u16ButtonPanel == LED_BASS)
    {
        if( ( fBassGain < FILT_GAIN_MAX ) && ( fBassGain >= FILT_GAIN_DEFAULT ) )
        {
            fBassGain = fBassGain + FILT_GAIN_STEP_BOOST;
        }
        else if( ( fBassGain < FILT_GAIN_DEFAULT ) && ( fBassGain >= FILT_GAIN_MIN )  )
        {
            fBassGain = fBassGain + FILT_GAIN_STEP_CUT;

        }
        else fBassGain = FILT_GAIN_MAX;
        u16UpdateBass = 1;
    }
    else if(u16ButtonPanel == LED_MID)
    {
        if( (fMidGain < FILT_GAIN_MAX) && (fMidGain >= FILT_GAIN_DEFAULT) )
        {
            fMidGain = fMidGain + FILT_GAIN_STEP_BOOST;
        }
        else if( ( fMidGain < FILT_GAIN_DEFAULT ) && ( fMidGain >= FILT_GAIN_MIN )  )
        {
            fMidGain = fMidGain + FILT_GAIN_STEP_CUT;

        }
        else fMidGain = FILT_GAIN_MAX;
        u16UpdateMid = 1;
    }
    else if(u16ButtonPanel == LED_TREBLE)
    {
        if( (fTrebleGain < FILT_GAIN_MAX) && (fTrebleGain >= FILT_GAIN_DEFAULT) )
        {
            fTrebleGain = fTrebleGain + FILT_GAIN_STEP_BOOST;
        }
        else if( ( fTrebleGain < FILT_GAIN_DEFAULT ) && ( fTrebleGain >= FILT_GAIN_MIN )  )
        {
            fTrebleGain = fTrebleGain + FILT_GAIN_STEP_CUT;
        }
        else fTrebleGain = FILT_GAIN_MAX;
        u16UpdateTreble = 1;
    }
}

void INTERFACE_vDecrease()
{
    if( (u16ButtonPanel == LED_NO_DIST) || (u16ButtonPanel == LED_SOFT_DIST) || (u16ButtonPanel == LED_SYM_SOFT_DIST) || (u16ButtonPanel == LED_HARD_DIST))
    {
        if(fOverdriveGain > DIST_GAIN_MIN)
            fOverdriveGain = fOverdriveGain - DIST_GAIN_STEP;
        else fOverdriveGain = DIST_GAIN_MIN;
    }
    else if(u16ButtonPanel == LED_BASS)
    {
        if( (fBassGain <= FILT_GAIN_MAX) && (fBassGain > FILT_GAIN_DEFAULT) )
        {
            fBassGain = fBassGain - FILT_GAIN_STEP_BOOST;
        }
        else if( (fBassGain > FILT_GAIN_MIN) && (fBassGain <= FILT_GAIN_DEFAULT) )
        {
            fBassGain = fBassGain - FILT_GAIN_STEP_CUT;
        }
        else  fBassGain = FILT_GAIN_MIN;
        u16UpdateBass = 1;
    }
    else if(u16ButtonPanel == LED_MID)
    {
        if( (fMidGain <= FILT_GAIN_MAX) && (fMidGain > FILT_GAIN_DEFAULT) )
        {
            fMidGain = fMidGain - FILT_GAIN_STEP_BOOST;
        }
        else if( (fMidGain > FILT_GAIN_MIN) && (fMidGain <= FILT_GAIN_DEFAULT) )
        {
            fMidGain = fMidGain - FILT_GAIN_STEP_CUT;
        }
        else fMidGain = FILT_GAIN_MIN;
        u16UpdateMid = 1;
    }
    else if(u16ButtonPanel == LED_TREBLE)
    {
        if( (fTrebleGain <= FILT_GAIN_MAX) && (fTrebleGain > FILT_GAIN_DEFAULT) )
        {
            fTrebleGain = fTrebleGain - FILT_GAIN_STEP_BOOST;
        }
        else if( (fTrebleGain > FILT_GAIN_MIN) && (fTrebleGain <= FILT_GAIN_DEFAULT) )
        {
            fTrebleGain = fTrebleGain - FILT_GAIN_STEP_CUT;
        }
        else fTrebleGain = FILT_GAIN_MIN;
        u16UpdateTreble = 1;
    }
}

void INTERFACE_vButtonActions()
{
    volatile Uint32 u32Port = GPIO_u32ReadPort(PORTB) & u32MaskButtons;
    if(u32Port != 0)
    {

        if(u32Port & u32MaskButtonLeft)
        {
            GPIO_vClear(LED_RED);
            INTERFACE_vNavigateLeft();
            GPIO_vSet(LED_RED);
        }
        else if( (u32Port & u32MaskButtonRight) == u32MaskButtonRight)
        {
            GPIO_vClear(LED_RED);
            INTERFACE_vNavigateRight();
            GPIO_vSet(LED_RED);
        }
        else if( (u32Port & u32MaskButtonSelect) == u32MaskButtonSelect)
        {
            GPIO_vClear(LED_RED);
            INTERFACE_vSelect();
            GPIO_vSet(LED_RED);
        }
        else if( (u32Port & u32MaskButtonBack) == u32MaskButtonBack)
        {
            GPIO_vClear(LED_RED);
            INTERFACE_vBack();
            GPIO_vSet(LED_RED);
        }
        else if( (u32Port & u32MaskButtonIncrease) == u32MaskButtonIncrease)
        {
            GPIO_vClear(LED_RED);
            INTERFACE_vIncrease();
            GPIO_vSet(LED_RED);
        }
        else if( (u32Port & u32MaskButtonDecrease) == u32MaskButtonDecrease)
        {
            GPIO_vClear(LED_RED);
            INTERFACE_vDecrease();
            GPIO_vSet(LED_RED);
        }
    }
}

void INTERFACE_vLEDActions()
{
    GPIO_vClear(u16LEDPanel);
    switch(u16ButtonPanel)
    {
        case LED_BASS:
            u16LEDPanel = BASS;
        break;

        case LED_MID:
            u16LEDPanel = MID;
        break;

        case LED_TREBLE:
            u16LEDPanel = TREBLE;
        break;

        case LED_NO_DIST:
            u16LEDPanel = NO_DIST;
        break;

        case LED_SOFT_DIST:
            u16LEDPanel = SOFT_DIST;
        break;

        case LED_SYM_SOFT_DIST:
            u16LEDPanel = SYM_SOFT_DIST;
        break;

        case LED_HARD_DIST:
            u16LEDPanel = HARD_DIST;
        break;
    }
    GPIO_vSet(u16LEDPanel);
}
