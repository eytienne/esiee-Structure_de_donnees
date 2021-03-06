#ifndef TREE_NODE_H
#define TREE_NODE_H

#include "BinarySequence.h"
#include "ShallowStack.h"
#include <stddef.h>

typedef struct TreeNode {
	void *value;
	struct TreeNode *left, *right;
} TreeNode;

enum PATHWAY { INFIX, PREFIX, POSTFIX };

enum WALK_CHECK { WALK_SUCCESS, WALK_FAILURE };

TreeNode *newTreeNode(const void *value, size_t size, TreeNode *left,
					  TreeNode *right);

void freeTreeNode(TreeNode *tn, void (*freeValue)(void *value));

int isLeaf(const TreeNode *t);

int isParent(const TreeNode *from, const TreeNode *it);

int walkExpert(const TreeNode *root, enum PATHWAY p,
				 int (*function)(const TreeNode *, void *buffer,
								 const BinarySequence *bs),
				 void *buffer);

int walk(const TreeNode *t, enum PATHWAY p,
		 int (*function)(const TreeNode *, void *buffer), void *buffer);


int transform(TreeNode* root, enum PATHWAY p,
		 int (*function)(TreeNode *, void *buffer), void *buffer);

int countTreeNodeNodes(const TreeNode *t, void *counter);

typedef struct PrintInfo {
	void (*printer)(const void *value);
	int first;
} PrintInfo;

int printTreeNode(const TreeNode *t, void *pi);

void printTreeNode2(const TreeNode *t, void (*printer)(const void *value));

int prefixPrint(const TreeNode *t, void *buffer, const BinarySequence *bs);

#endif