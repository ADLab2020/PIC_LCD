/* Author: Analogdigitallab
 * Website: http://www.analogdigitallab.com/
*/

#ifndef GRAPH_C
#define GRAPH_C

// Global includes
//#include <htc.h>


// Local includes

#include <stdlib.h>
#include "GLCD.h"
#include "Bitmap.h"

rom const unsigned char linTable[182] =
{
	0, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 6, 7, 7, 7, 8,
	8, 9, 9, 9, 10, 10, 10, 11, 11, 11, 12, 12, 12, 13, 13, 13, 14, 14,
	14, 15, 15, 15, 16, 16, 17, 17, 17, 18, 18, 18, 19, 19, 19, 20, 20,
	20, 21, 21, 21, 22, 22, 22, 23, 23, 23, 24, 24, 25, 25, 25, 26, 26,
	26, 27, 27, 27, 28, 28, 28, 29, 29, 29, 30, 30, 30, 31, 31, 31, 32,
	32, 33, 33, 33, 34, 34, 34, 35, 35, 35, 36, 36, 36, 37, 37, 37, 38,
	38, 38, 39, 39, 39, 40, 40, 41, 41, 41, 42, 42, 42, 43, 43, 43, 44,
	44, 44, 45, 45, 45, 46, 46, 46, 47, 47, 47, 48, 48, 49, 49, 49, 50,
	50, 50, 51, 51, 51, 52, 52, 52, 53, 53, 53, 54, 54, 54, 55, 55, 55,
	56, 56, 57, 57, 57, 58, 58, 58, 59, 59, 59, 60, 60, 60, 61, 61, 61,
	62, 62, 62, 63, 63, 63
};






short displayData[32];

/***********************************************************/
/*                  FUNCTIONS                              */
/***********************************************************/

void LCD_Init()
{
     DATA_DIRECTION=OUTPUT;
     TRISB=0b00000000;
    delay_ms(200);  /* Wait a bit after power-up */
    // Y=0
    WriteCmd(ySTART,0);
    WriteCmd(ySTART,1);
    // X=0
    WriteCmd(xSTART,0);
    WriteCmd(xSTART,1);
    // LINE_START=0
    WriteCmd(LINE_START,0);
    WriteCmd(LINE_START,1);
    //Turn the display on
    WriteCmd(LCD_ON,0);
    WriteCmd(LCD_ON,1);
    delay_ms(100); /* Wait a bit after power-up */
    LCD_Clear();
}

void LCD_Off(void)
{
    WriteCmd(LCD_OFF,0);
    WriteCmd(LCD_OFF,1);
}


void LCD_Clear()
{
    unsigned char i,j;
    for(i=0;i<8;i++){
        WriteCmd(xSTART+i,0);
        WriteCmd(xSTART+i,1);
        WriteCmd(0x40,0);
        WriteCmd(0x40,1);
        for(j=0;j<64;j++){
            WriteData(0x00,0);
            WriteData(0x00,1);
        }

    }
}

void CheckBusy(void)
{
    do
    {
        ENABLE=1;
        delay_us(EN_STROBE_DELAY);
        ENABLE=0;

    }while(DB7==1);
}

void WriteCmd(unsigned char cmd, unsigned char side)
{
    DATA_DIRECTION=LCD_READ;
    RW=LCD_READ;
    RS=LCD_CMD;

    CheckBusy();

    if(side==0)
    {
        CS1=1; CS2=0;   //left half side is active
    }
    else
    {
        CS1=0;CS2=1;   //right half side is active
    }

    DATA_DIRECTION=LCD_WRITE;
    RW=LCD_WRITE;
    DATA_BUS=cmd;    // put the command on data bus

    ENABLE=1;                         // Clock CMD into LCD
    delay_us(EN_STROBE_DELAY);   //Wait until display has processed the data
    ENABLE=0;
}


void WriteData(unsigned char data,unsigned char side)
{
    DATA_DIRECTION=LCD_READ;
    RW=LCD_READ;
    RS=LCD_CMD;

    CheckBusy();

    if(side==0)
    {
        CS1=1; CS2=0;   //left half side is active
    }
    else
    {
        CS1=0;CS2=1;   //right half side is active
    }
    RS=LCD_DATA;
    DATA_DIRECTION=LCD_WRITE;
    RW=LCD_WRITE;
    DATA_BUS=data;    // put the command on data bus

    ENABLE=1;                        // Clock data into LCD
    delay_us(EN_STROBE_DELAY);   //Wait until display has processed the data
    ENABLE=0;
    
}

