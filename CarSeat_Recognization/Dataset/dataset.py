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
    通过执行sql语句创建表单name，如果数据库已存在名为name的表单这删掉数据库重新创建
    :param con: con
    :param sql: 执行语句
    :param name:将要创建的表的名字
    """
    try:
        con.execute(sql)
    except (DB_EXC.OperationalError, DB_EXC.ProgrammingError):
        drop(con, name)
        con.execute(sql)


def create_user_table(con):
    """
    创建user_table表单
    :param con:
    :return:
    """
    sql = "create table user_table (user text PRIMARY KEY NOT NULL ,password text,priority integer NOT NULL )"
    create(con, sql, "user_table")
    print("Create user_table succeed!")


def create_carseat_table(con):
    """
    创建carseat_table表单
    :param con:
    :return:
    """
    sql = """create table carseat_table(barcode text PRIMARY KEY NOT NULL ,
                                      name text NOT NULL ,
                                      time text,
                                      type integer ,
                                      material integer,
                                       color integer ,
                                       correct integer ,
                                       mode integer ,
                                       cameracode text,
                                       line integer ,
                                       priority integer ,
                                       path text)"""
    create(con, sql, "carseat_table")
    print("Create table carseat_table succeed!")


def insert(con, sql, data):
    """
    插入向表单插入数据，每次可以插入多个数据，数据由data数组提供
    :param con: connection对象
    :param sql:执行的语句
    :param data: 插入的数据一般是包含元组的数组，例如[('a','b'),('c','d')]
    :return: null
    """
    con.executemany(sql, data)
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
    sql = "INSERT INTO %s VALUES(?,?,?,?,?,?,?,?,?,?,?,?)" % "carseat_table"
    insert(con, sql, data)


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
    insert(con, sql, data)


def update(con, sql):
    con.execute(sql)
    con.commit()


def delete(con, sql):
    con.execute(sql)
    con.commit()


def select(con, table_name):
    """

    :param con: connection对象
    :param sql: 查询语句
    :return: 符合条件的返回结果，包含元组的数组
    """
    sql = "select * from %s" % table_name
    cur = con.cursor()
    cur.execute(sql)
    result = cur.fetchall()
    return result


def select_user_table(con):
    data_user = select(con, "user_table")
    return data_user


def select_carseat_table(con):
    data_carseat = select(con, "carseat_table")
    return data_carseat


def init(con):
    """
    初始化，创建carseat_table和user表单
    :param con:
    :return:
    """
    create_carseat_table(con)
    create_user_table(con)


def main():
    db = setup()
    con = connect(db)
    if not con:
        printf('ERROR: %r not supported or unreachable, exit' % db)
        return
    init(con)
    # TODO :增删查改逻辑部分

    # 向user_table插入数据
    user_data = [("张三", "123", 0), ("李四", "345", 1), ("王五", "456", 2), ("maybe", "789", 3), ("GTMACE", "134", 3)]
    insert_user_table(con, user_data)

    # 向carseat_table插入数据
    carseat_data = [
        ("101", "image1.jpg", "20180802", 1, 1, 2, 0, 0, "L1", 2,1, "/home/yqw/CarsetPicture/date/image1.jpg"),
        ("102", "image2.jpg", "20180802", 1, 1, 2, 0, 0, "L1", 2,2, "/home/yqw/CarsetPicture/date/image2.jpg"),
        ("103", "image3.jpg", "20180803", 1, 1, 2, 0, 0, "L1", 2,1, "/home/yqw/CarsetPicture/date/image3.jpg"),
        ("104", "image4.jpg", "20180804", 1, 1, 2, 0, 0, "L1", 2,1, "/home/yqw/CarsetPicture/date/image4.jpg"),
        ("105", "image5.jpg", "20180805", 1, 1, 2, 0, 0, "L1", 2,2, "/home/yqw/CarsetPicture/date/image5.jpg")]
    insert_carseat_table(con,carseat_data)

    # 查询user_table
    data_user=select_user_table(con)
    print("user_table全部数据如下：")
    for data in data_user:
        print(data)

    # 查询carseat_table
    data_carseat=select_carseat_table(con)
    print("carseat_table全部数据如下: ")
    for data in data_carseat:
        print(data)


if __name__ == '__main__':
    main()
