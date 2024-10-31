#include <stdio.h>
#include <stdlib.h>

// Define the Car structure for storing car information
typedef struct Car {
    int carNumber;
    int stayDuration;  // Lower number represents shorter stay (higher priority)
    struct Car* nextCar;
} Car;

// Define a Stack for managing parking order (LIFO)
typedef struct ParkingStack {
    Car* topCar;
} ParkingStack;

// Define a node for the priority queue (for removing cars based on stay duration)
typedef struct PriorityQueueNode {
    Car* carDetails;
    struct PriorityQueueNode* nextNode;
} PriorityQueueNode;

// Priority Queue structure to manage priority removals
typedef struct StayDurationQueue {
    PriorityQueueNode* headNode;
} StayDurationQueue;

// Create a new car with a given ID and stay duration
Car* createCarRecord(int carNumber, int stayDuration) {
    Car* newCar = (Car*)malloc(sizeof(Car));
    newCar->carNumber = carNumber;
    newCar->stayDuration = stayDuration;
    newCar->nextCar = NULL;
    return newCar;
}

// Initialize an empty parking stack
ParkingStack* initializeParkingLot() {
    ParkingStack* stack = (ParkingStack*)malloc(sizeof(ParkingStack));
    stack->topCar = NULL;
    return stack;
}

// Initialize an empty priority queue
StayDurationQueue* initializeStayQueue() {
    StayDurationQueue* queue = (StayDurationQueue*)malloc(sizeof(StayDurationQueue));
    queue->headNode = NULL;
    return queue;
}

// Park a car in the stack
void parkCar(ParkingStack* lot, Car* car) {
    car->nextCar = lot->topCar;
    lot->topCar = car;
    printf("Car %d parked with stay duration %d hours.\n", car->carNumber, car->stayDuration);
}

// Release a car from the parking stack
Car* releaseCar(ParkingStack* lot, int carNumber) {
    if (lot->topCar == NULL) {
        printf("Parking lot is empty.\n");
        return NULL;
    }

    Car* currentCar = lot->topCar;
    Car* prevCar = NULL;

    while (currentCar != NULL) {
        if (currentCar->carNumber == carNumber) {
            if (prevCar == NULL) {
                lot->topCar = currentCar->nextCar;
            } else {
                prevCar->nextCar = currentCar->nextCar;
            }
            printf("Car %d has left the parking lot.\n", currentCar->carNumber);
            return currentCar;
        }
        prevCar = currentCar;
        currentCar = currentCar->nextCar;
    }

    printf("Car with number %d not found in the lot.\n", carNumber);
    return NULL;
}

// Add a car to the priority queue based on stay duration
void addCarToPriorityQueue(StayDurationQueue* queue, Car* car) {
    PriorityQueueNode* newNode = (PriorityQueueNode*)malloc(sizeof(PriorityQueueNode));
    newNode->carDetails = car;
    newNode->nextNode = NULL;

    if (queue->headNode == NULL || queue->headNode->carDetails->stayDuration > car->stayDuration) {
        newNode->nextNode = queue->headNode;
        queue->headNode = newNode;
    } else {
        PriorityQueueNode* temp = queue->headNode;
        while (temp->nextNode != NULL && temp->nextNode->carDetails->stayDuration <= car->stayDuration) {
            temp = temp->nextNode;
        }
        newNode->nextNode = temp->nextNode;
        temp->nextNode = newNode;
    }
}

// Remove a car from the priority queue based on the shortest stay duration
Car* removeCarFromPriorityQueue(StayDurationQueue* queue) {
    if (queue->headNode == NULL) {
        printf("No cars to remove by priority.\n");
        return NULL;
    }
    PriorityQueueNode* tempNode = queue->headNode;
    queue->headNode = queue->headNode->nextNode;
    Car* car = tempNode->carDetails;
    free(tempNode);
    printf("Car %d with stay duration %d is being removed by priority.\n", car->carNumber, car->stayDuration);
    return car;
}

// Check if the parking stack is empty
int isParkingLotEmpty(ParkingStack* lot) {
    return lot->topCar == NULL;
}

// Check if the priority queue is empty
int isPriorityQueueEmpty(StayDurationQueue* queue) {
    return queue->headNode == NULL;
}

// Main function to manage parking lot system
int main() {
    ParkingStack* lot = initializeParkingLot();
    StayDurationQueue* queue = initializeStayQueue();

    int option, carNumber, stayDuration;

    while (1) {
        printf("\n*** Parking Lot Management System ***\n");
        printf("1. Park a car\n");
        printf("2. Remove a car by car number\n");
        printf("3. Remove car based on shortest stay duration\n");
        printf("4. Exit\n");
        printf("Choose an option: ");
        scanf("%d", &option);

        switch (option) {
            case 1:
                printf("Enter car number: ");
                scanf("%d", &carNumber);
                printf("Enter stay duration (hours): ");
                scanf("%d", &stayDuration);
                Car* newCar = createCarRecord(carNumber, stayDuration);
                parkCar(lot, newCar);
                addCarToPriorityQueue(queue, newCar);
                break;
            case 2:
                printf("Enter car number to remove: ");
                scanf("%d", &carNumber);
                Car* removedCar = releaseCar(lot, carNumber);
                if (removedCar != NULL) {
                    free(removedCar);
                }
                break;
            case 3:
                if (!isPriorityQueueEmpty(queue)) {
                    Car* priorityCar = removeCarFromPriorityQueue(queue);
                    free(priorityCar);
                }
                break;
            case 4:
                printf("Exiting system...\n");
                exit(0);
            default:
                printf("Invalid option. Please try again.\n");
        }
    }

    return 0;
}