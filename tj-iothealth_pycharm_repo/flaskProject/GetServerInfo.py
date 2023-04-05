# -*- coding: utf-8 -*-
from flask import Flask, jsonify, request
from aliyunsdkcore.client import AcsClient
import json
from aliyunsdkecs.request.v20140526 import DescribeInstancesRequest, DescribeInstanceStatusRequest
import pymysql_function as sql_fun
import logging
#logging.basicConfig(level=logging.DEBUG)
from logging import FileHandler

app = Flask(__name__)

accessKeyId = ''
accessSecret = ''
region = 'cn-shanghai'
client = AcsClient(accessKeyId, accessSecret, region)

# 在app.route装饰器中声明响应的URL和请求方法
@app.route('/getServerInfo', methods=['GET'])
def getServerInfo():
    # GET方式获取请求参数
    instanceId = request.args.get("instanceId")
    if instanceId is None:
        return "Invalid Parameter"
    # 查询实例信息
    describeInstancesRequest = DescribeInstancesRequest.DescribeInstancesRequest()
    describeInstancesRequest.set_InstanceIds([instanceId])
    describeInstancesResponse = client.do_action_with_exception(describeInstancesRequest)
    # 返回数据为bytes类型，需要将bytes类型转换为str然后反序列化为json对象
    describeInstancesResponse = json.loads(str(describeInstancesResponse, 'utf-8'))
    instanceInfo = describeInstancesResponse['Instances']['Instance'][0]

    # 查询实例状态
    describeInstanceStatusRequest = DescribeInstanceStatusRequest.DescribeInstanceStatusRequest()
    describeInstanceStatusRequest.set_InstanceIds([instanceId])
    describeInstanceStatusResponse = client.do_action_with_exception(describeInstanceStatusRequest)
    describeInstanceStatusResponse = json.loads(str(describeInstanceStatusResponse, 'utf-8'))
    instanceStatus = describeInstanceStatusResponse['InstanceStatuses']['InstanceStatus'][0]['Status']

    # 封装结果
    result = {
        # cpu数
        'Cpu': instanceInfo['Cpu'],
        # 内存大小
        'Memory': instanceInfo['Memory'],
        # 操作系统名称
        'OSName': instanceInfo['OSName'],
        # 实例规格
        'InstanceType': instanceInfo['InstanceType'],
        # 实例公网IP地址
        'IpAddress': instanceInfo['PublicIpAddress']['IpAddress'][0],
        # 公网出带宽最大值
        'InternetMaxBandwidthOut': instanceInfo['InternetMaxBandwidthOut'],
        # 实例状态
        'instanceStatus': instanceStatus
    }
    return jsonify(result)

# 用户请求历史数据
@app.route('/getUserData', methods=['GET'])
def getUserData():
    DAYS=1 ##默认获得一天的数据
    UMAC = request.args.get("UMAC")##获得get中的UMAC地址
    if(request.args.get("DAYS")):
        DAYS = request.args.get("DAYS")
    #UMAC = request.args.get("UMAC")
    #print(UMAC)
    if(sql_fun.fetch_list(UMAC,DAYS)):
        data_list = sql_fun.fetch_list(UMAC,DAYS)
        print(data_list)
        senddata = {'data': data_list}
        print(senddata)
    #return "Invalid Parameter"
        return jsonify(senddata)
    else:
        return "No Data"

@app.route('/getUserData_Web', methods=['GET'])
def getUserData_Web():
    DAYS=1 ##默认获得一天的数据
    print(1)
    #app.logger.info(request)

    DeviceName = request.args.get("DeviceName")##获得get中的UMAC地址
    if(request.args.get("DAYS")):
        DAYS = request.args.get("DAYS")
    #UMAC = request.args.get("UMAC")
    #print(UMAC)
    #print("request data from{0},{1} days",DeviceName,DAYS)
    if(sql_fun.fetch_list_Web(DeviceName,DAYS)):
        data_list = sql_fun.fetch_list_Web(DeviceName,DAYS)
        #print(data_list)
        senddata = {'data': data_list}
        #打印信息到log中
        app.logger.info(request)
        app.logger.info("从数据库中查询的历史信息数量：{num} 条".format(num=len(data_list)))
        print("查询的历史信息数量:",len(data_list),"条")
    #return "Invalid Parameter"
        return jsonify(senddata)
    else:
        senddata={'data':[{"SpO2":-1,"HR":-1,"Time":'',"Tem":-1,"UMAC":'',"DeviceName":''}]}
        return senddata

@app.route('/getUserStatus_Web', methods=['GET'])
def getUserStatus_Web():

    DeviceName = request.args.get("DeviceName")##获得get中的设备名
    if(sql_fun.fetch_Statuslist_Web(DeviceName)):
        data_list = sql_fun.fetch_Statuslist_Web(DeviceName)
        #print(data_list)
        senddata = {'data': data_list}
        print(len(senddata))
    #return "Invalid Parameter"
        return jsonify(senddata)
    else:
        senddata={'data':[{"Status":"","LastTime":"","DeviceName":''}]}
        return senddata
if __name__ == "__main__":
    app.debug = True
    handler = logging.FileHandler('flask.log', encoding='UTF-8')  # 设置日志字符集和存储路径名字
    logging_format = logging.Formatter(  # 设置日志格式
        '%(asctime)s - %(levelname)s - %(filename)s - %(funcName)s - %(lineno)s - %(message)s')
    handler.setFormatter(logging_format)
    app.logger.addHandler(handler)
    app.run()