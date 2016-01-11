#include "stdlib.h"
#include "stdint.h"
#include "stdio.h"
#define bool uint8_t


//Find the next unique arrangement of N partitions of T
//  Partitions are in descending order
//  Proceeds in a descending lexicographic order.
//  If the 'parts' array is empty ([0]==0), sets the first arrangement
//  Returns 1 if successful, 0 if there are no more arrangements.
bool next_n_sum(const uint8_t N, const uint8_t T, uint8_t parts[]) {
	if (N>T) {													// No solution if more partitions than sum
		return 0;												//
	} else if (parts[0] == 0) {							// First solution;
		for(int i=1;i<N;i++) parts[i]=1;					//  Set [i] 1..N-1 to 1
		parts[0] = T-N+1;										//	 Set [0] to the remainder
		return 1;												//
	} else for(int i=1;i<N;i++) {							// Next solution:	Search each [i] 1..N-1
		if (parts[0]-parts[i]>1) {							//   If possible, 
			++parts[i]; --parts[0];							//    Move 1 from [i] to [0]
			for(int j=i-1;j>0;j--) {				 		//    And reset any parts [j] 1..i-1
				uint8_t d = parts[j] - parts[i];			//     (moving the difference back 
				parts[j]-=d; parts[0]+=d;					//      from [j] to [0])
			}														//
			return 1;											//
		}
	}
	return 0;													// No more solutions; finished
}


int main(int argc, char *argv[]) {
   if (argc<2) { printf("ERR: Must define total!"); exit(1); }
   uint8_t N = atoi(argv[1]);
   if (!N) { printf("ERR: Total must be at least one!"); exit(1); }  
	
	uint8_t parts[N]; parts[0]=0;
	int count,tcount=0;
	
	for(int n=N;n;n--) {
		for(count=0,parts[0]=0;next_n_sum(n,N,parts);count++) {
			//for(int i=0;i<n;i++) printf("%d ",parts[i]); printf("\n");			// Print each arrangement found
		}
		printf("Found %d partitions of %d parts summing to %d.\n",count,n,N);
		tcount+=count;
	}
	printf("In total; %d \n\n",tcount);

   return 0;
}

