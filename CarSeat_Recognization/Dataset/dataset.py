#!/usr/bin/env python

from distutils.log import warn as printf
import os

DB_NAME = 'carSeat'
DB_EXC = None


# 定义数据库名字
def setup():
    return 'sqlite'


# 创建数据库和连接
def connect(db):
    """

    :param db: 数据库类型，默认sqlite
    :return: 数据库的connection对象
    """
    global DB_EXC
    dbDir = db
    if db == 'sqlite':
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
    通过执行sql语句创建数据库，如果数据库已存在这删掉数据库重新创建
    :param con: con
    :param sql: 执行语句
    :param name:将要创建的数据库的名字
    """
    try:
        con.execute(sql)
    except (DB_EXC.OperationalError, DB_EXC.ProgrammingError):
        drop(con, name)
        create(con, sql)


def create_user(con):
    sql = "create table user_table (user text primary not null ,password text,priority integer not null )"
    create(con, sql, "user")


def create_carseat_table(con):
    sql = """create table carSeat_table(barcode text primary not null,
                                      time text,
                                      type integer ,
                                      material integer,
                                       color integer ,
                                       correct integer ,
                                       mode integer ,
                                       cameraCode text,
                                       line integer ,
                                       priority integer ,
                                       path text)"""
    create(con, sql, "carSeat_table")


def insert(con, dataname, data):
    """

    :param con: connection对象
    :param dataname: 插入的表格名字
    :param data: 插入的数据一般是包含元组的数据，例如[('a','b'),('c','d')]
    :return: null
    """
    statement = "INSERT INTO %s VALUES(?,?,?,?)" % dataname
    con.executemany(statement, data)
    con.commit()


def insert_carseat_table(con, data):
    # 确定data的关键字不能为空
    # TODO
    insert(con, "carset_table", data)
    return


def insert_user_table(con, data):
    # 确定data关键字不能为空
    # TODO
    insert(con, "user", data)


def update(con, sql):
    # TODO,维护user表格中权限修改以及密码变更的功能
    return


def delete(con):
    # TODO,服务器图像删除后，需要删除表单中的数据
    return


def select(con, sql):
    cur = con.cursor()
    cur.execute(sql)
    result = cur.fatchall()
    return result


def main():
    db = setup()
    con = connect(db)
    if not con:
        printf('ERROR: %r not supported or unreachable, exit' % db)
        return
    cur = con.cursor()

    # printf('\n*** Create users table')
    # create(cur)
    #
    # printf('\n*** Insert names into table')
    # insert(cur, db)
    # dbDump(cur)
    #
    # printf('\n*** Move users to a random group')
    # fr, to, num = update(cur)
    # printf('\t(%d users moved) from (%d) to (%d)' % (num, fr, to))
    # dbDump(cur)
    #
    # printf('\n*** Randomly delete group')
    # rm, num = delete(cur)
    # printf('\t(group #%d; %d users removed)' % (rm, num))
    # dbDump(cur)
    #
    # printf('\n*** Drop users table')
    # drop(cur)
    # printf('\n*** Close cxns')
    # cur.close()
    # cxn.commit()
    # cxn.close()


if __name__ == '__main__':
    main()
