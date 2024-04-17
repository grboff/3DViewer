#include "s21_view.h"

/*======================PARSING======================*/
int open_object(const char *path, data_t *obj) {
  int status = OK;
  FILE *fp = fopen(path, "r");
  if (fp) {
    status = parsing_object(fp, obj);
    fclose(fp);
  } else {
    status = INVALID_FILE;
    fprintf(stderr, "\tError: Invalid filename or unable to open file: %s\n",
            path);
  }
  return status;
}

// инициализация структуры
void init_object(data_t *obj) {
  obj->vertices = NULL;
  obj->vertex_count = 0;
  obj->ribs = NULL;
  obj->ribs_count = 0;

  obj->max_val.x = DBL_MIN;
  obj->min_val.x = DBL_MAX;
  obj->max_val.y = DBL_MIN;
  obj->min_val.y = DBL_MAX;
  obj->max_val.z = DBL_MIN;
  obj->min_val.z = DBL_MAX;
}

int parsing_object(FILE *fp, data_t *obj) {
  int status = OK;
  char line[LEN];
  init_object(obj);
  // Чтение данных из файла
  while (fgets(line, LEN, fp) != NULL) {
    if (strncmp(line, "v ", 2) == 0) {
      // Обработка строки с вершиной
      double vertex[3];
      status = parsing_vertex(line, vertex);
      if (status != OK) {
        clean_object(obj);
        break;
      }

      // Добавление вершины в массив vertices
      double *new_vertices =
          (double *)calloc(obj->vertex_count + 3, sizeof(double));
      if (new_vertices == NULL) {
        status = ERROR;  // Ошибка выделения памяти
        break;
      }
      memcpy(new_vertices, obj->vertices, obj->vertex_count * sizeof(double));
      free(obj->vertices);

      obj->vertices = new_vertices;
      obj->vertices[obj->vertex_count++] = vertex[0];
      obj->vertices[obj->vertex_count++] = vertex[1];
      obj->vertices[obj->vertex_count++] = vertex[2];

      finding_end_values(obj, vertex);

    } else if (strncmp(line, "f ", 2) == 0) {
      // Обработка строки с ребром
      int count_f = f_counter(line);
      int ribs[count_f];

      status = parsing_ribs(line, ribs, count_f);
      if (status != OK) {
        clean_object(obj);
        break;
      }

      // Добавление ребра в массив ribs
      int *new_ribs =
          (int *)calloc(obj->ribs_count + (count_f * 2), sizeof(int));
      if (new_ribs == NULL) {
        status = ERROR;  // Ошибка выделения памяти
        break;
      }
      memcpy(new_ribs, obj->ribs, obj->ribs_count * sizeof(int));
      free(obj->ribs);
      obj->ribs = new_ribs;
      for (int i = 0; i < count_f; i++) {
        obj->ribs[obj->ribs_count++] = ribs[i];
        if (i != count_f - 1)
          obj->ribs[obj->ribs_count++] = ribs[i + 1];
        else
          obj->ribs[obj->ribs_count++] = ribs[0];
      }
    }
  }

  if (!status && translate_neg_nums(obj)) status = INVALID_FILE;
  if (!status && check_count_value(obj)) status = INVALID_FILE;

  return status;
}

int parsing_vertex(char *line, double *vertex) {
  int status = OK;
  // Пропуск префикса "v " в строке
  char *data = line + 2;

  // Разбиение строки на координаты вершины
  char *token = strtok(data, " ");
  for (int i = 0; i < 3; i++) {
    if (token == NULL) {
      status = INVALID_FILE;
      fprintf(stderr, "\tError: Not enough coordinates\n");
      break;
    }

    // vertex[i] = atof(token);
    char *buf;
    vertex[i] = strtod(token, &buf);
    if (vertex[i] == 0 && buf == token) {
      status = INVALID_FILE;
      fprintf(stderr, "\tError: In input vertex not a number\n");
      break;
    }
    token = strtok(NULL, " ");
  }

  return status;
}