unsigned char ReadData(unsigned char page,unsigned char column)
{
    unsigned char state,side,temp,col;
    //choose the correct byte(correct address)

    if(column<=63)
    {
        side=0;col=column;
        WriteCmd(xSTART+page,0);
        WriteCmd(ySTART+column,0);
    }
    else
    {
        side=1;col=column-64;
        WriteCmd(xSTART+page,1);
        WriteCmd(ySTART+column-64,1);
    }
    //Set DATA_DIRECTION as READ
    DATA_DIRECTION=LCD_READ;
    RS=LCD_DATA;
    RW=LCD_READ;
  //  delay_us(EN_STROBE_DELAY);

   
    ENABLE=1;                     // Clock data into LCD
    delay_us(EN_STROBE_DELAY);   //Wait until display has processed the data   
    state=DATA_BUS;
    ENABLE=0;
  //  temp=state;
 //   WriteCmd(xSTART+page,side);
  //  WriteCmd(ySTART+col,side);
 //   WriteData(temp,side);
    //set DATA_DIRECTION as WRITE again
   //DATA_DIRECTION=LCD_WRITE;
   RS=LCD_CMD;
   RW=LCD_WRITE;
    return state;
}

/*
void WriteChar(unsigned char* ascii,unsigned char side)
{
    unsigned char j;
    if(side==0)
    {
        for(j=0;j<5;j++){WriteData( ascii[j],0);}
    }
    else
    {
        for(j=0;j<5;j++){WriteData( ascii[j],1);}
    }
}

void appendCharToLCD(char c,unsigned char side)
{
    switch(c)
   {

     case '_':WriteChar(Character_Cursor,side);break;
     case '?':WriteChar(Character_Question,side);break;
     case '\'':WriteChar(Character_Apostrophe,side);break;
     case '\"':WriteChar(Character_Quotation,side);break;
     case '!':WriteChar(Character_Exclamation,side);break;
     case ',':WriteChar(Character_Comma,side);break;
     case ' ':WriteChar(Character_Space,side);break;
     case '-':WriteChar(Character_Dash,side);break;
     case '<':WriteChar(Character_Left,side);break;
     case '>':WriteChar(Character_Right,side);break;
     case ':':WriteChar(Character_Colon,side);break;
     case '.':WriteChar(Character_Period,side);break;
     case '0':WriteChar(Character_ZERO,side);break;
     case '1':WriteChar(Character_ONE,side);break;
     case '2':WriteChar(Character_TWO,side);break;
     case '3':WriteChar(Character_THREE,side);break;
     case '4':WriteChar(Character_FOUR,side);break;
     case '5':WriteChar(Character_FIVE,side);break;
     case '6':WriteChar(Character_SIX,side);break;
     case '7':WriteChar(Character_SEVEN,side);break;
     case '8':WriteChar(Character_EIGHT,side);break;
     case '9':WriteChar(Character_NINE,side);break;
     case 'A':WriteChar(Character_A,side);break;
     case 'B':WriteChar(Character_B,side);break;
     case 'C':WriteChar(Character_C,side);break;
     case 'D':WriteChar(Character_D,side);break;
     case 'E':WriteChar(Character_E,side);break;
     case 'F':WriteChar(Character_F,side);break;
     case 'G':WriteChar(Character_G,side);break;
     case 'H':WriteChar(Character_H,side);break;
     case 'I':WriteChar(Character_I,side);break;
     case 'J':WriteChar(Character_J,side);break;
     case 'K':WriteChar(Character_K,side);break;
     case 'L':WriteChar(Character_L,side);break;
     case 'M':WriteChar(Character_M,side);break;
     case 'N':WriteChar(Character_N,side);break;
     case 'O':WriteChar(Character_O,side);break;
     case 'P':WriteChar(Character_P,side);break;
     case 'Q':WriteChar(Character_Q,side);break;
     case 'R':WriteChar(Character_R,side);break;
     case 'S':WriteChar(Character_S,side);break;
     case 'T':WriteChar(Character_T,side);break;
     case 'U':WriteChar(Character_U,side);break;
     case 'V':WriteChar(Character_V,side);break;
     case 'W':WriteChar(Character_W,side);break;
     case 'X':WriteChar(Character_X,side);break;
     case 'Y':WriteChar(Character_Y,side);break;
     case 'Z':WriteChar(Character_Z,side);break;
     default :WriteChar(Character_FULL,side);break;
   }
}
*/
void SetPixel(unsigned char xPos,unsigned char yPos, unsigned char value)
{
    unsigned char previousData,bitPos,side;
    yPos=64-yPos-1;
    bitPos=yPos -8*((unsigned char)(yPos/8));
    previousData=ReadData(yPos/8,xPos);
    previousData=ReadData(yPos/8,xPos);
    if(xPos<64) {WriteCmd(xSTART+yPos/8,0);WriteCmd(ySTART+xPos,0);side=0;}
    else {WriteCmd(xSTART+yPos/8,1);WriteCmd(ySTART+xPos-64,1);side=1;}

    switch(bitPos)
    {
        case 0:
                    if(value==0) WriteData(previousData&0b11111110,side);
                    else WriteData(previousData|0b00000001,side);
                    break;
        case 1:
                    if(value==0) WriteData(previousData&0b11111101,side);
                    else WriteData(previousData|0b00000010,side);
                    break;
        case 2:
                    if(value==0) WriteData(previousData&0b11111011,side);
                    else WriteData(previousData|0b00000100,side);
                    break;
        case 3:
                    if(value==0) WriteData(previousData&0b11110111,side);
                    else WriteData(previousData|0b00001000,side);
                    break;
        case 4:
                    if(value==0) WriteData(previousData&0b11101111,side);
                    else WriteData(previousData|0b00010000,side);
                    break;
        case 5:
                    if(value==0) WriteData(previousData&0b11011111,side);
                    else WriteData(previousData|0b00100000,side);
                    break;
        case 6:
                    if(value==0) WriteData(previousData&0b10111111,side);
                    else WriteData(previousData|0b01000000,side);
                    break;
        case 7:
                    if(value==0) WriteData(previousData&0b01111111,side);
                    else WriteData(previousData|0b10000000,side);
                    break;
        default:    break;
    }
 

}








