#ifdef TURBOS
#include <cmoc.h>
#include "turbos.h"
#elif FREERTOS
#else
#include <stdio.h>
#endif

/*
 * Run-length encoder.
 * 
 * Encodes a buffer of bytes into run-length encoded data.
 *
 * The worst case encoding size is ssz*2, so ensure dst points
 * to a buffer of that size + 1 for the terminal (nil) byte.
 *
 * Parameters:
 *	src = The source data to be encoded.
 *	dst = The destination encoded data plus a terminal (nil) byte.
 *  ssz = The size of the source buffer.
 *
 * Return value: The encoded size.
 *
 * Example 1:
 *  8 bytes of input:  ABACCCABA
 * 12 bytes of output: [1]A[1]B[1]A[3]C[1]A[1]B
 *
 * Example 2:
 * 16 bytes of input:  DDDDDACDBBBBBDDD
 * 12 bytes of output: [5]D[1]A[1]C[1]D[5]B[3]D
 */

int rlenc(unsigned char *src, unsigned char *dst, int ssz) {
    unsigned char rle_count = 1;	// run-length counter
    unsigned char *dstPtr = dst;	// a running pointer to destination
    int dst_size = 0;			// running size of encoded destination
    char rle_byte = src[0];		// byte to examine
    
    for (int i = 1; i <= ssz; i++) {
        // Compare the byte.
        if (src[i] == rle_byte && rle_count <= 255 && i < ssz) {
            rle_count++;
        } else {
            // Either we've found a different value at src[i], or we've exceeded the 255 byte RLE count. 
            *dstPtr = rle_count;
            dstPtr++;
            *dstPtr = rle_byte;
            dstPtr++;
            dst_size += 2;
            rle_count = 1;
            rle_byte = src[i];
        }
    }
    
    *dstPtr = 0;  // add terminal (nil) the encoded data.
    
    return dst_size;
}

void rlenc_task(void) {
	unsigned char src[128] = {1, 1, 2, 2, 1, 4, 4, 4,
						4, 4, 4, 12, 13, 13, 13, 13,
						4, 4, 4, 12, 13, 13, 13, 13,
						4, 4, 4, 12, 13, 13, 13, 13,
						4, 4, 4, 12, 13, 13, 13, 13,
						4, 4, 4, 12, 13, 13, 13, 13,
						4, 4, 4, 12, 13, 13, 13, 13,
						4, 4, 4, 12, 13, 13, 13, 13,
						4, 4, 4, 12, 13, 13, 13, 13,
						4, 4, 4, 12, 13, 13, 13, 13,
						4, 4, 4, 12, 13, 13, 13, 13,
						4, 4, 4, 12, 13, 13, 13, 13,
						4, 4, 4, 12, 13, 13, 13, 13,
						4, 4, 4, 12, 13, 13, 13, 13,
						4, 4, 4, 12, 13, 13, 13, 13,
						4, 4, 4, 12, 13, 13, 13, 13};	
					
	unsigned char dst[256];
											
	rlenc(src, dst, 128);
}

#ifdef TURBOS
// Destination is global variable for one-time allocation on heap.
unsigned char dst[256];

int main(int argc, char **argv) {
	// Sleep forever and wait for a signal to drive the routine.
	while (1) {
		rlenc_task();	
//		f_sleep(100);
	}
	
	return 0;
}
#elif FREERTOS
void rlenc_freertos(void *parameters) {
	while (1) {
		rlenc_task();
		vTaskDelay(10);
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
	
	unsigned char src[128] = {1, 1, 2, 2, 1, 4, 4, 4,
						4, 4, 4, 12, 13, 13, 13, 13,
						4, 4, 4, 12, 13, 13, 13, 13,
						4, 4, 4, 12, 13, 13, 13, 13,
						4, 4, 4, 12, 13, 13, 13, 13,
						4, 4, 4, 12, 13, 13, 13, 13,
						4, 4, 4, 12, 13, 13, 13, 13,
						4, 4, 4, 12, 13, 13, 13, 13,
						4, 4, 4, 12, 13, 13, 13, 13,
						4, 4, 4, 12, 13, 13, 13, 13,
						4, 4, 4, 12, 13, 13, 13, 13,
						4, 4, 4, 12, 13, 13, 13, 13,
						4, 4, 4, 12, 13, 13, 13, 13,
						4, 4, 4, 12, 13, 13, 13, 13,
						4, 4, 4, 12, 13, 13, 13, 13,
						4, 4, 4, 12, 13, 13, 13, 13};						

	unsigned char dst[256];					
	unsigned char expected_dst[] = {2, 1, 2, 2, 1, 1, 6, 4,
							 1, 12, 4, 13, 3, 4, 1, 12,
							 4, 13, 3, 4, 1, 12, 4, 13,
							 3, 4, 1, 12, 4, 13, 3, 4,
							 1, 12, 4, 13, 3, 4, 1, 12,
							 4, 13, 3, 4, 1, 12, 4, 13,
							 3, 4, 1, 12, 4, 13, 3, 4,
							 1, 12, 4, 13, 3, 4, 1, 12,
							 4, 13, 3, 4, 1, 12, 4, 13,
							 3, 4, 1, 12, 4, 13, 3, 4,
							 1, 12, 4, 13, 3, 4, 1, 12,
							 4, 13, 3, 4, 1, 12, 4, 13};						
	 
	int dsz = rlenc(src, dst, 128);

	for (int i = 0; i < dsz; i++) {
		if (dst[i] != expected_dst[i]) {
			fprintf(stderr, "Mismatch at offset %d!\n", i);
			status = -1;
		}
	}

	reportStatus(__func__, status);	
	
	return status;
}

int main(int argc, char **argv) {
	// Perform unit testing.
	test1();
}
#endif
