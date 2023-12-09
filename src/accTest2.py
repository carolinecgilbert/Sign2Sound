from pca2 import *
from knn3 import *
import os, glob, random
import sklearn
import numpy

#test kNN without PCA
def retLabels(labelPath):
    labelList = []
    for img in os.listdir(labelPath):
        labelList.append(img[0])
    return labelList

def confusion(trueLabels, predLabels):
    return sklearn.metrics.confusion_matrix(trueLabels, predLabels)

def testKnn(testPath, Xtrain, ytrain, kVal):
    ypreds = []
    blankImg = cv2.imread('blank.jpg')
    for img in os.listdir(testPath):
        ytest = img[0]
        testImg = cv2.imread(testPath + img)
        newImg = cv2.subtract(testImg, blankImg)
        Xtest = []
        Xtest.append(newImg.flatten())
        Xtest = (Xtest - np.mean(Xtest)) / np.std(Xtest)
        ypred = knnAlg3(Xtrain, ytrain, Xtest, ytest, kVal)
        ypreds.append(ypred)
    return ypreds

def testAlg(testPath, trainPath, kVal):
    ytrain = retLabels(trainPath)
    ytest = retLabels(testPath)

    blankImg = cv2.imread('blank.jpg')
    Xtrain = []
    for img in os.listdir(trainPath):
        trainImg = cv2.imread(trainPath + img)
        newImg = cv2.subtract(trainImg, blankImg)
        Xtrain.append(newImg.flatten())
    Xtrain = np.asarray(Xtrain)
    mean1 = np.mean(Xtrain, axis=0)
    std1 = np.std(Xtrain, axis=0)
    Xtrain = (Xtrain - mean1) / std1

    ypred = testKnn(testPath, Xtrain, ytrain, kVal)
    confMat = confusion(ytest, ypred)
    acc = np.sum(confMat.diagonal()) / np.sum(confMat)
    return ytest, ypred, confMat, acc

if __name__ == "__main__":
    print('testing algorithm')
    # ytrain = retLabels('data1/train/')
    # ytest = retLabels('data1/test/')

    kList = [3]
    accList = []
    for kVal in kList:
        testLabels, predLabels, confMat, acc = testAlg('data1/test/', 'data1/train/', kVal)
        print(f'{confMat}')
        disp = sklearn.metrics.ConfusionMatrixDisplay(confusion_matrix=confMat, display_labels=np.array(['a','b','c','d','e','f','g','h','i','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y']))
        disp.plot()
        plt.show()
        accList.append(acc)
        print(f'Accuracy:{acc}')
    print(kList[accList.index(max(accList))])
    print(max(accList))
