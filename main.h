#ifndef JAKDOJADE_MAIN_H
#define JAKDOJADE_MAIN_H

#include <stdio.h>
#include <stdlib.h>

#define COORDINATES_SIZE 2
#define NUMBER_OF_DIRECTIONS 4
#define BUFFER_SIZE 64
#define X_COORDINATE 0
#define Y_COORDINATE 1

typedef struct city {
    char *name;
    int index;
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
    int citiesCount;
    int flightsCount;
    char **mapVisualisation;
    int maximalCityNameLength;
    city **cities;
} map;

typedef struct graph {
    int **visited;
    int **distances;
    int **adjacencyMatrix;
} graph;

typedef struct node {
    struct node *next;
    int x;
    int y;
} node;

typedef struct queue {
    node *head;
    node *tail;
} queue;

void initializeMapGraph(map *map, graph *graph);

void findCities(map *map);

void findNames(map *map);

void enqueue(int x, int y, queue *queue);

void dequeue(queue *queue);

int isEmpty(queue *queue);

void bfs(city *sourceCity, map *map, graph *graph);

void clear(map *map, graph *graph);

city *findCity(int x, int y, map *map);

neighbour *findNeighbour(int x, int y, city *city);

void inputFlights(map *map);

void initializeAdjacencyMatrix(map *map, graph *graph);

void fillAdjacencyMatrix(map *map, graph *graph);

void deallocateMemory(map *map, graph *graph);

#endif //JAKDOJADE_MAIN_H
