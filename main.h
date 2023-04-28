#ifndef JAKDOJADE_MAIN_H
#define JAKDOJADE_MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COORDINATES_SIZE 2
#define HASH_TABLE_MULTIPLIER 2
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
    struct neighbour *neighbours;
} city;

typedef struct neighbour {
    city *city;
    int distance;
    struct neighbour *next;
} neighbour;

typedef struct hashTable {
    int size;
    struct cityNode **cities;
} hashTable;

typedef struct cityNode {
    city *city;
    struct cityNode *next;
} cityNode;

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
    struct neighbour **adjacencyList;
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

void findCities(map *map, graph *graph, hashTable *table);

void findNames(map *map, hashTable *table);

int isAlphaNumeric(map *map, int x, int y);

void enqueue(int x, int y, queue *queue);

void dequeue(queue *queue);

int isEmpty(queue *queue);

void bfs(city *sourceCity, map *map, graph *graph);

void addNeighbour(city *sourceCity, city *neighbourCity, int distance);

void clear(city *sourceCity, map *map, graph *graph);

city *findCity(int x, int y, map *map);

neighbour *findNeighbour(int x, int y, city *city);

void freeNeighbours(city *sourceCity);

void fillAdjacencyList(map *map, graph *graph);

void inputFlights(map *map);

int hash(const char *string);

void insertCity(city *city, hashTable *table);

city *lookupCity(char *name, hashTable *table);

void deallocateMemory(map *map, graph *graph, hashTable *table);

#endif //JAKDOJADE_MAIN_H
