#include "cLibrary.h"

extern void (*Mapfct) ();

void AllocateSKEWUseful(int, CACHEINT*, int);
int Is_In_CacheSKEWUseful(int, CACHEINT*, int);

void AllocateSKEWUseful( int addr, CACHEINT *cache, int memtype ){  
	int  i, set;
	
	Mapfct( addr, cache, memtype );
	set = -1;
	if ((*cache).Cache_Prio[AdBank[0]] == (*cache).Cache_Prio[AdBank[1]])
		set = (*cache).Cache_Prio[AdBank[0]];
	
	else
		set = random() & ((*cache).Number_of_Bank - 1);
	
	for (i = 0; i < (*cache).Number_of_Bank; i++)
		if ((*cache).Cache[AdBank[i]] == FreeAddr) 
		set = i;
		
	(*cache).Cache_Prio[AdBank[0]] = 1 - set;
	(*cache).Cache_Prio[AdBank[1]] = 1 - set;
	#ifdef SIML2
		EXCLUDE= (*cache).Cache[AdBank[set]];
	#endif
	(*cache).Cache[AdBank[set]] = addr;
}

int Is_In_CacheSKEWUseful( int addr, CACHEINT *cache, int memtype ){
	int	i;
	
	if ((*cache).Number_of_Bank != 2){
		printf("ERROR: The SkewUseful replacement policy only works for two banks.");
		exit(0);
	}

	Mapfct( addr, cache, memtype );
	
	for (i = 0; i < (*cache).Number_of_Bank; i++) {
		if ((*cache).Cache[AdBank[i]] == addr) {
			(*cache).Cache_Prio[AdBank[0]] = 1 - i;
			(*cache).Cache_Prio[AdBank[1]] = 1 - i;

			if ((*cache).Cache_DATE[AdBank[i]] < ((*cache).DATE & (0xffffffff - ((*cache).Size / 2 - 1))))
			(*cache).DATE++;
			
			(*cache).Cache_DATE[AdBank[i]] = (*cache).DATE;
			return (1);
		}
	}
	return (0);
}

void AllocateSKEWpseudoLRU(int, CACHEINT*, int);
int Is_In_CacheSKEWpseudoLRU(int, CACHEINT*, int);

void AllocateSKEWpseudoLRU(int addr, CACHEINT *cache, int memtype ){
	int		i, set;
	Mapfct( addr, cache, memtype );
	set = (*cache).Cache_Prio[AdBank[0]];
	
	for (i = 0; i < (*cache).Number_of_Bank; i++)
		if ((*cache).Cache[AdBank[i]] == FreeAddr) 
		set = i;
	
	#ifdef SIML2
		EXCLUDE= (*cache).Cache[AdBank[set]];
	#endif
	(*cache).Cache[AdBank[set]] = addr;
	(*cache).Cache_Prio[AdBank[0]] = 1 - set;
}

int Is_In_CacheSKEWpseudoLRU(int addr, CACHEINT *cache, int memtype ){
	int	i;
	
	if ((*cache).Number_of_Bank != 2){
		printf("ERROR: The SKEWpseudoLRU replacement policy only works for two banks.");
		exit(0);
	}

	Mapfct( addr, cache, memtype );
	
	for (i = 0; i < (*cache).Number_of_Bank; i++){
		if ((*cache).Cache[AdBank[i]] == addr) {
			(*cache).Cache_Prio[AdBank[0]] = 1 - i;
			return (1);
		}
	}
	return (0);
}

void AllocateSKEWENRU(int, CACHEINT*, int);
int Is_In_CacheSKEWENRU(int, CACHEINT*, int);

