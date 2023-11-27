// Name: Kyle Pazmino
// Course: COP 3502C
// Semester: Spring 2019
// NID: ky514084

#include "LonelyPartyArray.h"
#include <stdio.h>
#include <stdlib.h>

void printLPA(LonelyPartyArray *);

// Input(s):
//	num_fragments: number of fragments (arrays) in this LPA
//	fragment_length: number of cells per fragment in this LPA
// Possible Output(s):
//	NULL
//	pointer to a newly created LPA
// Description:
//	creates a new LPA given the number of fragments and the length of each fragment
LonelyPartyArray *createLonelyPartyArray(int num_fragments, int fragment_length)
{
	LPA *ptr;
	int i, j;

	// If either of the inputs are non-positive integers, it will cause the
	// program to crash so we need to check and return null if either of them are
	if (num_fragments <= 0 || fragment_length <= 0)
		return NULL;

	// The temp veraible that contains the address of the LPA we want to return
	ptr = malloc(sizeof(LPA));

	if (ptr == NULL)
		return NULL;

	// Instantiation of all the fields of LPA
	ptr->size = 0;
	ptr->num_fragments = num_fragments;
	ptr->fragment_length = fragment_length;
	ptr->num_active_fragments = 0;

	// Allocate memory for fragments and check if the allocation 
	// was successful. If not, free everything and return NULL.
	ptr->fragments = malloc(sizeof(int *)*num_fragments);
	if (ptr->fragments == NULL)
	{
		free(ptr);
		return NULL;
	}

	ptr->fragment_sizes = malloc(sizeof(int)*num_fragments);
	if (ptr->fragment_sizes == NULL)
	{
		free(ptr->fragments);
		free(ptr);
		return NULL;
	}

	for (i = 0; i < num_fragments; i++)
	{
		ptr->fragments[i] = NULL;
	}
	for (i = 0; i < num_fragments; i++)
	{
		ptr->fragment_sizes[i] = 0;
	}

	printf("-> A new LonelyPartyArray has emerged from the void. ");
	printf("(capacity: %d, fragments: %d)\n", num_fragments*fragment_length, num_fragments);

	return ptr;
}

// Input(s):
//	party: pointer to an LPA
// Possible Output(s):
//	NULL
// Description:
//	Frees LPA and all of it's dynamically allocated memory and prints
//	"-> The LonelyPartyArray has returned to the void." 
LonelyPartyArray *destroyLonelyPartyArray(LonelyPartyArray *party)
{
	// If the pointer is already null, we have nothing
	// to free, therefore we just return null
	if (party == NULL)
		return NULL;

	int i;

	for (i = 0; i < party->num_fragments; i++)
	{
		if (party->fragments[i] != NULL)
			free(party->fragments[i]);
	}
	free(party->fragment_sizes);
	free(party);

	printf("-> The LonelyPartyArray has returned to the void.\n");
	return NULL;
}

// Input(s):
//	party: pointer to an LPA
//	index: location within LPA
//	key: a value
// Possible Output(s):
//	LPA_SUCCESS
//	LPA_FAILURE
// Description:
//	Sets the location in LPA, corresponding to the index, as the value being passed 
int set(LonelyPartyArray *party, int index, int key)
{
	if (party == NULL)
	{
		printf("-> Bloop! NULL pointer detected in set().\n");
		return LPA_FAILURE;
	}

	int i;
	int r = index/party->fragment_length;
	int c = index%party->fragment_length;

	if (index >= party->fragment_length*party->num_fragments || index < 0)
	{
		printf("-> Bloop! Invalid access in set().");
		printf(" (index: %d, fragment: %d, offset: %d)\n", index, r, c);
		return LPA_FAILURE;
	}

	if (party->fragments[r] == NULL)
	{
		party->fragments[r] = malloc(sizeof(int) * party->fragment_length);
		if (party->fragments[r] == NULL)
			return LPA_FAILURE;

		party->num_active_fragments++;
		printf("-> Spawned fragment %d. (capacity: %d, indices: %d..%d)\n", r, party->fragment_length, r*party->fragment_length, (r+1)*party->fragment_length-1);

		for (i = 0; i < party->fragment_length; i++)
			party->fragments[r][i] = UNUSED;
	}

	if (party->fragments[r][c] == UNUSED)
	{
		party->fragment_sizes[r]++;
		party->size++;
	}

	party->fragments[r][c] = key;

	return LPA_SUCCESS;
}

