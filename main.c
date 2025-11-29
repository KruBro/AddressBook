#include <stdio.h>
#include <stdlib.h>
#include "contact.h"

void displayMenu(void)
{
    printf("\n");
    printf(COLOR_BOLD COLOR_MAGENTA "╔════════════════════════════════════╗\n");
    printf("║      📖 ADDRESS BOOK MENU          ║\n");
    printf("╚════════════════════════════════════╝\n" COLOR_RESET);
    printf(COLOR_CYAN "  1. 📝 Create Contact\n");
    printf("  2. 🔍 Search Contact\n");
    printf("  3. ✏️  Edit Contact\n");
    printf("  4. 🗑️  Delete Contact\n");
    printf("  5. 📋 List All Contacts\n");
    printf("  6. 💾 Save and Exit\n" COLOR_RESET);
    printf(COLOR_BOLD "────────────────────────────────────\n" COLOR_RESET);
    printf(COLOR_YELLOW "Enter your choice: " COLOR_RESET);
}

int main(void) 
{
    int choice;
    AddressBook addressBook;
    
    printf(COLOR_BOLD COLOR_GREEN "\n");
    printf("╔════════════════════════════════════╗\n");
    printf("║   Welcome to Address Book App!     ║\n");
    printf("╚════════════════════════════════════╝\n");
    printf(COLOR_RESET);
    
    initialize(&addressBook);

    do {
        displayMenu();
        
        if (scanf("%d", &choice) != 1)
        {
            printf(COLOR_RED "❌ Invalid input! Please enter a number.\n" COLOR_RESET);
            while (getchar() != '\n'); // Clear input buffer
            continue;
        }
        getchar(); // Consume newline
        
        switch (choice) 
        {
            case 1:
                createContact(&addressBook);
                break;
                
            case 2:
                searchContact(&addressBook);
                break;
                
            case 3:
                editContact(&addressBook);
                break;
                
            case 4:
                deleteContact(&addressBook);
                break;
                
            case 5:
                printf("\n" COLOR_CYAN "Select sort criteria:\n" COLOR_RESET);
                printf("  1. Sort by Name\n");
                printf("  2. Sort by Phone\n");
                printf("  3. Sort by Email\n");
                printf(COLOR_YELLOW "Enter your choice: " COLOR_RESET);
                
                int sortChoice;
                if (scanf("%d", &sortChoice) != 1)
                {
                    printf(COLOR_RED "❌ Invalid input!\n" COLOR_RESET);
                    while (getchar() != '\n');
                    break;
                }
                getchar();
                
                listContacts(&addressBook, sortChoice);
                break;
                
            case 6:
                printf(COLOR_GREEN "\n💾 Saving contacts...\n" COLOR_RESET);
                saveAndExit(&addressBook);
                break;
                
            default:
                printf(COLOR_RED "❌ Invalid choice! Please enter 1-6.\n" COLOR_RESET);
        }
    } while (choice != 6);
    
    return 0;
}