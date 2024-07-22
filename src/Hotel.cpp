#include "../header/Hotel.hpp"
#include "../header/User.hpp"
#include <sqlite3.h>
#include <iostream>

Hotel::Hotel() {
   sqlite3* DB;
   sqlite3_stmt* stmt;

   int exit = sqlite3_open("login.db", &DB);
   
   if(exit != SQLITE_OK) {
    std::cout << "Failed to open database: " << sqlite3_errmsg(DB) << std::endl;
    return;
   }

   const char* sqlstatement = "SELECT roomNum, IsAvilable FROM Room";
   int prepare = sqlite3_prepare_v2(DB, sqlstatement, -1, &stmt, 0);
   if(prepare != SQLITE_OK) {
    std::cout << "SQL error: " << sqlite3_errmsg(DB) << std::endl;
    return;
   }

   while(sqlite3_step(stmt) == SQLITE_ROW) {
    int roomNumber = sqlite3_column_int(stmt, 0);
    bool isAvilable = sqlite3_column_int(stmt, 1);
    Room* currentRoom = new Room(roomNumber, isAvilable);
    roomList.push_back(currentRoom);
   }
   sqlite3_finalize(stmt);
   sqlite3_close(DB);
}

Hotel::~Hotel() {
    for (Room* currentRoom: roomList) {
        delete currentRoom;
        currentRoom = nullptr;
    }
}


void Hotel::printAvilableRooms() {
    for(Room* currentRoom: roomList) {
        if(currentRoom->isRoomAvilable()) {
            std::cout << currentRoom->getRoomNum() << std::endl;
        }
    }
}

Room* Hotel::getRoom(int roomNum) {
    for(Room* currentRoom: roomList) {
        if(currentRoom->getRoomNum() == roomNum) {
            currentRoom->setTaken();
            return currentRoom;
        }
    }

    return nullptr;
}

void Hotel::checkIn(User &currentUser) {
    std::cout << "Here is all the avilable rooms numbers" << std::endl;
    printAvilableRooms();
    std::cout << "Choose from the list what room number you want. The 100s is the first floor, the 200s is the second floor, and so on" << std::endl;
    int roomNum;
    if(not(std::cin >> roomNum)) {
        throw std::runtime_error("couldn't get runtime error");
    }

    Room* currentRoom = getRoom(roomNum);
    currentUser.setRoom(currentRoom);
    
}