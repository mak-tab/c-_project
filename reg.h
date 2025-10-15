#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include "main.h"

using namespace std;

class User {
private:
    string user;
    string pass;

public:
    User(const string& u = "", const string& p = "") : user(u), pass(p) {}

    string getUser() const {
        return user;
    }
    string getPass() const {
        return pass;
    }

    string writeLog() const {
        return user + " " + pass + "\n";
    }

    static User fromLogs(istream& in) {
        string u, p;
        in >> u >> p;
        return User(u, p);
    }

    void display() const {
        cout << user;
    }
};

class UserControl {
private:
    User* user;
    int count;

public:
    UserControl() {
        user = nullptr;
        count = 0;
        load();
    }

    ~UserControl() {
        delete[] user;
    }

    int getc() const {
        return count;
    }

    void load() {
        ifstream file("users.txt");
        if (!file.is_open()) {
            return;
        }

        count = 0;
        string line;
        while (getline(file, line)) {
            if (!line.empty()) {
                ++count;
            }
        }

        file.clear();
        file.seekg(0);

        delete[] user;
        user = new User[count];

        for (int i = 0; i < count; i++) {
            user[i] = User::fromLogs(file);
        }

        file.close();
    }

    User* findUserFlexible(const string& input, int& outIndex) {
        try {
            int index = stoi(input);
            index--;
            User* u = get(index);
            if (u) {
                outIndex = index;
                return u;
            }
        } catch (...) {
        }

        for (int i = 0; i < count; i++) {
            if (user[i].getUser() == input) {
                outIndex = i;
                return &user[i];
            }
        }

        return nullptr;
    }

    void appendUser(const User& usr) {
        ofstream file("users.txt", ios::app);
        if (file.is_open()) {
            file << usr.writeLog();
        }
        file.close();
        load();
    }

    void save() {
        ofstream file("users.txt");
        for (int i = 0; i < count; i++) {
            file << user[i].writeLog();
        }
        file.close();
    }

    void displayUsers() {
        for (int i = 0; i < count; i++) {
            if (user[i].getUser().rfind("ban_", 0) == 0) {
                continue;
            }
            cout << "[" << i + 1 << "] ";
            user[i].display();
            cout << "\n";
        }
    }

    bool existing(const string& name) {
        for (int i = 0; i < count; i++) {
            if (user[i].getUser() == name) {
                return true;
            }
        }
        return false;
    }

    User* get(int i) {
        if (i >= 0 && i < count) {
            return &user[i];
        } else {
            return nullptr;
        }
    }

void changeUser() {
    displayUsers();

    cout << "\nEnter username or user number: ";
    string input;
    cin >> input;

    int idx = -1;
    User* up = findUserFlexible(input, idx);

    if (!up) {
        cout << "ERROR: User not found." << endl;
        return;
    }

    string curPass, newUser;
    cout << "Enter current password: ";
    cin >> curPass;

    if (up->getPass() != curPass) {
        cout << "ERROR: Incorrect current password." << endl;
        return;
    }

    cout << "Enter new username: ";
    cin >> newUser;

    if (existing(newUser)) {
        cout << "ERROR: Username already taken." << endl;
        return;
    }

    string oldLog = "users_logs/" + up->getUser() + "_logs.txt";
    string newLog = "users_logs/" + newUser + "_logs.txt";

    *up = User(newUser, curPass);
    save();

    ifstream check(oldLog);
    if (check.good()) {
        check.close();
        rename(oldLog.c_str(), newLog.c_str());
    }

    cout << "UserName updated successfully." << endl;
}

void changePass() {
    displayUsers();

    cout << "\nEnter UserName or Index: ";
    string input;
    cin >> input;

    int idx = -1;
    User* up = findUserFlexible(input, idx);

    if (!up) {
        cout << "ERROR: User not found." << endl;
        return;
    }

    string cur, nw;
    cout << "Enter current password: ";
    cin >> cur;

    if (up->getPass() != cur) {
        cout << "ERROR: Incorrect current password." << endl;
        return;
    }

    cout << "Enter new PassWord: ";
    cin >> nw;

    *up = User(up->getUser(), nw);
    save();
    cout << "Password updated successfully." << endl;
}

