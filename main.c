#include "main.h"

int main() {

    map map;
    graph graph;

    initializeMapGraph(&map, &graph);

    findCities(&map, &graph);
    findNames(&map);

    // Output cities
    for (int i = 0; i < map.citiesCount; i++) {
        printf("%s (%d, %d)\n", map.cities[i]->name, map.cities[i]->x, map.cities[i]->y);
    }

    for (int i = 0; i < map.citiesCount; i++) {
        clear(map.cities[i], &map, &graph);
        bfs(map.cities[i], &map, &graph);
    }

//    inputFlights(&map);
    fillAdjacencyList(&map, &graph);

    int x = 3;

    deallocateMemory(&map, &graph);

    return 0;
}

void initializeMapGraph(map *map, graph *graph) {
    // Input width and height
    scanf(" %d", &map->width);
    scanf(" %d", &map->height);

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

void findCities(map *map, graph *graph) {
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
    graph->adjacencyList = (struct neighbour **) calloc(map->citiesCount, sizeof(struct neighbour *));
    for (int i = 0; i < map->citiesCount; i++) {
        map->cities[i] = (city *) malloc(sizeof(city));
        map->cities[i]->index = i;
        map->cities[i]->name = (char *) calloc((map->maximalCityNameLength), sizeof(char));
        map->cities[i]->x = citiesCoordinates[i][X_COORDINATE];
        map->cities[i]->y = citiesCoordinates[i][Y_COORDINATE];
        map->cities[i]->neighboursCount = 0;
        map->cities[i]->neighbours = NULL;
        graph->adjacencyList[i] = NULL;
    }

    for (int i = 0; i < maximalCitiesCount; i++) {
        free(citiesCoordinates[i]);
    }
    free(citiesCoordinates);
}

void findNames(map *map) {
    // We need to add citiesCount for the null terminator
    map->maximalCityNameLength = map->width + map->citiesCount;

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
                    while (cityNameXTemporary - 1 >= 0 && isAlphaNumeric(map, cityNameXTemporary - 1, cityNameYTemporary)) {
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
            *(map->cities[i]->name + currentCharPosition) = map->mapVisualisation[cityNameY][cityNameX];
            currentCharPosition++;
            cityNameX++;
        }
        *(map->cities[i]->name + currentCharPosition) = '\0';
    }
}

int isAlphaNumeric(map *map, int x, int y) {
    return ('A' <= map->mapVisualisation[y][x] && map->mapVisualisation[y][x] <= 'Z') ||
           ('a' <= map->mapVisualisation[y][x] && map->mapVisualisation[y][x] <= 'z') ||
           ('0' <= map->mapVisualisation[y][x] && map->mapVisualisation[y][x] <= '9');
    }

void enqueue(int x, int y, queue *queue) {
    node *newNode = malloc(sizeof(node));
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
    node *temporary = queue->head;
    queue->head = queue->head->next;
    if (queue->head == NULL) {
        queue->tail = NULL;
    }
    free(temporary);
}

int isEmpty(queue *queue) {
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

    while (!isEmpty(&queue)) {
        int currentX = queue.head->x;
        int currentY = queue.head->y;
        dequeue(&queue);
        for (int i = 0; i < NUMBER_OF_DIRECTIONS; i++) {
            int newX = currentX + directionX[i];
            int newY = currentY + directionY[i];
            if (newX < 0 || newX >= map->width || newY < 0 || newY >= map->height) {
                continue;
            }
            if (graph->visited[newY][newX] && map->mapVisualisation[newY][newX] == '*') {
                neighbour *temporary = findNeighbour(newX, newY, sourceCity);
                if (temporary != NULL && graph->distances[currentY][currentX] + 1 < temporary->distance) {
                    temporary->distance = graph->distances[currentY][currentX] + 1;
                    continue;
                }
            }
            if (!graph->visited[newY][newX] && map->mapVisualisation[newY][newX] == '*') {
                addNeighbour(sourceCity, findCity(newX, newY, map), graph->distances[currentY][currentX] + 1);
                graph->visited[newY][newX] = 1;
                continue;
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

    while (!isEmpty(&queue)) {
        int currentX = queue.head->x;
        int currentY = queue.head->y;
        dequeue(&queue);
        for (int i = 0; i < NUMBER_OF_DIRECTIONS; i++) {
            int newX = currentX + directionX[i];
            int newY = currentY + directionY[i];
            if (newX < 0 || newX >= map->width || newY < 0 || newY >= map->height) {
                continue;
            }
            if (graph->visited[newY][newX] && map->mapVisualisation[newY][newX] == '*') {
                graph->visited[newY][newX] = 0;
                continue;
            }
            if (!graph->visited[newY][newX] || map->mapVisualisation[newY][newX] != '#') {
                continue;
            }
            graph->visited[newY][newX] = 0;
            graph->distances[newY][newX] = 0;
            enqueue(newX, newY, &queue);
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

void fillAdjacencyList(map *map, graph *graph) {
    for (int i = 0; i < map->citiesCount; i++) {
        for (int j = 0; j < map->cities[i]->neighboursCount; j++) {
            graph->adjacencyList[map->cities[i]->index] = map->cities[i]->neighbours;
        }
    }
}

void inputFlights(map *map) {
    map->flightsCount = 0;

    scanf(" %d", &map->flightsCount);

    char buffer[BUFFER_SIZE];
    int i = 0;
    while (i < map->flightsCount && fgets(buffer, BUFFER_SIZE, stdin)) {
        char *token = strtok(buffer, " ");
        while (token != NULL) {
            printf("%s",token);
            token = strtok(NULL, " ");
        }
        printf("\n");
        i++;
    }
}

void deallocateMemory(map *map, graph *graph) {
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
    free(graph->adjacencyList);
    free(map->cities);
}