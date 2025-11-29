#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "contact.h"
#include "file.h"

// Optimized: Added excludeIndex to skip current contact when editing
int uniquePhone(AddressBook *addressBook, const char *phone, int excludeIndex)
{
    for (int i = 0; i < addressBook->contactCount; i++)
    {
        if (i != excludeIndex && strcmp(addressBook->contacts[i].phone, phone) == 0)
        {
            printf(COLOR_RED "❌ Phone %s already exists at index %d!\n" COLOR_RESET, phone, i + 1);
            return 1;
        }
    }
    return 0;
}

int uniqueEmail(AddressBook *addressBook, const char *email, int excludeIndex)
{
    for (int i = 0; i < addressBook->contactCount; i++)
    {
        if (i != excludeIndex && strcmp(addressBook->contacts[i].email, email) == 0)
        {
            printf(COLOR_RED "❌ Email %s already exists at index %d!\n" COLOR_RESET, email, i + 1);
            return 1;
        }
    }
    return 0;
}

int validateName(const char *name)
{
    if (name[0] == '\0')
    {
        printf(COLOR_RED "❌ Name cannot be empty!\n" COLOR_RESET);
        return 1;
    }

    for (int i = 0; name[i]; i++)
    {
        if (!isalpha(name[i]) && name[i] != ' ')
        {
            printf(COLOR_RED "❌ Invalid name! Only letters and spaces allowed.\n" COLOR_RESET);
            return 1;
        }
    }
    return 0;
}

int validatePhone(const char *phone)
{
    int len = strlen(phone);
    
    if (len != 10)
    {
        printf(COLOR_RED "❌ Phone number must be exactly 10 digits!\n" COLOR_RESET);
        return 1;
    }

    for (int i = 0; i < len; i++)
    {
        if (!isdigit(phone[i]))
        {
            printf(COLOR_RED "❌ Phone number must contain only digits!\n" COLOR_RESET);
            return 1;
        }
    }
    return 0;
}

int validateEmail(const char *email)
{
    const char *at = strchr(email, '@');
    const char *dot = strrchr(email, '.');
    
    if (!at || !dot || at > dot || at == email || *(dot + 1) == '\0')
    {
        printf(COLOR_RED "❌ Invalid email format!\n" COLOR_RESET);
        return 1;
    }
    return 0;
}

// Optimized sorting using qsort with function pointers
static int compareByName(const void *a, const void *b)
{
    return strcmp(((Contact *)a)->name, ((Contact *)b)->name);
}

static int compareByPhone(const void *a, const void *b)
{
    return strcmp(((Contact *)a)->phone, ((Contact *)b)->phone);
}

static int compareByEmail(const void *a, const void *b)
{
    return strcmp(((Contact *)a)->email, ((Contact *)b)->email);
}

void listContacts(AddressBook *addressBook, int sortCriteria)
{
    if (addressBook->contactCount == 0)
    {
        printf(COLOR_YELLOW "📋 No contacts to display!\n" COLOR_RESET);
        return;
    }

    switch (sortCriteria)
    {
        case 1:
            qsort(addressBook->contacts, addressBook->contactCount, sizeof(Contact), compareByName);
            printf(COLOR_CYAN "📋 Sorted by Name\n" COLOR_RESET);
            break;
        case 2:
            qsort(addressBook->contacts, addressBook->contactCount, sizeof(Contact), compareByPhone);
            printf(COLOR_CYAN "📋 Sorted by Phone\n" COLOR_RESET);
            break;
        case 3:
            qsort(addressBook->contacts, addressBook->contactCount, sizeof(Contact), compareByEmail);
            printf(COLOR_CYAN "📋 Sorted by Email\n" COLOR_RESET);
            break;
        default:
            printf(COLOR_RED "❌ Invalid sort criteria!\n" COLOR_RESET);
            return;
    }

    printf(COLOR_BOLD "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n" COLOR_RESET);
    printf(COLOR_BOLD COLOR_MAGENTA "Index\t%-20s %-15s %-25s\n" COLOR_RESET, "Name", "Phone", "Email");
    printf(COLOR_BOLD "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n" COLOR_RESET);

    for (int i = 0; i < addressBook->contactCount; i++)
    {
        printf(COLOR_YELLOW "%d\t" COLOR_RESET, i + 1);
        printf(COLOR_WHITE "%-20s " COLOR_RESET, addressBook->contacts[i].name);
        printf(COLOR_CYAN "%-15s " COLOR_RESET, addressBook->contacts[i].phone);
        printf(COLOR_GREEN "%-25s\n" COLOR_RESET, addressBook->contacts[i].email);
    }
    printf(COLOR_BOLD "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n" COLOR_RESET);
}

