#include <stdio.h>
#include <unistd.h>

#include "read_temp.h"
#include "write_display.h"

int main(int argc, char **argv) {

	int temp;
	int fd;

	while(1) {

		temp = read_temp();

		fd = init_display();

		write_display(fd, temp);

		usleep(1000000);
	
	}
	
	shutdown_display(fd);

	return 0;
}

