#include "../header/Room.hpp"

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