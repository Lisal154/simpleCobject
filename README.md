Firstly, we are going to create `snekobject.h`, and define the values like this:
```C
#include <stddef.h>

typedef enum ObjectKind{
  INTEGER
} snek_object_kind_t;

typedef union ObjectData{
  int v_int;
} snek_object_data_t;

typedef struct Object{
  snek_object_kind_t kind;
  snek_object_data_t data;
} snek_object_t;
```

# Integer

Let's create an integer, and make it able to:
- Be allocated on the heap
- Can store additional metadata about itself (for now, just its type)

Let's firstly add this to our header file: `snek_object_t *new_snek_integer(int value);
`
Then set it inside our `snekobject.c` file:
```C
#include "snekobject.h"
#include <stdlib.h>
snek_object_t *new_snek_integer(int value){
  snek_object_t *object = malloc(sizeof(snek_object_t));
  if(object == NULL){
    return NULL;
  }
  
  object->kind = INTEGER;
  object->data.v_int = value;

  return object;
}
```

We can test it by doing this inside our main function:
```C
#include "snekobject.h"
#include <stdio.h>

int main(){
  snek_object_t *int_object = new_snek_integer(20);
}
```

# Float

So, how do we store both floats and ints into the same type?

We're going to use the `union` and `enum` features in C to be able to do this.

We will need to add both `INTEGER` and `FLOAT` to our header file and our .c file. We will also need to extend our .c file to allocate memory for the new pointer.

`snekobject.h`
```C
#include <stddef.h>

typedef enum ObjectKind{
  INTEGER,
  FLOAT
} snek_object_kind_t;

typedef union ObjectData{
  int v_int;
  float v_float;
} snek_object_data_t;

typedef struct Object{
  snek_object_kind_t kind;
  snek_object_data_t data;
} snek_object_t;

snek_object_t *new_snek_integer(int value);
snek_object_t *new_snek_float(float value);
```

`snekobject.c`
```C
#include "snekobject.h"
#include <stdlib.h>
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
```

`main.c` *Just for testing purposes*
```C
#include "snekobject.h"
#include <stdio.h>

int main(){
  snek_object_t *int_object = new_snek_integer(20);
  snek_object_t *float_object = new_snek_float(3.14);

  printf("%d\n", int_object->data.v_int);
  printf("%.2f\n", float_object->data.v_float);
}
```


# String

Now, we're going to do our first object that has something... **additional** allocated.

When we allocate memory for a "snek object", that reserves memory for the object itself. Small data types like integers and floats are stored directly in the object, so there's no need for additional memory allocation.

**Strings**, however, are a different story.

Strings in C are just arrays of characters, and because they can be any length, we need to **dynamically allocate memory for the string data separately from the object itself.**
```C
char *my_string = "hello world";
```

The character array contains:
```
h
e
l
l
o

w
o
r
l
d
\0
```

`snekobject.h`
```C
#include <stddef.h>

typedef enum ObjectKind{
  INTEGER,
  FLOAT,
  STRING
} snek_object_kind_t;

typedef union ObjectData{
  int v_int;
  float v_float;
  char *v_string;
} snek_object_data_t;

typedef struct Object{
  snek_object_kind_t kind;
  snek_object_data_t data;
} snek_object_t;

snek_object_t *new_snek_integer(int value);
snek_object_t *new_snek_float(float value);
snek_object_t *new_snek_string(char *value);
```

`snekobject.c`
```C
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
```

`main.c` *Again, for testing purposes*
```C
#include "snekobject.h"
#include <stdio.h>

int main(){
  snek_object_t *int_object = new_snek_integer(20);
  snek_object_t *float_object = new_snek_float(3.14);
  snek_object_t *string_object = new_snek_string("Lisa:)");

  printf("%d\n", int_object->data.v_int);
  printf("%.2f\n", float_object->data.v_float);
  printf("%s\n", string_object->data.v_string);
}
```


# Vector3

`Vector3` is going to be the first Snek object that can hold a reference to another Snek Object. It's a collection type: *a type that holds other types*.

*Arrays, lists, Dictionaries and sets are all examples of  collection types*.

`snekobject.h`
```C
#include <stddef.h>

typedef struct Object snek_object_t;

typedef struct Vector{
  snek_object_t *x;
  snek_object_t *y;
  snek_object_t *z;
} snek_vector_t;

typedef enum ObjectKind{
  INTEGER,
  FLOAT,
  STRING,
  VECTOR3
} snek_object_kind_t;

typedef union ObjectData{
  int v_int;
  float v_float;
  char *v_string;
  snek_vector_t v_vector3;
} snek_object_data_t;

typedef struct Object{
  snek_object_kind_t kind;
  snek_object_data_t data;
} snek_object_t;


snek_object_t *new_snek_integer(int value);
snek_object_t *new_snek_float(float value);
snek_object_t *new_snek_string(char *value);
snek_object_t *new_snek_vector3(snek_object_t *x, snek_object_t *y, snek_object_t *z);
```
`snekobject.c`
```C
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
```

`main.c`
```C
#include "snekobject.h"
#include <stdio.h>

