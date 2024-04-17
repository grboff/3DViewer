#include <check.h>

#include "../c_files/s21_view.h"

START_TEST(test_1_1) {
  data_t obj;
  int status = pars_object("obj_files/cube.obj", &obj);
  ck_assert_int_eq(status, 0);
  clean_object(&obj);
}
END_TEST

START_TEST(test_1_2) {
  data_t obj;
  int status = open_object("obj_files/cube neg.obj", &obj);
  ck_assert_int_eq(status, 0);
  clean_object(&obj);
}
END_TEST

START_TEST(test_1_3) {
  data_t obj;
  int status = open_object("obj_files/cube 4 ribs.obj", &obj);
  ck_assert_int_eq(status, 0);
  clean_object(&obj);
}
END_TEST

START_TEST(test_1_4) {
  data_t obj;
  int status = open_object("obj_files/cube dif forse.obj", &obj);
  ck_assert_int_eq(status, 0);
  clean_object(&obj);
}
END_TEST

START_TEST(test_1_5) {
  data_t obj;
  int status = open_object("obj_files/cow.obj", &obj);
  ck_assert_int_eq(status, 0);
  clean_object(&obj);
}
END_TEST

START_TEST(invalid_file_1) {
  data_t obj;
  int status = open_object("obj_files/Kalyaka.obj", &obj);
  ck_assert_int_eq(status, 1);
}
END_TEST

START_TEST(invalid_file_2) {
  data_t obj;
  int status = open_object("obj_files/worng file.obj", &obj);
  ck_assert_int_eq(status, 1);
}
END_TEST

START_TEST(invalid_file_3) {
  data_t obj;
  int status = open_object("obj_files/worng file 1.obj", &obj);
  ck_assert_int_eq(status, 1);
}
END_TEST

START_TEST(invalid_file_4) {
  data_t obj;
  int status = open_object("obj_files/worng file 2.obj", &obj);
  ck_assert_int_eq(status, 1);
}
END_TEST

START_TEST(invalid_file_5) {
  data_t obj;
  int status = open_object("obj_files/worng file 3.obj", &obj);
  ck_assert_int_eq(status, 1);
}
END_TEST

START_TEST(invalid_file_6) {
  data_t obj;
  int status = open_object("obj_files/worng file 4.obj", &obj);
  ck_assert_int_eq(status, 1);
}
END_TEST

START_TEST(invalid_file_7) {
  data_t obj;
  int status = open_object("obj_files/worng file 5.obj", &obj);
  ck_assert_int_eq(status, 1);
}
END_TEST

START_TEST(invalid_file_8) {
  data_t obj;
  int status = open_object("obj_files/worng file 6.obj", &obj);
  ck_assert_int_eq(status, 1);
}
END_TEST

START_TEST(test_2_1) {
  data_t obj, obj1;
  int status = open_object("obj_files/cube.obj", &obj);
  ck_assert_int_eq(status, 0);
  int status1 = open_object("obj_files/cube.obj", &obj1);
  ck_assert_int_eq(status1, 0);
  move_by_axis(&obj, 10, 0);
  move_by_axis(&obj, 10, 1);
  move_by_axis(&obj, 10, 2);

  for (size_t i = 0; i < obj.vertex_count; i++) {
    ck_assert_double_eq(obj.vertices[i], obj1.vertices[i] - 10);
  }

  clean_object(&obj);
  clean_object(&obj1);
}
END_TEST

START_TEST(test_2_2) {
  data_t obj, obj1;
  int status = open_object("obj_files/cube.obj", &obj);
  ck_assert_int_eq(status, 0);
  int status1 = open_object("obj_files/cube.obj", &obj1);
  ck_assert_int_eq(status1, 0);
  cscale(&obj, 4);

  for (size_t i = 0; i < obj.vertex_count; i++) {
    ck_assert_double_eq(obj.vertices[i], obj1.vertices[i] / 1.1);
  }

  clean_object(&obj);
  clean_object(&obj1);
}
END_TEST

START_TEST(test_2_3) {
  data_t obj, obj1;
  int status = open_object("obj_files/cube.obj", &obj);
  centralize(&obj);
  int status1 = open_object("obj_files/cube.obj", &obj1);
  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(status1, 0);

  for (size_t i = 0; i < obj.vertex_count; i++) {
    ck_assert_double_eq(obj.vertices[i], (obj1.vertices[i] == 1) ? 0.5 : -0.5);
  }

  clean_object(&obj);
  clean_object(&obj1);
}
END_TEST

