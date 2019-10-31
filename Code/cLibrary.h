#ifndef lib_file
#define lib_file
#define  NbSimul 3
#ifdef _WIN32
#define random rand
#endif

#include <stdio.h>
#include <malloc.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct{ 
	int Hit, Miss, Size, Logbank, Number_of_Bank, Size_Bank, DATE, ACCESS;
	unsigned int	*Cache;
	int	*Cache_DATE;
	int	*Cache_Prio;
	void (*Allocate) ();
	int (*Is_In_Cache) ();
}CACHEINT;
  
typedef struct {
	CACHEINT	cacheL2, cachedata, cacheinst;  
}CACHE;


#define FreeAddr -1
#define mem_inst 2 
#define mem_write 1
#define mem_read 0

#ifdef _main_

int	CacheSize;
int	LOGLINE;
void (*Mapfct) ();
char *ResFile;
unsigned int ADDR[16384];
unsigned int MEMTYPE[16384];
int	Index = 0;
int	nbref = 0;
int	nbinst = 0;
int	lastinst;
int	lastdata; 
int *AdBank;
int	DEUP[16] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768 };
int    	SCRAMBLE[32768][16];

#ifdef SIML2
int    	EXCLUDE = 0;
#endif

CACHE	DM[NbSimul],
	TWOSKEWLRU[NbSimul], 
	TWOSKEWRAND[NbSimul], 
	TWOSKEWpseudoLRU[NbSimul],
	TWOSKEWNRU[NbSimul],
	TWOSKEWUNRU[NbSimul],
	TWOSKEWENRU[NbSimul],
	TWOSKEWUseful[NbSimul],  
	FOURSKEWLRU[NbSimul],
	TWOASSOCLRU[NbSimul], 
	FOURSKEWENRU[NbSimul],
	FOURASSOCLRU[NbSimul], 
	FOURSKEWRAND[NbSimul], 
	TWOASSOCRAND[NbSimul], 
	FOURASSOCRAND[NbSimul], 
	EIGHTASSOCLRU[NbSimul],
	EIGHTASSOCRAND[NbSimul], 
    SIXTEENASSOCLRU[NbSimul],
	SIXTEENASSOCRAND[NbSimul];
	
#else

extern	int	*AdBank;  
extern	int	nbref;
extern	int	nbinst;
extern	int	lastinst;
extern	int	lastdata;
extern	int	CacheSize;
extern	int	LOGLINE;
extern	char	*ResFile;
extern	unsigned int	MEMTYPE[32768];
extern	unsigned int	ADDR[32768];
extern	int	Index;
extern	int	DEUP[16];
extern	int	SCRAMBLE[32768][16];
   
#ifdef SIML2
extern	int	EXCLUDE;
#endif

extern	CACHE	DM[NbSimul];
extern	CACHE	TWOSKEWLRU[NbSimul]; 
extern	CACHE	TWOSKEWRAND[NbSimul]; 
extern	CACHE	TWOSKEWNRU[NbSimul];
extern	CACHE	TWOSKEWUNRU[NbSimul];
extern	CACHE	TWOSKEWENRU[NbSimul];
extern	CACHE	TWOSKEWUseful[NbSimul];  
extern	CACHE	FOURSKEWLRU[NbSimul];
extern	CACHE	TWOASSOCLRU[NbSimul]; 
extern	CACHE	FOURSKEWENRU[NbSimul];
extern	CACHE	FOURASSOCLRU[NbSimul]; 
extern	CACHE	FOURSKEWRAND[NbSimul]; 
extern	CACHE	TWOASSOCRAND[NbSimul]; 
extern	CACHE	FOURASSOCRAND[NbSimul]; 
extern	CACHE	EIGHTASSOCLRU[NbSimul];
extern	CACHE	EIGHTASSOCRAND[NbSimul]; 
extern	CACHE	SIXTEENASSOCLRU[NbSimul];
extern	CACHE	SIXTEENASSOCRAND[NbSimul];
extern	CACHE	TWOSKEWpseudoLRU[NbSimul];
	
#endif
#endif
