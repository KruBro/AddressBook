#include <stdio.h>
#include "file.h"
#include "contact.h"

void saveContactsToFile(AddressBook *addressBook) 
{
    FILE *fp = fopen("contacts.csv", "w");
    if (fp == NULL)
    {
        printf(COLOR_RED "❌ Error: Unable to save contacts to file!\n" COLOR_RESET);
        return;
    }

    for (int i = 0; i < addressBook->contactCount; i++)
    {
        fprintf(fp, "%s,%s,%s\n",
            addressBook->contacts[i].name,
            addressBook->contacts[i].phone,
            addressBook->contacts[i].email
        );
    }

    fclose(fp);
    printf(COLOR_GREEN "✅ %d contact(s) saved to file.\n" COLOR_RESET, addressBook->contactCount);
}

void loadContactsFromFile(AddressBook *addressBook) 
{
    FILE *fp = fopen("contacts.csv", "r");
    if (fp == NULL)
    {
        printf(COLOR_YELLOW "⚠️  No existing contacts file found. Starting fresh.\n" COLOR_RESET);
        addressBook->contactCount = 0;
        return;
    }

    addressBook->contactCount = 0;

    while (addressBook->contactCount < MAX_CONTACTS &&
           fscanf(fp, " %49[^,],%19[^,],%49[^\n]\n",
                  addressBook->contacts[addressBook->contactCount].name,
                  addressBook->contacts[addressBook->contactCount].phone,
                  addressBook->contacts[addressBook->contactCount].email) == 3)
    {
        addressBook->contactCount++;
    }

    fclose(fp);
    
    if (addressBook->contactCount > 0)
    {
        printf(COLOR_GREEN "✅ Loaded %d contact(s) from file.\n" COLOR_RESET, addressBook->contactCount);
    }
}