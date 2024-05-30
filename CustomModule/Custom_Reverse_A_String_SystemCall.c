//Contributed by me
#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <string.h>

#define SYS_reverse_string 442

int main() {
    char input[256];
    char output[256];
    
    printf("Enter a string: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0; // remove newline character

    long result = syscall(SYS_reverse_string, input, output);
    
    if (result == 0) {
        printf("Reversed string: %s\n", output);
    } else {
        printf("Syscall failed with error code %ld\n", result);
    }
    
    return 0;
}
