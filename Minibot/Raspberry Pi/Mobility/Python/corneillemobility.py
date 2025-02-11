from time import sleep, time
import RPi.GPIO as GPIO
import spidev
import matplotlib.pyplot as plt
import numpy as np
import subprocess
from numpy import pi

# Constants
SPI_BUS = 0
SPI_DEVICE = 1
SPI_MAX_SPEED_HZ = 50000
SAMPLING_TIME = 0.01  # 10ms
MOTOR_PWM_FREQUENCY = 1120  # Hz
GEAR_RATIO = 30
ENCODER_COUNTS_PER_REV = 4 * 64 * GEAR_RATIO
VOLTAGE_LIMIT = 12  # Maximum voltage for motors
DURATION = 3  # Duration of the control loop in seconds
DATA_LENGTH = 263#int(DURATION / SAMPLING_TIME)+1

GPIO.setwarnings(False)

# Create an SPI instance
spi = spidev.SpiDev()


class Motor:
    """Motor abstraction for PWM control, speed, and distance measurement."""

    def __init__(self, pwm_pin, direction_pin, distance_address, speed_address, base_dir):
        self.pwm_pin = pwm_pin
        self.direction_pin = direction_pin
        self.distance_address = distance_address
        self.speed_address = speed_address
        self.base_dir = base_dir

        # Initialize GPIO pins
        GPIO.setup(self.pwm_pin, GPIO.OUT)
        GPIO.setup(self.direction_pin, GPIO.OUT)

        # Create PWM instance
        self.pwm = GPIO.PWM(self.pwm_pin, MOTOR_PWM_FREQUENCY)
        #self.pwm.start(0)  # Start with 0% duty cycle

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
        ticks_per_10ms = self.read_data("speed") 
        diametre = 6.0325     #cm
        meters_per_second = (ticks_per_10ms/ENCODER_COUNTS_PER_REV)*np.pi*diametre*4
        return meters_per_second

    def get_distance(self):
        """Get distance from the encoder."""
        ticks = self.read_data("distance")
        distance = (ticks * 3.14159 * 4.5) / (4 * 2048) *1.01617 #Corrective factor
        return distance
    
    def set_speed(self, voltage):
        """Set motor speed by controlling PWM duty cycle and direction."""
        voltage = max(-VOLTAGE_LIMIT, min(VOLTAGE_LIMIT, voltage))  # Limit voltage
        duty_cycle = 100 * abs(voltage) / VOLTAGE_LIMIT

        if (voltage < 0):
            GPIO.output(self.direction_pin, not (self.base_dir))  # Backward direction
        elif (voltage > 0):
            GPIO.output(self.direction_pin, self.base_dir)
        
        self.pwm.ChangeDutyCycle(duty_cycle)  # Update PWM duty cycle

    def stop(self):
        """Stop the motor by setting speed to zero."""
        self.pwm.stop(0)

    def start(self):
        """Start the motor by setting speed to zero."""
        self.pwm.start(0)  

