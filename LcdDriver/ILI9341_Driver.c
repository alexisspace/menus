//*****************************************************************************
//
// Template_Driver.c - Display driver for any LCD Controller. This file serves as
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
//*****************************************************************************
//
//! \addtogroup display_api
//! @{
//
//*****************************************************************************

//*****************************************************************************
//
// READ ME
//
// This template driver is intended to be modified for creating new LCD drivers
// It is setup so that only Template_DriverPixelDraw() and DPYCOLORTRANSLATE()
// and some LCD size configuration settings in the header file Template_Driver.h
// are REQUIRED to be written. These functions are marked with the string
// "TemplateDisplayFix" in the comments so that a search through Template_Driver.c and
// Template_Driver.h can quickly identify the necessary areas of change.
//
// Template_DriverPixelDraw() is the base function to write to the LCD
// display. Functions like WriteData(), WriteCommand(), and SetAddress()
// are suggested to be used to help implement the Template_DriverPixelDraw()
// function, but are not required. SetAddress() should be used by other pixel
// level functions to help optimize them.
// 
// This is not an optimized driver however and will significantly impact 
// performance. It is highly recommended to first get the prototypes working
// with the single pixel writes, and then go back and optimize the driver.
// Please see application note www.ti.com/lit/pdf/slaa548 for more information
// on how to fully optimize LCD driver files. In short, driver optimizations
// should take advantage of the auto-incrementing of the LCD controller. 
// This should be utilized so that a loop of WriteData() can be used instead
// of a loop of Template_DriverPixelDraw(). The pixel draw loop contains both a
// SetAddress() + WriteData() compared to WriteData() alone. This is a big time 
// saver especially for the line draws and Template_DriverPixelDrawMultiple.
// More optimization can be done by reducing function calls by writing macros,
// eliminating unnecessary instructions, and of course taking advantage of other 
// features offered by the LCD controller. With so many pixels on an LCD screen
// each instruction can have a large impact on total drawing time.
//
//*****************************************************************************


//*****************************************************************************
//
// Include Files
//
//*****************************************************************************
#include <msp430.h>
#include "grlib.h"
#include "ILI9341_Driver.h"


//*****************************************************************************
//
// Global Variables
//
//*****************************************************************************
// see init_display.h for command description
const unsigned char init_cmd[]={0xCB,0xCF,0xE8,0xEA,0xED,0xF7,0xC0,0xC1,0xC5,0xC7,0x36,0x3A,0xB1,0xB6,0xF2,0x26,0xE0,0xE1};
const unsigned char init_data_count[]={5,3,3,2,4,1,1,1,2,1,1,1,2,3,1,1,15,15};
const unsigned char init_data[]={0x39,0x2C,0x00,0x34,0x02,
            0x00,0xC1,0x30,
            0x85,0x00,0x78,
            0x00,0x00,
            0x64,0x03,0x12,0x81,
            0x20,
            0x23,
            0x10,
            0x3E,0x28,
            0x86,
            0x88,	// Memory access control
            0x55,
            0x00,0x18,
            0x08,0x82,0x27,
            0x00,
            0x01,
            0x0F,0x31,0x2B,0x0C,0x0E,0x08,0x4E,0xF1,0x37,0x07,0x10,0x03,0x0E,0x09,0x00,
            0x00,0x0E,0x14,0x03,0x11,0x07,0x31,0xC1,0x48,0x08,0x0F,0x0C,0x31,0x36,0x0F};


/* Global buffer for the display. This is especially useful on 1BPP, 2BPP, and 4BPP
  displays. This allows you to update pixels while reading the neighboring pixels
  from the buffer instead of a read from the LCD controller. A buffer is not required
  as a read followed by a write can be used instead.*/

//unsigned char Template_Memory[(LCD_X_SIZE * LCD_Y_SIZE * BPP + 7) / 8];





