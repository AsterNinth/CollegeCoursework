// Name: Kyle Pazmino
// NID: Ky514084
// Class: COP 3502
// Semester: Spring 2019

#include "ListyString.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

ListyNode *createListyNode(char c)
{
	ListyNode *ln = malloc(sizeof(ListyNode));
	ln->data = c;
	ln->next = NULL;
	return ln;
}

ListyNode *createListyNodeChain(char *str)
{
	int i, n;
	ListyNode *head, *tail;

	if (str == NULL || str == "")
		return NULL;

	n = strlen(str);
	head = createListyNode(str[0]);
	tail = head;

	for (int i = 1; i < n; i++)
	{
		tail->next = createListyNode(str[i]);
		tail = tail->next;
	}

	return head;
}

void deleteChar(ListyString *listy, char key)
{
	int i, n;
	ListyNode *temp1, *temp2;

	if (listy == NULL || listy->head == NULL)
		return;

	while (listy->head != NULL && listy->head->data == key)
	{
		temp1 = listy->head->next;
		free(listy->head);
		listy->head = temp1;
		listy->length--;
	}
	temp1 = listy->head;
	while (temp1 != NULL && temp1->next != NULL)
	{
		if (temp1->next->data == key)
		{
			temp2 = temp1->next;
			if (temp1->next != NULL)
				temp1->next = temp1->next->next;
			else
				temp1->next = NULL;

			free(temp2);
			listy->length--;
		}
		else
		{
			temp1 = temp1->next;
		}
		// printListyString(listy);
		// printf("%d\n", listyLength(listy));
	}
}

char *toString(ListyString *listy)
{
	int i;
	char *str;
	ListyNode *temp;

	if (listy == NULL || listy->head == NULL)
		return NULL;

	str = malloc(sizeof(char)*(listy->length+1));
	temp = listy->head;

	for (int i = 0; i < listy->length; i++)
	{
		str[i] = temp->data;
		temp = temp->next;
	}

	str[listyLength(listy)] = '\n';

	return str;
}

void destroyChain(ListyNode *head)
{
	if (head == NULL)
		return;
	destroyChain(head->next);
	free(head);
}

int min(int a, int b)
{
	return (a < b) ? a : b;
}

int processInputFile(char *filename)
{
	FILE *ifp;
	char *buffer = NULL, *com = NULL, *str = NULL;
	char key;
	ListyString *listy;

	// lines 70 - 84 of this code were taken from the lecture notes on Jan. 18, 2019
	if ((ifp = fopen(filename, "r")) == NULL)
	{
		return 1;
	}

	// It is given that the longest string will be 1023 characters long,
	// so we only need to account for that plus \0
	buffer = malloc(sizeof(char) * 1024);
	fscanf(ifp, "%s", buffer);
	str = malloc(sizeof(char)*(strlen(buffer)+1));
	strcpy(str, buffer);
	listy = createListyString(str);

	while (fscanf(ifp, "%s", buffer) != EOF)
	{
		com = malloc(sizeof(char)*(strlen(buffer)+1));
		strcpy(com, buffer);

		if (strcmp(com, "@") == 0)
		{
			fscanf(ifp, "%s", buffer);
			key = buffer[0];

			fscanf(ifp, "%s", buffer);
			free(str);
			str = malloc(sizeof(char)*(strlen(buffer)+1));
			strcpy(str, buffer);

			replaceChar(listy, key, str);
		}
		else if (strcmp(com, "+") == 0)
		{
			fscanf(ifp, "%s", buffer);
			free(str);
			str = malloc(sizeof(char)*(strlen(buffer)+1));
			strcpy(str, buffer);

			listyCat(listy, str);
		}
		else if (strcmp(com, "-") == 0)
		{
			fscanf(ifp, "%s", buffer);
			key = buffer[0];

			deleteChar(listy, key);
		}
		else if (strcmp(com, "~") == 0)
		{
			reverseListyString(listy);
		}
		else if (strcmp(com, "?") == 0)
		{
			printf("%d\n", listyLength(listy));
		}
		else if (strcmp(com, "!") == 0)
		{
			printListyString(listy);
		}
		else
		{
			printf("Oops... This line shouldn't appear. A hiccup happened somewhere.\n");
		}
	}

	destroyListyString(listy);
	fclose(ifp);
	free(ifp);
	free(buffer);
	free(com);
	free(str);
	return 0;
}

