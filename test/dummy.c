#include <stdio.h>
#include <assert.h>


// TODO -- Replace with a real testing framework 
int add(int a, int b) {
    return a + b;
}

int main() {
    int result = add(2, 2);
    assert(result == 4);
    printf("Test passed!\n");
    return 0;
}