#ifndef WINDOWS_H
#define WINDOWS_H


enum isys_fd_e {
	isys_handle_v,
};

union isys_fd_u {
	HANDLE handle;
};
struct isys_fd_t {
	enum isys_fd_e type;
	union isys_fd_u data;
};


typedef struct isys_fd_t isys_fd_t;

#endif
