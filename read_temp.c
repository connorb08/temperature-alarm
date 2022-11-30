#include <stdio.h>

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

#define CHANNEL_2 0xA << 4
#define LENGTH 3

double read_temp() {
	
	// Local variables
	int spi_fd;
	int error = -10000;
	int output = 0;
	int mode = SPI_MODE_0;

	float voltage = 0;
	float temp_c = 0;
	float temp_f = 0;

	struct spi_ioc_transfer spi;
	unsigned char data_out[LENGTH] = {0x1, CHANNEL_2, 0x0};
	unsigned char data_in[LENGTH];

	// Clear everything to 0
	memset(&spi, 0, sizeof(struct spi_ioc_transfer));

	// Set up spi transfer structure
	spi.tx_buf = (unsigned long) &data_out;
	spi.rx_buf = (unsigned long) &data_in;
	spi.len = LENGTH;
	spi.delay_usecs = 0;
	spi.speed_hz = 100000;
	spi.bits_per_word = 8;
	spi.cs_change = 0;

	/* Open SPI device */
	spi_fd = open("/dev/spidev0.0", O_RDWR);
	if ( spi_fd < 0 ) {
		return error;
	}

	/* Set SPI Mode_0 */
	if ( ioctl(spi_fd, SPI_IOC_WR_MODE, &mode) < 0)  {
		return error;
	}

	// Send SPI message
	if ( ioctl(spi_fd, SPI_IOC_MESSAGE(1), &spi) < 0 ) {
		return error;
	}

	// Get last 10 bits
	output = ( ( data_in[1] & 0x7 ) << 8 ) | ( data_in[2] );
	
	// Convert 10 bit output to voltage
	voltage = output * 3.3 / 1024.0;
	
	// Convert voltages to temperature
	temp_c = ( voltage * 100 ) - 50;

	temp_f = (temp_c * 9/5) + 32;

	return temp_f;

}