//*****************************************************************************
//
// Suggested functions to help facilitate writing the required functions below
//
//*****************************************************************************
void wait_ms(unsigned int m_sec)
{
  while(m_sec--)
    __delay_cycles(16000);  // 1000 for 1MHz
}

void wait_us(unsigned int u_sec)
{
  while(u_sec--)
    __delay_cycles(16);   // 1 for 1MHz
}

// Writes data to the LCD controller
/*
static void
WriteData(unsigned short usData)
{
	// Write data to the LCD controller. For instance this can be bit banged
	//with 6800 or 8080 protocol or this could be the SPI routine for a SPI LCD
}
*/
// Writes a command to the LCD controller
static void
set_instruction(unsigned char register_sel, unsigned char uData)
{
   /* This function is typically very similar (sometimes the same) as WriteData()
   The difference is that this is for the LCD to interpret commands instead of pixel
   data. For instance in 8080 protocol, this means pulling the DC line low.*/
  if(register_sel) 
  {
    // Data
    // Write the most significant byte of the data to the bus.
    SET_LCD_DATA(uData);

    // Assert the write enable signal.
    HWREGB(LCD_WR_BASE + GPIO_P3_DATA) &= ~LCD_WR_PIN;

    // Deassert the write enable signal.
    HWREGB(LCD_WR_BASE + GPIO_P3_DATA) |= LCD_WR_PIN;

  }
  else
  {
    // Command
    // Assert DC
    HWREGB(LCD_DC_BASE + GPIO_P3_DATA) &= ~LCD_DC_PIN;

    // Write the least significant byte of the data to the bus.
    SET_LCD_DATA(uData);

    // Assert the write enable signal.
    HWREGB(LCD_WR_BASE + GPIO_P3_DATA) &= ~LCD_WR_PIN;

    // Deassert the write enable signal.
    HWREGB(LCD_WR_BASE + GPIO_P3_DATA) |= LCD_WR_PIN;

    // Set the DC signal high, indicating that following writes are data.
    HWREGB(LCD_DC_BASE + GPIO_P3_DATA) |= LCD_DC_PIN;
  }
}

// Sets the pixel address of the LCD driver
unsigned char SetAddress(int x, int y)
{
	int x1;
    /* This function typically writes commands (using WriteCommand()) to the 
	LCD to tell it where to move the cursor for the next pixel to be drawn. */
	x1 = MAPPED_X(x, y);
	y = MAPPED_Y(x, y);
	if(!set_cursor_x(x1)||!set_cursor_y(y)) return 0;
	return 1;
}

unsigned char set_cursor_x(int x)
{
  if(x<0||x>=LCD_X_SIZE) return 0;
  set_instruction(0,0x2A);
  set_instruction(1,x>>8);
  set_instruction(1,x&0xFF);
  MEM_WRITE;
  return 1;
}

unsigned char set_cursor_y(int y)
{
  if(y<0||y>=LCD_Y_SIZE) return 0;
  set_instruction(0,0x2B);
  set_instruction(1,y>>8);
  set_instruction(1,y&0xFF);
  MEM_WRITE;
  return 1;
}


// Initializes the pins required for the GPIO-based LCD interface.
// This function configures the GPIO pins used to control the LCD display
// when the basic GPIO interface is in use. On exit, the LCD controller
// has been reset and is ready to receive command and data writes.
static void InitGPIOLCDInterface(void)
{
    /* Initialize the hardware to configure the ports properly for communication */
    P2DIR = 0xFF;     // Set Port 2 as outputs
    P2OUT = 0x00;     // Set Port 2 outputs low
    P3DIR = 0xFF;     // Set Port 3 as outputs
    P3OUT = 0xFF;     // Set Port 3 outputs low
    P8DIR = 0xFF;     // Set Port 8 as outputs
    P8OUT = 0xFF;     // Set Port 3 outputs low
}

