#include <stdio.h>
#include "except.h"
#include "log.h"
#include "mspace.h"

int main() {
	except_tbl_init();
	log_trace("Log trace in main %d %s", 67, "hello");
	except_throw(EXCEPT_IGNORE);
}