void AllocateSKEWENRU(int addr, CACHEINT *cache, int  memtype ){
	int Age[8], set, i, Start, pseudoDATE, pseudoDATE1;
	
	Start = random() & ((*cache).Number_of_Bank - 1);
	
	Mapfct( addr, cache, memtype );
	set = -1;
	
	pseudoDATE = (*cache).DATE & (0xffffffff - (((*cache).Size / 2) - 1));
	pseudoDATE1 = ((*cache).DATE +  ((*cache).Size / 4)) & (0xffffffff - ((*cache).Size / 2) - 1);

	for(i=0; i<(*cache).Number_of_Bank; i++)
		Age[i] = (((*cache).Cache_DATE[AdBank[i]] < pseudoDATE) & (((*cache).Cache_DATE[AdBank[i]] + ((*cache).Size / 4)) < pseudoDATE1));

	for(i=Start; i<(*cache).Number_of_Bank; i++)
		if (Age[i]) set=i;
	
	for(i=0; i<Start; i++)
		if (Age[i]) set=i;
	
	if (set == -1){
		for(i=0; i<(*cache).Number_of_Bank; i++)
			Age[i] = (((*cache).Cache_DATE[AdBank[i]] < pseudoDATE) |  ((((*cache).Cache_DATE[AdBank[i]] + ((*cache).Size / 4))) < pseudoDATE1));
		
		for(i=Start; i<(*cache).Number_of_Bank; i++)
			if (Age[i]) set=i;
		
		for(i=0; i<Start; i++)
			if (Age[i]) set=i;
	}
	
	if (set== -1)
		set = Start;
	
	for (i = 0; i < (*cache).Number_of_Bank; i++)
		if ((*cache).Cache[AdBank[i]] == FreeAddr) 
			set = i;
		
	(*cache).Cache_DATE[AdBank[set]] = (*cache).DATE;
	(*cache).DATE++;
	#ifdef SIML2
		EXCLUDE= (*cache).Cache[AdBank[set]];
	#endif

	(*cache).Cache[AdBank[set]] = addr;
}


int Is_In_CacheSKEWENRU( int addr, CACHEINT *cache, int memtype ){
	int	i;
	
	Mapfct( addr, cache, memtype );
	
	for (i = 0; i < (*cache).Number_of_Bank; i++) {
		if ((*cache).Cache[AdBank[i]] == addr) {
			if ((*cache).Cache_DATE[AdBank[i]] < ((*cache).DATE & (0xffffffff - ((*cache).Size / 2 - 1))))
				(*cache).DATE++;
			
			(*cache).Cache_DATE[AdBank[i]] = (*cache).DATE;
			return (1);
		}
	}
	return (0);
}

void AllocateSKEWLRU(int, CACHEINT*, int);
int Is_In_CacheSKEWLRU(int, CACHEINT*, int);

void AllocateSKEWLRU( int addr, CACHEINT *cache, int memtype ){
	int	i, min, set;
	
	Mapfct( addr, cache, memtype );
	
	set = 0;
	min = (*cache).Cache_DATE[AdBank[0]];
	for (i = 1; i < (*cache).Number_of_Bank; i++){
		if ((*cache).Cache_DATE[AdBank[i]] < min) {
			set = i;
			min = (*cache).Cache_DATE[AdBank[i]];
		}
	}
	for (i = 0; i < (*cache).Number_of_Bank; i++){
		if ((*cache).Cache[AdBank[i]] == FreeAddr)
			set = i;
	}
	#ifdef SIML2
		EXCLUDE= (*cache).Cache[AdBank[set]];
	#endif

	(*cache).Cache[AdBank[set]]= addr;
	(*cache).Cache_DATE[AdBank[set]]= (*cache).ACCESS ;
}

int Is_In_CacheSKEWLRU( int addr, CACHEINT *cache, int memtype ){
	int	i;
	Mapfct( addr, cache, memtype );
	
	for (i = 0; i < (*cache).Number_of_Bank; i++){
		if ((*cache).Cache[AdBank[i]] == addr) {
			(*cache).Cache_DATE[AdBank[i]] = (*cache).ACCESS;
			return (1);
		}
	}
	return (0);
}

void AllocateSKEWNRU(int, CACHEINT*, int);
int Is_In_CacheSKEWNRU(int, CACHEINT*, int);

void AllocateSKEWNRU( int addr, CACHEINT *cache, int memtype ){
	int Old[8], set, i, Start, pseudoDATE;
	
	Start = random() & ((*cache).Number_of_Bank - 1);
	Mapfct( addr, cache, memtype );
	set = -1;
	pseudoDATE = (*cache).DATE & (0xffffffff - (((*cache).Size / 2) - 1));
	
	for(i=0; i<(*cache).Number_of_Bank; i++)
		Old[i] = (((*cache).Cache_DATE[AdBank[i]] < pseudoDATE));
	
	for(i=Start; i<(*cache).Number_of_Bank; i++)
		if (Old[i]) set=i;
	
	for(i=0; i<Start; i++)
		if (Old[i]) set=i;
	
	if (set== -1) 
		set =Start;
	
	for (i = 0; i < (*cache).Number_of_Bank; i++)
		if ((*cache).Cache[AdBank[i]] == FreeAddr) 
			set = i;
	
	(*cache).Cache_DATE[AdBank[set]] = (*cache).DATE;
	(*cache).DATE++;
	#ifdef SIML2
		EXCLUDE= (*cache).Cache[AdBank[set]];
	#endif
	(*cache).Cache[AdBank[set]] = addr;
}

