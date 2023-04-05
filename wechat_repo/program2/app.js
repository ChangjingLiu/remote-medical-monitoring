// app.js
App({
  onLaunch() {
    // 展示本地存储能力
    const logs = wx.getStorageSync('logs') || []
    logs.unshift(Date.now())
    wx.setStorageSync('logs', logs)

    // 登录
    /*
    wx.login({
      success: res => {
        // 发送 res.code 到后台换取 openId, sessionKey, unionId
        this.globalData.code =  res.code
        http.authCode2Session({ // 调用接口，传入参数 这个接口就是自家后台给你提供的 post请求
          data: {
            jsCode: res.code,
            grantType: 'getPhoneNumber',
          },
          success: res => {
            this.globalData.openid = res.openid // 存入 globalData 
          },
          fail: err => {
            console.log(err)
          }
        })
      }
    })
    // 获取用户信息
    */
  },
  globalData: {
    userInfo: null
  }
})
