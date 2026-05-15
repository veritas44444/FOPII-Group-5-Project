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
Enjifeto
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
    void addBranch() {
   cout << "\n===== ADD BRANCH =====\n";
  string branchID = generateID("Branch", "BranchID", "B");
  string branchName = getText("Branch Name: ");
  string location = getText("Location: ");
  string phone = getText("Phone: ");
 
  string sql = "INSERT INTO Branch VALUES(?, ?, ?, ?);";
    sqlite3_stmt* stmt = nullptr;
 
  if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
  cerr << "Prepare failed: " << sqlite3_errmsg(db) << endl;
  return;
  }
 
  sqlite3_bind_text(stmt, 1, branchID.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 2, branchName.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 3, location.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 4, phone.c_str(), -1, SQLITE_TRANSIENT);
 
  if (sqlite3_step(stmt) != SQLITE_DONE)
  cerr << "Insert failed: " << sqlite3_errmsg(db) << endl;
else
  cout << "Branch added successfully.\n"
  << "Generated Branch ID: " << branchID << endl;
 
  sqlite3_finalize(stmt);
  }
 
  void displayBranches() {
displayQuery("BRANCH LIST", "SELECT BranchID, BranchName, Location, Phone FROM Branch;");
  }
  
 //Customer
  void addCustomer() {
  cout << "\n===== ADD CUSTOMER =====\n";
  string customerID = generateID("Customer", "CustomerID", "C");
  string fullName = getText("Full Name: ");
  string gender = getText("Gender: ");
  string phone = getText("Phone: ");
  string address = getText("Address: ");
  string registrationDate = getText("Registration Date: ");
  string branchID = getText("Branch ID: ");
string sql = "INSERT INTO Customer VALUES(?, ?, ?, ?, ?, ?, ?);";
  sqlite3_stmt* stmt = nullptr;
   if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
  cerr << "Prepare failed: " << sqlite3_errmsg(db) << endl;
  return;
  }
 
  sqlite3_bind_text(stmt, 1, customerID.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 2, fullName.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 3, gender.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 4, phone.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 5, address.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 6, registrationDate.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 7, branchID.c_str(), -1, SQLITE_TRANSIENT);
 
  if (sqlite3_step(stmt) != SQLITE_DONE)
  cerr << "Insert failed: " << sqlite3_errmsg(db) << endl;
  else
  cout << "Customer added successfully.\n"
  << "Generated Customer ID: " << customerID << endl;
sqlite3_finalize(stmt);
  }
   void displayCustomers() {
  displayQuery("CUSTOMER LIST",
  "SELECT CustomerID, FullName, Gender, Phone, Address, RegistrationDate, BranchID FROM Customer;");
  }
-----------------------------------------------------------------
Elabem
------------------------------------------------------------------

// ===================== EMPLOYEE =====================

void addEmployee() {
    cout << "\n===== ADD EMPLOYEE =====\n";
    string employeeID = generateID("Employee", "EmployeeID", "E");
    string fullName   = getText("Full Name: ");
    string position   = getText("Position: ");
    string email      = getText("Email: ");
    string phone      = getText("Phone: ");
    double salary     = getDouble("Salary: ");
    string branchID   = getText("Branch ID: ");

    string sql = "INSERT INTO Employee VALUES(?, ?, ?, ?, ?, ?, ?);";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "Prepare failed: " << sqlite3_errmsg(db) << endl;
        return;
    }

    sqlite3_bind_text(stmt,   1, employeeID.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt,   2, fullName.c_str(),   -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt,   3, position.c_str(),   -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt,   4, email.c_str(),      -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt,   5, phone.c_str(),      -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 6, salary);
    sqlite3_bind_text(stmt,   7, branchID.c_str(),   -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) != SQLITE_DONE)
        cerr << "Insert failed: " << sqlite3_errmsg(db) << endl;
    else
        cout << "Employee added successfully.\n"
             << "Generated Employee ID: " << employeeID << endl;

    sqlite3_finalize(stmt);
}

void displayEmployees() {
    displayQuery("EMPLOYEE LIST",
        "SELECT EmployeeID, FullName, Position, Email, Phone, Salary, BranchID FROM Employee;");
}

// ===================== ACCOUNT =====================

