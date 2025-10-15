#pragma once

#include <iostream>
#include "diagnos.h"
#include "management.h"
#include "history.h"

using namespace std;

class System {
public:
    virtual void execute() = 0;
    virtual ~System() {}
};

class Diagnos : public System {
private: 
    string un;

public:
    Diagnos(string u) : un(u) {}

    void execute() override {
        cout << "\n[Loading Health Diagnostics Module...]\n\n";
        diagnosMod(un);
    }
};

class Manage : public System {
    string un;

public:
    Manage(const string& user) : un(user) {}

    void execute() override {
        cout << "\n[Opening Medication Management...]\n";
        MedManage manager(un);
        manager.manage();
    }
};

class MedHistory : public System {
private:
    string un;

public:
    MedHistory(const string& user) : un(user) {}

    void execute() override {
        cout << "\n[Accessing Medical History...]\n";
        History hist(un);
        hist.menu();
    }
};

class MediTrack {
private:
    string curUser;

public:
    void setUser(const string& u) {
        curUser = u;
    }

    string getUser() {
        return curUser;
    }

    void displayMenu() {
        cout << R"(
    |=========================================================|
    ||                                                       ||
    ||                 MediTrack System v1.0                 ||
    ||         Intelligent Personal Health Assistant         ||  
    ||                  (AI-based analysis)                  ||   
    ||                                                       ||
    |=========================================================|
    ||                                                       ||
    || [1] Health Diagnostics                                ||  
    ||     |-Enter Symptoms, Vital Signs                     ||  
    ||                                                       ||
    || [2] Medication Management                             ||  
    ||     |-Tracking doses, reminders                       ||
    ||                                                       ||
    || [3] View Medical History                              ||  
    ||     |-All records, filters                            ||  
    ||                                                       ||
    || [4] Exit System                                       ||
    ||                                                       ||
    |=========================================================|
    )";
        cout << "\n\nPlease enter your choice and press [Enter]: "; 
    }

    void run() {
        int ch;
        do {
            displayMenu();           
            cin >> ch;
            cin.ignore();
            
            System* module = nullptr;

            switch (ch) {
                case 1: {
                    module = new Diagnos(curUser);
                    break;
                }
                case 2: { 
                    module = new Manage(curUser);
                    break;
                }
                case 3: {
                    module = new MedHistory(curUser);    
                    break;
                }
                case 4: { 
                    cout << "\nExiting MediTrack. Stay healthy!\n"; 
                    exit(0);
                }
                default: {
                    cout << "\nERROR: Invalid" << endl; 
                    continue;
                }
            }
            
            if (module) {
                module->execute();
                delete module;
            }
            
        } while (ch != 4);
    }
};
