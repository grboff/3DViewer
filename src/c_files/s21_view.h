#ifndef S21_VIEW_H_
#define S21_VIEW_H_

#include <ctype.h>
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LEN 255
#define OK 0
#define INVALID_FILE 1
#define ERROR 2

typedef struct {
  double x, y, z;
} point;

typedef struct data {
  double *vertices;
  size_t vertex_count;
  int *ribs;
  size_t ribs_count;
  point min_val;
  point max_val;
} data_t;

int pars_object(char *fileObj, data_t *obj);

int open_object(const char *path, data_t *obj);

void init_object(data_t *obj);

int parsing_object(FILE *fp, data_t *obj);
int parsing_vertex(char *line, double *vertex);
int parsing_ribs(char *line, int *ribs, int count_f);

int translate_neg_nums(data_t *obj);
int check_count_value(data_t *obj);

void finding_end_values(data_t *obj, double *vertex);

void centralize(data_t *obj);

void clean_object(data_t *obj);

int f_counter(char *str);

void move_by_axis(data_t *obj, int num, int axis);

void cscale(data_t *obj, float coef);

void rotate_by_x(data_t *obj, double angle);
void rotate_by_y(data_t *obj, double angle);
void rotate_by_z(data_t *obj, double angle);

#endif  // S21_VIEW_H_