void DrawSample(unsigned char xPos,unsigned char yPos)
{
    unsigned char bitPos,side,page;
     yPos=64-yPos-1;
    page=(yPos/(unsigned char)8);
    bitPos=yPos -8*page;
    if(xPos<64) {WriteCmd(xSTART+page,0);WriteCmd(ySTART+xPos,0);side=0;}
    else {WriteCmd(xSTART+page,1);WriteCmd(ySTART+xPos-64,1);side=1;}

    switch(bitPos)
    {
        case 0:     WriteData(0b00000001,side);
                    break;
        case 1:     WriteData(0b00000010,side);
                    break;
        case 2:     WriteData(0b00000100,side);
                    break;
        case 3:     WriteData(0b00001000,side);
                    break;
        case 4:     WriteData(0b00010000,side);
                    break;
        case 5:     WriteData(0b00100000,side);
                    break;
        case 6:     WriteData(0b01000000,side);
                    break;
        case 7:     WriteData(0b10000000,side);
                    break;
        default:    break;
    }
 

}




void DrawLine(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2)
{
  	unsigned char i,dxabs,dyabs,x,y,px,py;
	signed char sdx,sdy;
//----------Calculation for x cordinates-------------
	if(x2 > x1)
	{
		dxabs = x2 - x1;		/* the horizontal distance of the line */
		sdx = 1;
	}
	else if(x2 == x1)
	{
		dxabs = 0;		/* the horizontal distance of the line */
		sdx = 0;
	}
	else
	{
		dxabs = x1 = x2;		/* the horizontal distance of the line */
		sdx = -1;
	}
//----------Calculation for y cordinates-------------
	if(y2 > y1)
	{
		dyabs = y2 - y1;		/* the vertical distance of the line */
		sdy = 1;
	}
	else if(y2 == y1)
	{
		dyabs = 0;		/* the vertical distance of the line */
		sdy = 0;
	}
	else
	{
		dyabs = y1 - y2;		/* the vertical distance of the line */
		sdy = -1;
	}
//----------Actual plot of line starts here-------------
  x=dyabs>>1;
  y=dxabs>>1;
  px=x1;
  py=y1;

  if (dxabs>=dyabs) /* the line is more horizontal than vertical */
  {
    for(i=0;i<dxabs;i++)
    {
      y+=dyabs;
      if (y>=dxabs)
      {
        y-=dxabs;
        py+=sdy;
      }
      px+=sdx;
     SetPixel(px,py,1);
    }
  }
  else /* the line is more vertical than horizontal */
  {
    for(i=0;i<dyabs;i++)
    {
      x+=dxabs;
      if (x>=dyabs)
      {
        x-=dyabs;
        px+=sdx;
      }
      py+=sdy;
      SetPixel(px,py,1);
    }
  }
}



