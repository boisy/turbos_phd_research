#define F$Fork 		0x03
#define F$Icpt 		0x09
#define F$Sleep 	0x0A
#define F$SRqMem 	0x28
#define F$SRtMem 	0x29

int f_fork(char *modname, char *params, char typelang, char pagemem, int parammem);
int f_sleep(int ticks);
int f_icpt(interrupt void (*routine)(void));
void *f_srqmem(int size);
void f_srtmem(void *address, int size);

void mapVDG(void *address);