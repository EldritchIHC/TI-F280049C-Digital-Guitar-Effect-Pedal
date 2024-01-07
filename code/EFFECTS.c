/*
 * EFFECTS.c
 *
 *  Created on: 29 oct. 2023
 *      Author: Paul
 */
/* Project Headers */
#include "F28x_Project.h"

/* System Headerfiles*/
#include <stdlib.h>
#include "SYSPARAM.h"
/* Own Headerfiles */
#include "EFFECTS.h"
/* Extern Headerfiles */
/* Function Prototypes */
void  EFFECTS_vHighPassFilterUpdate(tstEffectHPF *Filter, float Coeff1, float Coeff2, float Coeff3);
float EFFECTS_fHighPassFilterCalculate(tstEffectHPF *Filter, float DataIn);

float EFFECTS_fSymmetricalSoftClipCalculate(float DataIn);

float EFFECTS_fSoftClipCalculate(float DataIn);

float EFFECTS_fHardClipCalculate(float DataIn);

float EFFECTS_fOverdriveCalculate(float Gain, float DataIn, Uint16 GainSelect);

void  EFFECTS_vLowPassFilterButterUpdate(tstEffectLPF *Filter, float Coeff1, float Coeff2, float Coeff3, float Coeff4, float Coeff5);
float EFFECTS_fLowPassFilterButterCalculate(tstEffectLPF *Filter, float DataIn);

void EFFECTS_vPeakFilterInit(tstEffectPeak *Filter, float fCenterFrequency, float fBandwidth, float fGain);
void EFFECTS_vPeakFilterUpdate(tstEffectPeak *Filter, float fCenterFrequency, float fBandwidth, float fGain);
float EFFECTS_fPeakFilterCalculate(tstEffectPeak *Filter, float DataIn);

/* Global Variables */
tstEffectHPF stHPF1 = HPF1_INIT;
tstEffectLPF stLPF1 = LPF1_INIT;
tstEffectPeak stBass;
float fBassGain = FILT_GAIN_DEFAULT;
tstEffectPeak stMid;
float fMidGain = FILT_GAIN_DEFAULT;
tstEffectPeak stTreble;
float fTrebleGain = FILT_GAIN_DEFAULT;

float fBuffer1 = 0.0;
float fBuffer2 = 0.0;
float fBuffer3 = 0.0;
float fBuffer4 = 0.0;
float fBuffer5 = 0.0;
float fBuffer6 = 0.0;
float fBuffer7 = 0.0;
float fOverdriveGain = DIST_GAIN_DEFAULT;
Uint16 u16DistortionSelect = NO_DISTORTION;
/*High Pass Filter*/
void  EFFECTS_vHighPassFilterUpdate(tstEffectHPF *Filter, float Coeff1, float Coeff2, float Coeff3)
{
   Filter->fCoeff1 = Coeff1;
   Filter->fCoeff2 = Coeff2;
   Filter->fCoeff3 = Coeff3;
}

float EFFECTS_fHighPassFilterCalculate(tstEffectHPF *Filter, float DataIn)
{
    volatile float DataOut;
    DataOut = (Filter->fCoeff1 * DataIn) - (Filter->fCoeff2 * Filter->fDataInOld) + (Filter->fCoeff3 * Filter->fDataOutOld);
    Filter->fDataInOld = DataIn;
    Filter->fDataOutOld = DataOut;
    return DataOut;
}
/*Distortion*/


float EFFECTS_fSymmetricalSoftClipCalculate(float DataIn)
{
    volatile float DataOut = 0.0 ;
    volatile float AbsoluteDataIn = __builtin_fabs(DataIn);
    volatile float SignDataIn = ( DataIn >= 0.0) ? 1.0 : -1.0;
    if( AbsoluteDataIn <= SYM_CLIP_THRESHOLD1){//0,333
        DataOut = 2.0 * DataIn;
    }else if( ( AbsoluteDataIn > SYM_CLIP_THRESHOLD1 ) && ( AbsoluteDataIn < SYM_CLIP_THRESHOLD2 ) ){//0.333 0.667
        DataOut = SignDataIn * ( 3.0 - ( ( 2.0  - ( 3.0 * AbsoluteDataIn ) ) * ( 2.0  - ( 3.0 * AbsoluteDataIn ) ) ) ) / 3.0;
    }else
    {
        DataOut = SignDataIn * SYM_CLIP_SAT;
    }
    return DataOut;
}

float EFFECTS_fSoftClipCalculate(float DataIn)
{
    volatile float DataOut;
    if( DataIn  <= -SOFT_CLIP_THRESHOLD )
        DataOut = -SOFT_CLIP_SAT;//0.667
    else if( ( DataIn >= -SOFT_CLIP_THRESHOLD ) && ( DataIn <= SOFT_CLIP_THRESHOLD ) )
        DataOut = DataIn - ( ( DataIn * DataIn * DataIn ) / 3 );
    else if( DataIn >= SOFT_CLIP_THRESHOLD)
        DataOut = SOFT_CLIP_SAT;
    return DataOut;
}

float EFFECTS_fHardClipCalculate(float DataIn)
{
    volatile float DataOut;
    if( DataIn  <= -HARD_CLIP_THRESHOLD )
        DataOut = -HARD_CLIP_SAT;
    else if( ( DataIn > -HARD_CLIP_THRESHOLD ) && ( DataIn < HARD_CLIP_THRESHOLD ) )
        DataOut = DataIn;
    else if( DataIn >= HARD_CLIP_THRESHOLD)
        DataOut = HARD_CLIP_SAT;
    return DataOut;
}

