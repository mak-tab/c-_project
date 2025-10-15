#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include "reg.h"
#include "doc.h"

using namespace std;

class AdminPanel {
private:
    const string userFile = "users.txt";
    const string adminFile = "admin.txt";

    bool isNumber(const string& s) {
        if (s.empty()) {
            return false;
        }
        for (char c : s) {
            if (c < '0' || c > '9') {
                return false;
            }
        }
        return true;
    }

    void defaultAdmin() {
        ifstream test(adminFile);
        if (!test.good()) {
            ofstream f(adminFile);
            f << "admin admin\n";
            f.close();
        }
    }

    bool logInAdmin(const string& login, const string& password) {
        ifstream in(adminFile);
        string l, p;
        while (in >> l >> p) {
            if (l == login && p == password) {
                return true;
            }
        }
        return false;
    }

    void listUsers() {
            ifstream in(userFile);
        if (!in) {
            cout << "\nNo users found." << endl;
            return;
        }
        cout << "\nRegistered users: " << endl;
        string un, pw;
        int ind = 1;
        while (in >> un >> pw) {
            if (un.rfind("ban_", 0) == 0) {
                continue;
            }
            cout << "[" << ind++ << "] " << un << endl;
        }
    }

    string getUsernameByIndex(int ind) {
        ifstream in(userFile);
        string un, pw;
        int i = 1;
        while (in >> un >> pw) {
            if (i == ind) {
                return un;
            }
            i++;
        }
        return "";
    }

    void changeUserName() {
        listUsers();
        string input, newU;
        cout << "\nEnter current UserName or Index: "; 
        cin >> input;

        string oldU = input;
        if (isNumber(input)) {
            oldU = getUsernameByIndex(stoi(input));
        }

        cout << "Enter new UserName: "; cin >> newU;

        ifstream in(userFile);
        ofstream out("user_temp.txt");
        string un, pw;
        bool changed = false;

        while (in >> un >> pw) {
            if (un == oldU) {
                out << newU << " " << pw << endl;
                changed = true;
            } else {
                out << un << " " << pw << endl;
            }
        }

        in.close(); 
        out.close();
        remove(userFile.c_str());
        rename("user_temp.txt", userFile.c_str());

        if (changed) {
            cout << "UserName updated." << endl;
            string oldLog = "users_logs/" + oldU + "_logs.txt";
            string newLog = "users_logs/" + newU + "_logs.txt";

            ifstream check(oldLog);
            if (check.good()) {
                check.close();
                if (rename(oldLog.c_str(), newLog.c_str()) == 0) {
                    cout << "Log file renamed accordingly." << endl;
                } else {
                    cout << "ERROR: Failed to rename log file." << endl;
                }
            } else {
                cout << "Log file not found for renaming: " << oldLog << endl;
            }
        }
    }

    void changePassword() {
        listUsers();
        string input, newPass;
        cout << "Enter UserName or Index: "; 
        cin >> input;

        string username = input;
        if (isNumber(input)) {
            username = getUsernameByIndex(stoi(input));
        }

        cout << "Enter new password: "; cin >> newPass;

        ifstream in(userFile);
        ofstream out("user_temp.txt");
        string un, pw;
        bool changed = false;

        while (in >> un >> pw) {
            if (un == username) {
                out << un << " " << newPass << endl;
                changed = true;
            } else {
                out << un << " " << pw << endl;
            }
        }

        in.close(); 
        out.close();
        remove(userFile.c_str());
        rename("user_temp.txt", userFile.c_str());

        if (changed) {
            cout << "Password updated." << endl;
        } else {
            cout << "UserName not found." << endl;
        }
    }

    void deleteUser() {
        listUsers();
        string input;
        cout << "Enter UserName or Index to delete: "; 
        cin >> input;

        string username = input;
        if (isNumber(input)) {
            username = getUsernameByIndex(stoi(input));
        }

        ifstream in(userFile);
        ofstream out("user_temp.txt");
        string un, pw;
        bool deleted = false;

        while (in >> un >> pw) {
            if (un == username) {
                deleted = true;
                continue;
            }
            out << un << " " << pw << endl;
        }

        in.close(); 
        out.close();
        remove(userFile.c_str());
        rename("user_temp.txt", userFile.c_str());

        if (deleted) {
            string logFile = "users_logs/" + username + "_logs.txt";
            ifstream check(logFile);
            if (check.good()) remove(logFile.c_str());
            cout << "User deleted." << endl;
        } else {
            cout << "User not found." << endl;
        }
    }