class Robot:
    """Robot with two motors and control routines."""

    def __init__(self):
        GPIO.setmode(GPIO.BCM)

        # Initialize SPI
        spi.open(SPI_BUS, SPI_DEVICE)
        spi.max_speed_hz = SPI_MAX_SPEED_HZ

        # Motors
        self.left_motor = Motor(pwm_pin=12, direction_pin=5, speed_address=0x10, distance_address=0x12, base_dir=True)
        self.right_motor = Motor(pwm_pin=13, direction_pin=6, speed_address=0x11, distance_address=0x13, base_dir=False)

        # Control variables
        self.int_e_speed_left, self.int_e_speed_right = 0.0, 0.0  # Speed controller integrals
        self.int_e_pos_left, self.int_e_pos_right = 0.0, 0.0 # Position controller integrals

        # PI gains
        self.Kp_pos, self.Ki_pos = 0.01, 0.0  # Position controller gains
        self.Kp_speed, self.Ki_speed = 10, 85.5 #17.3913 # Speed controller gains

        self.data_left = np.zeros(DATA_LENGTH)
        self.data_right = np.zeros(DATA_LENGTH)
        self.speed_left = np.zeros(DATA_LENGTH)
        self.speed_right = np.zeros(DATA_LENGTH)
        self.time= np.zeros(DATA_LENGTH)
        self.data_leftcontrol = np.zeros(DATA_LENGTH)
        self.data_rightcontrol = np.zeros(DATA_LENGTH)
        self.xcoordinates = np.zeros(DATA_LENGTH)
        self.ycoordinates = np.zeros(DATA_LENGTH)

        self.data_counter = 0

        # Robot position parameters
        self.x_coord = 3.8
        self.y_coord = 12.5

        # High level controller gains
        self.Kp_rho = 0.00333
        self.Kp_alpha = 0.004
        self.Kp_beta = 0

        # Displacement since last calling
        self.lastleftdistance = 0
        self.lastrightdistance = 0

        '''        
        # Chemin vers l'exécutable C++
        self.executable_path = "Le_BORDEL_DE_HUGO/lidar_example_cpp-main/output/Linux/Release/main_folder"
 
        #postionx, postiony = 0,0 #= self.get_position_from_cpp(self.executable_path)
        #Lidar pos
        self.lidarx = postionx
        self.lidary = postiony '''

    def stop(self):
        """Stop both motors."""
        self.left_motor.stop()
        self.right_motor.stop()
        self.int_e_pos_left, self.int_e_pos_right = 0.0, 0.0
        self.int_e_speed_left, self.int_e_speed_right = 0.0, 0.0

    def start(self):
        """Start both motors."""
        self.left_motor.start()
        self.right_motor.start()
        self.int_e_pos_left, self.int_e_pos_right = 0.0, 0.0
        self.int_e_speed_left, self.int_e_speed_right = 0.0, 0.0

    def get_angle(self):
        """Get the angle of the robot."""
        theta = 360*(self.right_motor.get_distance() - self.left_motor.get_distance())/(2*np.pi*23.65)
        return theta
    
    def get_position_from_cpp(self, executable_path):
        try:
            # Exécution de l'exécutable C++
            result = subprocess.run(
                [executable_path], 
                capture_output=True, 
                text=True, 
                check=True
            )
            # Récupération et nettoyage de la sortie
            output = result.stdout.strip()
            print(f"Sortie C++: {output}")
            
            # Extraction des coordonnées (x, y)
            position = output.split()
            if len(position) == 2:
                position_x = float(position[0])
                position_y = float(position[1])
                return position_x, position_y
            else:
                raise ValueError("La sortie n'est pas au bon format : attendu 'x y'.")
        except subprocess.CalledProcessError as e:
            print(f"Erreur lors de l'exécution de l'exécutable : {e}")
        except Exception as e:
            print(f"Erreur : {e}")

    def control_loop(self, ref_speed_left, ref_speed_right): 
        """PI controller for motor speed."""
        # Speed error
        e_speed_left = ref_speed_left - self.left_motor.get_speed()
        e_speed_right = ref_speed_right - self.right_motor.get_speed()

        # Integrate error
        self.int_e_speed_left += e_speed_left * SAMPLING_TIME
        self.int_e_speed_right += e_speed_right * SAMPLING_TIME

        # Calculate control signal
        u_volt_left = self.Kp_speed * e_speed_left + self.Ki_speed * self.int_e_speed_left
        u_volt_right = self.Kp_speed * e_speed_right + self.Ki_speed * self.int_e_speed_right

        # Set motor speeds
        self.left_motor.set_speed(u_volt_left)
        self.right_motor.set_speed(u_volt_right)





    def go_to_position(self, x, y, goal_angle):
        """Move the robot to a given position."""
        rho = 1000
        self.start()
        while rho > 10:
            
            # Calculate distance to target (distance error)
            rho = np.sqrt((x - self.x_coord) ** 2 + (y - self.y_coord) ** 2) # Distance in cm

            # Calculate angle to target (angle error)
            alpha = np.arctan2(y - self.y_coord, x - self.x_coord)*(360/(2*np.pi)) - (self.get_angle()) # Angle in degrees

            beta = goal_angle - (self.get_angle()) - alpha   # Angle in degrees

            # Calculate speed references
            v = self.Kp_rho * rho + 0.2
            w = self.Kp_alpha * alpha + self.Kp_beta * beta
            ref_speed_left = v - w
            ref_speed_right = v + w
            """print(f"V: {v:.2f}")
            print(f"W: {w:.2f}")"""

            # Control loop
            self.control_loop(ref_speed_left, ref_speed_right)

            # Update position
            displacement = (self.left_motor.get_distance() - self.lastleftdistance + self.right_motor.get_distance() - self.lastrightdistance)/2

            self.x_coord += displacement * np.cos((self.get_angle()/360)*2*np.pi) *(100/70.2)
            self.y_coord += displacement * np.sin((self.get_angle()/360)*2*np.pi) *(100/70.2)

            self.lastleftdistance = self.left_motor.get_distance()
            self.lastrightdistance = self.right_motor.get_distance()

            #print(f"X: {self.x_coord:.2f}, Y: {self.y_coord:.2f}, Theta: {(self.get_angle()):.2f}")
            
            """self.xcoordinates[self.data_counter] = self.x_coord
            self.ycoordinates[self.data_counter] = self.y_coord
            self.data_counter+=1"""

            # Sleep
            sleep(SAMPLING_TIME)
        self.stop()


    def reset_values(self):
        """Send reset command to encoder."""
        reset_command = [0x7F, 0x00, 0x00, 0x00, 0x00]
        spi.xfer2(reset_command)
        self.right_motor.set_speed(0)
        self.left_motor.set_speed(0)
        self.right_motor.get_speed()
        self.left_motor.get_speed()
        self.x_coord = 0
        self.y_coord = 0
        self.lastleftdistance = 0
        self.lastrightdistance = 0
        print("Encoder values reset.")

    def routine(self):
        """Run the robot for a fixed duration with a given reference speed."""
        
        self.data_left = np.zeros(DATA_LENGTH)
        self.data_right = np.zeros(DATA_LENGTH)
        self.speed_left = np.zeros(DATA_LENGTH)
        self.speed_right = np.zeros(DATA_LENGTH)
        self.time= np.zeros(DATA_LENGTH)
        self.data_leftcontrol = np.zeros(DATA_LENGTH)
        self.data_rightcontrol = np.zeros(DATA_LENGTH)
        self.data_counter = 0
 
        self.reset_values()
        #self.go_to_position(115, 0, 0)
        self.go_to_position(100, -100, 0)   #( x, y, dest_angle) 
        self.go_to_position(200, 0, 0)
        self.go_to_position(200, -120, 0)
        # Plot trajectory
        plt.plot( self.xcoordinates, self.ycoordinates, label="Left Motor position", color='blue')

        print("Data plotted")
        plt.savefig("plot_new.png")

        
        print("You arrived at destination")


    def plot_data(self):
        """Plot speed and control signal data on two separate plots."""
        fig, axs = plt.subplots(3, 1, figsize=(10, 8))  # Create two subplots vertically

        # Plot motor position
        axs[0].plot( self.data_left, label="Left Motor position", color='red')
        axs[0].plot( self.data_right, label="Right Motor position", color='blue')
        axs[0].set_title("Position")
        axs[0].set_xlabel("Time (samples)")
        axs[0].set_ylabel("Position (cm)")
        axs[0].legend()
        axs[0].grid(True)

        # Plot speed
        axs[1].plot( self.time, self.data_left, label="Wheel speed", color='red')
        #axs[1].plot( self.data_right, label="Right Wheel speed", color='blue')
        axs[1].set_title("Speed")
        axs[1].set_xlabel("Time (seconds)")
        axs[1].set_ylabel("Speed (m/s)")
        axs[1].legend()
        axs[1].grid(True)

        # Plot control signals
        axs[2].plot(self.time, self.data_leftcontrol, label="Control Signal", color='red')
        #axs[2].plot(self.data_rightcontrol,label="Control Signal Right", color='blue')
        axs[2].set_title("Control Signals")
        axs[2].set_xlabel("Time (seconds)")
        axs[2].set_ylabel("Control (Volts)")
        axs[2].legend()
        axs[2].grid(True)

        # Adjust layout and save
        plt.tight_layout()
        plt.savefig("plot_new.png")
        plt.show()