int parsing_ribs(char *line, int *ribs, int count_f) {
  int status = OK;
  // Пропуск префикса "f " в строке
  char *data = line + 2;

  // Разбиение строки на индексы ребра
  char *token = strtok(data, " ");
  for (int i = 0; i < count_f; i++) {
    if (token == NULL) {
      status = INVALID_FILE;
      fprintf(stderr, "\tError: Not enough indexes\n");
      break;
    }

    // Извлечение только первого индекса вершины (нам нужны только ребра)
    char *slash = strchr(token, '/');
    if (slash != NULL) {
      *slash = '\0';
    }

    // ribs[i] = atoi(token);
    char *buf;
    ribs[i] = (int)strtod(token, &buf);
    if (ribs[i] == 0 && buf == token) {
      status = INVALID_FILE;
      fprintf(stderr, "\tError: In input ribs not a number\n");
      break;
    }
    token = strtok(NULL, " ");
  }

  return status;
}

int translate_neg_nums(data_t *obj) {
  int status = OK;
  for (size_t i = 0; i < obj->ribs_count && !status; i++) {
    int temp = obj->ribs[i];
    if (temp == 0) {
      status = INVALID_FILE;
      fprintf(stderr, "\tError: Invalid vertex - zero index\n");
      break;
    } else if (temp < 0) {
      obj->ribs[i] = obj->vertex_count / 3 + temp;
    } else {
      obj->ribs[i] = temp - 1;
    }
  }
  return status;
}

int check_count_value(data_t *obj) {
  int status = OK;
  if (!(obj->vertex_count > 0) || !(obj->ribs_count > 0)) {
    status = INVALID_FILE;
    fprintf(stderr, "\tError: The model does not contain any meshes\n");
  }
  int max_rib = 0;
  for (size_t i = 0; i < obj->ribs_count && !status; i++) {
    if (obj->ribs[i] > max_rib) max_rib = obj->ribs[i];
  }
  if (!status && obj->vertex_count / 3 < (size_t)max_rib + 1) {
    status = INVALID_FILE;
    fprintf(stderr,
            "\tError: Invalid vertex index. Vert id: %d, but vert count: %lu\n",
            max_rib + 1, obj->vertex_count / 3);
  }

  return status;
}

void finding_end_values(data_t *obj, double *vertex) {
  if (obj->max_val.x < vertex[0])
    obj->max_val.x = vertex[0];
  else if (obj->min_val.x > vertex[0])
    obj->min_val.x = vertex[0];

  if (obj->max_val.y < vertex[1])
    obj->max_val.y = vertex[1];
  else if (obj->min_val.y > vertex[1])
    obj->min_val.y = vertex[1];

  if (obj->max_val.z < vertex[2])
    obj->max_val.z = vertex[2];
  else if (obj->min_val.z > vertex[2])
    obj->min_val.z = vertex[2];
}

/*======================CLEAN======================*/
void clean_object(data_t *obj) {
  if (obj->vertices != NULL) free(obj->vertices);
  if (obj->ribs != NULL) free(obj->ribs);
  obj->vertices = NULL;
  obj->vertex_count = 0;
  obj->ribs = NULL;
  obj->ribs_count = 0;
}

/*======================TRANSFORM======================*/
/// @brief Смещение вершины по оси x,y,z
/// @param obj Объект
/// @param num Значение смщения
/// @param axis x:0 / y:1 / z:2
void move_by_axis(data_t *obj, int num, int axis) {
  for (size_t i = axis; i < obj->vertex_count; i += 3) obj->vertices[i] -= num;
}

void cscale(data_t *obj, float coef) {
  // check?
  if (coef < 0) {
    for (size_t i = 0; i < obj->vertex_count; i++) {
      obj->vertices[i] *= 1.1;
      //            printf("%f vert %f coef\n", obj->vertices[i], coef);
    }
  } else if (coef > 0) {
    for (size_t i = 0; i < obj->vertex_count; i++) {
      obj->vertices[i] /= 1.1;
      //            printf("%f vert %f coef\n", obj->vertices[i], coef);
    }
  }
}

