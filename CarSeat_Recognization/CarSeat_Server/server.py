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
server_info = socket.socket()
server_search = socket.socket()
# 显示声明,允许地址重复使用
server_info.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR,1)
server_search.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR,1)
#监听接收端口元组(本地，端口)，绑定要监听的端口
server_info.bind(('',47001))
server_search.bind(('',47002))

#1.监听
#2.“5”最大监听数，允许多少人在排队
server_info.listen(5)
server_search.listen(5)

PicDirName = "picture"
ErrorEmpty  = 1
ErrorFormat = 2

xml_header = '''<?xml version="1.0" encoding="utf-8"?>'''

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

def receive_msg(conn):
    #1.接收数据,1024字节
    #2.如果发不完会存在缓冲去，下次在发送。
    #3.缓冲区每次最多发32768字节
    #4.每个系统不同，超出数值会有限制。
    recvData = conn.recv(102400)
    #Linux系统内可以判断是否为空。
    if not recvData:
        return ErrorEmpty
    if len(recvData) > 0:
        root = ""
        try:
            root = ET.fromstring(recvData)
        except xml.etree.ElementTree.ParseError, e:
            print (e)
            return ErrorFormat
        value=[]
        if root.tag == "identification":
            for child in root:
                value.append(child.text)
        else:
            return ErrorFormat
        value[6] = get_todayPath() + '/' + value[6]
        dbName = db.setup()
        dbCon = db.connect(dbName)
        db.insert_carseat_table(dbCon, tuple(value))
        dbCon.close()
    #res = "receive info done \n"
    #告诉客户端可以发送对应图片了
    #conn.send(res)
    return 0

def receive_pic(conn):
    fileinfo_size = struct.calcsize('128sl')
    buf = conn.recv(fileinfo_size)
    if buf:
        filename, filesize = struct.unpack('128sl', buf)
        fn = filename.strip('\00')
        new_filename = get_todayPath() + '/' + fn

        recvd_size = 0  # 定义已接收文件的大小
        fp = open(new_filename, 'wb')
        while not recvd_size == filesize:
            if filesize - recvd_size > 1024:
                data = conn.recv(1024)
                recvd_size += len(data)
            else:
                data = conn.recv(filesize - recvd_size)
                recvd_size = filesize
            fp.write(data)
        fp.close()

def process_info(conn, addr):
    # 先接收座椅信息
    rc = receive_msg(conn)
    if rc == 0:
        res = "0"
        conn.send(res)
        receive_pic(conn)
    elif rc == ErrorEmpty:
        res = "1"
        conn.send(res)
    elif rc == ErrorFormat:
        res = "2"
        conn.send(res)
    conn.close()

def create_xml(seat_data, user_flag):
    carseat_node = ("lineID", "ip", "time", "barcode", "barcodeResult", "imageName", "method", "usrName", "typeByRecog", "typeByBarcode", "typeByInput", "cameraName", "correct")
    picpath = []
    #创建根节点:q
    root = ET.Element("Reply")
    root.set("package","search")
    if user_flag != 'none':
        root.set("status","fail")
    else:
        root.set("status","sussess")
    root.set("reason", user_flag)
    dt = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
    root.set("time",dt)
    root.set("version","0.1")
    i = 1
    num = len(seat_data)
    if user_flag != 'none' or num==0:
        sonName1 = "seatCount"
        sonNode1 = ET.SubElement(root, sonName1)
        sonNode1.text = '0'
    else:
        sonName1 = "seatCount"
        sonNode1 = ET.SubElement(root, sonName1)
        sonNode1.text = str(num)
        for data in seat_data:
            #创建root的子节点sub2，并添加属性
            sonName2 = "Seat" + str(i)
            sonNode2 = ET.SubElement(root, sonName2)
            j = 0
            # 去掉version信息
            data = data[1:]
            for d in data:
                # Unicode 转 str
                d = d.encode('utf-8')
                Node = ET.SubElement(sonNode2, carseat_node[j])
                if j == 5:
                    picpath.append(d)
                    d = os.path.basename(d)
                Node.text = d
                j = j + 1
            i = i + 1
    xml = xml_header + ET.tostring(root)

    return picpath, xml

