// Name    : Kyle Pazmino
// NID     : Ky514084
// PID     : 4080977
// Class   : COP 3502C
// Semester: Spring 2019

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "TriePrediction.h"

// Note: [#] indicates there is more info at the bottom of the file.

// ==================
// STRUCT CONSTRUCTOR
// ==================

TrieNode *createTrieNode(void)
{
	int i;
	TrieNode *ptr = malloc(sizeof(TrieNode));
	ptr->count = 0;
	ptr->subtrie = NULL;

	for (i = 0; i < 26; i++)
	{
		ptr->children[i] = NULL;
	}

	return ptr;
}


// ===============
// PRINT FUNCTIONS
// ===============

// Helper function called by printTrie(). (Credit: Dr. S.)
void printTrieHelper(TrieNode *root, char *buffer, int k)
{
	int i;

	if (root == NULL)
		return;

	if (root->count > 0)
		printf("%s (%d)\n", buffer, root->count);

	buffer[k + 1] = '\0';

	for (i = 0; i < 26; i++)
	{
		buffer[k] = 'a' + i;

		printTrieHelper(root->children[i], buffer, k + 1);
	}

	buffer[k] = '\0';
}

// If printing a subtrie, the second parameter should be 1; otherwise, if
// printing the main trie, the second parameter should be 0. (Credit: Dr. S.)
void printTrie(TrieNode *root, int useSubtrieFormatting)
{
	char buffer[1026];

	if (useSubtrieFormatting)
	{
		strcpy(buffer, "- ");
		printTrieHelper(root, buffer, 2);
	}
	else
	{
		strcpy(buffer, "");
		printTrieHelper(root, buffer, 0);
	}
}


// ================
// HELPER FUNCTIONS
// ================

// takes away anything that isn't a letter from word, and flags isEOS if the 
// word was the last in the sentence.
void stripPunctuators(char *word, int *isEOS)
{
	char temp[strlen(word)];
	int i, j, n, letter;

	strcpy(temp, "");
	j = 0;
	n = strlen(word);

	for (i = 0; i < n; i++)
	{
		letter = word[i];

		if ('a' <= letter && letter <= 'z')
			temp[j++] = word[i];
		else if ('A' <= letter && letter <= 'Z')
			temp[j++] = word[i] - 'A' + 'a';
		else if (letter == '.' || letter == '!' || letter == '?') // [1]
			*isEOS = 1;
	}
	temp[j] = '\0';

	strcpy(word, temp);
}

// Returns the last node of a newly inserted word
TrieNode *addWord(TrieNode *root, char *word)
{
	TrieNode *temp;
	int i, n, num;

	if (root == NULL)
		return NULL;

	temp = root;

	n = strlen(word);

	for (i = 0; i < n; i++)
	{
		if (temp->children[word[i]-'a'] == NULL)
			temp->children[word[i]-'a'] = createTrieNode();

		temp = temp->children[word[i]-'a'];
	}

	temp->count++;

	return temp;
}

// Has the same functionality as addWord with the bonus of creating a subtrie
// for us if none exist
TrieNode *addWordToSubtrie(TrieNode *root, char *word)
{
	if (root == NULL)
		return NULL;
	
	if (root->subtrie == NULL)
		root->subtrie = createTrieNode();

	addWord(root->subtrie, word);

	return root;
}

// Counts all the words that are prefixed with a given root
int countAll(TrieNode *root)
{
	int i, sum;

	if (root == NULL)
		return 0;

	sum = 0;

	for (i = 0; i < 26; i++)
		sum += countAll(root->children[i]);

	sum += root->count;

	return sum;
}

// As you can guess by the name of the method, it's exactly the same as getNode()
// but without the pesky count check :P
TrieNode *getNodeRegardlessOfCount(TrieNode *root, char *str)
{
	// We traverse the trie until we reach the node corresponding to our given
	// string

	// if our initial node does not exist or the node corresponding to one of
	// the letters in our string does not exist, we return NULL to signify the
	// string is not in our trie. And if we are given a NULL string, just return 
	// NULL.
	if (root == NULL || str == NULL)
		return NULL;

	// This means we successfully reached the end of the word and the node we 
	// are currently on is the one that represents our initial string.
	if (strlen(str) == 0)
		return root;

	// The way we traverse the string is that we go to the child node representing 
	// our next char and increase our string pointer by one so that str[0] points 
	// to the next char in memory
	return getNode(root->children[tolower(str[0])-'a'], str+1);
}

