#ifndef mrgH__
#define mrgH__

struct mrg_platform; 

struct mrg_config {
  _Bool verbose;
};

struct mrg_state {
  int main_camera;

  struct mrg_platform *platform;
  const struct mrg_config *cfg;
};

int mrg_main(struct mrg_config *cfg);

#endif
