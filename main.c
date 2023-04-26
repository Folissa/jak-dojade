#include <stdio.h>
#include <stdlib.h>

#define COORDINATES_SIZE 2
#define EMPTY_INT (-1)

int main() {
    int width, height;

    // Input width and height
    scanf(" %d", &width);
    scanf(" %d", &height);

    // Create a map
    char input;
    // https://linuxhint.com/two-dimensional-array-malloc-c-programming/
    char *map = (char *)malloc((height * width) * sizeof(char));

    int citiesCount = 0;
    int *citiesCoordinates = (int *)malloc(((height * width) * COORDINATES_SIZE) * sizeof(int));

    // Input the data
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            scanf(" %c", &input);
            *(map + y * width + x) = input;
            // If this is a city
            if (input == '*') {
                *(citiesCoordinates + citiesCount * COORDINATES_SIZE + 0) = x;
                *(citiesCoordinates + citiesCount * COORDINATES_SIZE + 1) = y;
                citiesCount++;
            }
        }
    }

    // Output the data
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            printf("%c", *(map + y * width + x));
        }
        printf("\n");
    }

    for (int i = 0; i < citiesCount; i++) {
        printf("(%d, %d)\n", *(citiesCoordinates + i * COORDINATES_SIZE + 0), *(citiesCoordinates + i * COORDINATES_SIZE + 1));
    }

    free(map);
    return 0;
}
