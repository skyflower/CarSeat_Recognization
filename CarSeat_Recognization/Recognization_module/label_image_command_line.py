#encoding=utf-8
from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

import os
import shutil
import argparse
import numpy as np
import pandas as pd
import tensorflow as tf
import time



bInitFlag = False
input_height = 299
input_width = 299
input_mean = 0
input_std = 255
input_layer = "Mul"
output_layer = "final_result"
input_name = "import/" + input_layer
output_name = "import/" + output_layer
global graph
global sess




class reValue:
    def __init__(self, classifyStr, value):
        self.label = classifyStr
        self.value = value


def Init(label_file, model_file):
    global graph
    global labels
    global bInitFlag
    global sess
    if bInitFlag == False:
        labels = load_labels(label_file)
        graph = load_graph(model_file)
        sess = tf.Session(graph=graph)
        bInitFlag = True

def load_graph(model_file):
    #global graph
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
                                input_mean=0,
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

if __name__ == "__main__":
#def seatClassify(image_path, label_path, model_path):

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
    global bInitFlag
    label_model_dir = "D:\\Cache\\GithubCache\\CarSeat_Recognization\\CarSeat_recognization\\Recognization_module\\"
    label_path = label_model_dir + "output_labels.txt"
    model_path = label_model_dir + "output_graph.pb"

    if bInitFlag == False:
        Init(label_path, model_path)
        bInitFlag = True
    
    input_operation = graph.get_operation_by_name(input_name)
    output_operation = graph.get_operation_by_name(output_name)

    filedir = "J:\\AutocarSeat_Recognition\\OriginalImage\\20180707_hik_test\\"
    listSubdir = os.listdir(filedir)
    for tmpsubdir in listSubdir:
        listImagePath = os.walk(filedir + "\\" + tmpsubdir)
        print(listImagePath)
        print(type(listImagePath))
        for tmpImagePath in listImagePath[2]:
            image_path = filedir+"\\" + tmpsubdir + "\\" + tmpImagePath
            if ".jpg" not in image_path:
                continue
            t = read_tensor_from_image_file(
                image_path,
                input_height=input_height,
                input_width=input_width,
                input_mean=input_mean,
                input_std=input_std)

            results = sess.run(output_operation.outputs[0], {
                input_operation.outputs[0]: t
            })
            results = np.squeeze(results)
            top_k = results.argsort()[-1:][::-1]

            #tmpStr = reValue(labels[top_k[0]], results[top_k[0]])

            print(labels[top_k[0]])
            print(results[top_k[0]])

    #return (labels[top_k[0]], results[top_k[0]])
                

                

   




