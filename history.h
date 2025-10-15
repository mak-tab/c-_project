#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <direct.h>

using namespace std;

class History {
    string un;
    string logPath;

public:
    History(const string& user) : un(user) {
        _mkdir("users_logs");
        logPath = "users_logs/" + un + "_logs.txt";
    }

    void ChatHistory() {
        ifstream in(logPath);
        if (!in.is_open()) {
            cout << "\nNo chat history found.\n";
            return;
        }

        cout << "\n====== Chat History ======\n";
        string line;
        bool inChat = false;
        while (getline(in, line)) {
            if (line == "[Chat]") {
                inChat = true;
                continue;
            }
            if (line.rfind("[Condition]:", 0) == 0 || line == "---") {
                inChat = false;
            }
            if (inChat) {
                cout << line << endl;
            }
        }
        cout << "===========================\n";
    }

    void CondHistory() {
        ifstream in(logPath);
        if (!in.is_open()) {
            cout << "\nNo condition history found.\n";
            return;
        }

        cout << "\n==== Condition History ====\n";
        string line;
        bool inCond = false;
        while (getline(in, line)) {
            if (line.rfind("[Condition]:", 0) == 0) {
                inCond = true;
                cout << line << endl;
                continue;
            }
            if (line == "---") {
                cout << "----------------------------\n";
                inCond = false;
                continue;
            }
            if (inCond) {
                cout << line << endl;
            }
        }
        cout << "============================\n";
    }

    void menu() {
        int choice;
        while (true) {
            cout << R"(
    |=============================|
    ||                           ||
    ||       History Menu        ||
    ||                           ||
    |=============================|
    ||                           ||
    ||   [1] Chat History        ||
    ||                           ||
    ||   [2] Condition History   ||
    ||                           ||
    ||   [3] Exit                ||
    ||                           ||
    |=============================|
    )";
            cout << "\n\nPlease select an option and press [Enter]: ";
            cin >> choice;
            cin.ignore();

            switch(choice) {
                case 1: {
                    ChatHistory(); 
                    break;
                }
                case 2: {
                    CondHistory(); 
                    break;
                }
                case 3: {
                    return;
                }
                default: {
                    cout << "Invalid selection." << endl; 
                    break;
                }
            }
        }
    }

    static void logChat(const string& un, const string& userMsg, const string& botReply) {
        _mkdir("users_logs");
        ofstream out("users_logs/" + un + "_logs.txt", ios::app);
        if (!out) {
            cout << "[Error] Could not open log file for writing." << endl;
            return;
        }
        out << "[Chat]\n";
        out << "User: " << userMsg << "\n";
        out << "Bot: " << botReply << "\n\n";
    }

    static void logCondition(const string& un, const string& condition, const string& medList) {
        _mkdir("users_logs");
        ofstream out("users_logs/" + un + "_logs.txt", ios::app);
        if (!out) {
            cout << "[Error] Could not open log file for writing." << endl;
            return;
        }
        out << "[Condition]: " << condition << endl;
        out << medList;
        out << "---\n";
    }
};
