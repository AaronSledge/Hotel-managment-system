#pragma once

#include "Room.hpp"
#include <vector>

using std::vector;
class Hotel {
    private:
        vector<Room*> roomList;
    
    public:
        Hotel();
        ~Hotel();
        void checkIn(int roomNum);
        void checkOut();
};