/**************************************************************************
 *  Circle plotting                                                       *
 *    draws a line using Bresenham's line-drawing algorithm, which uses   *
 *    no multiplication or division.                                      *
 **************************************************************************/
void DrawCircle(unsigned char x0, unsigned char y0, unsigned char radius)
{
        signed char f = 1 - radius;
        signed char ddF_x = 0;
        signed char ddF_y = -2 * radius;
        signed char x = 0;
        signed char y = radius;

        SetPixel(x0, y0 + radius,1);
        SetPixel(x0, y0 - radius,1);
        SetPixel(x0 + radius, y0,1);
        SetPixel(x0 - radius, y0,1);

        while(x < y)
        {
                if(f >= 0)
                {
                        y--;
                        ddF_y += 2;
                        f += ddF_y;
                }
                x++;
                ddF_x += 2;
                f += ddF_x + 1;
                SetPixel(x0 + x, y0 + y,1);
                SetPixel(x0 - x, y0 + y,1);
                SetPixel(x0 + x, y0 - y,1);
                SetPixel(x0 - x, y0 - y,1);
                SetPixel(x0 + y, y0 + x,1);
                SetPixel(x0 - y, y0 + x,1);
                SetPixel(x0 + y, y0 - x,1);
                SetPixel(x0 - y, y0 - x,1);
        }
}


void DrawXLine(unsigned char x1,unsigned char x2,unsigned char y)
{
    unsigned char temp;
    if(x1>x2){temp=x1;x1=x2;x2=temp;}
    for(x1;x1<=x2;x1++) SetPixel(x1,y,1);
}


void DrawYLine(unsigned char x,unsigned char y1,unsigned char y2)
{
     unsigned char temp;
    if(y1>y2){temp=y1;y1=y2;y2=temp;}
    for(y1;y1<=y2;y1++) SetPixel(x,y1,1);
}

void Draw64X64(rom const unsigned char *image,unsigned char side)
{
     int i,j;
     unsigned char x,y;
     x=xSTART;y=ySTART;
    for(i=0;i<8;i++)
    {
        WriteCmd(x,side);WriteCmd(ySTART,side);
        for(j=0;j<63;j++)
        {
            WriteData(image[i*64+j],side);
        }
    x=x+0x01;
    }
}

void Draw128X64(rom const unsigned char*image)
{
    int i,j;
     unsigned char x,y;
     x=xSTART;y=ySTART;
    for(i=0;i<8;i++)
    {
        WriteCmd(x,0);WriteCmd(ySTART,0);
        for(j=0;j<64;j++)
        {
            WriteData(image[i*128+j],0);
        }
        WriteCmd(x,1);WriteCmd(ySTART,1);
        for(j;j<128;j++)
        {
            WriteData(image[i*128+j],1);
        }
    x=x+0x01;
    }
}


