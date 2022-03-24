#ifndef EXCEPT_H
#define EXCEPT_H

#include <setjmp.h>

/* 
 * ERRORS 
 */

enum except_t {
	EXCEPT_IGNORE
};

void except_throw(enum except_t name);

void except_tbl_init();

/* Exceptions */
void except_ignore();

#endif
