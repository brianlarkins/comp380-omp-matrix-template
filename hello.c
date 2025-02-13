#include <stdio.h>
#include <stdlib.h>

#include <omp.h>


int main(int argc, char **argv) {
   int i, size;

   // ./hello [loop-iters] [#nprocs]

   switch (argc) {
   case 3:
      omp_set_num_threads(atoi(argv[2]));
      size = atoi(argv[1]);
      break;
   case 2:
      omp_set_num_threads(1);
      size = atoi(argv[1]);
      break;
   default:
      omp_set_num_threads(1);
      size = 10;
   }
      
      
   double start = omp_get_wtime();

   #pragma omp parallel for schedule(static,3)
   for (i=0;i<size;i++) {
     printf("%d: hello from %d of %d\n", i, omp_get_thread_num(), omp_get_num_threads());
   }

   #pragma omp parallel
   {
      printf("*** hi from thread %d of %d\n", omp_get_thread_num(), omp_get_num_threads());
   }

   printf("elapsed time: %12.4lf\n", omp_get_wtime()-start);

   return 0;
}