void initialize(AddressBook *addressBook)
{
    addressBook->contactCount = 0;
    loadContactsFromFile(addressBook);
    printf(COLOR_GREEN "✅ Address Book initialized with %d contacts\n" COLOR_RESET, addressBook->contactCount);
}

void saveAndExit(AddressBook *addressBook)
{
    saveContactsToFile(addressBook);
    printf(COLOR_GREEN "✅ Contacts saved successfully. Goodbye!\n" COLOR_RESET);
    exit(EXIT_SUCCESS);
}

void createContact(AddressBook *addressBook)
{
    printf(COLOR_CYAN COLOR_BOLD "\n📝 Create New Contact\n" COLOR_RESET);
    printf(COLOR_BOLD "━━━━━━━━━━━━━━━━━━━━━━━━\n" COLOR_RESET);

    if (addressBook->contactCount >= MAX_CONTACTS)
    {
        printf(COLOR_RED "❌ Address book is full! (Maximum %d contacts)\n" COLOR_RESET, MAX_CONTACTS);
        return;
    }

    Contact newContact;
    char temp[50];

    do
    {
        printf(COLOR_YELLOW "👤 Enter name: " COLOR_RESET);
        scanf(" %[^\n]", temp);
        getchar();
    }
    while (validateName(temp));
    strcpy(newContact.name, temp);

    do
    {
        printf(COLOR_YELLOW "📱 Enter phone (10 digits): " COLOR_RESET);
        scanf("%s", temp);
        getchar();
    }
    while (validatePhone(temp) || uniquePhone(addressBook, temp, -1));
    strcpy(newContact.phone, temp);

    do
    {
        printf(COLOR_YELLOW "📧 Enter email: " COLOR_RESET);
        scanf("%s", temp);
        getchar();
    }
    while (validateEmail(temp) || uniqueEmail(addressBook, temp, -1));
    strcpy(newContact.email, temp);

    addressBook->contacts[addressBook->contactCount++] = newContact;
    printf(COLOR_GREEN COLOR_BOLD "✅ Contact added successfully!\n" COLOR_RESET);
}

int searchContact(AddressBook *addressBook)
{
    if (addressBook->contactCount == 0)
    {
        printf(COLOR_YELLOW "📋 No contacts available to search!\n" COLOR_RESET);
        return 1;
    }

    int opt;
    char temp[50];
    int found = 0;

    printf(COLOR_CYAN COLOR_BOLD "\n🔍 Search Contact\n" COLOR_RESET);
    printf(COLOR_BOLD "━━━━━━━━━━━━━━━━━━━━━━━━\n" COLOR_RESET);
    printf("1. Search by Name\n");
    printf("2. Search by Phone\n");
    printf("3. Search by Email\n");
    printf(COLOR_YELLOW "Enter choice: " COLOR_RESET);
    
    scanf("%d", &opt);
    getchar();

    switch (opt)
    {
        case 1:
            printf(COLOR_YELLOW "👤 Enter name to search: " COLOR_RESET);
            scanf(" %[^\n]", temp);
            getchar();

            printf(COLOR_BOLD "\n%-5s %-20s %-15s %-25s\n" COLOR_RESET, "Index", "Name", "Phone", "Email");
            printf(COLOR_BOLD "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n" COLOR_RESET);
            
            for (int i = 0; i < addressBook->contactCount; i++)
            {
                if (strcasestr(addressBook->contacts[i].name, temp))
                {
                    found = 1;
                    printf(COLOR_YELLOW "%-5d " COLOR_RESET, i + 1);
                    printf(COLOR_WHITE "%-20s " COLOR_RESET, addressBook->contacts[i].name);
                    printf(COLOR_CYAN "%-15s " COLOR_RESET, addressBook->contacts[i].phone);
                    printf(COLOR_GREEN "%-25s\n" COLOR_RESET, addressBook->contacts[i].email);
                }
            }
            break;

        case 2:
            printf(COLOR_YELLOW "📱 Enter phone to search: " COLOR_RESET);
            scanf("%s", temp);
            getchar();

            printf(COLOR_BOLD "\n%-5s %-20s %-15s %-25s\n" COLOR_RESET, "Index", "Name", "Phone", "Email");
            printf(COLOR_BOLD "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n" COLOR_RESET);
            
            for (int i = 0; i < addressBook->contactCount; i++)
            {
                if (strstr(addressBook->contacts[i].phone, temp))
                {
                    found = 1;
                    printf(COLOR_YELLOW "%-5d " COLOR_RESET, i + 1);
                    printf(COLOR_WHITE "%-20s " COLOR_RESET, addressBook->contacts[i].name);
                    printf(COLOR_CYAN "%-15s " COLOR_RESET, addressBook->contacts[i].phone);
                    printf(COLOR_GREEN "%-25s\n" COLOR_RESET, addressBook->contacts[i].email);
                }
            }
            break;

        case 3:
            printf(COLOR_YELLOW "📧 Enter email to search: " COLOR_RESET);
            scanf("%s", temp);
            getchar();

            printf(COLOR_BOLD "\n%-5s %-20s %-15s %-25s\n" COLOR_RESET, "Index", "Name", "Phone", "Email");
            printf(COLOR_BOLD "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n" COLOR_RESET);
            
            for (int i = 0; i < addressBook->contactCount; i++)
            {
                if (strcasestr(addressBook->contacts[i].email, temp))
                {
                    found = 1;
                    printf(COLOR_YELLOW "%-5d " COLOR_RESET, i + 1);
                    printf(COLOR_WHITE "%-20s " COLOR_RESET, addressBook->contacts[i].name);
                    printf(COLOR_CYAN "%-15s " COLOR_RESET, addressBook->contacts[i].phone);
                    printf(COLOR_GREEN "%-25s\n" COLOR_RESET, addressBook->contacts[i].email);
                }
            }
            break;

        default:
            printf(COLOR_RED "❌ Invalid option!\n" COLOR_RESET);
            return 1;
    }

    if (!found)
    {
        printf(COLOR_RED "❌ No matching contact found for: %s\n" COLOR_RESET, temp);
        return 1;
    }

    return 0;
}

