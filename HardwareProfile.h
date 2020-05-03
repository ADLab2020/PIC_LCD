/*
 * File:   HardwareProfile.h
 * Author: Analogdigitallab
 * Website: http://www.analogdigitallab.org/
 *
 * Created on 2012/10/20, 21:55
 */

#ifndef HARDWAREPROFIEL_H
#define	HARDWAREPROFIEL_H
#include <p18f4550.h>
#include "Delay.h"

// Fosc frequency (48 Mhz)
#define _XTAL_FREQ 48000000


// Constants
#define OFF		0
#define ON		1
#define TOGGLE	2

#define TRUE	1
#define FALSE	0

#define READ	1
#define WRITE	0

#define HIGH    1
#define LOW     0
#define PRESSED 0
#define INPUT   1
#define OUTPUT  0



// Data types
#define BYTE unsigned char



// Hardware mapping definitions
// gLCD character display hardware

#define	DB0            PORTDbits.RD0
#define DB1            PORTDbits.RD1
#define DB2            PORTDbits.RD2
#define DB3            PORTDbits.RD3
#define	DB4            PORTDbits.RD4
#define DB5            PORTDbits.RD5
#define DB6            PORTDbits.RD6
#define DB7            PORTDbits.RD7


#define DATA_BUS		PORTD
#define DATA_DIRECTION	TRISD

#define CS1            PORTBbits.RB7
#define Tris_CS1       TRISBbits.TRISB7
#define CS2            PORTBbits.RB6
#define Tris_CS2       TRISBbits.TRISB6
#define RW             PORTBbits.RB5
#define Tris_RW        TRISBbits.TRISB5
#define RS             PORTBbits.RB4
#define Tris_RS        TRISBbits.TRISB4
#define ENABLE            PORTBbits.RB3
#define Tris_EN        TRISBbits.TRISB3

//Buttons
#define Button1         PORTEbits.RE0
#define Button1_Tris    TRISEbits.TRISE0


#endif	/* HARDWAREPROFIEL_H */

