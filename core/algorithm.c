#include <kstdlib/algorithm.h>

//从网上照搬的算法
char*
NumberToString(s32i n,s32i base)
{
	register char *p;
	register int minus;
	static char buf[36];
	p = &buf[36];
	*--p = '\0';
	if (n < 0) {
		minus = 1;
		n = -n;
	}
	else
		minus = 0;
	if (n == 0)
		*--p = '0';
	else
		while (n > 0) {
			*--p = "0123456789abcdef"[n % base];
			n /= 10;
		}
	if (minus)
		*--p = '-';
	return p;
}

