from PIL import Image
from pylab import *
import os

"""
# test.py
image = Image.open('mifeng.png')
box = (250,100,550,400)
image2 = image.crop(box)
im = array(image2)
image2.save('1.jpg')
# imshow(array(image))
imshow(im)
show()
"""

path = '/home/yqw/CarSeat_Recognization_Picture/backupImage'

image_fold = os.listdir(path)
print(image_fold)


def shutter_up(image):
    box = (160, 100, 450, 450)
    im = image.crop(box)
    return im


def shutter_down(image):
    box = (135, 490, 475, 690)
    im = image.crop(box)
    return im


for image_dir in image_fold:

    images = os.listdir(os.path.join(path, image_dir))

    for image in images:
        img = Image.open(os.path.join(path, image_dir, image))
        im_up = shutter_up(img)
        im_down = shutter_down(img)
        im_up.save(image[:-4] + "_up.jpg")
        im_down.save(image[:-4] + "_down.jpg")
