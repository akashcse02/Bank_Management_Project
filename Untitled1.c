#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ACCOUNTS 100

struct BankAccount {
    int accountNumber;
    char name[100];
    int nidNumber;
    float balance;
    char password[50];
};

struct BankAccount accounts[MAX_ACCOUNTS];
int totalAccounts = 0;

void clearStdin() {
    while (getchar() != '\n');
}

// -------- File Handling --------
void loadAccountsFromFile() {
    FILE *fp = fopen("accounts.txt", "r");
    if (fp == NULL) {
        totalAccounts = 0;
        return;
    }

    totalAccounts = 0;
    while (fscanf(fp, "%5d | %[^|]| %7d | %f | %[^\n]\n",
           &accounts[totalAccounts].accountNumber,
           accounts[totalAccounts].name,
           &accounts[totalAccounts].nidNumber,
           &accounts[totalAccounts].balance,
           accounts[totalAccounts].password) == 5) {
        totalAccounts++;
        if (totalAccounts >= MAX_ACCOUNTS) break;
    }

    fclose(fp);
}

void saveAccountsToFile() {
    FILE *fp = fopen("accounts.txt", "w");
    if (fp == NULL) {
        printf("Error writing to file.\n");
        return;
    }

    for (int i = 0; i < totalAccounts; i++) {
        fprintf(fp, "%05d | %s| %07d | %.2f | %s\n",
                accounts[i].accountNumber,
                accounts[i].name,
                accounts[i].nidNumber,
                accounts[i].balance,
                accounts[i].password);
    }

    fclose(fp);
}

// -------- Account Functions --------
void createAccount() {
    if (totalAccounts >= MAX_ACCOUNTS) {
        printf("Maximum account limit reached!\n");
        return;
    }

    int accNum, nidNum;
    float initBalance;
    char accName[100];
    char pass[50];

    printf("\n--- Create New Account ---\n");

    while (1) {
        printf("Enter 5-digit account number: ");
        if (scanf("%d", &accNum) != 1 || accNum < 10000 || accNum > 99999) {
            printf("Invalid input! Must be 5 digits.\n");
            clearStdin();
        } else {
            int duplicate = 0;
            for (int i = 0; i < totalAccounts; i++) {
                if (accounts[i].accountNumber == accNum) {
                    printf("Account number already exists! Try again.\n");
                    duplicate = 1;
                    break;
                }
            }
            if (!duplicate) break;
        }
    }

    printf("Enter account holder name: ");
    clearStdin();
    fgets(accName, sizeof(accName), stdin);
    accName[strcspn(accName, "\n")] = '\0';

    while (1) {
        printf("Enter NID number (7 digits): ");
        if (scanf("%d", &nidNum) != 1 || nidNum < 1000000 || nidNum > 9999999) {
            printf("Invalid NID! Must be 7 digits.\n");
            clearStdin();
        } else break;
    }

    while (1) {
        printf("Enter initial balance: $");
        if (scanf("%f", &initBalance) != 1 || initBalance < 0) {
            printf("Invalid amount! Try again.\n");
            clearStdin();
        } else break;
    }

    printf("Set a password: ");
    clearStdin();
    fgets(pass, sizeof(pass), stdin);
    pass[strcspn(pass, "\n")] = '\0';

    accounts[totalAccounts].accountNumber = accNum;
    strcpy(accounts[totalAccounts].name, accName);
    accounts[totalAccounts].nidNumber = nidNum;
    accounts[totalAccounts].balance = initBalance;
    strcpy(accounts[totalAccounts].password, pass);

    totalAccounts++;
    saveAccountsToFile();
    printf("Account created successfully!\n");
}

void listAllAccounts() {
    printf("\n--- List of All Accounts ---\n");
    if (totalAccounts == 0) {
        printf("No accounts found!\n");
        return;
    }

    for (int i = 0; i < totalAccounts; i++) {
        printf("\nAccount Number: %05d\n", accounts[i].accountNumber);
        printf("Account Holder: %s\n", accounts[i].name);
        printf("NID Number    : %07d\n", accounts[i].nidNumber);
        printf("Balance       : $%.2f\n", accounts[i].balance);
    }
}

void checkAccount() {
    int accNo;
    char pass[50];
    printf("\n--- Check Account ---\n");
    printf("Enter account number: ");
    scanf("%d", &accNo);
    printf("Enter password: ");
    clearStdin();
    fgets(pass, sizeof(pass), stdin);
    pass[strcspn(pass, "\n")] = '\0';

    for (int i = 0; i < totalAccounts; i++) {
        if (accounts[i].accountNumber == accNo) {
            if (strcmp(accounts[i].password, pass) == 0) {
                printf("\nAccount Number : %05d\n", accounts[i].accountNumber);
                printf("Name           : %s\n", accounts[i].name);
                printf("NID Number     : %07d\n", accounts[i].nidNumber);
                printf("Balance        : $%.2f\n", accounts[i].balance);
                return;
            } else {
                printf("Incorrect password!\n");
                return;
            }
        }
    }
    printf("Account not found!\n");
}

