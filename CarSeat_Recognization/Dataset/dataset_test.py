import sqlite3

conn = sqlite3.connect("sqlite.db")  #创建sqlite.db数据库
print ("open database success")
conn.execute("drop table IF EXISTS student")
query = """create table IF NOT EXISTS student(
    customer VARCHAR(20),
    produce VARCHAR(40),
    amount FLOAT,
    date DATE   
);"""
conn.execute(query)
print ("Table created successfully")

#在表中插入数据

''' 方法1 '''
#data = '''INSERT INTO student(customer,produce,amount,date)\
#    VALUES("zhangsan","notepad",999,"2017-01-02")'''
#conn.execute(data)
#data = '''INSERT INTO student(customer,produce,amount,date)\
#    VALUES("lishi","binder",3.45,"2017-04-05")'''
#conn.execute(data)
#conn.commit()

''' 方法2 '''
statement = "INSERT INTO student VALUES(?,?,?,?)"
data = [("zhangsan","notepad",999,"2017-01-02"),("lishi","binder",3.45,"2017-04-05")]
conn.executemany(statement, data)
conn.commit()

curson = conn.execute("select * from student")
conn.commit()
print (curson)
rows = curson.fetchall()
print (rows)
conn.close()