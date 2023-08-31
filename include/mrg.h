#ifndef mrgH__
#define mrgH__

struct mrg_config {
  _Bool verbose;
};

int mrg_main(struct mrg_config *cfg);

#endif 
