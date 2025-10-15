#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <direct.h>
#include "history.h"
#include "management.h"

using namespace std;

const int MAX_SYMPTOMS = 10;
const int MAX_MEDICATIONS = 5;
const int MAX_DISEASE = 20;
const int LEVENSHTEIN = 2;
const int MAX_RESP = 10;

string GREET[MAX_RESP] = {
    "Hello! How can I assist you?",
    "Hi! What brings you here?",
    "Greetings! What’s bothering you?",
    "Hey! Ready to listen.",
    "You can describe your symptoms now.",
    "Hello there! Let's talk about your health.",
    "Welcome back! What's going on today?"
};
string FAREWELL[MAX_RESP] = {
    "Goodbye! Stay healthy.",
    "Take care!",
    "Farewell and rest well.",
    "See you soon!",
    "Wishing you good health!"
};
string THX[MAX_RESP] = {
    "You're welcome!",
    "Glad to help!",
    "No problem!",
    "Happy to assist!",
    "Any time!"
};

int greet = 0, bye = 0, thx = 0;

struct Disease {
    string n;
    string symptoms[MAX_SYMPTOMS];
    int symptomsCount = 0;
    string medications[MAX_MEDICATIONS];
    int medicationsCount = 0;
    string description;
};

int levenshtein(const string& a, const string& b) {
    int n = a.size(), m = b.size();
    int dp[50][50];
    for (int i = 0; i <= n; i++) { 
        dp[i][0] = i;
    }
    for (int j = 0; j <= m; j++) { 
        dp[0][j] = j;
    }

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            int cost; 
            if (tolower(a[i - 1]) != tolower(b[j - 1])) { 
                cost = 1; 
            } else { 
                cost = 0; 
            };
            dp[i][j] = min(min(dp[i - 1][j] + 1, dp[i][j - 1] + 1), dp[i - 1][j - 1] + cost);
        }
    }
    return dp[n][m];
}

