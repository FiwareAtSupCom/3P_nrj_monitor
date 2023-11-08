
# Project Description

## Introduction:
The Three phases Energy analyse and monitor is project that aims to accurately measure and present the digital twin representation of power quantity and quality in three-phase systems. This system leverages the ADE9000 integrated circuit for precise measurements. The data is then transmitted via the ISP interface to an ESP8266, which communicates over Wi-Fi with a FIWARE context broker. The FIWARE context broker is responsible for data storage and communication with Grafana for visualization.

## Project Objectives:
a. ADE9000 Integration:

Integrate the ADE9000 integrated circuit into the system for high-precision measurements of voltage, current, power factor, and harmonics in three-phase electrical systems.

b. Data Acquisition and Transmission:

Use the ISP interface to transfer data from the ADE9000 to an ESP8266 module, which will act as a data gateway.

c. Data Processing:

Develop algorithms to process the data obtained from the ADE9000 and prepare it for transmission.

d. Digital Twin Creation:

Create a digital twin model that mirrors the physical three-phase power system, continuously updated with real-time data from the ADE9000.

e. IoT Communication:

Implement the ESP8266 to establish a Wi-Fi connection and transmit data to a FIWARE context broker.

f. Data Storage:

Utilize the FIWARE context broker to store the collected data securely and efficiently.

g. Visualization:

Communicate with Grafana for real-time and historical data visualization.


# Architecture

![image](https://github.com/FiwareAtSupCom/3P_nrj_monitor/assets/93084127/8d696b53-4b43-4abd-801d-2c5109747c3d)

# Système embarqué
++
## Schema
...
## code
...
# NGSI / datamodeles 
....
# Base de données et registers
....
# Front End / Grafana ?
....


