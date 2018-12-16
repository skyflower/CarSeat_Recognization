#!/usr/bin/env python
# -*- coding=utf-8 -*-


"""
file: send.py
socket client
"""

import socket
import os
import sys
import struct
import time


def socket_client():
    # 连接服务器
    try:
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect(('192.168.1.124', 47001))
    except socket.error as msg:
        print(msg)
        sys.exit(1)

    # 发送座椅信息
    msg = open('seatinfo.xml','rb').read()
    if len(msg) == 0:return

    print(msg)
    #发送数据 b将字符串转为bys类型
    s.send(msg)

    #接收服务器端的返回，需要声明收多少，默认1024字节
    data = s.recv(1024)

    #打印data是recv的data
    print("recv: " + str(data))
    
    filepath = "img_004.jpg"
    if os.path.isfile(filepath):
        # 定义定义文件信息。128s表示文件名为128bytes长，l表示一个int或log文件类型，在此为文件大小
        fileinfo_size = struct.calcsize('128sl')
        # 定义文件头信息，包含文件名和文件大小
        fhead = struct.pack('128sl', bytes(os.path.basename(filepath), 'ascii'), os.stat(filepath).st_size)
        data = bytes(fhead)
        print(len(data))
        print(data)
        s.send(data)
        print('client filepath: {0}'.format(filepath))

        fp = open(filepath, 'rb')
        while 1:
            data = fp.read(1024)
            if not data:
                print('{0} file send over...'.format(filepath))
                break
            s.send(data)
    else:
        print("filepath no exist")
    s.close()


if __name__ == '__main__':
    socket_client()
