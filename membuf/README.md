Memory Buffer
=============

The memory buffer library provides a very simple API for accumulating data
into a sequential memory buffer. This should only be used if you do not know
the size of your data beforehand, such as parsing (otherwise, you should
simply allocate a fixed size buffer).


## API

THe memory buffer is a simple data structure that accumulates data written.
Although the buffer is set to an initial size, the internal data store will
dynamically resize itself to if the buffer is filled.

    struct membuf_t;

Memory buffers consist of the opaque `membuf_t` structure.

    struct membuf_t membuf_init(uint64_t init)
    void membuf_destroy(struct membuf_t buf)

The `membuf_init` function initializes the memory buffer, allocating an
initial buffer of `init` bytes. The buffer will expand after filling more than
`init` bytes.

The `membuf_destroy` function destroys the memory buffer, freeing the contents
of the buffer. If you are finished with the memory buffer and wish to use the
accumulated data, use `membuf_done`.

    void *membuf_done(struct membuf_t buf, uint64_t *len);

The `membuf_done` function returns a pointer to the allocated internal buffer
and renders the buffer `buf` invalid. The returned data is allocated on the
heap and must eventually be `free`d to prevent a memory leak. The parameter
`len` stores the total length of the buffer; the value may be set to `NULL` to
ignore the buffer length.

    void membuf_addu8(struct membuf_t *buf, uint8_t val)
    void membuf_addbuf(struct membuf_t *restrict buf, const void *restrict data, uint64_t nbytes)

The `membuf_addu8` function adds a single unsigned, 8-bit integer to the
memory buffer.

The `membuf_addbuf` function adds data to the memory buffer. The data is
pointed to by `data` and exactly `nbytes` long.

    void membuf_addch(struct membuf_t *buf, uint8_t val)
    void membuf_addstr(struct membuf_t *restrict buf, const char *restrict str)

The `membuf_addch` function adds a single character to the memory buffer.

The `membuf_addstr` function adds a null-terminated string to the memory
buffer, excluding the null terminating byte. To add a null terminating byte,
call `membuf_addch` with an explicit value of `'\0'`.


## Examples

### Ex1. Reading an entire file into a buffer.

    void *read_full_file(const char *path, uint64_t *len) {
      FILE *file = fopen(path, "r");
      assert(file != NULL);

      int ch;
      struct membuf_t buf = membuf_init(256);
      while((ch = fgetc(file)) != EOF)
        membuf_addu8(&buf, ch);

      return membuf_done(&buf, len);
    }
