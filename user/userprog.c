#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define DEVICE_PATH "/dev/factorialdev"

int main() {
    int fd;
    char input[256];
    char output[256];
    ssize_t bytes_read;

    // Відкриття пристрою
    fd = open(DEVICE_PATH, O_RDWR);
    if (fd < 0) {
        perror("Failed to open the device");
        return 1;
    }

    // Ввід числа
    printf("Enter a number to calculate its factorial: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0; // Видалити новий рядок

    // Запис числа до модуля
    if (write(fd, input, strlen(input)) < 0) {
        perror("Failed to write to the device");
        close(fd);
        return 1;
    }

    // Зчитування результату
    bytes_read = read(fd, output, sizeof(output) - 1);
    if (bytes_read < 0) {
        perror("Failed to read from the device");
        close(fd);
        return 1;
    }
    output[bytes_read] = '\0';

    printf("Result from kernel: %s\n", output);

    // Закриття пристрою
    close(fd);
    return 0;
}
