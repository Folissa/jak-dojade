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
#define MAXIMUM_CITY_NAME_LENGTH 32
#define INFINITY 2000000000

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
    int queriesCount;
    char **mapVisualisation;
    int maximalCityNameLength;
    city **cities;
} map;

typedef struct graph {
    int **visited;
    int **distances;
    struct result *results;
    int *totalDistance;
    city **previous;
} graph;

typedef struct result {
    int *totalDistance;
    city **previous;
} result;

typedef struct queueNode {
    struct queueNode *next;
    int x;
    int y;
} queueNode;

typedef struct queue {
    struct queueNode *head;
    struct queueNode *tail;
} queue;

typedef struct priorityQueueNode {
    city *city;
    int priority;
} priorityQueueNode;


typedef struct priorityQueue {
    priorityQueueNode **queue;
    int reachedSize;
    int size;
    int maxSize;
} priorityQueue;

void initializeMapGraph(map *map, graph *graph);

void findCities(map *map, graph *graph, hashTable *table);

void findNames(map *map, hashTable *table);

int isAlphaNumeric(map *map, int x, int y);

void enqueue(int x, int y, queue *queue);

void dequeue(queue *queue);

int isQueueEmpty(queue *queue);

void bfs(city *sourceCity, map *map, graph *graph);

void addNeighbour(city *sourceCity, city *neighbourCity, int distance);

void clear(city *sourceCity, map *map, graph *graph);

city *findCity(int x, int y, map *map);

neighbour *findNeighbour(int x, int y, city *city);

void freeNeighbours(city *sourceCity);

void inputFlights(map *map, hashTable *table);

void inputQueries(map *map, graph *graph, hashTable *table);

//void printPath(int stopper, result *result, city *source, city *destination);
void printPath(int stopper, city **visited, city *source, city *destination);

int hash(const char *string);

void insertCity(city *city, hashTable *table);

city *lookupCity(char *name, hashTable *table);

void freeCities(cityNode *city);

int parent(int index);

int left(int index);

int right(int index);

void heapify(int index, priorityQueue *queue);

city *heapGetMin(priorityQueue *queue);

void initializePriorityQueue(priorityQueue *queue, map *map);

int isPriorityQueueEmpty(priorityQueue *queue);

void addWithPriority(city *city, int priority, priorityQueue *queue);

void decreasePriority(city *city, int priority, priorityQueue *queue);

int contains(city *city, priorityQueue *queue);

void heapInsert(city *city, int priority, priorityQueue *queue);

void freePriorityQueue(priorityQueue *queue);

void dijkstra(city *source, map *map, graph *graph);

void deallocateMemory(map *map, graph *graph, hashTable *table);

#endif //JAKDOJADE_MAIN_H
