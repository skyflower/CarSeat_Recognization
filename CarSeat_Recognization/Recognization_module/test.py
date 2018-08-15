

import os
import sys

import xml
import xml.etree
import xml.etree.ElementTree
import os
import socket
import threading
import time
import random


def WriteData(data):
     tmpStr = '''<?xml version="1.0" encoding="gbk"?>' \
         <write>%d</write>'''

     tmpStr = tmpStr % data
     return tmpStr

def ReadData():
    tmpStr = '''<?xml version="1.0" encoding="gbk"?>' \
         <read></read>'''

    return tmpStr


def testLinkStatus():
    port = 47005
    ServerSocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    address = ("172.30.150.33", port)
    ServerSocket.bind(address)

    i= 3
    while True:
        #ServerSocket.connect(address)
        tmpStr = WriteData(i)
        i = i + 1
        ServerSocket.sendto(bytes(tmpStr, encoding='ascii'), address)
        #recvData = ServerSocket.send(bytes(tmpStr, encoding='ascii'))
        time.sleep(2)
        print(tmpStr)
        # if "hello" in recvData:
        #     tmpStr = "success"
        #     ServerSocket.send(tmpStr)
        #     ServerSocket.close()

testLinkStatus()