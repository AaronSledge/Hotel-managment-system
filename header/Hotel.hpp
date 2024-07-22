#pragma once

#include "Room.hpp"
#include "User.hpp"
#include <vector>

using std::vector;
class Hotel {
    private:
        vector<Room*> roomList;
    
    public:
        Hotel();
        ~Hotel();
        void printAvilableRooms();
        void checkIn(User &currentUser);
        void checkOut();
        Room* getRoom(int roomNum);
};