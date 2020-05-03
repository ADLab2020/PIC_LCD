#ifndef _FONT_
#define _FONT_

#include "GenericTypeDefs.h"

typedef struct FONT_DEF
{
	UINT8 u8Width;				/* Character width for storage         */
	UINT8 u8Height;				/* Character height for storage        */
	const UINT8 *au8FontTable;	/* Font table start address in memory  */
} FONT_DEF;

extern const FONT_DEF Font_System3x6;
extern const FONT_DEF Font_System5x8;
extern const FONT_DEF Font_System7x8;
extern const FONT_DEF Font_Courrier8x12;

#endif