void depositMoney() {
    int accNo;
    float amount;
    char pass[50];
    printf("\n--- Deposit Money ---\n");
    printf("Enter account number: ");
    scanf("%d", &accNo);
    printf("Enter password: ");
    clearStdin();
    fgets(pass, sizeof(pass), stdin);
    pass[strcspn(pass, "\n")] = '\0';

    for (int i = 0; i < totalAccounts; i++) {
        if (accounts[i].accountNumber == accNo) {
            if (strcmp(accounts[i].password, pass) == 0) {
                printf("Enter amount to deposit: $");
                if (scanf("%f", &amount) == 1 && amount > 0) {
                    accounts[i].balance += amount;
                    saveAccountsToFile();
                    printf("Deposit successful. New balance: $%.2f\n", accounts[i].balance);
                } else {
                    printf("Invalid amount!\n");
                }
                return;
            } else {
                printf("Incorrect password!\n");
                return;
            }
        }
    }
    printf("Account not found!\n");
}

void withdrawMoney() {
    int accNo;
    float amount;
    char pass[50];
    printf("\n--- Withdraw Money ---\n");
    printf("Enter account number: ");
    scanf("%d", &accNo);
    printf("Enter password: ");
    clearStdin();
    fgets(pass, sizeof(pass), stdin);
    pass[strcspn(pass, "\n")] = '\0';

    for (int i = 0; i < totalAccounts; i++) {
        if (accounts[i].accountNumber == accNo) {
            if (strcmp(accounts[i].password, pass) == 0) {
                printf("Enter amount to withdraw: $");
                if (scanf("%f", &amount) == 1 && amount > 0 && amount <= accounts[i].balance) {
                    accounts[i].balance -= amount;
                    saveAccountsToFile();
                    printf("Withdraw successful. New balance: $%.2f\n", accounts[i].balance);
                } else {
                    printf("Invalid or insufficient funds!\n");
                }
                return;
            } else {
                printf("Incorrect password!\n");
                return;
            }
        }
    }
    printf("Account not found!\n");
}

void updateAccount() {
    int accNo;
    char pass[50];
    printf("\n--- Update Account ---\n");
    printf("Enter account number: ");
    scanf("%d", &accNo);
    printf("Enter password: ");
    clearStdin();
    fgets(pass, sizeof(pass), stdin);
    pass[strcspn(pass, "\n")] = '\0';

    for (int i = 0; i < totalAccounts; i++) {
        if (accounts[i].accountNumber == accNo) {
            if (strcmp(accounts[i].password, pass) == 0) {
                printf("Enter new name: ");
                clearStdin();
                fgets(accounts[i].name, sizeof(accounts[i].name), stdin);
                accounts[i].name[strcspn(accounts[i].name, "\n")] = '\0';

                while (1) {
                    printf("Enter new NID number (7 digits): ");
                    if (scanf("%d", &accounts[i].nidNumber) != 1 || accounts[i].nidNumber < 1000000 || accounts[i].nidNumber > 9999999) {
                        printf("Invalid NID!\n");
                        clearStdin();
                    } else break;
                }

                printf("Enter new password: ");
                clearStdin();
                fgets(accounts[i].password, sizeof(accounts[i].password), stdin);
                accounts[i].password[strcspn(accounts[i].password, "\n")] = '\0';

                saveAccountsToFile();
                printf("Account updated successfully!\n");
                return;
            } else {
                printf("Incorrect password!\n");
                return;
            }
        }
    }
    printf("Account not found!\n");
}

void deleteAccount() {
    int accNo;
    char pass[50];
    printf("\n--- Delete Account ---\n");
    printf("Enter account number: ");
    scanf("%d", &accNo);
    printf("Enter password: ");
    clearStdin();
    fgets(pass, sizeof(pass), stdin);
    pass[strcspn(pass, "\n")] = '\0';

    for (int i = 0; i < totalAccounts; i++) {
        if (accounts[i].accountNumber == accNo) {
            if (strcmp(accounts[i].password, pass) == 0) {
                for (int j = i; j < totalAccounts - 1; j++) {
                    accounts[j] = accounts[j + 1];
                }
                totalAccounts--;
                saveAccountsToFile();
                printf("Account deleted successfully!\n");
                return;
            } else {
                printf("Incorrect password!\n");
                return;
            }
        }
    }
    printf("Account not found!\n");
}

// -------- Main --------
int main() {
    loadAccountsFromFile();
    int choice;

    while (1) {
        printf("\n--- Bank Management ---\n");
        printf("1. Create Account\n");
        printf("2. Check Account\n");
        printf("3. Deposit\n");
        printf("4. Withdraw\n");
        printf("5. Update Account\n");
        printf("6. Delete Account\n");
        printf("7. List Accounts\n");
        printf("8. Exit\n");
        printf("Choose option: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input!\n");
            clearStdin();
            continue;
        }

        switch (choice) {
            case 1: createAccount(); break;
            case 2: checkAccount(); break;
            case 3: depositMoney(); break;
            case 4: withdrawMoney(); break;
            case 5: updateAccount(); break;
            case 6: deleteAccount(); break;
            case 7: listAllAccounts(); break;
            case 8: printf("Goodbye!\n"); return 0;
            default: printf("Invalid choice!\n");
        }
    }

    return 0;
}
