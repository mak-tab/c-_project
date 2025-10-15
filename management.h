#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <direct.h>

using namespace std;

const int MAX_COND = 20;
const int MAX_MEDS = 10;

struct MedEntry {
    string name;
    string days;
};

struct CondBlock {
    string condition;
    MedEntry meds[MAX_MEDS];
    int medCount = 0;
};

bool readIntSafe(int& value) {
    string input;
    getline(cin, input);
    stringstream ss(input);
    return (ss >> value && ss.eof());
}

class MedManage {
private:
    string un;
    string filepath;

public:
    MedManage(const string& user) : un(user) {
        _mkdir("users_logs");
        filepath = "users_logs/" + un + "_logs.txt";
    }

    int load(CondBlock blocks[MAX_COND]) {
        ifstream in(filepath);
        string line;
        int condInd = -1;

        while (getline(in, line)) {
            if (line.find("[Condition]: ") == 0) {
                ++condInd;
                blocks[condInd].condition = line.substr(13);
                blocks[condInd].medCount = 0;
            } else if (line.find("- ") == 0 && line.find('|') != string::npos && condInd >= 0) {
                int idx = blocks[condInd].medCount++;
                size_t pipe = line.find('|');
                blocks[condInd].meds[idx].name = line.substr(2, pipe - 3);
                blocks[condInd].meds[idx].days = line.substr(pipe + 2);
            }
        }

        return condInd + 1;
    }

    void save(CondBlock blocks[MAX_COND], int count) {
        ifstream in(filepath);
        string line, preserved;
        bool inCond = false;
        while (getline(in, line)) {
            if (line.rfind("[Condition]:", 0) == 0) {
                inCond = true;
                continue;
            }
            if (line == "---") {
                inCond = false;
                continue;
            }
            if (!inCond) {
                preserved += line + "\n";
            }
        }
        in.close();

        ofstream out(filepath);
        out << preserved;

        for (int i = 0; i < count; i++) {
            out << "[Condition]: " << blocks[i].condition << endl;
            for (int j = 0; j < blocks[i].medCount; j++) {
                out << "- " << blocks[i].meds[j].name << " | " << blocks[i].meds[j].days << endl;
            }
            out << "---\n";
        }
    }

    void editMed(MedEntry& med) {
        while (true) {
            cout << "\nEditing: " << med.name << " [" << med.days << "]" << endl;
            int ch;
            cout << R"(
    |==========================|
    ||                        || 
    ||   [1] Transfer Day     ||
    ||                        ||
    ||   [2] Edit Day List    ||
    ||                        ||
    ||   [3] Back             ||
    ||                        ||
    ||========================||
)";
            cout << "\n\nPlease enter your choice and press [Enter]: ";
            if (!readIntSafe(ch)) {
                cout << "Invalid input.\n";
                continue;
            }

            switch (ch) {
                case 1: {
                    cout << "Enter new single day (e.g., Day 2): ";
                    getline(cin, med.days);
                    cout << "Day transferred." << endl;
                    break;
                }
                case 2: {
                    cout << "Enter new day list (e.g., Day 1, Day 2): ";
                    getline(cin, med.days);
                    cout << "Days updated." << endl;
                    break;
                }
                case 3: {
                    return;
                }
                default: {
                    cout << "Invalid choice." << endl;
                    continue;
                }
            }
        }
    }

    void manage() {
        CondBlock blocks[MAX_COND];
        int count = load(blocks);

        if (count == 0) {
            cout << "No medication history found." << endl;
            return;
        }

        while (true) {
            cout << "\nSelect a condition:\n";
            for (int i = 0; i < count; i++) {
                cout << "[" << i + 1 << "] " << blocks[i].condition << endl;
            }
            cout << "[0] Exit\nChoice: ";
            int cond;
            if (!readIntSafe(cond)) {
                cout << "Invalid input." << endl;
                continue;
            }

            if (cond == 0) break;
            if (cond < 1 || cond > count) {
                cout << "Invalid condition." << endl;
                continue;
            }

            CondBlock& selected = blocks[cond - 1];

            int action;
            do {
                cout << "\n|======== " << selected.condition << " ========|\n";
                for (int i = 0; i < selected.medCount; i++) {
                    cout << "[" << i + 1 << "] " << selected.meds[i].name << " - " << selected.meds[i].days << endl;
                }

                cout << R"(
    |====================================|
    ||                                  ||
    || [1] Edit Day                     ||
    ||                                  ||
    || [2] Remove Medication            ||
    ||                                  ||
    || [3] Add Medication               ||
    ||                                  ||
    || [4] Finish Editing Condition     ||
    ||                                  ||
    |====================================|
)";
                cout << "\n\nPlease enter your choice and press [Enter]: ";
                if (!readIntSafe(action)) {
                    cout << "Invalid input." << endl;
                    continue;
                }

                switch (action) {
                    case 1: {
                        int mi;
                        cout << "Select medication: ";
                        if (!readIntSafe(mi)) {
                            cout << "Invalid input." << endl;
                            continue;
                        }
                        if (mi < 1 || mi > selected.medCount) {
                            cout << "Invalid index." << endl;
                            continue;
                        }
                        editMed(selected.meds[mi - 1]);
                        save(blocks, count);
                        break;
                    }
                    case 2: {
                        int mi;
                        cout << "Select medication to remove: ";
                        if (!readIntSafe(mi)) {
                            cout << "Invalid input." << endl;
                            continue;
                        }
                        if (mi < 1 || mi > selected.medCount) {
                            cout << "Invalid index." << endl;
                            continue;
                        }
                        for (int j = mi - 1; j < selected.medCount - 1; j++) {
                            selected.meds[j] = selected.meds[j + 1];
                        }
                        --selected.medCount;
                        save(blocks, count);
                        cout << "Medication removed." << endl;
                        break;
                    }
                    case 3: {
                        if (selected.medCount >= MAX_MEDS) {
                            cout << "Cannot add more medications." << endl;
                            continue;
                        }
                        cout << "Enter medication name: ";
                        getline(cin, selected.meds[selected.medCount].name);
                        cout << "Enter day(s) (e.g., Day 1, Day 2): ";
                        getline(cin, selected.meds[selected.medCount].days);
                        ++selected.medCount;
                        save(blocks, count);
                        cout << "Medication added." << endl;
                        break;
                    }
                    case 4:
                        break;
                    default:
                        cout << "Invalid option." << endl;
                }
            } while (action != 4);
        } 
    }

    void addToHistoryDirectly(const string& condition, string meds[], int medCount) {
        CondBlock blocks[MAX_COND];
        int count = load(blocks);

        int index = -1;
        for (int i = 0; i < count; i++) {
            if (blocks[i].condition == condition) {
                index = i;
                break;
            }
        }

        if (index == -1 && count < MAX_COND) {
            index = count++;
            blocks[index].condition = condition;
        }

        for (int i = 0; i < medCount; i++) {
            if (blocks[index].medCount < MAX_MEDS) {
                blocks[index].meds[blocks[index].medCount++] = {meds[i], "Day 1"};
            }
        }

        save(blocks, count);
        cout << "Saved " << medCount << " medication(s) under '" << condition << "'.\n";
    }
};
