#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
 * This is a program to benchmark of the improvements of using logical operations
 * over expensive instructions such as mul, div or mod
 */

#define NUM_ELEMENTS_L1 2048       // For L1 cache
#define NUM_ELEMENTS_L2 786432     // For L2 cache
#define NUM_ELEMENTS_L3 2097152    // For L3 cache
#define NUM_ITERATIONS 100
#define MOD 8

double times[NUM_ITERATIONS];

int random_int() {
  return rand() + 1;
}

/*
 * Takes in a duration, and returns a double[] where 
 * 0 -> total duration
 * 1 -> average duration
 * 2 -> std dev
 */
double* get_results(double duration) {
  double* result = (double *)malloc(3*sizeof(double));
  result[0] = duration;                   // total time spent
  result[1] = duration / NUM_ITERATIONS;  // average time per iteration
  result[2] = 0;                          // std deviation

  for(size_t i = 0; i < NUM_ITERATIONS; ++i) {
    result[2] += pow(times[i] - result[1], 2);
  }
  result[2] = sqrt(result[2] / NUM_ITERATIONS);
  return result;
}

/*
 * Performs x mod N using the logical instructions
 * a double[] containing the statistics of the time spent in using the operations.
 */
double* logical_operation(int *x, int num_elements) {
  double duration = 0.f;
  int* y = (int *)malloc(num_elements * sizeof(int));

  if(!y) {
    fprintf(stderr, "Memory allocation failed for y\n");
    exit(EXIT_FAILURE);
  }

  for(size_t i = 0; i < NUM_ITERATIONS; ++i) {

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    for(size_t i = 0; i < num_elements; ++i) {
        y[i] = (x[i] & (MOD - 1));
    }
    clock_gettime(CLOCK_MONOTONIC, &end);

    double iter_duration = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    times[i] = iter_duration;
    duration += iter_duration;
  }

  free(y);
  return get_results(duration);
}


/*
 * Performs x mod N using the % instructions
 * a double[] containing the statistics of the time spent in using the operations.
 */
double* mod_operation(int *x, int num_elements) {
  double duration = 0.f;
  int* y = (int *)malloc(num_elements * sizeof(int));

  if(!y) {
    fprintf(stderr, "Memory allocation failed for y\n");
    exit(EXIT_FAILURE);
  }

  for(size_t i = 0; i < NUM_ITERATIONS; ++i) {

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    for(size_t i = 0; i < num_elements; ++i) {
      y[i] = x[i] % MOD;
    }
    clock_gettime(CLOCK_MONOTONIC, &end);

    double iter_duration = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    times[i] = iter_duration;
    duration += iter_duration;
  }

  free(y);
  return get_results(duration);
}

void benchmarking(long num_elements) {
  int* x = (int*)malloc(num_elements * sizeof(int));

  if(!x) {
    fprintf(stderr, "Memory allocation failed for x\n");
    exit(EXIT_FAILURE);
  }
  
  for(size_t i = 0; i < num_elements; ++i) {
    x[i] = random_int();
    // x[i] = i;
  }

  printf("Warming up the cache...\n");
  /*
   * Warm-up iterations: 
   * The first iteration might include cache misses, setup or CPU throttling effects.
   * This will bias the results
   */
  for(size_t i = 0; i < 10; ++i) {
    mod_operation(x, num_elements);
    logical_operation(x, num_elements);
  }

  printf("================================\n");
  printf("Using MOD Instruction:\n");
  double* mod_result = mod_operation(x, num_elements);
  printf("Total time: %f seconds\n", mod_result[0]);
  printf("Average time: %f seconds\n", mod_result[1]);
  printf("Standard Dev: %f seconds\n", mod_result[2]);


  printf("================================\n");
  printf("Using Logical Instructions:\n");
  double* log_result = logical_operation(x, num_elements);
  printf("Total time: %f seconds\n", log_result[0]);
  printf("Average time: %f seconds\n", log_result[1]);
  printf("Standard Dev: %f seconds\n", log_result[2]);

  free(x);
  free(mod_result);
  free(log_result);

  printf("\n\n");
}

int main() {
  printf("\n\nBenchmarking for L1 cache...\n");
  benchmarking(NUM_ELEMENTS_L1);

  printf("\nBenchmarking for L2 cache...\n");
  benchmarking(NUM_ELEMENTS_L2);

  printf("\nBenchmarking for L3 cache...\n");
  benchmarking(NUM_ELEMENTS_L3);

  return 0;
}
