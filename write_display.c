#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

#define ZERO 		0b00111111
#define ONE 		0b00000110
#define TWO 		0b01011011
#define THREE 		0b01001111
#define FOUR 		0b01100110
#define FIVE 		0b01101101
#define SIX 		0b01111101
#define SEVEN 		0b00000111
#define EIGHT 		0b01111111
#define NINE 		0b01100111
#define LETTER_C 	0b00111001
#define LETTER_F 	0b01110001
#define LETTER_E	0b01111001
#define LETTER_R	0b00110001
#define DECIMAL		0x80
#define BLANK		0x00
#define MINUS 		0b01000000

int init_display() {
	
	#define ENABLE_OSCILLATOR (0x2<<4) | (0x1)
	#define ENABLE_SCREEN (0x8<<4) | (0x1)
	#define SET_BRIGHTNESS (0xe<<4) | (0xc)

	int fd;
	unsigned char buffer[17];

	/* Open i2c device */
	fd = open("/dev/i2c-1", O_RDWR);
	if ( fd < 0 ) {
		// Error
		printf("error opening\n");
		close(fd);
	}

	// Set slave address
	if ( ioctl(fd, I2C_SLAVE, 0x70) < 0 ) {
		printf("Error writing to display, exiting.\n");
		exit(1);
	}

	/* Turn on oscillator */
	buffer[0] = ENABLE_OSCILLATOR;
	if ( write(fd, buffer, 1) < 0 ) {
		printf("Error writing to display, exiting.\n");
		exit(1);
	}

	/* Turn on Display, No Blink */
	buffer[0] = ENABLE_SCREEN;
	if ( write(fd, buffer, 1) < 0 ) {
		printf("Error writing to display, exiting.\n");
		exit(1);
	}

	/* Set Brightness */
	buffer[0] = SET_BRIGHTNESS;
	if ( write(fd, buffer, 1) < 0 ) {
		printf("Error writing to display, exiting.\n");
		exit(1);
	}

	return fd;
}

int integer_to_bit(int i) {
	
	// abs
	if (i < 0) {
		i *= -1;
	}
	
	switch(i) {
		case 0:
			return ZERO;
		case 1:
			return ONE;
		case 2:
			return TWO;
		case 3:
			return THREE;
		case 4:
			return FOUR;
		case 5:
			return FIVE;
		case 6:
			return SIX;
		case 7:
			return SEVEN;
		case 8:
			return EIGHT;
		case 9:
			return NINE;
		default:
			return BLANK;
	}
	return BLANK;
}

int write_display(int fd, double value) {

	unsigned char buffer[17];
	
	// Set unused bits to 0
	buffer[0] = 0x00;
	buffer[2] = 0x00;
	buffer[4] = 0x00;
	buffer[6] = 0x00;
	buffer[8] = 0x00;
	for (int i = 10; i < 16; i++) {
		buffer[i] = 0x00;
	}

	// Set middle colon to off
	buffer[5] = 0x00;

	if ( ( value >= 0.0 ) && ( value <= 99.9 ) ) {
		// First character
		buffer[1] = ( ( ((int) value) / 10) % 10) ? integer_to_bit( ( ( ((int) value) / 10) % 10) ) : BLANK;
		// Second character
		buffer[3] = integer_to_bit( ((int) value) % 10 ) | DECIMAL;
		// Third character
		buffer[7] = integer_to_bit( ((int)(value * 10)) % 10 );
		// Fourth character
		buffer[9] = LETTER_F;
	}

	else if ( ( value >= -99.9 ) && ( value < 0 ) ) {
		
		// First character
		buffer[1] = MINUS;

		if (value < 10) {
			// Second character
			buffer[3] = integer_to_bit(((int) value) % 10) | DECIMAL;
			// Third character
			buffer[7] = integer_to_bit( ((int) (value * 10)) % 10 );;
		}
		else {
			// Second character
			buffer[3] = integer_to_bit( ( ( ((int) value) / 10) % 10) );
			// Third character
			buffer[7] = integer_to_bit( ((int) value) % 10 );
		}
		// Fourth character
		buffer[9] = LETTER_F;
	}

	else if ( ( value >= 100 ) && ( value <= 999 ) ) {
		// First character
		buffer[1] = integer_to_bit( ( ( ((int) value) / 100) % 10) );
		// Second character
		buffer[3] = integer_to_bit( ( ( ((int) value) / 10) % 10) );
		// Third character
		buffer[7] = integer_to_bit( ((int) value) % 10 );
		// Fourth character
		buffer[9] = LETTER_F;
	}

	else {
		// Display ERR
		buffer[1] = LETTER_E;
		buffer[3] = LETTER_R;
		buffer[7] = LETTER_R;
		buffer[9] = BLANK;
	}
	
	// Write buffer to display
	if ( write(fd, buffer, 17) < 0 ) {
		printf("Error writing to display, exiting.\n");
		exit(1);
	}

	return 0;
}

int shutdown_display(int fd) {
	close(fd);
	return 0;
}