// Initialize DisplayBuffer.
// This function initializes the display buffer and discards any cached data.
static void
InitLCDDisplayBuffer(void *pvDisplayData, unsigned int ulValue)
{
	unsigned int i=0,j=0;
	for(i =0; i< LCD_Y_SIZE; i++)
	for(j =0; j< (LCD_X_SIZE * BPP + 7) / 8; j++)
		//ILI9341_Memory[i * LCD_Y_SIZE + j] = ulValue;
		;
}

// Initializes the display driver.
// This function initializes the LCD controller
//
// TemplateDisplayFix
void
ILI9341_DriverInit(void)
{
  unsigned char byte=0,bit_num=0,h_index=0,w_index=0;
	/*Initialize the LCD controller.
	This typically looks like:
	
    InitGPIOLCDInterface();
    InitLCDDisplayBuffer();
	// Init LCD controller parameters
	// Enable LCD
	// Init Backlight
	// Clear Screen
	*/
  

  InitGPIOLCDInterface();

  P3OUT |= 0xFF;

  P8OUT|=LCD_RST_PIN;
  wait_ms(10);
  P8OUT&=~LCD_RST_PIN;
  wait_ms(10);
  P8OUT|=LCD_RST_PIN;
  wait_ms(100);
  
  while(byte<sizeof(init_cmd))            // byte is init_cmd index
  {
    set_instruction(0,init_cmd[byte++]);      
    while(h_index<init_data_count[bit_num])     // h_index is size of init_data command part counter
    {                       // bit_num is init_data_count index
      set_instruction(1,init_data[w_index++]);  // w_index is init_data index
      h_index++;
    }
    bit_num++;
    h_index=0;
  }
    
  set_instruction(0,0x11);  // SLEEP OUT
  wait_ms(120);       // 120ms needed (see description of sleep-out-cmd)
  
    MEM_WRITE;
    //ILI9341_DriverClearScreen(&g_sILI9341_Driver, GRAPHICS_COLOR_WHITE);
    DISPLAY_ON;
    MEM_WRITE;

}


//*****************************************************************************
//
// All the following functions (below) for the LCD driver are required by grlib
//
//*****************************************************************************

//*****************************************************************************
//
//! Draws a pixel on the screen.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//! \param lX is the X coordinate of the pixel.
//! \param lY is the Y coordinate of the pixel.
//! \param ulValue is the color of the pixel.
//!
//! This function sets the given pixel to a particular color.  The coordinates
//! of the pixel are assumed to be within the extents of the display.
//!
//! \return None.
//
//*****************************************************************************
// TemplateDisplayFix
static void
ILI9341_DriverPixelDraw(void *pvDisplayData, int lX, int lY,
                                   unsigned int ulValue)
{  
  /* This function already has checked that the pixel is within the extents of  
  the LCD screen and the color ulValue has already been translated to the LCD.
  This function typically looks like:*/
  
  // Interpret pixel data (if needed)
  // Sets the pixel address of the LCD driver
  SetAddress(lX, lY);

  MEM_WRITE;
  WriteData(ulValue);

  /*
  // Update buffer (if applicable)
  // Template_Memory[lY * LCD_Y_SIZE + (lX * BPP / 8)] = , |= , &= ...
  // Template memory must be modified at the bit level for 1/2/4BPP displays

  // SetAddress(MAPPED_X(lX, lY), MAPPED_Y(lX, lY));
  // WriteData(ulValue);
  */
}

