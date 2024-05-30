//Contributed by Hasnain
CPP CODE FOR FILE SYSTEM CALLS.


#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace std;

// File Operations
void fileOperations() {
    int choice;
    char filename[50];
    char data[100];
    FILE* filePtr = nullptr;

    while (true) {
        cout << "File Operations:" << endl;
        cout << "16. Open a file stream (fopen_s)" << endl;
        cout << "17. Close a file stream (fclose)" << endl;
        cout << "18. Write formatted data to a file stream (fprintf)" << endl;
        cout << "19. Write data to a file stream (fwrite)" << endl;
        cout << "20. Read data from a file stream (fread)" << endl;
        cout << "0. Go Back to Main Menu" << endl;
        cout << "Enter your choice (0-20): ";
        cin >> choice;

        switch (choice) {
        case 16:
            // Open a file stream using fopen_s
            cout << "Enter filename to open: ";
            cin >> filename;
            if (fopen_s(&filePtr, filename, "w") != 0) {
                perror("Error opening file");
            }
            else {
                cout << "File stream opened successfully." << endl;
                fclose(filePtr);
            }
            break;

        case 17:
            // Close a file stream
            cout << "Enter filename to close: ";
            cin >> filename;
            if (fopen_s(&filePtr, filename, "r") != 0) {
                perror("Error opening file");
            }
            else {
                fclose(filePtr);
                cout << "File stream closed successfully." << endl;
            }
            break;

        case 18:
            // Write formatted data to a file stream
            cout << "Enter filename to write to: ";
            cin >> filename;
            cout << "Enter formatted data to write: ";
            cin.ignore(); // Ignore newline character in buffer
            cin.getline(data, sizeof(data));
            if (fopen_s(&filePtr, filename, "a") != 0) {
                perror("Error opening file");
            }
            else {
                fprintf(filePtr, "%s\n", data);
                fclose(filePtr);
                cout << "Formatted data written to file stream successfully." << endl;
            }
            break;

        case 19:
            // Write data to a file stream
            cout << "Enter filename to write to: ";
            cin >> filename;
            cout << "Enter data to write: ";
            cin.ignore(); // Ignore newline character in buffer
            cin.getline(data, sizeof(data));
            if (fopen_s(&filePtr, filename, "a") != 0) {
                perror("Error opening file");
            }
            else {
                fwrite(data, sizeof(char), strlen(data), filePtr);
                fclose(filePtr);
                cout << "Data written to file stream successfully." << endl;
            }
            break;

        case 20:
            // Read data from a file stream
            cout << "Enter filename to read from: ";
            cin >> filename;
            if (fopen_s(&filePtr, filename, "r") != 0) {
                perror("Error opening file");
            }
            else {
                char buffer[100];
                while (fgets(buffer, sizeof(buffer), filePtr) != NULL) {
                    cout << buffer;
                }
                fclose(filePtr);
            }
            break;

        case 0:
            // Go back to main menu
            return;

        default:
            cout << "Invalid choice. Please enter a number between 0 and 20." << endl;
            break;
        }
    }
}

// System Manipulation
void systemManipulation() {
    cout << "System Manipulation is not available in this environment." << endl;
}

// Administration Manipulation
void adminManipulation() {
    cout << "Administration Manipulation is not available in this environment." << endl;
}

int main() {
    int choice;

    while (true) {
        cout << "Welcome to File System Calls in C++" << endl;
        cout << "Choose an option:" << endl;
        cout << "1. File Operations" << endl;
        cout << "2. System Manipulation" << endl;
        cout << "3. Administration Manipulation" << endl;
        cout << "0. Exit" << endl;
        cout << "Enter your choice (0-3): ";
        cin >> choice;

        switch (choice) {
        case 1:
            fileOperations();
            break;
        case 2:
            systemManipulation();
            break;
        case 3:
            adminManipulation();
            break;
        case 0:
            cout << "Exiting program..." << endl;
            return 0;
        default:
            cout << "Invalid choice. Please enter a number between 0 and 3." << endl;
            break;
        }
    }

    return 0;
}


