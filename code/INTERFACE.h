/*
 * INTERFACE.h
 *
 *  Created on: 19 nov. 2023
 *      Author: Paul
 */
/*
Interface Map

LED1 - GPIO11
LED2 - GPIO12
LED3 - GPIO13
LED4 - GPIO14
LED5 - GPIO15
LED6 - GPIO16
LED7 - GPIO17

BT1 - GPIO32
BT2 - GPIO33
BT3 - GPIO39
BT4 - GPIO40
BT5 - GPIO56
BT6 - GPIO57

 */
#ifndef INTERFACE_H_
#define INTERFACE_H_

/*Defines*/

#define LED_RED 23
#define LED_GREEN 34

#define INPUTS 6
#define BUTTON_LEFT 32
#define BUTTON_RIGHT 33
#define BUTTON_SELECT 39
#define BUTTON_BACK 40
#define BUTTON_INCREASE 56
#define BUTTON_DECREAE 57

#define OUTPUTS 8
#define BASS 11
#define MID 12
#define TREBLE 13
#define NO_DIST 14
#define SOFT_DIST 15
#define SYM_SOFT_DIST 16
#define HARD_DIST 17

#define LED_BASS 0x0001
#define LED_MID 0x0002
#define LED_TREBLE 0x0004
#define LED_NO_DIST 0x0008
#define LED_SOFT_DIST 0x0010
#define LED_SYM_SOFT_DIST 0x0020
#define LED_HARD_DIST 0x0040

/*Structures*/
/*Function prototypes*/
void INTERFACE_vConfig();
void INTERFACE_vNavigateLeft();
void INTERFACE_vNavigateRight();
void INTERFACE_vSelect();
void INTERFACE_vBack();
void INTERFACE_vIncrease();
void INTERFACE_vDecease();
void INTERFACE_vButtonActions();
void INTERFACE_vLEDActions();
/*Variables*/
extern const Uint32 u32MaskButtonLeft;
extern const Uint32 u32MaskButtonRight;
extern const Uint32 u32MaskButtonSelect;
extern const Uint32 u32MaskButtonBack;
extern const Uint32 u32MaskButtonIncrease;
extern const Uint32 u32MaskButtonDecrease;
const extern Uint32 u32MaskButtons;
extern Uint16 u16InputPinList[];
extern Uint16 u16OutputPinList[];
extern Uint16 u16ButtonPanel;
extern Uint16 u16LEDPanel;
extern Uint16 u16UpdateBass;
extern Uint16 u16UpdateMid;
extern Uint16 u16UpdateTreble;



#endif /* INTERFACE_H_ */
