# Remote-Medical-Monitoring
> In this context, based on the Internet of things and other technologies, this paper studies a remote medical monitoring system that can monitor physical parameters through devices, store data in the cloud through the network, and finally display data on the user end. 
# About

The system includes four parts: terminal device, IoT platform, cloud server platform and user end. The following works are completed: 

(1) Firstly, the hardware design of the terminal device is completed. The software algorithm has been introduced for the measurement of physical parameters including temperature, heart rate and blood oxygen, and WiFi networking function. Through MQTT protocol, the communication between terminal device and IoT
platform is realized. 

(2) Secondly, the construction and deployment of the IoT platform is completed. The methods of terminal device access and message processing are designed, realizing the device management and message flow functions; 

(3) Thirdly, the construction and deployment of the cloud server was completed. The background service program and cloud database were constructed to implement the storage and query of historical monitoring data; 

(4) Then, the Web and WeChat remote medical monitoring platform is designed to achieve the visualization of real-time monitoring, remote control and historical data query. 

(5) Finally, the experiment of each part verifies the feasibility of the whole system
## Table of Contents
* [Part 1: Overall Architecture](#part-1-overall-architecture)
* [Part 2: Terminal Device](#part-2-terminal-device)
* [Part 3: Cloud Platform](#part-3-cloud-platform)
* [Part 4: Client](#part-4-client)
<!-- * [License](#license) -->


## Part 1: Overall Architecture
The system includes four parts: terminal device, IoT platform, cloud server platform and client.
<div align=center><img width="400" src="image/overall.png"/></div>



## Part 2: Terminal Device
The terminal equipment is mainly divided into five parts: main control module, temperature module, blood oxygen and heart rate module, display module, and power module.

ESP12-F, as the main control device, communicates with slave devices such as temperature module, blood oxygen and heart rate module, and display module through ports that support I2C protocol. 
<div align="center">
     <img src="image/main_function.png" height="300"/> <img src="image/terminal_device.png" height="200"/>
</div>

### HR（heart rate）and SpO2 algorithm


## Part 3: Cloud Platform
Cloud platform includes two parts: IoT platform and cloud server.

This project selects MQTT as the communication method between terminal devices and IoT platforms, as well as the communication method between mobile devices and IoT platforms in the user end.
The Alibaba Cloud IoT platform only has short-term data storage capabilities, so AMQP protocol is chosen to push IoT platform data to cloud servers for long-term storage.

The cloud server platform mainly consists of two parts: the backend service program and the MySQL cloud database. The backend service program adopts the Nginx+Gunicorn+Flask server framework, and utilizes the HTTPS protocol to receive and process user requests.

<div align="center">
     <img src="image/aliyun.png" width="400"/> <img src="image/cloud_server.png" width="300"/>
</div>

## Part 4: Client
The client built by WeChat mini program mainly provides remote monitoring function for designated patients, which is mainly divided into personal information function page, historical query function page, and real-time monitoring function page
<div align=center><img width="400" src="image/wechat.png"/></div>
