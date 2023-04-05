import mqtt from '../../utils/mqtt.js';

const aliyunOpt = require('../../utils/aliyun/aliyun_connect.js');

let that = null;
var app = getApp();

Page({  
  data: {    //这一堆是用来区分按钮的
rightIOname: [{ id: "10", name: "D0", }, { id: "11", name: "D1", }, { id: "12", name: "D2", }, { id: "13", name: "D3", }, { id: "14", name: "D4", }, { id: "15", name: "D5", }, { id: "16", name: "D6", }, { id: "17", name: "D7", }, { id: "18", name: "D8",}],  
        },

  onPullDownRefresh: function () {   //下拉刷新
  app.connect();                        //连接服务器
  wx.stopPullDownRefresh() },          //停止刷新

  onLoad: function () { /*app.connect(); */ },//连接服务器
 

  switchchange(e) {                      //点击按钮会触发这个函数
    var value = e.detail.value
    var propertyName = e.currentTarget.dataset.propertyName
    console.log(propertyName)
    var a = app.data.payload.a

    if(a == 0){                        //如果读取到的a为零，就提示按钮名+HIGH或LOW
    wx.showToast({
      title: propertyName+"-"+"HIGH",       
    })}else{
      wx.showToast({
        title: propertyName + "-" + "LOW",
      })
    }

   if (value == true && propertyName == "D0") {
      app.sendCommond(0);                           //改括号里的数字，就改变了发送的值
    } else if (value == false && propertyName == "D0") {
     app.sendCommond(0);
    } else if (value == true && propertyName == "D1") {
     app.sendCommond(1);
    
    } else if (value == false && propertyName == "D1") {
     app.sendCommond(1);
     
    } else if (value == true && propertyName == "D2") {
     app.sendCommond(2);
    
    } else if (value == false && propertyName == "D2") {
     app.sendCommond(2);
    
    } else if (value == true && propertyName == "D3") {
     app.sendCommond(3);
   
    } else if (value == false && propertyName == "D3") {
     app.sendCommond(3);
    
    } else if (value == true && propertyName == "D4") {
     app.sendCommond(4);
     
    } else if (value == false && propertyName == "D4") {
     app.sendCommond(4);
      
    } else if (value == true && propertyName == "D5") {
     app.sendCommond(5);
      
    } else if (value == false && propertyName == "D5") {
     app.sendCommond(5);
     
    } else if (value == true && propertyName == "D6") {
     app.sendCommond(6);
   
    } else if (value == false && propertyName == "D6") {
     app.sendCommond(6);
    
    } else if (value == true && propertyName == "D7") {
     app.sendCommond(7);
      
    } else if (value == false && propertyName == "D7") {
     app.sendCommond(7);
    
    } else if (value == true && propertyName == "D8") {
     app.sendCommond(8);
      
    } else if (value == false && propertyName == "D8") {
     app.sendCommond(8);
     
    } 
  },

 
})