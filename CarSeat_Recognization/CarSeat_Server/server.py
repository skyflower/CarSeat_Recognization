# -*- coding: UTF-8 -*-
import xml.etree.ElementTree as ET
import xml
import xml.etree
import xml.etree.ElementTree
import socket
import os
import time
import datetime
import multiprocessing
import threading
import struct
import database as db

#声明类型，生成socket链接对象
server_msg = socket.socket()
server_pic = socket.socket()
server_search = socket.socket()

#监听接收端口元组(本地，端口)，绑定要监听的端口
server_msg.bind(('',4700))
server_pic.bind(('',4701))
server_search.bind(('',4702))

#1.监听
#2.“5”最大监听数，允许多少人在排队
server_msg.listen(5)
server_pic.listen(5)
server_search.listen(5)

PicDirName = "picture"

def get_todayPath():
    curPath = os.getcwd()
    picPath = curPath + '/' + PicDirName
    today = datetime.datetime.now()
    todayPath = picPath + '/' + today.strftime('%Y-%m-%d')
    
    return todayPath

def get_tomorrowPath():
    curPath = os.getcwd()
    picPath = curPath + '/' + PicDirName
    today = datetime.datetime.now()
    tomorrow = today + datetime.timedelta(days=1)
    tomorrowPath = picPath + '/' + tomorrow.strftime('%Y-%m-%d')
    
    return tomorrowPath

def process_msg(conn, addr):
    #循环处理客户端请求
        while True:
            #1.接收数据,1024字节
            #2.如果发不完会存在缓冲去，下次在发送。
            #3.缓冲区每次最多发32768字节
            #4.每个系统不同，超出数值会有限制。
            recvData = conn.recv(102400)
            #返回
            print(recvData)
            #Linux系统内可以判断是否为空。
            if recvData == 'exit' or not recvData:
                break
            if len(recvData) > 0:
                root = ""
                try:
                    root = ET.fromstring(recvData)
                except xml.etree.ElementTree.ParseError, e:
                    print(recvData)
                    print("parse Error, data len = %u" % len(recvData))
                    continue
                value=[]
                if root.tag == "SeatInfo":
                    for child in root:
                        value.append(child.text)
                    print(value)
                value[-1] = get_todayPath() + '/' + value[-1]
                dbName = db.setup()
                dbCon = db.connect(dbName)
                db.insert_carseat_table(dbCon, tuple(value))
                dbCon.close()
            #执行命令，赋值给变量
            res = "I`ve got the msg \n"
            #返回一个值，返回res命令
            conn.send(res)
            #sendll就是循环send，用法发送大型文件。
            #conn.sendll(res)
        conn.close()
        
def process_pic(conn, addr):
    print 'Accept new connection from {0}'.format(addr)
    #conn.settimeout(500)
    conn.send('Hi, Welcome to the server!')

    while 1:
        fileinfo_size = struct.calcsize('128sl')
        buf = conn.recv(fileinfo_size)
        if buf:
            filename, filesize = struct.unpack('128sl', buf)
            fn = filename.strip('\00')
            new_filename = get_todayPath() + '/' + fn

            recvd_size = 0  # 定义已接收文件的大小
            fp = open(new_filename, 'wb')
            print 'start receiving...'
            while not recvd_size == filesize:
                if filesize - recvd_size > 1024:
                    data = conn.recv(1024)
                    recvd_size += len(data)
                else:
                    data = conn.recv(filesize - recvd_size)
                    recvd_size = filesize
                fp.write(data)
            fp.close()
            print 'end receive...'
    conn.close()
        
def process_search(conn, addr):
    #循环处理客户端请求
        while True:
            #1.接收数据,1024字节
            #2.如果发不完会存在缓冲去，下次在发送。
            #3.缓冲区每次最多发32768字节
            #4.每个系统不同，超出数值会有限制。
            recvData = conn.recv(102400)
            #返回
            print(recvData)
            #Linux系统内可以判断是否为空。
            if recvData == 'exit' or not recvData:
                break
            #执行命令，赋值给变量
            # 查询carseat_table
            dbName = db.setup()
            dbCon = db.connect(dbName)
            data_carseat=db.select_carseat_table(dbCon)
            dbCon.close()
            print("carseat_table全部数据如下: ")
            for data in data_carseat:
                print(data)
            res = "I`ve got the search \n"
            #返回一个值，返回res命令
            conn.send(res)
            #sendll就是循环send，用法发送大型文件。
            #conn.sendll(res)
        conn.close()

def recieve_msg():
    print("recieve_msg 进程开始等待客户端了")
    #循环锁定访问客户端
    while True:
        conn,addr = server_msg.accept()
        print("recieve_msg 接收到一个客户端:")
        #查看标记位与IP地址
        print(conn,addr)
        t=threading.Thread(target=process_msg,args=(conn, addr))  #t为新创建的线程
        t.start()
    #关闭链接
    server_msg.close()
    
def recieve_picture():
    print("recieve_picture 进程开始等待客户端了")
    #循环锁定访问客户端
    while True:
        conn,addr = server_pic.accept()
        print("recieve_picture 接收到一个客户端:")
        #查看标记位与IP地址
        print(conn,addr)
        t=threading.Thread(target=process_pic,args=(conn, addr))  #t为新创建的线程
        t.start()
    #关闭链接
    server_pic.close()
    
def recieve_query():
    print("recieve_query 进程开始等待客户端了")
    #循环锁定访问客户端
    while True:
        #1.等待客户端 #会返回链接的标记位conn，与连接的地址
        #2.客户端同过conn,addr进行通话
        #3.conn就是客户端连接过来而在服务器端为其生成的一个连接实例
        conn,addr = server_search.accept()
        print("recieve_query 接收到一个客户端:")
        #查看标记位与IP地址
        print(conn,addr)
        t=threading.Thread(target=process_search,args=(conn, addr))  #t为新创建的线程
        t.start()
    #关闭链接
    server_search.close()
def fun_timer():
    todayPath = get_todayPath()
    tomorrowPath = get_tomorrowPath()
    #创建当天图片储存文件夹
    if not os.path.isdir(todayPath):
        os.mkdir(todayPath)
    #因为定时器24小时执行一次，所以需提前一天创建当天文件夹
    if not os.path.isdir(tomorrowPath):
        os.mkdir(tomorrowPath)
    global PicTimer  #定义变量
    PicTimer = threading.Timer(60 * 60 * 24, fun_timer)   #24小时调用一次函数
    #定时器构造函数主要有2个参数，第一个参数为时间，第二个参数为函数名
    PicTimer.start()    #启用定时器
    
def server_init():
    # 创建文件夹，储存所有图片
    if not os.path.isdir(PicDirName):
        os.mkdir(PicDirName)
    # 创建定时器，用于每24小时创建一个文件夹处于图片
    PicTimer = threading.Timer(0,fun_timer)  #首次启动
    PicTimer.start()
    # 初始化数据库
    dbName = db.setup()
    dbCon = db.connect(dbName)
    if not dbCon:
        printf('ERROR: %r not supported or unreachable, exit' % db)
        return
    db.init(dbCon)
    dbCon.close()

if __name__=="__main__":
    server_init()
    p_msg = multiprocessing.Process(target=recieve_msg)
    p_pic = multiprocessing.Process(target=recieve_picture)
    p_search = multiprocessing.Process(target=recieve_query)
    p_msg.start()
    p_pic.start()
    p_search.start()
    p_msg.join()
    p_pic.join()
    p_search.join()
