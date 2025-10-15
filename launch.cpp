
#include "admin.h"

int main() {
    int ch;
    cout << "\n[1] Log in as Patient" << endl 
        << "[2] Log in as Doctor" << endl 
        << "[3] Log in as Admin\n" 
        << "\nPlease enter your choice and press [Enter]: ";
    cin >> ch;

    switch (ch) {
        case 1: {
            AdminPanel panel;
            panel.startAsUser();
            break;
        }
        case 2: {
            DoctorPanel doctor;
            doctor.menu();
            break;
        }
        case 3: {
            AdminPanel panel;
            panel.startAsAdmin();
            break;
        }
        default:
            cout << "Invalid choice.\n";
            break;
    }

    return 0;
}
