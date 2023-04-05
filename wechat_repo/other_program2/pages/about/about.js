Page({

  //获取个人信息
  getMyInfo: function (e) {
    console.log(e.detail.userInfo)
    let info = e.detail.userInfo
    this.setData({
      src: info.avatarUrl,
      nickName: info.nickName,
      isLogin: true,
    })},
  
  onLoad:function(options){
 
      }
    })
 
 