//*****************************************************************************
//
//! Draws a horizontal sequence of pixels on the screen.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//! \param lX is the X coordinate of the first pixel.
//! \param lY is the Y coordinate of the first pixel.
//! \param lX0 is sub-pixel offset within the pixel data, which is valid for 1
//! or 4 bit per pixel formats.
//! \param lCount is the number of pixels to draw.
//! \param lBPP is the number of bits per pixel; must be 1, 4, or 8.
//! \param pucData is a pointer to the pixel data.  For 1 and 4 bit per pixel
//! formats, the most significant bit(s) represent the left-most pixel.
//! \param pucPalette is a pointer to the palette used to draw the pixels.
//!
//! This function draws a horizontal sequence of pixels on the screen, using
//! the supplied palette.  For 1 bit per pixel format, the palette contains
//! pre-translated colors; for 4 and 8 bit per pixel formats, the palette
//! contains 24-bit RGB values that must be translated before being written to
//! the display.
//!
//! \return None.
//
//*****************************************************************************
static void
ILI9341_DriverPixelDrawMultiple(void *pvDisplayData, int lX,
                                           int lY, int lX0, int lCount,
                                           int lBPP,
                                           const unsigned char *pucData,
                                           const unsigned int *pucPalette)
{
	unsigned int ulByte;


    // Determine how to interpret the pixel data based on the number of bits
    // per pixel.
    //
    switch(lBPP)
    {
        // The pixel data is in 1 bit per pixel format
        case 1:
        {
            // Loop while there are more pixels to draw
            while(lCount > 0)
            {
                // Get the next byte of image data
                ulByte = *pucData++;

                // Loop through the pixels in this byte of image data
                for(; (lX0 < 8) && lCount; lX0++, lCount--)
                {
                    // Draw this pixel in the appropriate color
					ILI9341_DriverPixelDraw(pvDisplayData, lX++, lY,
											((unsigned int *)pucPalette)[(ulByte >> (7 - lX0)) & 1]);
                }
                
                // Start at the beginning of the next byte of image data
                lX0 = 0;
            }
            // The image data has been drawn
            
            break;
        }

		// The pixel data is in 2 bit per pixel format
        case 2:
        {
            // Loop while there are more pixels to draw
            while(lCount > 0)
            {
                // Get the next byte of image data
                ulByte = *pucData++;

                // Loop through the pixels in this byte of image data
                for(; (lX0 < 4) && lCount; lX0++, lCount--)
                {
                    // Draw this pixel in the appropriate color
					ILI9341_DriverPixelDraw(pvDisplayData, lX++, lY,
											((unsigned int *)pucPalette)[(ulByte >> (6 - (lX0 << 1))) & 3]);
                }
                
                // Start at the beginning of the next byte of image data
                lX0 = 0;
            }
            // The image data has been drawn
            
            break;
		}
        // The pixel data is in 4 bit per pixel format
        case 4:
        {
            // Loop while there are more pixels to draw.  "Duff's device" is
            // used to jump into the middle of the loop if the first nibble of
            // the pixel data should not be used.  Duff's device makes use of
            // the fact that a case statement is legal anywhere within a
            // sub-block of a switch statement.  See
            // http://en.wikipedia.org/wiki/Duff's_device for detailed
            // information about Duff's device.
            switch(lX0 & 1)
            {
                case 0:
                  
                    while(lCount)
                    {
                        // Get the upper nibble of the next byte of pixel data
                        // and extract the corresponding entry from the palette
                        ulByte = (*pucData >> 4);    
                        ulByte = (*(unsigned int *)(pucPalette + ulByte));
                        // Write to LCD screen
                        ILI9341_DriverPixelDraw(pvDisplayData, lX++, lY, ulByte);
                        
                        // Decrement the count of pixels to draw
                        lCount--;
                        
                        // See if there is another pixel to draw
                        if(lCount)
                        {
                case 1:
                            // Get the lower nibble of the next byte of pixel
                            // data and extract the corresponding entry from
                            // the palette
                            ulByte = (*pucData++ & 15);
                            ulByte = (*(unsigned int *)(pucPalette + ulByte));
                            // Write to LCD screen
                            ILI9341_DriverPixelDraw(pvDisplayData, lX++, lY, ulByte);

                            // Decrement the count of pixels to draw
                            lCount--;
                        }
                    }
            }
            // The image data has been drawn.
           
            break;
        }

        // The pixel data is in 8 bit per pixel format
        case 8:
        {
            // Loop while there are more pixels to draw
            while(lCount--)
            {
                // Get the next byte of pixel data and extract the
                // corresponding entry from the palette
                ulByte = *pucData++;
                ulByte = (*(unsigned int *)(pucPalette + ulByte));
                // Write to LCD screen
                ILI9341_DriverPixelDraw(pvDisplayData, lX++, lY, ulByte);
            }
            // The image data has been drawn
            break;
        }
    }
}

