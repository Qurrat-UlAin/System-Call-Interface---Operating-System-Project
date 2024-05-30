//Contributed by Fariya:
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

int main() {
    pid_t pid;

    // Fork a child process
    pid = fork();

    if (pid < 0) {
        std::cerr << "Fork failed" << std::endl;
        return 1;
    }
    else if (pid == 0) {
        // Child process
        std::cout << "Child process: " << std::endl;
        std::cout << "PID: " << getpid() << std::endl;
        std::cout << "Parent PID: " << getppid() << std::endl;

        // Execute a new program (e.g., /bin/ls)
        execl("/bin/ls", "ls", (char *)0);

        // If execl returns, it must have failed
        std::cerr << "Exec failed" << std::endl;
        return 1;
    }
    else {
        // Parent process
        std::cout << "Parent process: " << std::endl;
        std::cout << "PID: " << getpid() << std::endl;

        // Wait for the child process to complete
        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status)) {
            std::cout << "Child exited with status: " << WEXITSTATUS(status) << std::endl;
        } else {
            std::cerr << "Child process did not terminate normally" << std::endl;
        }
        // kill(getpid());
    }

    return 0;
}
