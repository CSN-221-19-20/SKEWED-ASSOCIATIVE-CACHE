#include "cLibrary.h"

void Mapfct1(int, CACHEINT*, int);
void initscramble(void);
int scramb(int, int);
int scramble(int X,int N);
int BITR(int, int);

void Mapfct1( int addr, CACHEINT *cache, int  memtype ){        
    int  LOGBank, T, X, X1, X2, X3, X4;

    LOGBank = (*cache).Logbank;
    T = (*cache).Size_Bank;
    X1 = addr & (T - 1);
    X1=scramble(X1,LOGBank);
    X = BITR(LOGBank, (addr >> LOGBank) & (T - 1));
    X2 = ((X1 * 2) & (T - 1)) + ((2 * X1) >> LOGBank);
    X3 = ((X2 * 2) & (T - 1)) + ((2 * X2) >> LOGBank);
    X4 = ((X3 * 2) & (T - 1)) + ((2 * X3) >> LOGBank);
    AdBank[0] = X ^ X1;
    AdBank[1] = ((X ^ X2)) + (*cache).Size_Bank;
    AdBank[2] = (X ^ X3) + 2 * (*cache).Size_Bank;
    AdBank[3] = (X ^ X4) + 3 * (*cache).Size_Bank;
}  

void initMappingfunction(void){
	int i,j;  
	for (i=0;i<16;i++)
		for (j=0;j<DEUP[i];j++)
		SCRAMBLE[j][i]=scramb(j,i);

}

int scramb(int x, int y){
	int inter1,inter2,i;
	int X;
	
	X=x;
	inter1 =0;
	
	for (i=0; i< y; i +=2){
		inter1+=DEUP[i>>1] * (X & 0x01); 
		X =X >>2;
	}

	inter2 =0;
	X=x>>1;
	for (i=1; i< y; i +=2){
		inter2+=DEUP[i>>1] * (X & 0x01); 
		X =X >>2;
	}
	
	inter1 = inter2 + (inter1 << (y/2));
	if (inter1>=DEUP[y]) printf("problem x %d y %d inter1 %d\n",x,y,inter1);

	return(inter1);	
}

int scramble(int X,int N){
	if((X < 32768)&&(N < 16))
		return (SCRAMBLE[X][N]);
	
	else{
		printf("\nERROR: bad access to the matrix SCRAMBLE\n");
		exit(0);
	}
}

int BITR(int A, int B){
	int   i, X;
	B= scramble(B,A);
	X=0;
	for (i = 1; i <= A; i++) {
		X = X + (DEUP[A - i] * (B & 0x01));
		B = B >> 1;
	}
	return (X);
}

void  Mapfct2(int, CACHEINT*, int);

int H(int Y, int shift, int Mask)
{
	int y1, yn, result;

	y1 = Y >> (shift - 1);
	yn = Y & 0x01;

	result = ((Y * 2) + (yn ^ y1)) & Mask;
	return(result);
}

int G(int Y, int shift, int Mask)  {
  int yk, y1, yn, result;
  
  y1 = (Y & 0x02) >> 1;
  yn = (y1 ^ Y) & 1;
  result =  (Y >> 1) + (yn << (shift-1));
  
  return(result);
}

void  Mapfct2( int addr, CACHEINT *cache, int  memtype ){
	int A1, A2;
	int mask,Log;
	
	mask = (*cache).Size_Bank - 1;
	Log = (*cache).Logbank;

	A1 = addr & mask;
	A2 = (addr >> Log) & mask;

	AdBank[0] = (H(A1, Log, mask) ^ G(A2, Log, mask) ^ A2);
	AdBank[1] = (H(A1, Log, mask) ^ G(A2, Log, mask) ^ A1) + (*cache).Size_Bank;
	AdBank[2] = (G(A1, Log, mask) ^ H(A2, Log, mask) ^ A2) + (2 * (*cache).Size_Bank);
	AdBank[3] = (G(A1, Log, mask) ^ H(A2, Log, mask) ^ A1) + (3 * (*cache).Size_Bank);
}

void  Mapfct3(int, CACHEINT*, int);

void  Mapfct3( int addr, CACHEINT *cache, int  memtype ){
	int A1, A2;
	int mask, Low, i;
	
	mask = (*cache).Size_Bank - 1;
	
	A1 = addr & mask;
	A2 = (addr >> (*cache).Logbank) & mask;
	for(i=0;i<(*cache).Number_of_Bank;i++){
		AdBank[i] = (A1 ^ A2) + (i * (*cache).Size_Bank);
		Low = A1 >> ((*cache).Logbank-1);
		A1 = ((2*A1) + Low) & mask;
	}
}