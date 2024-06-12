#include <iostream>
#include <stdexcept>
#include <string>
#include <cstdlib>
#include <sqlite3.h>
using std::string;
using std::cin;
using std::cout;
using std::endl;
using std::runtime_error;

void passwordChecker(string &password);

string hashFunction(const string &password);

int main() {
   
    string username;
    string password;
    cout << "Please Enter Username" << endl;

    if(not(cin >> username)) {
        throw runtime_error("Could not get username");
    }

    cout << "Please Enter password" << endl;

    if(not(cin >> password)) {
        throw runtime_error("Could not get password");
    }

    passwordChecker(password);

    cout << username << endl;
    cout << password << endl;

    string hashedpassword = hashFunction(password);

    cout << hashedpassword << endl;

    /*const char* DBpassword = std::getenv("DB_PASSWORD");
    if(DBpassword != std::getenv("DBPASSWORD")) {
        throw runtime_error("Please set the necessary environment variables.");
    }*/

    sqlite3* DB;
    sqlite3_stmt *stmt;

    string sqlstatement = "INSERT INTO Users (username, pass) VALUES (?, ?);";

    int exit = sqlite3_open("login.db", &DB);
    if(exit != SQLITE_OK) {
        cout << "Failed to open database: " << sqlite3_errmsg(DB) << endl;
        return -1;
    }

    int prepare = sqlite3_prepare_v2(DB, sqlstatement.c_str(), -1, &stmt, NULL);

    if(prepare != SQLITE_OK) {
        cout << "Failed to prepare statement: " << sqlite3_errmsg(DB) << endl;
        return -1;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, hashedpassword.c_str(), -1, SQLITE_STATIC);

    int step = sqlite3_step(stmt);

    if(step != SQLITE_DONE) {
        cout << "Excution failed: " << sqlite3_errmsg(DB) << endl;
    }
    else {
        cout << "Data inserted successfully" << endl;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(DB);
    return 0;
}

void passwordChecker(string &password) {
    if(isdigit(password[0])) {
        cout << "password can't start with number plase try again" << endl;
        if(not(cin >> password)) {
            throw runtime_error("Could not get password");
        }

        passwordChecker(password);
    }
    return;
}

string hashFunction(const string &password) {
    int hash = 13;
    for(std::size_t i = 0; i < password.length(); ++i) {
        hash = hash * 31 + password.at(i);
    }

    return std::to_string(hash);
}