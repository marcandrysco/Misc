#ifndef LEX_H
#define LEX_H

/*
 * common header
 */
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>


/**
 * Number type enumerator.
 *   @lex_int_v: Decimal integer.
 *   @lex_hex_v: Hexidecimal integer.
 *   @lex_oct_v: Octal integer.
 *   @lex_bin_v: Binary integer.
 *   @lex_flt_v: Floating-point number.
 */
enum lex_num_e {
	lex_int_v,
	lex_hex_v,
	lex_oct_v,
	lex_bin_v,
	lex_flt_v,
};

/**
 * File location.
 *   @path: The path.
 *   @line, col: The line and column.
 */
struct lex_loc_t {
	const char *path;
	uint32_t line, col;
};

/**
 * Lexer structure.
 *   @file: The file.
 *   @buf: The text buffer.
 *   @idx, size: The current index and size.
 *   @path: The path.
 *   @line, col: The line and column.
 *   @trim: The always trim option.
 */
struct lex_parse_t {
	FILE *file;
	int16_t *buf;
	uint32_t idx, size;

	char *path;
	uint32_t line, col;

	bool trim;
};


/*
 * parser declarations
 */
struct lex_parse_t *lex_parse_new(FILE *file, char *path, uint32_t init, bool trim);
void lex_parse_delete(struct lex_parse_t *parser);

char *lex_parse_open(struct lex_parse_t **parse, char *path, uint32_t init, bool trim);
void lex_parse_close(struct lex_parse_t *parse);

void lex_parse_resize(struct lex_parse_t *lex, uint32_t size);

char *lex_parse_error(const struct lex_parse_t *restrict parse, const char *restrict fmt, ...) __attribute__ ((format (printf, 2, 3)));

/*
 * reading declarations
 */
void lex_read_buf(struct lex_parse_t *lex, void *buf, uint32_t cnt);
void lex_read_str(struct lex_parse_t *lex, char *buf, uint32_t cnt);

void *lex_dup_buf(struct lex_parse_t *lex, uint32_t cnt);
void *lex_dup_str(struct lex_parse_t *lex, uint32_t cnt);

void lex_skip(struct lex_parse_t *lex, uint32_t cnt);

/*
 * token parsing declarations
 */
uint32_t lex_parse_trim(struct lex_parse_t *lex);

bool lex_keyword_try(struct lex_parse_t *lex, const char *keyword, struct lex_loc_t *loc);

bool lex_id_try(struct lex_parse_t *parse, char **id, struct lex_loc_t *loc);
char *lex_id_get(struct lex_parse_t *parse, char **id, struct lex_loc_t *loc);

bool lex_sym_try(struct lex_parse_t *parse, const char *sym, struct lex_loc_t *loc);
char *lex_sym_get(struct lex_parse_t *parse, const char *sym, struct lex_loc_t *loc);

bool lex_num_try(struct lex_parse_t *parse, char **num, enum lex_num_e *type);


/**
 * Retrieve the next input from the lexer.
 *   @lex: The lexer.
 *   @i: The index.
 *   &returns: The next input.
 */
static inline int lex_get(struct lex_parse_t *parse, uint32_t i)
{
	if(i >= parse->size)
		lex_parse_resize(parse, 2 * i);

	return parse->buf[(parse->idx + i) % parse->size];
}

/**
 * Check if at the end-of-file.
 *   @parse: The parser.
 *   &returns: True if end-of-file.
 */
static inline bool lex_eof(struct lex_parse_t *parse)
{
	return lex_get(parse, 0) == EOF;
}
static inline struct lex_loc_t lex_loc(const struct lex_parse_t *parse)
{
	return (struct lex_loc_t){ parse->path, parse->line, parse->col };
}


/**
 * Check if a character is alphabetical or unicode.
 *   @ch: The character.
 *   &returns: True if alphabetical or unicode.
 */
static inline bool lex_isalpha(char ch)
{
	uint8_t byte = ch;

	return ((byte >= 'A') && (byte <= 'Z')) || ((byte >= 'a') && (byte <= 'z')) || (byte >= 128);
}

/**
 * Check if a character is a digit.
 *   @ch: The character.
 *   &returns: True if digit.
 */
static inline bool lex_isdigit(char ch)
{
	uint8_t byte = ch;

	return (byte >= '0') && (byte <= '9');
}

/**
 * Check if a character is alphabetical, unicode, or digit.
 *   @ch: The character.
 *   &returns: True if alphabetical, unicode, or digit.
 */
static inline bool lex_isalnum(char ch)
{
	return lex_isalpha(ch) || lex_isdigit(ch);
}

/**
 * Check if a character is any whitespace character.
 *   @ch: The character.
 *   &returns: True if whitespace.
 */
static inline bool lex_isspace(char ch)
{
	return (ch == ' ') || (ch == '\t') || (ch == '\v') || (ch == '\f') || (ch == '\r') || (ch == '\n');
}

/**
 * Check if a character is any symbol. This excludes brackets, parenthesis, or
 * braces.
 *   @ch: The character.
 *   &returns: True if symbol.
 */
static inline bool lex_issym(int ch)
{
	return (ch == '~') || (ch == '|') || (ch == '&') || (ch == '^') || (ch == '+') || (ch == '-') || (ch == '*') || (ch == '/') || (ch == '.') || (ch == ',') || (ch == '=') || (ch == '>') || (ch == '<') || (ch == '?');
}

#endif
