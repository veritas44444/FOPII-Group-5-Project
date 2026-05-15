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
