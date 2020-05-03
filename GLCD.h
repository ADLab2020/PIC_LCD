/*
 * File:   GLCD.h
 * Author: Analogdigitallab
 * Website: http://www.analogdigitallab.com/
 *
 * Created on 2012/10/20, 21:55
 */

#ifndef GLCD_H
#define	GLCD_H
#include "HardwareProfile.h"
#include "Delay.h"

#define BIT8     /*8 bits mode */

// This define states the delay when strobing the EN
// line in uS
#define EN_STROBE_DELAY 10

/*    Commands    */
#define LCD_ON         0b00111111       /* 0x3F  Turn the display on     */
#define LCD_OFF        0b00111110       /* 0x3F  Turn the display off     */
#define xSTART         0b10111000       /* 0xB8  Set X address = 0       */
#define ySTART         0b01000000       /* 0x40  Set Y address = 0       */
#define LINE_START     0b11000000       /* 0xC0  Set start line = 0      */
#define CLEAR          0b00000001       /* 0x01  Clear the display       */
#define LCD_READ       0b11111111       /* 0xFF  Set R/W-pin to 1, read  */
#define LCD_WRITE      0b00000000       /* 0x00  Set R/W-pin to 0, write  */

#define LCD_CMD		0                 
#define LCD_DATA	1




void LCD_Init(void);
void LCD_Clear(void);  /* Clear the whole display */
//void LCD_Clear(unsigned char side);
/* Choose to clear left half side or right half side
 */
void LCD_Off(void);
void CheckBusy(void);
/* left side: 0, right side: 1
 data bit DB7=1 -> busy, DB7=0 -> aidle
 */
void WriteCmd(unsigned char cmd, unsigned char side);
void WriteData(unsigned char data,unsigned char side);
unsigned char ReadData(unsigned char,unsigned char);


void WriteChar(unsigned char*,unsigned char);
void appendCharToLCD(char,unsigned char);





/***********************************************************/
/*              Drawing an image                          */
/***********************************************************/
void SetPixel(unsigned char,unsigned char, unsigned char);
void DrawLine(unsigned char,unsigned char,unsigned char,unsigned char);
void DrawCircle(unsigned char, unsigned char, unsigned char);
void DrawSample(unsigned char,unsigned char);
void DrawXLine(unsigned char,unsigned char,unsigned char);
void DrawYLine(unsigned char,unsigned char,unsigned char);
void Draw64X64(rom const unsigned char*,unsigned char);
void Draw128X64(rom const unsigned char*);
void DrawFFTGraph(short[]);

//void AnalogDigitalLab(void);
/*******************     Demos      *******************/
//void Demo_01(void);
void Demo_02(unsigned short);
//void Demo_03(void);
/*
 Draw a table
 */
//void Demo_04(void);
/*
 Draw lines between two arbitrary points
 */
void Demo_05(void);
#endif	/* GLCD_H */

