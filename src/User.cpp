#include "../header/User.hpp"

#include <iostream>
#include <sqlite3.h>
#include <stdexcept>
#include <cstdlib>
#include <time.h>

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

string User::getPassword() {
    return password;
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

void User::gamble() {
    cout << "Welcome to the casino!" << endl;
    cout << "Pick a random number between 1 and 12. If you guess correctly you get double the money you bet." << endl;
    cout << "How much are you willing to bet?" << endl;
    int bet;
    do {
        if(not(cin >> bet)) {
            throw std::runtime_error("Could not get bet input");
        }

        if(bet > balance) {
            cout << "Insufficent funds: Please choose lower amount" << endl;
        }
    } while(bet > balance);

    cout << "Pick a number between 1 and 12" << endl;

    int guessNum;
    
    srand(time(0));
    char playAgain;
    do {
        do {
            if(not(cin >> guessNum)) {
                throw std::runtime_error("Could not get guess input");
            }
        
            if(guessNum < 1 || guessNum > 12) {
                cout << "Your guess must be between 1 and 12, try again" << endl;
            }
        } while(guessNum < 1 || guessNum > 12);


        int random = rand() % 12 + 1;
        if(random < 1) {
            throw std::underflow_error("Random failed, too low");
        }
        else if(random > 12) {
            throw std::overflow_error("Random failed, too high");
        }

        if(balance <= 0) {
            cout << "You are out of money, sorry." << endl;
            return;
        }

        if(random == guessNum) {
            balance += bet;
            cout << "congrats you won, want to play again? (y/n)" << endl;
            if (not(cin >> playAgain)) {
                throw std::runtime_error("couldn't read playAgain");
            }
        }
        else {
            balance -= bet;
            cout << "Sorry that wasn't the right number. The computer chose " << random << endl;
            cout << "Do you want to play again (y/n)?" << endl;
            if (not(cin >> playAgain)) {
                throw std::runtime_error("couldn't read playAgain");
            }
        }
    } while(playAgain == 'y');
    
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