# Main program
def main():
    corneille = Robot()
    incontrol = False
    while True:
        instr = input("Enter command (a=run, s=speed, d=distance, r=reset, c=angle, q=open loop): ")

        if instr == 'a':
            corneille.routine()

        elif instr == ' ':
            corneille.stop()

        elif instr == 's':
            print(f"Left Speed: {corneille.left_motor.get_speed()}")
            print(f"Right Speed: {corneille.right_motor.get_speed()}") 
        elif instr == 'd':
            print(f"Left Distance in centimeters: {corneille.left_motor.get_distance():.2f}")
            print(f"Right Distance in centimeters: {corneille.right_motor.get_distance():.2f}")
        elif instr == 'r':
            corneille.reset_values()
        elif instr == 'c':
            print(f"Angle of the robot : {corneille.get_angle():.2f}")
        elif instr == 'f':
            corneille.reset_values()
            while True :
            # Update position
                displacement = (corneille.left_motor.get_distance() - corneille.lastleftdistance + corneille.right_motor.get_distance() - corneille.lastrightdistance)/2

                corneille.x_coord += displacement * np.cos((corneille.get_angle()/360)*2*np.pi) *(100/70.2)
                corneille.y_coord += displacement * np.sin((corneille.get_angle()/360)*2*np.pi) *(100/70.2)

                corneille.lastleftdistance = corneille.left_motor.get_distance()
                corneille.lastrightdistance = corneille.right_motor.get_distance()
                print(f"X: {corneille.x_coord:.2f}, Y: {corneille.y_coord:.2f}, Theta: {(corneille.get_angle()):.2f}")
                sleep(SAMPLING_TIME)

        elif instr == 'o':
            corneille.start()
            start_time = time()
            while corneille.data_counter < DATA_LENGTH:
                corneille.control_loop(0.6,0.6)
                   
                timer = time()-start_time

                corneille.time[corneille.data_counter] = timer

                corneille.data_left[corneille.data_counter] = (corneille.left_motor.get_speed() + corneille.right_motor.get_speed())/2

                sleep(SAMPLING_TIME)
                print(corneille.data_counter)
                corneille.data_counter += 1
            
            corneille.stop()
            corneille.plot_data()
            print("data plotted")


        elif instr == 'q':
            corneille.start()
            GPIO.output(corneille.right_motor.direction_pin, True)
            GPIO.output(corneille.left_motor.direction_pin, False)
            corneille.right_motor.set_speed(6)
            corneille.left_motor.set_speed(6)
            start_time = time()

            while corneille.data_counter < DATA_LENGTH:
                sleep(SAMPLING_TIME)
                timer = time()-start_time

                corneille.time[corneille.data_counter] = timer

                corneille.data_left[corneille.data_counter] = (corneille.left_motor.get_speed() + corneille.right_motor.get_speed())/2
                #corneille.data_right[corneille.data_counter] = corneille.right_motor.get_speed()

                corneille.data_leftcontrol[corneille.data_counter] = 6
                corneille.data_rightcontrol[corneille.data_counter] = 6
                
                corneille.data_counter += 1
            
            corneille.stop()
            corneille.plot_data()

            # Stores the data in a file
            np.savetxt("data_left.txt", corneille.data_left)
            np.savetxt("data_leftcontrol.txt", corneille.data_leftcontrol)
            print(corneille.time[DATA_LENGTH-1]/DATA_LENGTH)


            corneille.data_counter = 0

        else:
            print("Invalid command!")





if __name__ == "__main__":
    main()








'''
# Position error
e_pos_left = reference_position - self.left_motor.get_distance()
e_pos_right = reference_position - self.right_motor.get_distance()

# Integrate error
#self.int_e_pos_left += e_pos_left * SAMPLING_TIME
#self.int_e_pos_right += e_pos_right * SAMPLING_TIME 

# Calculate reference speed
ref_speed_left = self.Kp_pos * e_pos_left #+ self.Ki_pos * self.int_e_pos_left
ref_speed_right = self.Kp_pos * e_pos_right #+ self.Ki_pos * self.int_e_pos_right
print("left ref speed : " + str(ref_speed_left))
print("right ref speed : " + str(ref_speed_right))'''