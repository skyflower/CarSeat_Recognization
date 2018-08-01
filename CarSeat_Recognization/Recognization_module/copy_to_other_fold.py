import os
import shutil

path = "/home/yqw/CarSeat_Recognization_Picture/shutter_pic"

image_dirs = os.listdir(path)

for image_dir in image_dirs:

    images = os.listdir(os.path.join(path, image_dir))

    for image in images:

        if image.endswith("up.jpg"):

            shutil.copy(os.path.join(path, image_dir, image), os.path(path, image_dir, 'up'))
            # shutil.copy(os.path.join(file_dir, image), os.path.join(file_dir, 'class_name'))
        else:
            shutil.copy(os.path.join(path, image_dir, image), os.path(path, image_dir, 'down'))
