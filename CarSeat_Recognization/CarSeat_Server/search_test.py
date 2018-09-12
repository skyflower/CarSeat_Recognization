# -*- coding: UTF-8 -*-
import socket
import time

#声明协议类型,同事生成socket连接对象
client = socket.socket()

#链接地址和端口,元组(本地，端口)
client.connect(('127.0.0.1',4702))

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
    data = client.recv(1024)

    #打印data是recv的data
    print("recv:",data)
    time.sleep(2)
    n = False
#关闭接口
client.close()