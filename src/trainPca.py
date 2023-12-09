from pca2 import *
import numpy as np
import string
import os
import numpy as np

labels = []

#os.remove('train_data1_pca.npy')
os.remove('train_data1_labels.npy')

imgDir = 'test_data/data1'
i=0
for image in os.listdir(imgDir):
    labels.append(image[0]) #add letter label
    print(image)
    i+=1
print(i)
# pca3 = pca2(imgDir, False)
# np.save('train_data1_pca.npy', pca3)
np.save('train_data1_labels.npy', labels)
