/*
 * GPIO.h
 *
 *  Created on: 14 oct. 2023
 *      Author: Paul
 */

#ifndef GPIO_H_
#define GPIO_H_

/*Defines*/
//#define LED1 23
//#define LED2 34

#define OUTPUT 1
#define INPUT 0

#define QUAL_SYNC 0
#define QUAL_3SPL 1
#define QUAL_6SPL 2
#define QUAL_ASYNC 3

#define PORTA 0
#define PORTB 1
/*Structures*/
/*Function prototypes*/
void GPIO_vConfigInput(const Uint16 u16PinList[], Uint16 u16Pins);
void GPIO_vConfigOutput(const Uint16 u16PinList[], Uint16 u16Pins);
void GPIO_vToggleMultiple(const Uint16 u16PinList[], Uint16 u16Pins);
void GPIO_vSetMultiple(const Uint16 u16PinList[], Uint16 u16Pins);
void GPIO_vClearMultiple(const Uint16 u16PinList[], Uint16 u16Pins);
void GPIO_vToggle(const Uint16 u16Pin);
void GPIO_vSet(const Uint16 u16Pin);
void GPIO_vClear(const Uint16 u16Pin);
void GPIO_vSetPort(const Uint16 u16FirstBit, Uint32 u32Data);
void GPIO_vClearPort(const Uint16 u16FirstBit, Uint32 u32Data);
Uint16 GPIO_u16ReadPin(const Uint16 u16Pin);
Uint16 GPIO_u16ReadPins(const Uint16 u16PinList[], Uint16 u16Pins);
Uint32 GPIO_u32ReadPort(Uint16 u16Port);
void GPIO_vI2CPins();
/*Variables*/
/*
extern const Uint16 u16PinList1[32];
extern const Uint16 u16PinList2[32];
extern const Uint16 u16PinList3[32];
extern const Uint16 u16PinList4[32];
extern const Uint16 u16PinList5[32];
extern const Uint16 u16PinList6[32];
*/


#endif /* GPIO_H_ */
