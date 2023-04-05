import mqtt from '/utils/mqtt.js';

const aliyunOpt = require('/utils/aliyun/aliyun_connect.js');
let that = null;

App({
  data:{
    payload:"",
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

  connect:function(e){
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
    
     that.data.options.clientId= clientOpt.clientId,
     that.data.options.password=clientOpt.password,
     that.data.options.username= clientOpt.username,
   
    console.log("这就是网址" + host);
    console.log("this.data.options data:" + JSON.stringify(this.data.options));

    this.data.client = mqtt.connect(host, this.data.options);
    console.log(this.data.client);

    this.data.client.on(
      'connect',
      function (connack) {
        wx.showToast({ title: '连接成功' })
        app.sendCommond(9); }
    )


    that.data.client.on("message", function (topic, payload) {
      console.log(" 收到 topic:" + topic + " , payload :" + payload)
      that.data.payload = JSON.parse(payload)
      
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

    }) },


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

  /**
   * 当小程序初始化完成时，会触发 onLaunch（全局只触发一次）
   */
  onLaunch: function () {},

  /**
   * 当小程序启动，或从后台进入前台显示，会触发 onShow
   */
  onShow: function (options) {
    
  },

  /**
   * 当小程序从前台进入后台，会触发 onHide
   */
  onHide: function () {
    
  },

  /**
   * 当小程序发生脚本错误，或者 api 调用失败时，会触发 onError 并带上错误信息
   */
  onError: function (msg) {
    
  }
})
