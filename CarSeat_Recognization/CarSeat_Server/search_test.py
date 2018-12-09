# -*- coding: UTF-8 -*-
import socket
import time
import struct
import xml.etree.ElementTree as ET
import xml
import xml.etree
import xml.etree.ElementTree
#声明协议类型,同事生成socket连接对象
client = socket.socket()
# 显示声明,允许地址重复使用
client.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR,1)
#链接地址和端口,元组(本地，端口)
client.connect(('127.0.0.1',47002))

#使用input循环向服务端发送请求
n = True
while n:
    #msg = raw_input("Plz imput what you want to submit:")
    msg = open('search.xml','rb').read()
    if len(msg) == 0:continue

    print(msg)
    #发送数据 b将字符串转为bys类型
    client.send(msg)

    #接收服务器端的返回，需要声明收多少，默认1024字节
    recvData = client.recv(10240)
    #打印data是recv的data
    print("recv:",recvData)
    msg = "send pic"
    client.send(msg)
    num = 0
    if len(recvData) > 0:
        root = ""
        try:
            root = ET.fromstring(recvData)
        except xml.etree.ElementTree.ParseError, e:
            print(recvData)
            print("parse Error, data len = %u" % len(recvData))
        for child in root:
            if child.tag == "seatCount":
                num = int(child.text)
                break
    print("receive num = %d" % num)
    fileinfo_size = struct.calcsize('128sl')
    if num >0 and num < 10:
        for i in range(num):
            buf = client.recv(fileinfo_size)
            if buf:
                filename, filesize = struct.unpack('128sl', buf)
                fn = filename.strip('\00')
                new_filename = "nwm_" + fn
                print new_filename
                recvd_size = 0  # 定义已接收文件的大小
                fp = open(new_filename, 'wb')
                print 'start receiving pic'
                while not recvd_size == filesize:
                    if filesize - recvd_size > 1024:
                        data = client.recv(1024)
                        recvd_size += len(data)
                    else:
                        data = client.recv(filesize - recvd_size)
                        recvd_size = filesize
                    fp.write(data)
                fp.close()
    n = False
#关闭接口
client.close()
