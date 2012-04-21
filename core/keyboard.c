#include <isr.h>
#include <kstdlib/kio.h>
#include <assembly.h>

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
	WriteChar(KeyMap[ScanCode&0x7f][UpperCase]);
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
	UpperCase ^= 0x1;
}

static void
fun(void){}

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
		/*38*/alt, pln, unp, cap, fun, fun, fun, fun,
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
