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

int Is_In_CacheASSOC( int addr, CACHEINT *cache, int memtype ){
	int i, X, T, inter;
	
	T = (*cache).Size_Bank;
	X = (addr & (T - 1)) * (*cache).Number_of_Bank;
	
	if ((*cache).Cache[X] == addr) return(1);
	
	for (i = 1; i < (*cache).Number_of_Bank; i++){
		if ((*cache).Cache[X + i] == addr) {
			(*cache).Cache_DATE[X + i] = (*cache).Cache_DATE[X];
			(*cache).Cache_DATE[X]= (*cache).ACCESS;
			inter = (*cache).Cache[X];
			(*cache).Cache[X]= (*cache).Cache[X + i];
			(*cache).Cache[X + i]= inter;
			
			return (1);
		}
	}
	return (0);
}


int main(int argc, char** argv)
{
  int memtype;
  int addr;

  INITCACHE(argc, &argv[0]);
	
  while (scanf("%d %x", &memtype, &addr)>=0) {
    memory_access(memtype, addr);
  };

  SIM();
  DUMPRES();

  return 0;
}