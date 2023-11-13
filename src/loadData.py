import numpy as np

# data1 = np.load('train_data1_dict.npy', allow_pickle=True)
# data2 = np.load('train_data1_list.npy', allow_pickle=True)
# data3 = np.load('train_data1_labels.npy', allow_pickle=True)
# dict1 = data1.item()
#

import string
import os, glob, random
letters = string.ascii_lowercase
letters = [x for x in letters]
letters.remove('j')
letters.remove('z')

# for x in letters:
#     os.mkdir('data2/'+x+'/')

folderPath = 'data2/'
for img in os.listdir('data2/train/'):
    for img1 in os.listdir('data2/train/'+img+'/'):
        os.rename('data2/train/'+img+'/'+img1,'data2/train/'+img1)
# for img in os.listdir(folderPath):
#     if(len(img)==1):
#         for img1 in os.listdir(folderPath+img+'/'):
#             if(img1[-4:] == 'rain'):
#                 os.rename(folderPath+img+'/'+img1, folderPath+'train/'+img1)
#             if(img1[-4:] == 'test'):
#                 os.rename(folderPath + img + '/' + img1, folderPath + 'test/' + img1)

    #for img1 in os.listdir(folderPath+img+'/'):
    #os.rename(folderPath+img+'/'+img1, folderPath+img1)
    # fileList = glob.glob('data2/'+img[0]+'/'+img[0]+'test/'+'/*.jpg')
    #random.shuffle(fileList)
    # for file in fileList[:2]:
    #     fileName = os.path.basename(file)
    #     os.rename(file,'data2/'+img[0]+'/'+img[0]+'train/'+os.path.splitext(fileName)[0]+'.jpg')
    # fileList = glob.glob('data2/' + img[0] + '/*.jpg')
    # for file in fileList:
    #     fileName = os.path.basename(file)
    #     os.rename(file,'data2/'+img[0]+'/'+img[0]+'train/'+os.path.splitext(fileName)[0]+'.jpg')