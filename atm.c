#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#define MAX_TRIES 3

#ifdef _WIN32
    #define CLEAR "cls"
#else
    #define CLEAR "clear"
#endif

int readInteger(void);
double readDouble(void);
void clearBuffer(void);
void readCharacterString(char aString[], int stringSize);
void pressEnter(void);
void displayMenu(void);
void querryBalance(double);
void depositMoney(double*);
void withdrawMoney(double*);
void overWriteSaldo(char*, double*, char*);


int main(void) {

    printf("*** Welcome to use the ATM! ***\n");
    char accNumber[256];
    char pinCode[256], checkPin[256];
    double saldo;
    FILE *pFile;
    int triesnum = 0;
    int result;


    while(1) {

        triesnum = 0;
        printf("\nGive account number: ");
        readCharacterString(accNumber, 256);

        strcat(accNumber, ".acc");

        pFile = fopen(accNumber, "r+");

        if (pFile == NULL) {
            printf("\nWrong account number!\n");
            continue;
        }
        else {
            fgets(checkPin, 256, pFile);
            if (checkPin[strlen(checkPin)-1] == '\n') {
                checkPin[strlen(checkPin)-1] = '\0';
            }
            if (checkPin[strlen(checkPin)-1] == '\r') {
                checkPin[strlen(checkPin)-1] = '\0';
            }

            do {
                system(CLEAR);
                printf("Give your account PIN code (%d. try): ", (triesnum+1));
                triesnum++;
                readCharacterString(pinCode, 256);
                if ((result = strcmp(pinCode, checkPin)) != 0) {

                    printf("\n***** Wrong PIN code! (Press Enter)");

                    if (triesnum == MAX_TRIES) {
                        printf("\n\n***** Too many wrong attempts!\n");
                        printf("\nYour account is closed\n");
                        fclose(pFile);
                        printf("\nPress Enter to restart the process!");
                        pressEnter();
                        system(CLEAR);
                        printf("*** Welcome to use the ATM! ***\n");
                        break;
                    }
                    pressEnter();
                    }

                else {
                    system(CLEAR);
                    fscanf(pFile, "%lf", &saldo);
                    int choice = 0;
                    while (choice != 9) {
                        displayMenu();
                        choice = readInteger();
                        switch (choice) {
                            case 1:
                                withdrawMoney(&saldo);
                                pressEnter();
                                system(CLEAR);
                                break;
                            case 2:
                                depositMoney(&saldo);
                                pressEnter();
                                system(CLEAR);
                                break;
                            case 3:
                                querryBalance(saldo);
                                pressEnter();
                                system(CLEAR);
                                break;
                            case 9:
                                fclose(pFile);
                                remove(accNumber);
                                overWriteSaldo(checkPin, &saldo, accNumber);
                                pressEnter();
                                system(CLEAR);
                                printf("*** Welcome to use the ATM! ***\n");
                                break;
                            default:
                                printf("\nWrong option, try again.\n");
                                break;
                        }
                    }
                }
            } while (result != 0);
        }

    }


    return 0;
}

void overWriteSaldo(char* checkPin, double* userBalance, char* accNumber) {
    FILE* tempFile;
    tempFile = fopen("temp.txt", "w");
    fprintf(tempFile, "%s\n%.2lf", checkPin, *userBalance);
    fclose(tempFile);
    rename("temp.txt", accNumber);
    printf("\nThe balance %.2lf saved to the account file \"%s\".", *userBalance, accNumber);
    printf("\n\nPress enter");
}

void withdrawMoney(double* userBalance) {

    int withdrawAmount;
    int twentyNotes;
    int fiftyNotes;

    do {
        printf("\nGive the amount to withdraw (full tens: 20, 40-1000): ");
        withdrawAmount = readInteger();

        if (withdrawAmount < 20 || withdrawAmount > 1000 || ((withdrawAmount % 10) != 0) || withdrawAmount == 30)
            {continue;}
        else if ((*userBalance - withdrawAmount) < 20)
            {continue;}
        else {
            fiftyNotes = withdrawAmount/50;
            twentyNotes = (withdrawAmount % 50)/20;
            while ((50*fiftyNotes + 20*twentyNotes) != withdrawAmount) {
                fiftyNotes -= 1;
                twentyNotes = (withdrawAmount - 50*fiftyNotes)/20;
            }
            printf("\n**********\nWithdrawal ok.\n**********\n");
            printf("\nAmount %d:", withdrawAmount);
            printf("\n*** Notes 50:   %d\n*** Notes 20:   %d", fiftyNotes, twentyNotes);
            printf("\n\nCheck: (%d * 50) + (%d * 20) = %d.", fiftyNotes, twentyNotes, withdrawAmount);
            *userBalance -= withdrawAmount;
            break;
        }

    } while (withdrawAmount != 0);

    if (withdrawAmount == 0) {
    printf("\n**********\nCancelled.\n**********\n");
    }
    printf("\n***** Balance: %.2lf", *userBalance);
    printf("\n\nPress enter");
}


void depositMoney(double* userBalance) {

    double deposit;

    do {
        printf("\nGive the money to deposit (min. 10.00, max. 1000.00 max. (max. saldo 2000.00)): ");
        deposit = readDouble();

        if (deposit < 10.0 || deposit > 1000.0) {
            continue;
        }
        else if ((*userBalance + deposit) > 2000.0) {
            continue;
        }
        else {
            printf("\n**********\nDeposit ok.\n**********\n");
            *userBalance += deposit;
            break;
        }

    } while (deposit != 0);

    if (deposit == 0) {
        printf("\n**********\nCancelled.\n**********\n");
    }
    printf("\n***** Balance: %.2lf", *userBalance);
    printf("\n\nPress enter");

}

void querryBalance(double userBalance) {

    printf("\n***** Balance: %.2lf", userBalance);
    printf("\n\nPress enter");

}

void displayMenu(void) {
    printf("\nSelect an option...\n");
    printf("1. = Withdrawal\n");
    printf("2. = Deposit\n");
    printf("3. = Query balance\n");
    printf("9. = Quit\n");
    printf("Your choice: ");
}

void pressEnter(void) {
    char ch = getchar();
    if (ch != '\n') {
    clearBuffer();
    }
}

int readInteger(void) {

    int number;
    char character;
    int status;

    while ((status = scanf("%d%c", &number, &character)) == 0 || (2 == status && character != '\n')) {
        clearBuffer();
        printf("\nYou did not enter an integer!\n");
        break;
    }

    return number;
}

double readDouble(void) {

    double number;
    char character;
    int status;

    while ((status = scanf("%lf%c", &number, &character)) == 0 || (2 == status && character != '\n')) {
        clearBuffer();
        printf("\nYou did not enter a real number!\n");
        break;
    }

    return number;
}

void clearBuffer(void) {

    while (fgetc(stdin) != '\n');
}

void readCharacterString(char aString[], int stringSize) {

    fgets(aString, stringSize, stdin);

    if (aString[strlen(aString) - 1] == '\n')
        aString[strlen(aString) - 1] = '\0';
    else
        clearBuffer();
}
