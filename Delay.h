/* 
 * File:   Delay.h
 * Author: sony
 *
 * Created on 2012?10?20?, ??9:58
 */

#ifndef DELAY_H
#define	DELAY_H


#define delay_us(n)       \
{	char a=n;  \
    _asm                  \
	MOVLW a               \
    MOVF PLUSW2, 1, 0     \
	BZ   12               \
	DECF PLUSW2, 1, 0     \
loop:                     \
	MOVLW a               \
    MOVF PLUSW2, 1, 0     \
	BZ   8                \
	NOP                   \
	NOP                   \
	NOP                   \
	NOP                   \
	NOP                   \
	NOP                   \
	DECF PLUSW2, 1, 0     \
	BRA  -11              \
end:                      \
	_endasm }

void delay_100us(int n);
void delay_ms(int n);
void delay_s(int n);


#endif	/* DELAY_H */

