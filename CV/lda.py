import numpy as np
from sklearn.discriminant_analysis import LinearDiscriminantAnalysis as LDA
import cv2
import os
import pickle
from sklearn.preprocessing import StandardScaler

def lda(imgPath):
    #process training data
    #blankImg = cv2.imread('blank.jpg')
    X_train = []
    y_train = []
    for image in os.listdir(imgPath):
        y_train.append(image[0])
        print(image)
        img = cv2.imread(imgPath + '/' + image)
        #newImg1 = cv2.resize(img, (400, 240))
        #newImg = cv2.subtract(img, blankImg)
        #X_train.append(newImg.flatten())
        X_train.append(img.flatten())
    X_train = np.asarray(X_train)
    ss = StandardScaler()
    # X_train = ss.fit_transform(X_train)
    # ss.fit(X_train)
    # mean1 = ss.mean_
    #
    mean1 = np.mean(X_train, axis=0)
    std1 = np.std(X_train, axis=0)
    # print(std1)
    X_train = (X_train - mean1) / std1
    # print(X_train)

    ldaMod = LDA(n_components=23)
    ldaMod.fit(X_train, y_train)
    pickle.dump(ldaMod, open('lda.pkl', 'wb'))
    return 0

if __name__ == "__main__":
    print("program running")
    img = 'data2/train/'
    i = lda(img)
    print("done")