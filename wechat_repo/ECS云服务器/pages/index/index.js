import * as echarts from '../../ec-canvas/echarts'; //需要注意这个地方的路径不用引用错误了
const app = getApp();
let chart = null;



Page({
  data: {
    ec: {
      onInit: initChart
    },
    queryResult: null,
    showView: 'false',
  },
  updateData: function () {
    let that = this;
    wx.request({
      url: 'http://127.0.0.1:5000/getUserData',
      method: 'GET',
      data: {
        UMAC: '12345'
      },
      success(res) {
        if (res.statusCode == 200) {
          console.log(res.data);
          that.setData({
            para: res.data[1],
            showView: !that.data.showView,
          });
          ///////////////////////数据处理
          //console.log(res.data.length)
          var data_tem = [];
          var data_SpO2 = [];
          var data_HR = [];
          for (var i = 0; i < res.data.length; i++) {
            var array_tem = [res.data[i].Time, res.data[i].Tem]
            var array_SpO2 = [res.data[i].Time, res.data[i].SpO2]
            var array_HR = [res.data[i].Time, res.data[i].HR]
            var temp_tem = {
              name:'温度',
              value: array_tem
            }
            var temp_SpO2 = {
              name:'血氧',
              value: array_SpO2
            }
            var temp_HR = {
              name:'心率',
              value: array_HR
            }
            data_tem.push(temp_tem),
            data_SpO2.push(temp_SpO2),
            data_HR.push(temp_HR)
          }

          //console.log(data)
          //时间显示范围自动生成
          //var anchor = [data[0], data[data.length - 1]];
          chart.setOption({
            xAxis: {
            },
            series: [{
              name:'温度',
              data: data_tem
            },
            {
              name:'血氧',
              data: data_SpO2
            },
            {
              name:'心率',
              data: data_HR
            }]
          });
          //////////////////////////
        }
      }

    })

  },
  bindblur: function (e) {
    let that = this;
    wx.request({
      url: 'http://127.0.0.1:5000/getUserData',
      method: 'GET',
      data: {
        UMAC: e.detail.value
      },
      success(res) {
        if (res.statusCode == 200) {
          that.setData({
            para: res.data,
            showView: !that.data.showView,
          });
        } else {
          that.setData({
            showView: 'false',
          });
          wx.showToast({
            title: '请输入正确的实例ID',
            duration: 1500,
            icon: 'none',
            mask: true
          })
        }
      }

    })
  }
})



function initChart(canvas, width, height) {
  chart = echarts.init(canvas, null, {
    width: width,
    height: height
  });
  canvas.setChart(chart);
  var data=[]
  var data_SpO2=[]
  var data_HR=[]
  //时间显示范围
  var anchor = [data[0], data[data.length - 1]];
  // 指定图表的配置项和数据
  var option = {
    legend: {},
    backgroundColor: '#fff', //背景色
    grid: [{
      left: '20%',
      //right: '5%',
      top: '10%',
      bottom: '65%',
      show: true
    },
    {
      left: '20%',
      //right: '5%',
      top: '39%',
      bottom:'39%',
      show: true
    },
    {
      left: '20%',
      //right: '5%',
      top: '65%',
      bottom:'10%',
      show: true
    }],

    // 悬浮图标
    tooltip: [{
      //hideDelay:2000,
      show: true,
      trigger: 'axis',
      //formatter: '°C',
      //这一步的效果是当你的光标在左边的时候，悬浮标题在右边，在右边的时候，悬浮标题在左边
      formatter: function (params){
        console.log(params)
        var str = '',
        str=params[0].seriesName+': '+'\n'+params[0].value[1]
        if (params[0].seriesName=='血氧')
{
          str+=' %';
        }
        if (params[0].seriesName=='温度')
        {
          str+=' °C';
        }
        if (params[0].seriesName=='心率')
        {
          str+=' BPM';
        }
        str+='\n'+'时间：'+'\n'+params[0].value[0].substring(0,10)+'\n'+params[0].value[0].substring(11,19)
        return str;
      }
    },
    /*
    {
      show: true,
      trigger: 'axis',
      //formatter: '°C',
      //这一步的效果是当你的光标在左边的时候，悬浮标题在右边，在右边的时候，悬浮标题在左边
      formatter: function (params){
        var str = ''
        str='血氧: '+params[0].value[1]+' ';
        return str;
      }
    }*/
  ],
    /*
    tooltip: {
      // trigger: 'axis',
      formatter: function (param) {
        return param.value[0] + "<br>" + param.value[1] + "°C";
      }
    },
    */
    xAxis: [{
      type: "time",
      gridIndex: 0,
      splitNumber: 10,
      //
      axisLabel: {
        show:false,
        // rotate: 60,
        textStyle: {
          fontSize: 8
        }
      }
    },
    {
      type: "time",
      gridIndex: 1,
      splitNumber: 10,
      axisLabel: {
        show:false,
        // rotate: 60,
        textStyle: {
          fontSize: 8
        }
      }
    },
    {
      type: "time",
      gridIndex: 2,
      splitNumber: 10,
      axisLabel: {
        // rotate: 60,
        textStyle: {
          fontSize: 8
        }
      }
    }],
    yAxis: [{
      //name:'温度',
      gridIndex: 0,
      min: 35,
      axisLabel: {
        // rotate: 60,
        textStyle: {
          fontSize: 10
        }
      }
    },
    {
      //name:'血氧',
      gridIndex: 1,
      min: 90,
      axisLabel: {
        // rotate: 60,
        textStyle: {
          fontSize: 10
        }
      }
    },
    {
      //name:'血氧',
      gridIndex: 2,
      min: 55,
      axisLabel: {
        // rotate: 60,
        textStyle: {
          fontSize: 10
        }
      }
    },
  ],
    /*
    visualMap: {
      top: 10,
      right: 10,
      pieces: [{
        gte: 0,
        lte: 37,
        color: '#4D74FA'
      }, {
        gt: 37,
        lte: 40,
        color: '#FA578B'
      }],
      outOfRange: {
        color: '#808080'
      }
      // show : false
    },
    */
    series: [{
        name:'温度',
        xAxisIndex: 0,
        yAxisIndex: 0,
        type: 'line',
        smooth: true,
        data: data,
        /*
        markLine: {
          silent: true,
          data: [{
            yAxis: 37
          }]
        },
        markArea: {
          data: [
            [{
                yAxis: 37
              },
              {}
            ]
          ],
          itemStyle: {
            opacity: 0.5,
            color: {
              type: 'linear',
              x: 0,
              y: 0,
              x2: 0,
              y2: 1,
              colorStops: [{
                  offset: 0,
                  color: '#fff' // 0% 处的颜色
                },
                {
                  offset: 1,
                  color: '#E7AEAD' // 100% 处的颜色
                }
              ],
              globalCoord: false // 缺省为 false
            }
          }
        }
        */
      },
      {
        name:'血氧',
        xAxisIndex: 1,
        yAxisIndex: 1,
        type: 'line',
        smooth: true,
        data: data_SpO2,
      },
      {
        name:'心率',
        xAxisIndex: 2,
        yAxisIndex: 2,
        type: 'line',
        smooth: true,
        data: data_HR,
      }
      
      /*
      {
        name: '.anchor',
        type: 'line',
        showSymbol: false,
        data: anchor,
        itemStyle: {
          normal: {
            opacity: 0
          }
        }, //不绘制该线条
        lineStyle: {
          normal: {
            opacity: 0
          }
        }
      }
      */
    ]
  };
  chart.setOption(option);
  return chart;
}