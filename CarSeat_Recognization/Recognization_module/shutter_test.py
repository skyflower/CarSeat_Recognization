from __future__ import absolute_import
from __future__ import division
from __future__ import print_function
import os
import numpy as np
import tensorflow as tf
from PIL import Image


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

    # input_name :operation的名字
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


def shutter_up(image):
    box = (160, 100, 450, 450)
    im = image.crop(box)
    return im


def shutter_down(image):
    box = (135, 490, 475, 690)
    im = image.crop(box)

    return im


if __name__ == "__main__":

    PATH_TO_TEST_IMAGES_DIR = r'/home/yqw/CarSeat_Recognization_Picture/test'
    IMAGES = os.listdir(PATH_TO_TEST_IMAGES_DIR)
    if len(IMAGES) == 0:
        tf.logging.warning('No files found')
    model_file = \
        r'/home/yqw/CarSeat_Recognization/CarSeat_Recognization/Recognization_module/part-classify/output_graph.pb'
    graph = load_graph(model_file)
    label_file = r"/home/yqw/CarSeat_Recognization/CarSeat_Recognization/Recognization_module/part-classify/output_labels.txt"
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

    with tf.Session(graph=graph) as sess:

        path_save = '/home/yqw/Github/test'
        # os.path.isfile('test.txt') #如果不存在就返回False
        # os.path.exists(directory) #如果目录不存在就返回False
        if os.path.exists(path_save):
            pass
        else:
            os.makedirs(path_save)
        for image in IMAGES:

            image_path = os.path.join(PATH_TO_TEST_IMAGES_DIR, image)
            image_read = Image.open(image_path)
            img_up = shutter_up(image_read)
            img_down = shutter_down(image_read)

            img_up.save(os.path.join(path_save, image[:-4] + "_up.jpg"))
            img_down.save(os.path.join(path_save, image[:-4] + "_down.jpg"))

            image_save_dir = os.listdir(path_save)

            for image_shutt in image_save_dir:
                image_save_path = os.path.join(path_save, image_shutt)
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
                labels = load_labels(label_file)
                list_map = []
                for i in top_k:
                    print(image_shutt, " ", labels[i], results[i])
                os.remove(os.path.join(path_save, image_shutt))
        os.removedirs(path_save)
