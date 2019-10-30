#define _main_
#include <stdio.h>
#include "cLibrary.h"
#include "SkewFile.c"
#include "mapFile.c"
#include "func.c"

void AllocateDM(int, CACHEINT*, int);
int Is_In_CacheDM(int, CACHEINT*, int);
void AllocateRAND(int, CACHEINT*, int);
void AllocateLRU( int, CACHEINT*, int);
int Is_In_CacheASSOC( int, CACHEINT*, int);

void AllocateDM( int addr, CACHEINT *cache, int memtype ){
    int  X,T;
    
    T = (*cache).Size_Bank;
    X = (addr & (T - 1));
    #ifdef SIML2
      	EXCLUDE= (*cache).Cache[X];
    #endif
      	(*cache).Cache[X] = addr; 
}



int Is_In_CacheDM(int addr, CACHEINT *cache, int memtype){
	int X,T;
	T = (*cache).Size_Bank;
	X = (addr & (T - 1));
	
	if ((*cache).Cache[X] == addr)
		return (1);
	
	return (0);
}

void AllocateRAND(int addr,CACHEINT *cache, int memtype){  
	int Choose_Bank;
	int T, X;

	T = (*cache).Size_Bank;
	X = (addr & (T - 1)) * (*cache).Number_of_Bank;
	Choose_Bank = random() % (*cache).Number_of_Bank;
	
	X = X + Choose_Bank;
	#ifdef SIML2
		EXCLUDE= (*cache).Cache[X];
	#endif
		(*cache).Cache[X] = addr;
}



void AllocateLRU( int addr, CACHEINT *cache, int memtype ){
	int min;
	int Choose_Bank;
	int T, X, i;
	
	T = (*cache).Size_Bank;
	X = (addr & (T - 1)) * (*cache).Number_of_Bank;
	Choose_Bank = 0;
	min = (*cache).Cache_DATE[X];
	for (i = 1; i < (*cache).Number_of_Bank; i++){
		if ((*cache).Cache_DATE[X + i] < min) {
			Choose_Bank = i;
			min = (*cache).Cache_DATE[X + i];
		};
	}
	for (i = 0; i < (*cache).Number_of_Bank; i++)
		if ((*cache).Cache[X + i] == -1) 
			Choose_Bank = i;
	
	X = X + Choose_Bank;
	
	#ifdef SIML2
		EXCLUDE= (*cache).Cache[X];
	#endif
		(*cache).Cache[X] = addr;
		(*cache).Cache_DATE[X] = (*cache).ACCESS;
}


