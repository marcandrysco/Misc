Lexigraphical Parser
====================

The lexigraphical parser is a library for partitioning an input stream of
data, either text or binary, into tokens (also called lexemes). The parser
uses an ordered set of rules in for producing tokens from the input. The
library also provides a common set of rules to make writing a parser even
easier.


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

    struct lex_token_t *lex_parse_next(struct lex_parse_t *parse);
    struct lex_token_t *lex_parse_token(struct lex_parse_t *parse, int id, uint32_t len);
    void lex_parse_resize(struct lex_parse_t *lex, uint32_t size)

The `lex_parse_next` function reads the next token from the parser.

The `lex_parse_token` function reads `len` bytes from the parser, and creates
a token using the `id`.

The `lex_parse_resize` function resizes the internal buffer for parsing a
lexeme.

    char *lex_parse_error(const struct lex_parse_t *restrict parse, const char *restrict fmt, ...)

The `lex_parse_error` function produces a formatted error message prepended by
the `parse` location. The `fmt, ...` arguments exactly match the format and
arguments of the `printf` functions.


### Parser Access

    int lex_char(struct lex_parse_t *parse, uint32_t i)
    int lex_read(struct lex_parse_t *parse)
    void lex_skip(struct lex_parse_t *parse, uint32_t cnt)

The `lex_char` function retrieves the `i`th character on the input. The parser
will buffer as many characters as needed.

    static inline void lex_adv(struct lex_parse_t *parse)

The `lex_adv` function advances the parser by a single token.

    struct lex_token_t *lex_get(struct lex_parse_t *parse, uint32_t idx)

The `lex_token` function retrieves the `i`th token from the parser. The parser
will buffer as many tokens as needed.

    struct lex_token_t *lex_peek(struct lex_parse_t *parse)
    struct lex_token_t *lex_try(struct lex_parse_t *parse, int32_t id)

The `lex_peek` function returns the top-most token without advancing the
parser.

The `lex_next` function returns the top-most token and advances the parser by
one.

The `lex_try` function checks if the top-most token is


### Parsing Rules

The rules are used to read input and produce the proper token. The rules are
processed in the order that they are added to the parser.

    void lex_rule_add(struct lex_parse_t *parse, struct lex_token_t *(*func)(struct lex_parse_t *, const void *), const void *arg);

The `lex_rule_add` function adds a rule to the parser. The rule function
`func` is called to in attempt to parse a token. If successful, the function
will return the token, otherwise it returns null.

    void lex_rule_keyword(struct lex_parse_t *parse, uint32_t cnt, const struct lex_keyword_t *keyword);
    void lex_rule_symbol(struct lex_parse_t *parse, uint32_t cnt, const struct lex_keyword_t *symbol);
    void lex_rule_ident(struct lex_parse_t *parse, int id);

