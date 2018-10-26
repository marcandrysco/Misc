#include "../emech/emech.h"
#include "../mdbg/mdbg.h"
#include "../membuf/membuf.h"

#include "lex.h"

#include <stdarg.h>


/**
 * Create a parser.
 *   @file: The file.
 *   @path: Consumed. The path.
 *   @init: The initial size.
 *   @trim; Always trim option.
 *   &returns: The parser.
 */
struct lex_parse_t *lex_parse_new(FILE *file, char *path, uint32_t init, bool trim)
{
	uint32_t i;
	struct lex_parse_t *lex;

	lex = malloc(sizeof(struct lex_parse_t));
	lex->file = file;
	lex->buf = malloc(2 * init);
	lex->idx = 0;
	lex->size = init;
	lex->trim = trim;

	lex->path = path;
	lex->line = lex->col = 0;

	for(i = 0; i < init; i++)
		lex->buf[i] = fgetc(file);

	return lex;
}

/**
 * Delete a parser.
 *   @parser: The parser.
 */
void lex_parse_delete(struct lex_parse_t *parser)
{
	free(parser->buf);
	free(parser->path);
	free(parser);
}


/**
 * Open a parser.
 *   @parse: Out. The opened parser.
 *   @path: Consumed. The path.
 *   @init: The initial size.
 *   @trim; Always trim option.
 *   &returns: Error.
 */
char *lex_parse_open(struct lex_parse_t **parse, char *path, uint32_t init, bool trim)
{
	onerr( free(path); );

	FILE *file;

	file = fopen(path, "r");
	if(file == NULL)
		fail("Cannot open '%s' for reading. %s.", path, strerror(errno));

	*parse = lex_parse_new(file, path, init, trim);
	return NULL;
}

/**
 * Close a parser.
 *   @parser: The parser.
 */
void lex_parse_close(struct lex_parse_t *parse)
{
	fclose(parse->file);
	lex_parse_delete(parse);
}


/**
 * Resize the lexer buffer.
 *   @lex: The lexer
 *   @size: The new size.
 */
void lex_parse_resize(struct lex_parse_t *lex, uint32_t size)
{
	uint32_t i;
	int16_t *buf;

	buf = malloc(2 * size);

	memcpy(buf, lex->buf + lex->idx, 2 * (lex->size - lex->idx));
	memcpy(buf + lex->size - lex->idx, lex->buf,  2 * lex->idx);

	for(i = lex->size; i < size; i++)
		buf[i] = fgetc(lex->file);

	free(lex->buf);
	lex->idx = 0;
	lex->size = size;
	lex->buf = buf;
}


/**
 * Create an error message on the parser.
 *   @parse: The parser.
 *   @fmt: The printf-style format string.
 *   @...: The printf-style arguments.
 */
char *lex_parse_error(const struct lex_parse_t *restrict parse, const char *restrict fmt, ...)
{
	char *tmp, *err;
	va_list args;

	va_start(args, fmt);
	tmp = vmprintf(fmt, args);
	va_end(args);

	err = mprintf("%s:%u:%u: %s", parse->path, parse->line, parse->col, tmp);
	free(tmp);

	return err;
}


/**
 * Advance the lexer by reading into a buffer.
 *   @lex: The lexer.
 *   @buf: The output buffer.
 *   @cnt: The number of characters to read and the size of the buffer.
 */
void lex_read_buf(struct lex_parse_t *lex, void *buf, uint32_t cnt)
{
	char ch;
	uint32_t i;

	i = lex->idx;

	while(cnt-- > 0) {
		if(buf != NULL)
			*(uint8_t *)buf++ = lex->buf[i];

		ch = fgetc(lex->file);
		lex->buf[i] = ch;
		i = (i + 1) % lex->size;

		if(ch == '\n') {
			lex->line++;
			lex->col = 0;
		}
		else
			lex->col++;
	}

	lex->idx = i;
}

/**
 * Advance the lexer by reading into a string.
 *   @lex: The lexer.
 *   @buf: The output string.
 *   @cnt: The number of characters to read and the size of the buffer.
 */
void lex_read_str(struct lex_parse_t *lex, char *buf, uint32_t cnt)
{
	lex_read_buf(lex, buf, cnt);

	if(buf != NULL)
		buf[cnt] = '\0';
}

/**
 * Advance the lexer by reading into an allocated buffer.
 *   @lex: The lexer.
 *   @cnt: The number of characters to read and size of the buffer.
 *   &returns: The allocated buffer.
 */
void *lex_dup_buf(struct lex_parse_t *lex, uint32_t cnt)
{
	void *buf;

	buf = malloc(cnt);
	lex_read_buf(lex, buf, cnt);

	return buf;
}

/**
 * Advance the lexer by reading into an allocated buffer.
 *   @lex: The lexer.
 *   @cnt: The number of characters to read and size of the buffer.
 *   &returns: The null-terminated string.
 */
void *lex_dup_str(struct lex_parse_t *lex, uint32_t cnt)
{
	char *buf;

	buf = malloc(cnt + 1);
	lex_read_buf(lex, buf, cnt);

	return buf;
}

