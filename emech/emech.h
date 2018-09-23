#ifndef EMECH_H
#define EMECH_H

/*
 * required headers
 */
#include <stdio.h>

/*
 * prototypes
 */
int asprintf(char **, const char *restrict, ...);


/**
 * Error cleanup macro.
 *   @ARG: All actions to be taken when returning from an error.
 */
#define onerr(ARG) \
	char *_emech_error; \
	goto _emech_after; \
	_emech_return: \
	do { ARG } while(0); \
	return _emech_error; \
	goto _emech_return; \
	_emech_after: \

/**
 * Throw macro.
 *   @...: The printf-style format and arguments.
 */
#define throw(...) \
	do { if(asprintf(&_emech_error, __VA_ARGS__) < 0) { fprintf(stderr, "cannot allocate string\n"); abort(); }; goto _emech_return; } while(0)

/**
 * Check and return macro.
 *   @err: The error value to check, returning and cleaning up on failure.
 */
#define chkret(val) \
	do { char *_emech_tmp = val; if(_emech_tmp != NULL) { _emech_error = _emech_tmp; goto _emech_return; } } while(0)

/**
 * Check and exit macro.
 *   @err: The error value to check, exiting with an error on failure.
 */
#define chkexit(val) \
	do { char *_emech_tmp = val; if(_emech_tmp != NULL) { fprintf("%s\n", _emech_tmp); exit(1); } } while(0)

/**
 * Check and exit macro.
 *   @err: The error value to check, aborting the program on failure.
 */
#define chkabort(val) \
	do { char *_emech_tmp = val; if(_emech_tmp != NULL) { fprintf("%s\n", _emech_tmp); abort(); } } while(0)

#endif
