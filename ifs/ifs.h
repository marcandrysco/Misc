#ifndef IFS_H
#define IFS_H

/*
 * required headers
 */
#include <stdbool.h>
#include <stdint.h>

/*
 * file tests
 */
bool ifs_exists(const char *path);

char *ifs_mkdir(const char *path, uint8_t mode);

/*
 * path declarations
 */
char *ifs_dirname(const char *path);

#endif