//*****************************************************************************
//
//! Draws a horizontal line.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//! \param lX1 is the X coordinate of the start of the line.
//! \param lX2 is the X coordinate of the end of the line.
//! \param lY is the Y coordinate of the line.
//! \param ulValue is the color of the line.
//!
//! This function draws a horizontal line on the display.  The coordinates of
//! the line are assumed to be within the extents of the display.
//!
//! \return None.
//
//*****************************************************************************
static void
ILI9341_DriverLineDrawH(void *pvDisplayData, int lX1, int lX2,
                                   int lY, unsigned int ulValue)
{
  /* Ideally this function shouldn't call pixel draw. It should have it's own
  definition using the built in auto-incrementing of the LCD controller and its 
  own calls to SetAddress() and WriteData(). Better yet, SetAddress() and WriteData()
  can be made into macros as well to eliminate function call overhead. */
#ifdef LANDSCAPE
	  int rc, c1;
	  rc = MAPPED_X(lX1,lY);
	  c1 = MAPPED_Y(lX2,lY);

	  set_instruction(0,0x2A);
	  set_instruction(1,rc>>8);
	  set_instruction(1,rc&0xFF);
	  // Same end column to increment cursor downwards
	  set_instruction(1,rc>>8);
	  set_instruction(1,rc&0xFF);

	  set_instruction(0,0x2B);
	  set_instruction(1,c1>>8);
	  set_instruction(1,c1&0xFF);
	  MEM_WRITE;
	  while(lX1++<=lX2)
	  {
	    WriteData(ulValue);
	  }
	  // Restore
	  set_instruction(0,0x2A);
	  set_instruction(1,rc>>8);
	  set_instruction(1,rc&0xFF);
	  // Restore end colunm OF THE DISPLAY
	  c1 = MAPPED_X(LCD_X_SIZE, LCD_Y_SIZE) - 1;
	  set_instruction(1,c1>>8);
	  set_instruction(1,c1&0xFF);
#endif
}

//*****************************************************************************
//
//! Draws a vertical line.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//! \param lX is the X coordinate of the line.
//! \param lY1 is the Y coordinate of the start of the line.
//! \param lY2 is the Y coordinate of the end of the line.
//! \param ulValue is the color of the line.
//!
//! This function draws a vertical line on the display.  The coordinates of the
//! line are assumed to be within the extents of the display.
//!
//! \return None.
//
//*****************************************************************************
static void
ILI9341_DriverLineDrawV(void *pvDisplayData, int lX, int lY1,
                                   int lY2, unsigned int ulValue)
{
#ifdef LANDSCAPE
	  int rc, c1;
	  rc = MAPPED_Y(lX,lY1);
	  c1 = MAPPED_X(lX,lY1);

	  set_instruction(0,0x2A); // LCD colunm
	  set_instruction(1,c1>>8);
	  set_instruction(1,c1&0xFF);
	  set_instruction(0,0x2B); // LCD row
	  set_instruction(1,rc>>8);
	  set_instruction(1,rc&0xFF);
	  MEM_WRITE;
	  while(lY1++<=lY2)
	  {
	    WriteData(ulValue);
	  }

#endif
}

