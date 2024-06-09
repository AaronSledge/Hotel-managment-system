#include <iostream>
#include <stdexcept>
#include <string>

using std::string;
using std::cin;
using std::cout;
using std::endl;
using std::runtime_error;

void passwordChecker(string &password);

string  hashFunction(string &password);

int main() {
   
    string username;
    string password;
    cout << "Please Enter Username" << endl;

    if(not(cin >> username)) {
        throw runtime_error("Could not get username");
    }

    cout << "Please Enter password" << endl;

    if(not(cin >> password)) {
        throw runtime_error("Could not get password");
    }

    passwordChecker(password);
    
    cout << username << endl;
    cout << password << endl;

    string hashedpassword = hashFunction(password);

    cout << hashedpassword << endl;
    return 0;
}

void passwordChecker(string &password) {
    if(isdigit(password[0])) {
        cout << "password can't start with number plase try again" << endl;
        if(not(cin >> password)) {
            throw runtime_error("Could not get password");
        }

        passwordChecker(password);
    }
    return;
}

string hashFunction(string &password) {
    int hash = 13;
    for(std::size_t i = 0; i < password.length(); ++i) {
        hash = hash * 31 + password.at(i);
    }

    return std::to_string(hash);
}