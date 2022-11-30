#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "read_temp.h"
#include "write_display.h"

int main(int argc, char **argv) {

	double temp;
	int fd;

	fd = init_display();
	temp = read_temp();
	write_display(fd, temp);
	printf("%lf", temp);

	if (argc > 1) {
		if (strcmp(argv[1], "c") == 0) {
			shutdown_display(fd);
		}
	}

	// if (argc > 1) {

	// 	if (strcmp(argv[1], "r") == 0) {
	// 		printf("%lf", read_temp());
	// 		//return read_temp();
	// 	}

	// 	if (strcmp(argv[1], "i") == 0) {
	// 		printf("%d", init_display());
	// 		return 0;
	// 		//return (int) init_display();
	// 	}

	// 	if (strcmp(argv[1], "w") == 0) {
	// 		if (argc > 3) {
	// 			fd = (int) atoi(argv[2]);
	// 			temp = (double) atof(argv[3]);
	// 			printf("fd: %d, temp: %f", fd, temp);
	// 			write_display(fd, temp);
	// 			// turn arg[2] into fd
	// 			// turn arg[3] into temp
	// 		}
	// 		else {
	// 			return -1;
	// 		}
	// 	}

	// 	if (strcmp(argv[1], "c") == 0) {
	// 		if (argc > 2) {
	// 			fd = atoi(argv[2]);
	// 			shutdown_display(fd);
	// 			//shutdown_display(fd);
	// 			// turn arg2 into fd
	// 		}
			
	// 	}

	// }
	// else {
	// 	fd = init_display();
	// 	temp = read_temp();
	// 	write_display(fd, temp);
	// 	printf("ex");
	// 	return -1;
	// }

	return 0;
}

