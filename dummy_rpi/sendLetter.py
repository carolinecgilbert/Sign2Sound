import serial
import sys
import time

def send_letter(letter):
    # Open serial port
    ser = serial.Serial(port='/dev/ttyAMA1', baudrate=9600, 
                        parity=serial.PARITY_NONE, 
                        stopbits=serial.STOPBITS_ONE,
                        bytesize=serial.EIGHTBITS,
                        timeout=1)  

    try: 
        # Send letter to stm32
        ser.write(letter.encode())
        # Print confirmation message
        print(f"Sent Letter: {letter}")
        ser.close()


    except Exception as e:
        if ser.isOpen():
            ser.close()
        print("sendLetter() Exception: " + str(e))
