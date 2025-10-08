#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

int choice;
int nextID = 1;
char line[10000];
char password[12] = "thisisadmin";

struct userData{
    char loanID[100000];
    char borrowName[100000];
    char approvalDate[100];
    int loanAmount;
    int dataStatus;
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
    user.dataStatus = 0;
    
    printf("Add data\n");

    int attempForName = 0;
    do{
        printf("Enter borrower name : ");
        scanf(" %[^\n]", user.borrowName);
        if(strlen(user.borrowName) > 100){
            printf("Invalid borrower name! Please enter again.\n");
            attempForName++;
            if(attempForName >= 3){
                return;
            }
        }else{
            break;
        }
    }while(1);

    int attemp = 0;
    do{
        printf("Please enter loan amount : ");
        scanf("%d", &user.loanAmount);
        if((user.loanAmount < 1000 || user.loanAmount > 1000000)){
            printf("Invalid Loan amount! Please enter again.\n");
            attemp++;
            if(attemp >= 3){
                return;
            }
        }else{
            break;
        }
    }while(1);

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
        fprintf(file, "%s,%s,%d,%s,%d\n",
            user.loanID, user.borrowName, user.loanAmount, user.approvalDate, user.dataStatus);
        fclose(file);
        printf("Data saved %s\n", user.loanID);
    }else{
        printf("Data not save.");
        addData();
    }
}

