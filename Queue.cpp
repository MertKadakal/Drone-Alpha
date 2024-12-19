#include "Queue.h"
#include <iostream>

// Constructor to initialize an empty queue
Queue::Queue() {
    // TODO: Your code here
    front = -1;
    rear = -1;
}

// Adds a province to the end of the queue
void Queue::enqueue(int province) {
    // TODO: Your code here
    // Implement circular structure 
    // Add the province

    if (front == -1)
    {
        front = rear = 0;
        data[rear] = province;
    }

    else if (rear == MAX_QUEUE_SIZE-1 && front != 0)
    {
        rear = 0;
        data[rear] = province;
    }

    else
    {
        rear++;
        data[rear] = province;
    }
}

// Removes and returns the front province from the queue
int Queue::dequeue() {
    // TODO: Your code here

    int data2 = data[front];
    data[front] = -1;
    if (front == rear)
    {
        front = -1;
        rear = -1;
    }
    else if (front == MAX_QUEUE_SIZE-1)
        front = 0;
    else
        front++;

    return data2;
}

// Returns the front province without removing it
int Queue::peek() const {
    // TODO: Your code here
    if (!isEmpty()) {
        return data[front];
    }
    return -1;
}

// Checks if the queue is empty
bool Queue::isEmpty() const {
    // TODO: Your code here
    return front == -1;
}

// Add a priority neighboring province in a way that will be dequeued and explored before other non-priority neighbors
void Queue::enqueuePriority(int province) {
    // TODO: Your code here
    if ((rear + 1) % MAX_QUEUE_SIZE != front) {
        if (front == -1) {
            front = 0;
            rear = 0;
            data[front] = province;
        } else {
            // add province to the head of the queue
            front = (front - 1 + MAX_QUEUE_SIZE) % MAX_QUEUE_SIZE;
            data[front] = province;
        }
    }
}

void Queue::resetQueue() {
    // TODO: Your code here
    front = -1;
    rear = -1;
}