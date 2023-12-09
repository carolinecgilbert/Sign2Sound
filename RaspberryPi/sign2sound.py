import time
from sendLetter import send_letter
from sendAudio import send_audio
from accTest4 import *
import RPi.GPIO as GPIO
from picamera import PiCamera
from PIL import Image
import subprocess
import os

# logoImg = Image.open('')
# logoImg.show()
# subP = subprocess.Popen(["display",""])

GPIO.setmode(GPIO.BOARD)
button=16
camera = PiCamera()
camera.framerate = 15
camera.awb_mode = 'fluorescent'
GPIO.setup(button, GPIO.IN, pull_up_down=GPIO.PUD_UP)

i = 0

try:
    while True:
        # Check if button is pressed
        input1 = GPIO.input(button)
        
        if input1 == False:
           
           # GPIO.output(led, GPIO.HIGH)
           #print("Button pressed")
           # Stream camera and capture image
           #while(not input1):
                   
                time.sleep(0.001)
                input1 = GPIO.input(button)
                imgPath = 'test.jpg'
                camera.start_preview()
                i += 1
                if (i == 1):
                        img = Image.open('omg.png')
                        img.resize((256, 256))
                        pad = Image.new('RGB', (((img.size[0] + 31) // 32) * 32, ((img.size[1] + 15) // 16) * 16,))
                        pad.paste(img, (0, 0))
                        x = (camera.resolution.width - pad.width) // 2
                        y = (camera.resolution.height - pad.height) // 2
                        o = camera.add_overlay(pad.tobytes(), size=(256, 256), layer=3, alpha=80, window=(x, y, 256, 256))
                time.sleep(3)
                camera.capture(imgPath)
                time.sleep(0.01)
                camera.stop_preview()
                img = Image.open(imgPath)
                imgCrop = img.crop((160, 0, 640, 480))
                imgCrop.save(imgPath)
                translated_letter = testLda('test.jpg')
                #print(f'test letter: {translated_letter}')
           

                # Send letter to stm32
                print(f'\nLetter To Send: {translated_letter}')
                send_letter(translated_letter)
                # input1=GPIO.input(button)
                time.sleep(3)

                # Send audio to stm32
                send_audio(translated_letter)

        # Wait to send next letter
                print('Translation Complete!\n')

                # Remove image
                try:
                        os.remove('test.jpg')
                except OSError as e:
                        print("Error: %s : %s" % ('test.jpg', e.strerror))
        # time.sleep(8)
    
except Exception as e:
    print("sign2sound.py Exception: " + str(e))
