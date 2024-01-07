/*
 * GPIO.c
 *
 *  Created on: 14 oct. 2023
 *      Author: Paul
 */

/* Project Headers */
#include "F28x_Project.h"

/* System Headers*/
#include <stdlib.h>
#include "SYSPARAM.h"
/* Own Headers */
#include "GPIO.h"

/* External Headers */

/* Function Prototypes */
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
void  GPIO_vI2CPins();
/* Global Variables */

void GPIO_vI2CPins()
{
    EALLOW;
    /*Set SCL and SDA as outputs*/
    GpioCtrlRegs.GPBDIR.bit.GPIO37 = OUTPUT;//SCL
    GpioCtrlRegs.GPBDIR.bit.GPIO35 = OUTPUT;//SDA
    GpioCtrlRegs.GPBQSEL1.bit.GPIO37 = QUAL_ASYNC;
    GpioCtrlRegs.GPBQSEL1.bit.GPIO35 = QUAL_ASYNC;
    /*Enable Pull-ups*/
    GpioCtrlRegs.GPBPUD.bit.GPIO37 = 0;
    GpioCtrlRegs.GPBPUD.bit.GPIO35 = 0;
    /*MUX to SDA and SCL*/
    /*Higher 2 bits*/
    GpioCtrlRegs.GPBGMUX1.bit.GPIO35 = 0;
    GpioCtrlRegs.GPBMUX1.bit.GPIO35 = 0;
    /*Lower 2 bits*/
    GpioCtrlRegs.GPBGMUX1.bit.GPIO37 = 3;
    GpioCtrlRegs.GPBMUX1.bit.GPIO37 = 3;
    EDIS;
}

void GPIO_vConfigInput(const Uint16 u16PinList[], Uint16 u16Pins)
{
    EALLOW;

    volatile Uint32 u32MaskA = 0;
    volatile Uint32 u32MaskB = 0;
    volatile Uint16 i = 0;
    for ( i = 0; i < u16Pins; i++)
    {
        if( u16PinList[i] < 32 )
        {
            u32MaskA = u32MaskA | ( 1UL << u16PinList[i] );
        }
        else
            u32MaskB = u32MaskB | ( 1UL << ( u16PinList[i] % 32 ) );
    }
    //Disable pull-up
    GpioCtrlRegs.GPAPUD.all |= u32MaskA;// 0 PU en, 1 PU dis
    GpioCtrlRegs.GPBPUD.all |= u32MaskB;

    //GPIO Direction Input
    GpioCtrlRegs.GPADIR.all &= ~( u32MaskA );
    GpioCtrlRegs.GPBDIR.all &= ~( u32MaskB );


    EDIS;
}

void GPIO_vConfigOutput(const Uint16 u16PinList[], Uint16 u16Pins)
{
    EALLOW;

    volatile Uint32 u32MaskA = 0;
    volatile Uint32 u32MaskB = 0;
    volatile Uint16 i = 0;
    for ( i = 0; i < u16Pins; i++)
    {
        if( u16PinList[i] < 32 )
        {
            u32MaskA = u32MaskA | ( 1UL << u16PinList[i] );
        }
        else
            u32MaskB = u32MaskB | ( 1UL << ( u16PinList[i] % 32 ) );
    }
    //GPIO Direction Output
    GpioCtrlRegs.GPADIR.all |=  u32MaskA;
    GpioCtrlRegs.GPBDIR.all |=  u32MaskB;

    EDIS;
}

void GPIO_vToggleMultiple(const Uint16 u16PinList[], Uint16 u16Pins)
{
    EALLOW;

    volatile Uint32 u32MaskA = 0;
    volatile Uint32 u32MaskB = 0;
    volatile Uint16 i = 0;
    for ( i = 0; i < u16Pins; i++)
    {
        if( u16PinList[i] < 32 )
        {
            u32MaskA = u32MaskA | ( 1UL << u16PinList[i] );
        }
        else
            u32MaskB = u32MaskB | ( 1UL << ( u16PinList[i] % 32 ) );
    }
    //GPIO Toggle Output
    GpioDataRegs.GPATOGGLE.all |=  u32MaskA;
    GpioDataRegs.GPBTOGGLE.all |=  u32MaskB;

    EDIS;
}

void GPIO_vSetMultiple(const Uint16 u16PinList[], Uint16 u16Pins)
{
    EALLOW;

    volatile Uint32 u32MaskA = 0;
    volatile Uint32 u32MaskB = 0;
    volatile Uint16 i = 0;
    for ( i = 0; i < u16Pins; i++)
    {
        if( u16PinList[i] < 32 )
        {
            u32MaskA = u32MaskA | ( 1UL << u16PinList[i] );
        }
        else
            u32MaskB = u32MaskB | ( 1UL << ( u16PinList[i] % 32 ) );
    }
    //GPIO Toggle Output
    GpioDataRegs.GPASET.all |=  u32MaskA;
    GpioDataRegs.GPBSET.all |=  u32MaskB;

    EDIS;
}

