import * as echarts from '../../ec-canvas/echarts'; //需要注意这个地方的路径不用引用错误了
const app = getApp();
let chart = null;

function initChart(canvas, width, height) {
  chart = echarts.init(canvas, null, {
    width: width,
    height: height
  });
  canvas.setChart(chart);
  var data = [{
      //name: '"2018-07-05 18:00:29"',
      value: ["2018-07-05 18:00:29", 35]
    },
    {
      //name: '2018-07-04 18:00:29',
      value: ["2018-07-04 18:00:29", 36]
    },
    {
      //name: '2018-07-03 18:00:29',
      value: ["2018-07-03 18:00:29", 36.5]
    },
    {
      //name: '2018-07-03 12:00:29',
      value: ["2018-07-03 12:00:29", 36]
    },
    {
      //name: '2018-07-02 18:00:29',
      value: ["2018-07-02 18:00:29", 37.5]
    },
    {
      //name: '2018-07-01 18:00:29',
      value: ["2018-07-01 18:00:29", 38]
    }
  ];
  //时间显示范围
  var anchor = [data[0], data[data.length - 1]];
  // 指定图表的配置项和数据
  var option = {
    backgroundColor: '#fff', //背景色
    grid: {
      left: 35,
      right: 20,
      top: 10,
      bottom: 40,
      show: false
    },

    // 悬浮图标
    tooltip: {
      // 表示不使用默认的『显示』『隐藏』触发规则。
      triggerOn: 'none',
      formatter: function (params) {
        return 'X: ' + params.data[0].toFixed(2) + '<br>Y: ' + params.data[1].toFixed(2);
      }
    },
    /*
    tooltip: {
      // trigger: 'axis',
      formatter: function (param) {
        return param.value[0] + "<br>" + param.value[1] + "°C";
      }
    },
    */
    xAxis: {
      type: "time",
      splitNumber: 10,
      axisLabel: {
        // rotate: 60,
        textStyle: {
          fontSize: 8
        }
      }
    },
    yAxis: {
      min: 35
    },
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
        color: '#FA578B'
      }
      // show : false
    },
    series: [{
        data: data,
        markLine: {
          silent: true,
          data: [{
            yAxis: 37
          }]
        },
        type: 'line',
        smooth: true,
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
      },
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
    ]
  };
  chart.setOption(option);
  return chart;
}
Page({
  data: {
    ec: {
      onInit: initChart
    }
  },
  onReady() {},
  updateData() {
    var data = [{
        //name: '"2018-07-05 18:00:29"',
        value: ["2018-07-05 18:00:29", 37]
      },
      {
        //name: '2018-07-04 18:00:29',
        value: ["2018-07-04 18:00:29", 36]
      },
      {
        //name: '2018-07-03 18:00:29',
        value: ["2018-07-03 18:00:29", 36.5]
      },
      {
        //name: '2018-07-03 12:00:29',
        value: ["2018-07-03 12:00:29", 36]
      },
      {
        //name: '2018-07-02 18:00:29',
        value: ["2018-07-02 18:00:29", 37.5]
      },
      {
        //name: '2018-07-01 18:00:29',
        value: ["2018-07-01 18:00:29", 38]
      }
    ];
    //时间显示范围
    var anchor = [data[0], data[data.length - 1]];
    chart.setOption({
      xAxis: {
        //data: month
      },
      series: [{
        //name: ‘蒸发量‘, // 根据名字对应到相应的系列
        data: data
      }, {
        //name: ‘降水量‘,
        data: anchor
      }]
    });
  }
});