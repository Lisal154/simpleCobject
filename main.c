#include "snekobject.h"
#include <stdio.h>
#include <stdbool.h>

int main(){
  snek_object_t *int_object = new_snek_integer(10);
  snek_object_t *int_object2 = new_snek_integer(20);
  snek_object_t *float_object = new_snek_float(2.3);
  snek_object_t *float_object2 = new_snek_float(1.1);
  snek_object_t *string_object = new_snek_string("hii");
  snek_object_t *vector_object = new_snek_vector3(int_object, float_object, string_object);
  snek_object_t *array_object = new_snek_array(3);
  
  snek_array_set(array_object, 1, float_object);
  bool a = snek_array_set(array_object, 2, int_object);
  printf("%.2lf\n", snek_add(float_object, int_object2)->data.v_float);
}