void DrawFFTGraph(short inputData[])
{
  short inputValue;
  unsigned char xPos;
 unsigned char xByte, requiredY, y, pointer;
 unsigned char counter;
	// Scale the input data to 0-63 and perform the dampening of the display
	for (counter = 0; counter < 32; counter++)
	{
		// Scale the input data for the display (linear) x1 or x8
		//if (SWITCH0 == 0) inputValue = inputData[counter] * 8;
		inputValue = inputData[counter];
		if (inputValue > 181) inputValue = 181;

		// Apply a linear or logarithmic conversion on the data
		//if (SWITCH1 == 0) inputValue = (short)logTable[inputValue];
		inputValue = (short)linTable[inputValue];

		// Perform damping on the displayed output
		if (inputValue >= displayData[counter]) displayData[counter] = inputValue;
		else displayData[counter] -= 15;
		if (displayData[counter] < 0) displayData[counter] = 0;
	}

	// Our FFT animation speed is dependent on how fast the LCD can
	// be updated, so here we use a bargraph drawing routine which
	// is highly optimised to the manner in which the LCD is updated.
	
	for (y = 0; y < 8; y++)
	{
		// Move to the correct screen page

		// Left bank
		WriteCmd(y | 0b10111000,0 );
		WriteCmd(0b01000000,0);

		// Right bank
		WriteCmd( y | 0b10111000,1);
		WriteCmd(0b01000000,1);

		xPos = 0;

		// We only draw buckets 1 to 31 (bucket 0 is invalid)
		for (pointer = 0; pointer < 32; pointer++)
		{
			xByte = 0;
			requiredY = 63 - displayData[pointer];

			// Either fill the whole byte or
			// left shift according to the remainder of
			// the division to get the right number of pixels
			if (requiredY <= y * 8) xByte = 0b11111111;
			else if (requiredY / 8 <= y) xByte = 0b11111111 << (requiredY % 8);

			if (xPos < 64)  WriteData(xByte,0); // 1/3 of bar
			else  WriteData(xByte,1);
			xPos++;

			if (xPos < 64)  WriteData(xByte,0); // 1/3 of bar
			else  WriteData(xByte,1);
			xPos++;

			if (xPos < 64)  WriteData(xByte,0); // 1/3 of bar
			else  WriteData(xByte,1);
			xPos++;

			if (xPos < 64)  WriteData(0x00,0); // gap
			else  WriteData(0x00,1);
			xPos++;
		}
	}
}

/*
void AnalogDigitalLab(void)
{
    WriteCmd(xSTART+0x01,0);
    WriteCmd(ySTART+6,0);
    appendCharToLCD('A',0);
    WriteData(0x00,0);
    WriteCmd(xSTART+0x01,0);
    WriteCmd(ySTART+6,0);
    appendCharToLCD('N',0);
    WriteData(0x00,0);
    WriteCmd(xSTART+0x01,0);
    WriteCmd(ySTART+6,0);
    appendCharToLCD('A',0);
    WriteData(0x00,0);
    WriteCmd(xSTART+0x01,0);
    WriteCmd(ySTART+6,0);
    appendCharToLCD('L',0);
    WriteData(0x00,0);
    WriteCmd(xSTART+0x01,0);
    WriteCmd(ySTART+6,0);
    appendCharToLCD('O',0);
    WriteData(0x00,0);
    WriteCmd(xSTART+0x01,0);
    WriteCmd(ySTART+6,0);
    appendCharToLCD('G',0);
    WriteData(0x00,0);
    WriteData(0x00,0);
    WriteCmd(xSTART+0x01,0);
    WriteCmd(ySTART+6,0);
    appendCharToLCD('D',0);
    WriteData(0x00,0);
    WriteCmd(xSTART+0x01,0);
    WriteCmd(ySTART+6,0);
    appendCharToLCD('I',0);
    WriteData(0x00,0);
    WriteCmd(xSTART+0x01,0);
    WriteCmd(ySTART+6,0);
    appendCharToLCD('G',0);
    WriteData(0x00,0);
    WriteCmd(xSTART+0x01,0);
    WriteCmd(ySTART+6,0);
    appendCharToLCD('I',0);
    WriteData(0x00,0);
    WriteCmd(xSTART+0x01,0);
    WriteCmd(ySTART+6,0);
    appendCharToLCD('T',0);
    WriteData(0x00,0);
    WriteCmd(xSTART+0x01,0);
    WriteCmd(ySTART+6,0);
    appendCharToLCD('A',0);
    WriteData(0x00,0);
    WriteCmd(xSTART+0x01,0);
    WriteCmd(ySTART+6,0);
    appendCharToLCD('L',0);
    WriteData(0x00,0);
    WriteData(0x00,0);
    WriteCmd(xSTART+0x01,0);
    WriteCmd(ySTART+6,0);
    appendCharToLCD('L',0);
    WriteData(0x00,0);
    WriteCmd(xSTART+0x01,0);
    WriteCmd(ySTART+6,0);
    appendCharToLCD('A',0);
    WriteData(0x00,0);
    WriteCmd(xSTART+0x01,0);
    WriteCmd(ySTART+6,0);
    appendCharToLCD('B',0);
    WriteData(0x00,0);


    
}
*/
/*
void Demo_01(void)
{
    int i,j;
    unsigned char DATA1[]={ 0x00,0x00,0xFC,0x04,0x04,0x04,0x04,0xFF,0x04,0x04,0x04,0x04,0xFC,0x00,0x00,0x00,
0x00,0x00,0x03,0x01,0x01,0x01,0x01,0x3F,0x01,0x01,0x01,0x01,0x03,0x00,0x00,0x00,};//?   ???? + ????

   




    unsigned char DATA2[]={0x00,0x00,0xFF,0x01,0x05,0x25,0x25,0xFD,0x25,0xA5,0x25,0x05,0x01,0xFF,0x00,0x00,
0x00,0x00,0x3F,0x10,0x12,0x12,0x12,0x13,0x12,0x12,0x13,0x12,0x10,0x3F,0x00,0x00,};//?  ???? + ????



   
   
       
    WriteCmd(xSTART+0x01,0);   //move to second page in the left half side
    for(j=0;j<16;j++)
    {
        WriteData( DATA1[j],0);
    }
      
    WriteCmd(xSTART+0x02,0);
    WriteCmd(ySTART,0);
    for(j=0;j<16;j++)
    {
        WriteData( DATA1[j+16],0);
    }
       

    WriteCmd(xSTART+0x01,1);
    WriteCmd(ySTART,1);
    for(j=0;j<16;j++)
    {
        WriteData( DATA2[j],1);
    }

    WriteCmd(xSTART+0x02,1);
    WriteCmd(ySTART,1);
    for(j=0;j<16;j++)
    {
        WriteData( DATA2[j+16],1);
    }
    delay_s(1);
    LCD_Clear();
    delay_s(1);

}
*/
/*  Demo 02: Numerical test */

