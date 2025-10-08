#include "trie.h"

typedef struct TrieNode
{
    struct TrieNode *children[26];
    int isEndOfWord;
    char *contactNumber;
} TrieNode;

TrieNode *createNode()
{
    TrieNode *node = (TrieNode *)malloc(sizeof(TrieNode));
    if (!node)
    {
        perror("Failed to allocate memory for TrieNode");
        exit(EXIT_FAILURE);
    }
    node->isEndOfWord = 0;
    node->contactNumber = NULL;
    for (int i = 0; i < 26; i++)
        node->children[i] = NULL;
    return node;
}

void insert(TrieNode *root, const char *word, const char *number)
{
    TrieNode *curr = root;
    for (int i = 0; word[i] != '\0'; i++)
    {
        char ch = tolower(word[i]);
        int idx = ch - 'a';
        if (idx < 0 || idx >= 26)
            continue;

        if (curr->children[idx] == NULL)
            curr->children[idx] = createNode();
        curr = curr->children[idx];
    }
    curr->isEndOfWord = 1;

    if (curr->contactNumber)
    {
        free(curr->contactNumber);
    }

    curr->contactNumber = (char *)malloc(strlen(number) + 1);
    if (!curr->contactNumber)
    {
        perror("Failed to allocate memory for contact number");
        return;
    }
    strcpy(curr->contactNumber, number);
}

void suggestionsRec(TrieNode *curr, char *prefix)
{
    if (curr->isEndOfWord)
    {
        printf("   - %s - %s \n", prefix, curr->contactNumber);
    }
    for (int i = 0; i < 26; i++)
    {
        if (curr->children[i])
        {
            // **FIX**: Efficiently build prefix and backtrack to avoid stack overflows.
            int len = strlen(prefix);
            prefix[len] = i + 'a';
            prefix[len + 1] = '\0';
            suggestionsRec(curr->children[i], prefix);
            prefix[len] = '\0';
        }
    }
}

void autocomplete(TrieNode *root, const char *prefix)
{
    TrieNode *curr = root;
    for (int i = 0; prefix[i] != '\0'; i++)
    {
        int idx = tolower(prefix[i]) - 'a';
        if (idx < 0 || idx >= 26 || curr->children[idx] == NULL)
        {
            printf("  No suggestions found for \"%s\"\n", prefix);
            return;
        }
        curr = curr->children[idx];
    }

    printf("Suggestions for \"%s\":\n", prefix);
    // Use a mutable copy for the recursive suggestions
    char prefixCopy[MAX_LEN];
    strncpy(prefixCopy, prefix, MAX_LEN - 1);
    prefixCopy[MAX_LEN - 1] = '\0';
    suggestionsRec(curr, prefixCopy);
}

void freeTrie(TrieNode *root)
{
    if (!root)
        return;
    // **FIX**: Use post-order traversal to free memory correctly.
    for (int i = 0; i < 26; i++)
        freeTrie(root->children[i]);

    if (root->contactNumber)
    {
        free(root->contactNumber);
    }
    free(root);
}

//  JSON Persistence Functions

void saveContactsUtil(TrieNode *root, char *word, int level, cJSON *json_array)
{
    if (root->isEndOfWord && root->contactNumber)
    {
        word[level] = '\0';
        cJSON *contact_obj = cJSON_CreateObject();
        cJSON_AddStringToObject(contact_obj, "name", word);
        cJSON_AddStringToObject(contact_obj, "number", root->contactNumber);
        cJSON_AddItemToArray(json_array, contact_obj);
    }

    for (int i = 0; i < 26; i++)
    {
        if (root->children[i])
        {
            word[level] = i + 'a';
            saveContactsUtil(root->children[i], word, level + 1, json_array);
        }
    }
}

void saveContactsToFile(TrieNode *root)
{
    FILE *fp = fopen(FILENAME, "w");
    if (fp == NULL)
    {
        perror("Error opening file for writing");
        return;
    }

    cJSON *root_json = cJSON_CreateArray();
    char word[MAX_LEN] = {0};
    saveContactsUtil(root, word, 0, root_json);

    char *json_string = cJSON_Print(root_json);
    fprintf(fp, "%s", json_string);

    fclose(fp);
    free(json_string);
    cJSON_Delete(root_json);

    printf("Contacts saved to %s\n", FILENAME);
}

void loadContactsFromFile(TrieNode *root)
{
    FILE *fp = fopen(FILENAME, "r");
    if (fp == NULL)
    {
        printf("Contact file '%s' not found. A new one will be created on exit.\n", FILENAME);
        return;
    }

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char *buffer = (char *)malloc(size + 1);
    if (!buffer)
    {
        perror("Failed to allocate buffer for file reading");
        fclose(fp);
        return;
    }
    fread(buffer, 1, size, fp);
    fclose(fp);
    buffer[size] = '\0';

    cJSON *root_json = cJSON_Parse(buffer);
    if (root_json == NULL)
    {
        fprintf(stderr, "Error parsing JSON. File might be corrupt.\n");
        free(buffer);
        return;
    }

    cJSON *contact_obj = NULL;
    cJSON_ArrayForEach(contact_obj, root_json)
    {
        cJSON *name = cJSON_GetObjectItemCaseSensitive(contact_obj, "name");
        cJSON *number = cJSON_GetObjectItemCaseSensitive(contact_obj, "number");
        if (cJSON_IsString(name) && cJSON_IsString(number))
        {
            insert(root, name->valuestring, number->valuestring);
        }
    }

    cJSON_Delete(root_json);
    free(buffer);
    printf("Contacts loaded successfully from %s\n", FILENAME);
}
