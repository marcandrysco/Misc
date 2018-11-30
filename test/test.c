#include "../misc.h"


/*
 * external declarations
 */
void inline_test();
void isys_test();


int main()
{
	setbuf(stdout, NULL);

	inline_test();
	isys_test();

	return 0;
}