void addAccount() {
    cout << "\n===== ADD ACCOUNT =====\n";
    string accountNo   = generateID("Account", "AccountNo", "A");
    string customerID  = getText("Customer ID: ");
    string branchID    = getText("Branch ID: ");
    string accountType = getText("Account Type (Savings/Current): ");
    double balance     = getDouble("Initial Balance: ");
    string password    = getText("Password: ");

    string sql = "INSERT INTO Account VALUES(?, ?, ?, ?, ?, ?);";
    sqlite3_stmt* stmt = nullptr;

if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "Prepare failed: " << sqlite3_errmsg(db) << endl;
        return;
    }

    sqlite3_bind_text(stmt,   1, accountNo.c_str(),   -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt,   2, customerID.c_str(),  -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt,   3, branchID.c_str(),    -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt,   4, accountType.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 5, balance);
    sqlite3_bind_text(stmt,   6, password.c_str(),    -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) != SQLITE_DONE)
        cerr << "Insert failed: " << sqlite3_errmsg(db) << endl;
    else
        cout << "Account added successfully.\n"
             << "Generated Account No: " << accountNo << endl;

    sqlite3_finalize(stmt);
}

void displayAccounts() {
    displayQuery("ACCOUNT LIST",
        "SELECT AccountNo, CustomerID, BranchID, AccountType, Balance FROM Account;");
}
    
-----------------------------------------------------------------
void addLoan() {
    cout << "\n===== ADD LOAN =====\n";
    string loanID = generateID("Loan", "LoanID", "L");
    string customerID = getText("Customer ID: ");
    string loanType = getText("Loan Type: ");
    double amount = getDouble("Loan Amount: ");
    string applicationDate = getText("Application Date: ");
    string branchID = getText("Branch ID: ");

    string sql = "INSERT INTO Loan VALUES(?, ?, ?, ?, ?, ?);";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "Prepare failed: " << sqlite3_errmsg(db) << endl;
        return;
    }

    sqlite3_bind_text(stmt, 1, loanID.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, customerID.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, loanType.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 4, amount);
    sqlite3_bind_text(stmt, 5, applicationDate.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 6, branchID.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) != SQLITE_DONE)
        cerr << "Insert failed: " << sqlite3_errmsg(db) << endl;
    else
        cout << "Loan added successfully.\n"
             << "Generated Loan ID: " << loanID << endl;

    sqlite3_finalize(stmt);
}

void displayLoans() {
    displayQuery("LOAN LIST",
                 "SELECT LoanID, CustomerID, LoanType, Amount, ApplicationDate, BranchID FROM Loan;");
}

// ===================== TRANSACTION =====================

void addTransaction() {
    cout << "\n===== ADD TRANSACTION =====\n";
    string transactionID = generateID("Transactions", "TransactionID", "T");
    string accountNo = getText("Account No: ");
    string branchID = getText("Branch ID: ");
    string transactionType = getText("Transaction Type: ");
    double amount = getDouble("Amount: ");
    string transactionDate = getText("Transaction Date: ");

    string sql = "INSERT INTO Transactions VALUES(?, ?, ?, ?, ?, ?);";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "Prepare failed: " << sqlite3_errmsg(db) << endl;
        return;
    }

    sqlite3_bind_text(stmt, 1, transactionID.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, accountNo.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, branchID.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, transactionType.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 5, amount);
    sqlite3_bind_text(stmt, 6, transactionDate.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) != SQLITE_DONE)
        cerr << "Insert failed: " << sqlite3_errmsg(db) << endl;
    else
        cout << "Transaction added successfully.\n"
             << "Generated Transaction ID: " << transactionID << endl;

    sqlite3_finalize(stmt);
}
void displayTransactions() {
    displayQuery("TRANSACTION LIST",
        "SELECT TransactionID, AccountNo, BranchID, TransactionType, Amount, TransactionDate FROM Transactions;");
}
void deposit(const string& accountNo) {
    cout << "\n===== DEPOSIT =====\n";
    double amount = getDouble("Deposit Amount: ");
    string transactionID = generateID("Transactions", "TransactionID", "T");
    string transactionDate = getText("Transaction Date: ");

    string branchID;
    if (!getAccountBranchID(accountNo, branchID)) {
        cout << "Account not found.\n";
        return;
    }

    string updateSQL = "UPDATE Account SET Balance = Balance + ? WHERE AccountNo = ?;";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, updateSQL.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "Prepare failed: " << sqlite3_errmsg(db) << endl;
        return;
    }

    sqlite3_bind_double(stmt, 1, amount);
    sqlite3_bind_text(stmt,   2, accountNo.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        cerr << "Deposit update failed: " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        return;
    }
    sqlite3_finalize(stmt);

    string insertSQL = "INSERT INTO Transactions VALUES(?, ?, ?, ?, ?, ?);";
    if (sqlite3_prepare_v2(db, insertSQL.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "Prepare failed: " << sqlite3_errmsg(db) << endl;
        return;
    }

    sqlite3_bind_text(stmt,   1, transactionID.c_str(),   -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt,   2, accountNo.c_str(),       -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt,   3, branchID.c_str(),        -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt,   4, "Deposit",               -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 5, amount);
    sqlite3_bind_text(stmt,   6, transactionDate.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) != SQLITE_DONE)
        cerr << "Transaction log failed: " << sqlite3_errmsg(db) << endl;
    else
        cout << "Deposit successful.\n"
             << "Generated Transaction ID: " << transactionID << endl;

    sqlite3_finalize(stmt);
}

