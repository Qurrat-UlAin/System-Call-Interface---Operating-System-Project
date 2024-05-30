//This file has all four modules combined:

#include <iostream>
#include <vector>
#include <algorithm> // for sort
#include <cstdlib>
#include <cstdio> // for C-style I/O functions
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <cstring>
#include <sys/stat.h>
#include <fcntl.h>

using namespace std;

// Memory Management
class MemoryPool {
public:
    MemoryPool(size_t size) : poolSize(size), pool(new char[size]) {
        freeList.push_back({0, size});
    }

    ~MemoryPool() {
        delete[] pool;
    }

    void* customMalloc(size_t size);
    void customFree(void* ptr);

private:
    struct Block {
        size_t start;
        size_t size;
    };

    size_t poolSize;
    char* pool;
    vector<Block> freeList;

    void mergeFreeBlocks();
};

void MemoryPool::mergeFreeBlocks() {
    sort(freeList.begin(), freeList.end(), [](const Block& a, const Block& b) {
        return a.start < b.start;
    });

    for (auto it = freeList.begin(); it != freeList.end() - 1; ++it) {
        auto nextIt = it + 1;
        if (it->start + it->size == nextIt->start) {
            it->size += nextIt->size;
            freeList.erase(nextIt);
            --it;
        }
    }
}

void* MemoryPool::customMalloc(size_t size) {
    for (auto it = freeList.begin(); it != freeList.end(); ++it) {
        if (it->size >= size) {
            size_t start = it->start;
            if (it->size > size) {
                it->start += size;
                it->size -= size;
            } else {
                freeList.erase(it);
            }
            return pool + start;
        }
    }
    return nullptr; // Out of memory
}

void MemoryPool::customFree(void* ptr) {
    size_t start = static_cast<char*>(ptr) - pool;
    freeList.push_back({start, poolSize - start});
    mergeFreeBlocks();
}

void memoryManagement() {
    MemoryPool memoryPool(1024); // 1 KB memory pool

    void* ptr1 = memoryPool.customMalloc(128); // Allocate 128 bytes
    void* ptr2 = memoryPool.customMalloc(256); // Allocate 256 bytes

    cout << "ptr1: " << ptr1 << endl;
    cout << "ptr2: " << ptr2 << endl;

    memoryPool.customFree(ptr1); // Free the 128 bytes block
    memoryPool.customFree(ptr2); // Free the 256 bytes block
}

/////////////////////////
// Process Management  //
/////////////////////////
void processManagement() {
    pid_t pid;

    // Fork a child process
    pid = fork();

    if (pid < 0) {
        cerr << "Fork failed" << endl;
    } else if (pid == 0) {
        // Child process
        cout << "Child process: " << endl;
        cout << "PID: " << getpid() << endl;
        cout << "Parent PID: " << getppid() << endl;

        // Execute a new program (e.g., /bin/ls)
        execl("/bin/ls", "ls", (char *)0);

        // If execl returns, it must have failed
        cerr << "Exec failed" << endl;
    } else {
        // Parent process
        cout << "Parent process: " << endl;
        cout << "PID: " << getpid() << endl;

        // Wait for the child process to complete
        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status)) {
            cout << "Child exited with status: " << WEXITSTATUS(status) << endl;
        } else {
            cerr << "Child process did not terminate normally" << endl;
        }
    }
}