void GPIO_vClearMultiple(const Uint16 u16PinList[], Uint16 u16Pins)
{
    EALLOW;

    volatile Uint32 u32MaskA = 0;
    volatile Uint32 u32MaskB = 0;
    volatile Uint16 i = 0;
    for ( i = 0; i < u16Pins; i++)
    {
        if( u16PinList[i] < 32 )
        {
            u32MaskA = u32MaskA | ( 1UL << u16PinList[i] );
        }
        else
            u32MaskB = u32MaskB | ( 1UL << ( u16PinList[i] % 32 ) );
    }
    //GPIO Toggle Output
    GpioDataRegs.GPACLEAR.all |=  u32MaskA;
    GpioDataRegs.GPBCLEAR.all |=  u32MaskB;

    EDIS;
}

void GPIO_vToggle(const Uint16 u16Pin)
{
    EALLOW;

    volatile Uint32 u32MaskA = 0;
    volatile Uint32 u32MaskB = 0;
    if( u16Pin < 32 )
    {
        u32MaskA = u32MaskA | ( 1UL << u16Pin );
    }
    else
        u32MaskB = u32MaskB | ( 1UL << ( u16Pin% 32 ) );
    //GPIO Toggle Output
    GpioDataRegs.GPATOGGLE.all |=  u32MaskA;
    GpioDataRegs.GPBTOGGLE.all |=  u32MaskB;

    EDIS;
}

void GPIO_vSet(const Uint16 u16Pin)
{
    EALLOW;

    volatile Uint32 u32MaskA = 0;
    volatile Uint32 u32MaskB = 0;
    if( u16Pin < 32 )
    {
        u32MaskA = u32MaskA | ( 1UL << u16Pin );
    }
    else
        u32MaskB = u32MaskB | ( 1UL << ( u16Pin % 32 ) );
    //GPIO Toggle Output
    GpioDataRegs.GPASET.all |=  u32MaskA;
    GpioDataRegs.GPBSET.all |=  u32MaskB;

    EDIS;
}

void GPIO_vClear(const Uint16 u16Pin)
{
    EALLOW;

    volatile Uint32 u32MaskA = 0;
    volatile Uint32 u32MaskB = 0;
    if( u16Pin < 32 )
    {
        u32MaskA = u32MaskA | ( 1UL << u16Pin );
    }
    else
        u32MaskB = u32MaskB | ( 1UL << ( u16Pin % 32 ) );
    //GPIO Toggle Output
    GpioDataRegs.GPACLEAR.all |=  u32MaskA;
    GpioDataRegs.GPBCLEAR.all |=  u32MaskB;

    EDIS;
}
void GPIO_vSetPort(const Uint16 u16FirstBit, Uint32 u32Data)
{
    EALLOW;
    volatile Uint32 u32Mask = u32Data;
    if(u16FirstBit != 0)u32Mask = u32Data << ( u16FirstBit % 32 );
    if(u16FirstBit < 32)
    {
        GpioDataRegs.GPASET.all |= u32Mask;
    }
    else
        GpioDataRegs.GPBSET.all |= u32Mask;

    EDIS;
}
void GPIO_vClearPort(const Uint16 u16FirstBit, Uint32 u32Data)
{
    EALLOW;
    volatile Uint32 u32Mask = u32Data;
    if(u16FirstBit != 0)u32Mask = u32Data << ( u16FirstBit % 32 );
    if(u16FirstBit < 32)
    {
        GpioDataRegs.GPACLEAR.all |= u32Mask;
    }
    else
        GpioDataRegs.GPBCLEAR.all |= u32Mask;

    EDIS;
}

Uint16 GPIO_u16ReadPin(const Uint16 u16Pin)
{
    volatile Uint32 u32MaskA = 0;
    volatile Uint32 u32MaskB = 0;
    if( u16Pin < 32 )
    {
        u32MaskA = u32MaskA | ( 1UL << u16Pin );
        if(u32MaskA == ( u32MaskA & GpioDataRegs.GPADAT.all ) )
            return 1;
        else
            return 0;
    }
    else
    {
        u32MaskB = u32MaskB | ( 1UL << ( u16Pin % 32 ) );
        if(u32MaskB == ( u32MaskB & GpioDataRegs.GPBDAT.all ) )
            return 1;
        else
            return 0;
    }

}

Uint16 GPIO_u16ReadPins(const Uint16 u16PinList[], Uint16 u16Pins)
{
    volatile Uint16 i = 0;
    volatile Uint16 u16ReadPins = 0;
    for ( i = 0; i < u16Pins; i++)
    {
        if( GPIO_u16ReadPin(u16PinList[i]) ) u16ReadPins = u16ReadPins | ( 1U << i );
    }
    return u16ReadPins;
}

Uint32 GPIO_u32ReadPort(Uint16 u16Port)
{
    if( u16Port == PORTA)
        return GpioDataRegs.GPADAT.all;
    else
        return GpioDataRegs.GPBDAT.all;
}
