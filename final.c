#include <stdio.h>  // For standard input/output functions
#include <stdlib.h> // For general-purpose functions (e.g., file operations)
#include <string.h> // For string manipulation (e.g., comparison, copying)

// Define a structure to represent contact details
typedef struct
{
    char name[50];   // Name of the contact (up to 49 characters + null terminator)
    char number[15]; // Phone number (up to 14 characters + null terminator)
} Contact;

typedef struct
{
    char name[25];     // Name of the contact (up to 49 characters + null terminator)
    char fileName[25]; // Phone number (up to 14 characters + null terminator)
} Company;

Company companies[3] = {
    {"Airtel", "airtel_contacts.txt"}, // Company: Airtel
    {"JIO", "jio_contacts.txt"},       // Company: JIO
    {"VI", "vi_contacts.txt"}          // Company: VI
};
int companiesSize = 3;

// Function declarations for managing contact operations
void addContact(const char *filePath);                            // Adds a new contact to a specific file
void viewContacts(const char *filePath);                          // Displays all contacts in a file
void searchContact(const char *filePath);                         // Searches for a contact by number
void updateContact(const char *filePath);                         // Updates details of an existing contact
void removeContact(const char *filePath);                         // Deletes a contact from a file
void moveContact(const char *sourceFile, const char *targetFile); // Transfers a contact to another file
void manageProviderOptions(int providerChoice);                   // Menu handler for managing contacts by provider

const char *MENU_OPTIONS[] = {
    "Transfer Contact", // Option to transfer a contact between providers
    "Exit"              // Option to exit the program
};
int menuOptionsSize = 2;

void printCompanies(char *prefix, char *suffix)
{
    for (int index = 0; index < companiesSize; index++)
    {
        printf("%d. %s%s%s\n", index + 1, prefix, companies[index].name, suffix);
    }
}

int isValidPhoneNumberString(const char *str)
{
    // If the string is empty, return false
    if (str == NULL || str[0] == '\0')
    {
        return 0;
    }

    // Iterate through each character in the phone number
    for (int i = 0; str[i] != '\0'; i++)
    {
        // Allow '+' only as the first character
        if (str[i] == '+')
        {
            if (i != 0)
            {
                return 0;
            }
        }
        // Check if the character is not a digit
        else if (str[i] < '0' || str[i] > '9')
        {
            return 0;
        }
    }
    return 1; // If all characters are valid, return true
}

void getPhoneNumber(char *prefix, char *suffix, char *inp)
{
    printf("Enter the %sphone number%s: ", prefix, suffix);
    scanf("%s", inp);

    int res = isValidPhoneNumberString(inp);
    if (res == 0)
    {
        printf("\nWarning: Invalid Phone Number Detected!\n");
    }
}

// Main function that serves as the entry point of the program
int main()
{
    int choice; // Variable to store the user's choice from the main menu

    do
    {
        // Display the main menu
        printf("\n=== Contact and Service Provider Management System ===\n");
        printCompanies("Manage ", " Contacts");
        for (int index = companiesSize; index < menuOptionsSize + companiesSize; index++)
        {
            printf("%d. %s\n", index + 1, MENU_OPTIONS[index - companiesSize]);
        }
        printf("Select an option: ");
        scanf("%d", &choice); // Get user input for the choice

        switch (choice)
        {
        case 1:
        case 2:
        case 3:
            // Pass the provider choice to the function to handle provider-specific options
            manageProviderOptions(choice);
            break;
        case 4:
        {
            // Handle transferring a contact between providers
            int from, to; // Variables for source and target providers
            printf("\nSelect Source Provider:\n");
            printCompanies("", "");
            scanf("%d", &from); // Get source provider
            printf("\nSelect Target Provider:\n");
            printCompanies("", "");
            scanf("%d", &to); // Get target provider

            if (from != to)
            { // Ensure source and target providers are not the same
                const char *sourceFile = companies[from - 1].fileName;
                const char *targetFile = companies[to - 1].fileName;
                moveContact(sourceFile, targetFile); // Call function to move contact
            }
            else
            {
                printf("\nSource and destination providers must be different.\n");
            }
            break;
        }
        case 5:
            // Exit the program
            printf("\nExiting... Have a great day!\n");
            break;
        default:
            // Handle invalid input
            printf("\nError: Invalid choice. Please try again.\n");
        }
    } while (choice != 5); // Repeat until the user chooses to exit

    return 0;
}

// Handles operations for a specific provider
void manageProviderOptions(int providerChoice)
{
    const char *filePath = companies[providerChoice - 1].fileName;
    int subOption;

    do
    {
        // Display sub-menu for managing contacts of the chosen provider
        printf("\n=== Manage Contacts (%s) ===\n", providerChoice == 1 ? "Airtel" : (providerChoice == 2 ? "JIO" : "VI"));
        printf("1. Add Contact\n");
        printf("2. View Contacts\n");
        printf("3. Search Contact\n");
        printf("4. Edit Contact\n");
        printf("5. Delete Contact\n");
        printf("6. Return to Main Menu\n");
        printf("Select an option: ");
        scanf("%d", &subOption); // Get user's sub-menu choice

        switch (subOption)
        {
        case 1:
            addContact(filePath); // Call function to add a new contact
            break;
        case 2:
            viewContacts(filePath); // Call function to view all contacts
            break;
        case 3:
            searchContact(filePath); // Call function to search for a specific contact
            break;
        case 4:
            updateContact(filePath); // Call function to update contact details
            break;
        case 5:
            removeContact(filePath); // Call function to delete a contact
            break;
        case 6:
            printf("\nReturning to Main Menu.\n");
            break;
        default:
            printf("\nError: Invalid option. Try again.\n");
        }
    } while (subOption != 6); // Loop until the user chooses to return to the main menu
}

