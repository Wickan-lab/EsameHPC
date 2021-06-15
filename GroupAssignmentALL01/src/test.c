#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int a[15];
int N = 15;

void exchange(int i, int j) {
  int t;
  t = a[i];
  a[i] = a[j];
  a[j] = t;
}

void impBitonicSort() {
  int i,j,k;
  
  for (k=2; k<=N; k=2*k) {
    for (j=k>>1; j>0; j=j>>1) {
      for (i=0; i<N; i++) {
				int ij=i^j;
				if ((ij)>i) {
	  			if ((i&k)==0 && a[i] > a[ij])
			      exchange(i,ij);
	  			if ((i&k)!=0 && a[i] < a[ij])
	      		exchange(i,ij);
				}
      }
    }
  }
}

int main(int argc, char**argv){
	for (int i = 0; i < N; ++i)
	{	
		a[i] = rand() % 50;
	}
	impBitonicSort();
	for(int y = 0; y < 15; y++)
		printf("%d\n",a[y]);
}