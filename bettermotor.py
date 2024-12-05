from time import sleep
import RPi.GPIO as GPIO
import spidev
import matplotlib.pyplot as plt

# Constants
SPI_BUS = 0
SPI_DEVICE = 1
SPI_MAX_SPEED_HZ = 50000
SAMPLING_TIME = 0.01  # 10ms
MOTOR_PWM_FREQUENCY = 2000  # Hz
GEAR_RATIO = 30
ENCODER_COUNTS_PER_REV = 4 * 64 * GEAR_RATIO
VOLTAGE_LIMIT = 12  # Maximum voltage for motors

GPIO.setwarnings(False)

# Create an SPI instance
spi = spidev.SpiDev()


class Motor:
    """Motor abstraction for PWM control, speed, and distance measurement."""

    def __init__(self, pwm_pin, direction_pin, distance_address, speed_address):
        self.pwm_pin = pwm_pin
        self.direction_pin = direction_pin
        self.distance_address = distance_address
        self.speed_address = speed_address

        # Initialize GPIO pins
        GPIO.setup(self.pwm_pin, GPIO.OUT)
        GPIO.setup(self.direction_pin, GPIO.OUT)

        # Create PWM instance
        self.pwm = GPIO.PWM(self.pwm_pin, MOTOR_PWM_FREQUENCY)
        self.pwm.start(0)  # Start with 0% duty cycle

    def read_data(self, distance_or_speed):
        """Read encoder data (speed or distance) from SPI."""
        if distance_or_speed == "distance":
            read_command = [self.distance_address, 0x00, 0x00, 0x00, 0x00]
        else:
            read_command = [self.speed_address, 0x00, 0x00, 0x00, 0x00]
        response = spi.xfer2(read_command)
        return int.from_bytes(response[1:], byteorder='big', signed=True)

    def get_speed(self):
        """Get speed from the encoder."""
        ticks_per_ms = self.read_data("speed")
        rpm = 60*(ticks_per_ms * 100)/ (4*ENCODER_COUNTS_PER_REV*GEAR_RATIO)
        return rpm

    def get_distance(self):
        """Get distance from the encoder."""
        distance = self.read_data("distance")
        return distance
    
    def set_speed(self, voltage):
        """Set motor speed by controlling PWM duty cycle and direction."""
        voltage = max(-VOLTAGE_LIMIT, min(VOLTAGE_LIMIT, voltage))  # Limit voltage
        duty_cycle = 100 * abs(voltage) / VOLTAGE_LIMIT

        GPIO.output(self.direction_pin, voltage < 0)  # Set direction
        self.pwm.ChangeDutyCycle(duty_cycle)  # Update PWM duty cycle


class Robot:
    """Robot with two motors and control routines."""

    def __init__(self):
        GPIO.setmode(GPIO.BCM)

        # Initialize SPI
        spi.open(SPI_BUS, SPI_DEVICE)
        spi.max_speed_hz = SPI_MAX_SPEED_HZ

        # Motors
        self.left_motor = Motor(pwm_pin=12, direction_pin=5, speed_address=0x10, distance_address=0x12)
        self.right_motor = Motor(pwm_pin=13, direction_pin=6, speed_address=0x11, distance_address=0x13)

        # Control variables
        self.int_e_speed_left, self.int_e_speed_right = 0.0, 0.0  # Speed controller integrals
        self.int_e_pos_left, self.int_e_pos_right = 0.0, 0.0 # Position controller integrals

        # PI gains
        self.Kp_pos, self.Ki_pos = 1.0, 0.0  # Position controller gains
        self.Kp_speed, self.Ki_speed = 0.1, 0.1  # Speed controller gains

        self.data = []
        self.datacontrol = []

    def stop(self):
        """Stop both motors."""
        self.left_motor.set_speed(0)
        self.right_motor.set_speed(0)

    def control_loop(self, reference_speed): #reference_position,
        global int_e_speed_left, int_e_speed_right, int_e_pos_left, int_e_pos_right
        """PI controller for motor speed."""
        """# Position error
        e_pos_left = reference_position - self.left_motor.get_distance()
        e_pos_right = reference_position - self.right_motor.get_distance()

        # Integrate error
        self.int_e_pos_left += e_pos_left * SAMPLING_TIME
        self.int_e_pos_right += e_pos_right * SAMPLING_TIME 

        # Calculate reference speed
        ref_speed_left = self.Kp_pos * e_pos_left + self.Ki_pos * int_e_pos_left
        ref_speed_right = self.Kp_pos * e_pos_right + self.Ki_pos * int_e_pos_right"""
    
        # Speed error
        e_speed_left = reference_speed - self.left_motor.get_speed()
        e_speed_right = reference_speed - self.right_motor.get_speed()

        # Integrate error
        self.int_e_speed_left += e_speed_left * SAMPLING_TIME
        self.int_e_speed_right += e_speed_right * SAMPLING_TIME

        # Calculate control signal
        u_volt_left = self.Kp_speed * e_speed_left + self.Ki_speed * self.int_e_speed_left
        u_volt_right = self.Kp_speed * e_speed_right + self.Ki_speed * self.int_e_speed_right

        # Set motor speeds
        # Collect data
        self.data.append((self.left_motor.get_speed(), self.right_motor.get_speed()))
        self.datacontrol.append((u_volt_left, u_volt_right))

        # Set motor speeds
        self.right_motor.set_speed(u_volt_right)


    def reset_values(self):
        """Send reset command to encoder."""
        reset_command = [0x7F, 0x00, 0x00, 0x00, 0x00]
        spi.xfer2(reset_command)
        print("Encoder values reset.")

    def routine(self, reference_speed, duration):
        """Run the robot for a fixed duration with a given reference speed."""
        self.data.clear()  # Clear old data
        self.datacontrol.clear()  # Clear old data
        start_time = 0
        while start_time < duration:
            self.control_loop(reference_speed)
            sleep(SAMPLING_TIME)
            start_time += SAMPLING_TIME
        self.stop()

        # Plot data
        self.plot_data()

    def plot_data(self):
        """Plot speed and control signal data."""
        speeds = list(zip(*self.data))
        controls = list(zip(*self.datacontrol))
        plt.plot(speeds[0], label="Left Motor Speed")
        plt.plot(speeds[1], label="Right Motor Speed")
        plt.plot(controls[0], label="Control Signal Left")
        plt.plot(controls[1], label="Control Signal Right")
        plt.legend()
        plt.savefig("plot.png")
        plt.show()


# Main program
def main():
    corneille = Robot()

    while True:
        instr = input("Enter command (a=run, s=stop, d=distance, r=reset, q=quit): ")
        if instr == 'a':
            corneille.routine(reference_speed=20, duration=10)  # Example
        elif instr == 's':
            print(f"Left Speed: {corneille.left_motor.get_speed()}")
            print(f"Right Speed: {corneille.right_motor.get_speed()}") 
        elif instr == 'd':
            print(f"Left Distance: {corneille.left_motor.get_distance()}")
            print(f"Right Distance: {corneille.right_motor.get_distance()}")
        elif instr == 'r':
            corneille.reset_values()
        elif instr == 'q':
            corneille.stop()
            spi.close()
            GPIO.cleanup()
        else:
            print("Invalid command!")



if __name__ == "__main__":
    main()
