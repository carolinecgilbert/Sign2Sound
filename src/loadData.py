import numpy as np

# data1 = np.load('train_data1_dict.npy', allow_pickle=True)
# data2 = np.load('train_data1_list.npy', allow_pickle=True)
# data3 = np.load('train_data1_labels.npy', allow_pickle=True)
# dict1 = data1.item()
#

import string
import os, glob, random
letters = string.ascii_lowercase

folderPath = 'data1/test/'
for img in os.listdir(folderPath):
    for img1 in os.listdir(folderPath+img+'/'):
        os.rename(folderPath+img+'/'+img1, folderPath+img1)
    # fileList = glob.glob('data1/'+img[0]+'/*.jpg')
    # random.shuffle(fileList)
    # for file in fileList[:4]:
    #     fileName = os.path.basename(file)
    #     os.rename(file,'data1/'+img[0]+'/'+img[0]+'test/'+os.path.splitext(fileName)[0]+'.jpg')
    # fileList = glob.glob('data1/' + img[0] + '/*.jpg')
    # for file in fileList:
    #     fileName = os.path.basename(file)
    #     os.rename(file,'data1/'+img[0]+'/'+img[0]+'train/'+os.path.splitext(fileName)[0]+'.jpg')