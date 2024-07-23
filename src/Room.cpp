#include "../header/Room.hpp"
#include <iostream>
#include <stdexcept>

using std::cout;
using std::endl;
using std::cin;

int Room::getRoomNum() {
    return roomNum;
}

bool Room::isRoomAvilable() {
    return isAvilable;
}

void Room::setAvilable() {
    isAvilable = true;
}

void Room::setTaken() {
    isAvilable = false;
}

void Room::checkOut() {
    helperCheckOut();
}

void Room::helperCheckOut() {
    roomNum = 0;
}



