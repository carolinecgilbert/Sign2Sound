
from IPython.display import Image, display
import matplotlib.pyplot as plt
import pandas as pd
import seaborn as sns
import cv2
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.gridspec as grid


def PCA(input_image, n_bands=7, debug_mode=True):
    # Read RGB image into an array
    img = cv2.imread(input_image)
    img_shape = img.shape[:2]

    if (debug_mode):
        print('image size = ',img_shape)

    # 3 dimensional dummy array with zeros
    MB_img = np.zeros((img_shape[0],img_shape[1],n_bands))
    # stacking up images into the array
    for i in range(n_bands):
        MB_img[:,:,i] = cv2.imread('color-glove'+str(i+1)+'.jpg', 
                                   cv2.IMREAD_GRAYSCALE)  

    MB_matrix = np.zeros((MB_img[:,:,0].size,n_bands))
    for i in range(n_bands):
        MB_array = MB_img[:,:,i].flatten()  # covert 2d to 1d array 
        MB_arrayStd = (MB_array - MB_array.mean())/MB_array.std()  
        MB_matrix[:,i] = MB_arrayStd
    MB_matrix.shape

    # Covariance
    np.set_printoptions(precision=3)
    cov = np.cov(MB_matrix.transpose())

    # Eigen Values
    EigVal,EigVec = np.linalg.eig(cov)
    if (debug_mode):
        print("Eigenvalues:\n\n", EigVal,"\n")

    # Ordering Eigen values and vectors
    order = EigVal.argsort()[::-1]
    EigVal = EigVal[order]
    EigVec = EigVec[:,order]

    #Projecting data on Eigen vector directions resulting to Principal Components 
    PC = np.matmul(MB_matrix,EigVec)   #cross product
    if (debug_mode):
        print("PC.size = ", PC.size)
        print("Principal Components:\n\n", PC,"\n")
    return PC

def main():
    img = 'color-glove1.jpg'
    n_bands = 3
    debug_mode = True
    PC = PCA(img, n_bands, debug_mode)
    print('main() is done')
    
if __name__ == "__main__":
    main()