// Input(s):
//	party: pointer to an LPA
//	index: location within LPA
// Possible Output(s):
//	LPA_FAILURE
//	Value corresponding to index
// Description:
//	retrieves the value of party at index if there is one
int get(LonelyPartyArray *party, int index)
{
	if (party == NULL)
	{
		printf("-> Bloop! NULL pointer detected in get().\n");
		return LPA_FAILURE;
	}

	int r = index/party->fragment_length;
	int c = index%party->fragment_length;

	if (index >= party->fragment_length*party->num_fragments || index < 0)
	{
		printf("-> Bloop! Invalid access in get().");
		printf(" (index: %d, fragment: %d, offset: %d) \n", index, r, c);
		return LPA_FAILURE;
	}

	if (party->fragments[r] == NULL)
		return UNUSED;

	return party->fragments[r][c];
}

// Input(s):
//	party: pointer to an LPA
//	index: location within LPA
// Possible Output(s):
//	LPA_FAILURE
//	LPA_SUCCESS
// Description:
//	Sets the location at index to UNUSED
int delete(LonelyPartyArray *party, int index)
{
	int r, c;

	if (party == NULL)
	{
		printf("-> Bloop! NULL pointer detected in delete().\n");
		return LPA_FAILURE;
	}

	r = index/party->fragment_length;
	c = index%party->fragment_length;

	if (index >= party->fragment_length*party->num_fragments || index < 0)
	{
		printf("-> Bloop! Invalid access in delete().");
		printf(" (index: %d, fragment: %d, offset: %d) \n", index, r, c);
		return LPA_FAILURE;
	}

	if (party->fragments[r] == NULL)
		return LPA_FAILURE;

	if (party->fragments[r][c] == UNUSED)
		return LPA_FAILURE;

	party->fragments[r][c] = UNUSED;
	party->fragment_sizes[r]--;
	party->size--;

	if (party->fragment_sizes[r] == 0)
	{
		free(party->fragments[r]);
		party->fragments[r] = NULL;
		party->num_active_fragments--;
		printf("-> Deallocated fragment %d. (capacity: %d, indices: %d..%d)\n", r, party->fragment_length, r*party->fragment_length, (r+1)*party->fragment_length-1);
	}

	return LPA_SUCCESS;
}

// Input(s):
//	party: pointer to an LPA
//	key: value
// Possible Output(s):
//	0: key is not in LPA
//  1: Key is in LPA
// Description:
//	Loops through LPA to see if it contains key
int containsKey(LonelyPartyArray *party, int key)
{
	int i, j;

	if (party == NULL)
		return 0;

	for (i = 0; i < party->num_fragments; i++)
	{
		if (party->fragments[i] == NULL)
			continue;

		for (j = 0; j < party->fragment_length; j++)
		{
			if (party->fragments[i][j] == key)
				return 1;
		}
	}


	return 0;
}

// Input(s):
//	party: pointer to an LPA
//	index: location within LPA
// Possible Output(s):
//	1: party at index has a non-UNUSED value
//	0: it doesn't for various reasons
// Description:
//	checks party at index 
int isSet(LonelyPartyArray *party, int index)
{
	int r, c;

	if (party == NULL)
		return 0;

	r = index/party->fragment_length;
	c = index%party->fragment_length;

	if (index >= party->fragment_length*party->num_fragments || index < 0)
		return 0;

	if (party->fragments[r] == NULL)
		return 0;

	if (party->fragments[r][c] == UNUSED)
		return 0;
	else
		return 1;
}

// Input(s):
//	party: pointer to an LPA
//	index: location within LPA
// Possible Output(s):
//	LPA_FAILURE
//	LPA_SUCCESS
// Description:
//	if party is not NULL and index is valid, then
//  this function prints the value of LPA at index
int printIfValid(LonelyPartyArray *party, int index)
{
	int r, c;

	if (party == NULL)
		return LPA_FAILURE;

	if (index >= party->fragment_length*party->num_fragments || index < 0)
		return LPA_FAILURE;

	r = index/party->fragment_length;
	c = index%party->fragment_length;

	if (party->fragments[r] == NULL)
		return LPA_FAILURE;

	if (party->fragments[r][c] == UNUSED)
		return LPA_FAILURE;

	printf("%d\n", party->fragments[r][c]);

	return LPA_SUCCESS;
}

