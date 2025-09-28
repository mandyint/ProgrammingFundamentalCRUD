#include <stdio.h>
#include <string.h>

int choice;
char search[100000], line[100000];

struct userData{
    char loanID[1000];
    char borrowName[1000];
    char approvalDate[1000];
    int loanAmount;
};

int isDuplicated(const char *loanID) {
    FILE *file = fopen("loan_data.csv", "r");
    if (file == NULL) return 0;
    while (fgets(line, sizeof(line), file)) {
        char existingLoanID[1000];
        sscanf(line, "%[^,]", existingLoanID);
        if (strcmp(existingLoanID, loanID) == 0) {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

void addData(){
    struct userData user;
    char status;
    printf("Add data\n");
    printf("Loan ID: ");
    scanf("%s", user.loanID);
            
    printf("Borrow name: ");
    scanf("%s", user.borrowName);

    printf("Loan amount: ");
    scanf("%d", &user.loanAmount);

    printf("Approval date (d/m/y): ");
    scanf("%s", user.approvalDate);

    FILE *file = fopen("loan_data.csv", "a");
    if(file == NULL) printf("Failed to open the file.");

    printf("Confirm data? (y/n): ");
    scanf(" %c", &status);
    if (status == 'y') {
        if (isDuplicated(user.loanID)) {
            printf("⚠️ Error: Loan ID already exists! Data not saved.\n");
            addData();
        }

        fprintf(file, "%s,%s,%d,%s\n",
                user.loanID, user.borrowName,
                user.loanAmount, user.approvalDate);
        fclose(file);
        printf("✅ Data saved to loan_data.csv\n");
    } else {
        addData();
    }
}

void searchData(){
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
    int count = 0, i = 1;
    char updateStatus;
    FILE *file = fopen("loan_data.csv", "r");
    int num;

    printf("Update data");
    while(fgets(line, sizeof(line), file) != NULL){
        if(count < 1){
            printf("+----------+------------+-------------+----------------+\n");
            printf("| Loan ID  | Borrower   | Loan Amount | Approval Date  |\n");
            printf("+----------+------------+-------------+----------------+\n");
        }
            
        char *loanID = strtok(line, ",");
        char *borrowName = strtok(NULL, ",");
        char *loanAmount = strtok(NULL, ",");
        char *approvalDate = strtok(NULL, "\n");

        printf("| %-8s | %-10s | %-11s | %-14s | %-3d |\n", loanID, borrowName, loanAmount, approvalDate, i);
        printf("+----------+------------+-------------+----------------+\n");
        count++;
        i++;
    }
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
        default:
            break;
        }
    }while(choice != 5);


    return 0;
}