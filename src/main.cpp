#include <iostream>
#include <stdexcept>
#include <string>
#include <cstdlib>
#include <sqlite3.h>
#include "../header/User.hpp"
#include "../header/Hotel.hpp"
using std::string;
using std::cin;
using std::cout;
using std::endl;
using std::runtime_error;

void passwordChecker(string &password);

void registerFirstTime(string &username, string &hashedpassword);

void login(string &username, string &hashedpassword);

string hashFunction(const string &password);

void printWelcomeMenu(User &currentUser, Hotel &currentHotel);

int main() {
    //prompt user to sign in for the first time or login existing account
    cout << "Are you signing in for the first time (y/n)" << endl;
    char firstTime;
    if(not(cin >> firstTime)) {
        throw runtime_error("Couldn't get input");
    }

    tolower(firstTime); //turns input lowercase


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

    //check password follows guidelines and hash it for security
    passwordChecker(password);


    string hashedpassword = hashFunction(password);

    if(firstTime == 'y') {
       registerFirstTime(username, hashedpassword); 
    }
    else if(firstTime == 'n') {
        login(username, hashedpassword);
    }

    //create user and hotel objects, then print menu
    Hotel currentHotel;
    User currentUser = User(username, hashedpassword);
    if(currentUser.getRoomNum() != 0) {
        currentUser.setRoom(currentHotel.getRoom(currentUser.getRoomNum()));
    }

    printWelcomeMenu(currentUser, currentHotel);

    return 0;
}

void passwordChecker(string &password) {
    //password must start with letter
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
    //get sqlite database ready
    sqlite3* DB;
    sqlite3_stmt* stmt;
    int exit = sqlite3_open("C:/Users/User/OneDrive/Documents/Login-system/hotel-mangement-system/login.db", &DB);

    //error message
    if(exit != SQLITE_OK) {
        cout << "Failed to open database: " << sqlite3_errmsg(DB) << endl;
        return;
    }

    //insert username and password into database if registering for the first time
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
    //get sqlite open
    sqlite3* DB;
    sqlite3_stmt* stmt;
    int exit = sqlite3_open("C:/Users/User/OneDrive/Documents/Login-system/hotel-mangement-system/login.db", &DB);

    //read from database and see username and password exist within
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
        cout << "Please eneter username or password again" << endl;
        sqlite3_finalize(stmt);
        sqlite3_close(DB);
        cin >> username;
        cin >> hashedpassword;
        passwordChecker(hashedpassword);
        hashedpassword = hashFunction(hashedpassword);
        login(username, hashedpassword);
        return;

    }
    else {
        cout << "Failed to execute statement: " << sqlite3_errmsg(DB) << endl;
    }
    sqlite3_finalize(stmt);
    sqlite3_close(DB);
}

void printWelcomeMenu(User &currentUser, Hotel &currentHotel) {
    cout << "Welcome to *** hotel. We hope you have a good time" << endl;

    cout << "Type c if you want to see and check in to avilable rooms" << endl;
    cout << "Type o if you want to check out room" << endl;
    cout << "Type l if you are ready to log out" << endl;
    cout << "Type g if you want to go to the casino" << endl;

    cout << "Make sure to log out when you are done or progress may not be saved" << endl;
    char input;
    if(not(cin >> input)) {
        throw runtime_error("Couldn't get input");
    }

    if(input == 'c') {
        if(currentUser.getRoom() != nullptr) {
            cout << "Room is already picked" << endl;
            printWelcomeMenu(currentUser, currentHotel);
        }
        else {
            currentHotel.checkIn(currentUser);
            printWelcomeMenu(currentUser, currentHotel);
        }
    }

    if (input == 'o') {
        currentHotel.checkOut(currentUser);
        printWelcomeMenu(currentUser, currentHotel);
    }

    if(input == 'g') {
        currentUser.gamble();
        printWelcomeMenu(currentUser, currentHotel);
    }
    if(input == 'l') {
        currentUser.logOut();
    }


}