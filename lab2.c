#include <getopt.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <papi.h>

#include "omp.h"
#include "wctimer.h"

typedef double fp_t;

/*
 * prototypes
 */
void check_papi_error(int ret);
void init2d(fp_t **m, int rows, int cols, int base);
void mxm_ijk(fp_t *a, fp_t *b, fp_t *c, int s);
void mxm_ikj(fp_t *a, fp_t *b, fp_t *c, int s);
void mxm_kji(fp_t *a, fp_t *b, fp_t *c, int s);
void mxm2(fp_t *a, fp_t *b, fp_t *c, int s);
void mT(fp_t *m, int s);
double normf(fp_t *m, int s);
void print_matrix(char *mname, fp_t *m, int s);



/*
* check_papi_error() - check the return value of a PAPI call
*/
void check_papi_error(int ret) {
  if (ret != PAPI_OK) {
    fprintf(stderr, "PAPI error: %d: %s\n", ret, PAPI_strerror(ret));
    exit(1);
  }
}



/*
 * init2d() - allocates and initializes a contiguous 2-D square array
 */
void init2d(fp_t **m, int rows, int cols, int base) {
}


/*
* mxm_ijk() - multiply two square matrices using the ijk loop order
*/
void mxm_ijk(fp_t *a, fp_t *b, fp_t *c, int s) {
}


/* 
 * mxm_ikj() - multiply two square matrices using the ikj loop order
 */
void mxm_ikj(fp_t *a, fp_t *b, fp_t *c, int s) {
}


/*
 * mxm_jki() - multiply two square matrices using the jki loop order
 */
void mxm_jki(fp_t *a, fp_t *b, fp_t *c, int s) {
}


/* 
 * mxm_kji() - multiply two square matrices using the kji loop order
 */
void mxm_kji(fp_t *a, fp_t *b, fp_t *c, int s) {
}



/*
 * mxm2() - multiply two square matrices using the ijk loop order
 *          but with the inner loop unrolled by a factor of 2
 */
void mxm2(fp_t *a, fp_t *b, fp_t *c, int s) {
}


/*
 * mT() - transpose a square matrix
 */
void mT(fp_t *m, int s) {
}



/* 
 * normf() - compute the Frobenius norm of a square matrix
 */
double normf(fp_t *m, int s) {
  double result = 0.0, tmp;
  int i,j;

  for (i=0;i<s;i++) {
    for (j=0;j<s;j++) {
      tmp = m[(i*s) + j];
      result += tmp * tmp;
    }
  }
  return sqrt(result);
}



/* 
 * print_matrix() - print the contents of a square matrix
 */
void print_matrix(char *mname, fp_t *m, int s) {
  int i,j;
  if (s>5) return;

  printf("matrix %s:\n", mname);
  for (i=0;i<s;i++) {
    printf("|");
    for (j=0;j<s;j++) {
      printf(" %4.2f ", m[(i*s)+j]);
    }
    printf("|\n");
  }
  printf("norm: %7.3f\n\n", normf(m,s));
}



int main(int argc, char **argv) {
  wc_timer_t t;
  int nthreads = 1;
  int eventset = PAPI_NULL;
  long long counts[5];
  fp_t *a = NULL, *b = NULL, *c = NULL;

  if (argc < 2) {
    printf("usage: lab1 <size>\n\t<size>\t size of matrices and vectors\n");
    exit(1);
  }

  setbuf(stdout, NULL);
  wc_tsc_calibrate();   // we must always calibrate the TSC timer in every program

  int s = atoi(argv[1]);

  if (argc == 3)
    nthreads = atoi(argv[2]);

  printf("using matrix/vector size of %d -- running on %d threads\n", s, nthreads);
  omp_set_num_threads(nthreads);

  int ret = PAPI_library_init(PAPI_VER_CURRENT);
  if (ret != PAPI_VER_CURRENT) {
    fprintf(stderr, "PAPI initialization error.\n");
    exit(1);
  }

  PAPI_create_eventset(&eventset);

  ret = PAPI_add_named_event(eventset,"PAPI_TOT_INS");
  check_papi_error(ret);

  PAPI_reset(eventset);

  WC_INIT_TIMER(t);

  WC_START_TIMER(t);
  init2d(&a,s,s,1);
  init2d(&b,s,s,2);
  init2d(&c,s,s,0);
  WC_STOP_TIMER(t);
  printf("matrix/vector initialization: %10.7f ms\n", WC_READ_TIMER_MSEC(t));

  WC_INIT_TIMER(t);
  printf("computing mxm_ijk -- ");
  PAPI_start(eventset);
  WC_START_TIMER(t);
  mxm_ijk(a,b,c,s);
  WC_STOP_TIMER(t);
  PAPI_stop(eventset, counts);
  printf("matrix/matrix multiplication: %10.7f ms -- norm: %10.7f\n", WC_READ_TIMER_MSEC(t), normf(c,s));
  printf("\tPAPI total number of instructions: %lld\n", counts[0]);

  // reset timer and counters before next run
  WC_INIT_TIMER(t);
  PAPI_reset(eventset);

  PAPI_cleanup_eventset(eventset);
  PAPI_destroy_eventset(&eventset);

  free(a);
  free(b);
  free(c);
}
