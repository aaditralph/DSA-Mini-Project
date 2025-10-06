#ifndef TRIE_H
#define TRIE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "cJSON.h"

#define FILENAME "contacts.json"
#define MAX_LEN 100

typedef struct TrieNode TrieNode;

TrieNode *createNode();
void insert(TrieNode *root, const char *word, const char *number);
void autocomplete(TrieNode *root, const char *prefix);
void freeTrie(TrieNode *root);

void saveContactsToFile(TrieNode *root);
void loadContactsFromFile(TrieNode *root);

#endif