ListyString *createListyString(char *str)
{
	int i, k;
	ListyString *ls;
	ListyNode *temp;

	ls = malloc(sizeof(ListyString));

	if (ls == NULL)
		return NULL;

	if (str == NULL || strlen(str) == 0)
	{
		ls->head = NULL;
		ls->length = 0;
	}
	else
	{
		ls->head = createListyNodeChain(str);
		ls->length = strlen(str);
	}

	return ls;
}


ListyString *destroyListyString(ListyString *listy)
{
	ListyNode *temp;

	if (listy == NULL)
		return NULL;

	destroyChain(listy->head);

	return NULL;
}

ListyString *cloneListyString(ListyString *listy)
{
	ListyString *ls;

	if (listy == NULL)
		return NULL;
	
	ls = createListyString(toString(listy));

	return ls;
}

void replaceChar(ListyString *listy, char key, char *str)
{
	ListyNode *temp, *temp2;
	int i, n = strlen(str);

	if (listy == NULL || listy->head == NULL)
		return;

	if (str == "")
		deleteChar(listy, key);

	temp = listy->head;

	while(temp != NULL)
	{
		if (temp->data == key)
		{
			temp->data = str[0];
			temp2 = temp->next;
			for (i = 1; i < n; i++)
			{
				temp->next = createListyNode(str[i]);
				temp = temp->next;
				listy->length++;
			}
			temp->next = temp2;
		}
		temp = temp->next;
	}
}

void reverseListyString(ListyString *listy)
{
	int i, n;
	char *str;
	ListyNode *temp;

	if (listy == NULL || listy->head == NULL)
		return;

	n = listy->length;
	//str = malloc(sizeof(char)*(n+1));

	str = toString(listy);

	temp = listy->head;

	for (i = n-1; i >= 0; i--)
	{
		temp->data = str[i];
		temp = temp->next;
	}

	free(str);
}

ListyString *listyCat(ListyString *listy, char *str)
{
	ListyNode *cat, *tail;
	int i, n;


	if (listy == NULL && str != NULL)
	{
		return createListyString(str);
	}
	else if (listy == NULL && str == NULL)
	{
		return NULL;
	}
	else if (listy != NULL && str == NULL)
	{
		return listy;
	}

	n = strlen(str);

	if (listy->head == NULL)
	{
		listy->head = createListyNode(str[0]);
		tail = listy->head;
		listy->length++;
		for (i = 1; i < n; i++)
		{
			tail->next = createListyNode(str[i]);
			tail = tail->next;
			listy->length++;
		}
	}
	else
	{
		tail = listy->head;
		while (tail->next != NULL)
			tail = tail->next;
		for (i = 0; i < n; i++)
		{
			tail->next = createListyNode(str[i]);
			tail = tail->next;
			listy->length++;
		}
	}

	return listy;
}

int listyCmp(ListyString *listy1, ListyString *listy2)
{
	int i, n;
	ListyNode *ptr1, *ptr2;

	if (listy1 == NULL && listy2 == NULL)
		return 0;
	if (listy1 != NULL && listy2 == NULL)
		return -1;
	if (listy1 == NULL && listy2 != NULL)
		return 1;

	n = min(listyLength(listy1), listyLength(listy2));

	ptr1 = listy1->head;
	ptr2 = listy2->head;

	for (i = 0; i < n; i++)
	{
		if (ptr1->data < ptr2->data)
			return -1;
		if (ptr1->data > ptr2->data)
			return 1;

		ptr1 = ptr1->next;
		ptr2 = ptr2->next;
	}

	return 0;
}

int listyLength(ListyString *listy)
{
	if (listy == NULL)
		return -1;
	if (listy->head == NULL)
		return 0;
	return listy->length;
}

void printListyString(ListyString *listy)
{
	ListyNode *temp;

	if (listy == NULL || listy->head == NULL)
	{
		printf("(empty string)\n");
		return;
	}

	temp = listy->head;

	// printf("**");
	while(temp != NULL)
	{
		printf("%c", temp->data);
		temp = temp->next;
	}
	printf("\n");
}

double difficultyRating(void)
{
	return 2;
}

double hoursSpent(void)
{
	return 10.5;
}

int main(int argc, char **argv)
{
	processInputFile(argv[1]);
	return 0;
}