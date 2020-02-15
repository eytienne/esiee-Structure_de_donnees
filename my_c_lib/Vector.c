#include "Vector.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// private method
int changeCapactity(Vector *v, size_t newCapacity);

void create_expert(Vector *v, int capacity, int increment, size_t sizeofEach) {
	assert(v != NULL);
	v->capacity = capacity;
	v->sizeofEach = sizeofEach;
	v->values = malloc(v->capacity * v->sizeofEach);
	v->size = 0;
	v->increment = increment;
}

void create(Vector *v, size_t sizeofEach) {
	create_expert(v, 10, 3, sizeofEach);
}

void destroy(Vector *v) {
	assert(v != NULL);
	for (int i = 0; i < v->size; i++) {
		free(&v->values[i]);
	}
	free(v->values);
	v->values = NULL;
	v->capacity = 0;
	v->size = 0;
}

int size(const Vector *v) { return v->size; }

int capacity(const Vector *v) { return v->capacity; }

void *get(const Vector *v, int index) {
	assert(index >= 0 && index < v->size);
	int i = 0;
	printf("step %d\n", i++);
	if (v->values[index] == NULL)
		return NULL;

	printf("step %d\n", i++);
	void *ret = malloc(v->sizeofEach);

	printf("step %d (%p)\n", i++, v->values[index]);
	memcpy(ret, v->values[index], v->sizeofEach);
	return ret;
}

int add(Vector *v, const void *element) {
	assert(v != NULL);
	return insert(v, v->size, element);
}

int insert(Vector *v, int index, const void *element) {
	assert(v != NULL);
	assert(element != NULL);
	assert(index >= 0);

	if (v->size + 1 > v->capacity) {
		int newCapacity = v->capacity + v->increment;
		int failed = changeCapactity(v, newCapacity);
		if (failed)
			return failed;
	}

	if (index < v->size) {
		for (int i = v->size; i < index; i--)
			v->values[i] = v->values[i - 1];
	}

	void *newOne = malloc(sizeof(v->sizeofEach));
	memcpy(newOne, element, v->sizeofEach);
	v->values[index] = newOne;

	v->size++;
	return 0;
}

void set(Vector *v, int index, const void *element) {
	assert(v != NULL);
	assert(element != NULL);
	assert(index >= 0 && index < v->size);
	memcpy(v->values[index], element, v->sizeofEach);
}

void *removeFromVectorAtIndex(Vector *v, int index) {
	assert(v != NULL);
	void *removed = get(v, index);
	delete (v, index, index + 1);
	return removed;
}

int delete (Vector *v, int start, int end) {
	assert(v != NULL);
	assert(start >= 0 && start < v->size && start < end);

	int nbRemoved = (end - start);
	int i = start;
	for (; i < start + nbRemoved, i + nbRemoved < v->size; i++)
		v->values[i] = v->values[i + nbRemoved];
	for (; i < v->size; i++)
		v->values[0];

	v->size -= nbRemoved;
	int newCapacity =
		v->capacity - (v->capacity - v->size) / v->increment * v->increment;
	if (newCapacity < v->capacity) {
		int failed = changeCapactity(v, newCapacity);
		if (failed)
			return failed;
	}
	return 0;
}

int adjust(Vector *v) {
	assert(v != NULL);
	if (v->size < v->capacity)
		return changeCapactity(v, v->size);
	return 0;
}

int changeCapactity(Vector *v, size_t newCapacity) {
	void **newValues = realloc(v->values, newCapacity * v->sizeofEach);
	if (newValues == NULL)
		return EXIT_FAILURE;
	v->values = newValues;
	v->capacity = newCapacity;
	return EXIT_SUCCESS;
}