#include <driver/video/videodriver.h>
#include <types.h>
#include <kstdlib/algorithm.h>

static s32i PositionX = 0,PositionY = 0;
static u8i* VideoRam = (u8i*) 0xb8000;
static s8i Color;

void
ClearScreen()
{
	for( int i = 1 ; i != 80*25 ; i++ )
		WriteChar(' ');
	PositionX = 0;
	PositionY = 0;
}


void
ScrollScreen()
{
	u8i* P1 = (u8i*) 0xb8000;
	u8i* P2 = (u8i*) 0xb8000;
	P2+=160;
	for (int i = 0; i != 80*25*2-160; i++) {
		*P1=*P2;
		P1++;
		P2++;
	}
	while (P1!=P2) {
		*P1 = 0;
		P1++;
		*P1 = 0x07;
		P1++;
	}
}

void
WriteNumber(s32i Number,s32i Base)
{
	WriteString(NumberToString(Number,Base));
}

void
WriteString(char* String)
{
	for( int i = 0 ; String[i] != '\0' ; i++ )
		WriteChar(String[i]);
}

		       
void
WriteChar(char A)
{
	if ( A == '\n' )
		NewLine();
	else {
	VideoRam[PositionY*80*2+PositionX*2] = A;
	VideoRam[PositionY*80*2+PositionX*2+1] = Color;
	PositionX++;
	}
}

void
SetColor(COLOUR BG,COLOUR FG)
{
	Color = (s8i) (BG<<4|FG);
}

void
NewLine()
{
	if ( PositionY != 25 ) {
		PositionY++;
		PositionX = 0;
	} else {
		ScrollScreen();
	}
}
