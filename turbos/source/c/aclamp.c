#ifdef TURBOS
#include <cmoc.h>
#include "turbos.h"
#elif FREERTOS
#else
#include <stdio.h>
#endif

/*
 * Audio clamp.
 *
 * Example 1:
 * 12 bytes of input:  AFIRSVBHIOWL, lower clamp value: C, upper clamp value, M
 * 12 bytes of output: CFIMMMCCCMML
 */

int aclamp(unsigned char *src, unsigned char *dst, int src_size, unsigned char lcv, unsigned char ucv) {    
    for (int i = 0; i < src_size; i++) {
        // Compare the byte.
        if (src[i] < lcv) {
	   		dst[i] = lcv;
        } else if (src[i] > ucv) {
	   		dst[i] = ucv;
        } else {
	   		dst[i] = src[i];
	   }
    }
    
    return src_size;
}

void aclamp_task() {
	unsigned char src[12] = {1, 6, 9, 18, 19, 21, 2, 8,
						9, 15, 23, 12};

	unsigned char dst[12];
	
	aclamp(src, dst, 12, 3, 13);
}

#ifdef TURBOS
interrupt void handler(void) {
	aclamp_task();
}

int main(int argc, char **argv) {
	f_icpt(handler);
	
	// Sleep forever and wait for a signal to drive the routine.
	while (1) {
		aclamp_task();
//		f_sleep(10);
	}
	
	return 0;
}
#elif FREERTOS
void aclamp_freertos(void *parameters) {
	while (1) {
		aclamp_task();
//		vTaskDelay(100);
	}
}
#else

/* Unit tests on POSIX */

void reportStatus(const char *function, int status) {
	if (status >= 0) {
		printf("%s: SUCCESS!\n", function);
	} else {
		printf("%s: FAIL!\n", function);
	}
}

int test1() {
	int status = 0;

	unsigned char src[12] = {1, 6, 9, 18, 19, 21, 2, 8,
						9, 15, 23, 12};

	unsigned char clamped_test[12] = {3, 6, 9, 13, 13, 13, 3, 8,
						9, 13, 13, 12};

	unsigned char dst[12];
	
	aclamp(src, dst, 12, 3, 13);

	for (int i = 0; i < 12; i++) {
		if (dst[i] != clamped_test[i]) {
			status = -1;
			break;
		}
	}

	reportStatus(__func__, status == -1);	
	
	return status;
}

int main(int argc, char **argv) {
	// Perform unit testing.
	test1();
}
#endif
