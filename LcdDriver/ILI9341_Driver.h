//*****************************************************************************
//
// Template_Driver.h - Display driver for any LCD Controller. This file serves as
//						a template for creating new LCD driver files
//
// Copyright (c) 2008-2011 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
// 
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
// 
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
// 
//
//*****************************************************************************

#ifndef __ILI9341_Driver_H__
#define __ILI9341_Driver_H__

//*****************************************************************************
//
// Include Files
//
//*****************************************************************************

/*Put any necessary include files here*/
#include <msp430f5529.h>

//*****************************************************************************
//
// User Configuration for the LCD Driver
//
//*****************************************************************************
// Number of pixels on LCD X-axis
#define LCD_X_SIZE	240

// Number of pixels on LCD Y-axis
#define LCD_Y_SIZE	320

// Number of bits required to draw one pixel on the LCD screen
#define BPP	16

// Define LCD Screen Orientation Here
#define LANDSCAPE
//#define LANDSCAPE_FLIP
//#define PORTRAIT
//#define PORTRAIT_FLIP

#define SYSTEM_CLOCK_SPEED 25000000
//*****************************************************************************
//
// Defines for the pins that are used to communicate with the LCD Driver
//
//*****************************************************************************

/*Define Pin Names here i.e. #define LCD_RESET_OUT	P6OUT*/

//*****************************************************************************
//
// Defines for LCD driver configuration
//
//*****************************************************************************

/* Defines for pixels, colors, masks, etc. Anything Template_Driver.c needs*/

#define LCD_DATA_BUS 8
#define LCD_BUS_WIDTH 8
//*****************************************************************************
//
// This driver operates in four different screen orientations.  They are:
//
// * Portrait - The screen is taller than it is wide. This is selected by defining
//              PORTRAIT.
//
// * Landscape - The screen is wider than it is tall. This is selected by defining
//               LANDSCAPE.
//
// * Portrait flip - The screen is taller than it is wide. This is
//                   selected by defining PORTRAIT_FLIP.
//
// * Landscape flip - The screen is wider than it is tall. This is
//                    selected by defining LANDSCAPE_FLIP.
//
// These can also be imagined in terms of screen rotation; if landscape mode is
// 0 degrees of screen rotation, portrait flip is 90 degrees of clockwise
// rotation, landscape flip is 180 degrees of rotation, and portrait is
// 270 degress of clockwise rotation.
//
// If no screen orientation is selected, "landscape" mode will be used.
//
//*****************************************************************************
#if ! defined(PORTRAIT) && ! defined(PORTRAIT_FLIP) && \
    ! defined(LANDSCAPE) && ! defined(LANDSCAPE_FLIP)
#define LANDSCAPE
#endif

//*****************************************************************************
//
// Various definitions controlling coordinate space mapping and drawing
// direction in the four supported orientations.
//
//*****************************************************************************
#ifdef PORTRAIT
#define MAPPED_X(x, y) (x)
#define MAPPED_Y(x, y) (y)
#endif
#ifdef PORTRAIT_FLIP
#define MAPPED_X(x, y) (LCD_X_SIZE - (y) - 1)
#define MAPPED_Y(x, y) (x)
#endif
#ifdef LANDSCAPE
#define MAPPED_X(x, y)  (y)
#define MAPPED_Y(x, y)  (LCD_Y_SIZE - (x) - 1)
#endif
#ifdef LANDSCAPE_FLIP
#define MAPPED_X(x, y)  (LCD_X_SIZE - (x) - 1)
#define MAPPED_Y(x, y)  (LCD_Y_SIZE - (y) - 1)
#endif


//*****************************************************************************
//
// Various LCD Controller command name labels and associated control bits
//
//*****************************************************************************
// GPIO Pin values on MSP430
#define GPIO_PIN_0              0x0001  // GPIO pin 0
#define GPIO_PIN_1              0x0002  // GPIO pin 1
#define GPIO_PIN_2              0x0004  // GPIO pin 2
#define GPIO_PIN_3              0x0008  // GPIO pin 3
#define GPIO_PIN_4              0x0010  // GPIO pin 4
#define GPIO_PIN_5              0x0020  // GPIO pin 5
#define GPIO_PIN_6              0x0040  // GPIO pin 6
#define GPIO_PIN_7              0x0080  // GPIO pin 7

// Data register offsets into Ports on MSP430
#define GPIO_P1_DATA             0x0002  // GPIO Data
#define GPIO_P2_DATA             0x0003  // GPIO Data
#define GPIO_P3_DATA             0x0002  // GPIO Data
#define GPIO_P4_DATA             0x0003  // GPIO Data
#define GPIO_P5_DATA             0x0002  // GPIO Data
#define GPIO_P6_DATA             0x0003  // GPIO Data
#define GPIO_P7_DATA             0x0002  // GPIO Data
#define GPIO_P8_DATA             0x0003  // GPIO Data
// Data Pins
#define LCD_DATA_BASE          P2_BASE

