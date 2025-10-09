Loan ID Management - How to Compile and Run
===========================================

1. Clone or download the project:
   --------------------------------
   git clone https://github.com/mandyint/ProgrammingFundamentalCRUD.git
   cd ProgrammingFundamentalCRUD

2. Compile the program using GCC (GNU Compiler):
   ----------------------------------------------
   Main program :
   gcc -o loan_management loan_management.c

   - This will generate an executable named "loan_management".
   - Make sure GCC is installed on your system. On Linux/Mac:
       sudo apt install build-essential   # Ubuntu/Debian
       brew install gcc                   # MacOS with Homebrew

3. Run the program:
   ----------------
   Main program :
   ./loan_management       # Linux/Mac
   loan_management.exe     # Windows (if compiled with MinGW)



4. Features:
   ---------
   1. Add new loan record
   2. Search loan by borrower name or Loan ID
   3. Update an existing record (admin password required)
   4. Delete a record (soft delete, admin password required)
   5. Recover a deleted record (admin password required)
   6. Run unit test: add loan data
   7. Run unit test: search loan data
   8. Run End-to-End (E2E) test
   9. Exit program

5. Notes:
   --------
   - Data is saved in 'loan_data.csv'.
   - Make sure you have read/write permission in the project folder.
   - Admin password is required for update/delete/recover operations (default password: "thisisadmin").
   - To compile e2etest.c and unitTest.c, do the same method with main file. Compile then execute.