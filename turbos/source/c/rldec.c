#ifdef TURBOS
#include <cmoc.h>
#include "turbos.h"
#elif FREERTOS
#else
#include <stdio.h>
#endif

/*
 * Run-length decoder.
 * 
 * This function decodes a buffer of run-length encoded data.
 *
 * Parameters:
 *	src = The source encoded data plus a terminal (nil) byte.
 *	dst = The destination decoded data.
 *  dsz = The size of the destination buffer.
 *
 * Returns: 0 if decode buffer fully accomodated the decoded data; or -1 if not.
 *
 * Example 1:
 * 12 bytes of input: [1]A[1]B[1]A[3]C[1]A[1]B
 *  8 bytes of output:  ABACCCABA
 *
 * Example 2:
 * 12 bytes of input: [5]D[1]A[1]C[1]D[5]B[3]D
 * 16 bytes of output:  DDDDDACDBBBBBDDD
 */

int rldec(unsigned char *src, unsigned char *dst, int dsz)
{
	int status = 0;
	int decode_size = 0;
	int i = 0;
	
	while (1) {
		// Get the count.
		unsigned char count = src[i];
		
		// If the count is 0, we've reached the end of the source buffer.
		if (count == 0) {
			break;
		}
	   
		// Verify that the count plus current decode size is less than or
		// equal to the destination buffer size.
		if (count + decode_size > dsz) {
			// It's greater; the destination buffer can't accommodate.
			status = -1;
			break;
		}
		i++;
		unsigned char value = src[i];
		for (int j = 0; j < count; j++) {
			*dst = value;
			dst++;
			decode_size++;
		}
		i++;
    }
    
    return status;
}

void rldec_task(void) {
	unsigned char src[33] = {1, 1, 2, 2, 1, 4, 4, 4,
						4, 4, 4, 12, 13, 13, 13, 13,
						4, 4, 4, 12, 13, 13, 13, 13,
						4, 4, 4, 12, 13, 13, 13, 13, 0};
	unsigned char dst[256];					

	rldec(src, dst, 256);
}

#ifdef TURBOS
interrupt void sighandler(void) {
	rldec_task();
}

int main(int argc, char **argv) {
	f_icpt(sighandler);
	
	// Sleep forever and wait for a signal to drive the routine.
	while (1) {
		f_sleep(0);
	}
	
	return 0;
}
#elif FREERTOS
void rldec_freertos(void *parameters) {
	while(1) {
		rldec_task();
		vTaskDelay(100);
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

/*
 * Returns the decoded size of an run-length encoded buffer.
 */
int decode_size(unsigned char *buffer) {
	int result = 0;
	int i = 0;
	
	while (1) {
		if (buffer[i] == 0) {
			break;
		}
		result = result + buffer[i];
		i += 2;
	}
	
	return result;
}

int test1() {
	int status = 0;

	unsigned char src[33] = {255, 1, 2, 2, 1, 4, 4, 4,
						4, 4, 4, 12, 13, 13, 13, 13,
						4, 4, 4, 12, 13, 13, 13, 13,
						4, 4, 4, 12, 13, 13, 13, 13, 0};
	unsigned char dst[256];					
			
	status = rldec(src, dst, 256);
	
	reportStatus(__func__, status == -1);	
	
	return status;
}

int test2() {
	int status = 0;

	unsigned char src[33] = {1, 5, 2, 8, 1, 12, 4, 16,
						4, 2, 4, 3, 13, 6, 13, 7,
						4, 1, 4, 0, 13, 3, 13, 11,
						4, 5, 4, 6, 13, 19, 13, 9, 0};
	unsigned char dst[256];					
	
	status = rldec(src, dst, 256);

	reportStatus(__func__, status);	
	
	return status;
}

int main(int argc, char **argv) {
	// Perform unit testing.
	test1();
	test2();
}
#endif
