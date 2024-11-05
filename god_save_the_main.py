from time import sleep
import RPi.GPIO as GPIO

class Robot():

    def __init__(self):
        GPIO.setmode(GPIO.BCM)
        GPIO.setup((12, 13), GPIO.OUT)
        GPIO.setup((5, 6), GPIO.OUT)

        self.speed = 0
        self.direction = "idle"

        self.leftM = GPIO.PWM(12, 2000) # channel=12 frequency=2000Hz
        self.rightM = GPIO.PWM(13, 2000)   

        self.leftM.start(self.speed)  # 0% duty cycle
        self.rightM.start(self.speed)

    def accelerate(self):
        self.speed += 5
        self.leftM.start(self.speed)
        self.rightM.start(self.speed)

    def deccelarate(self):
        self.speed -= 5
        self.leftM.start(self.speed)
        self.rightM.start(self.speed)

    def stop(self):
        self.leftM.stop()
        self.rightM.stop()

    def forward(self):
        GPIO.output((5, 6), True) #
        self.direction = "forward"

    def backward(self):
        GPIO.output((5, 6), False)
        self.direction = "backward"

    def left(self):
        GPIO.output((5, 6), (False, True)) # 
        self.leftM.start(self.speed/2) 
        self.rightM.start(self.speed/2) 
        self.direction = "left"

    def right(self):
        GPIO.output((5, 6), (True, False))
        self.leftM.start(self.speed/2)
        self.rightM.start(self.speed/2)
        self.direction = "right"

    def cleanRobot(self):
        GPIO.output((12, 13), GPIO.LOW)
        GPIO.output((5, 6), False)


corneille = Robot()

while True:
    instr = input()
    if instr == 'z':
        if corneille.direction == 'forward':
            corneille.accelerate()
        else:
            corneille.forward()
    elif instr == 's':
        if corneille.direction == 'backward':
            corneille.accelerate()
        else:
            corneille.backward()
    elif instr == 'q':
        if corneille.direction == 'left':
            corneille.accelerate()
        else:
            corneille.left()
    elif instr == 'd':
        if corneille.direction == 'right':
            corneille.accelerate()
        else:
            corneille.right()
    elif instr == ' ':
        corneille.stop()
        corneille.cleanRobot()
        break


corneille.cleanRobot()