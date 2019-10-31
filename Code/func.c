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

void initCACHE(CACHE *cache, int NbLines, int NbBank, void (*Allo) (), int (*Is_In) ()){
	#ifdef SIML2
		INITCACHEINT(&(*cache).cacheL2, NbLines, NbBank, Allo, Is_In);
		INITCACHEINT(&(*cache).cacheinst, 512, 1, AllocateDM, Is_In_CacheDM);
		INITCACHEINT(&(*cache).cachedata, 512, 1, AllocateDM, Is_In_CacheDM);
	#else
		INITCACHEINT(&(*cache).cacheinst, NbLines, NbBank, Allo, Is_In);
		INITCACHEINT(&(*cache).cachedata, NbLines, NbBank, Allo, Is_In);
	#endif
}

void INITCACHEINT(CACHEINT *cache, int NbLines, int NbBank, void (*Allo) (), int (*Is_In) ())
{
	int i, t;
	
	(*cache).Size = NbLines;
	(*cache).Number_of_Bank = NbBank;
	(*cache).Size_Bank = NbLines / NbBank;
	(*cache).Logbank = 0;
	t = 1;
	while (t < (*cache).Size_Bank) {
		(*cache).Logbank++;
		t = t * 2;
	}
	
	(*cache).Cache = (unsigned int *) allocation(NbLines + 27);
	(*cache).Cache_DATE = (int *) allocation(NbLines + 39);
	(*cache).Cache_Prio = (int *) allocation(2 * NbLines + 67);
	
	for (i = 0; i < NbLines; i++) {
		(*cache).Cache[i] = FreeAddr;
		(*cache).Cache_DATE[i] = -1;
		(*cache).Cache_Prio[i] = 0;
	}
	(*cache).Allocate = Allo;
	(*cache).Is_In_Cache = Is_In;
}

char* allocation(int size){
	char *pt;
	pt = malloc(sizeof(int) * size);
	if (!pt) {
		fprintf(stderr, "Not enough Memory\n");
		exit(1);
	}
	return (pt);
}

void DUMPRES(void);
void printCACHE( FILE* , CACHE* );
 
void DUMPRES(void){
	FILE *fx;
	int i, Taille;
	
	fx = fopen(ResFile, "a");

	fprintf(fx, "#instructions : %d  || #data : %d \n", nbinst, nbref - nbinst);
	fprintf(fx, "\tnbinst: number of instructions\n");

	#ifdef SIML2
		fprintf(fx, "\tL2Miss: number of miss of the second level cache\n");
		fprintf(fx,"\tthe first level of cache is fixed to 8K, 16bytes line size with a Direct Map replacement policy.");
	#else
		fprintf(fx, "\tdataMiss: number of miss of the data cache\n\tinstMiss:number of miss of the instruction cache\n");
	#endif

	fprintf(fx, "\n\n");
	Taille = CacheSize;
	for (i = 0; i < 3; i++) {
		fprintf(fx, "\n# %d \n", Taille);
		fprintf(fx, "strategy\tNbBank\tnbinst\t");

		#ifdef SIML2
			fprintf(fx, "L2Miss\t100*L2Miss/nbinst\n\n");
		#else
			fprintf(fx, "dataMiss\tinstMiss\t100*nbMiss/nbinst\t\n\n");
		#endif

		fprintf(fx, "DirectMap \t1\t");    printCACHE(fx, &DM[i]);
		fprintf(fx, "AssocLRU  \t2\t");    printCACHE(fx, &TWOASSOCLRU[i]);
		fprintf(fx, "AssocLRU  \t4\t");    printCACHE(fx, &FOURASSOCLRU[i]);
		fprintf(fx, "AssocLRU  \t8\t");    printCACHE(fx, &EIGHTASSOCLRU[i]);
		fprintf(fx, "AssocLRU  \t16\t");   printCACHE(fx, &SIXTEENASSOCLRU[i]);
		fprintf(fx, "AssocRAND \t2\t");   printCACHE(fx, &TWOASSOCRAND[i]);
		fprintf(fx, "AssocRAND \t4\t");   printCACHE(fx, &FOURASSOCRAND[i]);
		fprintf(fx, "AssocRAND \t8\t");   printCACHE(fx, &EIGHTASSOCRAND[i]);
		fprintf(fx, "AssocRAND \t16\t");  printCACHE(fx, &SIXTEENASSOCRAND[i]);
		fprintf(fx, "SkewLRU   \t2\t");   printCACHE(fx, &TWOSKEWLRU[i]);
		fprintf(fx, "SkewRAND  \t2\t");   printCACHE(fx, &TWOSKEWRAND[i]);
		fprintf(fx, "SkewPseudoLRU\t2\t");   printCACHE(fx, &TWOSKEWpseudoLRU[i]);
		fprintf(fx, "SkewNRU   \t2\t");   printCACHE(fx, &TWOSKEWNRU[i]);
		fprintf(fx, "SkewUNRU  \t2\t");   printCACHE(fx, &TWOSKEWUNRU[i]);
		fprintf(fx, "SkewENRU  \t2\t");   printCACHE(fx, &TWOSKEWENRU[i]);
		fprintf(fx, "SkewUseful\t2\t");   printCACHE(fx, &TWOSKEWUseful[i]);
		fprintf(fx, "SkewLRU   \t4\t");   printCACHE(fx, &FOURSKEWLRU[i]);
		fprintf(fx, "SkewRAND  \t4\t");   printCACHE(fx, &FOURSKEWRAND[i]);
		fprintf(fx, "SkewENRU  \t4\t");  printCACHE(fx, &FOURSKEWENRU[i]);

		Taille = Taille * 2;
		fprintf(fx, "\n");
	}
	fclose(fx);
}



