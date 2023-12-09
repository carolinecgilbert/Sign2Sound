import cv2
import numpy as np
import matplotlib.pyplot as plt #delete later
import matplotlib.image as mpimg
import os
from sklearn.decomposition import PCA
from sklearn.preprocessing import StandardScaler
import pickle

def pca2(imgPath, debug_mode=True):
    X_train = []
    blankImg = cv2.imread('blank.jpg')
    for image in os.listdir(imgPath):
        print(image)
        img = cv2.imread(imgPath + '/' + image)
        newImg = cv2.subtract(img, blankImg)
        X_train.append(newImg.flatten()) #subtract blank white background from it
    print(X_train)
    X_train = np.asarray(X_train)
    print(X_train)
    #X_train = X_train / 255      size is 384 x 1,152,000
    mean1 = np.mean(X_train, axis=0)
    std1 = np.std(X_train, axis=0)
    X_train = (X_train - mean1) / std1
    print('got here')
    pca = PCA(n_components=383)
    print('got here')
    pcaFit = pca.fit(X_train)
    print('1')
    trainPca = pcaFit.transform(X_train)
    print('2')

    pickle.dump(pcaFit, open('pcaMod.pkl', 'wb'))
    return trainPca

if __name__ == "__main__":
    print("program running")
    img = 'data1/train'
    debug_mode = True
    PC = pca2(img, debug_mode)
    np.save('train_data1_pca.npy', PC)