// - Recursively traverses through the trie in alphabetical order and changes 
//   str when it finds a new "global" max count 
// - Called by getMostFrequentWord()
// - Note: Heavy Inspiration from printTrieHelper() (Credit: Dr. S.)
void gMFWHelper(TrieNode *root, char *buffer, int ind, int *max, char *str)
{
	int i;

	if (root == NULL)
		return;

	if (root->count > *max)
	{
		*max = root->count;
		strcpy(str, buffer);
	}

	buffer[ind + 1] = '\0';

	for (i = 0; i < 26; i++)
	{
		buffer[ind] = 'a' + i;

		gMFWHelper(root->children[i], buffer, ind+1, max, str);
	}

	buffer[ind] = '\0';
}

// prints the most frequent word in the previous word's subtrie (n - 1) times 
void gMFWChain(TrieNode *root, char *str, int n)
{
	TrieNode *ptr;
	char buffer[MAX_CHARACTERS_PER_WORD+1];

	// print the string regardless of if it is in the trie or not
	printf("%s", str);
	
	// Does the Top-level node exist?
	if (root == NULL)
	{
		printf("\n");		
		return;
	}

	strcpy(buffer, str);

	while (n > 0)
	{
		// Is the string not in our tree?
		if ((ptr = getNode(root, buffer)) == NULL)
			break;

		// Does the string not have any next words?
		if (ptr->subtrie == NULL)
			break;

		// get our next word and copy it to buffer
		getMostFrequentWord(ptr->subtrie, buffer);

		// print that string (with a space before it)
		printf(" %s", buffer);

		n--;
	}

	printf("\n");
}

// ==================
// REQUIRED FUNCTIONS
// ==================

TrieNode *buildTrie(char *filename)
{
	FILE *input;
	TrieNode *root, *lastWord;
	char buffer[MAX_CHARACTERS_PER_WORD+1];
	// A flag for the end of a sentence
	int *isEOS;

	input = fopen(filename, "r");

	// Since it is safe to assume that any .txt file given during testing will 
	// be valid, this is purely for debugging purposes.
	if (input == NULL)
	{
		printf("Something went wrong with the input file within buildTrie()\n");
		return NULL;
	}

	root = createTrieNode();
	lastWord = NULL;
	*isEOS = 0;

	while(fscanf(input, "%s", buffer) != EOF) 
	{
		stripPunctuators(buffer, isEOS);

		// We want to skip over doing this step if we are currently on the first 
		// word of a sentence.
		if (lastWord != NULL)
			addWordToSubtrie(lastWord, buffer);

		// addWord() returns the last node of a newly inserted string, so we want
		// to save that location so we can add the next word to it's subtrie
		lastWord = addWord(root, buffer);

		// If the word we are on is the last in the sentence, we do not want to 
		// add the next word into this one's subtrie.
		if (*isEOS)
		{
			lastWord = NULL;
			*isEOS = 0;
		}
	}

	fclose(input);
	return root;
}

int processInputFile(TrieNode *root, char *filename)
{
	FILE *input;
	char buffer[MAX_CHARACTERS_PER_WORD+1];
	int *n;
	TrieNode *temp;

	input = fopen(filename, "r");

	if (input == NULL)
		return 1;

	while(fscanf(input, "%s", buffer) != EOF)
	{
		if (strcmp(buffer, "@") == 0)
		{
			fscanf(input, "%s", buffer);
			fscanf(input, "%d", n);
			gMFWChain(root, buffer, *n);
		}
		else if (strcmp(buffer, "!") == 0)
		{
			printTrie(root, 0);
		}
		else
		{
			printf("%s\n", buffer);

			temp = getNode(root, buffer);

			if (temp == NULL)
			{
				printf("(INVALID STRING)\n");
			}
			else if (temp->subtrie == NULL)
			{
				// If the subtrie pointer is NULL, that means we never added any 
				// words to it, thus it is empty
				printf("(EMPTY)\n");
			}
			else 
			{
				printTrie(temp->subtrie, 1);
			}
		}
	}

	fclose(input);
	return 0;
}

