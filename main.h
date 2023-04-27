#ifndef JAKDOJADE_MAIN_H
#define JAKDOJADE_MAIN_H

#include <stdio.h>
#include <stdlib.h>

#define COORDINATES_SIZE 2
#define X_COORDINATE 0
#define Y_COORDINATE 1

typedef struct city {
    char *name;
    int x;
    int y;
    struct neighbour **neighbours;
} city;

typedef struct neighbour {
    city *city;
    int distance;
} neighbour;

typedef struct map {
    int width;
    int height;
    int maximalCityNameLength;
    int citiesCount;
    char **mapVisualisation;
    city **cities;
} map;

typedef struct node {
    struct node *next;
    int x;
    int y;
} node;

void initializeMap(map *map);

void findCities(map *map);

void findNames(map *map);

void deallocateMemory(map *map);

#endif //JAKDOJADE_MAIN_H
