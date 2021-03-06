

from __future__ import absolute_import
from __future__ import division
from __future__ import print_function
import os
import numpy as np
import tensorflow as tf
from PIL import Image


__author__ = 'Administrator'



bInitFlag = False

input_height = 299
input_width = 299
input_mean = 0
input_std = 255
input_layer = "Mul"
output_layer = "final_result"
input_name = "import/" + input_layer
output_name = "import/" + output_layer
label_path = ""
model_path = ""
path_save = ""
global graph
global sess

def Init(label_file, model_file, cachePath):
    global graph
    global labels
    global bInitFlag
    global sess
    global path_save
    if bInitFlag == False:
        labels = load_labels(label_file)
        graph = load_graph(model_file)
        sess = tf.Session(graph=graph)
        path_save = cachePath
        bInitFlag = True

def load_graph(model_file):
    graph = tf.Graph()
    graph_def = tf.GraphDef()

    with open(model_file, "rb") as f:
        graph_def.ParseFromString(f.read())
    with graph.as_default():
        tf.import_graph_def(graph_def)
    return graph


def read_tensor_from_image_file(file_name,
                                input_height=299,
                                input_width=299,
                                input_mean=128,
                                input_std=255):
    input_name = "file_reader"
    output_name = "normalized"

    # input_name :operation
    file_reader = tf.read_file(file_name, input_name)

    if file_name.endswith(".png"):
        image_reader = tf.image.decode_png(
            file_reader, channels=3, name="png_reader")
    elif file_name.endswith(".gif"):
        image_reader = tf.squeeze(
            tf.image.decode_gif(file_reader, name="gif_reader"))
    elif file_name.endswith(".bmp"):
        image_reader = tf.image.decode_bmp(file_reader, name="bmp_reader")
    else:
        image_reader = tf.image.decode_jpeg(
            file_reader, channels=3, name="jpeg_reader")
    float_caster = tf.cast(image_reader, tf.float32)
    dims_expander = tf.expand_dims(float_caster, 0)
    resized = tf.image.resize_bilinear(dims_expander, [input_height, input_width])
    normalized = tf.divide(tf.subtract(resized, [input_mean]), [input_std])
    sess = tf.Session()
    result = sess.run(normalized)
    return result


def load_labels(label_file):
    label = []
    proto_as_ascii_lines = tf.gfile.GFile(label_file).readlines()
    for l in proto_as_ascii_lines:
        label.append(l.rstrip())
    return label

def del_file(path):
    ls = os.listdir(path)
    for i in ls:
        c_path = os.path.join(path, i)
        if os.path.isdir(c_path):
            del_file(c_path)
        else:
            os.remove(c_path)

def shutter_up(image):
    box = (160, 100, 450, 450)
    im = image.crop(box)
    return im

def shutter_down(image):
    box = (135, 490, 475, 690)
    im = image.crop(box)

    return im


#if __name__ == "__main__":
#    image_path = "D:\\Cache\\GithubCache\\CarSeatImage\\20180805_164224.jpg"
#    modelPath="C:\\Users\\Administrator\\Documents\\Downloads\\test1-master\\carseat\\part-classify\\output_graph.pb"
#    labelPath="C:\\Users\\Administrator\\Documents\\Downloads\\test1-master\\carseat\\part-classify\\output_labels.txt"
def seatClassify(image_path, labelPath, modelPath, cachePath):
    global bInitFlag
    global graph
    global input_height
    global input_width
    global input_mean
    global input_std
    global input_layer
    global output_layer
    global input_name
    global output_name
    global labels
    global sess
    global path_save

    #PATH_TO_TEST_IMAGES_DIR = r'/home/yqw/CarSeat_Recognization_Picture/test'
    #IMAGES = os.listdir(PATH_TO_TEST_IMAGES_DIR)
    # if len(IMAGES) == 0:
    #     tf.logging.warning('No files found')
    if bInitFlag == False:
        Init(labelPath, modelPath, cachePath)
        bInitFlag = True

    input_height = 299
    input_width = 299
    input_mean = 128
    input_std = 128
    input_layer = "Mul"
    output_layer = "final_result"
    input_name = "import/" + input_layer
    output_name = "import/" + output_layer
    input_operation = graph.get_operation_by_name(input_name)
    output_operation = graph.get_operation_by_name(output_name)

    sess = tf.Session(graph=graph)
    #with tf.Session(graph=graph) as sess:

        # path_save = '/home/yqw/Github/test'
        # # os.path.isfile('test.txt') #if not exits then return False
        # # os.path.exists(directory) #if directory not exits then return False
        # if os.path.exists(path_save):
        #     pass
        # else:
        #     os.makedirs(path_save)
        # for image in IMAGES:
        #
        #     image_path = os.path.join(PATH_TO_TEST_IMAGES_DIR, image)
    index = image_path.rfind('\\')
    #path_save = image_path[:index]
    #path_save = "D:\\Cache\\GithubCache\\tmpImage"
    image = image_path[index + 1:]
    #print(image)

    image_read = Image.open(image_path)

    image_read = image_read.resize((624, 832))

    #img_up = shutter_up(image_read)
    #img_down = shutter_down(image_read)

    #img_up.save(os.path.join(path_save, image[:-4] + "_up.jpg"))
    #img_down.save(os.path.join(path_save, image[:-4] + "_down.jpg"))

    #image_save_dir = os.listdir(path_save)
    reValue = []

    #for image_shutt in image_path:
    image_save_path = image_path;#os.path.join(path_save, image_shutt)
    t = read_tensor_from_image_file(
        image_save_path,
        input_height=input_height,
        input_width=input_width,
        input_mean=input_mean,
        input_std=input_std)

    results = sess.run(output_operation.outputs[0], {
        input_operation.outputs[0]: t
    })

    results = np.squeeze(results)
    top_k = results.argsort()[-1:][::-1]
    #labels = load_labels(label_file)
    list_map = []
    #print(top_k)
    #for i in top_k:
    #print(image_shutt, " ", labels[top_k], results[top_k])
    reValue.append(labels[top_k[0]])
    reValue.append(results[top_k[0]])
    reValue.append(labels[top_k[0]])
    reValue.append(results[top_k[0]])
    #os.remove(os.path.join(path_save, image_shutt))
    #os.removedirs(path_save)
    return tuple(reValue)