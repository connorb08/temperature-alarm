#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "write_display.h"

int main(int argc, char** argv) {

    int fd;
    const float UPPER_BOUND = 1.0;
    double temp = 60.0;
    float x = 0;
    srand ( time(NULL) );

    fd = init_display();

    if (argc > 1) {
        temp = (float) atof(argv[1]);
    }
    
    if (temp < 75.0) {
        x = ((float)rand()/(float)(RAND_MAX)) * UPPER_BOUND;
        temp += x;
    }
    else {
        x = ((float)rand()/(float)(RAND_MAX)) * 2;
        temp = 75.0 + x;
    }
    
    printf("%lf", temp);
    write_display(fd, temp);

    

    return 0;
}