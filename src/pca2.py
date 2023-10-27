import cv2
import numpy as np
import matplotlib.pyplot as plt #delete later
import matplotlib.image as mpimg
import os
from sklearn.decomposition import PCA
from sklearn.preprocessing import StandardScaler

def pca2(imgPath, debug_mode=True):
    X_train = []
    for image in os.listdir(imgPath):
        img = cv2.imread(imgPath + '/' + image)
        X_train.append(img.flatten())
    X_train = np.asarray(X_train)
    #X_train = X_train / 255
    mean1 = np.mean(X_train, axis=0)
    std1 = np.std(X_train, axis=0)
    X_train = (X_train - mean1) / std1
    pca = PCA(n_components=24)
    pcaFit = pca.fit(X_train)
    trainPca = pcaFit.transform(X_train)
    return trainPca, pcaFit

if __name__ == "__main__":
    print("program running")
    img = 'test_data/data1'
    debug_mode = True
    PC = pca2(img, debug_mode)