/**
 * Advance the lexer by skipping over characters.
 *   @lex: The lexer.
 *   @cnt: The number of characters.
 */
void lex_skip(struct lex_parse_t *lex, uint32_t cnt)
{
	lex_read_buf(lex, NULL, cnt);
}


/**
 * Trim whitespace.
 *   @lex: The lexer.
 *   &returns: The number of whitespace removed.
 */
uint32_t lex_parse_trim(struct lex_parse_t *lex)
{
	uint32_t cnt = 0;

	while(lex_isspace(lex_get(lex, 0))) {
		cnt++;
		lex_skip(lex, 1);
	}

	return cnt;
}


/**
 * Try to parse a keyword.
 *   @parse: The parser.
 *   @id: The identifier.
 *   @loc: Out. Optional. The location.
 *   &returns: True if parsed.
 */
bool lex_keyword_try(struct lex_parse_t *parse, const char *keyword, struct lex_loc_t *loc)
{
	uint32_t i;

	if(parse->trim)
		lex_parse_trim(parse);

	for(i = 0; keyword[i] != '\0'; i++) {
		if(lex_get(parse, i) != keyword[i])
			break;
	}

	if((keyword[i] != '\0') || lex_isalnum(lex_get(parse, i)))
		return false;

	if(loc != NULL)
		*loc = lex_loc(parse);

	lex_skip(parse, i);

	return true;
}


/**
 * Try to parse an identifier.
 *   @parse: The parser.
 *   @id: The identifier.
 *   @loc: Out. Optional. The location.
 *   &returns: True if parsed.
 */
bool lex_id_try(struct lex_parse_t *parse, char **id, struct lex_loc_t *loc)
{
	uint32_t i;

	if(parse->trim)
		lex_parse_trim(parse);

	if(!lex_isalpha(lex_get(parse, 0)))
		return false;

	i = 1;
	while(lex_isalnum(lex_get(parse, i)))
		i++;


	if(loc != NULL)
		*loc = lex_loc(parse);

	*id = lex_dup_str(parse, i);
	return true;
}

/**
 * Parse an identifier.
 *   @parse: The parser.
 *   @id: The identifier.
 *   @loc: Out. Optional. The location.
 *   &returns: Error.
 */
char *lex_id_get(struct lex_parse_t *parse, char **id, struct lex_loc_t *loc)
{
	if(lex_id_try(parse, id, loc))
		return NULL;
	else
		return lex_parse_error(parse, "Expected identifier.");
}


/**
 * Try to parse a symbol.
 *   @parse: The parser.
 *   @sym: The symbol.
 *   @loc: Out. Optional. The location.
 *   &returns: True if parsed.
 */
bool lex_sym_try(struct lex_parse_t *parse, const char *sym, struct lex_loc_t *loc)
{
	uint32_t i;

	if(parse->trim)
		lex_parse_trim(parse);

	for(i = 0; sym[i] != '\0'; i++) {
		if(lex_get(parse, i) != sym[i])
			break;
	}

	if((sym[i] != '\0') || lex_issym(lex_get(parse, i)))
		return false;

	if(loc != NULL)
		*loc = lex_loc(parse);

	lex_skip(parse, i);

	return true;
}

/**
 * Parse a symbol.
 *   @parse: The parser.
 *   @sym: The symbol.
 *   @loc: Out. Optional. The location.
 *   &returns: True if parsed.
 */
char *lex_sym_get(struct lex_parse_t *parse, const char *sym, struct lex_loc_t *loc)
{
	if(lex_sym_try(parse, sym, loc))
		return NULL;
	else
		return lex_parse_error(parse, "Expected '%s'.", sym);
}


/**
 * Parse a number.
 *   @parse: The lexer.
 *   @id: Out. The identifier.
 *   @type: Out. The number type.
 *   &returns: True if parsed.
 */
bool lex_num_try(struct lex_parse_t *parse, char **num, enum lex_num_e *type)
{
	uint32_t i;

	if(parse->trim)
		lex_parse_trim(parse);

	if(lex_get(parse, 0) == '0') {
		if(lex_get(parse, 1) == 'x') {
			i = 2;
			*type = lex_hex_v;
		}
		else if(lex_get(parse, 1) == 'b') {
			i = 2;
			*type = lex_bin_v;
		}
		else
			i = 1;

		while(lex_isalnum(lex_get(parse, i)))
			i++;

		*num = lex_dup_str(parse, i);
		return true;
	}
	else if(lex_isdigit(lex_get(parse, 0)) || (lex_get(parse, 0) == '.')) {
		bool flt = lex_get(parse, 0) == '.';

		i = 1;

		while(lex_isalnum(lex_get(parse, i)) || (lex_get(parse, i) == '.')) {
			if(lex_get(parse, i) == '.')
				flt = true;

			i++;
		}

		*type = flt ? lex_flt_v : lex_int_v;
		*num = lex_dup_str(parse, i);
		return false;
	}
	else
		return false;
}
