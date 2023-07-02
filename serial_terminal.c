/**
 * @file serial_terminal.c
 * 
 * @author Vikrant A. P. (vikrant_ap@hotmail.com)
 * 
 * @brief 
 * A simple serial port terminal implementation for Linux and 
 * Mac operating systems. This simple implementation inteded to 
 * echo the tranmission and reception.
 * 
 * For more details kindly follow the blog
 * https://hexavik.github.io
 * 
 * or connect me through the links specified on the website.
 * 
 * @date 2023-06-26
 */

#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include <fcntl.h>		// Contains file controls like O_RDWR
#include <errno.h>		// Error integer and strerror() function
#include <termios.h>	// Contains POSIX terminal control definitions
#include <unistd.h>		// write(), read(), close()

/*
 * Change the device in accordance your COM port.
 * In Linux it is specified as ttyUSB* or ttyACM*
 * In MacOS it is specified as cu.usbmodem* or cu.usbserial-*
 */
#define DEVICE_PORT		"/dev/cu.usbserial-21310"

/*
 * Change the Baudrate in accordance with your requirement
 */
#define BAUD_RATE	B9600

int main() {
	// Open the serial port
	int serial_port = open(DEVICE_PORT, O_RDWR);

	// Create new temrios struct
	struct termios com;

	// Read in existing settings, and handle any error occurs
	if (tcgetattr(serial_port, &com) != 0) {
		printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
		return 1;
	}

	// Clear parity bit, disabling parity
	com.c_cflag &= ~PARENB;
	// Clear stop bit field, only one stop bit used
	com.c_cflag &= ~CSTOPB;
	// Clear all bits that set the data size
	com.c_cflag &= ~CSIZE;
	// Set the data size to 8 bits per byte
	com.c_cflag |= CS8;
	// Disable RTS/CTS hardware flow control
	com.c_cflag &= ~CRTSCTS;
	// Turn on READ and ignore lines (CLOCAL = 1)
	com.c_cflag |= CREAD | CLOCAL;

	// Disable canonical mode
	com.c_lflag &= ~ICANON;
	// Disable echo
	com.c_lflag &= ~ECHO;
	// Disable erasure
	com.c_lflag &= ~ECHOE;
	// Disable new-line echo
	com.c_lflag &= ~ECHONL;
	// Disable interpretation of INTR, QUIT, and SUSP
	com.c_lflag &= ~ISIG;
	// Turn off software flow control
	com.c_iflag &= ~(IXON | IXOFF | IXANY);
	// Disable any special handling of received bytes
	com.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL);

	// Prevent special interpretation of output bytes
	com.c_oflag &= ~OPOST;
	// Prevent conversion of newline to carriage return/line feed
	com.c_oflag &= ~ONLCR;

	// Wait for up to 1s (10 deciseconds), 
	// returning as soon as any data is received.
	com.c_cc[VTIME] = 10;
	com.c_cc[VMIN] = 0;

	/*
	 * Set in/out baud rate to be 9600.
	 * `cfsetispeed` can be used for input speed.
	 * `cfsetospeed` can be used for output speed.
	 * `cfsetspeed` for both input and output speed.
	 */
	//cfsetispeed(&com, BAUD_RATE);
	//cfsetospeed(&com, BAUD_RATE);
	cfsetspeed(&com, BAUD_RATE);

	// Save tty settings, also checking for error
	if (tcsetattr(serial_port, TCSANOW, &com) != 0) {
		printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
		return 1;
	}

	// Write to serial port
	uint8_t msg[] = { 0x35, 0xA3, 0x65, 0xBB, 0x1C, 0x88 };
	printf("Written bytes: %zi\n", write(serial_port, msg, sizeof(msg)));

	sleep(1);

	// Allocate memory for read buffer
	uint8_t read_buf[256];

	/*
	 * Normally you wouldn't do this memset() call, but since
	 * we will just receive ASCII data for this example we'll
	 * set everything to 0 (0x00 or '\0') so we can call printf() easily.
	 */
	memset(&read_buf, 0x00, sizeof(read_buf));

	/*
	 * Read bytes. The behaviour of read() (e.g. does it block?,
	 * how long does it block for?) depends on the configuration
	 * settings above, specifically VMIN and VTIME.
	 */
	int bytes_read = read(serial_port, &read_buf, sizeof(read_buf));

	// bytes_read may be 0 if no bytes were received, and can 
	// also be -1 to signal an error.
	if (bytes_read < 0) {
		printf("Error reading: %s", strerror(errno));
		return 1;
	}

	// Display received bytes as a string
	//printf("Read %i bytes. Received message: %s", bytes_read, read_buf);
	// Display received bytes in HEX
	printf("Read %i bytes. Received message: ", bytes_read);
	for (int i = 0; i < bytes_read; i++) {
		printf("0x%2X ", read_buf[i]);
	}
	printf("\n");

	// Close the opened port properly
	close(serial_port);

	return 0;	// Success
}
