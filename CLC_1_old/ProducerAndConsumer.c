# include <stdio.h>

int theProduct;
int buffer[5];
int start = 0;
int end = 0;
int length = 0;

int produce() {
    return theProduct++;
} 

void consume(int i) {
    printf("%i", i);
}

void put(int i) {
    if (length <= 5) {
        buffer[end] = i;
        end += 1;
        end = end % 5;
        length += 1;
    }
    else {
        printf("Failed: Queue is full.");
    }
}

int get() {
    if (length != 0) {
        int i;
        i = buffer[start];
        start += 1;
        start = start % 5;
        length -= 1;
        return i;
    }
    else {
        printf("Failed: No item in the queue.");
    }
}

void producer() {
    int i;
    while(1) {
        i = produce();
        put(i);
    }
}

void consumer() {
    int i;
    while(1) {
        i = get();
        consume(i);
    }
}

void main() {
    // Create an integer variable that will store the number we get from the user
    int myNum;

    // Ask the user to type a number
    printf("Type a number:\n");

    // Get and save the number the user types
    scanf("%d", &myNum);

    producer();

    // Output the number the user typed
    printf("Your number is: %d\n", myNum);
}