TrieNode *destroyTrie(TrieNode *root)
{
	int i;

	// If the node we are given does not exist, there is nothing to free.
	if (root == NULL)
		return NULL;

	// Free the children!
	for (i = 0; i < 26; i++)
		destroyTrie(root->children[i]);

	// Free the subtrie
	destroyTrie(root->subtrie);

	// All that is left is to free the node we are on.
	free(root);

	return NULL;
}

TrieNode *getNode(TrieNode *root, char *str)
{
	// We traverse the trie until we reach the node corresponding to our given
	// string

	// if our initial node does not exist or the node corresponding to one of
	// the letters in our string does not exist, we return NULL to signify the
	// string is not in our trie. And if we are given a NULL string, just return 
	// NULL.
	if (root == NULL || str == NULL)
		return NULL;

	// This means we successfully reached the end of the word and the node we 
	// are currently on is the one that represents our initial string.
	if (strlen(str) == 0)
	{
		// However, the string is only considered to be in the tree if the count 
		// value of the node itself is greater then 0.
		if (root->count > 0)
			return root;
	}
	// The way we traverse the string is that we go to the child node representing 
	// our next char and increase our string pointer by one so that str[0] points 
	// to the next char in memory
	return getNode(root->children[tolower(str[0])-'a'], str+1);
}

void getMostFrequentWord(TrieNode *root, char *str)
{
	int x, *max;
	char buffer[MAX_CHARACTERS_PER_WORD+1];

	strcpy(str, "");
	strcpy(buffer, "");

	if (root == NULL)
		return;	

	// the point of doing this is so that max can be looked at regardless of 
	// gMFWHelper()'s location in the tree, acting as a pseudo-global variable
	x = 0;
	max = &x;

	gMFWHelper(root, buffer, 0, max, str);
}

int containsWord(TrieNode *root, char *str)
{
	// If the corresponding node to str exists and has count > 0, it won't be NULL. [1]
	if (getNode(root, str) == NULL)
		return 0;
	
	return 1;
}

int prefixCount(TrieNode *root, char *str)
{
	// The purpose of this method is to return the total number of words that 
	// are prefixed by str, or rather to return the sum of node->count of all 
	// the children nodes
	
	// Note: since str is not guaranteed to be in the trie already, we cannot 
	// jump to the location of str's respective node using getNode().
	int temp = countAll(getNodeRegardlessOfCount(root, str));
	printf("%d\n", temp);
	return temp;
}

double difficultyRating(void)
{
	// [2]
	return 2.95;
}

double hoursSpent(void)
{
	// [3]
	return 7.5;
}

int main(int argc, char **argv)
{
	TrieNode *trie = buildTrie(argv[1]);
	processInputFile(trie, argv[2]);
	// printTrie(trie, 0);
	destroyTrie(trie);
	return 0;
}


// ======================
// ADDITIONAL INFORMATION
// ======================
// This section was added so that I could go more into detail about the code
// without making the programming itself harder to read due to the cluttering 

// [1]
// Assumptions:
// - addWord()
// - addWordToSubtrie()
// - stripPunctuator()
//   - each line ends in a ( . || ! || ?), including the last line.
// - buildTrie()
//   - all corpora given during grading test cases will be valid
//   - each word contains at least one letter and at most MAX_CHARACTERS_PER_WORD.
//   - each line contains at least one word and at most thirty words.
// - getMostFrequentWord()
//   - str is created beforehand is at least the length MAX_CHARACTERS_PER_WORD
//   - str is not guaranteed to be clean (meaning it might contain garbage data)
// - containsWord()
//   - getNode() works properly :P.
