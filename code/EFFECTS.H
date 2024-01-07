/*
 * EFFECTS.H
 *
 *  Created on: 29 oct. 2023
 *      Author: Paul
 */

#ifndef EFFECTS_H_
#define EFFECTS_H_

/*Defines*/
#define SOFT_CLIP_THRESHOLD 0.1
#define SOFT_CLIP_SAT 0.09

#define HARD_CLIP_THRESHOLD 0.09
#define HARD_CLIP_SAT 0.1

#define SYM_CLIP_THRESHOLD1 0.1
#define SYM_CLIP_THRESHOLD2 0.2
#define SYM_CLIP_SAT 0.25

#define SAMPLE_PERIOD 1.0417e-5

#define BASS_CENTER_FREQUENCY 77.5
#define BASS_BANDWIDTH 280.0
#define MID_CENTER_FREQUENCY 1095.0
#define MID_BANDWIDTH 3700.0
#define TREBLE_CENTER_FREQUENCY 7746.0
#define TREBLE_BANDWIDTH 11000.0

#define NO_DISTORTION 0
#define SOFT_CLIP 1
#define SYM_SOFT_CLIP 2
#define HARD_CLIP 3

#define DIST_GAIN_DEFAULT 1.0
#define DIST_GAIN_MAX 16.0
#define DIST_GAIN_MIN 1.0
#define DIST_GAIN_STEP 1.0

#define FILT_GAIN_DEFAULT 1.0
#define FILT_GAIN_MAX 16.0
#define FILT_GAIN_MIN 0.1
#define FILT_GAIN_STEP_BOOST 2.5
#define FILT_GAIN_STEP_CUT 0.3

/*Structures*/
typedef struct{
    float fDataInOld;
    float fDataOutOld;
    float fCoeff1;
    float fCoeff2;
    float fCoeff3;
}tstEffectHPF;

typedef struct{
    float fDataInOld1;
    float fDataInOld2;
    float fDataOutOld1;
    float fDataOutOld2;
    float fCoeff1;
    float fCoeff2;
    float fCoeff3;
    float fCoeff4;
    float fCoeff5;
}tstEffectLPF;

typedef struct{
    float fDataInOld1;
    float fDataInOld2;
    float fDataOutOld1;
    float fDataOutOld2;
    float fCoeff1;//a0
    float fCoeff2;//a1
    float fCoeff3;//a2
    float fCoeff4;//b0
    float fCoeff5;//b1
    float fCoeff6;//b2
    float fGain;
    float fBandwidth;
    float fCenterFrequency;
}tstEffectPeak;

/*Structure Initializations*/

#define HPF1_INIT {  0, 0, 0.9993 ,0.9993, 0.9987 }//fs = 96k

#define LPF1_INIT { 0, 0, 0, 0, 0.2262, 0.4523, 0.2262, 0.2809, 0.1956 }//fs = 96k



/*Function prototypes*/
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

/*Variables*/
extern tstEffectHPF stHPF1;
extern tstEffectHPF stHPF2;

extern float fOverdriveGain;
extern Uint16 u16DistortionSelect;

extern tstEffectLPF stLPF1;
extern tstEffectLPF stLPF2;

extern tstEffectPeak stBass;
extern float fBassGain;
extern tstEffectPeak stMid;
extern float fMidGain;
extern tstEffectPeak stTreble;
extern float fTrebleGain;

extern float fBuffer1;
extern float fBuffer2;
extern float fBuffer3;
extern float fBuffer4;
extern float fBuffer5;
extern float fBuffer6;
extern float fBuffer7;
#endif /* EFFECTS_H_ */
