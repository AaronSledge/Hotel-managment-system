#include <iostream>
#include <stdexcept>
#include <string>

using namespace std;

void passwordChecker(string &password);

int main() {
   try {
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
    }
    catch(const exception &e) {
        cout << "Exception " << e.what() << endl;
    }
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