void diagnosMod(const string& un) {
    _mkdir("users_logs"); 

    MedManage medLog(un);

    Disease db[MAX_DISEASE];
    int dbCount = 0;
    ifstream fin("symptoms.txt");
    if (!fin) {
        cout << "symptoms.txt not found.\n";
        return;
    }

    string line;
    while (getline(fin, line) && dbCount < MAX_DISEASE) {
        size_t c = line.find(':');
        size_t p = line.find('|');
        size_t s = line.find('/');

        if (c == string::npos || p == string::npos || s == string::npos) {
            continue;
        }

        Disease& d = db[dbCount];
        d.n = line.substr(0, c);
        string symptom = line.substr(c + 1, p - c - 1);
        string medication = line.substr(p + 1, s - p - 1);
        d.description = line.substr(s + 1);

        stringstream ss(symptom), ms(medication);
        string token;
        auto stripParens = [](const string& text) {
            string result;
            bool inParens = false;
            for (char c : text) {
                if (c == '(') {
                    inParens = true;
                } else if (c == ')') { 
                    inParens = false;
                } else if (!inParens) { 
                    result += c;
                }
            }
            return result;
        };

        while (getline(ss, token, ',') && d.symptomsCount < MAX_SYMPTOMS) {
            d.symptoms[d.symptomsCount++] = stripParens(token);
        }
        while (getline(ms, token, ',') && d.medicationsCount < MAX_MEDICATIONS) {
            d.medications[d.medicationsCount++] = token;
        }
        ++dbCount;
    }

    cout << "Hi, " << un << "! Type 'q' to exit." << endl;
    cout << "You can ask me about your symptoms or medications." << endl;

    string input;
    while (true) {
        cout << "You: ";
        getline(cin, input);
        if (input.empty()) { 
            continue;
        }

        string lower = input;
        for (char& c : lower) { 
            c = tolower(c);
        }

        if (lower == "q" || lower == "quit" || lower == "exit" || lower == "bye") {
            string reply = FAREWELL[bye++ % 5];
            cout << reply << endl;
            History::logChat(un, input, reply);
            break;
        }

        if (lower.find("hey") != string::npos || lower.find("hi") != string::npos || lower.find("hello") != string::npos || lower.find("how are you") != string::npos) {
            string reply = GREET[greet++ % 7];
            cout << reply << endl;
            History::logChat(un, input, reply);
            continue;
        }

        if (lower.find("thank") != string::npos || lower.find("thanks") != string::npos || lower.find("thank you") != string::npos) {
            string reply = THX[thx++ % 5];
            cout << reply << endl;
            History::logChat(un, input, reply);
            continue;
        }

        if (lower.find("what is") != string::npos) {
            bool found = false;
            for (int i = 0; i < dbCount; ++i) {
                string loweredDisease = db[i].n;
                for (char& c : loweredDisease) {
                    c = tolower(c);
                }
                if (lower.find(loweredDisease) != string::npos) {
                    string reply = "\n" + db[i].n + " - " + db[i].description + "\n";
                    cout << reply << endl;
                    History::logChat(un, input, reply);
                    found = true;
                    break;
                }
            }
            if (!found) {
                string reply = "I'm not sure which condition you're referring to.\n";
                cout << reply;
                History::logChat(un, input, reply);
            }
            continue;
        }

        if (lower.find("add") == 0) {
            bool handled = false;
            for (int i = 0; i < dbCount; i++) {
                string lowered = db[i].n;
                for (char& c : lowered) {
                    c = tolower(c);
                }
                if (lower.find(lowered) != string::npos) {
                    istringstream cmd(input);
                    string word;
                    int medsInd[MAX_MEDICATIONS], idxCount = 0;
                    while (cmd >> word) {
                        if (isdigit(word[0]) && idxCount < MAX_MEDICATIONS) {
                            medsInd[idxCount++] = stoi(word) - 1;
                        }
                    }

                    cout << "Added from " << db[i].n << ":\n";
                    string addedMeds[MAX_MEDICATIONS];
                    int addedCount = 0;
                    for (int k = 0; k < idxCount; k++) {
                        if (medsInd[k] >= 0 && medsInd[k] < db[i].medicationsCount) {
                            string med = db[i].medications[medsInd[k]];
                            cout << "- " << med << "\n";
                            addedMeds[addedCount++] = med;
                        }
                    }

                    if (addedCount > 0) {
                        medLog.addToHistoryDirectly(db[i].n, addedMeds, addedCount);
                    }
                    History::logChat(un, input, "Added medications from " + db[i].n);
                    handled = true;
                    break;
                }
            }
            if (!handled) {
                cout << "No matching condition found to add medications.\n";
            }
            continue;
        }

        string sym[50];
        int symCount = 0;
        istringstream iss(lower);
        string word;
        while (iss >> word && symCount < 50) {
            sym[symCount++] = word;
        }

        bool foundExact = false;
        for (int i = 0; i < dbCount; i++) {
            if (db[i].symptomsCount != symCount) { 
                continue;
            }
            bool allMatch = true;
            for (int s = 0; s < symCount; s++) {
                bool matched = false;
                for (int d = 0; d < db[i].symptomsCount; d++) {
                    if (sym[s] == db[i].symptoms[d]) {
                        matched = true;
                        break;
                    }
                }
                if (!matched) {
                    allMatch = false;
                    break;
                }
            }
            if (allMatch) {
                cout << "\nExact match: " << db[i].n << "\nMedications: ";
                for (int m = 0; m < db[i].medicationsCount; m++) {
                    cout << db[i].medications[m];
                    if (m < db[i].medicationsCount - 1) {
                        cout << ", ";
                    }
                }
                cout << "\n";
                History::logChat(un, input, "Exact match: " + db[i].n);
                foundExact = true;
                break;
            }
        }
        if (foundExact) {
            continue;
        }

        int matchIdx[MAX_DISEASE] = {}, matchScores[MAX_DISEASE] = {}, totalMatches = 0;
        for (int i = 0; i < dbCount; i++) {
            int score = 0;
            for (int s = 0; s < db[i].symptomsCount; s++) {
                for (int u = 0; u < symCount; u++) {
                    istringstream symStream(db[i].symptoms[s]);
                    string symWord;
                    while (symStream >> symWord) {
                        if (levenshtein(symWord, sym[u]) <= LEVENSHTEIN) {
                            ++score;
                            break;
                        }
                    }
                }
            }
            if (score >= 1) {
                matchIdx[totalMatches] = i;
                matchScores[totalMatches] = score;
                ++totalMatches;
            }
        }

        if (totalMatches == 0) {
            cout << "No match found yet. Please continue.\n";
            continue;
        }

        for (int i = 0; i < totalMatches - 1; i++) {
            for (int j = i + 1; j < totalMatches; j++) {
                if (matchScores[i] < matchScores[j]) {
                    swap(matchScores[i], matchScores[j]);
                    swap(matchIdx[i], matchIdx[j]);
                }
            }
        }
        for (int i = 0; i < min(3, totalMatches); i++) {
            Disease& d = db[matchIdx[i]];
            cout << "\nIt might be: " << d.n << "\nMedications: ";
            for (int m = 0; m < d.medicationsCount; m++) {
                cout << d.medications[m];
                if (m < d.medicationsCount - 1) {
                    cout << ", ";
                }
            }
            cout << "\n";
            History::logChat(un, input, "Match: " + d.n);
        }
    }
}
