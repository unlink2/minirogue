#ifndef RAND_H_
#define RAND_H_

// set global seed 
void mrg_srand(int seed);

// call rand with custom seed 
int mrg_randa(int *tl_seed);

// call rand with global seed
int mrg_rand(void);

#endif