int main(){
  snek_object_t *int_object = new_snek_integer(20);
  snek_object_t *float_object = new_snek_float(3.14);
  snek_object_t *string_object = new_snek_string("Lisa:)");
  snek_object_t *vector_object = new_snek_vector3(int_object, float_object, string_object);
  printf("%d\n", int_object->data.v_int);
  printf("%.2f\n", float_object->data.v_float);
  printf("%s\n", string_object->data.v_string);
  printf("%d\n", vector_object->data.v_vector3.x->data.v_int);
  printf("%.2f\n", vector_object->data.v_vector3.y->data.v_float);
  printf("%s\n", vector_object->data.v_vector3.z->data.v_string);
}
```

Basically, first we [[Forward Declaration|forward declare]] the `snek_object_t` struct, because it is going to reference itself. 

Then we create a new struct that takes 3 values (x,  y, z), an enum, a value and the function.

After that we complete the function, by firstly checking if any of the inputs are empty, then allocating memory for the object, setting its kind (`object->kind = VECTOR3`) and finally pointing to the members of the struct. (`object->data.v_vector3.x = x;`)

# Array

Let's add a **dynamically sized array**. We'll do it in multiple parts. First, we'll just create an empty array.

Inside our header file, we are going to create all the necessary dependencies to create the array, creating the ENUM, adding members to the struct, adding the data value and finally declaring the function.
```C
#include <stddef.h>

typedef struct Object snek_object_t;

typedef struct Vector{
  snek_object_t *x;
  snek_object_t *y;
  snek_object_t *z;
} snek_vector_t;

typedef struct Array{
  size_t size;
  snek_object_t **elements;
} snek_array_t;

typedef enum ObjectKind{
  INTEGER,
  FLOAT,
  STRING,
  VECTOR3,
  ARRAY
} snek_object_kind_t;

typedef union ObjectData{
  int v_int;
  float v_float;
  char *v_string;
  snek_vector_t v_vector3;
  snek_array_t v_array;
} snek_object_data_t;

typedef struct Object{
  snek_object_kind_t kind;
  snek_object_data_t data;
} snek_object_t;


snek_object_t *new_snek_integer(int value);
snek_object_t *new_snek_float(float value);
snek_object_t *new_snek_string(char *value);
snek_object_t *new_snek_vector3(snek_object_t *x, snek_object_t *y, snek_object_t *z);
snek_object_t *new_snek_array(size_t size);
```

Now inside our `.c` file we can set our function. Firstly, it will accept a size parameter and allocate memory for our object. After that, we will create a new pointer that will take in an array of objects (`snek_object_t **`). For this, we will use the [[calloc |calloc]] function, and pass in our `size` for the number of elements and `sizeof(snek_object_t *)` for the size of each element.

Then we will set the kind field, and create a new `snek_array_t` struct with the `size` and `elements`, and set the `v_array` field with the newly created pointer.

After all that we can simply return the object pointer.
```C
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
```

The reason why we declared `snek_array_t array = {size, pointer};` is to not initialize them individually like:
```C
object->data.v_array.size = size;
object->data.v_array.elements = pointer;
```


# Set

Our new function `snek_array_set` sets a value at a specific index in the array.

*example(in python)*:`array[3] = new_value`

As we have done in other functions, we first declare our function inside our header file and then we start editing it in our `.c` file.

1. We check if the object or the new value is `NULL`, in which we will return `false`
2. We check if the object's `kind` is `ARRAY`, if not return `false`
3. We check if our `index` is out of bounds, while remembering our `v_array`'s `size` field.
4. We set the value in the `elements` array at the specified index, and return `true`.

```C
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
```

# Get

Now that we can store values in arrays, we need a way to retrieve them.

Our new function `snek_array_get` will do just that.

Now in our function we will:
1. check if `object` is null
2. check if `kind` is array
3. check if `index` is out of bounds
4. return the value in the `elements` array at the specified index.

```C
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
```

# Length

Now that we have all sorts of objcects (int, float, string, array etc.), we need a way to get the `length` of any Snek object.

We will create the `snek_length` function:

1. If the input is `NULL` return `-1` (which we've chosen to indicate failure).
2. If the input is an integer, its size is always `1`.
3. If the input is a float, its size is always `1`.
4. If the input is a string, its size is the lenht of the string in bytes
5. If the input is a `VECTOR3`, its size is always `3`.   
6. If the input is an array, its size is stored in the `v_array.size` field.    
7. If the input is none of the above, return `-1`.
```C
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
```

# Add

The Add function allows developers to specify custom logic for different types of objects. *For example,* adding two integers behave like this:
```python
3 + 5
# 8
```

But with an array, we probably want it to work like this:
```python
[3] + [5]
# [3, 5]
```

Our add function will accept 2 pointers to `snek_object_t` object("a" and "b") and will return a pointer to a new `snek_object_t` object.


