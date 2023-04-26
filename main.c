#include <stdio.h>
#include <stdlib.h>

int main() {
    int width, height;

    // Input width and height
    scanf(" %d", &width);
    scanf(" %d", &height);

    // Create a map
    char input;
    // https://linuxhint.com/two-dimensional-array-malloc-c-programming/
    char *arrayOfChars = (char *)malloc((width * height) * sizeof(char));

    
    // Input the data
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            scanf(" %c", &input);
            *(arrayOfChars + y * width + x) = input;
        }
    }

    // Output the data
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            printf("%c", *(arrayOfChars + y * width + x));
        }
        printf("\n");
    }

    free(arrayOfChars);
    return 0;
}
