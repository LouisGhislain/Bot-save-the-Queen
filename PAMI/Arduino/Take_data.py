import serial
import time

ser = serial.Serial('/dev/ttyUSB0', 9600)  # Remplace avec COMx sur Windows
time.sleep(2)

with open("log.csv", "w") as f:
    while True:
        line = ser.readline().decode().strip()
        print(line)
        f.write(line + "\n")
