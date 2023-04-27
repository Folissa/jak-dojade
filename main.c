#include "main.h"

int main() {

    map map;

    initializeMap(&map);

    findCities(&map);
    findNames(&map);

    // Output cities
    for (int i = 0; i < map.citiesCount; i++) {
        printf("%s (%d, %d)\n", map.cities[i]->name, map.cities[i]->x, map.cities[i]->y);
    }

    deallocateMemory(&map);

    return 0;
}


void initializeMap(map *map) {
    // Input width and height
    scanf(" %d", &map->width);
    scanf(" %d", &map->height);

    // We need to add citiesCount for the null terminator
    map->maximalCityNameLength = map->width + map->citiesCount;

    // Create mapVisualisation
    map->mapVisualisation = (char **) calloc(map->height, sizeof(char *));
    for (int y = 0; y < map->height; y++) {
        map->mapVisualisation[y] = (char *) calloc(map->width, sizeof(char));
    }
}

void findCities(map *map) {
    map->citiesCount = 0;

    // Create cities coordinates array
    const int maximalCitiesCount = map->height * map->width;
    int **citiesCoordinates = (int **) calloc(maximalCitiesCount, sizeof(int *));
    for (int i = 0; i < maximalCitiesCount; i++) {
        citiesCoordinates[i] = (int *) calloc(COORDINATES_SIZE, sizeof(int));
    }

    char input;

    // Input the mapVisualisation data
    for (int y = 0; y < map->height; y++) {
        for (int x = 0; x < map->width; x++) {
            scanf(" %c", &input);
            map->mapVisualisation[y][x] = input;
            // If this is a city, add its coordinates to the citiesCoordinates array
            if (input == '*') {
                citiesCoordinates[map->citiesCount][X_COORDINATE] = x;
                citiesCoordinates[map->citiesCount][Y_COORDINATE] = y;
                map->citiesCount++;
            }
        }
    }
    map->cities = (city **) calloc(map->citiesCount, sizeof(city *));
    for (int i = 0; i < map->citiesCount; i++) {
        map->cities[i] = (city *) malloc(sizeof(city));
        map->cities[i]->name = (char *) calloc((map->maximalCityNameLength), sizeof(char));
        map->cities[i]->x = citiesCoordinates[i][X_COORDINATE];
        map->cities[i]->y = citiesCoordinates[i][Y_COORDINATE];
        // TODO: map->citiesCount - 1 or map->citiesCount?
        map->cities[i]->neighbours = (neighbour **) calloc((map->citiesCount - 1), sizeof(neighbour *));
    }

    for (int i = 0; i < maximalCitiesCount; i++) {
        free(citiesCoordinates[i]);
    }
    free(citiesCoordinates);
}

void findNames(map *map) {
    for (int i = 0; i < map->citiesCount; i++) {
        int x = (*(map->cities + i))->x;
        int y = (*(map->cities + i))->y;
        int cityNameX = x;
        int cityNameY = y;
        for (int cityNameYTemporary = y - 1; cityNameYTemporary <= y + 1; cityNameYTemporary++) {
            int stop = 0;
            for (int cityNameXTemporary = x - 1; cityNameXTemporary <= x + 1; cityNameXTemporary++) {
                if (cityNameXTemporary == x && cityNameYTemporary == y) {
                    continue;
                }
                if (cityNameXTemporary < 0 || cityNameXTemporary >= map->width || cityNameYTemporary < 0 ||
                    cityNameYTemporary >= map->height) {
                    continue;
                }
                if (('A' <= map->mapVisualisation[cityNameYTemporary][cityNameXTemporary] &&
                     map->mapVisualisation[cityNameYTemporary][cityNameXTemporary] <= 'Z') ||
                    ('a' <= map->mapVisualisation[cityNameYTemporary][cityNameXTemporary] &&
                     map->mapVisualisation[cityNameYTemporary][cityNameXTemporary] <= 'a') ||
                    ('0' <= map->mapVisualisation[cityNameYTemporary][cityNameXTemporary] &&
                     map->mapVisualisation[cityNameYTemporary][cityNameXTemporary] <= '9')) {

                    // Move left until we find the first letter
                    while (cityNameXTemporary - 1 >= 0 &&
                           ('A' <= map->mapVisualisation[cityNameYTemporary][cityNameXTemporary - 1] &&
                            map->mapVisualisation[cityNameYTemporary][cityNameXTemporary - 1] <= 'Z') ||
                           ('a' <= map->mapVisualisation[cityNameYTemporary][cityNameXTemporary - 1] &&
                            map->mapVisualisation[cityNameYTemporary][cityNameXTemporary - 1] <= 'z') ||
                           ('0' <= map->mapVisualisation[cityNameYTemporary][cityNameXTemporary - 1] &&
                            map->mapVisualisation[cityNameYTemporary][cityNameXTemporary - 1] <= '9')) {
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
        // Uncover city name
        int currentCharPosition = 0;
        while (cityNameX < map->width &&
               ('A' <= map->mapVisualisation[cityNameY][cityNameX] &&
                map->mapVisualisation[cityNameY][cityNameX] <= 'Z') ||
               ('a' <= map->mapVisualisation[cityNameY][cityNameX] &&
                map->mapVisualisation[cityNameY][cityNameX] <= 'z') ||
               ('0' <= map->mapVisualisation[cityNameY][cityNameX] &&
                map->mapVisualisation[cityNameY][cityNameX] <= '9')) {
            *(map->cities[i]->name + currentCharPosition) = map->mapVisualisation[cityNameY][cityNameX];
            currentCharPosition++;
            cityNameX++;
        }
        *(map->cities[i]->name + currentCharPosition) = '\0';
    }
}

void deallocateMemory(map *map) {
    for (int y = 0; y < map->height; y++) {
        free(map->mapVisualisation[y]);
    }
    free(map->mapVisualisation);
    for (int i = 0; i < map->citiesCount; i++) {
        free(map->cities[i]->name);
        free(map->cities[i]->neighbours);
        free(map->cities[i]);
    }
    free(map->cities);
}