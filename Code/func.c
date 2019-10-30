#include "cLibrary.h"

void INITCACHE(int, char**);
void initCACHE(CACHE*, int, int, void (*) () , int (*) ());
void INITCACHEINT(CACHEINT*, int, int, void (*) (), int (*) ());
char* allocation( int );

extern void (*Mapfct) ();
extern void Mapfct1(int, CACHEINT*, int);
extern void Mapfct2(int, CACHEINT*, int);
extern void Mapfct3(int, CACHEINT*, int);
extern void (*Allocate) ();
extern int (*Is_In_Cache) ();
extern void AllocateDM ();
extern void AllocateLRU ();
extern void AllocateRAND ();
extern void AllocateSKEWLRU ();
extern void AllocateSKEWRAND ();
extern void AllocateSKEWpseudoLRU ();
extern void AllocateSKEWNRU ();
extern void AllocateSKEWUNRU ();
extern void AllocateSKEWENRU ();
extern void AllocateSKEWUseful ();
extern int Is_In_CacheDM ();
extern int Is_In_CacheASSOC ();
extern int Is_In_CacheSKEWLRU ();
extern int Is_In_CacheSKEWRAND ();
extern int Is_In_CacheSKEWpseudoLRU ();
extern int Is_In_CacheSKEWNRU ();
extern int Is_In_CacheSKEWUNRU ();
extern int Is_In_CacheSKEWENRU ();
extern int Is_In_CacheSKEWUseful ();

void INITCACHE(int argc, char** argv){
	int i,NbLines,LINE,Map;

	if(argc != 5){
		printf("Usage: NAME NbMapfct CacheSize LogLine ResFile < SimFile\n");
		printf("\tNAME: CACHESKEW or CACHESKEWL2\n\tNbMapfct: Number of the mapping function\n\tCacheSize: Size in bytes of the cache\n\tLogLine: Log base 2 of line size\n\tResFile: Result File\n\tSimFile: Simulation File\n"); 
		exit(0);
	};

	Map = atoi(argv[1]);
	CacheSize = atoi(argv[2]);
	LOGLINE = atoi(argv[3]);
	ResFile = argv[4];

	switch (Map) {
		case 1: Mapfct = Mapfct1; break;
		case 2: Mapfct = Mapfct2; break;
		case 3: Mapfct = Mapfct3; break;
		default: printf("Error: Unknown mapping fonction\n"); 
		exit(0);
	}

	LINE = 1;
	for (i = 0; i < LOGLINE; i++)
		LINE = 2 * LINE;
	
	NbLines = CacheSize / LINE;
	AdBank = (int*)malloc(sizeof(int)*4);

	for (i = 0; i < NbSimul; i++) {
		initCACHE(&DM[i], NbLines, 1, AllocateDM, Is_In_CacheDM);
		initCACHE(&TWOASSOCLRU[i], NbLines, 2, AllocateLRU, Is_In_CacheASSOC);
		initCACHE(&FOURASSOCLRU[i], NbLines, 4, AllocateLRU, Is_In_CacheASSOC);
		initCACHE(&EIGHTASSOCLRU[i], NbLines, 8, AllocateLRU, Is_In_CacheASSOC);
		initCACHE(&SIXTEENASSOCLRU[i], NbLines, 16, AllocateLRU, Is_In_CacheASSOC);
		initCACHE(&TWOASSOCRAND[i], NbLines, 2, AllocateRAND, Is_In_CacheASSOC);
		initCACHE(&FOURASSOCRAND[i], NbLines, 4, AllocateRAND, Is_In_CacheASSOC);
		initCACHE(&EIGHTASSOCRAND[i], NbLines, 8, AllocateRAND, Is_In_CacheASSOC);     
		initCACHE(&SIXTEENASSOCRAND[i], NbLines, 16, AllocateRAND, Is_In_CacheASSOC);
		initCACHE(&TWOSKEWLRU[i], NbLines, 2, AllocateSKEWLRU, Is_In_CacheSKEWLRU);
		initCACHE(&FOURSKEWLRU[i], NbLines, 4, AllocateSKEWLRU, Is_In_CacheSKEWLRU);
		initCACHE(&TWOSKEWpseudoLRU[i], NbLines, 2, AllocateSKEWpseudoLRU, Is_In_CacheSKEWpseudoLRU);
		initCACHE(&TWOSKEWNRU[i], NbLines, 2, AllocateSKEWNRU, Is_In_CacheSKEWNRU);
		initCACHE(&TWOSKEWUNRU[i], NbLines, 2, AllocateSKEWUNRU, Is_In_CacheSKEWUNRU);
		initCACHE(&TWOSKEWENRU[i], NbLines, 2, AllocateSKEWENRU, Is_In_CacheSKEWENRU);
		initCACHE(&TWOSKEWUseful[i], NbLines, 2, AllocateSKEWUseful, Is_In_CacheSKEWUseful);
		initCACHE(&FOURSKEWENRU[i], NbLines, 4, AllocateSKEWENRU, Is_In_CacheSKEWENRU);
		initCACHE(&TWOSKEWRAND[i], NbLines, 2, AllocateSKEWRAND, Is_In_CacheSKEWRAND);
		initCACHE(&FOURSKEWRAND[i], NbLines, 4, AllocateSKEWRAND, Is_In_CacheSKEWRAND);

		NbLines = NbLines * 2;
	}
}