void printCACHE(FILE *fx, CACHE *cache){
	fprintf(fx, "%d\t",nbinst);
	
	#ifdef SIML2
		fprintf(fx, "%d\t%f",(*cache).cacheL2.Miss,100. * (float) ((*cache).cacheL2.Miss) / ((float) nbinst));

	#else
		fprintf(fx, "%d\t\t%d\t\t%f\t", (*cache).cachedata.Miss, (*cache).cacheinst.Miss, 100.* ((float) ((*cache).cachedata.Miss + (*cache).cacheinst.Miss)) / ((float) nbinst));

	#endif
		fprintf(fx, "\n");
}

void SIM(void);
void Simul(CACHE *, int, int );
void memory_access( int, int );
void enforceinclusion( CACHE*);

void SIM(void)
{
	int i,K;
	
	for (i = 0; i < NbSimul; i++) {
		for (K = 0; K < Index; K++)
			Simul(&DM[i], ADDR[K], MEMTYPE[K]);
		for (K = 0; K < Index; K++)
			Simul(&TWOASSOCLRU[i], ADDR[K], MEMTYPE[K]);
		for (K = 0; K < Index; K++)
			Simul(&FOURASSOCLRU[i], ADDR[K], MEMTYPE[K]);
		for (K = 0; K < Index; K++)
			Simul(&EIGHTASSOCLRU[i], ADDR[K], MEMTYPE[K]);
		for (K = 0; K < Index; K++)
			Simul(&SIXTEENASSOCLRU[i], ADDR[K], MEMTYPE[K]);
		for (K = 0; K < Index; K++)
			Simul(&TWOASSOCRAND[i], ADDR[K], MEMTYPE[K]);
		for (K = 0; K < Index; K++)
			Simul(&FOURASSOCRAND[i], ADDR[K], MEMTYPE[K]);
		for (K = 0; K < Index; K++)
			Simul(&EIGHTASSOCRAND[i], ADDR[K], MEMTYPE[K]);
		for (K = 0; K < Index; K++)
			Simul(&SIXTEENASSOCRAND[i], ADDR[K], MEMTYPE[K]);
		for (K = 0; K < Index; K++)
			Simul(&TWOSKEWLRU[i], ADDR[K], MEMTYPE[K]);
		for (K = 0; K < Index; K++)
			Simul(&TWOSKEWRAND[i], ADDR[K], MEMTYPE[K]);
		for (K = 0; K < Index; K++)
			Simul(&TWOSKEWpseudoLRU[i], ADDR[K], MEMTYPE[K]);
		for (K = 0; K < Index; K++)
			Simul(&TWOSKEWNRU[i], ADDR[K], MEMTYPE[K]);
		for (K = 0; K < Index; K++)
			Simul(&TWOSKEWUNRU[i], ADDR[K], MEMTYPE[K]);
		for (K = 0; K < Index; K++)
			Simul(&TWOSKEWENRU[i], ADDR[K], MEMTYPE[K]);
		for (K = 0; K < Index; K++)
			Simul(&TWOSKEWUseful[i], ADDR[K], MEMTYPE[K]);
		for (K = 0; K < Index; K++)
			Simul(&FOURSKEWLRU[i], ADDR[K], MEMTYPE[K]);
		for (K = 0; K < Index; K++)
			Simul(&FOURSKEWRAND[i], ADDR[K], MEMTYPE[K]);
		for (K = 0; K < Index; K++)
			Simul(&FOURSKEWENRU[i], ADDR[K], MEMTYPE[K]);
	}
	Index = 0;
}


void Simul(CACHE *cache, int add, int memtype){
	CACHEINT *	cacheint;

	if (memtype == mem_inst)
		cacheint = &(cache->cacheinst);
	
	else
		cacheint = &(cache->cachedata);

	(*cacheint).ACCESS++;
	if ((*cacheint).Is_In_Cache(add, cacheint, memtype))
		cacheint->Hit++;
	
	else {
		cacheint->Miss++;
		((*cacheint).Allocate(add, cacheint, memtype));
		#ifdef SIML2
			cacheint = &(cache->cacheL2);
			(*cacheint).ACCESS++;
			add = add >> (LOGLINE - 4);
			if ((*cacheint).Is_In_Cache(add, cacheint, memtype))
				cacheint->Hit++;

			else {
				cacheint->Miss++;
				((*cacheint).Allocate(add, cacheint, memtype));
				enforceinclusion(cache);
			}
		#endif
	}
}



#ifdef SIML2
void enforceinclusion(CACHE* cache){
	int i;
	EXCLUDE = EXCLUDE << LOGLINE - 4;
	for (i = 0; i < (1 << LOGLINE - 4) - 1; i++) {
		if ((*cache).cacheinst.Cache[EXCLUDE & 511] == EXCLUDE)
			(*cache).cacheinst.Cache[EXCLUDE & 511] = -1;
		if ((*cache).cachedata.Cache[EXCLUDE & 511] == EXCLUDE)
			(*cache).cachedata.Cache[EXCLUDE & 511] = -1;
		EXCLUDE++;
	}
}
#endif

void memory_access(int memtype, int addr){
	#ifdef SIML2
		addr = addr >> 4;
	#else
		addr = addr >> LOGLINE;
	#endif
		nbref++;
	if (nbref & 0xffff == 0)
		printf(" (%d) ", nbref);

	if (memtype == mem_inst){
		nbinst++;
		if (lastinst == addr) return;
		lastinst = addr;
	} 
	else {
		if (lastdata == addr) return;
		lastdata = addr;
	}
	
	MEMTYPE[Index] = memtype;
	ADDR[Index] = addr;
	Index++;
	if (Index != 16384)
		return;
	SIM();
}