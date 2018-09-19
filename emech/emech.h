#ifndef EMECH_H
#define EMECH_H


/*
 * fail macros
 */
#define fail(...) do { char *_fail_err = hax_mprintf(__VA_ARGS__); do { onexit } while(0); return _fail_err; } while(0)
#define chkret(ptr) do { char *_fail_err = ptr; if(_fail_err != NULL) { return _fail_err; } } while(0)
#define chkfail(ptr) do { char *_fail_err = ptr; if(_fail_err != NULL) { do { onexit } while(0); return _fail_err; } } while(0)
#define chkexit(ptr) do { char *_fail_err = ptr; if(_fail_err != NULL) { hax_fprintf(stderr, "%s\n", _fail_err); exit(1); } } while(0)
#define chkabort(ptr) do { char *_fail_err = ptr; if(_fail_err != NULL) { hax_fprintf(stderr, "%s\n", _fail_err); abort(); } } while(0)

#define chkbool(err) ({ char *_emech_err = err; free(_emech_err), _emech_err != null; })
#define chkwarn(err) ({ char *_emech_err = err; fprintf(sterr, "%s\n", _emech_err); free(_emech_err), _emech_err != null; })

#endif
