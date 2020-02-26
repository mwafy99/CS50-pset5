// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "dictionary.h"


// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 60000;

// Hash table
node *HASH_TABLE[N];

// Word Counter
int WORD_COUNTER = 0;

// https://stackoverflow.com/questions/628790/have-a-good-hash-function-for-a-c-hash-table
// hash function used to hash words
size_t precision = 2;

size_t generate_hash(const char *str)
{
    return ((*(size_t *) str) >> precision) % N;
}



// Returns true if word is in dictionary else false
bool check(const char *word)
{
    int word_len = strlen(word);
    // create a temp holder with the size of word + nul terminator
    char *lower = malloc(sizeof(char) * (word_len + 1));

    for (int i = 0; i < word_len; i++)
    {
        lower[i] = tolower(word[i]);
    }

    // generate a hash index for lower word
    int word_index = generate_hash(lower);

    node *tmp = HASH_TABLE[word_index];

    while (tmp != NULL)
    {
        if (strcmp(tmp->word, lower) == 0)
        {
            return true;
        }
    }
    return false;
}


// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // #1 create a file pointer
    FILE *dict = fopen(dictionary, "r");

    // #2 create a word_temp holder
    char word_temp[LENGTH + 1] = {'\0'};

    // #3 loop to load dictionary and insert words in index
    while (fscanf(dict, "%s", word_temp) == 1)
    {
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            return false;
        }
        n->next = NULL;
        strcpy(n->word, word_temp);

        // #4 generate a hash using hash function brought from stackoverflow
        int word_index = generate_hash(n->word);

        // #5 insert word into hash
        if (HASH_TABLE[word_index] != NULL)
        {
            for (node *tmp = HASH_TABLE[word_index]; tmp != NULL; tmp = tmp->next)
            {
                if (tmp->next == NULL)
                {
                    tmp->next = n;
                    break;
                }
            }
        }

        else
        {
            HASH_TABLE[word_index] = n;
        }
        WORD_COUNTER++;
    }

    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    // return word count using WORD_COUNTER
    return WORD_COUNTER;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{

    for (int i = 0; i < N; i++)
    {
        node *head = HASH_TABLE[i];
        while (head != NULL)
        {
            node *tmp = head;
            head = head->next;
            free(tmp);
        }
    }
    return true;
}
