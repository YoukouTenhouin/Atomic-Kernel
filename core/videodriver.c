/* videodriver.c --> 显卡驱动的实现 */
/* Copyright (c) 1998 著作权由Chapaev所有。著作权人保留一切权利。
 * 
 * 这份授权条款，在使用者符合以下三条件的情形下，授予使用者使用及再散播本
 * 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然：
 * 
 * * 对于本软件源代码的再散播，必须保留上述的版权宣告、此三条件表列，以
 *   及下述的免责声明。
 * * 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或者其他附
 *   于散播包装中的媒介方式，重制上述之版权宣告、此三条件表列，以及下述
 *   的免责声明。
 * * 未获事前取得书面许可，不得使用本软件贡献者之名称，
 *   来为本软件之衍生物做任何表示支持、认可或推广、促销之行为。
 * 
 * 免责声明：本软件是由本软件之贡献者以现状（"as is"）提供，
 * 本软件包装不负任何明示或默示之担保责任，包括但不限于就适售性以及特定目
 * 的的适用性为默示性担保。加州大学董事会及本软件之贡献者，无论任何条件、
 * 无论成因或任何责任主义、无论此责任为因合约关系、无过失责任主义或因非违
 * 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
 * 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
 * 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
 * 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。*/

#include <driver/video/videodriver.h>
#include <types.h>
#include <kstdlib/algorithm.h>
#include <assembly.h>
#include <kstdlib/string.h>
#include <timer.h>

static s32i PositionX = 0,PositionY = 0;
static u8i* VideoRam = (u8i*) 0xb8000;
static s8i Color;

//Doesn't work
int
FindFirstCleanLine()
{
	for( int i = 0 ;i < 25 ; i++ )
		for ( int j = 0; j < 80 ; j+=2 ){
			if ( VideoRam[j] != 0 ) break;
			else if ( j == 158 ) return i;
		}
	PositionY = 24;
	NewLine();
}

void
VideoInit()
{
	ClearScreen();
}

void
ClearScreen()
{
	memset(0xb8000,0,80*25*2);
	PositionY = PositionX = 0;
}

void
ScrollScreen()
{
	memcpy(0xb8000,0xb8000+160,80*24*2);
	memset(0xb8000+80*24*2,0,80*2);
	PositionX = 0;
	PositionY--;
}

void
WriteHex(u32i n) //直接从jamesm那复制的
{
    s32i tmp;

    WriteString("0x");

    char noZeroes = 1;

    int i;
    for (i = 28; i > 0; i -= 4)
    {
	    tmp = (n >> i) & 0xF;
        if (tmp == 0 && noZeroes != 0)
        {
            continue;
        }
    
        if (tmp >= 0xA)
        {
            noZeroes = 0;
            WriteChar (tmp-0xA+'A' );
        }
        else
        {
            noZeroes = 0;
            WriteChar( tmp+'0' );
        }
    }
  
    tmp = n & 0xF;
    if (tmp >= 0xA)
    {
        WriteChar (tmp-0xA+'A');
    }
    else
    {
        WriteChar (tmp+'0');
    }

}

void
WriteDec(u32i Number)
{
	WriteString(NumberToString(Number,10));
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
	switch(A) {
	case '\n':
		NewLine();
		break;
	case '\b':
		PositionX--;
		WriteChar(0);
		PositionX--;
		break;
	case '\t':
		for( int i = 0 ; i != 8 ; i++ )
			WriteChar(' ');
		break;
	default:
		VideoRam[PositionY*80*2+PositionX*2] = A;
		VideoRam[PositionY*80*2+PositionX*2+1] = Color;
		PositionX++;
		if (PositionX > 80) NewLine();
		break;
	}
	UpdateCursor();
	if (PositionY >= 25) ScrollScreen();
}

void
SetColor(COLOUR BG,COLOUR FG)
{
	Color = (s8i) (BG<<4|FG);
}

void
NewLine()
{
	VideoRam[PositionX*2+PositionY*80*2+1] = Color;
	if ( PositionY < 25 ) {
		PositionY++;
		PositionX = 0;
	} else {
		ScrollScreen();
	}
}

void GetPosition(s32i* X,s32i* Y)
{
	*X=PositionX;
	*Y=PositionY;
}

void SetPosition(s32i X,s32i Y)
{
	PositionX=X;
	PositionY=Y;
}

void UpdateCursor()
{
	VideoRam[PositionX*2+PositionY*80*2+1] = WHITE<<4|BLACK;
	VideoRam[PositionX*2+PositionY*80*2+3] = Color;
}

void MoveCursor(s32i Offset)
{
	PositionX += Offset;
	if ( PositionX < 0 ) {
		PositionY--;
		PositionX = 80 - PositionX;
	}

}
