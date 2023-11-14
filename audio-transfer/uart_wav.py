import serial
import sys
import time
import wave

from wav_helper import get_data

# Open serial port
ser = serial.Serial(port='/dev/cu.usbserial-A50285BI', baudrate=115200, 
                    parity=serial.PARITY_NONE, 
                    stopbits=serial.STOPBITS_ONE,
                    bytesize=serial.EIGHTBITS,
                    timeout=1)  

letters = ['a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'k', 'l', \
     'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y']
letter_index = 0

try: 
    while True:
        # Get letter to send
        translated_letter = letters[letter_index]
        letter_index += 1
        if letter_index >= len(letters):
            letter_index = 0
        print(f"Sending {translated_letter}.wav...")

        # Get raw data of translated letter
        rawData = get_data(translated_letter)

        # Send raw data to stm32
        words_sent = 0
        for sample in rawData:
            # print(sample.to_bytes(2, byteorder='big', signed=True))
            ser.write(sample.to_bytes(2, byteorder='big', signed=False))
            time.sleep(0.00001)
            # Update bytes counter
            words_sent += 1
        # Print confirmation message
        print("File Transfer Complete")
        
        # Wait to send file again
        time.sleep(3)


except KeyboardInterrupt:
    ser.close()
    print("Keyboard Interrupt")