import cv2
import numpy as np


def PCA(input_image, n_bands=3, debug_mode=True):
    # Read RGB image into an array with cv2 lib
    img = cv2.imread(input_image)
    img_shape = img.shape[:2]

    # stack images into an array
    stacked_imgs = np.zeros((img_shape[0],img_shape[1],n_bands))
    for i in range(n_bands):
        stacked_imgs[:,:,i] = cv2.imread('color-glove'+str(i+1)+'.jpg', 
                                   cv2.IMREAD_GRAYSCALE)  

    # Flatten stacked images into standardized 1D array
    img_matrix = np.zeros((stacked_imgs[:,:,0].size,n_bands))
    for i in range(n_bands):
        img_arr = stacked_imgs[:,:,i].flatten() 
        img_arr_std = (img_arr - img_arr.mean())/img_arr.std()  
        img_matrix[:,i] = img_arr_std

    # Calculate covariance and eigen values/vectors
    np.set_printoptions(precision=3)
    cov = np.cov(img_matrix.transpose())
    eigen_val, eigen_vec = np.linalg.eig(cov)

    # Find highest order of egien value and corresponding eigen vector
    order = eigen_val.argsort()[::-1]
    eigen_val = eigen_val[order]
    eigen_vec = eigen_vec[:,order]

    # Projecting data on Eigen vector directions resulting to Principal Components 
    PC = np.matmul(img_matrix, eigen_vec)   #cross product
    if (debug_mode):
        print("PC.size = ", PC.size, "\n\nPrincipal Components:\n", PC,"\n")
    return PC

def main():
    img = 'src/color-glove1.jpg'
    n_bands = 1
    debug_mode = True
    PC = PCA(img, n_bands, debug_mode)
    
if __name__ == "__main__":
    main()

