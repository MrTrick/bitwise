#include "stdlib.h"
#include "stdint.h"
#include "stdio.h"
#include "string.h"
#include "unistd.h"

#define ELEM uint16_t
#define ffs __builtin_ffs

//Exhaustively calculate *all* possible latin squares of size N, sending solutions back to the given function.
int latin(ELEM sq[], const uint8_t N, void cb(ELEM[],const uint8_t)) {
	//Precalculated values
   const ELEM ALLb=(1<<N)-1;
   const uint8_t L=N*N, Lr=L-N;
   uint8_t hmore[L]; 
   memset(hmore,1,sizeof(hmore)); 
   for(int i=N-1;i<L;i+=N) hmore[i]=0;

	//Variables
   uint64_t count=0;                               //Tally of how many squares found
   ELEM hfree[L], vfree[L];                    		//Free bits in each row/col
   ELEM free[L];                                 	//Free and unchecked bits in each element
   uint8_t i;                                      //Current index
   ELEM el;                                        //Current element
   //memset(free,0,sizeof(free));   memset(hfree,0,sizeof(hfree));  memset(vfree,0,sizeof(vfree)); //Clear memory to make debugging clearer

   //Initial values
   for(i=0;i<N;i++) hfree[N*i]=vfree[i]=ALLb;      // Top and left sides are available
   sq[i=0]=0;
   free[i]=hfree[i]&vfree[i];

   loop_el: do {                                   // While there is search space
      while ((el=free[i]&-free[i])) {              // While there are valid and unchecked values at [i]
                                                   //     (el = lowest '1', eg if free is 0b101100, el is 0b000100)
         free[i]&=~el;                            	// Mark el as checked
         if (hmore[i]) hfree[i+1]=hfree[i]&~el;    // Value can't be used to the right
         if (i<Lr) vfree[i+N]=vfree[i]&~el;   		// Value can't be used underneath
         sq[i]=el;                                 // Record element
         if (i==L-1) {                           	// Is the square complete?
            cb(sq,N);                              //  Yes; Pass back to the caller
            count++;                               //       Tally the solution
         } else {
            i++;                                   //  No; PUSH!
            free[i]=hfree[i]&vfree[i];             //      Calculate available bits
            goto loop_el;                          //      Start search at next i
         }
      }
   } while(i--);                                   // POP! No more values at this position

   return count;
}

void found_solution(ELEM sq[], const uint8_t N) {
   for(int j=0;j<N*N;j++) putchar(ffs(sq[j])+'0');
   putchar('\n');
}

int main(int argc, char *argv[]) {
   if (argc<2) { printf("ERR: Must define dimension!"); exit(1); }
   uint8_t N = atoi(argv[1]);
   if (!N) { printf("ERR: Dimension must be at least one!"); exit(1); }

   ELEM sq[N*N]; memset(sq,0,sizeof(sq));    //Create state space with zeros
   int count = latin(sq,N,found_solution);
   if (isatty(1)) printf("Found %d solutions\n",count);
   
   return 0;
}

