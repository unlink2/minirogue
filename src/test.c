#include "entity.h"
#include "fxp.h"
#include "platform.h"
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>
#include <string.h>

void test_isqrt(void **state) {
  assert_int_equal(0, mrg_isqrt(0));
  assert_int_equal(1, mrg_isqrt(1));
  assert_int_equal(1, mrg_isqrt(2));
  assert_int_equal(2, mrg_isqrt(5));
  assert_int_equal(5, mrg_isqrt(25));
  assert_int_equal(10, mrg_isqrt(100));
  assert_int_equal(31, mrg_isqrt(1000));
  assert_int_equal(100, mrg_isqrt(10000));
  assert_int_equal(316, mrg_isqrt(100000));
  assert_int_equal(1000, mrg_isqrt(1000000));
  assert_int_equal(3162, mrg_isqrt(10000000));
}

void test_fixed(void **state) {
  assert_int_equal(0xFAB, MRG_FIXED_WHOLE(0x000FAB12));
  assert_int_equal(0x123FAB, MRG_FIXED_WHOLE(0x123FAB45));

  assert_int_equal(0x12, MRG_FIXED_FRACT(0x000FAB12));
  assert_int_equal(0x45, MRG_FIXED_FRACT(0x123FAB45));
}

void test_mrg_join(void **state) {
  const char *res = mrg_join(strdup("prefix"), "/", "suffix");
  assert_string_equal("prefix/suffix", res);
  free((void *)res);
}

void test_entity_alloc(void **state) {
  struct mrg_entity_tbl tbl = mrg_entity_tbl_init();

  for (int i = 0; i < tbl.slots_len; i++) {
    int handle = mrg_entity_alloc(&tbl);
    assert_int_not_equal(-1, handle);
    assert_true((tbl.slots[handle].flags & MRG_ENTITY_FLAG_ALLOCED) > 0);
  }

  assert_int_equal(-1, mrg_entity_alloc(&tbl));

  mrg_entity_free(&tbl, 4);
  assert_int_equal(4, mrg_entity_alloc(&tbl));
  assert_int_equal(-1, mrg_entity_alloc(&tbl));

  mrg_entity_tbl_free(&tbl);
}

int main(int arc, char **argv) {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(test_isqrt),
      cmocka_unit_test(test_fixed),
      cmocka_unit_test(test_mrg_join),
      cmocka_unit_test(test_entity_alloc),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