def send_pictures(conn, pic_path):
    # 等待客户端请求发送图片
    data = conn.recv(10240)
    for filepath in pic_path:
        if os.path.isfile(filepath):
            # 定义定义文件信息。128s表示文件名为128bytes长，l表示一个int或log文件类型，在此为文件大小
            fileinfo_size = struct.calcsize('128sl')
            # 定义文件头信息，包含文件名和文件大小
            fhead = struct.pack('128sl', os.path.basename(filepath),
                                os.stat(filepath).st_size)
            conn.send(fhead)

            fp = open(filepath, 'rb')
            while 1:
                data = fp.read(1024)
                if not data:
                    break
                conn.send(data)

def send_search_result(conn, seat_data, user_flag):
    num = len(seat_data)
    pic_path,search_result = create_xml(seat_data, user_flag)
    #发送查询结果
    conn.send(search_result)
    #查询结果小于10个，发送图片数据
    if num > 0 and num <= 10:
        send_pictures(conn, pic_path)

def parse_search_info(data):
    user_info = []
    condition = ''
    root = ""
    try:
        root = ET.fromstring(data)
    except xml.etree.ElementTree.ParseError, e:
        print(e)
        return ErrorFormat,None,None
    for child in root:
        if child.tag == 'info':
            user_info.append(child.attrib['usr'])
            user_info.append(child.attrib['passwd'])
        elif child.tag == 'timePolicy':
            if child.attrib['control']=='1':
                condition = '''time >= '%s' AND time <= '%s' ''' % (child.attrib['startTime'], child.attrib['endTime'])
        elif child.tag == 'linePolicy':
            if child.attrib['control']=='1':
                condition += ''' AND lineID >= '%s' AND lineID <= '%s' ''' % (child.attrib['startLine'], child.attrib['endLine'])
        elif child.tag == 'barcodePolicy':
            if child.attrib['control']=='1':
                condition += ''' AND barcode >= '%s' AND barcode <= '%s' ''' % (child.attrib['startBarcode'], child.attrib['endBarcode'])
        elif child.tag == 'typePolicy':
            if child.attrib['control']=='1':
                condition += ''' AND typeByRecog = '%s' ''' % child.attrib['type']
        elif child.tag == 'methodPolicy':
            if child.attrib['control']=='1':
                condition += ''' AND method = '%s' ''' % child.attrib['method']
        if condition[:4] == ' AND':
            condition = condition[5:]
    
    return 0,user_info, condition

def process_search(conn, addr):
    #循环处理客户端请求
    recvData = conn.recv(102400)
    if not recvData:
        res = "the seat msg is empty, please re-send \n"
        conn.send(res)
        conn.close()
        return
    err, user_info, carset_cond = parse_search_info(recvData)
    if err:
        res = "the search.xml format error, please re-send"
        conn.send(res)
        conn.close()
        return
    dbName = db.setup()
    dbCon = db.connect(dbName)
    # 查询用户名密码是否正确
    user_cond = '''user = '%s' ''' % user_info[0]
    data_user=db.select_user_table(dbCon, user_cond)
    if len(data_user)==0:
        send_search_result(conn, data_user, 'user_unvalid')
    else:
        if data_user[0][1] != user_info[1]:
            send_search_result(conn, data_user, 'password_unvalid')
        else:
            data_carseat=db.select_carseat_table(dbCon, carset_cond)
            dbCon.close()
            send_search_result(conn, data_carseat, 'none')
    conn.close()

def recieve_info():
    while True:
        conn,addr = server_info.accept()
        t=threading.Thread(target=process_info,args=(conn, addr))  #t为新创建的线程
        t.start()
    #关闭链接
    server_msg.close()
    
def recieve_query():
    while True:
        conn,addr = server_search.accept()
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
    p_info = multiprocessing.Process(target=recieve_info)
    p_search = multiprocessing.Process(target=recieve_query)
    p_info.start()
    p_search.start()
    p_info.join()
    p_search.join()
