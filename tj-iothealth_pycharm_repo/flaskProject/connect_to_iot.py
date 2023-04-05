# encoding=utf-8

import time
import sys
import hashlib
import hmac
import base64
import stomp
import ssl
import schedule
import threading
import json
import re
import time
import datetime
import pymysql_function as sql_fun

'''
def create_one(time,HR,SpO2,Tem,UMAC):

    with UsingMysql(log_time=True) as um:
        sql = "insert into user_data(UMAC, Time, SpO2, HR, Tem) values(%s, %s, %s, %s, %s)"
        #now = datetime.datetime.now()
        #now = now.strftime("%Y-%m-%d %H:%M:%S")
        params = ('qwrqr2',time,SpO2,HR,Tem)
        um.cursor.execute(sql, params)

        # 查看结果
        #select_one(um.cursor)
'''

def connect_and_subscribe(conn):
    accessKey = ""
    accessSecret = ""
    consumerGroupId = "DEFAULT_GROUP"
    # iotInstanceId：购买的实例请填写实例ID，公共实例请填空字符串""。
    iotInstanceId = " "
    clientId = "47.117.167.31"
    # 签名方法：支持hmacmd5，hmacsha1和hmacsha256。
    signMethod = "hmacsha1"
    timestamp = current_time_millis()
    # userName组装方法，请参见AMQP客户端接入说明文档。
    # 若使用二进制传输，则userName需要添加encode=base64参数，服务端会将消息体base64编码后再推送。具体添加方法请参见下一章节“二进制消息体说明”。
    username = clientId + "|authMode=aksign" + ",signMethod=" + signMethod \
               + ",timestamp=" + timestamp + ",authId=" + accessKey \
               + ",iotInstanceId=" + iotInstanceId \
               + ",consumerGroupId=" + consumerGroupId + "|"
    signContent = "authId=" + accessKey + "&timestamp=" + timestamp
    # 计算签名，password组装方法，请参见AMQP客户端接入说明文档。
    password = do_sign(accessSecret.encode("utf-8"), signContent.encode("utf-8"))

    conn.set_listener('', MyListener(conn))
    conn.connect(username, password, wait=True)
    # 清除历史连接检查任务，新建连接检查任务
    schedule.clear('conn-check')
    schedule.every(1).seconds.do(do_check, conn).tag('conn-check')


class MyListener(stomp.ConnectionListener):
    def __init__(self, conn):
        self.conn = conn

    def on_error(self, headers, message):
        print('received an error "%s"' % message)


    def on_message(self, headers, message):
        print('received a message "%s"' % message)
        ##判断数据类型
        msg = json.loads(message)
        print(headers['topic'])
        ##匹配topic格式

        if(headers['topic'][0:16]=='/as/mqtt/status/'):
            print("上报上下线信息")
            LastTime = msg['lastTime']
            DeviceName = msg['deviceName']
            Status=msg['status']
            sql_fun.create_one_status(DeviceName, LastTime, Status)
        pattern = "thing/event/property/post"
        if(headers['topic'].find(pattern)!=-1):
            print("上报体征参数")
            DeviceName = msg['deviceName']
            UMAC = msg['items']['MACAddress']['value']
            HR = msg['items']['HeartRate']['value']
            Tem = msg['items']['BodyTemperature']['value']
            SpO2 = msg['items']['SpO2']['value']
            timeStamp = (msg['items']['HeartRate']['time'])/1000
            timeArray = time.localtime(timeStamp)
            formatTime = time.strftime("%Y-%m-%d %H:%M:%S", timeArray)
            #print(formatTime)
            sql_fun.create_one(DeviceName,UMAC, formatTime, SpO2, HR, Tem)

    def on_disconnected(self):
        print('disconnected')
        connect_and_subscribe(self.conn)

    def on_heartbeat_timeout(self):
        print('on_heartbeat_timeout')

    def on_connected(self, headers, body):
        print("successfully connected")
        conn.subscribe(destination='/topic/#', id=1, ack='auto')
        print("successfully subscribe")


def current_time_millis():
    return str(int(round(time.time() * 1000)))


def do_sign(secret, sign_content):
    m = hmac.new(secret, sign_content, digestmod=hashlib.sha1)
    return base64.b64encode(m.digest()).decode("utf-8")


# 检查连接，如果未连接则重新建连
def do_check(conn):
    print('check connection, is_connected: %s', conn.is_connected())
    if (not conn.is_connected()):
        try:
            connect_and_subscribe(conn)
        except Exception as e:
            print('disconnected, ', e)


#  接入域名，请参见AMQP客户端接入说明文档。这里直接填入域名，不需要带amqps://前缀
conn = stomp.Connection([('1534808308038151.iot-amqp.cn-shanghai.aliyuncs.com', 61614)])
conn.set_ssl(for_hosts=[('1534808308038151.iot-amqp.cn-shanghai.aliyuncs.com', 61614)], ssl_version=ssl.PROTOCOL_TLS)

try:
    connect_and_subscribe(conn)
except Exception as e:
    print('connecting failed')
    raise e

# 定时任务方法，检查连接状态，监听推送消息
def connection_check_timer():
    while 1:
        schedule.run_pending()
        time.sleep(5)

# 异步线程运行定时任务，检查连接状态
thread = threading.Thread(
    target=connection_check_timer)
thread.start()