float EFFECTS_fOverdriveCalculate(float Gain, float DataIn, Uint16 GainSelect)
{
   switch( GainSelect )
   {
       case NO_DISTORTION: return ( Gain * DataIn ) ;
       case SOFT_CLIP: return EFFECTS_fSoftClipCalculate( Gain * DataIn );
       case SYM_SOFT_CLIP: return EFFECTS_fSymmetricalSoftClipCalculate( Gain * DataIn );
       case HARD_CLIP: return EFFECTS_fHardClipCalculate( Gain * DataIn );
       default: return ( Gain * DataIn ) ;
   }
}
/*Low Pass Filter*/
void  EFFECTS_vLowPassFilterButterUpdate(tstEffectLPF *Filter, float Coeff1, float Coeff2, float Coeff3, float Coeff4, float Coeff5)
{
    Filter->fCoeff1 = Coeff1;
    Filter->fCoeff2 = Coeff2;
    Filter->fCoeff3 = Coeff3;
    Filter->fCoeff4 = Coeff4;
    Filter->fCoeff5 = Coeff5;
}

float EFFECTS_fLowPassFilterButterCalculate(tstEffectLPF *Filter, float DataIn)
{
    volatile float DataOut;
    DataOut = (Filter->fCoeff1 * DataIn) + (Filter->fCoeff2 * Filter->fDataInOld1) + (Filter->fCoeff3 * Filter->fDataInOld2) - (Filter->fCoeff4 * Filter->fDataOutOld1) - (Filter->fCoeff5 * Filter->fDataOutOld2);
    Filter->fDataInOld2 = Filter->fDataInOld1;
    Filter->fDataOutOld2 = Filter->fDataOutOld1;
    Filter->fDataInOld1 = DataIn;
    Filter->fDataOutOld1 = DataOut;
    return DataOut;
}
/*Peak Filter*/
void EFFECTS_vPeakFilterUpdate(tstEffectPeak *Filter, float fCenterFrequency, float fBandwidth, float fGain)
{
   Filter->fGain = fGain;
   Filter->fBandwidth = fBandwidth;
   Filter->fCenterFrequency = fCenterFrequency;
   volatile float Q = fCenterFrequency / fBandwidth;
   volatile float OmegaCenter = 2 * 3.14 * fCenterFrequency;
   Filter->fCoeff1 = 4.0 + ( 2 * fGain / Q * OmegaCenter * SAMPLE_PERIOD ) + ( OmegaCenter * SAMPLE_PERIOD * OmegaCenter * SAMPLE_PERIOD );//a0
   Filter->fCoeff2 = ( 2 * ( OmegaCenter * SAMPLE_PERIOD * OmegaCenter * SAMPLE_PERIOD ) ) - 8.0;//a1
   Filter->fCoeff3 = 4.0 - ( 2 * fGain / Q * OmegaCenter * SAMPLE_PERIOD ) + ( OmegaCenter * SAMPLE_PERIOD * OmegaCenter * SAMPLE_PERIOD );//a2
   Filter->fCoeff4 = 4.0 + ( 2 / Q * OmegaCenter * SAMPLE_PERIOD ) + ( OmegaCenter * SAMPLE_PERIOD * OmegaCenter * SAMPLE_PERIOD );//b0
   Filter->fCoeff5 = - ( 2 * ( OmegaCenter * SAMPLE_PERIOD * OmegaCenter * SAMPLE_PERIOD ) - 8.0 );//b1
   Filter->fCoeff6 = - ( 4.0 - ( 2 / Q * OmegaCenter * SAMPLE_PERIOD ) + ( OmegaCenter * SAMPLE_PERIOD * OmegaCenter * SAMPLE_PERIOD ) );//b2
}
void EFFECTS_vPeakFilterInit(tstEffectPeak *Filter, float fCenterFrequency, float fBandwidth, float fGain)
{
    Filter->fDataInOld1 = 0.0;
    Filter->fDataInOld2 = 0.0;
    Filter->fDataOutOld1 = 0.0;
    Filter->fDataOutOld2 = 0.0;
    EFFECTS_vPeakFilterUpdate(Filter, fCenterFrequency, fBandwidth, fGain);
}

float EFFECTS_fPeakFilterCalculate(tstEffectPeak *Filter, float DataIn)
{
    volatile float DataOut;
    if(Filter->fGain == 1.0)
        return DataIn;
    else
        {
            DataOut = ( ( Filter->fCoeff1 * DataIn ) + ( Filter->fCoeff2 * Filter->fDataInOld1 ) + ( Filter->fCoeff3 * Filter->fDataInOld2 ) + ( Filter->fCoeff5 * Filter->fDataOutOld1 ) + ( Filter->fCoeff6 * Filter->fDataOutOld2 ) ) /Filter->fCoeff4;
            Filter->fDataInOld2 = Filter->fDataInOld1;
            Filter->fDataOutOld2 = Filter->fDataOutOld1;
            Filter->fDataInOld1 = DataIn;
            Filter->fDataOutOld1 = DataOut;
            return DataOut;
        }
}
