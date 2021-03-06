#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>

struct Vector {
	void **values;
	size_t sizeofEach;
	int size;
	int capacity;
	int increment;
};
typedef struct Vector Vector;

Vector *newVectorExpert(int capacity, int increment, size_t sizeofEach);

Vector *newVector(size_t sizeofEach);

void freeVector(Vector *v, void (*freeValue)(void *element));

int size(const Vector *v);

int capacity(const Vector *v);

void *get(const Vector *v, int index);

int forEach(const Vector *v, const void **element);

int add(Vector *v, const void *element);

int insert(Vector *v, int index, const void *element);

void set(Vector *v, int index, const void *element);

void *removeFromVectorAtIndex(Vector *v, int index);

int delete (Vector *v, int start, int end);

int adjust(Vector *v);

#endif