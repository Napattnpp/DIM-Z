from serial import Serial
import time

serialPort = '/dev/ttyUSB0'
baudRate = 115200
alphaEn_output_path = ''

def main():
    # Open serial port
    with Serial(port=serialPort, baudrate=baudRate, timeout=12) as ser:
        time.sleep(3)

        # Open file for read encoded image
        with open(alphaEn_output_path, 'rb') as file:
            # Read data up to 32 bytes from file
            while (buffer := file.read(255)):
                try:
                    # Send data to serial port
                    print(buffer)
                    ser.write(buffer)
                    time.sleep(1.5)
                except KeyboardInterrupt:
                    exit(1)

if __name__ == "__main__":
    main()

