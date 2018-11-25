#pragma once

/*
 * common headers
 */
#include <stdio.h>

/*
 * checking macro for testing a boolean.
 */
#define check(cond, nfails) \
	do { if(!(cond)) { fprintf(stderr, "%s:%u: failed '%s'\n", __FILE__, __LINE__, __func__); (*nfails)++; } } while(0)