// Adds a new contact to the specified file
void addContact(const char *filePath)
{
    Contact contact;                   // Structure to hold new contact details
    FILE *file = fopen(filePath, "a"); // Open the file in append mode

    if (!file)
    { // Check if the file opened successfully
        perror("Error opening file");
        return;
    }

    // Get new contact details from the user
    printf("Enter name: ");
    scanf("%s", contact.name);
    getPhoneNumber("", "", contact.number);

    // Write the new contact to the file
    fprintf(file, "%s %s\n", contact.name, contact.number);
    fclose(file); // Close the file
    printf("\nContact added successfully!\n");
}

// Displays all contacts stored in the file
void viewContacts(const char *filePath)
{
    Contact contact;                   // Structure to hold contact details during reading
    FILE *file = fopen(filePath, "r"); // Open the file in read mode

    if (!file)
    { // Check if the file opened successfully
        perror("\nError opening file");
        return;
    }

    // Display all contacts in the file
    printf("\nContacts List:\n");
    while (fscanf(file, "%s %s", contact.name, contact.number) == 2)
    {
        printf("Name: %s, Number: %s\n", contact.name, contact.number);
    }
    fclose(file); // Close the file
}

// Searches for a contact by phone number
void searchContact(const char *filePath)
{
    Contact contact;                   // Structure to hold contact details during reading
    char searchNum[15];                // Phone number to search for
    int found = 0;                     // Flag to indicate if the contact was found
    FILE *file = fopen(filePath, "r"); // Open the file in read mode

    if (!file)
    { // Check if the file opened successfully
        perror("Error opening file");
        return;
    }

    // Get the phone number to search for
    getPhoneNumber("", " to search", searchNum);

    // Search the file for the contact
    while (fscanf(file, "%s %s", contact.name, contact.number) == 2)
    {
        if (strcmp(contact.number, searchNum) == 0)
        {
            printf("Contact Found: Name: %s, Number: %s\n", contact.name, contact.number);
            found = 1; // Set the flag if found
            break;
        }
    }

    if (!found)
    { // If no contact is found
        printf("\nError: No matching contact found.\n");
    }
    fclose(file); // Close the file
}

// Updates details of an existing contact in the file
void updateContact(const char *filePath)
{
    FILE *file = fopen(filePath, "r");
    FILE *tempFile = fopen("temp.txt", "w");
    Contact contact;
    char targetNum[15];
    int updated = 0;

    if (!file || !tempFile)
    {
        perror("Error opening file");
        if (file)
            fclose(file);
        if (tempFile)
            fclose(tempFile);
        return;
    }

    getPhoneNumber("", "of the contact to update", targetNum);

    while (fscanf(file, "%s %s", contact.name, contact.number) == 2)
    {
        if (strcmp(contact.number, targetNum) == 0)
        {
            printf("Enter new name: ");
            scanf("%s", contact.name);
            getPhoneNumber("new ", "", contact.number);
            updated = 1;
        }
        fprintf(tempFile, "%s %s\n", contact.name, contact.number);
    }

    fclose(file);
    fclose(tempFile);

    if (updated)
    {
        remove(filePath);
        rename("temp.txt", filePath);
        printf("\nContact updated successfully.\n");
    }
    else
    {
        remove("temp.txt");
        printf("\nError: No matching contact found.\n");
    }
}

// Removes a contact from the specified file
void removeContact(const char *filePath)
{
    FILE *file = fopen(filePath, "r");
    FILE *tempFile = fopen("temp.txt", "w");
    Contact contact;
    char targetNum[15];
    int removed = 0;

    if (!file || !tempFile)
    {
        perror("Error opening file");
        if (file)
            fclose(file);
        if (tempFile)
            fclose(tempFile);
        return;
    }

    getPhoneNumber("", " of the contact to delete", targetNum);

    while (fscanf(file, "%s %s", contact.name, contact.number) == 2)
    {
        if (strcmp(contact.number, targetNum) != 0)
        {
            fprintf(tempFile, "%s %s\n", contact.name, contact.number);
        }
        else
        {
            removed = 1;
        }
    }

    fclose(file);
    fclose(tempFile);

    if (removed)
    {
        remove(filePath);
        rename("temp.txt", filePath);
        printf("\nContact deleted successfully.\n");
    }
    else
    {
        remove("temp.txt");
        printf("\nError: No matching contact found.\n");
    }
}

// Moves a contact from one provider's file to another
void moveContact(const char *sourceFile, const char *targetFile)
{
    FILE *file = fopen(sourceFile, "r");
    FILE *tempFile = fopen("temp.txt", "w");
    FILE *target = fopen(targetFile, "a");
    Contact contact;
    char targetNum[15];
    int moved = 0;

    if (!file || !tempFile || !target)
    {
        perror("Error opening file");
        if (file)
            fclose(file);
        if (tempFile)
            fclose(tempFile);
        if (target)
            fclose(target);
        return;
    }

    getPhoneNumber("", " of the contact to transfer", targetNum);

    while (fscanf(file, "%s %s", contact.name, contact.number) == 2)
    {
        if (strcmp(contact.number, targetNum) == 0)
        {
            fprintf(target, "%s %s\n", contact.name, contact.number);
            moved = 1;
        }
        else
        {
            fprintf(tempFile, "%s %s\n", contact.name, contact.number);
        }
    }

    fclose(file);
    fclose(tempFile);
    fclose(target);

    if (moved)
    {
        remove(sourceFile);
        rename("temp.txt", sourceFile);
        printf("\nContact transferred successfully.\n");
    }
    else
    {
        remove("temp.txt");
        printf("\nError: No matching contact found.\n");
    }
}