#include "main.h"

int main() {

    map map;
    graph graph;
    hashTable table;

    initializeMapGraph(&map, &graph);

    findCities(&map, &graph, &table);
    findNames(&map, &table);

    inputFlights(&map, &table);

    for (int i = 0; i < map.citiesCount; i++) {
        bfs(map.cities[i], &map, &graph);
        clear(map.cities[i], &map, &graph);
    }

    inputQueries(&map, &graph, &table);

    deallocateMemory(&map, &graph, &table);

    return 0;
}

void initializeMapGraph(map *map, graph *graph) {
    // Input width and height
    scanf("%d %d", &map->width, &map->height);

    // Create mapVisualisation
    map->mapVisualisation = (char **) calloc(map->height, sizeof(char *));
    graph->visited = (int **) calloc(map->height, sizeof(int *));
    graph->distances = (int **) calloc(map->height, sizeof(int *));
    for (int y = 0; y < map->height; y++) {
        map->mapVisualisation[y] = (char *) calloc(map->width, sizeof(char));
        graph->visited[y] = (int *) calloc(map->width, sizeof(int));
        graph->distances[y] = (int *) calloc(map->width, sizeof(int));
    }
}

void findCities(map *map, graph *graph, hashTable *table) {
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
    graph->totalDistance = (int *) calloc(map->citiesCount, sizeof(int));
    graph->previous = (city **) calloc(map->citiesCount, sizeof(city *));
    table->size = map->citiesCount * HASH_TABLE_MULTIPLIER;
    table->cities = (cityNode **) calloc(table->size, sizeof(cityNode *));
    map->maximalCityNameLength = MAXIMUM_CITY_NAME_LENGTH;

    for (int i = 0; i < map->citiesCount; i++) {
        map->cities[i] = (city *) malloc(sizeof(city));
        map->cities[i]->index = i;
        map->cities[i]->name = (char *) malloc(map->maximalCityNameLength * sizeof(char));
        memset(map->cities[i]->name, '\0', map->maximalCityNameLength);
        map->cities[i]->x = citiesCoordinates[i][X_COORDINATE];
        map->cities[i]->y = citiesCoordinates[i][Y_COORDINATE];
        map->cities[i]->neighboursCount = 0;
        map->cities[i]->neighbours = NULL;
        table->cities[i] = NULL;
    }

    for (int i = 0; i < maximalCitiesCount; i++) {
        free(citiesCoordinates[i]);
    }
    free(citiesCoordinates);
}

