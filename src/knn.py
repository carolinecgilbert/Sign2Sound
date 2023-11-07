import numpy as np
from pca2 import *
import numpy as np
from sklearn.decomposition import PCA
import cv2
from pca2 import *
import pickle

def knnAlg(testPath, trainPath, trainLabels, testLabel, kVal, debug_mode=True):
    #load in training data
    #labels = np.load('train_data1_labels.npy', allow_pickle=True)
    # labels = []
    # for img in os.listdir(imgPathTrain):
    #     labels.append(img[0])
    pcaModel = pickle.load(open("pcaMod.pkl",'rb'))
    pcaData = np.load('train_data1_pca.npy', allow_pickle=True)
    #pcaData, pcaModel = pca2(trainPath, debug_mode)


    #run pca on test image
    img = cv2.imread(testPath)
    blankImg = cv2.imread('blank.jpg')
    newImg = cv2.subtract(img, blankImg)
    X_test = []
    X_test.append(newImg.flatten())
    X_test = np.asarray(X_test)
    X_test = (X_test - np.mean(X_test)) / np.std(X_test)
    testPca = pcaModel.transform(X_test)

    k = kVal #not sure what this value should be

    distances = []
    for i in range(len(pcaData)):
        distances.append(np.sqrt(np.sum((testPca - pcaData[i]) ** 2, axis=1))[0])
    #print(distances)
    nearest_indices = np.argsort(distances)[:k]
    print(f"nearest indices: {nearest_indices}")
    nearest_labels = [trainLabels[ind] for ind in nearest_indices]
    print(f"nearest labels: {nearest_labels}")
    unique_labels, counts = np.unique(nearest_labels, return_counts=True) # Count the occurrences of each label in the nearest neighbors
    print(f"unique labels: {unique_labels}")
    print(f"counts: {counts}")
    maxCounts = [i for i, x in enumerate(counts) if x == max(counts)]
    # if len(maxCounts) > 1:
    #     distCounts = []
    #     for ind in maxCounts:
    #         distCounts.append(distances[ind])
    #     predicted_label = nearest_labels[distCounts.index(min(distCounts))]
    # else:
    predicted_label = unique_labels[np.argmax(counts)]
    #print(f"predicted: {predicted_label}")
    print(f"Predicted label for test image: {predicted_label}")
    print(f"Actual label for test image: {testLabel}")
    return predicted_label

if __name__ == "__main__":
    print("program running")
    trainLabels = []
    for img in os.listdir('data1/train/'):
        trainLabels.append(img[0])
    img = 'test1.jpg'
    debug_mode = True
    letter = knnAlg(img, 'data1/train/', trainLabels, 'a', 3, debug_mode)

    #could add cross validation, take all images with printer paper on whiteboard and w signs not cut off, try LDA, data augmentation