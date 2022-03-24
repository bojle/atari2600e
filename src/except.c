#include <stdlib.h>
#include <setjmp.h>
#include <stdio.h>
#include "except.h"

/*
 * ERROR
 */


typedef void (*except_fptr) (void);

static except_fptr except_tbl[32];

void except_tbl_init() {
	except_tbl[EXCEPT_IGNORE] = except_ignore;
}

void except_throw(enum except_t name) {
	except_tbl[name]();
}

/*
 * EXCEPTIONS
 */

void except_ignore() {
	printf("exception ignored\n");
	return;
}
