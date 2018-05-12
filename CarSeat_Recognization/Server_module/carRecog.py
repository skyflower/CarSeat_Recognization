#coding=utf-8


import xml
import xml.etree
import xml.etree.ElementTree
import os
import socket
import threading
import time
import sys

__author__ = 'Administrator'

FileRootDir = "F:\\AutocarSeat_Recognition\\ImageDir\\"

blood = dict()
boot = dict()
__author__ = 'Administrator'

imgListMutex = threading.Lock()

imgList = []

class CLog:
    def __init__(self):
        self._fp = None
        self._time = None
    def write(self, tmpStr):
        curTime = time.time()
        if self._time != None:
            if curTime  - self._time > 6 * 60 * 60:
                if self._fp != None:
                    self._fp.close()
                tm_time = time.localtime(curTime)
                tmpStr = "Log-%d%d%d-%d.txt" %(tm_time.tm_year, tm_time.tm_mon, tm_time.tm_mday, tm_time.tm_hour)
                self._fp = open(tmpStr, "w+")
                self._time = curTime
        else:
            tm_time = time.localtime(time.time())
            tmpStr = "Log-%d%d%d-%d.txt" %(tm_time.tm_year, tm_time.tm_mon, tm_time.tm_mday, tm_time.tm_hour)
            self._fp = open(tmpStr, "w+")
            self._time = curTime




def ImageTransmission(port):
    pass

def identification(usr, passwd):
    return True

def xmlValidTokenCheck(xmlData):
    if xmlData is None:
        return None
    if len(xmlData) == 0:
        return None
    p = bytearray(xmlData)
    length = len(p)
    for i in range(length):
        if p[i] > 128:
            print("i = %d, p[i] = %u" %(i, p[i]))
            if i > 0:
                print("i = %d, p[i] = %u, p[i - 1] = %u, p[i + 1] = %u" %(i, p[i], p[i - 1], p[i + 1]))
                
            p[i] = ord(' ')
    return str(p)

    

def testLinkStatus(port):
    ServerSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    address = ("", port)
    ServerSocket.bind(address)

    ServerSocket.listen(10)

    while True:
        clientSocket, destAddr = ServerSocket.accept()
        recvData = clientSocket.recv(1024)

        if "hello" in recvData:
            tmpStr = "success"
            clientSocket.send(tmpStr)
            clientSocket.close()



def NetworkTask():
    tcpServerSocket = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
    address = ('', 8080)
    tcpServerSocket.bind(address)
    tcpServerSocket.listen(10)

    global blood
    global boot
    global imgList
    global imgListMutex
    #byteArrayPrintFlag = True
    #tmpErrorCount = 1
    while True:
        newServerSocket, destAddr = tcpServerSocket.accept()
        while True:
            recvData = newServerSocket.recv(1024)
            if len(recvData) > 0:
                root = ""
                try:
                    parser = xml.etree.ElementTree.XMLParser()
                    if isinstance(recvData, (bytes, bytearray)):
                        recvData = xmlValidTokenCheck(str(recvData))

                    
                    parser.feed(recvData)
                    root = parser.close()
                except xml.etree.ElementTree.ParseError, e:

                    print(recvData)
                    print("parse Error, data len = %u" % len(recvData))
                    continue

                if root.tag == "bloodheart":
                    value = {"status":root.attrib['status'], 'ip':root.attrib['ip'], 'time':root.attrib['time']}
                    blood[root.attrib['pcName']] = value
                    tmpStr = '''<?xml version="1.0" encoding="gbk"?>' \
                          <reply package="bloodheart" status="success" time="%s"/>'''
                    tm_time = time.localtime(time.time())
                    curTime = "%04d%02d%02d-%02d%02d%02d" %(tm_time.tm_year, tm_time.tm_mon, tm_time.tm_mday,
                                                            tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec)
                    tmpStr = tmpStr % curTime


                    newServerSocket.send(tmpStr)

                elif root.tag == "FirstBoot":
                    usr = root.attrib['usr']
                    passwd = root.attrib['passwd']
                    if identification(usr, passwd) == True:
                        value = {'ip':root.attrib['ip'], 'time':root.attrib['time'], 'camera':root.attrib['camera']}
                        if root.attrib['lineID'] in boot:
                            boot[root.attrib['lineID']] = value

                        tmpStr = '''<?xml version="1.0" encoding="gbk"?>' \
                          <reply package="FirstBoot" status="success" time="%s"/>'''
                        tm_time = time.localtime(time.time())
                        curTime = "%04d%02d%02d-%02d%02d%02d" %(tm_time.tm_year, tm_time.tm_mon, tm_time.tm_mday,
                                                                tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec)
                        tmpStr = tmpStr % curTime
                        newServerSocket.send(tmpStr)


                elif root.tag == "identification":
                    subEle = root.getchildren()
                    if subEle is None:
                        continue
                    if subEle.tag != "data":
                        continue
                    tmp = {"barcode":root.attrib['barcode'], "barcodeResult" : root.attrib['barcodeResult'],
                           "md5" : root.attrib['md5'], 'type' : root.attrib['type'], 'method' : root.attrib['method']}
                    if imgListMutex.acquire(1):
                        imgList.append(tmp)
                        imgListMutex.release()

                elif root.tag == "replay":
                    if root.attrib['package'] == "updateBarcode":
                        pass
                elif root.tag == "login":
                    tmpStr = '''<?xml version="1.0" encoding="gbk"?>' \
                      <reply package="login" status="success" time="%s"/>'''
                    tm_time = time.localtime(time.time())
                    curTime = "%04d%02d%02d-%02d%02d%02d" %(tm_time.tm_year, tm_time.tm_mon, tm_time.tm_mday,
                                                            tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec)
                    tmpStr = tmpStr % curTime
                    newServerSocket.send(tmpStr)

                elif root.tag == "search":
                    pass
                elif root.tag == "barcode":
                    pass
                elif root.tag == "query":

                    pass




            elif len(recvData) == 0:
                newServerSocket.close()
                
                break
    tcpServerSocket.close()

if __name__ == "__main__":
    thread = threading.Thread(target=NetworkTask)
    linkThread = threading.Thread(target=testLinkStatus, args=(10909,))

    thread.start()
    linkThread.start()
    thread.join()
    linkThread.join()
    print("program over")


