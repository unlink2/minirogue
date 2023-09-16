#include "arena.h"
#include "command.h"
#include "entity.h"
#include "fxp.h"
#include "idc.h"
#include "platform.h"
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdio.h>
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
  assert_int_equal(0, arena.oom);
  assert_int_equal(4, arena.aptr);
  *d1 = -2;

  int32_t *d2 = mrg_arena_malloc(&arena, 4);
  assert_non_null(d2);
  assert_int_equal(0, arena.oom);
  assert_int_equal(8, arena.aptr);
  *d2 = -3;

  int32_t *d3 = mrg_arena_malloc(&arena, 4);
  assert_int_equal(4, arena.oom);
  assert_null(d3);

  int16_t *d4 = mrg_arena_malloc(&arena, 2);
  assert_non_null(d2);
  assert_int_equal(0, arena.oom);
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
  struct mrg_arena a = mrg_arena_init(1024);
  {
    const char d[] = {
        'i', 'd', 'c', 0, 0, 0, 0, 2, 0, 0, 0, MRG_IDC_HEADER_LEN, 0, 0, 0,
        0, // end of header
        0, 0, 0, MRG_IDC_DIR_ROOM, 0, 0, 0,
        MRG_IDC_DIR_LEN * 2 + MRG_IDC_HEADER_LEN, 0, 0, 0, MRG_IDC_DIR_ENTITY,
        0, 0, 0,
        MRG_IDC_DIR_LEN * 2 + MRG_IDC_HEADER_LEN + MRG_IDC_ENTRY_LEN + 4 +
            4, // end of
               // dir
        0, 0, 0, 1, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0,
        MRG_IDC_DIR_LEN * 2 + MRG_IDC_HEADER_LEN + MRG_IDC_ENTRY_LEN, 0, 0, 0,
        MRG_IDC_DIR_LEN * 2 + MRG_IDC_HEADER_LEN + MRG_IDC_ENTRY_LEN + 4, '1',
        '2', '3', '4', '5', '6', '7', '\0', // end of room entry
        1, 2, 3, 4,                         // end of room tile map
        4, 3, 2, 1,                         // end of room flags map
        0, 0, 0, 2, 0, 0, 0, 1, 0, 0, 0, 4, 0, 0, 5, 0, 0, 0, 0, 7, '1', '2',
        '3', '4', '5', '6', '7', '\0', // end of entity entry
    };

    struct mrg_idc_file file = mrg_idc_de(&a, d, sizeof(d));

    assert_int_equal(0, file.ok);
    assert_int_equal(0x00, file.header.version);
    assert_int_equal(0x02, file.header.n_entries);
    assert_int_equal(MRG_IDC_HEADER_LEN, file.header.directory_offset);

    assert_non_null(file.dirs);

    assert_int_equal(MRG_IDC_DIR_ROOM, file.dirs[0].type);
    assert_int_equal(0x20, file.dirs[0].offset);
    assert_int_equal(1, file.dirs[0].entry->room.room_id);
    assert_int_equal(2, file.dirs[0].entry->room.room_w);
    assert_int_equal(2, file.dirs[0].entry->room.room_h);
    assert_int_equal(0x3C, file.dirs[0].entry->room.tiles_offset);
    assert_int_equal(0x40, file.dirs[0].entry->room.flags_offset);
    assert_non_null(file.dirs[0].entry->room.tiles);
    assert_non_null(file.dirs[0].entry->room.flags);
    const char expected_tiles[] = {1, 2, 3, 4};
    const char expected_flags[] = {4, 3, 2, 1};
    assert_memory_equal(expected_tiles, file.dirs[0].entry->room.tiles, 4);
    assert_memory_equal(expected_flags, file.dirs[0].entry->room.flags, 4);
    assert_string_equal("1234567", file.dirs[0].entry->room.tile_set);

    assert_int_equal(MRG_IDC_DIR_ENTITY, file.dirs[1].type);
    assert_int_equal(0x44, file.dirs[1].offset);
    assert_int_equal(2, file.dirs[1].entry->entity.room_id);
    assert_int_equal(1, file.dirs[1].entry->entity.x);
    assert_int_equal(4, file.dirs[1].entry->entity.y);
    assert_int_equal(0x500, file.dirs[1].entry->entity.flags);
    assert_int_equal(0x07, file.dirs[1].entry->entity.type);

    // reverse
    size_t len = 0;
    const char *se = mrg_idc_se(&a, &file, &len);

    assert_non_null(se);
    // assert_int_equal(sizeof(d), len);
    assert_memory_equal(d, se, len);
  }
  mrg_arena_free(&a);
}

void test_mrg_tok(void **state) {
  const size_t dst_len = 128;
  char dst[dst_len];
  memset(dst, 1, dst_len);
  size_t read = 1;

  assert_null(mrg_tok(dst, "   ", dst_len, &read));
  assert_int_equal(3, read);

  assert_null(mrg_tok(dst, "", dst_len, &read));
  assert_int_equal(0, read);

  assert_null(mrg_tok(dst, NULL, dst_len, &read));
  assert_int_equal(0, read);

  assert_string_equal("test", mrg_tok(dst, "test", dst_len, &read));
  assert_int_equal(4, read);
  assert_string_equal("test", mrg_tok(dst, "   test", dst_len, &read));
  assert_int_equal(7, read);
  assert_string_equal("another", mrg_tok(dst, "another test", dst_len, &read));
  assert_int_equal(7, read);
  assert_string_equal("another",
                      mrg_tok(dst, "   another test", dst_len, &read));
  assert_int_equal(10, read);
}

void test_mrg_arg(void **state) {
  {
    int out = 0;
    size_t read = 0;
    assert_int_equal(0, mrg_arg_int(&out, "123", &read));
    assert_int_equal(3, read);
    assert_int_equal(123, out);
  }
  {
    int out = 0;
    size_t read = 0;
    assert_int_equal(0, mrg_arg_int(&out, "0xA123", &read));
    assert_int_equal(6, read);
    assert_int_equal(0xA123, out);
  }
  {
    int out = 0;
    size_t read = 0;
    assert_int_equal(-1, mrg_arg_int(&out, "", &read));
    assert_int_equal(0, read);
  }

  {
    float out = 0;
    size_t read = 0;
    assert_int_equal(0, mrg_arg_float(&out, "123.456", &read));
    assert_int_equal(7, read);
    assert_int_equal(123, out);
  }
  {
    float out = 0;
    size_t read = 0;
    assert_int_equal(-1, mrg_arg_float(&out, "", &read));
    assert_int_equal(0, read);
  }

  {
    char buffer[64];
    size_t read = 0;
    assert_int_equal(0, mrg_arg_string("123.456", buffer, 64, &read));
    assert_int_equal(7, read);
    assert_string_equal("123.456", buffer);
  }
}

int main(int arc, char **argv) {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(test_isqrt),    cmocka_unit_test(test_fixed),
      cmocka_unit_test(test_mrg_join), cmocka_unit_test(test_entity_alloc),
      cmocka_unit_test(test_arena),    cmocka_unit_test(test_idc),
      cmocka_unit_test(test_mrg_tok),  cmocka_unit_test(test_mrg_arg)};

  return cmocka_run_group_tests(tests, NULL, NULL);
}