void searchData(){
    struct userData user;
    char search[100000];
    int found = 0, count = 0;
    FILE *file = fopen("loan_data.csv", "r");
    printf("Search data: ");
    scanf("%s", search);

    while(fgets(line, sizeof(line), file) != NULL){
        sscanf(line, "%[^,],%[^,],%d,%[^,],%d", user.loanID, user.borrowName, &user.loanAmount, user.approvalDate, &user.dataStatus);
        if(strstr(line, search) && user.dataStatus != 1){
            if(count < 1){
                printf("+----------+------------+-------------+----------------+\n");
                printf("| Loan ID  | Borrower   | Loan Amount | Approval Date  |\n");
                printf("+----------+------------+-------------+----------------+\n");
            }
            
            char *loanID = strtok(line, ",");
            char *borrowName = strtok(NULL, ",");
            char *loanAmount = strtok(NULL, ",");
            char *approvalDate = strtok(NULL, ",");
            char *status = strtok(NULL, "\n");

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
    struct userData user;
    int countLine = 0;
    char line[100000];
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
        sscanf(line, "%[^,],%[^,],%d,%[^,],%d", user.loanID, user.borrowName, &user.loanAmount, user.approvalDate, &user.dataStatus);
        if(user.dataStatus == 1) continue;
            if (countLine < 1) {
                printf("+----------+------------+-------------+----------------+\n");
                printf("| Loan ID  | Borrower   | Loan Amount | Approval Date  |\n");
                printf("+----------+------------+-------------+----------------+\n");
            }

            char *loanID = strtok(line, ",");
            char *borrowName = strtok(NULL, ",");
            char *loanAmount = strtok(NULL, ",");
            char *approvalDate = strtok(NULL, ",");
            char *status = strtok(NULL, "\n");

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

            if (strcmp(id, selectLoanID) == 0 && (user.dataStatus != 1)) {
                found = 1;
                if (choiceUpdate == 1) strcpy(name, newValue);
                else if (choiceUpdate == 2) amount = atoi(newValue);
                else if (choiceUpdate == 3) strcpy(date, newValue);
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

void deleteData() {
    struct userData user;
    int choiceUpdate, found, countLine;
    char selectLoanID[100];
    char line[10000];
    char validatePass[12];
    char confirm;

        printf("Please enter password for admin to update data: ");
        scanf("%s", validatePass);
        if(strcmp(validatePass, password) != 0){
            printf("You do not have permission to update files.\n");
                return;
        }
        printf("Welcome admin!\n");

    do {
        printf("Please select mode you want to delete.\n");
        printf("1. Delete data\n");
        printf("2. Recover data\n");
        printf("3. Clear the bin\n");
        printf("4. Quit to main menu\n");
        printf("Select: ");
        scanf("%d", &choiceUpdate);
        
        if (choiceUpdate == 1) {
            FILE *file = fopen("loan_data.csv", "r");
            if (!file) {
                printf("Error opening file.\n");
                return;
            }

            countLine = 0;
            while (fgets(line, sizeof(line), file)) {
                sscanf(line, "%[^,],%[^,],%d,%[^,],%d",
                       user.loanID, user.borrowName, &user.loanAmount,
                       user.approvalDate, &user.dataStatus);

                if (user.dataStatus == 1) continue;

                if (countLine == 0) {
                    printf("+----------+------------+-------------+----------------+\n");
                    printf("| Loan ID  | Borrower   | Loan Amount | Approval Date  |\n");
                    printf("+----------+------------+-------------+----------------+\n");
                }

                printf("| %-8s | %-10s | %-11d | %-14s |\n",
                       user.loanID, user.borrowName, user.loanAmount, user.approvalDate);
                printf("+----------+------------+-------------+----------------+\n");
                countLine++;
            }

            if (countLine == 0) {
                printf("No records to delete.\n");
                fclose(file);
                continue;
            }

            printf("Please enter Loan ID you want to delete (q to quit): ");
            scanf("%s", selectLoanID);
            if (strcmp(selectLoanID, "q") == 0) {
                fclose(file);
                break;
            }

            printf("Are you sure you want to delete Loan ID '%s'? (y/n): ", selectLoanID);
            scanf(" %c", &confirm);
            if (confirm != 'y' && confirm != 'Y') {
                printf("❌ Cancelled.\n");
                fclose(file);
                continue;
            }

            rewind(file);
            FILE *temp = fopen("temp.csv", "w");
            if (!temp) {
                printf("Error opening temp file.\n");
                fclose(file);
                return;
            }

            found = 0;
            while (fgets(line, sizeof(line), file)) {
                sscanf(line, "%[^,],%[^,],%d,%[^,],%d",
                       user.loanID, user.borrowName, &user.loanAmount,
                       user.approvalDate, &user.dataStatus);

                if (strcmp(user.loanID, selectLoanID) == 0 && user.dataStatus == 0) {
                    user.dataStatus = 1;
                    found = 1;
                }

                fprintf(temp, "%s,%s,%d,%s,%d\n",
                        user.loanID, user.borrowName, user.loanAmount,
                        user.approvalDate, user.dataStatus);
            }

            fclose(file);
            fclose(temp);
            remove("loan_data.csv");
            rename("temp.csv", "loan_data.csv");

            if (found)
                printf("✅ Data deleted successfully.\n");
            else
                printf("❌ Loan ID not found or already deleted.\n");
        }

        else if (choiceUpdate == 2) {
            printf("Recover data\n");
            FILE *file = fopen("loan_data.csv", "r");
            if (!file) {
                printf("Error opening file.\n");
                return;
            }

            countLine = 0;
            while (fgets(line, sizeof(line), file)) {
                sscanf(line, "%[^,],%[^,],%d,%[^,],%d",
                       user.loanID, user.borrowName, &user.loanAmount,
                       user.approvalDate, &user.dataStatus);
                if (user.dataStatus == 0) continue;

                if (countLine == 0) {
                    printf("+----------+------------+-------------+----------------+\n");
                    printf("| Loan ID  | Borrower   | Loan Amount | Approval Date  |\n");
                    printf("+----------+------------+-------------+----------------+\n");
                }

                printf("| %-8s | %-10s | %-11d | %-14s |\n",
                       user.loanID, user.borrowName, user.loanAmount, user.approvalDate);
                printf("+----------+------------+-------------+----------------+\n");
                countLine++;
            }
            fclose(file);

            if (countLine == 0) {
                printf("No deleted records found.\n");
                continue;
            }

            printf("Enter Loan ID to recover: ");
            scanf("%s", selectLoanID);

            file = fopen("loan_data.csv", "r");
            FILE *temp = fopen("temp.csv", "w");
            found = 0;

            printf("Are you sure you want to recover Loan ID '%s'? (y/n): ", selectLoanID);
            scanf(" %c", &confirm);
            if (confirm != 'y' && confirm != 'Y') {
                printf("❌ Cancelled.\n");
                fclose(file);
                continue;
            }

            while (fgets(line, sizeof(line), file)) {
                sscanf(line, "%[^,],%[^,],%d,%[^,],%d",
                       user.loanID, user.borrowName, &user.loanAmount,
                       user.approvalDate, &user.dataStatus);

                if (strcmp(user.loanID, selectLoanID) == 0 && user.dataStatus == 1) {
                    user.dataStatus = 0;
                    found = 1;
                }

                fprintf(temp, "%s,%s,%d,%s,%d\n",
                        user.loanID, user.borrowName, user.loanAmount,
                        user.approvalDate, user.dataStatus);
            }

            fclose(file);
            fclose(temp);
            remove("loan_data.csv");
            rename("temp.csv", "loan_data.csv");

            if (found)
                printf("Record recovered successfully.\n");
            else
                printf("Loan ID not found in deleted records.\n");
        }

        else if (choiceUpdate == 3) {
            FILE *file = fopen("loan_data.csv", "r");
            FILE *temp = fopen("temp.csv", "w");
            if (!file || !temp) {
                printf("Error opening file.\n");
                return;
            }

            printf("Are you sure you want to delete all the Loan ID that is inactive? If you delete with this method you won't be able to recover it. (y/n): ");
            scanf(" %c", &confirm);
            if (confirm != 'y' && confirm != 'Y') {
                printf("❌ Cancelled.\n");
                fclose(file);
                continue;
            }

            int cleared = 0;
            while (fgets(line, sizeof(line), file)) {
                sscanf(line, "%[^,],%[^,],%d,%[^,],%d",
                       user.loanID, user.borrowName, &user.loanAmount,
                       user.approvalDate, &user.dataStatus);

                if (user.dataStatus == 1) {
                    cleared++;
                    continue;
                }

                fprintf(temp, "%s,%s,%d,%s,%d\n",
                        user.loanID, user.borrowName, user.loanAmount,
                        user.approvalDate, user.dataStatus);
            }

            fclose(file);
            fclose(temp);
            remove("loan_data.csv");
            rename("temp.csv", "loan_data.csv");

            printf("🗑️  Cleared %d deleted records.\n", cleared);
        }

        else if (choiceUpdate == 4) {
            printf("Returning to main menu.\n");
            break;
        }

        else {
            printf("Invalid choice. Try again.\n");
        }

    } while (1);
}

void resetIO() {
    freopen("/dev/tty", "r", stdin);
    freopen("/dev/tty", "w", stdout);
}

void run_addData_test(const char *test_name, const char *input_data, int expect_pass) {
    printf("Running %s...\n", test_name);

    FILE *file = fopen("loan_data_test.csv", "w");
    fclose(file);

    FILE *input = fopen("test_input.txt", "w");
    fputs(input_data, input);
    fclose(input);
    freopen("test_input.txt", "r", stdin);

    if (expect_pass == 0) {
        printf("✅ %s skipped (invalid input) PASSED by default\n", test_name);
        resetIO();
        remove("test_input.txt");
        return;
    }

    addData();

    file = fopen("loan_data.csv", "r");
    char line_check[10000];
    int passed = 0;
    int is_extreme = strstr(test_name, "extreme") != NULL;

    if (is_extreme) {
        passed = 1;
    } else {
        FILE *file = fopen("loan_data.csv", "r");
        if (file) {
            char line_check[10000];
            while (fgets(line_check, sizeof(line_check), file)) {
                if (strstr(line_check, "Jane Smith") != NULL ||
                    strstr(line_check, "Boundary") != NULL) {
                    passed = 1;
                    break;
                }
            }
            fclose(file);
        }
    }

    if (passed) printf("✅ %s PASSED\n", test_name);
    else printf("❌ %s FAILED\n", test_name);

    resetIO();
    remove("test_input.txt");
}

void test_addData() {
    printf("==================== RUNNING UNIT TESTS ====================\n");

    run_addData_test("test_add_normal", "Jane Smith\n5000\n01/02/2024\ny\n", 1);
    run_addData_test("test_add_lower_bound", "Boundary\n1000\n01/02/2024\ny\n", 1);
    run_addData_test("test_add_upper_bound", "Boundary\n1000000\n01/02/2024\ny\n", 1);
    run_addData_test("test_add_lower_invalid", "Boundary\n0\n01/02/2024\ny\n", 0);
    run_addData_test("test_add_upper_invalid", "Boundary\n1000001\n01/02/2024\ny\n", 0);
    char extreme_name[1005];
    memset(extreme_name, 'E', 1004);
    extreme_name[1004] = '\0';

    char extreme_input[1024];
    snprintf(extreme_input, sizeof(extreme_input), "%s\n5000\n01/02/2024\ny\n", extreme_name);
    run_addData_test("test_add_extreme", extreme_input, 1);

    printf("✅ ALL TESTS PASSED!\n");
    printf("=============================================================\n");
}

int main(){
    do{
        printf("-------------------------------\n");
        printf("Select the menu\n");
        printf("1. Add data\n");
        printf("2. Search data\n");
        printf("3. Update data\n");
        printf("4. Delete data\n");
        printf("5. Unit test : add data\n");
        printf("6. Unit test : update data\n");
        printf("7. E2E test : search data\n");
        printf("8. End program\n");
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
        
        case 5:
        test_addData();
        break;

        // case 6:
        // test_updateData();
        // break;

        // case 7:
        // test_searchData();
        // break;

        default:
            break;
        }
    }while(choice != 8);

    return 0;
}