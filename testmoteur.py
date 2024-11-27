from time import sleep
import RPi.GPIO as GPIO
import spidev

GPIO.setwarnings(False)

# Create an SPI instance
spi = spidev.SpiDev()

# Open SPI bus 0, device 1 which is the one for DE0-Nano (device 0 is for CAN)
spi.open(0, 1)

# Set SPI speed and mode if needed
spi.max_speed_hz = 1000000 
spi.mode = 1


class Robot():

    def __init__(self):
        GPIO.setmode(GPIO.BCM)
        GPIO.setup((12, 13), GPIO.OUT)
        GPIO.setup((5, 6), GPIO.OUT)

        self.speed = 0
        self.direction = "idle"

        self.leftMotor = GPIO.PWM(12, 2000)
        self.rightMotor = GPIO.PWM(13, 2000)

    def stop(self):
        self.leftMotor.stop()
        self.rightMotor.stop()
        GPIO.output((12, 13), GPIO.LOW)
        GPIO.output((5, 6), False)

    def get_speed(self):
        # Construct the command to read from address 0x10
        # First byte: set the 8th bit to 0 (read) and bits 1-7 to 0x10 (address)
        read_command = [0x10, 0x00, 0x00, 0x00, 0x00]
        
        # Send the command and receive 5 bytes of response
        response = spi.xfer2(read_command)
        print("Received response:", response)

        # Convert the last 4 bytes of the response to a signed 32-bit integer
        speed_bytes = response[1:]  # [255, 255, 255, 242]
        ticks_per_ms = int.from_bytes(speed_bytes, byteorder='big', signed=True)

        # Calculate the speed in ticks per second (scaling from 10 ms to 1 second)
        ticks_per_second = ticks_per_ms * 100  # because data represents a 10 ms interval

        # Using a gearbox, divide by encoder counts per output shaft rotation (64 * gear_ratio)
        rotations_per_second = ticks_per_second / (64 * 30)  # gear_ratio specific to motor
        rpm = rotations_per_second * 60  # Convert to RPM

        print("Speed (RPM):", rpm)

    def get_distance(self):
        # Construct the command to read from address 0x12
        # First byte: set the 8th bit to 0 (read) and bits 1-7 to 0x12 (address)
        read_command = [0x12, 0x00, 0x00, 0x00, 0x00]
        
        # Send the command and receive 5 bytes of response
        response = spi.xfer2(read_command)
        print("Received response:", response)

        # Convert the last 4 bytes of the response to a signed 32-bit integer
        distance_bytes = response[1:]
        distance = int.from_bytes(distance_bytes, byteorder='big', signed=True)
        print("Distance:", distance)

    def routine(self):
        GPIO.output(5, False)
        GPIO.output(6, False)
        self.leftMotor.start(10)
        self.rightMotor.start(20)
        sleep(5)
        self.get_speed()
        self.stop()

corneille = Robot()

# Control of the motors

# Initialize variables
reference_position = 0.0
measured_position = corneille.get_distance()
reference_speed = 0.0
measured_speed = corneille.get_speed()
u_volt = 0.0

# PI gains
Kp_pos, Ki_pos = 1.0, 0.5  # Position controller gains
Kp_speed, Ki_speed = 1.0, 0.5  # Speed controller gains

# Error and integral terms
e_pos_prev, e_speed_prev = 0.0, 0.0
int_e_pos, int_e_speed = 0.0, 0.0

# Sampling time (in seconds)
Ts = 0.01

# Control loop
def control_loop(reference_position, reference_speed, measured_speed, measured_position):
    global e_pos_prev, e_speed_prev, int_e_pos, int_e_speed

    # Position controller
    e_pos = reference_position - measured_position
    int_e_pos += e_pos * Ts
    reference_speed = Kp_pos * e_pos + Ki_pos * int_e_pos

    # Speed controller
    e_speed = reference_speed - measured_speed
    int_e_speed += e_speed * Ts
    u_volt_ref = Kp_speed * e_speed + Ki_speed * int_e_speed

    # Voltage saturation
    u_volt = max(-12, min(12, u_volt_ref))

    # Update previous errors
    e_pos_prev = e_pos
    e_speed_prev = e_speed

    return u_volt

# Convert voltage to PWM duty cycle

def voltage_to_duty_cycle(voltage):
    # Convert voltage to duty cycle (0-100%)
    duty_cycle = 100 * abs(voltage) / 12

    # Set direction
    if voltage > 0:
        GPIO.output((5, 6), (False, True))
    else:
        GPIO.output((5, 6), (True, False))

    return duty_cycle

# Main loop

while True:
    instr = input("Press 'a' to run the routine, s to print speed, d to print distance : ")
    if instr == 'a':
        corneille.routine()
        corneille.stop()
        instr =input("Press 'a' to run the routine: ")
    elif instr == 's':
        corneille.get_speed()
    elif instr == 'd':
        corneille.get_distance()
    else :
        corneille.stop()
        SPI.close()
        GPIO.cleanup()
        break
