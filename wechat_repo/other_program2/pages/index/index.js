import mqtt from '../../utils/mqtt.js';

const aliyunOpt = require('../../utils/aliyun/aliyun_connect.js');

let that = null;
Page({


    // wx.stopPullDownRefresh() // 可以停止当前页面的下拉刷新。

  
  
  data: {
  
    rightIOname: [{ id: "10", name: "D0", }, { id: "11", name: "D1", }, { id: "12", name: "D2", }, { id: "13", name: "D3", }, { id: "14", name: "D4", }, { id: "15", name: "D5", }, { id: "16", name: "D6", }, { id: "17", name: "D7", }, { id: "18", name: "D8",}],
       client: null,//记录重连的次数
       reconnectCounts: 0,//MQTT连接的配置
       options: {
      protocolVersion: 4, //MQTT连接协议版本
      clean: false,
      reconnectPeriod: 1000, //1000毫秒，两次重新连接之间的间隔
      connectTimeout: 30*1000, //1000毫秒，两次重新连接之间的间隔
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
      subTopic: '/a1qNrNN1l9s/weixin/user/weixin', //订阅消息的主题
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


  },
  switchchange(e) {
    var value = e.detail.value
    var propertyName = e.currentTarget.dataset.propertyName
    console.log(propertyName)
   if (value == true && propertyName == "D0") {
      that.sendCommond(0);
      wx.showToast({
        title: 'D0-HIGH',
      })
    } else if (value == false && propertyName == "D0") {
      that.sendCommond(0);
      wx.showToast({
        title: 'D0-LOW',
      })
    } else if (value == true && propertyName == "D1") {
      that.sendCommond(1);
      wx.showToast({
        title: 'D1-HIGH',
      })
    } else if (value == false && propertyName == "D1") {
      that.sendCommond(1);
      wx.showToast({
        title: 'D1-LOW',
      })
    } else if (value == true && propertyName == "D2") {
      that.sendCommond(2);
      wx.showToast({
        title: 'D2-HIGH',
      })
    } else if (value == false && propertyName == "D2") {
      that.sendCommond(2);
      wx.showToast({
        title: 'D2-LOW',
      })
    } else if (value == true && propertyName == "D3") {
      that.sendCommond(3);
      wx.showToast({
        title: 'D3-HIGH',
      })
    } else if (value == false && propertyName == "D3") {
      that.sendCommond(3);
      wx.showToast({
        title: 'D3-LOW',
      })
    } else if (value == true && propertyName == "D4") {
      that.sendCommond(4);
      wx.showToast({
        title: 'D4-HIGH',
      })
    } else if (value == false && propertyName == "D4") {
      that.sendCommond(4);
      wx.showToast({
        title: 'D4-LOW',
      })
    } else if (value == true && propertyName == "D5") {
      that.sendCommond(5);
      wx.showToast({
        title: 'D5-HIGH',
      })
    } else if (value == false && propertyName == "D5") {
      that.sendCommond(5);
      wx.showToast({
        title: 'D5-LOW',
      })
    } else if (value == true && propertyName == "D6") {
      that.sendCommond(6);
      wx.showToast({
        title: 'D6-HIGH',
      })
    } else if (value == false && propertyName == "D6") {
      that.sendCommond(6);
      wx.showToast({
        title: 'D6-LOW',
      })
    } else if (value == true && propertyName == "D7") {
      that.sendCommond(7);
      wx.showToast({
        title: 'D7-HIGH',
      })
    } else if (value == false && propertyName == "D7") {
      that.sendCommond(7);
      wx.showToast({
        title: 'D7-LOW',
      })
    } else if (value == true && propertyName == "D8") {
      that.sendCommond(8);
      wx.showToast({
        title: 'D8-HIGH',
      })
    } else if (value == false && propertyName == "D8") {
      that.sendCommond(8);
      wx.showToast({
        title: 'D8-LOW',
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