// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include "dictionary.h"
#include <ctype.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// ADDITIONAL functions
bool free_list(node *w);
unsigned int numeric(char c);

// Number of buckets in hash table
const unsigned int N = 26 * 28 * 28;
// Initialize number of word count in dictionary
int word_count = 0;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO: initiate a node before looping
    node *n = table[hash(word)];
    // Checks if malloc succeeded, returns false if not
    while (true)
    {
        if (n == NULL)
        {
            return false;
        }
        else if (strcasecmp(n->word, word) == 0)
        {
            return true;
        }
        else
        {
            n = n->next;
        }
    }
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO
    int ret;
    ret = tolower(word[0]) - 97 + 26 * numeric(word[1]);
    if (word[1] != '\0')
    {
        ret += 26 * 28 * numeric(word[2]);
    }
    return ret;
}

// ADDITION: Numeric char value
unsigned int numeric(char c)
{
    int ret;
    if (isalpha(c))
    {
        ret = (tolower(c) - 96);
    }
    else if (c == '\0')
    {
        ret = 0;
    }
    else
    {
        ret = 27;
    }
    return ret;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO: Open file
    FILE *dict = fopen(dictionary, "r");
    if (dict == NULL)
    {
        printf("Could not open dictionary.\n");
        return false;
    }

    //Initialize the list to be NULL
    for (int i = 0; i < N; i += 1)
    {
        table[i] = NULL;
    }

    // COPY words in dictionary to hash table
    char c;
    char tmp[LENGTH + 1];
    int index = 0;
    while (fread(&c, sizeof(char), 1, dict))
    {
        if (c != '\n')
        {
            tmp[index] = c;
            index += 1;
        }
        else // means this is a full word
        {
            // Terminate current word
            tmp[index] = '\0';

            // Put the word to the hash
            // allocate memory to a new node
            node *n = malloc(sizeof(node));
            // Checks if malloc succeeded, returns false if not
            if (n == NULL)
            {
                unload();
                return false;
            }
            // assign values to new node
            n->next = table[hash(tmp)];
            strcpy(n->word, tmp);

            // make the hash point to the new node
            table[hash(tmp)] = n;

            // Clear the tmp array for next word
            for (int i = 0; i < index + 1; i += 1)
            {
                tmp[index] = 0;
            }
            word_count += 1; //Increase word count for size function
            index = 0;
        }
    }
    fclose(dict);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return word_count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    for (int i = 0; i < N; i += 1)
    {
        free_list(table[i]);
    }

    return true;
}

// Create free linked-list function
bool free_list(node *w)
{
    if (w == NULL)
    {
        return true;
    }
    else if (w->next == NULL)
    {
        free(w);

        return true;
    }
    else
    {
        free_list(w->next);
        return true;
    }

}
