#include "../header/User.hpp"

#include <iostream>

using std::cout;
using std::endl;

string User::getUsername() {
    return username;
}

void User::logIn(Hotel currentHotel) {
    cout << "Here is all the avilable rooms numbers" << endl;
    currentHotel.printAvilableRooms();

}