void withdraw(const string& accountNo) {
    cout << "\n===== WITHDRAW =====\n";
    double amount = getDouble("Withdrawal Amount: ");
    string transactionID = generateID("Transactions", "TransactionID", "T");
    string transactionDate = getText("Transaction Date: ");

    string branchID;
    if (!getAccountBranchID(accountNo, branchID)) {
        cout << "Account not found.\n";
        return;
    }

    string updateSQL = "UPDATE Account SET Balance = Balance - ? WHERE AccountNo = ? AND Balance >= ?;";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, updateSQL.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "Prepare failed: " << sqlite3_errmsg(db) << endl;
        return;
    }

    sqlite3_bind_double(stmt, 1, amount);
    sqlite3_bind_text(stmt,   2, accountNo.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 3, amount);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        cerr << "Withdraw update failed: " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        return;
    }

    int changed = sqlite3_changes(db);
    sqlite3_finalize(stmt);

    if (changed == 0) {
        cout << "Insufficient balance.\n";
        return;
    }

    string insertSQL = "INSERT INTO Transactions VALUES(?, ?, ?, ?, ?, ?);";
    if (sqlite3_prepare_v2(db, insertSQL.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "Prepare failed: " << sqlite3_errmsg(db) << endl;
        return;
    }

    sqlite3_bind_text(stmt,   1, transactionID.c_str(),   -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt,   2, accountNo.c_str(),       -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt,   3, branchID.c_str(),        -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt,   4, "Withdrawal",            -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 5, amount);
    sqlite3_bind_text(stmt,   6, transactionDate.c_str(), -1, SQLITE_TRANSIENT);
    if (sqlite3_step(stmt) != SQLITE_DONE)
        cerr << "Transaction log failed: " << sqlite3_errmsg(db) << endl;
    else
        cout << "Withdrawal successful.\n"
             << "Generated Transaction ID: " << transactionID << endl;

    sqlite3_finalize(stmt);
}

void viewMyAccount(const string& accountNo) {
    string sql = "SELECT AccountNo, CustomerID, BranchID, AccountType, Balance "
                 "FROM Account WHERE AccountNo = '" + accountNo + "';";
    displayQuery("MY ACCOUNT DETAILS", sql);
}

void viewMyTransactions(const string& accountNo) {
    string sql = "SELECT TransactionID, TransactionType, Amount, TransactionDate, BranchID "
                 "FROM Transactions WHERE AccountNo = '" + accountNo + "';";
    displayQuery("MY TRANSACTION HISTORY", sql);
}
-----------------------------------------------------------------
Enjifeto
------------------------------------------------------------------
void userMenu(const string& accountNo) {
    int choice;
    do {
        cout << "\n****************************************\n";
        cout << "        USER PANEL  [Acc: " << accountNo << "]\n";
        cout << "******************************************\n";
        cout << "1.  View My Account Details\n";
        cout << "2.  View My Transaction History\n";
        cout << "3.  Deposit\n";
        cout << "4.  Withdraw\n";
        cout << "5.  Logout\n";
        cout << "*******************************\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: viewMyAccount(accountNo); break;
            case 2: viewMyTransactions(accountNo); break;
            case 3: deposit(accountNo); break;
            case 4: withdraw(accountNo); break;
            case 5: cout << "Logging out...\n"; break;
            default: cout << "Invalid choice.\n";
        }

    } while (choice != 5);
}
-----------------------------------------------------------------
Enjifeto
------------------------------------------------------------------
