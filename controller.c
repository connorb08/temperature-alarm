#include <stdio.h>
#include <unistd.h>

#include "read_temp.h"
#include "write_display.h"

int main(int argc, char **argv) {

	int temp;
	int fd;

	while(1) {

		temp = read_temp();

		// check if temp is outlier??

		fd = init_display();

		write_display(fd, temp);

		usleep(1000000);
	}

	// ?? When will it break out of loop? should it do this every time it is written?
	shutdown_display(fd);

	return 0;
}

