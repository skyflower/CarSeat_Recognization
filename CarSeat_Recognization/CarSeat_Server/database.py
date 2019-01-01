# -*- coding: UTF-8 -*-
#!/usr/bin/env python

from distutils.log import warn as printf
import os

DB_NAME = 'carseat'
DB_EXC = None


# 定义数据库名字
def setup():
    return 'sqlite'


# 创建数据库和连接
def connect(db_type):
    """
    创建数据库，数据库名为carseat
    :param db: 数据库类型，默认sqlite
    :return: 数据库的connection对象
    """
    global DB_EXC
    dbDir = db_type
    if db_type == 'sqlite':
        try:
            import sqlite3
        except ImportError:
            try:
                from pysqlite2 import dbapi2 as sqlite3
            except ImportError:
                return None
        DB_EXC = sqlite3
        if not os.path.isdir(dbDir):
            os.mkdir(dbDir)
        con = sqlite3.connect(os.path.join(dbDir, DB_NAME))
    else:
        return None
    return con


def drop(con, name):
    con.execute('DROP TABLE %s' % name)
    con.commit()


def create(con, sql, name):
    """
    通过执行sql语句创建表单name，如果数据库已存在名为name的表单,则直接返回
    :param con: con
    :param sql: 执行语句
    :param name:将要创建的表的名字
    """
    try:
        con.execute(sql)
    except (DB_EXC.OperationalError, DB_EXC.ProgrammingError):
        #drop(con, name)
        #con.execute(sql)
        print'the %s is exist' % name


def create_user_table(con):
    """
    创建user_table表单
    :param con:
    :return:
    """
    sql = "create table user_table (user text PRIMARY KEY NOT NULL ,password text,priority integer NOT NULL )"
    create(con, sql, "user_table")
    #print("Create user_table succeed!")


def create_carseat_table(con):
    """
    创建carseat_table表单
    :param con:
    :return:
    """
    sql = """create table carseat_table(version text,
                                      lineID text,
                                      ip text,
                                      time text,
                                      barcode text PRIMARY KEY NOT NULL ,
                                      barcodeResult text,
                                      imageName text,
                                      method text,
                                      usrName text,
                                      typeByRecog text,
                                      typeByBarcode text,
                                      typeByInput text,
                                      cameraName text,
                                      correct text)"""
    create(con, sql, "carseat_table")
    #print("Create table carseat_table succeed!")


def insert(con, sql, data, table_name):
    """
    插入向表单插入数据，每次可以插入多个数据，数据由data数组提供
    :param con: connection对象
    :param sql:执行的语句
    :param data: 插入的数据一般是包含元组的数组，例如[('a','b'),('c','d')]
    :return: null
    """
    con.text_factory = str
    try:
        con.execute(sql, data)
    except:
        if table_name == "carseat_table":
            print 'the barcode = %s in %s is exist, insert refused' % (data[4], table_name)
        else:
            print 'the user = %s in %s is exist, insert refused' % (data[4], table_name)
    else:
        con.commit()


def insert_carseat_table(con, data):
    """
    carseat_table设计如下
    --------------------------------------------------------------------------------------
    条形码  |图像名字|时间|座椅类型|座椅材质|颜色  |识别结果是否一致|识别模式|相机编码  | 产线|权限要求 |图像路径
    --------------------------------------------------------------------------------------
    barcode| name  |time| type |materia|color|    correct  |  mode |cameracod|line|priority| path
    --------------------------------------------------------------------------------------
    :param con:
    :param data:
    :return:
    """
    sql = "INSERT INTO %s VALUES(?,?,?,?,?,?,?,?,?,?,?,?,?,?)" % "carseat_table"
    insert(con, sql, data, "carseat_table")


def insert_user_table(con, data):
    """
    user_table设计如下
    -------------------------
    用户名| mode |权限等级 |
    -------------------------
    user |passwd|Priority|
    -------------------------
    :param con:
    :param data:
    :return:
    """
    sql = "INSERT INTO %s VALUES(?,?,?)" % "user_table"
    insert(con, sql, data, "user_table")


def update(con, sql):
    con.execute(sql)
    con.commit()


def delete(con, sql):
    con.execute(sql)
    con.commit()


def select(con, table_name, condition):
    """
    :param con: connection对象
    :param sql: 查询语句
    :return: 符合条件的返回结果，包含元组的数组
    """
    sql = "select * from %s where %s" % (table_name, condition)
    cur = con.cursor()
    print(sql)
    cur.execute(sql)
    result = cur.fetchall()
    return result


def select_user_table(con, condition):
    data_user = select(con, "user_table", condition)
    return data_user


def select_carseat_table(con, condition):
    data_carseat = select(con, "carseat_table", condition)
    return data_carseat


def init(con):
    """
    初始化，创建carseat_table和user表单
    :param con:
    :return:
    """
    create_carseat_table(con)
    create_user_table(con)
