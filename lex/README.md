Lexigraphical Parser
====================


## API


### Parser Management

    struct lex_parse_t *lex_parse_new(FILE *file, char *path, uint32_t init)
    void lex_parse_delete(struct lex_parse_t *lex)

The `lex_parse_new` creates a new parser on an input `file`. The `path`
parameters is consumed and will be `free`d when the parser is deleted. The
`init` parameter specifies the initial size of the internal buffer. This
should be set to largest expected size of a lexeme in characters.

The `lex_parse_delete` function deletes the parser including the `path` given
to `lex_parse_new`.

    void lex_parse_resize(struct lex_parse_t *lex, uint32_t size)

The `lex_parse_resize` function resizes the internal buffer for parsing a
lexeme.

    char *lex_parse_error(const struct lex_parse_t *restrict parse, const char *restrict fmt, ...)

The `lex_parse_error` function produces a formatted error message prepended by
the `parse` location. The `fmt, ...` arguments exactly match the format and
arguments of the `printf` functions.


### Advancing the Parser

    void lex_read_buf(struct lex_parse_t *parse, void *buf, uint32_t cnt);
    void lex_read_str(struct lex_parse_t *parse, char *buf, uint32_t cnt);

The `lex_read_buf` reads `cnt` bytes of data from the parser, written to
`buf` as raw byes.

The `lex_read_buf` reads `cnt` bytes of data from the parser, written to
`buf` as characters.

    void *lex_dup_buf(struct lex_parse_t *parse, uint32_t cnt);
    void *lex_dup_str(struct lex_parse_t *parse, uint32_t cnt);

The `lex_dup_buf` reads `cnt`bytes of data from the parser, returning it as an
allocated buffer. The buffer must eventually be `free`d.

The `lex_dup_str` reads `cnt`bytes of data from the parser, returning it as an
allocated string. The string must eventually be `free`d.

    void lex_skip(struct lex_parse_t *lex, uint32_t cnt);

The `lex_skip` function skips `cnt` bytes and directly advances the parser.


### Directly Parsing Lexemes


