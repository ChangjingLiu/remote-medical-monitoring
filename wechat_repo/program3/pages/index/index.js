import mqtt from'../../utils/mqtt.js';

const aliyunOpt = require('../../utils/aliyun/aliyun_connect.js');

let that = null;
Page({
    data:{
      T:'',
      SpO2:'',
      HB:'',
      client:null,//记录重连的次数
      reconnectCounts:0,//MQTT连接的配置
      options:{
        protocolVersion: 4, //MQTT连接协议版本
        clean: false,
        reconnectPeriod: 1000, //1000毫秒，两次重新连接之间的间隔
        connectTimeout: 30 * 1000, //1000毫秒，两次重新连接之间的间隔
        resubscribe: true, //如果连接断开并重新连接，则会再次自动订阅已订阅的主题（默认true）
        clientId: '',
        password: '',
        username: '',
      },

      aliyunInfo: {
        productKey: 'a1RXUd800wl', //阿里云连接的三元组 ，请自己替代为自己的产品信息!!
        deviceName: 'wechat-1', //阿里云连接的三元组 ，请自己替代为自己的产品信息!!
        deviceSecret: '29b37d33bd03bd446d87437ee98f7588', //阿里云连接的三元组 ，请自己替代为自己的产品信息!!
        regionId: 'cn-shanghai', //阿里云连接的三元组 ，请自己替代为自己的产品信息!!
        pubTopic: '/sys/a1RXUd800wl/wechat-1/user/weixin', //发布消息的主题
        subTopic: '/a1RXUd800wl/wechat-1/user/get', //订阅消息的主题
      },


    },
  onLoad:function(){
    //生命周期函数，监听页面加载
  that = this;
      let clientOpt = aliyunOpt.getAliyunIotMqttClient({
        productKey: that.data.aliyunInfo.productKey,
        deviceName: that.data.aliyunInfo.deviceName,
        deviceSecret: that.data.aliyunInfo.deviceSecret,
        regionId: that.data.aliyunInfo.regionId,
        port: that.data.aliyunInfo.port,
      });
    
      let host = 'wxs://' + clientOpt.host;
      console.log("get data:" + JSON.stringify(clientOpt));
      this.setData({
        'options.clientId': clientOpt.clientId,
        'options.password': clientOpt.password,
        'options.username': clientOpt.username,
      })
      console.log("this.data.options host:" + host);
      console.log("this.data.options data:" + JSON.stringify(this.data.options));

      this.data.client = mqtt.connect(host, this.data.options);
      this.data.client.on('connect', function (connack) {
        wx.showToast({
          title: '连接成功'
        })
      })
      //订阅
      //this.data.client.subscribe(this.data.aliyunInfo.subTopic, function (err) {
      //})

      that.data.client.on("message", function (topic, payload) {
        // message is Buffer
        var buffer=JSON.parse(payload);
        //var that=this;
        that.setData({
          temperature: JSON.stringify(buffer.Temperature),
          heartrate: JSON.stringify(buffer.HeartRate),
          SpO2: JSON.stringify(buffer.SpO2),
          timestamp: JSON.stringify(buffer.timestamp),
        });
        
        console.log(" 收到 topic:" + topic + " , payload :" + JSON.stringify(buffer.Temperature))
        /*
        wx.showModal({
          content: " 收到topic:[" + topic + "],温度 :[" + JSON.stringify(buffer.Temperature) + "°C]",
          showCancel: false,
        })
        */;
      })
      //服务器连接异常的回调
      that.data.client.on("error", function (error) {
        console.log(" 服务器 error 的回调" + error)

      })
      //服务器重连连接异常的回调
      that.data.client.on("reconnect", function () {
        console.log(" 服务器 reconnect的回调")

      })
      //服务器连接异常的回调
      that.data.client.on("offline", function (errr) {
        console.log(" 服务器offline的回调")
      })


    },
  onClickOpen() {
    that.sendCommond('set', 37);
  },
  onClickSub() {
    if (this.data.client && this.data.client.connected){
      this.data.client.subscribe(this.data.aliyunInfo.subTopic, function (err) {
        if(!err){
          wx.showToast({
            title: '订阅成功'
          })
          console.log("订阅成功");
        }
        else{
          console.log("订阅失败");
        }
      })
    }else {
      wx.showToast({
        title: '请先连接服务器',
        icon: 'none',
        duration: 2000
      })
    }
  },
  onClickunSub() {
    if (this.data.client && this.data.client.connected){
      this.data.client.unsubscribe(this.data.aliyunInfo.subTopic, function (err) {
        if(!err){
          wx.showToast({
            title: '取消订阅成功'
          })
          console.log("取消订阅成功");
        }
        else{
          console.log("取消订阅失败");
        }
      })
    }else {
      wx.showToast({
        title: '请先连接服务器',
        icon: 'none',
        duration: 2000
      })
    }
  },
  sendCommond(cmd, data) {
    let sendData = {
      //cmd: cmd,
      temperature: data,
    };
    if (this.data.client && this.data.client.connected) {
      this.data.client.publish(this.data.aliyunInfo.pubTopic, JSON.stringify(sendData));
      wx.showToast({
        title: '发布成功'
      })

    } else {
      wx.showToast({
        title: '请先连接服务器',
        icon: 'none',
        duration: 2000
      })
    }
  }
})