// File Operations
void fileOperations() {
    int choice;
    char filename[50];
    char data[100];
    FILE* filePtr = nullptr;

    while (true) {
        cout << "File Operations:" << endl;
        cout << "1. Open a file stream (fopen)" << endl;
        cout << "2. Close a file stream (fclose)" << endl;
        cout << "3. Write formatted data to a file stream (fprintf)" << endl;
        cout << "4. Write data to a file stream (fwrite)" << endl;
        cout << "5. Read data from a file stream (fread)" << endl;
        cout << "6. Go Back to Main Menu" << endl;
        cout << "Enter your choice (1-6): ";
        cin >> choice;

        switch (choice) {
        case 1:
            // Open a file stream using fopen
            cout << "Enter filename to open: ";
            cin >> filename;
            filePtr = fopen(filename, "w");
            if (!filePtr) {
                perror("Error opening file");
            } else {
                cout << "File stream opened successfully." << endl;
                fclose(filePtr);
            }
            break;

        case 2:
            // Close a file stream
            cout << "Enter filename to close: ";
            cin >> filename;
            filePtr = fopen(filename, "r");
            if (!filePtr) {
                perror("Error opening file");
            } else {
                fclose(filePtr);
                cout << "File stream closed successfully." << endl;
            }
            break;

        case 3:
            // Write formatted data to a file stream
            cout << "Enter filename to write to: ";
            cin >> filename;
            cout << "Enter formatted data to write: ";
            cin.ignore(); // Ignore newline character in buffer
            cin.getline(data, sizeof(data));
            filePtr = fopen(filename, "a");
            if (!filePtr) {
                perror("Error opening file");
            } else {
                fprintf(filePtr, "%s\n", data);
                fclose(filePtr);
                cout << "Formatted data written to file stream successfully." << endl;
            }
            break;

        case 4:
            // Write data to a file stream
            cout << "Enter filename to write to: ";
            cin >> filename;
            cout << "Enter data to write: ";
            cin.ignore(); // Ignore newline character in buffer
            cin.getline(data, sizeof(data));
            filePtr = fopen(filename, "a");
            if (!filePtr) {
                perror("Error opening file");
            } else {
                fwrite(data, sizeof(char), strlen(data), filePtr);
                fclose(filePtr);
                cout << "Data written to file stream successfully." << endl;
            }
            break;

        case 5:
            // Read data from a file stream
            cout << "Enter filename to read from: ";
            cin >> filename;
            filePtr = fopen(filename, "r");
            if (!filePtr) {
                perror("Error opening file");
            } else {
                char buffer[100];
                while (fgets(buffer, sizeof(buffer), filePtr) != NULL) {
                    cout << buffer;
                }
                fclose(filePtr);
            }
            break;

        case 6:
            // Go back to main menu
            return;

        default:
            cout << "Invalid choice. Please enter a number between 0 and 6." << endl;
            break;
        }
    }
}

/////////////////////////
// Custom Syscalls     //
/////////////////////////
#include <limits>  // Include this header for std::numeric_limits

#define SYS_reverse_string 442
#define SYS_helloworld 441

// Function to call the reverse string syscall
void reverseStringSyscall(const char* input, char* output) {
    long result = syscall(SYS_reverse_string, input, output);
    if (result == 0) {
        cout << "Reversed string: " << output << endl;
    } else {
        cerr << "Reverse string syscall failed with error code " << result << endl;
    }
}

// Function to call the hello world syscall
void helloWorldSyscall() {
    long int checkcall = syscall(SYS_helloworld);
    cout << "System call .: sys_helloworld :. returned " << checkcall << endl;
}

void customSyscalls() {
    char input[256];
    char output[256];
    int choice;

    cout << "Choose an option:\n1. Reverse String\n2. Hello World Syscall\n";
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear the input buffer

    switch (choice) {
    case 1:
        cout << "Enter a string: ";
        cin.getline(input, sizeof(input));
        reverseStringSyscall(input, output);
        break;
    case 2:
        helloWorldSyscall();
        break;
    default:
        cerr << "Invalid choice" << endl;
        break;
    }
}

int main() {
    int choice;
    char choicee = 'y';  // Initialize choicee to 'y'
    while (choicee == 'y' || choicee == 'Y') {
        cout << "Choose a system call type:" << endl;
        cout << "1. Process Management" << endl;
        cout << "2. Memory Management" << endl;
        cout << "3. File System Calls" << endl;
        cout << "4. Custom System Calls" << endl;
        cout << "0. Exit" << endl;
        cout << "Enter your choice (0-4): ";
        cin >> choice;

        switch (choice) {
        case 1:
            processManagement();
            break;
        case 2:
            memoryManagement();
            break;
        case 3:
            fileOperations();
            break;
        case 4:
            customSyscalls();
            break;
        case 0:
            cout << "Exiting program..." << endl;
            return 0;
        default:
            cout << "Invalid choice. Please enter a number between 0 and 4." << endl;
            break;
        }
        cout << "Explore more? [y/n]: ";
        cin >> choicee;
    }

    return 0;
}