    void banUser() {
        listUsers();
        string input;
        cout << "Enter UserName or Index to BAN: ";
        cin >> input;

        string username = input;
        if (isNumber(input)) {
            username = getUsernameByIndex(stoi(input));
        }

        ifstream in(userFile);
        ofstream out("user_temp.txt");
        string un, pw;
        bool banned = false;

        while (in >> un >> pw) {
            if (un == username) {
                out << "ban_" << un << " " << pw << endl;
                banned = true;
            } else {
                out << un << " " << pw << endl;
            }
        }

        in.close(); 
        out.close();
        remove(userFile.c_str());
        rename("user_temp.txt", userFile.c_str());

        if (banned) {
            cout << "User '" << username << "' has been banned." << endl;
        } else {
            cout << "User not found." << endl;
        }
    }

    void addNewAdmin() {
        string adm, pass;
        cout << "Enter new admin username: "; 
        cin >> adm;
        cout << "Enter password: "; 
        cin >> pass;

        ifstream in(adminFile);
        string l, dummy;
        while (in >> l >> dummy) {
            if (l == adm) {
                cout << "Admin already exists." << endl;
                return;
            }
        }

        ofstream out(adminFile, ios::app);
        out << adm << " " << pass << endl;
        cout << "Admin added." << endl;
    }

    void logs() {
        listUsers();
        string input;
        cout << "Enter UserName or Index to View Logs: "; 
        cin >> input;

        string username = input;
        if (isNumber(input)) {
            username = getUsernameByIndex(stoi(input));
        }

        string path = "users_logs/" + username + "_logs.txt";
        ifstream in(path);
        if (!in) {
            cout << "Log file not found." << endl;
            return;
        }

        cout << "\n=== Logs for " << username << " ===\n";
        string line;
        while (getline(in, line)) {
            cout << line << endl;
        }
        cout << "\n=============================\n";
    }

    void adminMenu() {
        while (true) {
            int ch;
            cout << R"(
    ||===========================||
    ||                           ||
    ||        ADMIN PANEL        ||
    ||                           ||
    |=============================|
    ||                           ||
    ||   [1] View All Users      ||
    ||                           ||
    ||   [2] Change UserName     ||
    ||                           ||
    ||   [3] Change PassWord     ||
    ||                           ||
    ||   [4] Delete User         ||
    ||                           ||
    ||   [5] Ban User            ||
    ||                           ||
    ||   [6] Add Admin           ||
    ||                           ||
    ||   [7] View Logs           ||
    ||                           ||
    ||   [8] Exit Admin Panel    ||
    ||                           ||
    |=============================|
    )";
            cout << "\n\nPlease enter your choice and press [Enter]: ";
            cin >> ch;

            switch (ch) {
                case 1: {
                    listUsers(); 
                    break;
                }
                case 2: {
                    changeUserName(); 
                    break;
                }
                case 3: {
                    changePassword(); 
                    break;
                }
                case 4: {
                    deleteUser(); 
                    break;
                }
                case 5: {
                    banUser(); 
                    break;
                }
                case 6: {
                    addNewAdmin(); 
                    break;
                }
                case 7: {
                    logs(); 
                    break;
                }
                case 8: {
                    cout << "Exiting Admin panel." << endl; 
                    return;
                }
                default: {
                    cout << "Invalid choice." << endl; 
                    break;
                }
            }
        }
    }

public:
    void startAsAdmin() {
        defaultAdmin();
        string l, p;
        cout << "Admin Login: "; 
        cin >> l;
        cout << "Admin Password: "; 
        cin >> p;
        if (logInAdmin(l, p)) {
            adminMenu();
        } else {
            cout << "Access denied." << endl;
        }
    }

    void startAsUser() {
        UserControl c;
        string currentUser;
        while (currentUser.empty()) {
            currentUser = logInMenu(c);
        }
        cout << "\n[Session started for user: " << currentUser << "]\n";
        MediTrack system;
        system.setUser(currentUser);
        system.run();
    }
};
