// Implements a dictionary's functionality

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h>
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
const unsigned int N = 7000;

// Hash table
node *table[N];

// size of table initialized to 0
unsigned int table_size = 0;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // get hash index of string
    int hash_index = hash(word);

    if (table[hash_index] == NULL)
    {
        return false;
    }

    // loop through linked list at hash_index and return true if found
    for (node *curr = table[hash_index]; curr != NULL; curr = curr->next)
    {
        if (strcasecmp(word, curr->word) == 0)
        {
            return true;
        }
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    char copy[LENGTH];
    strcpy(copy, word);

    // based on djb2 hash function by Dan Bernstein (source: http://www.cse.yorku.ca/~oz/hash.html)
    unsigned int hash = 5381;
    for (int i = 0; copy[i] != '\0'; i++)
    {
        hash = ((hash << 5) + hash) + tolower(copy[i]); /* hash * 33 + c */
    }

    return hash % N;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // open file pointer to dictionary
    FILE *dict_file = fopen(dictionary, "r");

    if (dict_file == NULL)
    {
        printf("Not enough memory to open dictionary file %s\n", dictionary);
        return false;
    }

    // initialize string s for reading from dictionary file
    char s[LENGTH];
    // read each string from file one at a time
    while (fscanf(dict_file, "%s", s) != EOF)
    {
        // allocate memory for new node for hash table
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            printf("Not enough memory for new node");
            return false;
        }

        // copy the read dictionary word
        strcpy(n->word, s);

        // get hash index of string
        int hash_index = hash(s);

        // if bucket is empty in table
        if (table[hash_index] == NULL)
        {
            // have table at hash of string point to the newly allocated memory
            table[hash_index] = n;

            // set next to NULL
            n->next = NULL;
        }
        else // add new node to head of linked list in bucket
        {
            // set the new node's next pointer to the original starting node of the linked list
            n->next = table[hash_index];

            // point the head of the linked list to the new node
            table[hash_index] = n;
        }

        // increment table_size
        table_size++;
    }

    fclose(dict_file);

    return true; // dictionary loaded into table successfully
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return table_size;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // loop through table array
    for (int i = 0; i < N; i++)
    {
        // if there is a linked list in the bucket, loop through each node and free one by one
        if (table[i] != NULL)
        {
            node *curr = table[i];
            node *tmp = curr;
            while (curr != NULL)
            {
                // move curr to next node
                tmp = curr->next;

                // free memory pointed to by tmp
                free(curr);

                // set tmp to point to same block of memory as curr
                curr = tmp;
            }
        }
    }
    return true;
}
