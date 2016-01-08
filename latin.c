#include "stdlib.h"
#include "stdint.h"
#include "stdio.h"
#include "string.h"

#define ELEM uint16_t
#define ffs __builtin_ffs

//      print_bits(free[j],N); putchar(' ');
//      if (!((j+1)%N)) putchar('|');
//   }
//}
int latin(ELEM sq[], const uint8_t N, void cb(ELEM[],const uint8_t)) {
   const ELEM ALLb=(1<<N)-1;

   uint64_t count=0;                               //Tally of how many squares found
   ELEM hfree[N*N], vfree[N*N];                    //Free bits in each row/col
   ELEM free[N*N];                                 //Free and unchecked bits in each element
   uint8_t i;                                      //Current index
   ELEM el;                                        //Current element
   memset(free,0,sizeof(free));   memset(hfree,0,sizeof(hfree));  memset(vfree,0,sizeof(vfree)); //Clear memory to make debugging clearer

   //Initial values
   for(i=0;i<N;i++) hfree[N*i]=vfree[i]=ALLb;      // Top and left sides are available
   sq[i=0]=0;
   free[i]=hfree[i]&vfree[i];

   loop_el: do {                                   // While there is search space
      //printf("loop_el - i:%d, free[i]:",i);print_bits(free[i],N);printf("\n");
      while ((el=free[i]&-free[i])) {              // While there are valid and unchecked values at [i]
                                                   //     (el = lowest '1', eg if free[i] is 0b101100, el is 0b000100)
         free[i]&=~el;                             // Mark el as checked
         if ((i+1)%N) hfree[i+1]=hfree[i]&~el;     // Value can't be used to the right
         if ((i+N)<N*N) vfree[i+N]=vfree[i]&~el;   // Value can't be used underneath
         sq[i]=el;                                 // Record in table

         //printf("free[i] is valid;  el:");print_bits(el,N);printf("\n");
         //printf("  hfree:"); print_free(hfree,N,i); printf("\n");
         //printf("  vfree:"); print_free(vfree,N,i); printf("\n");
         //printf("   free:"); print_free(free,N,i); printf("\n");
         //printf(" square:"); print_free(sq,N,i); printf("\n");

         if (i==N*N-1) {                           // Is the square complete?
            cb(sq,N);                              // Yes; Pass back to the caller
            count++;                               // Tally the solution
         } else {
            //printf("Pushing: "); print_bits(hfree[i],N); putchar(' '); print_bits(hfree[i+1],N); putchar('\n');
            i++;                                   // No; PUSH!
            free[i]=hfree[i]&vfree[i];             // Calculate available bits
            goto loop_el;                          // Start search at next i
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

   latin(sq,N,found_solution);
   //printf("Found %d solutions.\n",count);

   return 0;
}

