//const Core = require('@alicloud/pop-core');
import mqtt from '../../utils/mqtt.js';
const aliyunOpt = require('../../utils/aliyun/aliyun_connect.js');
import * as echarts from '../../ec-canvas/echarts'; //需要注意这个地方的路径不用引用错误了
const app = getApp();


Component({
  pageLifetimes: {
    show() {
      if (typeof this.getTabBar === 'function' &&
        this.getTabBar()) {
        this.getTabBar().setData({
          selected: 0
        })
      }
    },
    
  },
  data: {
    devicestauts:"下线",
    temperature: '',
    SpO2: '',
    heartrate: '',
    client: null, //记录重连的次数
    reconnectCounts: 0, //MQTT连接的配置
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
      productKey: 'a1RXUd800wl', //阿里云连接的三元组 ，请自己替代为自己的产品信息!!
      deviceName: 'wechat-1', //阿里云连接的三元组 ，请自己替代为自己的产品信息!!
      deviceSecret: '29b37d33bd03bd446d87437ee98f7588', //阿里云连接的三元组 ，请自己替代为自己的产品信息!!
      regionId: 'cn-shanghai', //阿里云连接的三元组 ，请自己替代为自己的产品信息!!
      pubTopic: '/a1RXUd800wl/wechat-1/user/wechatpub', //发布消息的主题
      subTopic: '/a1RXUd800wl/wechat-1/user/wechatget', //订阅消息的主题
    },
    //图表
    ec1: {
      //onInit: initChart1
    }
  },

  methods: {

    onLoad: function () {
      //生命周期函数，监听页面加载
      let that = this;
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
      //获取设备信息
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
        var buffer = JSON.parse(payload);
        //var that=this;
        that.setData({
          temperature: JSON.stringify(buffer.Temperature),
          SpO2: JSON.stringify(buffer.SpO2),
          heartrate: JSON.stringify(buffer.HeartRate),
          timestamp: JSON.stringify(buffer.timestamp),
        });

        //console.log(" 收到 topic:" + topic + " , payload :" + JSON.stringify(buffer.Temperature))
        console.log(" 收到 topic:" + topic + " , payload :" + JSON.stringify(buffer))
        /*
        wx.showModal({
          content: " 收到topic:[" + topic + "],温度 :[" + JSON.stringify(buffer.Temperature) + "°C]",
          showCancel: false,
        })
        */
        ;
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
    onClickPub(){
      if (this.data.client && this.data.client.connected) {
        var str={item: 'get'}
        
        this.data.client.publish(this.data.aliyunInfo.pubTopic, JSON.stringify(str),function (err) {
          if (!err) {
            wx.showToast({
              title: '发布成功'
            })
            console.log("发布成功");
            //console.log(err)
          } else {
            console.log("发布失败");
          }
        })
      } else {
        wx.showToast({
          title: '请先连接服务器',
          icon: 'none',
          duration: 2000
        })
      }

    },
    onClickSub() {
      if (this.data.client && this.data.client.connected) {
        this.data.client.subscribe(this.data.aliyunInfo.subTopic, function (err) {
          if (!err) {
            wx.showToast({
              title: '订阅成功'
            })
            console.log("订阅成功");
            //console.log(err)
          } else {
            console.log("订阅失败");
          }
        })
      } else {
        wx.showToast({
          title: '请先连接服务器',
          icon: 'none',
          duration: 2000
        })
      }
    },
    onClickunSub() {
      if (this.data.client && this.data.client.connected) {
        this.data.client.unsubscribe(this.data.aliyunInfo.subTopic, function (err) {
          if (!err) {
            wx.showToast({
              title: '取消订阅成功'
            })
            console.log("取消订阅成功");
          } else {
            console.log("取消订阅失败");
          }
        })
      } else {
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
  },
  onReady() {}
})


function initChart1(canvas, width, height) {
  var value = 84; //这里控制着圆环图的进度
  const chart = echarts.init(canvas, null, {
    width: width,
    height: height
  });
  canvas.setChart(chart);
  var option = {
    series: [{
      type: 'pie', //指定类型为饼状图
      clockWise: true,
      radius: ['70%', '75%'], //指定半径，注意不建议直接指定px，不利于自适应。
      itemStyle: {
        normal: {
          label: {
            show: false
          },
          labelLine: {
            show: false
          }
        }
      },
      hoverAnimation: false,
      data: [{
        value: value,
        name: 'completed',
        itemStyle: {
          normal: {
            borderWidth: 5,
            borderColor: {
              colorStops: [{
                offset: 0,
                color: '#F13577' // 0% 处的颜色
              }, {
                offset: 1,
                color: '#6076E1' // 100% 处的颜色
              }]
            },

            label: {
              show: false
            },
            labelLine: {
              show: false
            }
          }
        }
      }, {
        name: 'gap',
        value: 100 - value,
        itemStyle: {
          normal: {
            label: {
              show: false
            },
            labelLine: {
              show: false
            },
            color: 'rgba(0, 0, 0, 0)',
            borderColor: 'rgba(0, 0, 0, 0)',
            borderWidth: 0
          }
        }
      }]
    }]
  };
  chart.setOption(option);
  chart.resize();
  return chart;
}