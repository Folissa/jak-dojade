#include <stdio.h>
#include <stdlib.h>

#define COORDINATES_SIZE 2
#define X_COORDINATE 0
#define Y_COORDINATE 1

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

    // Create cities coordinates array
    int citiesCount = 0;
    int *citiesCoordinates = (int *)malloc(((height * width) * COORDINATES_SIZE) * sizeof(int));

    // Input the data
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            scanf(" %c", &input);
            *(map + y * width + x) = input;
            // If this is a city, add its coordinates to the citiesCoordinates array
            if (input == '*') {
                *(citiesCoordinates + citiesCount * COORDINATES_SIZE + X_COORDINATE) = x;
                *(citiesCoordinates + citiesCount * COORDINATES_SIZE + Y_COORDINATE) = y;
                citiesCount++;
            }
        }
    }

    const int maximalCityNameLength = width;

    // Output the data
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            printf("%c", *(map + y * width + x));
        }
        printf("\n");
    }

    // Output the coordinates of the cities
    for (int i = 0; i < citiesCount; i++) {
        printf("(%d, %d)\n", *(citiesCoordinates + i * COORDINATES_SIZE + X_COORDINATE), *(citiesCoordinates + i * COORDINATES_SIZE + Y_COORDINATE));
    }

    free(map);
    return 0;
}
