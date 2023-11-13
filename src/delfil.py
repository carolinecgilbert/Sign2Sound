import os

for img in os.listdir('data2/train/'):
    if(int(img[1:-4])  > 20):
        os.remove('data2/train/'+img)
    print(int(img[1:-4]))