START_TEST(test_2_4) {
  data_t obj, obj1;
  int status = open_object("obj_files/cube.obj", &obj);
  int status1 = open_object("obj_files/cube.obj", &obj1);
  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(status1, 0);
  rotate_by_x(&obj, 0);

  for (size_t i = 0; i < obj1.vertex_count; i += 3) {
    double *vertex = obj1.vertices + i;
    double y = vertex[1];
    double z = vertex[2];
    vertex[1] = y * cos(0) + z * sin(0);
    vertex[2] = -y * sin(0) + z * cos(0);
    ck_assert_double_eq(obj.vertices[i + 1], vertex[1]);
    ck_assert_double_eq(obj.vertices[i + 2], vertex[2]);
  }

  clean_object(&obj);
  clean_object(&obj1);
}
END_TEST

START_TEST(test_2_5) {
  data_t obj, obj1;
  int status = open_object("obj_files/cube.obj", &obj);
  int status1 = open_object("obj_files/cube.obj", &obj1);
  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(status1, 0);
  rotate_by_y(&obj, 0);

  for (size_t i = 0; i < obj1.vertex_count; i += 3) {
    double *vertex = obj1.vertices + i;
    double x = vertex[0];
    double z = vertex[2];
    vertex[0] = x * cos(0) + z * sin(0);
    vertex[2] = -x * sin(0) + z * cos(0);
    ck_assert_double_eq(obj.vertices[i], vertex[0]);
    ck_assert_double_eq(obj.vertices[i + 2], vertex[2]);
  }

  clean_object(&obj);
  clean_object(&obj1);
}
END_TEST

START_TEST(test_2_6) {
  data_t obj, obj1;
  int status = open_object("obj_files/cube.obj", &obj);
  int status1 = open_object("obj_files/cube.obj", &obj1);
  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(status1, 0);
  rotate_by_z(&obj, 0);

  for (size_t i = 0; i < obj1.vertex_count; i += 3) {
    double *vertex = obj1.vertices + i;
    double x = vertex[0];
    double y = vertex[1];
    vertex[0] = x * cos(0) - y * sin(0);
    vertex[1] = -x * sin(0) + y * cos(0);
    ck_assert_double_eq(obj.vertices[i], vertex[0]);
    ck_assert_double_eq(obj.vertices[i + 1], vertex[1]);
  }

  clean_object(&obj);
  clean_object(&obj1);
}
END_TEST

int main() {
  Suite *s = suite_create("viewer_test");
  TCase *tc_viewer = tcase_create("viewer_test");
  SRunner *sr = srunner_create(s);

  suite_add_tcase(s, tc_viewer);
  tcase_add_test(tc_viewer, test_1_1);
  tcase_add_test(tc_viewer, test_1_2);
  tcase_add_test(tc_viewer, test_1_3);
  tcase_add_test(tc_viewer, test_1_4);
  tcase_add_test(tc_viewer, test_1_5);

  tcase_add_test(tc_viewer, invalid_file_1);
  tcase_add_test(tc_viewer, invalid_file_2);
  tcase_add_test(tc_viewer, invalid_file_3);
  tcase_add_test(tc_viewer, invalid_file_4);
  tcase_add_test(tc_viewer, invalid_file_5);
  tcase_add_test(tc_viewer, invalid_file_6);
  tcase_add_test(tc_viewer, invalid_file_7);
  tcase_add_test(tc_viewer, invalid_file_8);

  tcase_add_test(tc_viewer, test_2_1);
  tcase_add_test(tc_viewer, test_2_2);
  tcase_add_test(tc_viewer, test_2_3);
  tcase_add_test(tc_viewer, test_2_4);
  tcase_add_test(tc_viewer, test_2_5);
  tcase_add_test(tc_viewer, test_2_6);

  srunner_set_fork_status(sr, CK_NOFORK);
  srunner_run_all(sr, CK_ENV);
  int res = srunner_ntests_failed(sr);
  srunner_free(sr);
  return res == 0 ? 0 : 1;
}
