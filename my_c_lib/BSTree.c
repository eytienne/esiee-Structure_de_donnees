#include "BSTree.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

TreeNode **findNodeWithParent(BSTree *t, const void *value) {
	assert(t != NULL);
	TreeNode **twp = malloc(2 * sizeof(TreeNode *));
	twp[0] = twp[1] = NULL;
	TreeNode *cur = t->root;
	while (cur != NULL) {
		int cmp = t->nodecmp(value, cur->value);
		if (cmp == 0) {
			twp[0] = cur;
			break;
		} else if (cmp > 0)
			cur = cur->right;
		else
			cur = cur->left;
		twp[1] = cur;
	}
	return twp;
}

BSTree *newTree(size_t sizeofEach,
				int (*nodecmp)(const void *newValue, const void *existing)) {
	assert(sizeofEach > 0);
	assert(nodecmp != NULL);
	BSTree *newOne = (BSTree *)malloc(sizeof(BSTree));
	newOne->root = NULL;
	newOne->sizeofEach = sizeofEach;
	newOne->nodecmp = nodecmp;
	return newOne;
}

void freeTree(BSTree *t, void (*freeValue)(void *value)) {
	assert(t != NULL);
	freeTreeNode(t->root, freeValue);
	t->sizeofEach = -1;
	t->nodecmp = NULL;
}

int countTreeNodes(const BSTree *t) {
	assert(t != NULL);
	int counter = 0;
	walk(t->root, INFIX, countTreeNodeNodes, &counter);
	return counter;
}

void printTree(const BSTree *t, void (*printer)(const void *value)) {
	assert(t != NULL);
	PrintInfo pi = {printer, 1};
	walk(t->root, INFIX, printTreeNode, &pi);
	printf("\n");
}

void printTree2(const BSTree *t, void (*printer)(const void *value)) {
	assert(t != NULL);
	printTreeNode2(t->root, printer);
	printf("\n");
}

void prefixPrintTree(const BSTree *t, void (*printer)(const void *value)) {
	walkExpert(t->root, PREFIX, prefixPrint, printer);
}

void insertIntoTree(BSTree *t, const void *newValue) {
	assert(t != NULL);
	if (t->root == NULL) {
		t->root = newTreeNode(newValue, t->sizeofEach, NULL, NULL);
		return;
	}
	TreeNode *cur = t->root;
	while (1) {
		if (t->nodecmp(newValue, cur->value) > 0) {
			if (cur->right == NULL) {
				cur->right = newTreeNode(newValue, t->sizeofEach, NULL, NULL);
				return;
			} else
				cur = cur->right;
		} else if (t->nodecmp(newValue, cur->value) <= 0) {

			if (cur->left == NULL) {
				cur->left = newTreeNode(newValue, t->sizeofEach, NULL, NULL);
				return;
			} else
				cur = cur->left;
		}
	}
}

const TreeNode *findTreeNode(BSTree *t, const void *value) {
	assert(t != NULL);
	TreeNode **twp = findNodeWithParent(t, value);
	TreeNode *ret = twp[0];
	free(twp);
	return ret;
}

typedef struct CheckInfo {
	void *last;
	int (*cmpFunc)(const void *value, const void *existing);
} CheckInfo;

int __isOrdered(const TreeNode *t, void *buffer) {
	CheckInfo *ci = (CheckInfo *)buffer;
	if (ci->last != NULL && ci->cmpFunc(t->value, ci->last) < 0)
		return ISNOTBST;
	ci->last = t->value;
	return ISBST;
}

int isOrdered(const BSTree *t) {
	assert(t != NULL);
	CheckInfo ci;
	ci.last = NULL;
	ci.cmpFunc = t->nodecmp;
	int badWalk = walk(t->root, INFIX, __isOrdered, &ci);
	return badWalk ? ISNOTBST : ISBST;
}

typedef struct SortInfo {
	void **dest;
	int index;
	size_t size;
} SortInfo;

int __treeSort(const TreeNode *t, void *si) {
	if (t == NULL)
		return WALK_SUCCESS;
	SortInfo *csi = (SortInfo *)si;
	printf("%d : %d\n", csi->index, *(int *)t->value);
	memcpy(csi->dest[csi->index], t->value, csi->size);
	printf("%d\n", csi->index);
	csi->index++;
	return WALK_SUCCESS;
}

void intprint(const void *v);

void **treeSort(void **src, int n, size_t size,
				int (*nodecmp)(const void *newValue, const void *existing)) {
	assert(size > 0);
	if (n <= 0)
		return NULL;
	assert(src != NULL);
	assert(nodecmp != NULL);

	BSTree *t = newTree(size, nodecmp);
	for (int i = 0; i < n; i++)
		insertIntoTree(t, src[i]);

	SortInfo si;
	si.dest = malloc(n * sizeof(void *));
	for (int i = 0; i < n; i++)
		si.dest[i] = malloc(t->sizeofEach);
	si.index = 0;
	si.size = t->sizeofEach;

	int failed = walk(t->root, INFIX, __treeSort, &si);

	void **ret = si.dest;
	if (failed) {
		free(ret);
		return NULL;
	}
	return ret;
}

void deleteFromTree(BSTree *t, const void *oldValue) {
	assert(t != NULL);
	TreeNode **pair = findNodeWithParent(t, oldValue);
	TreeNode *toRemove = pair[0], *parent = pair[1];
	if (toRemove == NULL)
		return;
	TreeNode *newSon = NULL;
	if (toRemove->left != NULL && toRemove->right != NULL) {
		TreeNode **spd = (TreeNode **)minNodeWithParent(toRemove->right);
		// clean min removal
		spd[1]->left = spd[0]->right;

		newSon = spd[0];
	} else if (toRemove->left == NULL ^ toRemove->right == NULL) {
		newSon = toRemove->left != NULL ? toRemove->left : toRemove->right;
	}
	if (newSon != NULL) {
		newSon->left = toRemove->left;
		newSon->right = toRemove->right;
	}
	if (parent == NULL)
		t->root = newSon;
	else {
		if (parent->left == NULL)
			parent->left = newSon;
		else
			parent->right = newSon;
	}
	free(toRemove);
}

const TreeNode **minNodeWithParent(const TreeNode *t) {
	const TreeNode **spd = malloc(2 * sizeof(TreeNode *));
	spd[0] = t;
	spd[1] = NULL;
	while (spd[0] != NULL && spd[0]->left != NULL) {
		spd[1] = t;
		spd[0] = t->left;
	}
	return spd;
}
