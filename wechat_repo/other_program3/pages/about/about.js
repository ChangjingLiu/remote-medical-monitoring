Page({
  data:{
    
    productKey:"",      //这个是为了把三元组和发布TOPIC存入缓存建的数组
    deviceName:"",
    deviceSecret:"",
    pubTopic:"",

  },

  //获取个人信息
  getMyInfo: function (e) {
    this.getvalue();
    console.log(e.detail.userInfo)
    let info = e.detail.userInfo
    this.setData({
      src: info.avatarUrl,
      nickName: info.nickName,
      isLogin: true,
    })},
  
  onLoad:function(options){
 
      },
      //页面刚开始加载时触发这个函数
  onShow:function () {
   
    if (this.data.isLogin) {    //如果我们已经登录，就执行这个函数
      this.getvalue();
    }
  },

  productKey: function (e) {                 //我们输入东西会触发这个函数
    console.log("productKey:" + e.detail.value)
    let value = e.detail.value
    wx.setStorageSync("productKey", value)  //   将数据存入缓存
  },
  deviceName: function (e) {
    console.log("deviceName:" + e.detail.value)
    let value = e.detail.value
    wx.setStorageSync("deviceName", value)
  },
  deviceSecret: function (e) {
    console.log("deviceSecret:" + e.detail.value) 
    let value = e.detail.value
    wx.setStorageSync("deviceSecret", value)
    },
  pubTopic: function (e) {
    console.log("pubTopic:" + e.detail.value)
    let value = e.detail.value
    wx.setStorageSync("pubTopic", value)
  },
 
  getvalue:function(){                       //把数据从缓存中拿出来，传值给data里的数组
    this.setData({
      productKey: wx.getStorageSync("productKey"),
      deviceName: wx.getStorageSync("deviceName"),
      deviceSecret:wx.getStorageSync("deviceSecret"),
      pubTopic: wx.getStorageSync("pubTopic"),
    })
  }, 
  help:function(){
    wx.showModal({
      content: " 源码及使用方法，请在billbill,搜索：爱学习的素丸子，UID：269864982；在专栏3中查看",
      showCancel: false,
    });
  }
    })
 
 