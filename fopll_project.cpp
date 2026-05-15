#include <iostream>
#include <iomanip>
#include <string>
#include <limits>
#include <cstdlib>
#include "sqlite3.h"

using namespace std;

sqlite3* db = nullptr;

//helper functions

bool executeSQL(const string& sql) {
    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        cerr << "SQL Error: " << errMsg << endl;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

void pause() {
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void openDatabase() {
    if (sqlite3_open("bank.db", &db) != SQLITE_OK) {
        cerr << "Cannot open database: " << sqlite3_errmsg(db) << endl;
        exit(1);
    }
    executeSQL("PRAGMA foreign_keys = ON;");
}
-----------------------------------------------------------------
    -----> elsa
------------------------------------------------------------------
void createTables() {
    string sql;

    sql = R"(
        CREATE TABLE IF NOT EXISTS Branch(
            BranchID TEXT PRIMARY KEY,
            BranchName TEXT NOT NULL,
            Location TEXT NOT NULL,
            Phone TEXT NOT NULL
        );
    )";
    executeSQL(sql);

    sql = R"(
        CREATE TABLE IF NOT EXISTS Customer(
            CustomerID TEXT PRIMARY KEY,
            FullName TEXT NOT NULL,
            Gender TEXT NOT NULL,
            Phone TEXT NOT NULL,
            Address TEXT NOT NULL,
            RegistrationDate TEXT NOT NULL,
            BranchID TEXT NOT NULL,
            FOREIGN KEY (BranchID) REFERENCES Branch(BranchID)
        );
    )";
    executeSQL(sql);

    sql = R"(
        CREATE TABLE IF NOT EXISTS Employee(
            EmployeeID TEXT PRIMARY KEY,
            FullName TEXT NOT NULL,
            Position TEXT NOT NULL,
            Email TEXT NOT NULL,
            Phone TEXT NOT NULL,
            Salary REAL NOT NULL,
            BranchID TEXT NOT NULL,
            FOREIGN KEY (BranchID) REFERENCES Branch(BranchID)
        );
    )";
    executeSQL(sql);

    sql = R"(
        CREATE TABLE IF NOT EXISTS Account(
            AccountNo TEXT PRIMARY KEY,
            CustomerID TEXT NOT NULL,
            BranchID TEXT NOT NULL,
            AccountType TEXT NOT NULL,
            Balance REAL NOT NULL,
            Password TEXT NOT NULL,
            FOREIGN KEY (CustomerID) REFERENCES Customer(CustomerID),
            FOREIGN KEY (BranchID) REFERENCES Branch(BranchID)
        );
    )";
    executeSQL(sql);

    sql = R"(
        CREATE TABLE IF NOT EXISTS Loan(
            LoanID TEXT PRIMARY KEY,
            CustomerID TEXT NOT NULL,
            LoanType TEXT NOT NULL,
            Amount REAL NOT NULL,
            ApplicationDate TEXT NOT NULL,
            BranchID TEXT NOT NULL,
            FOREIGN KEY (CustomerID) REFERENCES Customer(CustomerID),
            FOREIGN KEY (BranchID) REFERENCES Branch(BranchID)
        );
    )";
    executeSQL(sql);

    sql = R"(
        CREATE TABLE IF NOT EXISTS Transactions(
            TransactionID TEXT PRIMARY KEY,
            AccountNo TEXT NOT NULL,
            BranchID TEXT NOT NULL,
            TransactionType TEXT NOT NULL,
            Amount REAL NOT NULL,
            TransactionDate TEXT NOT NULL,
            FOREIGN KEY (AccountNo) REFERENCES Account(AccountNo),
            FOREIGN KEY (BranchID) REFERENCES Branch(BranchID)
        );
    )";
    executeSQL(sql);

    if (!columnExists("Account", "Password")) {
        executeSQL("ALTER TABLE Account ADD COLUMN Password TEXT;");
    }
}

void displayQuery(const string& title, const string& query) {
    cout << "\n========== " << title << " ==========\n";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "Query Error: " << sqlite3_errmsg(db) << endl;
        return;
    }

    int cols = sqlite3_column_count(stmt);

    for (int i = 0; i < cols; i++)
        cout << left << setw(18) << sqlite3_column_name(stmt, i);
    cout << "\n";
    for (int i = 0; i < cols; i++) cout << string(18, '-');
    cout << "\n";

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        for (int i = 0; i < cols; i++) {
            const unsigned char* text = sqlite3_column_text(stmt, i);
            cout << left << setw(18) << (text ? reinterpret_cast<const char*>(text) : "NULL");
        }
        cout << "\n";
    }

    sqlite3_finalize(stmt);
}

string getText(const string& prompt) {
    string value;
    cout << prompt;
    getline(cin >> ws, value);
    return value;
}

double getDouble(const string& prompt) {
    double value;
    cout << prompt;
    cin >> value;
    return value;
}
bool getAccountBranchID(const string& accountNo, string& branchID) {
    sqlite3_stmt* stmt = nullptr;
    string sql = "SELECT BranchID FROM Account WHERE AccountNo = ?;";

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
        return false;

    sqlite3_bind_text(stmt, 1, accountNo.c_str(), -1, SQLITE_TRANSIENT);

    bool found = false;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        branchID = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        found = true;
    }

    sqlite3_finalize(stmt);
    return found;
}
//Branch