// Input(s):
//	party: pointer to an LPA
// Possible Output(s):
//	a pointer to an LPA
// Description:
//	resets an LPA to right after it was instantiated 
LonelyPartyArray *resetLonelyPartyArray(LonelyPartyArray *party)
{
	int i, j;

	if (party == NULL)
	{
		printf("-> Bloop! NULL pointer detected in get().\n");
		return party;
	}

	for (i = 0; i < party->num_fragments; i++)
	{
		if (party->fragments[i] != NULL)
		{
			free(party->fragments[i]);
			party->fragments[i] = NULL;
			party->num_active_fragments--;		
		}
	}

	for (i = 0; i < party->num_fragments; i++)
		party->fragment_sizes[i] = 0;

	party->size = 0;

	printf("-> The LonelyPartyArray has returned to its nascentstate. ");
	printf("(capacity: %d, fragments: %d)\n", getCapacity(party), party->num_fragments);

	return party;
}

// Input(s):
//	party: pointer to an LPA
// Possible Output(s):
//	-1
//	A value
// Description:
//	If party is not a null pointer, then this function returns
//  how many active cells contain values that aren't UNUSED
int getSize(LonelyPartyArray *party) 
{
	if (party == NULL)
		return -1;
	return party->size;
}

// Input(s):
//	party: pointer to an LPA
// Possible Output(s):
//	-1
//	A value
// Description:
//	If party is not a null pointer, then this function returns
//  how many total integers can be held by this LPA
int getCapacity(LonelyPartyArray *party)
{
	if (party == NULL)
		return -1;
	return party->num_fragments*party->fragment_length;	
}

// Input(s):
//	party: pointer to an LPA
// Possible Output(s):
//	-1
//	A value
// Description:
//	If party is not a null pointer, then this function returns
//  the number of cells being used by the LPA
int getAllocatedCellCount(LonelyPartyArray *party)
{
	if (party == NULL)
		return -1;
	return party->num_active_fragments*party->fragment_length;
}

// Input(s):
//	party: pointer to an LPA
// Possible Output(s):
//	-1
//	A value
// Description:
//	If party is not a null pointer, then this function returns
//  how much memory would be taken up if an array were to be used
long long unsigned int getArraySizeInBytes(LonelyPartyArray *party)
{
	if (party == NULL)
		return -1;
	return (sizeof(int)*((long long unsigned int)getCapacity(party)));
}

// Input(s):
//	party: pointer to an LPA
// Possible Output(s):
//	0
//	A value
// Description:
//	If party is not a null pointer, then this function returns
//	how much memory is being used up currently by the LPA
long long unsigned int getCurrentSizeInBytes(LonelyPartyArray *party)
{
	long long unsigned int total = 0;
	int i;

	if (party == NULL)
	return 0;

	// The size of the pointer to the LPA
	total += sizeof(LPA *);
	// The size of the LPA itself
	total += sizeof(LPA);
	// The size of the first layer of the fragments array
	total += sizeof(int *) * party->num_fragments;
	// The size of the fragment_sizes array
	total += sizeof(int) * party->num_fragments;
	// The size of the fragmented arrays currently active
	total += sizeof(int) * getAllocatedCellCount(party);

	return total;
}

// Input(s):
//	none
// Possible Output(s):
//	a rating
// Description:
//	gives a rating of how hard the programmer thought writing this program was
double difficultyRating(void)
{
	return 3.0;
}

// Input(s):
//	none
// Possible Output(s):
//	a time
// Description:
// gives an estimation of long it took to complete this assignment
double hoursSpent(void)
{
	return 9.75;
}

void printLPA(LPA *party) 
{
	// prints the basic fields of LPA
	printf("num_fragments:\n %d\n", party->num_fragments);
	printf("fragment_length:\n %d\n", party->fragment_length);
	printf("size:\n %d\n", party->size);
	printf("num_active_fragments:\n %d\n", party->num_active_fragments);
	// prints the fragment_sizes array
	printf("fragment_sizes:\n");
	for (int i = 0; i < party->num_fragments; i++)
	{
		printf(" %d\n", party->fragment_sizes[i]);
	}
	// prints the fragments array
	printf("fragments:\n");
	for (int i = 0; i < party->num_fragments; i++)
	{
		if (party->fragments[i] == NULL)
			printf("NULL\n");
		else
			printf("%p\n", &party->fragments[i]);
	}
	printf("\n");
}