//*****************************************************************************
//
//! Fills a rectangle.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//! \param pRect is a pointer to the structure describing the rectangle.
//! \param ulValue is the color of the rectangle.
//!
//! This function fills a rectangle on the display.  The coordinates of the
//! rectangle are assumed to be within the extents of the display, and the
//! rectangle specification is fully inclusive (in other words, both sXMin and
//! sXMax are drawn, along with sYMin and sYMax).
//!
//! \return None.
//
//*****************************************************************************
static void
ILI9341_DriverRectFill(void *pvDisplayData, const tRectangle *pRect,
                                  unsigned int ulValue)
{
  int x0 = pRect->sXMin;
  int x1 = pRect->sXMax;
  int y0 = pRect->sYMin;
  int y1 = pRect->sYMax;
  
  while(y0 <= y1)
  {
    ILI9341_DriverLineDrawH(pvDisplayData, x0, x1, y0, ulValue);
    y0++;
  }
}

//*****************************************************************************
//
//! Translates a 24-bit RGB color to a display driver-specific color.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//! \param ulValue is the 24-bit RGB color.  The least-significant byte is the
//! blue channel, the next byte is the green channel, and the third byte is the
//! red channel.
//!
//! This function translates a 24-bit RGB color into a value that can be
//! written into the display's frame buffer in order to reproduce that color,
//! or the closest possible approximation of that color.
//!
//! \return Returns the display-driver specific color.
//
//*****************************************************************************
static unsigned long
ILI9341_DriverColorTranslate(void *pvDisplayData,
                                        unsigned long ulValue)
{
	/* The DPYCOLORTRANSLATE macro should be defined in TemplateDriver.h */
	
    //
    // Translate from a 24-bit RGB color to a color accepted by the LCD.
    //
    return(DPYCOLORTRANSLATE(ulValue));
}

//*****************************************************************************
//
//! Flushes any cached drawing operations.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//!
//! This functions flushes any cached drawing operations to the display.  This
//! is useful when a local frame buffer is used for drawing operations, and the
//! flush would copy the local frame buffer to the display.
//!
//! \return None.
//
//*****************************************************************************
static void
ILI9341_DriverFlush(void *pvDisplayData)
{
  // Flush Buffer here. This function is not needed if a buffer is not used,
  // or if the buffer is always updated with the screen writes.
	int i=0,j=0;
	for(i =0; i< LCD_Y_SIZE; i++)
	for(j =0; j< (LCD_X_SIZE * BPP + 7) / 8; j++)
		//ILI9341_DriverPixelDraw(pvDisplayData, j, i, ILI9341_Memory[i * LCD_Y_SIZE + j]);
		;
}

//*****************************************************************************
//
//! Send command to clear screen.
//!
//! \param pvDisplayData is a pointer to the driver-specific data for this
//! display driver.
//!
//! This function does a clear screen and the Display Buffer contents
//! are initialized to the current background color.
//!
//! \return None.
//
//*****************************************************************************
static void
ILI9341_DriverClearScreen(void *pvDisplayData, unsigned int ulValue)
{
	// This fills the entire display to clear it
	// Some LCD drivers support a simple command to clear the display
	int y0 = -1;
#if defined(PORTRAIT) || defined(PORTRAIT_FLIP)
	while(y0++ <= (LCD_Y_SIZE - 1))
	{
		ILI9341_DriverLineDrawH(pvDisplayData, 0, LCD_X_SIZE - 1, y0, ulValue);
	}
#else
	while(y0++ <= (LCD_X_SIZE - 1))
	{
		ILI9341_DriverLineDrawH(pvDisplayData, 0, LCD_Y_SIZE - 1, y0, ulValue);
	}
#endif
}

//*****************************************************************************
//
//! The display structure that describes the driver for the blank template.
//
//*****************************************************************************
const tDisplay g_sILI9341_Driver =
{
    sizeof(tDisplay),
    0,
	320,
	240,
    ILI9341_DriverPixelDraw,
    ILI9341_DriverPixelDrawMultiple,
    ILI9341_DriverLineDrawH,
    ILI9341_DriverLineDrawV,
    ILI9341_DriverRectFill,
    ILI9341_DriverColorTranslate,
    ILI9341_DriverFlush,
    ILI9341_DriverClearScreen
};


//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
