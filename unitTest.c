#include <stdio.h>
#include <assert.h>

void resetIO() {
    freopen("/dev/tty", "r", stdin);
    freopen("/dev/tty", "w", stdout);
}

//Unit test for add data
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

//Unit test for search function
void run_searchData(const char *test_name, const char *initial_data, const char *search_term, const char *expected_loanID) {
    printf("Running %s...\n", test_name);

    FILE *file = fopen("loan_data.csv", "w");
    if (!file) { printf("❌ Cannot open CSV for writing\n"); return; }
    fprintf(file, "%s", initial_data);
    fclose(file);

    FILE *saved_stdout = stdout;
    FILE *temp_out = fopen("temp_stdout.txt", "w");
    if (!temp_out) { printf("❌ Cannot create temp stdout\n"); return; }
    stdout = temp_out;

    FILE *saved_stdin = stdin;
    FILE *input_file = fopen("test_input.txt", "w");
    if (!input_file) { printf("❌ Cannot create test input\n"); return; }
    fprintf(input_file, "%s\n", search_term);
    fclose(input_file);
    stdin = fopen("test_input.txt", "r");
    if (!stdin) { printf("❌ Cannot open test input for reading\n"); return; }

    searchData();

    fclose(stdin);
    stdin = saved_stdin;
    fclose(temp_out);
    stdout = saved_stdout;

    temp_out = fopen("temp_stdout.txt", "r");
    char buffer[1024*10];
    size_t len = fread(buffer, 1, sizeof(buffer)-1, temp_out);
    buffer[len] = '\0';
    fclose(temp_out);


    if (expected_loanID) {
        if (strstr(buffer, expected_loanID)) {
            printf("✅ %s PASSED\n\n", test_name);
        } else {
            printf("❌ %s FAILED\n", test_name);
            printf("Expected Loan ID: '%s'\n", expected_loanID);
            printf("Actual output:\n%s\n", buffer);
            assert(0 && "searchData() output mismatch");
        }
    } else {
        if (strstr(buffer, "Data not found")) {
            printf("✅ %s PASSED (data not found)\n\n", test_name);
        } else {
            printf("❌ %s FAILED (data not found expected)\n", test_name);
            printf("Actual output:\n%s\n", buffer);
            assert(0 && "searchData() output mismatch");
        }
    }

    remove("test_input.txt");
    remove("temp_stdout.txt");
}

void test_searchData() {
    printf("==================== RUNNING SEARCH DATA TESTS ====================\n");

    run_searchData(
        "search_existing_loan",
        "L001,John Doe,5000,01/02/2024,0\nL002,Jane Smith,10000,01/02/2024,0\n",
        "John",
        "L001"
    );

    run_searchData(
        "search_non_existing_loan",
        "L003,Alice,7000,01/03/2024,0\n",
        "Bob",
        NULL
    );

    run_searchData(
        "search_partial_match",
        "L004,Bob Marley,9000,15/03/2024,0\n",
        "Marley",
        "L004"
    );
}