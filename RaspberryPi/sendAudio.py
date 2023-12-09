import serial
import sys
import time
import wave

from wav_helper import get_data

def send_audio(letter):
    # Open serial port
    ser = serial.Serial(port='/dev/serial0', baudrate=115200, 
                        parity=serial.PARITY_NONE, 
                        stopbits=serial.STOPBITS_ONE,
                        bytesize=serial.EIGHTBITS,
                        timeout=1)  

    # Send wav file to stm32
    try: 
        # Get raw data of translated letter
        rawData = get_data(letter)
        # Send raw data to stm32
        words_sent = 0
        for sample in rawData:
            # print(sample.to_bytes(2, byteorder='big', signed=True))
            ser.write(sample.to_bytes(2, byteorder='big', signed=False))
            time.sleep(0.0001)
            # Update bytes counter
            words_sent += 1
        # Print confirmation message
        print(f"Sent Audio: {letter}.wav")

        # Close serial port
        ser.close()


    except Exception as e:
        if ser.isOpen():
            ser.close()
        print("sendAudio() Exception: " + str(e))
