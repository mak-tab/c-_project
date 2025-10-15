#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <direct.h>
#include "admin.h"

using namespace std;

class DoctorPanel {
private:
    string currentPatient;
    string logPath;

    void defaultDoctor() {
        ifstream test("doctors.txt");
        if (!test.good()) {
            ofstream f("doctors.txt");
            f << "doc doc" << endl;
            f.close();
        }
    }

    void loadPatient() {
        cout << "Enter patient UserName: ";
        cin >> currentPatient;
        logPath = "users_logs/" + currentPatient + "_logs.txt";
    }

    void viewFullLog() {
        ifstream in(logPath);
        if (!in.is_open()) {
            cout << "No logs found for this user." << endl;
            return;
        }
        cout << "\n====== Full Log ======\n";
        string line;
        while (getline(in, line)) {
            cout << line << "\n";
        }
        cout << "\n======================\n";
    }

    void viewDoctorNotes() {
        ifstream in(logPath);
        if (!in.is_open()) {
            cout << "No logs found for this user." << endl;
            return;
        }

        cout << "\n====== Doctor Notes ======\n";
        string line;
        bool inNote = false;
        bool found = false;

        while (getline(in, line)) {
            if (line == "[DoctorNote]") {
                inNote = true;
                found = true;
                continue;
            }
            if (line == "---" && inNote) {
                inNote = false;
                cout << "--------------------------\n";
                continue;
            }
            if (inNote) {
                cout << line << endl;
            }
        }

        if (!found) {
            cout << "No doctor notes found." << endl;
        }
        cout << "==========================\n";
    }

    void editMedications() {
        ifstream in(logPath);
        if (!in.is_open()) {
            cout << "No logs found to edit." << endl;
            return;
        }

        string blocks[20];
        string preamble;
        string line, currentBlock;
        int blockCount = 0;
        bool inCondition = false;

        while (getline(in, line)) {
            if (line.rfind("[Condition]:", 0) == 0) {
                if (!currentBlock.empty() && blockCount < 20) {
                    blocks[blockCount++] = currentBlock;
                }
                currentBlock = line + "\n";
                inCondition = true;
            } else if (line == "---" && inCondition) {
                currentBlock += line + "\n";
                if (blockCount < 20) {
                    blocks[blockCount++] = currentBlock;
                }
                currentBlock = "";
                inCondition = false;
            } else if (inCondition) {
                currentBlock += line + "\n";
            } else {
                preamble += line + "\n";
            }
        }
        if (!currentBlock.empty() && blockCount < 20) {
            blocks[blockCount++] = currentBlock;
        }
        in.close();

        if (blockCount == 0) {
            cout << "No conditions found to edit." << endl;
            return;
        }

        for (int i = 0; i < blockCount; i++) {
            cout << "[" << i + 1 << "]\n" << blocks[i] << "----------------\n";
        }

        int conditionIndex;
        cout << "Select condition number to edit: ";
        cin >> conditionIndex;
        conditionIndex--;

        if (conditionIndex < 0 || conditionIndex >= blockCount) {
            cout << "Invalid selection." << endl;
            return;
        }

        cin.ignore();
        stringstream ss(blocks[conditionIndex]);
        string header;
        getline(ss, header);

        string meds[50];
        int medCount = 0;
        while (getline(ss, line)) {
            if (line == "---") {
                break;
            }
            if (!line.empty() && medCount < 50) {
                meds[medCount++] = line;
            }
        }

        while (true) {
            cout << "\nCurrent Medications:\n";
            for (int i = 0; i < medCount; i++) {
                cout << "[" << i + 1 << "] " << meds[i] << "\n";
            }

            cout << R"(
    |=============================|
    ||                           ||
    ||          OPTIONS          ||
    ||                           ||
    |=============================|
    ||                           ||
    ||   [1] Add Medication      ||
    ||                           ||
    ||   [2] Remove Medication   ||
    ||                           ||
    ||   [3] Exit                ||
    ||                           ||
    |=============================|
    )";
            cout << "\n\nPlease enter your choice and press [Enter]: ";
            int choice;
            cin >> choice;
            cin.ignore();

            if (choice == 1) {
                if (medCount >= 50) {
                    cout << "Cannot add more medications." << endl;
                    continue;
                }

                string newMed;
                cout << "Enter full medication line (e.g., - Paracetamol | Day 1): ";
                getline(cin, newMed);
                meds[medCount++] = newMed;
            } else if (choice == 2) {
                int idx;
                cout << "Enter Index to remove: ";
                cin >> idx;
                cin.ignore();
                if (idx >= 1 && idx <= medCount) {
                    for (int i = idx - 1; i < medCount - 1; i++) {
                        meds[i] = meds[i + 1];
                    }
                    medCount--;
                    cout << "Medication removed.\n";
                } else {
                    cout << "Invalid index.\n";
                }
            } else if (choice == 3) {
                break;
            } else {
                cout << "Invalid choice." << endl;
            }
        }

        string newBlock = header + "\n";
        for (int i = 0; i < medCount; i++) {
            newBlock += meds[i] + "\n";
        }
        newBlock += "---\n";
        blocks[conditionIndex] = newBlock;

        ofstream out(logPath);
        out << preamble;
        for (int i = 0; i < blockCount; i++) {
            out << blocks[i];
        }
        out.close();

        cout << "Medication updated successfully." << endl;
    }

    void addDoctorNote() {
        cin.ignore();
        cout << "Enter your note (end with empty line):" << endl;
        string note, line;
        while (true) {
            getline(cin, line);
            if (line.empty()) {
                break;
            }
            note += line + "\n";
        }

        ofstream out(logPath, ios::app);
        out << "[DoctorNote]\n" << note << "---\n";
        out.close();

        cout << "Note added." << endl;
    }

public:
    void menu() {
        defaultDoctor();
        loadPatient();
        int choice;
        while (true) {
            cout << R"(
    |==============================|
    ||                            ||
    ||         DOCTOR PANEL       ||
    ||                            ||
    |==============================|
    ||                            ||
    || [1] View Full Log          ||
    ||                            ||
    || [2] Edit Medications       ||
    ||                            ||
    || [3] Add Doctor Note        ||
    ||                            ||
    || [4] View Doctor Notes      ||
    ||                            ||
    || [5] Exit                   ||
    ||                            ||
    |==============================|
    )";
            cout << "\n\nPlease enter your choice and press [Enter]: ";
            cin >> choice;
            switch (choice) {
                case 1: {
                    viewFullLog(); 
                    break;
                }
                case 2: {
                    editMedications(); 
                    break;
                }
                case 3: {
                    addDoctorNote(); 
                    break;
                }
                case 4: {
                    viewDoctorNotes(); 
                    break;
                }
                case 5: {
                    return;
                }
                default: {
                    cout << "Invalid option." << endl; 
                    break;
                }
            }
        }
    }
};