void rotate_by_x(data_t *obj, double angle) {
  float angleRad = angle * (M_PI / 180);
  for (size_t i = 0; i < obj->vertex_count; i = i + 3) {
    double arrayY = obj->vertices[i + 1];
    double arrayZ = obj->vertices[i + 2];
    obj->vertices[i + 1] = cos(angleRad) * arrayY + sin(angleRad) * arrayZ;
    obj->vertices[i + 2] = -sin(angleRad) * arrayY + cos(angleRad) * arrayZ;
  }
}

void rotate_by_y(data_t *obj, double angle) {
  float angleRad = angle * (M_PI / 180);
  for (size_t i = 0; i < obj->vertex_count; i = i + 3) {
    double arrayX = obj->vertices[i];
    double arrayZ = obj->vertices[i + 2];
    obj->vertices[i] = cos(angleRad) * arrayX + sin(angleRad) * arrayZ;
    obj->vertices[i + 2] = -sin(angleRad) * arrayX + cos(angleRad) * arrayZ;
  }
}

void rotate_by_z(data_t *obj, double angle) {
  float angleRad = angle * (M_PI / 180);
  for (size_t i = 0; i < obj->vertex_count; i = i + 3) {
    double arrayX = obj->vertices[i];
    double arrayY = obj->vertices[i + 1];
    obj->vertices[i] = cos(angleRad) * arrayX + sin(angleRad) * arrayY;
    obj->vertices[i + 1] = -sin(angleRad) * arrayX + cos(angleRad) * arrayY;
  }
}

void centralize(data_t *obj) {
  for (size_t i = 0; i < obj->vertex_count; i += 3) {
    obj->vertices[i] -= (obj->min_val.x + obj->max_val.x) / 2;
    obj->vertices[i + 1] -= (obj->min_val.y + obj->max_val.y) / 2;
    obj->vertices[i + 2] -= (obj->min_val.z + obj->max_val.z) / 2;
  }
}

/*======================OTHER======================*/
int f_counter(char *str) {
  int count = 0;
  char *data = NULL, *data1 = NULL;
  while (*str != '\0') {
    if (*str >= '0' && *str <= '9') {
      data = str - 1;
      data1 = str - 2;
      if (*data == ' ' || (*data == '-' && *data1 == ' ')) {
        count++;
      }
    }
    str++;
  }
  return count;
}

int pars_object(char *fileObj, data_t *obj) {
  const char *filename = fileObj;

  int status = open_object(filename, obj);
  centralize(obj);

  if (status == OK) {
    printf("\nVERTICES: %zu\n", obj->vertex_count);
    for (size_t i = 0; i < obj->vertex_count; i += 3) {
      printf("%lu:v %.3f %.3f %.3f\n", i + 3, obj->vertices[i],
             obj->vertices[i + 1], obj->vertices[i + 2]);
    }

    printf("\nRIBS: %zu\n", obj->ribs_count);
    for (size_t i = 0; i < obj->ribs_count; i += 2) {
      printf("%lu:f %d %d\n", i + 2, obj->ribs[i], obj->ribs[i + 1]);
    }

    printf("max_x: %f | min_x: %f\n", obj->max_val.x, obj->min_val.x);
    printf("max_y: %f | min_y: %f\n", obj->max_val.y, obj->min_val.y);
    printf("max_z: %f | min_z: %f\n", obj->max_val.z, obj->min_val.z);
  } else if (status == INVALID_FILE) {
    fprintf(stderr, "\tError: Invalid file: %s\n", filename);
  } else {
    fprintf(stderr, "\tError: Failed to allocate memory");
  }
  return status;
}

// int main() {
//   data_t obj;
//   int status = pars_object("../obj_files/wrong file 1.obj", &obj);
//   if (!status) clean_object(&obj);
//   printf("STATUS: %d", status);
//   return 0;
// }
