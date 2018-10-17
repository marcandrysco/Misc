#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../mdbg/mdbg.h"
#include "../membuf.h"

void *read_full_file(const char *path, uint64_t *len);

int main()
{
	void *buf;
	uint64_t len;

	realloc(malloc(2), 4);
	return 0;

	buf = read_full_file("infile", &len);
	fwrite(buf, len, 1, stdout);

	return 0;
}

void *read_full_file(const char *path, uint64_t *len)
{
	FILE *file = fopen(path, "r");
	assert(file != NULL);

	int ch;
	struct membuf_t buf = membuf_init(256);
	while((ch = fgetc(file)) != EOF)
		membuf_addu8(&buf, ch);

	return membuf_done(&buf, len);
}
