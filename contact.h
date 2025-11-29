#ifndef CONTACT_H
#define CONTACT_H

#define MAX_CONTACTS 100

// ANSI Color codes
#define COLOR_RESET   "\x1b[0m"
#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_WHITE   "\x1b[37m"
#define COLOR_BOLD    "\x1b[1m"

typedef struct {
    char name[50];
    char phone[20];
    char email[50];
} Contact;

typedef struct {
    Contact contacts[MAX_CONTACTS];
    int contactCount;
} AddressBook;

// Core functions
void createContact(AddressBook *addressBook);
int searchContact(AddressBook *addressBook);
void editContact(AddressBook *addressBook);
void deleteContact(AddressBook *addressBook);
void listContacts(AddressBook *addressBook, int sortCriteria);
void initialize(AddressBook *addressBook);
void saveAndExit(AddressBook *addressBook);

// Validation functions
int validateName(const char *name);
int validatePhone(const char *phone);
int validateEmail(const char *email);
int uniquePhone(AddressBook *addressBook, const char *phone, int excludeIndex);
int uniqueEmail(AddressBook *addressBook, const char *email, int excludeIndex);

#endif