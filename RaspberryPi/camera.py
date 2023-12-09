from picamera import PiCamera
import time
camera = PiCamera()
camera.start_preview()
time.sleep(3)
camera.capture('test_data/data1/y20.jpg')
camera.stop_preview()
