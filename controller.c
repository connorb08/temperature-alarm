#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "read_temp.h"
#include "write_display.h"

int main(int argc, char **argv) {

	double temp;
	int fd;

	temp = read_temp();

	fd = init_display();
	write_display(fd, temp);
	printf("%lf", temp);
	shutdown_display(fd);

	if (argc > 1) {
		if (strcmp(argv[1], "c") == 0) {
			shutdown_display(fd);
		}
	}

	return 0;
}

