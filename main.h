#ifndef JAKDOJADE_MAIN_H
#define JAKDOJADE_MAIN_H

#include <stdio.h>
#include <stdlib.h>

#define COORDINATES_SIZE 2
#define NUMBER_OF_DIRECTIONS 4
#define X_COORDINATE 0
#define Y_COORDINATE 1

typedef struct city {
    char *name;
    int x;
    int y;
    int neighboursCount;
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
    int **visited;
    int **distances;
    city **cities;
} map;

typedef struct node {
    struct node *next;
    int x;
    int y;
} node;

typedef struct queue {
    node *head;
    node *tail;
} queue;

void initializeMap(map *map);

void findCities(map *map);

void findNames(map *map);

void enqueue(int x, int y, queue *queue);

void dequeue(queue *queue);

int isEmpty(queue *queue);

void bfs(city *sourceCity, map *map);

void clear(map *map);

city *findCity(int x, int y, map *map);

neighbour *findNeighbour(int x, int y, city *city);

void deallocateMemory(map *map);

#endif //JAKDOJADE_MAIN_H
