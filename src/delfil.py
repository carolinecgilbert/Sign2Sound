import os

for img in os.listdir('data1/train/'):
    if(int(img[1:-4])  > 20):
        os.remove('data1/train/'+img)
    print(int(img[1:-4]))