int Is_In_CacheSKEWNRU( int addr, CACHEINT *cache, int memtype ){
  int	i;
  
  Mapfct( addr, cache, memtype );
  
  for (i = 0; i < (*cache).Number_of_Bank; i++) {
    if ((*cache).Cache[AdBank[i]] == addr) {
      	if ((*cache).Cache_DATE[AdBank[i]] < ((*cache).DATE & (0xffffffff - ((*cache).Size / 2 - 1))))
			(*cache).DATE++;
      
		(*cache).Cache_DATE[AdBank[i]] = (*cache).DATE;
		return (1);
    }
  };
  return (0);
}

void AllocateSKEWUNRU(int, CACHEINT*, int);
int Is_In_CacheSKEWUNRU(int, CACHEINT*, int);

void AllocateSKEWUNRU( int addr, CACHEINT *cache, int memtype ){
	int Young0, Young1, i, set;
	int pseudoDATE;
	
	Mapfct( addr, cache, memtype );
	
	set = -1;
	pseudoDATE = (*cache).DATE & (0xffffffff - (((*cache).Size / 2) - 1));
	Young0 = ((*cache).Cache_DATE[AdBank[0]] >= pseudoDATE);
	Young1 = ((*cache).Cache_DATE[AdBank[1]] >= pseudoDATE);
	
	if (Young0 != Young1) {
		if (Young0)
			set = 1;
		
		else
			set = 0;
	} 
	else {
		if ((*cache).Cache_Prio[AdBank[0]] == (*cache).Cache_Prio[AdBank[1]])
			set = (*cache).Cache_Prio[AdBank[0]];
		
		else
			set = random() & ((*cache).Number_of_Bank - 1);
	}
	
	for (i = 0; i < (*cache).Number_of_Bank; i++)
		if ((*cache).Cache[AdBank[i]] == FreeAddr)
			set = i;

	(*cache).Cache_Prio[AdBank[1]] = 1 - set;
	(*cache).Cache_Prio[AdBank[0]] = 1 - set;    
	(*cache).Cache_DATE[AdBank[set]] = (*cache).DATE;
	(*cache).DATE++;

	#ifdef SIML2
		EXCLUDE= (*cache).Cache[AdBank[set]];
	#endif
		(*cache).Cache[AdBank[set]] = addr;
		
}

int Is_In_CacheSKEWUNRU( int addr, CACHEINT *cache, int memtype ){
	int	i;
	
	if ((*cache).Number_of_Bank != 2){
		printf("ERROR: The SkewUNRU replacement policy only works for two banks.");
		exit(0);
	}

	Mapfct( addr, cache, memtype );
	
	for (i = 0; i < (*cache).Number_of_Bank; i++) {
		if ((*cache).Cache[AdBank[i]] == addr) {
			(*cache).Cache_Prio[AdBank[0]] = 1 - i;
			(*cache).Cache_Prio[AdBank[1]] = 1 - i;
			if ((*cache).Cache_DATE[AdBank[i]] < ((*cache).DATE & (0xffffffff - ((*cache).Size / 2 - 1))))
				(*cache).DATE++;
			
			(*cache).Cache_DATE[AdBank[i]] = (*cache).DATE;
			return (1);
		}
	}
	return (0);
}

void AllocateSKEWRAND(int, CACHEINT*, int);
int Is_In_CacheSKEWRAND(int, CACHEINT*, int);

void AllocateSKEWRAND(int addr, CACHEINT *cache,int memtype ){
	int	i, set;
	Mapfct( addr, cache, memtype );
	set = random() % (*cache).Number_of_Bank;

	for (i = 0; i < (*cache).Number_of_Bank; i++)
		if ((*cache).Cache[AdBank[i]] == FreeAddr) 
			set = i;

	#ifdef SIML2
		EXCLUDE= (*cache).Cache[AdBank[set]];
	#endif
	(*cache).Cache[AdBank[set]] = addr;
}

int Is_In_CacheSKEWRAND( int addr, CACHEINT *cache, int memtype ){
	int	i;
	
	Mapfct( addr, cache, memtype );
	
	for (i = 0; i < (*cache).Number_of_Bank; i++)
		if ((*cache).Cache[AdBank[i]] == addr)
			return (1);
		
	return (0);
}