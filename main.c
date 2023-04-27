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

    for (int i = 0; i < map.citiesCount; i++) {
        clear(&map);
        bfs(map.cities[i], &map);
    }
    int x = 3;
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
    map->visited = (int **) calloc(map->height, sizeof(int *));
    map->distances = (int **) calloc(map->height, sizeof(int *));
    for (int y = 0; y < map->height; y++) {
        map->mapVisualisation[y] = (char *) calloc(map->width, sizeof(char));
        map->visited[y] = (int *) calloc(map->width, sizeof(int));
        map->distances[y] = (int *) calloc(map->width, sizeof(int));
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
        map->cities[i]->neighboursCount = 0;
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

void bfs(city *sourceCity, map *map) {
    queue queue;
    queue.head = NULL, queue.tail = NULL;

    // Directions: up, right, down, left
    int directionX[] = {0, 1, 0, -1};
    int directionY[] = {-1, 0, 1, 0};

    enqueue(sourceCity->x, sourceCity->y, &queue);
    map->visited[sourceCity->y][sourceCity->x] = 1;
    map->distances[sourceCity->y][sourceCity->x] = 0;

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
            if (map->visited[newY][newX] && map->mapVisualisation[newY][newX] == '*') {
                // TODO: Check if this works
                // TODO: If we find the same neighbour again check if the distance is smaller
                neighbour *temporary = findNeighbour(newX, newY, sourceCity);
                if (temporary != NULL && map->distances[currentY][currentX] + 1 < temporary->distance) {
                    temporary->distance = map->distances[currentY][currentX] + 1;
                    continue;
                }
            }
            if (!map->visited[newY][newX] && map->mapVisualisation[newY][newX] == '*') {
                sourceCity->neighbours[sourceCity->neighboursCount] = (neighbour *) malloc(sizeof(neighbour));
                sourceCity->neighbours[sourceCity->neighboursCount]->city = findCity(newX, newY, map);
                sourceCity->neighbours[sourceCity->neighboursCount]->distance = map->distances[currentY][currentX] + 1;
                sourceCity->neighboursCount++;
                map->visited[newY][newX] = 1;
                continue;
            }
            if (map->visited[newY][newX] || map->mapVisualisation[newY][newX] != '#') {
                continue;
            }
            map->visited[newY][newX] = 1;
            map->distances[newY][newX] = map->distances[currentY][currentX] + 1;
            enqueue(newX, newY, &queue);
        }
    }
}

void clear(map *map) {
    for (int y = 0; y < map->height; y++) {
        for (int x = 0; x < map->width; x++) {
            map->visited[y][x] = 0;
            map->distances[y][x] = -1;
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

neighbour *findNeighbour(int x, int y, city *city) {
    for (int i = 0; i < city->neighboursCount; i++) {
        if (city->neighbours[i]->city->x == x && city->neighbours[i]->city->y == y) {
            return city->neighbours[i];
        }
    }
    return NULL;
}


void deallocateMemory(map *map) {
    for (int y = 0; y < map->height; y++) {
        free(map->mapVisualisation[y]);
        free(map->visited[y]);
        free(map->distances[y]);
    }
    free(map->visited);
    free(map->distances);
    free(map->mapVisualisation);
    for (int i = 0; i < map->citiesCount; i++) {
        free(map->cities[i]->name);
        for (int j = 0; j < map->cities[i]->neighboursCount; j++) {
            free(map->cities[i]->neighbours[j]);
        }
        free(map->cities[i]->neighbours);
        free(map->cities[i]);
    }
    free(map->cities);
}