#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int choice;
int nextID = 1;
char line[10000];
char password[12] = "thisisadmin";

struct userData{
    char loanID[100000];
    char borrowName[100000];
    char approvalDate[100000];
    int loanAmount;
};

int readFile(const char *loanDataFile){
    FILE *file = fopen(loanDataFile, "r");
    if (file == NULL) return 0;
    int count = 0;

    while (fgets(line, sizeof(line), file)) {
        if (strlen(line) > 1) count++;
    }

    fclose(file);
    return count;
}

void addData(){
    struct userData user;
    char confirmStatus;
    int day, month, year, valid;
    FILE *file = fopen("loan_data.csv", "a");
    if(file == NULL) printf("File cannot be opened");

    nextID = readFile("loan_data.csv") + 1;
    sprintf(user.loanID, "L%03d", nextID);
    
    printf("Add data\n");

    printf("Enter borrower name : ");
    scanf(" %[^\n]", user.borrowName);
    printf("Please enter loan amount : ");
    scanf("%d", &user.loanAmount);
    do {
        printf("Enter approval date (dd/mm/yyyy): ");
        scanf(" %10s", user.approvalDate);
        valid = sscanf(user.approvalDate, "%2d/%2d/%4d", &day, &month, &year);
        if (valid != 3 || strlen(user.approvalDate) != 10 ||
            day < 1 || day > 31 || month < 1 || month > 12 || year < 1900) {
            printf("Invalid format! Please enter again.\n");
            valid = 0;
        }
    } while (!valid);
    printf("Confirm data? (y/n) : ");
    scanf(" %c", &confirmStatus);

    if(confirmStatus == 'y'){
        fprintf(file, "%s,%s,%d,%s\n",
            user.loanID, user.borrowName, user.loanAmount, user.approvalDate);
        fclose(file);
        printf("Data saved %s\n", user.loanID);
    }else{
        printf("Data not save.");
        addData();
    }
}

void searchData(){
    char search[100000];
    int found = 0, count = 0;
    FILE *file = fopen("loan_data.csv", "r");
    printf("Search data: ");
    scanf("%s", search);

    while(fgets(line, sizeof(line), file) != NULL){
        if(strstr(line, search)){
            if(count < 1){
                printf("+----------+------------+-------------+----------------+\n");
                printf("| Loan ID  | Borrower   | Loan Amount | Approval Date  |\n");
                printf("+----------+------------+-------------+----------------+\n");
            }
            
            char *loanID = strtok(line, ",");
            char *borrowName = strtok(NULL, ",");
            char *loanAmount = strtok(NULL, ",");
            char *approvalDate = strtok(NULL, "\n");

            printf("| %-8s | %-10s | %-11s | %-14s |\n", loanID, borrowName, loanAmount, approvalDate);
            found = 1;   
            printf("+----------+------------+-------------+----------------+\n");
        }
        count++;
    }

    if(found != 1) printf("Data is not found.\n");
    fclose(file);
}

void updateData(){
    int countLine = 0;
    char line[1024];
    char validatePass[12];
    int choiceUpdate;
    char selectLoanID[1000];
    char newValue[1000];
    int found;

    FILE *file = fopen("loan_data.csv", "r");

    printf("Please enter password for admin to update data: ");
    scanf("%s", validatePass);
    if (strcmp(validatePass, password) == 0) {
        printf("Welcome admin!\n");
    }else{
        printf("You do not have permission to update files.\n");
        return;
    }

    printf("Update data\n");
    while (fgets(line, sizeof(line), file) != NULL) {
        if (countLine < 1) {
            printf("+----------+------------+-------------+----------------+\n");
            printf("| Loan ID  | Borrower   | Loan Amount | Approval Date  |\n");
            printf("+----------+------------+-------------+----------------+\n");
        }

        char *loanID = strtok(line, ",");
        char *borrowName = strtok(NULL, ",");
        char *loanAmount = strtok(NULL, ",");
        char *approvalDate = strtok(NULL, "\n");

        printf("| %-8s | %-10s | %-11s | %-14s |\n", loanID, borrowName, loanAmount, approvalDate);
        printf("+----------+------------+-------------+----------------+\n");
        countLine++;
    }
    fclose(file);

    do {
        printf("Please enter Loan ID you want to update (q to quit to main menu): ");
        scanf("%s", selectLoanID);
        if(strcmp(selectLoanID, "q") == 0) break;

        printf("Please select element you want to update.\n");
        printf("1. Borrower\n");
        printf("2. Loan Amount\n");
        printf("3. Approval date\n");
        printf("4. Quit to main menu\n");
        printf("Select: ");
        scanf("%d", &choiceUpdate);

        if (choiceUpdate == 4) continue;

        printf("Enter new value: ");
        scanf(" %[^\n]", newValue);

        FILE *file = fopen("loan_data.csv", "r");
        FILE *temp = fopen("temp.csv", "w");

        found = 0;
        while (fgets(line, sizeof(line), file)) {
            char id[100], name[100], date[100];
            int amount;

            sscanf(line, "%[^,],%[^,],%d,%s", id, name, &amount, date);

            if (strcmp(id, selectLoanID) == 0) {
                found = 1;
                if (choiceUpdate == 1)
                    strcpy(name, newValue);
                else if (choiceUpdate == 2)
                    amount = atoi(newValue);
                else if (choiceUpdate == 3)
                    strcpy(date, newValue);
            }

            fprintf(temp, "%s,%s,%d,%s\n", id, name, amount, date);
        }

        fclose(file);
        fclose(temp);

        if (found) {
            remove("loan_data.csv");
            rename("temp.csv", "loan_data.csv");
            printf("Data updated successfully!\n");
        } else {
            remove("temp.csv");
            printf("Loan ID not found. Please try again.\n");
        }

    }while(1);
}

void deleteData(){
    int countLine = 0;
    char validatePass[12];
    FILE *file = fopen("loan_data.csv", "r");
    FILE *temp = fopen("temp.csv", "w");

    printf("Please enter password for admin to delete data: ");
    scanf("%s", validatePass);
    if (strcmp(validatePass, password) == 0) {
        printf("Welcome admin!\n");
    }else{
        printf("You do not have permission to delete files.\n");
        return;
    }

    printf("Delete data\n");
    while (fgets(line, sizeof(line), file) != NULL) {
        if (countLine < 1) {
            printf("+----------+------------+-------------+----------------+\n");
            printf("| Loan ID  | Borrower   | Loan Amount | Approval Date  |\n");
            printf("+----------+------------+-------------+----------------+\n");
        }

        char *loanID = strtok(line, ",");
        char *borrowName = strtok(NULL, ",");
        char *loanAmount = strtok(NULL, ",");
        char *approvalDate = strtok(NULL, "\n");

        printf("| %-8s | %-10s | %-11s | %-14s |\n", loanID, borrowName, loanAmount, approvalDate);
        printf("+----------+------------+-------------+----------------+\n");
        countLine++;
    }
    fclose(file);
    
}

int main(){
    do{
        printf("-------------------------------\n");
        printf("Select the menu\n");
        printf("1. Add data\n");
        printf("2. Search data\n");
        printf("3. Update data\n");
        printf("4. Delete data\n");
        printf("5. End program\n");
        printf("-------------------------------\n");
        scanf("%d", &choice);

        switch (choice){
        case 1:
        addData();
            break;
        
        case 2:
        searchData();
            break;

        case 3:
        updateData();
            break;

        case 4:
        deleteData();
            break;
            
        default:
            break;
        }
    }while(choice != 5);


    return 0;
}