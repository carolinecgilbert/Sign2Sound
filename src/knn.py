import numpy as np
from pca2 import *
import numpy as np
from sklearn.decomposition import PCA
import cv2
from pca2 import *

def knnAlg(imgPath, imgPathTrain, debug_mode=True):
    #load in training data
    labels = np.load('train_data1_labels.npy', allow_pickle=True)
    pcaData, pcaModel = pca2(imgPathTrain, debug_mode)

    #run pca on test image
    img = cv2.imread(imgPath)
    X_test = []
    X_test.append(img.flatten())
    X_test = np.asarray(X_test)
    # print(img.flatten())
    # print(X_test)
    # print(X_test.shape)
    #X_test = X_test / 255
    X_test = (X_test - np.mean(X_test)) / np.std(X_test)
    testPca = pcaModel.transform(X_test)

    k = 3#not sure what this value should be

    distances = []
    for i in range(len(pcaData)):
        distances.append(np.sqrt(np.sum((testPca - pcaData[i]) ** 2, axis=1))[0])
    print(distances)
    nearest_indices = np.argsort(distances)[:k]
    print(f"nearest indices: {nearest_indices}")
    nearest_labels = [labels[ind] for ind in nearest_indices]
    print(f"nearest labels: {nearest_labels}")
    unique_labels, counts = np.unique(nearest_labels, return_counts=True) # Count the occurrences of each label in the nearest neighbors
    print(f"unique labels: {unique_labels}")
    print(f"counts: {counts}")
    predicted_label = unique_labels[np.argmax(counts)]
    print(f"predicted: {predicted_label}")
    print(f"Predicted label for test image: {predicted_label}")
    return predicted_label

if __name__ == "__main__":
    print("program running")
    img = 'test1.jpg'
    debug_mode = True
    letter = knnAlg(img, 'test_data/data1',debug_mode)
    #subtract plain white background from each image, take more training images, calculate accuracy, change number of components, could add cross validation