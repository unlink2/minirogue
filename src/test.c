#include "arena.h"
#include "entity.h"
#include "fxp.h"
#include "idc.h"
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

  assert_int_equal(0x000FAB12, MRG_FIXED(0xFAB, 0x12));
}

void test_mrg_join(void **state) {
  struct mrg_arena a = mrg_arena_init(1);
  const char *res = mrg_join(&a, "prefix", "/", "suffix");
  assert_string_equal("prefix/suffix", res);
  mrg_arena_free(&a);
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

void test_arena(void **state) {
  struct mrg_arena arena = mrg_arena_init(10);
  assert_int_equal(10, arena.arena_len);

  int32_t *d1 = mrg_arena_malloc(&arena, 4);
  assert_non_null(d1);
  assert_int_equal(4, arena.aptr);
  *d1 = -2;

  int32_t *d2 = mrg_arena_malloc(&arena, 4);
  assert_non_null(d2);
  assert_int_equal(8, arena.aptr);
  *d2 = -3;

  int32_t *d3 = mrg_arena_malloc(&arena, 4);
  assert_null(d3);

  int16_t *d4 = mrg_arena_malloc(&arena, 2);
  assert_non_null(d2);
  assert_int_equal(10, arena.aptr);
  *d4 = -4;

  assert_int_equal(-2, *d1);
  assert_int_equal(-3, *d2);
  assert_int_equal(-4, *d4);

  void *before = arena.data;
  assert_int_equal(0, mrg_arena_resize(&arena, 9));
  assert_ptr_equal(before, arena.data);
  assert_int_equal(10, arena.arena_len);

  assert_int_equal(0, mrg_arena_resize(&arena, 11));
  assert_int_equal(11, arena.arena_len);

  mrg_arena_clear(&arena);
  assert_int_equal(0, arena.aptr);

  mrg_arena_free(&arena);
}

void test_idc(void **test) {
  struct mrg_arena a = mrg_arena_init(16);
  {
    const char d[] = {'i', 'd', 'c', 0, 0, 0, 0, 4, 0, 0, 0, MRG_IDC_HEADER_LEN,
                      0,   0,   0,   0, 14};
    struct mrg_idc_file file = mrg_idc_de(&a, d, sizeof(d));

    assert_int_equal(0x00, file.header.version);
    assert_int_equal(0x04, file.header.n_entries);
    assert_int_equal(MRG_IDC_HEADER_LEN, file.header.directory_offset);

    assert_int_equal(0, file.ok);
  }
  mrg_arena_free(&a);
}

int main(int arc, char **argv) {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(test_isqrt),    cmocka_unit_test(test_fixed),
      cmocka_unit_test(test_mrg_join), cmocka_unit_test(test_entity_alloc),
      cmocka_unit_test(test_arena),    cmocka_unit_test(test_idc)};

  return cmocka_run_group_tests(tests, NULL, NULL);
}