void Demo_02(unsigned short value)
{
    int j;
    unsigned short X1,X2,X3,X4;
    WriteCmd(xSTART+0x01,0);   //move to second page in the left half side
    WriteCmd(ySTART,0);
    X1=value/1000;
    X2=(value-X1*1000)/100;
    X3=(value-(value/100)*100)/10;
    X4=value-(value/10)*10;

    if(X1==0)
    {
          for(j=0;j<5;j++){WriteData(Character_ZERO[j],0);}
    }
    else
    {
    for(j=0;j<5;j++){ WriteData( Character_ONE[j],0);}
    }

    switch(X2)
    {
        case 0:for(j=0;j<5;j++){WriteData( Character_ZERO[j],0);} break;
        case 1:for(j=0;j<5;j++){WriteData( Character_ONE[j],0);} break;
        case 2:for(j=0;j<5;j++){WriteData( Character_TWO[j],0);} break;
        case 3:for(j=0;j<5;j++){WriteData( Character_THREE[j],0);} break;
        case 4:for(j=0;j<5;j++){WriteData( Character_FOUR[j],0);} break;
        case 5:for(j=0;j<5;j++){WriteData( Character_FIVE[j],0);} break;
        case 6:for(j=0;j<5;j++){WriteData( Character_SIX[j],0);} break;
        case 7:for(j=0;j<5;j++){WriteData( Character_SEVEN[j],0);} break;
        case 8:for(j=0;j<5;j++){WriteData( Character_EIGHT[j],0);} break;
        case 9:for(j=0;j<5;j++){WriteData( Character_NINE[j],0);} break;
        default: break;
    }

        switch(X3)
    {
        case 0:for(j=0;j<5;j++){WriteData( Character_ZERO[j],0);} break;
        case 1:for(j=0;j<5;j++){WriteData( Character_ONE[j],0);} break;
        case 2:for(j=0;j<5;j++){WriteData( Character_TWO[j],0);} break;
        case 3:for(j=0;j<5;j++){WriteData( Character_THREE[j],0);} break;
        case 4:for(j=0;j<5;j++){WriteData( Character_FOUR[j],0);} break;
        case 5:for(j=0;j<5;j++){WriteData( Character_FIVE[j],0);} break;
        case 6:for(j=0;j<5;j++){WriteData( Character_SIX[j],0);} break;
        case 7:for(j=0;j<5;j++){WriteData( Character_SEVEN[j],0);} break;
        case 8:for(j=0;j<5;j++){WriteData( Character_EIGHT[j],0);} break;
        case 9:for(j=0;j<5;j++){WriteData( Character_NINE[j],0);} break;
        default: break;
    }
  
       switch(X4)
    {
        case 0:for(j=0;j<5;j++){WriteData( Character_ZERO[j],0);} break;
        case 1:for(j=0;j<5;j++){WriteData( Character_ONE[j],0);} break;
        case 2:for(j=0;j<5;j++){WriteData( Character_TWO[j],0);} break;
        case 3:for(j=0;j<5;j++){WriteData( Character_THREE[j],0);} break;
        case 4:for(j=0;j<5;j++){WriteData( Character_FOUR[j],0);} break;
        case 5:for(j=0;j<5;j++){WriteData( Character_FIVE[j],0);} break;
        case 6:for(j=0;j<5;j++){WriteData( Character_SIX[j],0);} break;
        case 7:for(j=0;j<5;j++){WriteData( Character_SEVEN[j],0);} break;
        case 8:for(j=0;j<5;j++){WriteData( Character_EIGHT[j],0);} break;
        case 9:for(j=0;j<5;j++){WriteData( Character_NINE[j],0);} break;
        default: break;
    }

    


}

