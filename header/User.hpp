#pragma once

#include <string>
#include "Room.hpp"
using std::string;

class User{
    private:
        string username;
        string password;
        int balance;
        int roomNum;
        Room* myRoom;

    public:
        User(const string &userName, const string &passWord);
        double getBalance();
        string getUsername();
        void logOut();
        void setRoom(Room* currentRoom);
        int getRoomNum();
        Room* getRoom();
        void gamble();
};