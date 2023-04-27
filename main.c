#include <stdio.h>
#include <stdlib.h>

#define COORDINATES_SIZE 2
#define X_COORDINATE 0
#define Y_COORDINATE 1

typedef struct city {
    char *name;
    int x;
    int y;
    struct city **neighbours;
} city;

int main() {
    int width, height;

    // Input width and height
    scanf(" %d", &width);
    scanf(" %d", &height);

    // Create a map
    char input;
    // https://linuxhint.com/two-dimensional-array-malloc-c-programming/
    char *map = (char *) calloc((height * width), sizeof(char));

    // Create cities coordinates array
    int citiesCount = 0;
    const int maximalCitiesCount = height * width;
    int *citiesCoordinates = (int *) calloc((maximalCitiesCount * COORDINATES_SIZE), sizeof(int));

    // Input the map data
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

    // We need to add citiesCount to the size of the array to account for the null terminator
    const int maximalCityNameLength = width + citiesCount;

    char *citiesNames = (char *) calloc((citiesCount * maximalCityNameLength), sizeof(char));

    for (int i = 0; i < citiesCount; i++) {
        int x = *(citiesCoordinates + i * COORDINATES_SIZE + X_COORDINATE);
        int y = *(citiesCoordinates + i * COORDINATES_SIZE + Y_COORDINATE);
        int cityNameX = x;
        int cityNameY = y;
        for (int cityNameYTemporary = y - 1; cityNameYTemporary <= y + 1; cityNameYTemporary++) {
            int stop = 0;
            for (int cityNameXTemporary = x - 1; cityNameXTemporary <= x + 1; cityNameXTemporary++) {
                if (cityNameXTemporary == x && cityNameYTemporary == y) {
                    continue;
                }
                if (cityNameXTemporary < 0 || cityNameXTemporary >= width || cityNameYTemporary < 0 ||
                    cityNameYTemporary >= height) {
                    continue;
                }
                if (('A' <= *(map + cityNameYTemporary * width + cityNameXTemporary) &&
                     *(map + cityNameYTemporary * width + cityNameXTemporary) <= 'Z') ||
                    ('0' <= *(map + cityNameYTemporary * width + cityNameXTemporary) &&
                     *(map + cityNameYTemporary * width + cityNameXTemporary) <= '9')) {

                    // Move left until we find the first letter
                    while (cityNameXTemporary - 1 >= 0 &&
                           ('A' <= *(map + cityNameYTemporary * width + cityNameXTemporary - 1) &&
                            *(map + cityNameYTemporary * width + cityNameXTemporary - 1) <= 'Z') ||
                           ('0' <= *(map + cityNameYTemporary * width + cityNameXTemporary - 1) &&
                            *(map + cityNameYTemporary * width + cityNameXTemporary - 1) <= '9')) {
                        cityNameXTemporary--;
                    }

                    cityNameX = cityNameXTemporary;
                    cityNameY = cityNameYTemporary;

                    stop = 1;
                    break;
                }
            }
            if (stop) {
                break;
            }
        }
        // TODO: Make this a function
        // Uncover city name
        int j = 0;
        while (cityNameX < width &&
               ('A' <= *(map + cityNameY * width + cityNameX) && *(map + cityNameY * width + cityNameX) <= 'Z') ||
               ('0' <= *(map + cityNameY * width + cityNameX) && *(map + cityNameY * width + cityNameX) <= '9')) {
            *(citiesNames + i * maximalCityNameLength + j) = *(map + cityNameY * width + cityNameX);
            j++;
            cityNameX++;
        }
        *(citiesNames + i * maximalCityNameLength + j) = '\0';
    }

    int **cityMaps = (int **) calloc((citiesCount * (height * width)), sizeof(int));


    for (int i = 0; i < citiesCount; i++) {

    }

//    // Output the map
//    for (int y = 0; y < height; y++) {
//        for (int x = 0; x < width; x++) {
//            printf("%c", *(map + y * width + x));
//        }
//        printf("\n");
//    }
//
    // Output city names and their coordinates
    for (int i = 0; i < citiesCount; i++) {
        printf("%s (%d, %d)\n", citiesNames + i * maximalCityNameLength,
               *(citiesCoordinates + i * COORDINATES_SIZE + X_COORDINATE),
               *(citiesCoordinates + i * COORDINATES_SIZE + Y_COORDINATE));
    }

    free(map);
    free(citiesCoordinates);
    free(citiesNames);
    free(cityMaps);

    return 0;
}
