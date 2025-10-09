#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

void testE2E() {
    printf("==================== RUNNING E2E TEST ====================\n");

    const char *input_data =
        "thisisdian\n"
        "1243\n"
        "11/11/1901\n"
        "y\n"
        "thisisdian\n"
        "thisisadmin\n"
        "L001\n"
        "2\n"
        "7500\n"
        "q\n"
        "thisisadmin\n"
        "1\n"
        "L001\n"
        "y\n"
        "1\n"
        "2\n"
        "L001\n"
        "y\n"
        "1\n"
        "L001\n"
        "y\n"
        "3\n"
        "y\n"
        "4\n"
        "8\n";

    FILE *file = fopen("loan_data.csv", "a");
    if(file) fclose(file);

    FILE *input = fopen("test_input.txt", "a");
    fputs(input_data, input);
    fclose(input);

    FILE *saved_stdin = stdin;
    freopen("test_input.txt", "r", stdin);

    addData();
    searchData();
    updateData();
    deleteData();

    stdin = saved_stdin;

    remove("test_input.txt");

    printf("✅ E2E TEST COMPLETED SUCCESSFULLY!\n");
    printf("============================================================\n");
}
