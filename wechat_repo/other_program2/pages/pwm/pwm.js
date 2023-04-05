import mqtt from '../../utils/mqtt.js';

const aliyunOpt = require('../../utils/aliyun/aliyun_connect.js');

let that = null;
Page({
  data: {
    numberD2:0,
    numberD5:0,
    numberD6:0,
    numberD8:0,
    numberP2:0,
    numberP5:0,
    numberP6:0,
    numberP8:0,
    pwmName: [{ id: "1",name:"D2"},{ id: "2",name:"D5"},{ id: "3",name:"D6"},{ id: "4",name:"D8",}],
    posName: [{ id: "5", name: "P2" }, { id: "6", name: "P5" }, { id: "7", name: "P6" }, { id: "8", name: "P8", }],


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
      productKey: 'a1qNrNN1l9s', //阿里云连接的三元组 ，请自己替代为自己的产品信息!!
      deviceName: 'weixin', //阿里云连接的三元组 ，请自己替代为自己的产品信息!!
      deviceSecret: 'LlpWwcZtMRTwMun6eIlNvwTPDGul9M62', //阿里云连接的三元组 ，请自己替代为自己的产品信息!!
      regionId: 'cn-shanghai', //阿里云连接的三元组 ，请自己替代为自己的产品信息!!
      pubTopic: '/a1qNrNN1l9s/weixin/user/weixin', //发布消息的主题
      subTopic: '/sys/a1qNrNN1l9s/weixin/thing/service/property/set', //订阅消息的主题
    },

  },
  onPullDownRefresh: function () {
    that = this;
    let clientOpt = aliyunOpt.getAliyunIotMqttClient({
      productKey: that.data.aliyunInfo.productKey,
      deviceName: that.data.aliyunInfo.deviceName,
      deviceSecret: that.data.aliyunInfo.deviceSecret,
      regionId: that.data.aliyunInfo.regionId,
      port: that.data.aliyunInfo.port,//port
    });
    console.log(clientOpt);
    console.log("get data:" + JSON.stringify(clientOpt));
    let host = 'wxs://' + clientOpt.host;

    this.setData({
      'options.clientId': clientOpt.clientId,
      'options.password': clientOpt.password,
      'options.username': clientOpt.username,
    })
    console.log("这就是网址" + host);



    this.data.client = mqtt.connect(host, this.data.options);
    console.log(this.data.client);
    this.data.client.on('connect',function (connack) {
        wx.showToast({
          title: '连接成功'
        })
      }
    )
    console.log("------------------");
    console.log(that.data.client.on);

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
    wx.stopPullDownRefresh()
  },

  onLoad: function () {
    that = this;
    let clientOpt = aliyunOpt.getAliyunIotMqttClient({
      productKey: that.data.aliyunInfo.productKey,
      deviceName: that.data.aliyunInfo.deviceName,
      deviceSecret: that.data.aliyunInfo.deviceSecret,
      regionId: that.data.aliyunInfo.regionId,
      port: that.data.aliyunInfo.port,//port
    });
    console.log(clientOpt);
    console.log("get data:" + JSON.stringify(clientOpt));
    let host = 'wxs://' + clientOpt.host;
    console.log("get data:" + JSON.stringify(clientOpt));
    this.setData({
      'options.clientId': clientOpt.clientId,
      'options.password': clientOpt.password,
      'options.username': clientOpt.username,
    })
    console.log("这就是网址" + host);
    console.log("this.data.options data:" + JSON.stringify(this.data.options));

    this.data.client = mqtt.connect(host, this.data.options);
    console.log(this.data.client);
    this.data.client.on(
      'connect',
      function (connack) {
        wx.showToast({
          title: '连接成功'
        })
      }
    )
  

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
  changeProperty:function(e){
    console.log(e)
    let data= e.detail.value
    var propertyName = e.currentTarget.dataset.propertyName
    if (propertyName == "D2"){
    that.sendCommond(data);
    this.setData({
      numberD2: data*28
      })
    } else if (propertyName == "D5") {
      that.sendCommond(data);
      this.setData({
        numberD5: data*28
      })
    } else if (propertyName == "D6") {
      that.sendCommond(data);
      this.setData({
        numberD6: data*28
      })
    } else if (propertyName == "D8"){
      that.sendCommond(data);
      this.setData({
        numberD8: data*28
      })
    } else if (propertyName == "P2") {
      that.sendCommond(data);
      this.setData({
        numberP2: data*20
      })
    } else if (propertyName == "P5") {
      that.sendCommond( data);
      this.setData({
        numberP5: data*20
      })
    } else if (propertyName == "P6") {
      that.sendCommond( data);
      this.setData({
        numberP6: data*20
      })
    } else if (propertyName == "P8") {
      that.sendCommond(data);
      this.setData({
        numberP8: data*20
      })
    }
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
  }
})