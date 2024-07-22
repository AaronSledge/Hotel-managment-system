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