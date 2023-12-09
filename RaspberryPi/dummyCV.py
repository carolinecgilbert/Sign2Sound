# Dummy CV Algorithm for integration purposes
import time

def dummy_algo(letter_index):
	# Iterate through alphabet to send letters and audio
	letters = ['a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'k', 'l', \
		'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y']
	translated_letter = letters[letter_index]
	
	# Dummy computation time
	time.sleep(2)
		
	return translated_letter
