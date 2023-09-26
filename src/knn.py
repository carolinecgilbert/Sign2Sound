import numpy as np
from pca import *

# Perform PCA on sample images
glovePC = PCA('color-glove1.jpg', 1, False)
dogPC = PCA('dog.webp', 1, False)
catPC = PCA('cat.webp', 1, False)

# Sample data points (features) and their corresponding labels
X = np.array([[1, 2], [2, 3], [3, 4], [5, 6], [6, 7]])
y = np.array([0, 0, 1, 1, 1])

# New data point to classify
new_point = np.array([4, 5])

# Define the value of K (number of neighbors to consider)
k = 3

# Calculate Euclidean distances between the new point and all data points
distances = np.sqrt(np.sum((X - new_point) ** 2, axis=1))

# Sort the distances and get the indices of the K nearest neighbors
nearest_indices = np.argsort(distances)[:k]

# Get the labels of the K nearest neighbors
nearest_labels = y[nearest_indices]

# Count the occurrences of each label in the nearest neighbors
unique_labels, counts = np.unique(nearest_labels, return_counts=True)

# Predict the label for the new data point as the one with the highest count
predicted_label = unique_labels[np.argmax(counts)]

print(f"Predicted label for the new point {new_point}: {predicted_label}")
