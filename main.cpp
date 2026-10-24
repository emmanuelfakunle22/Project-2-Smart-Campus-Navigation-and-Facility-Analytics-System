
#include <iostream>
using namespace std;

// Print Menu Function
int printMenu() {
    int choice;

    cout << "\n===== Smart Campus Navigation System =====" << endl;
    cout << " 1. View all facilities" << endl;
    cout << " 2. Search facility by ID" << endl;
    cout << " 3. View campus graph" << endl;
    cout << " 4. Display graph counts and degrees" << endl;
    cout << " 5. Find reachable buildings (BFS)" << endl;
    cout << " 6. Add department" << endl;
    cout << " 7. Search department" << endl;
    cout << " 8. Display department hierarchy" << endl;
    cout << " 9. Run preorder traversal" << endl;
    cout << " 0. Exit" << endl;
    cout << "Enter your choice: ";
    cin >> choice;

    return choice;
}

int main() {
    int usrChoice;

// Print Menu, then ask the user for their choice
    do {
        usrChoice = printMenu();

        switch (usrChoice) {
            case 1:
                cout << "Not implemented yet." << endl;
                break;

            case 2:
                cout << "Not implemented yet." << endl;
                break;

            case 3:
                cout << "Not implemented yet." << endl;
                break;

            case 4:
                cout << "Not implemented yet." << endl;
                break;

            case 5:
                cout << "Not implemented yet." << endl;
                break;

            case 6:
                cout << "Not implemented yet." << endl;
                break;

            case 7:
                cout << "Not implemented yet." << endl;
                break;

            case 8:
                cout << "Not implemented yet." << endl;
                break;

            case 9:
                cout << "Not implemented yet." << endl;
                break;

            case 0:
                cout << "Goodbye." << endl;
                break;

            default:
                cout << "Please enter a valid choice." << endl;
                break;
        }

    } while (usrChoice != 0);

    return 0;
}
