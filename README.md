
# Project Description

## Introduction:
The Three phases Energy analyse and monitor is project that aims to accurately measure and present the digital twin representation of power quantity in three-phase systems. This system mirrors the physical three-phase power system, continuously updated with real-time. The measurements are transmitted via the ISP interface to an ESP8266, which communicates over Wi-Fi with a FIWARE context broker. The FIWARE context broker is responsible for data storage and communication with Grafana for visualization.

## Project Objectives:

Measure the power and the quantity of energy consumed in real time from the power supplier as well as from the photovoltaic panels.

Visualize the power and the amount of energy consumed in real time in the form of curves.

Application of machine learning for detection of consumption of each device to determine abnormal behavior.

## Project Steps:
a. ADE9000 Integration:

Integrate the ADE9000 integrated circuit into the system for high-precision measurements of voltage, current, power factor, and harmonics in three-phase electrical systems.

b. Data Acquisition and Transmission:

Use the ISP interface to transfer data from the ADE9000 to an ESP8266 module, which will act as a data gateway.

c. Data Processing:

Develop algorithms to process the data obtained from the ADE9000 and prepare it for transmission.

d. Digital Twin Creation:

Create a digital twin model that mirrors the physical three-phase power system, continuously updated with real-time.

e. IoT Communication:

Implement the ESP8266 to establish a Wi-Fi connection and transmit data to a FIWARE context broker.

f. Data Storage:

Utilize the FIWARE context broker to store the collected data securely and efficiently.

g. Visualization:

Communicate with Grafana for real-time and historical data visualization.

# Architecture

![image](https://github.com/FiwareAtSupCom/3P_nrj_monitor/assets/93084127/8d696b53-4b43-4abd-801d-2c5109747c3d)

# Functional diagram of the measurement system

![Diagramme sans nom drawio](https://github.com/FiwareAtSupCom/3P_nrj_monitor/assets/93084127/2a480adf-7056-47c0-aae9-7733829a45e9)

![Diagramme sans nom2 drawio](https://github.com/FiwareAtSupCom/3P_nrj_monitor/assets/93084127/1fe59228-57df-442a-9e5b-9e3132cff200)
