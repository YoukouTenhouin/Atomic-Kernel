/* keyboard.c --> 键盘中断处理服务 */
/* 暂时搁置 */
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
 * 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。 */

#include <isr.h>
#include <kstdlib/kio.h>
#include <assembly.h>
#include <kcommon.h>
#include <types.h>
#include <keyboard.h>

static u8i ShiftPressed = 0;
static u8i CtrlPressed = 0;
static u8i AltPressed = 0;
static u8i UpperCase = 0;
static u8i ScanCode;

static void
pln()
{
	static const char KeyMap[0x3a][2] = {
		/*00*/{0x0, 0x0}, {0x0, 0x0}, {'1', '!'}, {'2', '@'}, 
		/*04*/{'3', '#'}, {'4', '$'}, {'5', '%'}, {'6', '^'}, 
		/*08*/{'7', '&'}, {'8', '*'}, {'9', '('}, {'0', ')'},
		/*0c*/{'-', '_'}, {'=', '+'}, {'\b','\b'},{'\t','\t'},
		/*10*/{'q', 'Q'}, {'w', 'W'}, {'e', 'E'}, {'r', 'R'},
		/*14*/{'t', 'T'}, {'y', 'Y'}, {'u', 'U'}, {'i', 'I'},
		/*18*/{'o', 'O'}, {'p', 'P'}, {'[', '{'}, {']', '}'},
		/*1c*/{'\n','\n'},{0x0, 0x0}, {'a', 'A'}, {'s', 'S'},
		/*20*/{'d', 'D'}, {'f', 'F'}, {'g', 'G'}, {'h', 'H'},
		/*24*/{'j', 'J'}, {'k', 'K'}, {'l', 'L'}, {';', ':'},
		/*28*/{'\'','\"'},{'`', '~'}, {0x0, 0x0}, {'\\','|'}, 
		/*2c*/{'z', 'Z'}, {'x', 'X'}, {'c', 'C'}, {'v', 'V'}, 
		/*30*/{'b', 'B'}, {'n', 'N'}, {'m', 'M'}, {',', '<'},
		/*34*/{'.', '>'}, {'/', '?'}, {0x0, 0x0}, {'*', '*'},
		/*38*/{0x0, 0x0}, {' ', ' '} };
	if (ScanCode & 0x80)
		return;
	WriteToKeyBuff(KeyMap[ScanCode&0x7f][UpperCase]);
}



static void
ctl(void)
{
	CtrlPressed ^= 0x1;
}

static void
alt(void)
{
	AltPressed ^= 0x1;
}

static void
shf(void)
{
	ShiftPressed ^= 0x1;
	UpperCase ^= 0x1;
}

static void
cap(void)
{
	if (ScanCode & 0x80)
		return;
	UpperCase ^= 0x1;
}

static void
fun(void){
	asm volatile("int $0x80");
}

static void
unp(void){}

static void KeyboardCallback(RegistersType Regs)
{
	int com;
	void (*KeyWay[0x80])(void) = {
		/*00*/unp, unp, pln, pln, pln, pln, pln, pln,
		/*08*/pln, pln, pln, pln, pln, pln, pln, pln,
		/*10*/pln, pln, pln, pln, pln, pln, pln, pln,
		/*18*/pln, pln, pln, pln, pln, ctl, pln, pln,
		/*20*/pln, pln, pln, pln, pln, pln, pln, pln,
		/*28*/pln, pln, shf, pln, pln, pln, pln, pln,
		/*30*/pln, pln, pln, pln, pln, pln, shf, pln,
		/*38*/alt, pln, cap, fun, fun, fun, fun, fun,
		/*40*/fun, fun, fun, fun, fun, unp, unp, unp,
		/*48*/unp, unp, unp, unp, unp, unp, unp, unp,
		/*50*/unp, unp, unp, unp, unp, unp, unp, fun,
		/*58*/fun, unp, unp, unp, unp, unp, unp, unp,
		/*60*/unp, unp, unp, unp, unp, unp, unp, unp,
		/*68*/unp, unp, unp, unp, unp, unp, unp, unp,
		/*70*/unp, unp, unp, unp, unp, unp, unp, unp,
		/*78*/unp, unp, unp, unp, unp, unp, unp, unp,
	};
	com = 0;
	ScanCode = inb(0x60);
	(*KeyWay[ScanCode&0x7f])();
	outb(0x61,(com=inb(0x61))|0x80);
	outb(com&0x7f,0x61);
}

void InitKeyboard()
{
	RegisterInterruptHandler(33,&KeyboardCallback);
}
