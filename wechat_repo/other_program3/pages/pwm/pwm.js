
import mqtt from '../../utils/mqtt.js';

const aliyunOpt = require('../../utils/aliyun/aliyun_connect.js');
let that = null;

var app = getApp();

Page({
  data: {
    light: "",
    currentTemperature:"",


    a0: "A0读取值",
    wendu:"温度",
    pwm:"PWM",
    servo:"Servo",
    backgroundimage: false,

    client: null,//记录重连的次数
    reconnectCounts: 0,//MQTT连接的配置
    options: {
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
      productKey: wx.getStorageSync("productKey"), //阿里云连接的三元组 ，请自己替代为自己的产品信息!!
      deviceName: wx.getStorageSync("deviceName"), //阿里云连接的三元组 ，请自己替代为自己的产品信息!!
      deviceSecret: wx.getStorageSync("deviceSecret"), //阿里云连接的三元组 ，请自己替代为自己的产品信息!!
      regionId: 'cn-shanghai', //阿里云连接的三元组 ，请自己替代为自己的产品信息!!
      pubTopic: wx.getStorageSync("pubTopic"), //发布消息的主题
      subTopic: '/sys/a1qNrNN1l9s/weixin/thing/service/property/set', //订阅消息的主题
    },
  },


  onLoad: function (){
    this.connect(); 
  },


 

  onPullDownRefresh: function () {
    this.connect();
    this.setData({
      
    a0: wx.getStorageSync("a0"),
    wendu: wx.getStorageSync("wendu"),
     pwm: wx.getStorageSync("pwm"),
     servo: wx.getStorageSync("servo"),
    })
    wx.stopPullDownRefresh()
  },

  sliderchange(e){              //滑动按钮触发
      console.log(e)
      let data = e.detail.value
      var propertyName = e.currentTarget.dataset.propertyName
      if (propertyName == "PWM") {
        that.sendCommond(data);
        this.setData({
          numberD2: data * 28
        })
      } else if (propertyName == "Servo") {
        that.sendCommond(data);
        this.setData({
          numberD5: data * 20
        })}
   
  },

  a0: function (e){
    let value = e.detail.value
    wx.setStorageSync("a0", value)
  },
  wendu: function (e) {
    let value = e.detail.value
    wx.setStorageSync("wendu", value)
  },
  pwm: function (e){
    let value = e.detail.value
    wx.setStorageSync("pwm", value)
  },
   servo:function(e) {
    let value = e.detail.value
    wx.setStorageSync("servo", value)
  },








  connect: function () {
    that = this;
    let clientOpt = aliyunOpt.getAliyunIotMqttClient({
      productKey: that.data.aliyunInfo.productKey,
      deviceName: that.data.aliyunInfo.deviceName,
      deviceSecret: that.data.aliyunInfo.deviceSecret,
      regionId: that.data.aliyunInfo.regionId,
      port: that.data.aliyunInfo.port,
    });
    console.log(clientOpt);
    console.log("get data:" + JSON.stringify(clientOpt));
    let host = 'wxs://' + clientOpt.host;
    console.log("get data:" + JSON.stringify(clientOpt));

    that.data.options.clientId = clientOpt.clientId,
      that.data.options.password = clientOpt.password,
      that.data.options.username = clientOpt.username,

      console.log("这就是网址" + host);
    console.log("this.data.options data:" + JSON.stringify(this.data.options));

    this.data.client = mqtt.connect(host, this.data.options);
    console.log(this.data.client);

    this.data.client.on(
      'connect',
      function (connack) {
        wx.showToast({ title: '连接成功' })
        that.sendCommond("");
      }
    )


    that.data.client.on("message", function (topic, payload) {
      console.log(" 收到 topic:" + topic + " , payload :" + payload)
      that.setData({
        light: JSON.parse(payload).read,
        currentTemperature: JSON.parse(payload).currentTemperature,
      })

    })
  },
  sendCommond(data) {
    let sendData = data

    if (this.data.client && this.data.client.connected) {
      this.data.client.publish(this.data.aliyunInfo.pubTopic, JSON.stringify(sendData));

    } else {
      wx.showToast({
        title: '请先连接服务器',
        icon: 'none',
        duration: 2000
      })
    }
  },


  
  
})