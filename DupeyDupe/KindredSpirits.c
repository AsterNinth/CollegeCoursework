// Name    : Kyle Pazmino
// NID     : Ky514084
// Class   : COP 3502C
// Semester: Spring 2019

#include <stdlib.h>
#include <stdio.h>
#include "KindredSpirits.h"

// Note: [#] indicates there is more info at the bottom of the file.

// ==============
// HELPER STRUCTS
// ==============

// These two structs are used in kindredSpirits()
// to compare the pre and post orders of the trees
typedef struct Cell
{
	int data;
	struct Cell *next;
} Cell;

// We have a tail pointer in our list struct so that inserting
// at the tail goes from O(n) runtime to O(1) runtime
typedef struct List
{
	int size;
	Cell *head;
	Cell *tail;
} List;


// [1]
// ===================
// STRUCT CONSTRUCTORS
// ===================

Cell *createCell(int data)
{
	Cell *temp = malloc(sizeof(Cell));
	temp->data = data;
	temp->next = NULL;
	return temp;
}

List *createList(void)
{
	List *temp = malloc(sizeof(List));
	temp->size = 0;
	temp->head = NULL;
	temp->tail = NULL;
	return temp;
}


// ==============
// STRUCT METHODS
// ==============

int getSize(List *list)
{
	if (list == NULL)
		return 0;
	
	return list->size;
}

void insertAtTail(List *list, Cell *cell)
{
	Cell *temp;
	if (cell == NULL || list == NULL)
		return;

	if (getSize(list) == 0)
	{
		// if the list is empty, the new cell
		// is both the head and tail of the list
		list->head = cell;
		list->tail = cell;
	}
	else
	{
		// if there is already a head, we
		// just make the cell the new tail
		list->tail->next = cell;
		list->tail = cell;
	}

	list->size++;
}

// Used for debugging purposes, so that we can see what was inside the given list
void printList(List *list)
{
	int n, i;
	Cell *temp;

	if (list == NULL)
	{
		printf("List not initialized.\n");
		return;
	}

	if (getSize(list) == 0)
	{
		printf("EMPTY!\n");
		return;
	}

	n = getSize(list);
	temp = list->head;

	for (i = 0; i < n; i++)
		printf("%d%c", temp->data, (i == n-1) ? '\n' : ' ');
}

// Freeing methods to prevent memory leak
Cell *freeCells(Cell *cell)
{
	if (cell == NULL)
		return NULL;

	freeCells(cell->next);
	free(cell);

	return NULL;
}

List *freeList(List *list)
{
	if (list == NULL)
		return NULL;

	freeCells(list->head);
	free(list);

	return NULL;
}

// ==============
// HELPER METHODS
// ==============

int min(int a, int b)
{
	return (a < b) ? a : b;
}

// Runtime: O(n)
void createPreOrder(node *root, List *list)
{
	if (root == NULL || list == NULL)
		return;

	insertAtTail(list, createCell(root->data));

	createPreOrder(root->left, list);
	createPreOrder(root->right, list);
}

// Runtime: O(n)
void createPostOrder(node *root, List *list)
{
	if (root == NULL || list == NULL)
		return;

	createPostOrder(root->left, list);
	createPostOrder(root->right, list);
	
	insertAtTail(list, createCell(root->data));
}

// Runtime: O(n)
int isEqual(List *a, List *b)
{
	Cell *temp1, *temp2;

	if (getSize(a) != getSize(b))
		return 0;

	temp1 = a->head;
	temp2 = b->head;

	while (temp1 != NULL && temp2 != NULL)
	{
		if (temp1->data != temp2->data)
			return 0;

		temp1 = temp1->next;
		temp2 = temp2->next;
	}

	return 1;
}

int comparePrePost(node *a, node *b)
{
	List *preA = createList();
	List *postB = createList();

	// We don't need to null check since we already did that in kindredSpirits().
	// Runtime: O(n) + O(n) = O(2n) = O(n)
	createPreOrder(a, preA);
	createPostOrder(b, postB);

	// Runtime: O(n)
	if (isEqual(preA, postB))
	{
		freeList(preA);
		freeList(postB);
		return 1;
	}

	// Regardless of whether the trees are kindred spirits,
	// we want to free the lists to avoid memory leaks.
	freeList(preA);
	freeList(postB);

	return 0;
}

// ================
// REQUIRED METHODS
// ================

int isReflection(node *a, node *b)
{
	// 
	if (a == NULL && b == NULL)
		return 1;
	if (a == NULL || b == NULL)
		return 0;

	if (a->data != b->data)
		return 0;

	// This makes it so that if there's an inequality present,
	// it gets translated to the top level return value
	return min(
		isReflection(a->left, b->right),
		isReflection(a->right, b->left)
	);
}

node *makeReflection(node *root)
{
	if (root == NULL)
		return NULL;

	// Create a new Node
	node *temp = malloc(sizeof(node));
	temp->data = root->data;

	// Like a mirror image, the original's left is the reflection's right
	temp->left = makeReflection(root->right);
	temp->right = makeReflection(root->left);

	return temp;
}

// Total Runtime: O(1) + O(n) = O(n), where n = # of nodes in the tree
int kindredSpirits(node *a, node *b)
{
	// Runtime: O(1) = O(1) = O(2) = O(1)
	if (a == NULL && b == NULL)
		return 1;
	if (a == NULL || b == NULL)
		return 0;

	// Runtime: O(n) + O(n) = O(2n) = O(n)
	if (comparePrePost(a, b) == 1)
		return 1;
	if (comparePrePost(b, a) == 1)
		return 1;

	return 0;
}

// [2]
double difficultyRating(void)
{
	return 2.25;
}

// [3]
double hoursSpent(void)
{
	return 8.0;
}


// ================
// ADDITIONAL NOTES
// ================
//   This section was added so that I could go more into detail about the code
//   without making the programming itself harder to read due to the cluttering 

// [1]
// Method grouping:
//   Having programmed in Java for the past 3 years, I decided to group the methods
//   for creating the structs and methods directly involving the fields of the structs
//   seperately from the helper methods used to make skimming the code much easier 

// [2]
// Difficulty Breakdown:
// - Overall, this assignment was fun and challenging, but not too challenging.
// - isReflection() and makeReflection() could each be their own exam question
//   as they are quick and easy to write, but both require a bit of attention
//   to the overall picture during their respective recursions.
// - kindredSpirits() was the real meat of the assignment, and got me to think
//   deeper about the nature of binary trees, such as if you want to compare two
//   trees mid-traversal, it is much easier to do so when iteratively traversing 
//   as opposed to recursively traversing.

// [3]
// Time Breakdown:
// - isReflection                    : 15 minutes
// - makeReflection                  : 10 minutes
// - kindredSpirits                  : 3 hours
// - trying to find a more efficient : 2 hours
//   way to do kindredSpirits and
//   failing :P
// - commenting & cleaning up code   : 1.5 hours
// - Finding the better solution to  : 1.5 hours
//   kindredSpirits() :D