void editContact(AddressBook *addressBook)
{
    printf(COLOR_CYAN COLOR_BOLD "\n✏️  Edit Contact\n" COLOR_RESET);
    printf(COLOR_BOLD "━━━━━━━━━━━━━━━━━━━━━━━━\n" COLOR_RESET);

    if (searchContact(addressBook))
    {
        return;
    }

    int index;
    printf(COLOR_YELLOW "\n📝 Enter index to edit: " COLOR_RESET);
    scanf("%d", &index);
    getchar();

    if (index <= 0 || index > addressBook->contactCount)
    {
        printf(COLOR_RED "❌ Invalid index!\n" COLOR_RESET);
        return;
    }

    int opt;
    char temp[50];

    printf("\n1. Edit Name\n");
    printf("2. Edit Phone\n");
    printf("3. Edit Email\n");
    printf(COLOR_YELLOW "Enter choice: " COLOR_RESET);
    scanf("%d", &opt);
    getchar();

    switch (opt)
    {
        case 1:
            do
            {
                printf(COLOR_YELLOW "👤 Enter new name: " COLOR_RESET);
                scanf(" %[^\n]", temp);
                getchar();
            }
            while (validateName(temp));
            strcpy(addressBook->contacts[index - 1].name, temp);
            break;

        case 2:
            do
            {
                printf(COLOR_YELLOW "📱 Enter new phone: " COLOR_RESET);
                scanf("%s", temp);
                getchar();
            }
            while (validatePhone(temp) || uniquePhone(addressBook, temp, index - 1));
            strcpy(addressBook->contacts[index - 1].phone, temp);
            break;

        case 3:
            do
            {
                printf(COLOR_YELLOW "📧 Enter new email: " COLOR_RESET);
                scanf("%s", temp);
                getchar();
            }
            while (validateEmail(temp) || uniqueEmail(addressBook, temp, index - 1));
            strcpy(addressBook->contacts[index - 1].email, temp);
            break;

        default:
            printf(COLOR_RED "❌ Invalid option!\n" COLOR_RESET);
            return;
    }

    printf(COLOR_GREEN "✅ Contact updated successfully!\n" COLOR_RESET);
}

void deleteContact(AddressBook *addressBook)
{
    printf(COLOR_CYAN COLOR_BOLD "\n🗑️  Delete Contact\n" COLOR_RESET);
    printf(COLOR_BOLD "━━━━━━━━━━━━━━━━━━━━━━━━\n" COLOR_RESET);

    if (searchContact(addressBook))
    {
        return;
    }

    int index;
    printf(COLOR_YELLOW "\n🗑️  Enter index to delete: " COLOR_RESET);
    scanf("%d", &index);
    getchar();

    if (index <= 0 || index > addressBook->contactCount)
    {
        printf(COLOR_RED "❌ Invalid index!\n" COLOR_RESET);
        return;
    }

    // Shift contacts left
    for (int i = index - 1; i < addressBook->contactCount - 1; i++)
    {
        addressBook->contacts[i] = addressBook->contacts[i + 1];
    }

    addressBook->contactCount--;
    printf(COLOR_GREEN "✅ Contact deleted successfully!\n" COLOR_RESET);
}