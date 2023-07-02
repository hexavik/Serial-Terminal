# Serial Terminal

This is a simple serial port terminal implementation for Linux and Mac operating systems. The purpose of this implementation is to provide a basic example of a serial terminal that echoes transmission and reception.

For more details and usage instructions, please refer to the corresponding blog post available on my website [here](https://hexavik.github.io). You can also connect with me through the links provided on the website.

## Usage

1. Modify the `DEVICE_PORT` constant in the code to match your COM port. For Linux, it is typically specified as `ttyUSB*` or `ttyACM*`, and for MacOS, it is specified as `cu.usbmodem*` or `cu.usbserial-*`.

2. Adjust the `BAUD_RATE` constant in the code according to your specific requirements.

3. Compile the code using a C compiler.

4. Run the compiled executable to open the serial port and establish communication.

5. The terminal will send a predefined message, specified by the `msg` array, to the serial port.

6. It will then read any incoming data from the serial port and display it both as a string and in hexadecimal format.

7. Finally, the serial port will be closed properly.

Please note that this code assumes the availability of the required libraries and headers for serial port communication. Ensure that these dependencies are installed on your system before compiling and running the code.

Feel free to explore, modify, and adapt this code according to your needs. Contributions and suggestions are welcome.
