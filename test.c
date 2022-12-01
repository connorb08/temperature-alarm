#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int main(int argc, char** argv) {

    const float UPPER_BOUND = 0.5;
    double temp = 60.0;
    float x = 0;
    srand ( time(NULL) );

    if (argc > 1) {
        temp = (float) atof(argv[1]);
    }
    
    x = ((float)rand()/(float)(RAND_MAX)) * UPPER_BOUND;
    temp += x;
    printf("%lf", temp);

    return 0;
}