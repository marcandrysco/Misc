#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <sys/stat.h>

#include "posix.h"
#include "ifs.h"
#include "../mdbg/mdbg.h"


/**
 * Create a directory.
 *   @path: The path.
 *   @mode: The mode.
 *   &returns: Error.
 */
char *ifs_mkdir(const char *path, uint8_t mode)
{
	if(mkdir(path) < 0)
		return mprintf("Failed to create directory '%s'. %s.", path, strerror(errno));

	if(chmod(path, mode) < 0)
		return mprintf("Failed to set permissions on '%s'. %s.", path, strerror(errno));

	return NULL;
}
