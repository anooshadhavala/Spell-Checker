#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Explicit declaration of strcasecmp
int strcasecmp(const char *s1, const char *s2);

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// Number of buckets in hash table
const unsigned int N = 26;

// Hash table
node *table[N];

// Counter for the number of words in the dictionary
unsigned int dictionary_size = 0;

// Hash function
unsigned int hash(const char *word)
{
    // Simple hash function: hash based on the first letter of the word
    return tolower(word[0]) - 'a';
}

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // Get the hash index for the word
    unsigned int index = hash(word);

    // Traverse the linked list at the hash index
    node *cursor = table[index];
    while (cursor != NULL)
    {
        // Compare word case-insensitively
        if (strcasecmp(cursor->word, word) == 0)
        {
            return true; // Word found in dictionary
        }
        cursor = cursor->next;
    }
    return false; // Word not found
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Open dictionary file
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        return false; // Failed to open file
    }

    // Initialize hash table
    for (int i = 0; i < N; i++)
    {
        table[i] = NULL;
    }

    char word[LENGTH + 1];
    while (fscanf(file, "%s", word) != EOF)
    {
        // Create a new node for the word
        node *new_node = malloc(sizeof(node));
        if (new_node == NULL)
        {
            fclose(file);
            return false; // Memory allocation failed
        }
        strcpy(new_node->word, word);
        new_node->next = NULL;

        // Get the hash index for the word
        unsigned int index = hash(word);

        // Insert the new node at the beginning of the linked list
        new_node->next = table[index];
        table[index] = new_node;

        dictionary_size++;
    }

    // Close dictionary file
    fclose(file);

    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return dictionary_size;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *cursor = table[i];
        while (cursor != NULL)
        {
            node *temp = cursor;
            cursor = cursor->next;
            free(temp);
        }
    }
    return true;
}
