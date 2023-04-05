#! /usr/bin/python
# -*- coding: UTF-8 -*-
import datetime
from pymysql_comm import UsingMysql

#################### 增

# 新增单条设备监测的体征参数记录
def create_one(DeviceName,UMAC, Time, SpO2, HR, Tem):

    with UsingMysql(log_time=True) as um:
        sql = "insert into device_alldata(DeviceName,UMAC, Time, SpO2, HR, Tem) values(%s, %s, %s, %s, %s, %s)"
        params = (DeviceName,UMAC, Time, SpO2, HR, Tem)
        um.cursor.execute(sql, params)

        # 查看结果
        select_one(um.cursor)
# 新增单条设备上下线信息
def create_one_status(DeviceName,LastTime,status):

    with UsingMysql(log_time=True) as um:
        sql = "insert into devicestatus(DeviceName,LastTime,status) values(%s,%s,%s)"
        params = (DeviceName,LastTime,status)
        um.cursor.execute(sql, params)

        # 查看结果
        select_one(um.cursor)

#################### 删
# 删除对应MAC地址的记录
def delete_one(cursor, name):
    sql = 'delete from device_alldata where UMAC = %s'
    params = name
    cursor.execute(sql, params)
    print('--- 已删除UMAC为%s用户. ' % name)

#################### 改

#################### 查
def select_one(cursor):
    ##查找最新插入的数据
    cursor.execute("select * from device_alldata")
    data = cursor.fetchone()
    print("-- 单条记录: {0} ".format(data))

# 依据UMAC查找用户的所有数据
def select_one_by_UMAC(cursor, UMAC):
    sql = 'select * from Product where name = %s'
    params = UMAC
    cursor.execute(sql, params)
    data = cursor.fetchone()
    if data:
        print('--- 已找到用户名为%s的数据. ' % data['UMAC'])
    else:
        print('--- 名字为%s的已经没有了' % UMAC)

# 依据UMAC和Time查找指定用户在指定时间的数据
def fetch_list_by_UMAC_time(cursor, UMAC,days=1):
    #查找距离现在一天之内的数据
    #传递时间{0}和终端设备MAC地址两个参数{1}
    sql = 'SELECT * FROM `device_alldata` WHERE Time >= date_sub(curdate(),interval {0} day) and UMAC = "{1}"'
    #a=100
    #params =UMAC
    sql1=sql.format(days,UMAC)
    cursor.execute(sql1)
    data_list = cursor.fetchall()
    #时间格式处理原来为datetime.datetime(2021, 4, 1, 8, 21, 37)
    for i in data_list:
        i['Time']=i['Time'].strftime("%Y-%m-%d %H:%M:%S")
        #print(i)
    print('-- 总数: %d' % len(data_list))
    #print(type(data_list[0]))
    return data_list

def fetch_list_by_DeviceName_time(cursor, DeviceName,days=1):
    #查找距离现在一天之内的数据
    #传递时间{0}和终端设备MAC地址两个参数{1}
    sql = 'SELECT * FROM `device_alldata` WHERE Time >= date_sub(curdate(),interval {0} day) and DeviceName = "{1}"'
    #a=100
    #params =UMAC
    sql1=sql.format(days,DeviceName)
    cursor.execute(sql1)
    data_list = cursor.fetchall()
    #时间格式处理原来为datetime.datetime(2021, 4, 1, 8, 21, 37)
    for i in data_list:
        i['Time']=i['Time'].strftime("%Y-%m-%d %H:%M:%S")
        #print(i)
    print('-- 总数: %d' % len(data_list))
    #print(type(data_list[0]))
    return data_list

def fetch_Statuslist_by_DeviceName(cursor, DeviceName):
    #查找距离现在一天之内的数据
    #传递时间{0}和终端设备MAC地址两个参数{1}
    sql = 'SELECT * FROM `devicestatus` WHERE DeviceName = "{0}"'
    #a=100
    #params =UMAC
    sql1=sql.format(DeviceName)
    cursor.execute(sql1)
    data_list = cursor.fetchall()
    #时间格式处理原来为datetime.datetime(2021, 4, 1, 8, 21, 37)
    for i in data_list:
        i['LastTime']=i['LastTime'].strftime("%Y-%m-%d %H:%M:%S")
        #print(i)
    print('-- 总数: %d' % len(data_list))
    #print(type(data_list[0]))
    return data_list

def fetch_list(UMAC,DAYS):
    with UsingMysql(log_time=True) as um:
        data_list = fetch_list_by_UMAC_time(um.cursor,UMAC,DAYS)
    return data_list

def fetch_list_Web(DeviceName,DAYS):
    with UsingMysql(log_time=True) as um:
        data_list = fetch_list_by_DeviceName_time(um.cursor,DeviceName,DAYS)
    return data_list
def fetch_Statuslist_Web(DeviceName):
    with UsingMysql(log_time=True) as um:
        data_list = fetch_Statuslist_by_DeviceName(um.cursor,DeviceName)
    return data_list

# 获取数量
def get_count(cursor):
    cursor.execute("select count(id) as total from Product")

    # 使用 fetchone() 方法获取单条数据.
    data = cursor.fetchone()

    print("-- 当前数量: %d " % data['total'])


def check_it():

    with UsingMysql(log_time=True) as um:
        um.cursor.execute("select count(UMAC) as total from device_alldata")
        data = um.cursor.fetchone()
        print("-- 当前数量: %d " % data['total'])

if __name__ == '__main__':
    UMAC='12345'
    fetch_list(UMAC)
    #now = datetime.datetime.now()
    #now = now.strftime("%Y-%m-%d %H:%M:%S")
    #create_one('1535335',now,88,11,11)