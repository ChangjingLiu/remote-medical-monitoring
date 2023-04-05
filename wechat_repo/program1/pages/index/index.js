import mqtt from'../../utils/mqtt.js';

const aliyunOpt = require('../../utils/aliyun/aliyun_connect.js');

let that = null;
Page({
    data:{
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
        productKey: '', //阿里云连接的三元组 ，请自己替代为自己的产品信息!!
        deviceName: 'wechat-1', //阿里云连接的三元组 ，请自己替代为自己的产品信息!!
        deviceSecret: '29b37d33bd03bd446d87437ee98f7588', //阿里云连接的三元组 ，请自己替代为自己的产品信息!!
        regionId: 'cn-shanghai', //阿里云连接的三元组 ，请自己替代为自己的产品信息!!
        pubTopic: '/a1RXUd800wl/weixin/user/weixin', //发布消息的主题
        subTopic: '/a1RXUd800wl/weixin/user/get', //订阅消息的主题
      },


    },
  aliiot_connect(){
  that = this;
      let clientOpt = aliyunOpt.getAliyunIotMqttClient({
        productKey: that.data.aliyunInfo.productKey,
        deviceName: that.data.aliyunInfo.deviceName,
        deviceSecret: that.data.aliyunInfo.deviceSecret,
        regionId: that.data.aliyunInfo.regionId,
        port: that.data.aliyunInfo.port,
      });
      console.log("get data:" + JSON.stringify(clientOpt));
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

      that.data.client.on("message", function (topic, payload) {
        console.log(" 收到 topic:" + topic + " , payload :" + payload)
        wx.showModal({
          content: " 收到topic:[" + topic + "], payload :[" + payload + "]",
          showCancel: false,
        });
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
    formBindsubmit:function(e){
      console.log(e.detail.value);
      this.data.aliyunInfo.productKey=e.detail.value.productKey;
      this.data.aliyunInfo.deviceName=e.detail.value.deviceName;
      this.data.aliyunInfo.deviceSecret=e.detail.value.deviceSecret;
      this.data.aliyunInfo.pubTopic='/'+e.detail.value.productKey+'/weixin/user/weixin';
      this.data.aliyunInfo.subTopic='/'+e.detail.value.productKey+'/weixin/user/get';

      this.aliiot_connect();
    },
  submit() {
    //this.formBindsubmit;
    },
  onClickOpen() {
    that.sendCommond('set', 1);
  },
  onClickOff() {
    that.sendCommond('set', 0);
  },
  sendCommond(cmd, data) {
    let sendData = {
      cmd: cmd,
      data: data,
    };
    if (this.data.client && this.data.client.connected) {
      this.data.client.publish(this.data.aliyunInfo.pubTopic, JSON.stringify(sendData));

    } else {
      wx.showToast({
        title: '请先连接服务器',
        icon: 'none',
        duration: 2000
      })
    }
  }
})