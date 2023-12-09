from lda import *
import os, sklearn, cv2
import matplotlib.pyplot as plt
from sklearn.preprocessing import StandardScaler
import numpy as np

def retLabels(labelPath):
    labelList = []
    for img in os.listdir(labelPath):
        labelList.append(img[0])
    return labelList


def confusion(trueLabels, predLabels):
    return sklearn.metrics.confusion_matrix(trueLabels, predLabels)

def testLda(testImg):
    ldaMod = pickle.load(open("lda.pkl", 'rb'))
    #blankImg = cv2.imread('blank.jpg')
    predLetters = []
    for image in os.listdir(testImg):
        X_train = []
        img = cv2.imread(testImg + '/' + image)
        #newImg1 = cv2.resize(img, (800,480))
        #newImg = cv2.subtract(img, blankImg)
        #X_train.append(newImg.flatten())
        X_train.append(img.flatten())
        X_train = np.asarray(X_train)
        # ss = StandardScaler()
        # X_train = ss.fit_transform(X_train)
        X_train = (X_train - np.mean(X_train)) / np.std(X_train)
        predLetter = ldaMod.predict(X_train)
        print(predLetter[0])
        predLetters.append(predLetter[0])
    return predLetters

def testAlg(testPath, trueLabels):
    predLabels = testLda(testPath)
    confMat = confusion(trueLabels, predLabels)
    acc = np.sum(confMat.diagonal()) / np.sum(confMat)
    return acc, confMat, predLabels

if __name__ == "__main__":
    print('testing algorithm')
    trueLabels = retLabels('data2/test/')
    print(trueLabels)
    acc, confMat, predLabels = testAlg('data2/test/', trueLabels)
    print(f'{confMat}')
    disp = sklearn.metrics.ConfusionMatrixDisplay(confusion_matrix=confMat, display_labels=np.array(
        ['a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w',
         'x', 'y']))
    disp.plot()
    plt.show()
    print(f'Accuracy: {acc*100}%')

