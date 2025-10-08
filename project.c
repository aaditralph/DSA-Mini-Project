#include "trie.h"

int main()
{
    TrieNode *root = createNode();
    loadContactsFromFile(root);

    int choice;
    char nameInput[MAX_LEN];
    char numberInput[MAX_LEN];

    do
    {
        printf("\n===== CONTACT AUTOCOMPLETE SYSTEM =====\n");
        printf("1. Add New Contact\n");
        printf("2. Search Suggestions\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1)
        {
            choice = 0;
        }
        while (getchar() != '\n')
            ;

        switch (choice)
        {
        case 1:
            printf("Enter new contact name: ");
            scanf("%99s", nameInput);
            printf("Enter contact number: ");
            scanf("%99s", numberInput);
            insert(root, nameInput, numberInput);
            printf("Contact \"%s\" added successfully!\n", nameInput);
            break;

        case 2:
            printf("Enter prefix to search: ");
            scanf("%99s", nameInput);
            autocomplete(root, nameInput);
            break;

        case 3:
            saveContactsToFile(root);
            printf("Exiting... Thank you!\n");
            break;

        default:
            printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 3);

    freeTrie(root);
    return 0;
}