    void deleteUser(int ind) {
        ofstream f("users.txt");
        for (int i = 0; i < count; i++) {
            if (i != ind) {
                f << user[i].writeLog();
            }
        }
        f.close();

        string logFile = "users_logs/" + user[ind].getUser() + "_logs.txt";
        ifstream check(logFile);
        if (check.good()) {
            remove(logFile.c_str());
        }

        load();
    }
};

User inputUser() {
    string u, p;
    cout << "Enter username: "; 
    cin >> u;
    cout << "Enter password: "; 
    cin >> p;
    return User(u, p);
}

string logInMenu(UserControl& c) {
    while (true) { 
        int ch;
        cout << R"(
    |==========================|
    ||                        ||
    ||  WELCOME TO MEDITRACK  ||
    ||                        ||
    ||========================||
    ||                        ||
    ||  [1] Log In            ||
    ||                        ||
    ||  [2] Sign Up           ||
    ||                        ||
    ||  [3] Change UserName   ||
    ||                        ||
    ||  [4] Change PassWord   ||
    ||                        ||
    ||  [5] Delete Account    ||
    ||                        ||
    ||  [6] Exit System       ||
    ||                        ||
    |==========================|
    )";
        cout << "\nPlease enter your choice and press [Enter]: ";
        cin >> ch;

        switch (ch) {
            case 1: {
                if (c.getc() == 0) {
                    cout << "\nNo users available. Please register first.\n";
                    continue;
                }
            
                cout << "\nAvailable Users:\n";
                c.displayUsers();
            
                string input;
                cout << "Enter username or user number: ";
                cin >> input;
            
                int idx = -1;
                User* u = c.findUserFlexible(input, idx);
            
                if (!u) {
                    cout << "ERROR: User not found." << endl;
                    continue;
                }
            
                string p;
                cout << "Enter your password: ";
                cin >> p;
            
                if (u->getPass() == p) {
                    if (u->getUser().rfind("ban_", 0) == 0) {
                        cout << "Access denied. This account is banned." << endl;
                        continue;
                    }
                    cout << "Log in successfully." << endl;
                    return u->getUser();
                } else {
                    cout << "ERROR: Incorrect password." << endl;
                    continue;
                }
            }
        
            case 2: {
                User u = inputUser();
                if (c.existing(u.getUser())) {
                    cout << "User already existing." << endl;
                    continue;
                }
            
                c.appendUser(u);
                cout << "Registration successfully completed." << endl;
                return u.getUser();
            }
            case 3: {
                c.changeUser();
                continue;;
            }
            case 4: {
                c.changePass();
                break;
            }
            case 5: {
                if (c.getc() == 0) {
                    cout << "\nNo users available.\n";
                    continue;
                }
            
                cout << "\nRegistered users:\n";
                c.displayUsers();
            
                string input;
                cout << "Enter UserName or Index: ";
                cin >> input;
            
                int idx = -1;
                User* u = c.findUserFlexible(input, idx);
            
                if (!u) {
                    cout << "ERROR: User not found." << endl;
                    continue;
                }
            
                string p;
                cout << "Enter password to confirm: ";
                cin >> p;
            
                if (u->getPass() == p) {
                    c.deleteUser(idx);
                    cout << "User deleted successfully." << endl;
                } else {
                    cout << "ERROR: Incorrect password." << endl;
                }
                continue;
            }
            case 6: {
                cout << "Exiting system. Stay healthy!" << endl;
                exit(0);
            }
        
            default: {
                cout << "ERROR: Invalid option. Please try again." << endl;
                continue;
            }
        }
    } 
}
