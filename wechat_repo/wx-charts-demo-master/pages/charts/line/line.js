var wxCharts = require('../../../utils/wxcharts.js');
var app = getApp();
var lineChart = null;
Page({
    data: {
    },
    touchHandler: function (e) {//返回键，后期可以删除
        console.log(lineChart.getCurrentDataIndex(e));
        lineChart.showToolTip(e, {
            // background: '#7cb5ec',
            format: function (item, category) {
                return category + ' ' + item.name + ':' + item.data 
            }
        });
    },    
    createSimulationData: function () {//生成模拟数据，这个后期要删除
        var categories = [];//表的xlabel
        var data = [];//表数据
        for (var i = 0; i < 24; i++) {
            categories.push((i/2+1)+'时');
            data.push(Math.random()*(20-10)+60);
        }
        // data[4] = null;
        return {
            categories: categories,
            data: data
        }
    },
    updateData: function () {//更新数据
        var simulationData = this.createSimulationData();
        var series = [{
            name: '心率',
            data: simulationData.data,
            format: function (val, name) {
                return val.toFixed(2) + 'BPM';
            }
        }];
        lineChart.updateData({
            categories: simulationData.categories,
            series: series
        });
    },
    
    onLoad: function (e) {//页面第一次加载
        var windowWidth = 320;
        try {
            var res = wx.getSystemInfoSync();
            windowWidth = res.windowWidth;
        } catch (e) {
            console.error('getSystemInfoSync failed!');
        }
        
        var simulationData = this.createSimulationData();
        lineChart = new wxCharts({
            canvasId: 'lineCanvas',
            type: 'line',
            categories: simulationData.categories,
            animation: true,
            // background: '#f5f5f5',
            
            series: [{
                name: '心率',
                data: simulationData.data,
                
                format: function (val, name) {
                    return val.toFixed(2) + 'BPM';
                }
                
            }],
            
            xAxis: {
                disableGrid: true
            },
            yAxis: {
                title: 'HR',
                format: function (val) {
                    return val.toFixed(2);
                },
                min: 55
            },
            width: windowWidth,
            height: 200,
            dataLabel: false,
            dataPointShape: true,
            extra: {
                lineStyle: 'curve'
            }
        });
    }
    
});
