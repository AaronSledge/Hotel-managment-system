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

void registerFirstTime(string &username, string &hashedpassword);

void login(string &username, string &hashedpassword);

string hashFunction(const string &password);

int main() {
    cout << "Are you signing in for the first time" << endl;
    string firstTime;
    if(not(cin >> firstTime)) {
        throw runtime_error("Couldn't get input");
    }
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


    string hashedpassword = hashFunction(password);
    
    if(firstTime == "yes") {
       registerFirstTime(username, hashedpassword); 
    }
    else if(firstTime == "no") {
        login(username, hashedpassword);
    }
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

void registerFirstTime(string &username, string &hashedpassword) {
    sqlite3* DB;
    sqlite3_stmt* stmt;
    int exit = sqlite3_open("login.db", &DB);

    if(exit != SQLITE_OK) {
         cout << "Failed to open database: " << sqlite3_errmsg(DB) << endl;
        return;
    }

    string sqlstatement = "INSERT INTO Users (username, pass) VALUES (?, ?);";

    int prepare = sqlite3_prepare_v2(DB, sqlstatement.c_str(), -1, &stmt, NULL);

    if(prepare != SQLITE_OK) {
        cout << "Failed to prepare statement: " << sqlite3_errmsg(DB) << endl;
        sqlite3_close(DB);
        return;
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
}

void login(string &username, string &hashedpassword) {
    sqlite3* DB;
    sqlite3_stmt* stmt;
    int exit = sqlite3_open("login.db", &DB);

    string sqlstatement = "SELECT * FROM Users WHERE username = ? AND pass = ?";
    int prepare = sqlite3_prepare_v2(DB, sqlstatement.c_str(), -1, &stmt, NULL);
    if (prepare != SQLITE_OK) {
        cout << "Failed to prepare statement: " << sqlite3_errmsg(DB) << endl;
        sqlite3_close(DB);
        return;
    }

    int usernameBind = sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    int passwordBind = sqlite3_bind_text(stmt, 2, hashedpassword.c_str(), -1, SQLITE_STATIC);

    int step = sqlite3_step(stmt);

    if(step == SQLITE_ROW) {
        const unsigned char* DBusername = sqlite3_column_text(stmt, 1);
        const unsigned char* DBpassword = sqlite3_column_text(stmt, 2);
    }
    else if(step == SQLITE_DONE) {
        cout << "No user found with the provided username and password." << endl;
    }
    else {
        cout << "Failed to execute statement: " << sqlite3_errmsg(DB) << endl;
    }
    sqlite3_finalize(stmt);
    sqlite3_close(DB);
}