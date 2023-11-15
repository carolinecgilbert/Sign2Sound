import time
from sendLetter import send_letter
from sendAudio import send_audio
from dummyCV import dummy_algo

# Pi Camera and Button Setup
import RPi.GPIO as GPIO
import time
from picamera import PiCamera

GPIO.setmode(GPIO.BOARD)
led=12
button=16
camera = PiCamera()
# GPIO.setup(led, GPIO.OUT)
GPIO.setup(button, GPIO.IN, pull_up_down=GPIO.PUD_UP)

# Variable for dummy CV algorithm
letter_index = 0

try:
    while True:
        # Check if button is pressed
        input = GPIO.input(button)
        if input == False:
           # GPIO.output(led, GPIO.HIGH)
           print("Button pressed")
           # Stream camera and capture image
           while(not input):
              time.sleep(0.001)
              input = GPIO.input(button)
              camera.start_preview()
              time.sleep(3)
              camera.capture('test.jpg')
              camera.stop_preview()
           
           # Run CV algorithm on image to get translation
           print('Running CV Algorithm')
           translated_letter = dummy_algo(letter_index)
           # Update letter index for dummy CV algorithm
           letter_index += 1
           if (letter_index >= 24):
                   letter_index = 0
                   
           # Send letter to stm32
           print(f'\nLetter To Send: {translated_letter}')
           send_letter(translated_letter)
           time.sleep(3)

           # Send audio to stm32
           send_audio(translated_letter)

           # Wait to send next letter
           print('Translation Complete!\n')
           # time.sleep(8)
    
except Exception as e:
    print("sign2sound.py Exception: " + str(e))