/*
void Demo_03(void)
 {

     SetPixel(7,0,1);  delay_s(1);
     SetPixel(7,1,1);  delay_s(1);
     SetPixel(7,2,1);  delay_s(1);
     SetPixel(7,3,1);  delay_s(1);
     SetPixel(7,4,1);  delay_s(1);
     SetPixel(7,5,1);  delay_s(1);
     SetPixel(7,6,1);  delay_s(1);
     SetPixel(7,7,1);  delay_s(1);
     delay_s(2);
     LCD_Clear();
     delay_ms(500);
    
    DrawXLine(0,127,0);
    DrawXLine(0,127,7);
    DrawXLine(0,127,15);
    DrawXLine(0,127,23);
    DrawXLine(0,127,31);
    DrawXLine(0,127,39);
    DrawXLine(0,127,47);
    DrawXLine(0,127,55);
    DrawXLine(0,127,63);

    DrawYLine(0,0,63);
    DrawYLine(15,0,63);
    DrawYLine(31,0,63);
    DrawYLine(47,0,63);
    DrawYLine(63,0,63);
    DrawYLine(79,0,63);
    DrawYLine(95,0,63);
    DrawYLine(111,0,63);
    DrawYLine(127,0,63);

}

 */



/*
void Demo_04(void)
{
    int j;
 while(1)
    {

 DrawLine(6,0,6,63);
  DrawLine(6,0,121,0);
     WriteCmd(xSTART,0);
    WriteCmd(ySTART,0);
    appendCharToLCD('Y',0);
        WriteCmd(xSTART+0x07,1);
    WriteCmd(ySTART+59,1);
    appendCharToLCD('X',1);
        delay_s(1);
    LCD_Clear();
    delay_s(1);
     WriteCmd(xSTART+0x01,0);   
    WriteCmd(ySTART,0);
    appendCharToLCD('A',0);WriteData(0x00,0);appendCharToLCD('B',0);WriteData(0x00,0);appendCharToLCD('C',0);WriteData(0x00,0);appendCharToLCD('D',0);WriteData(0x00,0);
    appendCharToLCD('E',0);WriteData(0x00,0);appendCharToLCD('F',0);WriteData(0x00,0);appendCharToLCD('G',0);WriteData(0x00,0);appendCharToLCD('H',0);WriteData(0x00,0);
    appendCharToLCD('I',0);WriteData(0x00,0);appendCharToLCD('J',0);WriteData(0x00,0);delay_s(1);
    
    WriteCmd(xSTART+0x01,1);
    WriteCmd(ySTART,1);
    appendCharToLCD('K',1);WriteData(0x00,1);appendCharToLCD('L',1);WriteData(0x00,1);appendCharToLCD('M',1);WriteData(0x00,1);appendCharToLCD('N',1);WriteData(0x00,1);
    appendCharToLCD('O',1);WriteData(0x00,1);appendCharToLCD('P',1);WriteData(0x00,1);appendCharToLCD('Q',1);WriteData(0x00,1);appendCharToLCD('R',1);WriteData(0x00,1);
    appendCharToLCD('S',1);WriteData(0x00,1);appendCharToLCD('T',1);WriteData(0x00,1);delay_s(1);
     WriteCmd(xSTART+0x02,0);
    WriteCmd(ySTART,0);
    appendCharToLCD('U',0);WriteData(0x00,0);appendCharToLCD('V',0);WriteData(0x00,0);appendCharToLCD('W',0);WriteData(0x00,0);appendCharToLCD('X',0);WriteData(0x00,0);
    appendCharToLCD('Y',0);WriteData(0x00,0);appendCharToLCD('Z',0);WriteData(0x00,0);delay_s(1);
     WriteCmd(xSTART+0x02,1);
    WriteCmd(ySTART,1);
    appendCharToLCD('O',1);WriteData(0x00,1);appendCharToLCD('1',1);WriteData(0x00,1);appendCharToLCD('2',1);WriteData(0x00,1);appendCharToLCD('3',1);WriteData(0x00,1);
    appendCharToLCD('4',1);WriteData(0x00,1);appendCharToLCD('5',1);WriteData(0x00,1);appendCharToLCD('6',1);WriteData(0x00,1);appendCharToLCD('7',1);WriteData(0x00,1);
    appendCharToLCD('8',1);WriteData(0x00,1);appendCharToLCD('9',1);WriteData(0x00,1);delay_s(1);
    WriteCmd(xSTART+0x03,0);
    WriteCmd(ySTART,0);
    appendCharToLCD('_',0);WriteData(0x00,0);appendCharToLCD('?',0);WriteData(0x00,0);appendCharToLCD('\'',0);WriteData(0x00,0);appendCharToLCD('\"',0);WriteData(0x00,0);
    appendCharToLCD('!',0);WriteData(0x00,0);appendCharToLCD(',',0);WriteData(0x00,0);appendCharToLCD('-',0);WriteData(0x00,0);appendCharToLCD('<',0);WriteData(0x00,0);
    appendCharToLCD('>',0);WriteData(0x00,0);appendCharToLCD(':',0);WriteData(0x00,0);delay_s(1);

    delay_s(1);
    LCD_Clear();
    delay_s(1);
    }
}

*/
/*
void Demo_05(void)
{
     int j;
 while(1)
    {

 DrawLine(6,0,6,63);
  DrawLine(6,0,121,0);
     WriteCmd(xSTART,0);
    WriteCmd(ySTART,0);
    appendCharToLCD('Y',0);
        WriteCmd(xSTART+0x07,1);
    WriteCmd(ySTART+59,1);
    appendCharToLCD('X',1);
        delay_s(1);
    LCD_Clear();
    delay_s(1);
     WriteCmd(xSTART+0x01,0);
    WriteCmd(ySTART,0);
    WriteChar(Character_a,0);WriteData(0x00,0);WriteChar(Character_b,0);WriteData(0x00,0);WriteChar(Character_c,0);WriteData(0x00,0);WriteChar(Character_d,0);WriteData(0x00,0);
    WriteChar(Character_e,0);WriteData(0x00,0);WriteChar(Character_f,0);WriteData(0x00,0);WriteChar(Character_g,0);WriteData(0x00,0);WriteChar(Character_h,0);WriteData(0x00,0);
    WriteChar(Character_i,0);WriteData(0x00,0);WriteChar(Character_j,0);WriteData(0x00,0);delay_s(1);

    WriteCmd(xSTART+0x01,1);
    WriteCmd(ySTART,1);
    WriteChar(Character_k,1);WriteData(0x00,1);WriteChar(Character_l,1);WriteData(0x00,1);WriteChar(Character_m,1);WriteData(0x00,1);WriteChar(Character_n,1);WriteData(0x00,1);
    WriteChar(Character_o,1);WriteData(0x00,1);WriteChar(Character_p,1);WriteData(0x00,1);WriteChar(Character_q,1);WriteData(0x00,1);WriteChar(Character_r,1);WriteData(0x00,1);
    WriteChar(Character_s,1);WriteData(0x00,1);WriteChar(Character_t,1);WriteData(0x00,1);delay_s(1);
     WriteCmd(xSTART+0x02,0);
    WriteCmd(ySTART,0);
    WriteChar(Character_u,0);WriteData(0x00,0);WriteChar(Character_v,0);WriteData(0x00,0);WriteChar(Character_w,0);WriteData(0x00,0);WriteChar(Character_x,0);WriteData(0x00,0);
    WriteChar(Character_y,0);WriteData(0x00,0);WriteChar(Character_z,0);
    delay_s(1);
    LCD_Clear();
    delay_s(1);
    }
    
}
*/
#endif
