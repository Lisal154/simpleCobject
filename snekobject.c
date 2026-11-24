#include "snekobject.h"
#include <stdlib.h>
#include <string.h>


snek_object_t *new_snek_integer(int value){
  snek_object_t *object = malloc(sizeof(snek_object_t));
  if(object == NULL){
    return NULL;
  }
  
  object->kind = INTEGER;
  object->data.v_int = value;

  return object;
}

snek_object_t *new_snek_float(float value){
  snek_object_t *object = malloc(sizeof(snek_object_t));
  if(object == NULL){
    return NULL;
  }
  
  object->kind = FLOAT;
  object->data.v_float = value;

  return object;
}


snek_object_t *new_snek_string(char *value){
  snek_object_t *object = malloc(sizeof(snek_object_t));
  if(object == NULL){
    return NULL;
  }
  
  size_t length = strlen(value) + 1;
 
  char *new_string = malloc(length);
  if(new_string == NULL){
    free(object);
    return NULL;
  }
  
  strcpy(new_string, value);

  object->kind = STRING;
  object->data.v_string = new_string;

  return object;
}

snek_object_t *new_snek_vector3(snek_object_t *x, snek_object_t *y, snek_object_t *z){
  if(x == NULL || y == NULL || z == NULL){
    return NULL;
  }
  snek_object_t *object = malloc(sizeof(snek_object_t));
  if(object == NULL){
    return NULL;
  }

  object->kind = VECTOR3;
  object->data.v_vector3.x = x;
  object->data.v_vector3.y = y;
  object->data.v_vector3.z = z;

  return object;
}

snek_object_t *new_snek_array(size_t size){
  snek_object_t *object = malloc(sizeof(snek_object_t));
  if(object == NULL){
    return NULL;
  }

  snek_object_t **pointer = calloc(size, sizeof(snek_object_t *));
  if(pointer == NULL){
    free(object);
    return NULL;
  }
  
  object->kind = ARRAY;

  snek_array_t array = {size, pointer};

  object->data.v_array = array;

  return object;
}

bool snek_array_set(snek_object_t *snekobj, size_t index, snek_object_t *value){
  if(snekobj == NULL || value == NULL){
    return false;
  }
  if(snekobj->kind != ARRAY){
    return false;
  }
  if(index+1 > snekobj->data.v_array.size){
    return false;
  }

  snekobj->data.v_array.elements[index] = value;
  return true;
}

snek_object_t *snek_array_get(snek_object_t *snekobj, size_t index){
  if(snekobj == NULL){
    return NULL;
  }
  if(snekobj->kind != ARRAY){
    return NULL;
  }
  if(index+1 > snekobj->data.v_array.size){
    return NULL;
  }
  return snekobj->data.v_array.elements[index];
}

int snek_length(snek_object_t *snekobj){
  if(snekobj == NULL){
    return -1;
  }
  if(snekobj->kind == INTEGER || snekobj->kind == FLOAT){
    return 1;
  }
  if(snekobj->kind == STRING){
    return strlen(snekobj->data.v_string);
  }
  if(snekobj->kind == VECTOR3){
    return 3;
  }
  if(snekobj->kind == ARRAY){
    return snekobj->data.v_array.size;
  }
  return -1;
}

snek_object_t *snek_add(snek_object_t *a, snek_object_t *b){
  if(a == NULL || b == NULL){
    return NULL;
  }
  
  // if a is integer
  if(a->kind == INTEGER && b->kind == INTEGER){
    snek_object_t *sum = new_snek_integer(a->data.v_int + b->data.v_int);
    return sum;
  } else if(a->kind == INTEGER && b->kind == FLOAT){
    snek_object_t *sum = new_snek_integer(a->data.v_int + b->data.v_float);
    return sum;
  }
  // if a is float
  if(a->kind == FLOAT && b->kind == INTEGER){
    snek_object_t *sum = new_snek_float(a->data.v_float + b->data.v_int);
    return sum;
  }else if (a->kind == FLOAT && b->kind == FLOAT){
    snek_object_t *sum  = new_snek_float(a->data.v_float + b->data.v_float);
    return sum; 
  }
  
  // if a is a string
  if(a->kind == STRING && b->kind != STRING){
    return NULL;
  }else if(a->kind == STRING && b-> kind == STRING){
    size_t new_length = snek_length(a) + snek_length(b);
    char *temp = calloc(new_length, sizeof(char));
    strcat(temp, a->data.v_string);
    strcat(temp, b->data.v_string);

    snek_object_t *new_string = new_snek_string(temp);
    free(temp);
    return new_string;
  }

  // if a is a vector3
  if(a->kind == VECTOR3 && b->kind != VECTOR3){
    return NULL;
  } else if(a->kind == VECTOR3 && b->kind == VECTOR3){
    snek_object_t *x = snek_add(a->data.v_vector3.x, b->data.v_vector3.x);
    snek_object_t *y = snek_add(a->data.v_vector3.y, b->data.v_vector3.y);
    snek_object_t *z = snek_add(a->data.v_vector3.z, b->data.v_vector3.z);
    snek_object_t *new_vector3 = new_snek_vector3(x, y, z);
    return new_vector3;
  }
}

