import numpy as np
from pca2 import *
import numpy as np
from sklearn.decomposition import PCA
import cv2
from pca2 import *
import pickle

#knn used without pca
def knnAlg3(Xtrain, ytrain, Xtest, ytest, kVal):
    k = kVal #not sure what this value should be

    distances = []
    for i in range(len(Xtrain)):
        distances.append(np.sqrt(np.sum((Xtest - Xtrain[i]) ** 2, axis=1))[0])
    #print(distances)
    nearest_indices = np.argsort(distances)[:k]
    print(f"nearest indices: {nearest_indices}")
    nearest_labels = [ytrain[ind] for ind in nearest_indices]
    print(f"nearest labels: {nearest_labels}")
    unique_labels, counts = np.unique(nearest_labels, return_counts=True) # Count the occurrences of each label in the nearest neighbors
    print(f"unique labels: {unique_labels}")
    print(f"counts: {counts}")
    #maxCounts = [i for i, x in enumerate(counts) if x == max(counts)]
    # if len(maxCounts) > 1:
    #     distCounts = []
    #     for ind in maxCounts:
    #         distCounts.append(distances[ind])
    #     predicted_label = nearest_labels[distCounts.index(min(distCounts))]
    # else:
    predicted_label = unique_labels[np.argmax(counts)]
    #print(f"predicted: {predicted_label}")
    print(f"Predicted label for test image: {predicted_label}")
    print(f"Actual label for test image: {ytest}")
    return predicted_label

if __name__ == "__main__":
    print("program running")
    trainLabels = []
    for img in os.listdir('data1/train/'):
        trainLabels.append(img[0])
    img = 'test1.jpg'
    debug_mode = True

    #could add cross validation, take all images with printer paper on whiteboard and w signs not cut off, try LDA, data augmentation