// Control Pins
#define LCD_CS_BASE             P8_BASE
#define LCD_CS_PIN              GPIO_PIN_1
#define LCD_RST_BASE            P8_BASE
#define LCD_RST_PIN             GPIO_PIN_2
#define LCD_RD_BASE             P3_BASE
#define LCD_RD_PIN              GPIO_PIN_5
#define LCD_WR_BASE             P3_BASE
#define LCD_WR_PIN              GPIO_PIN_6
#define LCD_DC_BASE             P3_BASE
#define LCD_DC_PIN              GPIO_PIN_7


#define C_FUCHSIA		0xF81F


//*****************************************************************************
//
// Macros for the Display Driver
//
//*****************************************************************************

/* All macros can go here. This is typically the color translation function (example below)
and could also include Set_Address(), Write_Data(), etc. */


#define DISPLAY_ON      set_instruction(0,0x29)
#define DISPLAY_OFF     set_instruction(0,0x28)
#define MEM_WRITE       set_instruction(0,0x2C)


// Macro for direct hardware access
//
// \param x is the hardware register address.
//
// This macro is used to directly access hardware registers. The various HWREGX
// macros use a different sized pointer for different types of data access.
//
// \return None

#define HWREG(x)                                                              \
        (*((volatile unsigned int *)(x)))
#define HWREGH(x)                                                             \
        (*((volatile unsigned short *)(x)))
#define HWREGB(x)                                                             \
        (*((volatile unsigned char *)(x)))
		

		// Macro used to set the LCD data bus 
//
// \param uByte is the 8 or 16 bit (depending on parallel bus type)
// value to write to bus
//
// Define depending on the bus width, 8 or 16 pins, a char or an int
// is written to the Port Pin registers
//
// \return None

#if LCD_BUS_WIDTH == 8
#define SET_LCD_DATA(uByte)                                                \
	HWREGB(LCD_DATA_BASE + GPIO_P2_DATA) = (uByte)
    //HWREGB(LCD_DATA_BASE + P2OUT) = (uByte)

#else
#define SET_LCD_DATA(uByte)                                               \
    HWREG(LCD_DATA_BASE + GPIO_P2_DATA) = (uByte)

#endif

//
// Writes a command to the LCD Driver
//
// \param ucCommand is the 8 bit command to send to the LCD driver
// This particular LCD has only 8 bit commands
//
// \return None

#define WriteCommand(ucCommand)                                            \
    HWREGB(LCD_DC_BASE + GPIO_P3_DATA) &= ~LCD_DC_PIN;                     \
    SET_LCD_DATA(ucCommand);                                               \
    HWREGB(LCD_WR_BASE + GPIO_P3_DATA) &= ~LCD_WR_PIN;                     \
    HWREGB(LCD_WR_BASE + GPIO_P3_DATA) |= LCD_WR_PIN;                      \
    HWREGB(LCD_DC_BASE + GPIO_P3_DATA) |= LCD_DC_PIN


//
// Writes data to the LCD Driver
//
// \param uData is the 8 or 16 bit command to send to the LCD driver
// Uses the SET_LCD_DATA macro
//
// \return None
#if LCD_DATA_BUS == 8
#define WriteData(uData)                                                  \
    SET_LCD_DATA((uData)>>8);                                             \
    HWREGB(LCD_WR_BASE + GPIO_P3_DATA) &= ~LCD_WR_PIN;                    \
    HWREGB(LCD_WR_BASE + GPIO_P3_DATA) |= LCD_WR_PIN;                     \
    SET_LCD_DATA(uData&0xFF);                                             \
    HWREGB(LCD_WR_BASE + GPIO_P3_DATA) &= ~LCD_WR_PIN;                    \
    HWREGB(LCD_WR_BASE + GPIO_P3_DATA) |= LCD_WR_PIN;                     \

#else
#define WriteData(uData)                                                  \
    SET_LCD_DATA(uData);                                                  \
    HWREGB(LCD_WR_BASE + P3_BASE) &= ~LCD_WR_PIN;                    \
    HWREGB(LCD_WR_BASE + P3_BASE) |= LCD_WR_PIN
#endif


//
// Translates a 24-bit RGB color to a display driver-specific color.
//
// \param c is the 24-bit RGB color.  The least-significant byte is the blue
// channel, the next byte is the green channel, and the third byte is the red
// channel.
//
// This macro translates a 24-bit RGB color into a value that can be written
// into the display's frame buffer in order to reproduce that color, or the
// closest possible approximation of that color. This particular example
// requires the 8-8-8 24 bit RGB color to convert into 5-6-5 16 bit RGB Color
// Your conversion should be made to fit your LCD settings.
//
// \return Returns the display-driver specific color
//
// TemplateDisplayFix

#define DPYCOLORTRANSLATE(c)    ((((c) & 0x00f80000) >> 8) |               \
                                 (((c) & 0x0000fc00) >> 5) |               \
                                 (((c) & 0x000000f8) >> 3))

//*****************************************************************************
//
// Prototypes for the globals exported by this driver.
//
//*****************************************************************************
void wait_ms(unsigned int );
void wait_us(unsigned int );
static void set_instruction(unsigned char register_sel, unsigned char uData);
unsigned char SetAddress(int x, int y);
unsigned char set_cursor_x(int x);
unsigned char set_cursor_y(int y);
extern void ILI9341_DriverInit(void);
extern const tDisplay g_sILI9341_Driver;
//extern unsigned char Template_Memory[];


#endif // __ILI9341_Driver_H__
