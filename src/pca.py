import cv2
import numpy as np
import matplotlib.pyplot as plt #delete later
import matplotlib.image as mpimg

def PCA(input_image, n_bands=3, debug_mode=True):
    # Read RGB image into an array with cv2 lib
    img = cv2.imread(input_image)
    # cv2.imshow("image",img) #delete later
    # cv2.waitKey(0)
    # cv2.destroyAllWindows()
    img_shape = img.shape[:2]
    b,g,r=cv2.split(img)
    cv2.imwrite('blue_channel.jpg',b)
    cv2.imwrite('green_channel.jpg',g)
    cv2.imwrite('red_channel.jpg',r)


    # stack images into an array
    stacked_imgs = np.zeros((img_shape[0],img_shape[1],n_bands))
    stacked_imgs[:,:,0] = cv2.imread('blue_channel.jpg', cv2.IMREAD_GRAYSCALE)
    stacked_imgs[:,:,1] = cv2.imread('green_channel.jpg', cv2.IMREAD_GRAYSCALE)
    stacked_imgs[:,:,2] = cv2.imread('red_channel.jpg', cv2.IMREAD_GRAYSCALE)

    # for i in range(n_bands):
    #     stacked_imgs[:,:,i] = cv2.imread(input_image, cv2.IMREAD_GRAYSCALE)

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

    # Find highest order of eigen value and corresponding eigen vector
    order = eigen_val.argsort()[::-1]
    eigen_val = eigen_val[order]
    eigen_vec = eigen_vec[:,order]

    # Projecting data on Eigen vector directions resulting to Principal Components
    PC = np.matmul(img_matrix, eigen_vec)   #cross product


    # Rearranging 1-d arrays to 2-d arrays of image size
    PC_2d = np.zeros((img_shape[0],img_shape[1],n_bands))

    for i in range(n_bands):
        PC_2d[:,:,i] = PC[:,i].reshape(-1,img_shape[1])

    # normalizing between 0 to 255
    PC_2d_Norm = np.zeros((img_shape[0],img_shape[1],n_bands))

    for i in range(n_bands):
        PC_2d_Norm[:,:,i] = cv2.normalize(PC_2d[:,:,i],
                        np.zeros(img_shape),0,255 ,cv2.NORM_MINMAX)
    # Comparsion of RGB and Image produced using first three bands
    # fig,axes = plt.subplots(1,2,figsize=(20,10),
    #                         sharex='all', sharey='all')
    # fig.subplots_adjust(wspace=0.1, hspace=0.15)
    #
    # img = cv2.imread(input_image)
    # axes[0].imshow(img)
    # axes[1].imshow(PC_2d_Norm[:,:,:3][:,:,[0,2,1]].astype(int))
    #plt.show()



    if (debug_mode):
        print("PC.size = ", PC.size, "\n\nPrincipal Components:\n", PC,"\n")
    return PC



if __name__ == "__main__":
    print("program running")
    img = 'test1.jpg'
    n_bands = 3
    debug_mode = True
    PC = PCA(img, n_bands, debug_mode)