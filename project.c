#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Trie Node definition
typedef struct TrieNode {
    struct TrieNode *children[26];
    int isEndOfWord;
} TrieNode;

// Create new Trie node
TrieNode* createNode() {
    TrieNode *node = (TrieNode*)malloc(sizeof(TrieNode));
    node->isEndOfWord = 0;
    for (int i = 0; i < 26; i++)
        node->children[i] = NULL;
    return node;
}

// Insert a word into Trie
void insert(TrieNode *root, char *word) {
    TrieNode *curr = root;
    for (int i = 0; word[i] != '\0'; i++) {
        char ch = tolower(word[i]);
        int idx = ch - 'a';
        if (idx < 0 || idx >= 26) continue; // skip invalid chars

        if (curr->children[idx] == NULL)
            curr->children[idx] = createNode();
        curr = curr->children[idx];
    }
    curr->isEndOfWord = 1;
}

// Recursive function to print suggestions
void suggestionsRec(TrieNode *curr, char *prefix) {
    if (curr->isEndOfWord)
        printf("   %s\n", prefix);

    for (int i = 0; i < 26; i++) {
        if (curr->children[i]) {
            char newPrefix[100];
            sprintf(newPrefix, "%s%c", prefix, i + 'a');
            suggestionsRec(curr->children[i], newPrefix);
        }
    }
}

// Search for prefix in Trie and suggest words
void autocomplete(TrieNode *root, char *prefix) {
    TrieNode *curr = root;
    for (int i = 0; prefix[i] != '\0'; i++) {
        int idx = tolower(prefix[i]) - 'a';
        if (idx < 0 || idx >= 26 || curr->children[idx] == NULL) {
            printf("  No suggestions found for \"%s\"\n", prefix);
            return;
        }
        curr = curr->children[idx];
    }

    // If prefix exists, show suggestions
    printf("Suggestions for \"%s\":\n", prefix);
    suggestionsRec(curr, prefix);
}

// Free memory of Trie
void freeTrie(TrieNode *root) {
    if (!root) return;
    for (int i = 0; i < 26; i++)
        freeTrie(root->children[i]);
    free(root);
}

// Menu-driven interface (TUI style)
int main() {
    TrieNode *root = createNode();

    // Preload some contacts
    char *contacts[] = {"ananya", "anand", "ankit", "rahul", "ramesh", "riya", "amit", "arjun"};
    int n = sizeof(contacts) / sizeof(contacts[0]);
    for (int i = 0; i < n; i++)
        insert(root, contacts[i]);

    int choice;
    char input[100];

    do {
        printf("\n===== CONTACT AUTOCOMPLETE SYSTEM =====\n");
        printf("1. Add New Contact\n");
        printf("2. Search Suggestions\n");
        printf("3. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter new contact: ");
                scanf("%s", input);
                insert(root, input);
                printf(" Contact \"%s\" added successfully!\n", input);
                choice = 0;
                break;

            case 2:
                printf("Enter prefix to search: ");
                scanf("%s", input);
                autocomplete(root, input);
                choice = 0;
                break;

            case 3:
                printf("Exiting... Thank you!\n");
                break;

            default:
                printf(" Invalid choice. Try again.\n");
        }
    } while (choice != 3);

    freeTrie(root);
    return 0;
}
