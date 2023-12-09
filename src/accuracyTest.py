from pca2 import *
from knn import *
import os, glob, random
import sklearn
import numpy

def retLabels(labelPath):
    labelList = []
    for img in os.listdir(labelPath):
        labelList.append(img[0])
    return labelList

def confusion(trueLabels, predLabels):
    return sklearn.metrics.confusion_matrix(trueLabels, predLabels)

def testKnn(testPath, trainPath, trainLabels, kVal):
    predLabels = []
    for img in os.listdir(testPath):
        testLabel = img[0]
        #print(testLabel)
        predlabel = knnAlg(testPath+img,trainPath,trainLabels, testLabel, kVal, debug_mode=False)
        #print(predlabel)
        predLabels.append(predlabel)
    return predLabels

def testAlg(testPath, trainPath, kVal):
    trainLabels = retLabels(trainPath)
    testLabels = retLabels(testPath)
    predLabels = testKnn(testPath, trainPath, trainLabels, kVal)
    confMat = confusion(testLabels, predLabels)
    acc = np.sum(confMat.diagonal()) / np.sum(confMat)
    return testLabels, predLabels, confMat, acc

if __name__ == "__main__":
    print('testing algorithm')
    # trainLabels = retLabels('data1/train/')
    # testLabels = retLabels('data1/test/')
    # predLabels = testKnn('data1/test/', 'data1/train/')
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
