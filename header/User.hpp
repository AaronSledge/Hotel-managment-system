#pragma once

#include <string>

using std::string;

class User{
    private:
        string username;
        string password;
        double balance;
        int roomNum;

    public:
        User(const string &username, const string &password) : username(username), password(password) {
            balance = 200.0;
            roomNum = 0;
        } 
        double getBalance();
        string getUsername();
        void logOut();
        void setRoomNum(int number);
        int getRoomNum();
};