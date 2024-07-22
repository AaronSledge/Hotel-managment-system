#pragma once

class Room {
    private:
        bool isAvilable;
        int roomNum;
    
    public:
        Room(int roomNum, bool isAvilable) : roomNum(roomNum), isAvilable(isAvilable) {}
        void getRoomService();
        int getRoomNum();
        bool isRoomAvilable();
        void cleanService();
        void setAvilable();
        void setTaken();
};