void findNames(map *map, hashTable *table) {
    // We need to add citiesCount for the null terminator

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
                if (isAlphaNumeric(map, cityNameXTemporary, cityNameYTemporary)) {
                    // Move left until we find the first letter
                    while (cityNameXTemporary - 1 >= 0 &&
                           isAlphaNumeric(map, cityNameXTemporary - 1, cityNameYTemporary)) {
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
        while (cityNameX < map->width && isAlphaNumeric(map, cityNameX, cityNameY)) {
            map->cities[i]->name[currentCharPosition] = map->mapVisualisation[cityNameY][cityNameX];
            currentCharPosition++;
            cityNameX++;
        }
        map->cities[i]->name[currentCharPosition] = '\0';
        insertCity(map->cities[i], table);
    }
}

int isAlphaNumeric(map *map, int x, int y) {
    return ('A' <= map->mapVisualisation[y][x] && map->mapVisualisation[y][x] <= 'Z') ||
           ('a' <= map->mapVisualisation[y][x] && map->mapVisualisation[y][x] <= 'z') ||
           ('0' <= map->mapVisualisation[y][x] && map->mapVisualisation[y][x] <= '9');
}

void enqueue(int x, int y, queue *queue) {
    queueNode *newNode = malloc(sizeof(queueNode));
    newNode->next = NULL;
    newNode->x = x;
    newNode->y = y;
    if (queue->tail == NULL) {
        queue->head = queue->tail = newNode;
        return;
    }
    queue->tail->next = newNode;
    queue->tail = newNode;
}

void dequeue(queue *queue) {
    if (queue->head == NULL) {
        return;
    }
    queueNode *temporary = queue->head;
    queue->head = queue->head->next;
    if (queue->head == NULL) {
        queue->tail = NULL;
    }
    free(temporary);
}

int isQueueEmpty(queue *queue) {
    return queue->head == NULL;
}

void bfs(city *sourceCity, map *map, graph *graph) {
    queue queue;
    queue.head = NULL, queue.tail = NULL;

    // Directions: up, right, down, left
    int directionX[] = {0, 1, 0, -1};
    int directionY[] = {-1, 0, 1, 0};

    enqueue(sourceCity->x, sourceCity->y, &queue);

    graph->visited[sourceCity->y][sourceCity->x] = 1;
    graph->distances[sourceCity->y][sourceCity->x] = 0;

    while (!isQueueEmpty(&queue)) {
        int currentX = queue.head->x;
        int currentY = queue.head->y;
        dequeue(&queue);
        for (int i = 0; i < NUMBER_OF_DIRECTIONS; i++) {
            int newX = currentX + directionX[i];
            int newY = currentY + directionY[i];
            if (newX < 0 || newX >= map->width || newY < 0 || newY >= map->height) {
                continue;
            }
            if (map->mapVisualisation[newY][newX] == '*') {
                neighbour *temporary = findNeighbour(newX, newY, sourceCity);
                if (temporary != NULL && graph->distances[currentY][currentX] + 1 < temporary->distance) {
                    temporary->distance = graph->distances[currentY][currentX] + 1;
                    graph->visited[newY][newX] = 1;
                    continue;
                } else if (temporary == NULL) {
                    addNeighbour(sourceCity, findCity(newX, newY, map), graph->distances[currentY][currentX] + 1);
                    graph->visited[newY][newX] = 1;
                    continue;
                }
            }
            if (graph->visited[newY][newX] || map->mapVisualisation[newY][newX] != '#') {
                continue;
            }
            graph->visited[newY][newX] = 1;
            graph->distances[newY][newX] = graph->distances[currentY][currentX] + 1;
            enqueue(newX, newY, &queue);
        }
    }
}

void addNeighbour(city *sourceCity, city *neighbourCity, int distance) {
    neighbour *newNeighbour = malloc(sizeof(neighbour));
    newNeighbour->city = neighbourCity;
    newNeighbour->distance = distance;
    newNeighbour->next = sourceCity->neighbours;
    sourceCity->neighbours = newNeighbour;
    sourceCity->neighboursCount++;
}

neighbour *findNeighbour(int x, int y, city *city) {
    neighbour *current = city->neighbours;
    while (current != NULL) {
        if (current->city->x == x && current->city->y == y) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void freeNeighbours(city *sourceCity) {
    neighbour *currentNeighbour = sourceCity->neighbours;
    while (currentNeighbour != NULL) {
        neighbour *nextNeighbour = currentNeighbour->next;
        free(currentNeighbour);
        currentNeighbour = nextNeighbour;
    }
    sourceCity->neighbours = NULL;
    sourceCity->neighboursCount = 0;
}

void clear(city *sourceCity, map *map, graph *graph) {
    queue queue;
    queue.head = NULL, queue.tail = NULL;

    // Directions: up, right, down, left
    int directionX[] = {0, 1, 0, -1};
    int directionY[] = {-1, 0, 1, 0};

    enqueue(sourceCity->x, sourceCity->y, &queue);

    graph->visited[sourceCity->y][sourceCity->x] = 0;
    graph->distances[sourceCity->y][sourceCity->x] = 0;

    while (!isQueueEmpty(&queue)) {
        int currentX = queue.head->x;
        int currentY = queue.head->y;
        dequeue(&queue);
        for (int i = 0; i < NUMBER_OF_DIRECTIONS; i++) {
            int newX = currentX + directionX[i];
            int newY = currentY + directionY[i];
            if (newX < 0 || newX >= map->width || newY < 0 || newY >= map->height) {
                continue;
            }
            if (graph->visited[newY][newX]) {
                graph->visited[newY][newX] = 0;
                graph->distances[newY][newX] = 0;
                enqueue(newX, newY, &queue);
            }
        }
    }
}

// TODO: Optimize this function
city *findCity(int x, int y, map *map) {
    for (int i = 0; i < map->citiesCount; i++) {
        if (map->cities[i]->x == x && map->cities[i]->y == y) {
            return map->cities[i];
        }
    }
    return NULL;
}

void inputFlights(map *map, hashTable *table) {
    map->flightsCount = 0;

    scanf("%d ", &map->flightsCount);

    char buffer[BUFFER_SIZE];
    int i = 0;
    while (i < map->flightsCount && fgets(buffer, BUFFER_SIZE, stdin)) {
        // https://www.geeksforgeeks.org/strtok-strtok_r-functions-c-examples/
        i++;
        char *token = strtok(buffer, " ");
        city *source = lookupCity(token, table);
        token = strtok(NULL, " ");
        city *destination = lookupCity(token, table);
        token = strtok(NULL, "\n");
        int distance = atoi(token);
        addNeighbour(source, destination, distance);
    }
}

void inputQueries(map *map, graph *graph, hashTable *table) {
    map->queriesCount = 0;

    scanf("%d ", &map->queriesCount);

    char buffer[BUFFER_SIZE];
    int i = 0;
    while (i < map->queriesCount && fgets(buffer, BUFFER_SIZE, stdin)) {
        i++;
        char *token = strtok(buffer, " ");
        city *source = lookupCity(token, table);

        token = strtok(NULL, " ");
        city *destination = lookupCity(token, table);

        token = strtok(NULL, " \n");
        int type = atoi(token);

        dijkstra(source, map, graph);

        if (type == 0) {
            int distance = graph->totalDistance[destination->index];
            printf("%d\n", distance);
        } else if (type == 1) {
            int distance = graph->totalDistance[destination->index];
            printf("%d ", distance);
            printPath(0, graph->previous, source, destination);
            printf("\n");
        }
    }
}

void printPath(int stopper, city **previous, city *source, city *destination) {
    stopper++;
    if (destination->index == source->index) {
        return;
    }
    printPath(stopper, previous, source, previous[destination->index]);
    if (stopper != 1)
        printf("%s ", destination->name);
}

//int hash(const char *string) {
//    int key = 0, i = 0;
//
//    while (string[i] != '\0') {
//        key += string[i];
//        i++;
//    }
//    return key;
//}

int power(int base, int power) {
    int i, result = 1;

    for (i = 0; i < power; ++i)
        result *= base;

    return result;
}

int hash(const char *string) {
    int digitGroup = 5;
    int base = 'Z' - 'A' + 1;
    int digitPosition = 0;
    int key = 0;
    for (int i = 0; string[i] != '\0'; i++) {
        int charFromString = toupper(string[i]);
        if (charFromString >= 'A' && charFromString <= 'Z') {
            int digit = charFromString - 'A';
            key += digit * power(base, digitPosition);
            digitPosition = (digitPosition + 1) % (digitGroup + 1);
            }
        }
    return key;
}

void insertCity(city *city, hashTable *table) {
    unsigned long hashValue = hash(city->name) % table->size;
    cityNode *node = table->cities[hashValue];

    if (node == NULL) {
        node = (cityNode *) malloc(sizeof(cityNode));
        node->city = city;
        node->next = NULL;
        table->cities[hashValue] = node;
    } else {
        while (node->next != NULL) {
            node = node->next;
        }
        cityNode *newNode = (cityNode *) malloc(sizeof(cityNode));
        newNode->city = city;
        newNode->next = NULL;
        node->next = newNode;
    }
}

city *lookupCity(char *name, hashTable *table) {
    unsigned long hashValue = hash(name) % table->size;
    cityNode *node = table->cities[hashValue];

    while (node != NULL) {
        if (strcmp(node->city->name, name) == 0) {
            return node->city;
        }

        node = node->next;
    }
    return NULL;
}

void freeCities(cityNode *city) {
    cityNode *currentCity = city;
    while (currentCity != NULL) {
        cityNode *nextCity = currentCity->next;
        free(currentCity);
        currentCity = nextCity;
    }
}

int parent(int index) {
    return index / 2;
}

int left(int index) {
    return index * 2;
}

int right(int index) {
    return index * 2 + 1;
}

void heapify(int index, priorityQueue *queue) {
    int leftIndex = left(index);
    int rightIndex = right(index);
    int smallestIndex = index;
    if (leftIndex <= queue->size && queue->queue[leftIndex - 1]->priority < queue->queue[index - 1]->priority) {
        smallestIndex = leftIndex;
    }
    if (rightIndex <= queue->size && queue->queue[rightIndex - 1]->priority < queue->queue[smallestIndex - 1]->priority) {
        smallestIndex = rightIndex;
    }
    if (smallestIndex != index) {
        priorityQueueNode *temporary = queue->queue[index - 1];
        queue->queue[index - 1] = queue->queue[smallestIndex - 1];
        queue->queue[smallestIndex - 1] = temporary;
        heapify(smallestIndex, queue);
    }
}

city *heapGetMin(priorityQueue *queue) {
    if (queue->size < 1)
        return NULL;
    else {
        city *minimumCity = queue->queue[0]->city;
        free(queue->queue[0]);
        queue->queue[0] = queue->queue[queue->size - 1];
        queue->size = queue->size - 1;
        // Heapify from the root
        heapify(1, queue);
        return minimumCity;
    }
}

void initializePriorityQueue(priorityQueue *queue, map *map) {
    queue->size = 0;
    queue->maxSize = map->citiesCount;
    queue->queue = (priorityQueueNode **) calloc(queue->maxSize, sizeof(priorityQueueNode *));
}

int isPriorityQueueEmpty(priorityQueue *queue) {
    return queue->size ==  0;
}

void addWithPriority(city *city, int priority, priorityQueue *queue) {
    if (queue->size >= queue->maxSize) {
        return;
    }
    queue->size = queue->size + 1;
    priorityQueueNode *node = malloc(sizeof(priorityQueueNode));
    node->city = city;
    node->priority = priority;

    int index = queue->size;
    while (index > 1 && queue->queue[parent(index) - 1]->priority > priority) {
        queue->queue[index - 1] = queue->queue[parent(index) - 1];
        index = parent(index);
    }

    queue->queue[index - 1] = node;
}

void decreasePriority(city *city, int priority, priorityQueue *queue) {
    for (int i = 0; i < queue->size; i++) {
        if (queue->queue[i]->city == city) {
            if (priority < queue->queue[i]->priority) {
                queue->queue[i]->priority = priority;
                heapify(1, queue);
            }
            return;
        }
    }
}

int contains(city *city, priorityQueue *queue) {
    for (int i = 0; i < queue->size; i++) {
        if (queue->queue[i]->city == city) {
            return 1;
        }
    }
    return 0;
}

void heapInsert(city *city, int priority, priorityQueue *queue) {
    if (contains(city, queue)) {
        decreasePriority(city, priority, queue);
    } else {
        addWithPriority(city, priority, queue);
    }
}

void freePriorityQueue(priorityQueue *queue) {
    free(queue->queue);
}

void dijkstra(city *source, map *map, graph *graph) {
    priorityQueue queue;
    initializePriorityQueue(&queue, map);

    graph->totalDistance[source->index] = 0;

    for (int i = 0; i < map->citiesCount; i++) {
        if (i != source->index) {
            graph->totalDistance[i] = INF;
            graph->previous[i] = NULL;
        }
        heapInsert(map->cities[i], graph->totalDistance[i], &queue);
    }

    while (!isPriorityQueueEmpty(&queue)) {
        city *minimum = heapGetMin(&queue);
        if (minimum == NULL) {
            break;
        }
        neighbour *currentNeighbor = minimum->neighbours;
        while (currentNeighbor != NULL) {
            int temporaryDistance = graph->totalDistance[minimum->index] + currentNeighbor->distance;
            if (temporaryDistance < graph->totalDistance[currentNeighbor->city->index]) {
                graph->totalDistance[currentNeighbor->city->index] = temporaryDistance;
                graph->previous[currentNeighbor->city->index] = minimum;
                heapInsert(currentNeighbor->city, temporaryDistance, &queue);
            }

            currentNeighbor = currentNeighbor->next;
        }
    }
    freePriorityQueue(&queue);
}

void deallocateMemory(map *map, graph *graph, hashTable *table) {
    for (int y = 0; y < map->height; y++) {
        free(map->mapVisualisation[y]);
        free(graph->visited[y]);
        free(graph->distances[y]);
    }
    free(graph->visited);
    free(graph->distances);
    free(map->mapVisualisation);
    for (int i = 0; i < map->citiesCount; i++) {
        free(map->cities[i]->name);
        freeNeighbours(map->cities[i]);
        free(map->cities[i]);
    }
    for (int i = 0; i < table->size; i++) {
        freeCities(table->cities[i]);
    }
    free(table->cities);
    free(graph->previous);
    free(graph->totalDistance);
    free(map->cities);
}