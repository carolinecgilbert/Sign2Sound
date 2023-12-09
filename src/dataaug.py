import cv2
import numpy as np
from PIL import Image
import os
import glob

inputFolder = 'data2/train/'
folderlen = len(inputFolder)

i = 0
counter = 21
for img in os.listdir(inputFolder):
    rotater = 1
    letter = img[0]
    for i in range(5, 6):
        #Rotation
        counter += 1
        rotater += 0.1
        image = cv2.imread(inputFolder + img)
        height, width = image.shape[:2]
        rotation_matrix = cv2.getRotationMatrix2D((width/2,height/2),rotater,.99)
        rotated_image = cv2.warpAffine(image,rotation_matrix,(width,height))
        cv2.imwrite(inputFolder + img[0] + str(counter) + '.jpg', rotated_image)
        counter += 1

        #Increase Brightness
    for j in range(5, 6):
        bright = np.ones(image.shape, dtype="uint8") * j
        brightincrease = cv2.add(image,bright)
        cv2.imwrite(inputFolder + img[0] + str(counter) + '.jpg', brightincrease)
        counter += 1

        #Decrease Brightness
    for k in range(5, 6):
        bright2 = np.ones(image.shape , dtype="uint8") * k
        brightdecrease = cv2.subtract(image,bright2)
        cv2.imwrite(inputFolder + img[0] + str(counter) + '.jpg', brightdecrease)
        counter += 1
   
    
cv2.imshow('Rotated' , rotated_image)    
cv2.waitKey(0)

cv2.imshow('Increased Brightness' , brightincrease)    
cv2.waitKey(0)

cv2.imshow('Decreased Brightness' , brightdecrease)    
cv2.waitKey(0)

cv2.destroyAllWindows()