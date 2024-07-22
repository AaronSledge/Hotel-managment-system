#include "../header/User.hpp"

#include <iostream>
#include <sqlite3.h>
#include <stdexcept>

using std::cout;
using std::endl;
using std:: cin;


User::User(const string &userName, const string &passWord) {
    username = userName;
    password = passWord;
    myRoom = nullptr;

    sqlite3* DB;
    sqlite3_stmt* stmt;

    int exit = sqlite3_open("login.db", &DB);

    if(exit != SQLITE_OK) {
        cout << "Failed to open database: " << sqlite3_errmsg(DB) << endl;
        return;
    }

    const char* sqlstatement = "SELECT balance, roomNum FROM Users WHERE username = ? AND pass = ?";
    int prepare = sqlite3_prepare_v2(DB, sqlstatement, -1, &stmt, 0);
    if(prepare != SQLITE_OK) {
        cout << "SQL error: " << sqlite3_errmsg(DB) << endl;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);

    if(sqlite3_step(stmt) == SQLITE_ROW) {
        balance = sqlite3_column_int(stmt, 0);
        roomNum = sqlite3_column_int(stmt, 1);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(DB);
}
string User::getUsername() {
    return username;
}


void User::setRoom(Room* currentRoom) {
    myRoom = currentRoom;
    roomNum = currentRoom->getRoomNum();
}

Room* User::getRoom() {
    return myRoom;
}

int User::getRoomNum() {
    return roomNum;
}

void User::logOut() {
    sqlite3* DB;
    sqlite3_stmt* stmt;
    
    int exit = sqlite3_open("login.db", &DB);

    if(exit != SQLITE_OK) {
        cout << "Failed to open database: " << sqlite3_errmsg(DB) << endl;
    }

    const char* sqlstatement = "UPDATE Room SET isAvilable = ? WHERE roomNum = ?;";
    int prepare = sqlite3_prepare_v2(DB, sqlstatement, -1, &stmt, 0);
    
    if(prepare != SQLITE_OK) {
        cout << "Failed to prepare statement: " << sqlite3_errmsg(DB) << endl;
    }

    sqlite3_bind_int(stmt, 1, myRoom->isRoomAvilable());
    sqlite3_bind_int(stmt, 2, myRoom->getRoomNum());

    int step = sqlite3_step(stmt);
    if(step != SQLITE_DONE) {
        cout << "Excution failed: " << sqlite3_errmsg(DB) << endl;
    }

    sqlite3_finalize(stmt);

    sqlstatement = "UPDATE Users SET roomNum = ? WHERE username = ? AND pass = ?;";
    prepare = sqlite3_prepare_v2(DB, sqlstatement, -1, &stmt, 0);

    if(prepare != SQLITE_OK) {
        cout << "Failed to prepare statement: " << sqlite3_errmsg(DB) << endl;
    }

    sqlite3_bind_int(stmt, 1, myRoom->getRoomNum());
    sqlite3_bind_text(stmt, 2, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, password.c_str(), -1, SQLITE_STATIC);

    step = sqlite3_step(stmt);

    if(step != SQLITE_DONE) {
        cout << "Excution failed: " << sqlite3_errmsg(DB) << endl;
    }
    sqlite3_finalize(stmt);
    sqlite3_close(DB);
}

