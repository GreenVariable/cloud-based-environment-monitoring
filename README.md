# cloud-based-environment-monitoring
This project utilizes ESP8266 NodeMCU board for cloud-based (Firebase) real-time monitoring of temperature &amp; light using sensors.

Description: 
This project integrates two NodeMCU 8266 boards to create a system for real-time monitoring of temperature and light levels. This solution utilizes Firebase Realtime Database for cloud-based data storage and retrieval. 
 
Working Principle: 
The project consists of two main parts: sensing and display. 
In the sensing part, a NodeMCU board is used to gather temperature and light data from respective sensors. The NodeMCU utilizes its Wi-Fi capabilities to establish a connection with Firebase Realtime Database. Through this connection, the data is uploaded to the cloud in real-time, ensuring continuous monitoring of environmental conditions. 
In the display part, another NodeMCU board receives the data from Firebase Realtime Database. A TFT ILI9341 display is connected to the board where the transmitted temperature and light data are visualized for user observation. 
 
Key Features: 
1. Real-time monitoring of temperature and light levels. 
2. Utilization of NodeMCU for sensor data collection and transmission. 
3. Integration with Firebase Realtime Database for cloud-based storage and retrieval. 
4. User-friendly display of sensor data on a TFT display for easy observation and analysis. 
5. Wireless connectivity allows for remote access to environmental data from any location with internet access. 
 
This project offers a user-friendly solution for monitoring temperature and light levels in real-time, providing insights into the surrounding environment. 

