#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Avriel Lyon
//11/14/2021
//P5 Word sort program

//defining structs
struct wordStr
{
	char *word;
	int freq;
	int depth;
}

wordStr;

struct treeNode
{
	struct wordStr * item;
	struct treeNode * left;
	struct treeNode * right;
}

treeNode;

//function declarations 
void fillStructArray(struct treeNode *root, struct wordStr *array);
struct treeNode* createNode(struct treeNode *root, char *word);
struct treeNode* insert(struct treeNode *curr, struct treeNode *newNode, int depth);
int search(struct treeNode *root, char *word);
void mergeSort(struct wordStr *array, int low, int high);
void mergeFunction(struct wordStr *array, int low, int mid, int high);

//keeps track of the index
int arrIdx;

int main()
{
	//initializing variables
	struct treeNode *root = NULL;
	struct treeNode *temp = NULL;
	char *word = malloc(21* sizeof(char));
	int choice;
	int totalWords;
	arrIdx = 0;
	int i, j;

	scanf("%d", &totalWords);

	//loops for how many words there are
	for (i = 0; i < totalWords; i++)
	{
		scanf("%d", &choice);
		scanf("%s", word);

		//if it is insertion
		if (choice == 1)
		{
			temp = createNode(root, word);
			root = insert(root, temp, 0);
		}

		//if it is query
		else
		{
			//if search never returned 1
			if (search(root, word) == 0)
			{
				printf("-1 -1\n");
			}
		}
	}
	
	//mallocing space for the array of struct
	struct wordStr *array = malloc(totalWords* sizeof(struct wordStr));

	//copying the bst to the array
	fillStructArray(root, array);

	//sorting the array
	mergeSort(array, 0, arrIdx - 1);

	//prints out the array
	for (j = 0; j < arrIdx; j++)
	{
		printf("%s %d\n", array[j].word, array[j].freq);
	}

	//freeing the rest of memory
	free(root);
	free(array);

	return 0;
}

//copies the bst nodes to the array
void fillStructArray(struct treeNode *root, struct wordStr *array)
{
	if (root == NULL)
	{
		return;
	}

	array[arrIdx++] = *root->item;
	fillStructArray(root->left, array);
	fillStructArray(root->right, array);
}

//searches for the node and prints the freq and depth if found
int search(struct treeNode *node, char *word)
{
	if (node == NULL)
	{
		return 0;
	}

	//value for conditional
	int cmp = strcmp(node->item->word, word);

	//go left
	if (cmp > 0)
	{
		return search(node->left, word);
	}

	//go right
	else if (cmp < 0)
	{
		return search(node->right, word);
	}

	//print if found
	else
	{
		printf("%d %d\n", node->item->freq, node->item->depth);
		return 1;
	}
}

//creates a new node
struct treeNode* createNode(struct treeNode *root, char *word)
{
	//initalize and malloc the struct
	struct treeNode * temp;
	temp = (struct treeNode *) malloc(sizeof(struct treeNode));
	//malloc for the struct inside the struct
	temp->item = (struct wordStr *) malloc(sizeof(struct wordStr));
	//malloc space for word
	temp->item->word = malloc(strlen(word) + 1);

	//copy over the word
	strcpy(temp->item->word, word);

	//default values 
	temp->item->depth = 0;
	temp->item->freq = 1;
	temp->left = NULL;
	temp->right = NULL;

	return temp;
}

//inserts into the binary search tree
struct treeNode* insert(struct treeNode *curr, struct treeNode *newNode, int depth)
{
	if (curr == NULL)
	{
		//set the depth equal to where we stopped
		newNode->item->depth = depth;
		return newNode;
	}

	//had an issue here where depth maintained 0
	if (depth == 0)
	{
		depth++;
	}

	//conditional
	int cmp = strcmp(newNode->item->word, curr->item->word);

	//if equal, increase the frequency it appears and return the current node
	if (cmp == 0)
	{
		curr->item->freq++;
		return curr;
	}
	else if (cmp > 0)
	{
		//if there is something to the right, advance
		if (curr->right != NULL)
		{
			insert(curr->right, newNode, ++depth);
		}

		//set the curr->right node to newNode
		else
		{
			newNode->item->depth = depth;
			curr->right = newNode;
		}
	}

	//if cmp is less than 0
	else
	{
		//if there is nothing to the left, advance
		if (curr->left != NULL)
		{
			insert(curr->left, newNode, ++depth);
		}

		//set the curr->left node to newNode
		else
		{
			newNode->item->depth = depth;
			curr->left = newNode;
		}
	}

	return curr;
}

void mergeSort(struct wordStr *array, int low, int high)
{
	if (low < high)
	{
		//set the mid to be the average between low and high
		int mid = (low + high) / 2;

		//call merge sort for the left of the array
		mergeSort(array, low, mid);

		//call merge sort for the right of the array
		mergeSort(array, mid + 1, high);

		//merge the arrays
		mergeFunction(array, low, mid + 1, high);

	}
}

void mergeFunction(struct wordStr *array, int low, int mid, int high)
{
	//three values to increment
	int index1 = low;	//starts at the begining of the array
	int index2 = mid;	//starts at the middle of the array
	int mainIndex = 0;	//starts at index 0

	int i;

	//creating a temporary array to store the sorted values
	struct wordStr *tempArray = malloc(arrIdx* sizeof(struct wordStr));

	while ((index1 < mid) || (index2 <= high))
	{
		//if one frequency is greater than the other
		if (index2 > high || (index1 < mid && array[index1].freq > array[index2].freq))
		{
			//placing the values in their reverse sorted position
			tempArray[mainIndex] = array[index1];

			//increment from the start of the array to the mid point
			index1++;
			mainIndex++;
		}

		//if the frequencies are the same
		else if (index2 > high || (index1 < mid && array[index1].freq == array[index2].freq))
		{
			int cmp = strcmp(array[index1].word, array[index2].word);
			
			if (cmp > 0)
			{
				tempArray[mainIndex] = array[index2];
				index2++;
				mainIndex++;

			}
			else
			{
				tempArray[mainIndex] = array[index1];
				index1++;
				mainIndex++;
			}
		}
		else
		{
			//placing the values in their sorted position
			tempArray[mainIndex] = array[index2];

			//increment from the middle of the array to the end of the array
			index2++;
			mainIndex++;
		}
	}

	//put the values from the tempArray back into the array
	for (i = low; i <= high; i++)
	{
		array[i] = tempArray[i - low];
	}

	//free the temporary array to avoid memory errors
	free(tempArray);
}





