# Project Description

## Introduction:
The Three phases Energy analyse and monitor is project that aims to accurately measure and present the digital twin representation of power quantity and quality in three-phase systems. This system leverages the ADE9000 integrated circuit for precise measurements. The data is then transmitted via the ISP interface to an ESP8266, which communicates over Wi-Fi with a FIWARE context broker. The FIWARE context broker is responsible for data storage and communication with Grafana for visualization.

## Project Objectives:
The central objective of our project is to develop a smart metering system that enables precise and comprehensive measurement of quantitative and qualitative values associated with electric energy production and consumption systems. Unlike the focus on the ADE 9000, our attention is directed towards creating an innovative and high-performance tool that will meet specific needs related to monitoring and optimizing energy processes.


## Systèmes monitorés:
The three-phase energy measurement and monitoring system could be utilized to monitor various electric energy production and consumption systems. Here are some examples of systems that the system can monitor:

1. Local Electrical Grid: The system could be used to monitor energy consumption in a building, enabling users to make informed decisions to save energy. It could monitor energy consumption for lighting, heating, air conditioning, etc., to optimize energy efficiency.

2. Renewable Energy Systems: It could be employed to monitor renewable energy production systems such as wind farms, solar power plants, etc.

# Architecture
### Functional & description
![image](https://github.com/FiwareAtSupCom/3P_nrj_monitor/assets/93084127/8d696b53-4b43-4abd-801d-2c5109747c3d)
1. ADE9000 Electrical Parameter Acquisition:
The ADE9000 meter captures electrical parameters and stores them in its registers.

2.  ESP32 Data Retrieval via SPI Interface:
The ESP32 reads data from the ADE9000 using the SPI interface.

3.  Data Transmission using Wi-Fi and HTTP Requests:
The ESP32 transmits data through HTTP requests over Wi-Fi, facilitating seamless communication.

4.   Orchestrating Data with Context Broker:
The Context Broker, acting as the data orchestrator, receives data from ESP32 and stores it in MongoDB for digital twin creation.

5.  Time Series Database with Quantum Leap and CrateDB:
Quantum Leap ensures data continuity by transferring information from MongoDB to CrateDB, establishing a robust time series database.

6.  Visualization with Grafana:
Grafana, interfacing with CrateDB, provides an intuitive and real-time visualization platform for effective analysis and monitoring of the energy system.
### Information system & description
![MyImage](img/image.png)
1. Data Flow Overview for IoT Energy Monitoring System:
In our project, the data flow initiation involves the IoT device sending NGSI V2 requests to the Orion Context Broker. Subsequently, the Context Broker stores this data in MongoDB, establishing a robust foundation for the creation of digital twins, virtual instances that faithfully mirror the real-time states of corresponding physical systems.

2. Quantum Leap to CrateDB Integration:
Simultaneously, Orion Context Broker extends its data recording to CrateDB through Quantum Leap. This process ensures the backup of information in a highly performant database, ensuring data availability for in-depth analyses.

3. Grafana Visualization via CrateDB:
To intuitively visualize this data, we employ Grafana, which connects to CrateDB to extract the necessary information. This integration facilitates real-time visualization and provides powerful tools for the analysis and comprehension of the performance of monitored energy systems. Thus, our comprehensive architecture ensures efficient data collection, storage, and intelligent visualization, contributing to optimal management of energy processes.
# Système embarqué

L'ESP32 effectue des mesures périodiques via des interruptions de timer. Les données sont transmises par Wi-Fi, et en cas d'interruption de la connexion, l'ESP32 stocke les informations dans une mémoire limitée de 512 octets. Elle surveille activement la connexion Wi-Fi, ajustant les timers pour minimiser le stockage local si la connexion est interrompue. Cette approche garantit une gestion efficace des données malgré les perturbations de la connectivité
![MyImage](img/processes_sur_ESP-32.jpg width="40" height="400")
![MyImage](img/power_interrpt.jpg width="40" height="400")
![MyImage](img/Energy_interupt.jpg width="40" height="400")
## Schema
...
## code
...

# Prerequisites
## Docker and Docker Compose

For simplicity, all components will be executed using [Docker](https://www.docker.com), a container technology that facilitates the isolation of different components within their respective environments.
 - To set up Docker on Windows, refer to the provided instructions [here](https://docs.docker.com/docker-for-windows/).
 - For Mac users, the installation steps for Docker can be found[here](https://docs.docker.com/docker-for-mac/).
 - Linux users can install Docker by following the instructions available [here](https://docs.docker.com/install/).
 Docker Compose, a tool designed for defining and running multi-container Docker applications, utilizes [YAML files](https://raw.githubusercontent.com/Fiware/tutorials.Time-Series-Data/master/docker-compose.yml)files to configure the necessary services for the application. This approach allows for the simultaneous launch of all container services with a single command. While Docker Compose comes pre-installed with Docker for Windows and Docker for Mac, Linux users may need to refer to the instructions provided [here](https://docs.docker.com/compose/install/) for installation.
 You can check your current **Docker** and **Docker Compose** versions using the following commands:


```console
docker-compose -v
docker version
```
It is required to have a version of Docker Compose equal to or greater than 3 for this project.
# Start Up
1. ### Wi-Fi Network Setup:
Ensure your Wi-Fi network is properly configured for connectivity between devices. This is crucial for seamless data transmission.

2. ### Context Broker (Orion):
The FIWARE Orion Context Broker serves as the central hub for managing context data in our IoT system. It receives requests using the NGSI-v2 standard and handles crucial aspects such as data entities, subscriptions, and registrations.
### Configuration:
```yaml
orion:
    image: quay.io/fiware/orion:latest
    hostname: orion
    container_name: orion
    depends_on:
        - mongo-db
    networks:
        - default
    expose:
        - '1026'
    ports:
        - '1026:1026'
    command: -dbhost mongo-db -logLevel DEBUG
```
3. ### MongoDB
MongoDB is employed as the backend for the Orion Context Broker. It plays a pivotal role in storing context data information, managing data entities, and facilitating various operations related to context management.
### configuration:
```yaml
mongo-db:
    image: mongo:4.2
    hostname: mongo-db
    container_name: db-mongo
    expose:
        - '27017'
    ports:
        - '27017:27017'
    networks:
        - default
```
4. ### QuantumLeap Configuration
FIWARE QuantumLeap is a powerful generic enabler designed to simplify the storage and retrieval of time-series data generated by the Orion Context Broker. Unlike the STH-Comet generic enabler, QuantumLeap seamlessly integrates with time-series databases, offering compatibility with databases like CrateDB and TimescaleDB.
### configuration:
```yaml
quantumleap:
    image: smartsdk/quantumleap
    hostname: quantumleap
    ports:
        - '8668:8668'
    depends_on:
        - crate-db
    environment:
        - CRATE_HOST=crate-db
    networks:
      - fiware_network
```
The quantumleap container is listening on one port:

The Operations for port for QuantumLeap - 8668 is where the service will be listening for notifications from the Orion context broker and where users can query data from.
The CRATE_HOST environment variable defines the location where the data will be persisted.

5. ###  Connecting FIWARE to CrateDB via QuantumLeap

In the configuration, QuantumLeap listens to NGSI v2 notifications on port 8668 and persists historic context data to CrateDB. CrateDB is accessible using port 4200 and can either be queried directly or attached to the Grafana analytics tool.

6. ### CrateDB Database Server Configuration

CrateDB serves as a dedicated data sink for our IoT system, with a specific focus on handling time-based historical context data. As a distributed SQL database system tailored for IoT applications, CrateDB excels in ingesting a high volume of data points per second and supports real-time querying. Its capabilities make it well-suited for handling complex queries involving geospatial and time-series data.
#### Analyzing time series data
Analyzing time series data is crucial for effectively monitoring the three-phase electricity system. The suitability of time series data analysis hinges on the specific requirements of your project and the reliability of the collected measurements. This analytical approach enables you to address key questions related to the electricity system, such as determining the maximum, average, and cumulative measurements of the devices over specific time intervals. Additionally, time series analysis can be applied to mitigate the impact of outliers by employing smoothing techniques, thereby enhancing the overall robustness of the monitoring process.

To set up the CrateDB Database Server, use the following configuration in your docker-compose.yml file:

```yaml
crate-db:
    image: crate:4.1.4
    hostname: crate-db
    ports:
        - '4200:4200'
        - '4300:4300'
    command:
        crate -Clicense.enterprise=false -Cauth.host_based.enabled=false  -Ccluster.name=democluster
        -Chttp.cors.enabled=true -Chttp.cors.allow-origin="*"
    environment:
        - CRATE_HEAP_SIZE=2g
```
If CrateDB exits immediately with a max virtual memory areas vm.max_map_count [65530] is too low, increase to at least [262144] error, this can be fixed by running the sudo sysctl -w vm.max_map_count=262144 command on the host machine. 

The cratedb container is listening on two ports:

The Admin UI is available on port 4200
The transport protocol is available on port 4300 


After configuring the docker-compose file, execute it using the following command:
```console 
docker-compose up -d
```

# NGSI / datamodeles 
The Data Model intended to measure the electrical energies consumed by an electrical system which uses an Alternating Current (AC) for a three-phase (L1, L2, L3) or single-phase (L) and neutral (N). It integrates the initial version of the data Modem [THREEPHASEMEASUREMENT], extended to also perform single-phase measurements. It includes attributes for various electrical measurements 

The data we will use is:


• activeEnergyImport: Active energy imported i.e. consumed per phase. The unit code (text) is given using the [UN/CEFACT Common Codes](http://wiki.goodrelations-vocabulary.org/Documentation/UN/CEFACT_Common_Codes). Units:'kilowatt hour'

• activePower: Active power consumed per phase. The unit code (text) is given using the [UN/CEFACT Common Codes](http://wiki.goodrelations-vocabulary.org/Documentation/UN/CEFACT_Common_Codes). Units:'Watt'

• current: Electrical current. The unit code (text) is given using the [UN/CEFACT Common Codes](http://wiki.goodrelations-vocabulary.org/Documentation/UN/CEFACT_Common_Codes). Units:'Ampere'

• dateEnergyMeteringStarted: start date of energy metering.

• dateModified: Timestamp of the last modification of the entity.


• Power factor :  It is the ratio of real power (the power that actually performs the work) to apparent power (the total power in the circuit).


The requests for writing IoT systems corresponding to the NGSI are found in the following files: 

https://github.com/FiwareAtSupCom/3P_nrj_monitor/blob/main/NGSI%20requests/3%20phases%20request.txt 

https://github.com/FiwareAtSupCom/3P_nrj_monitor/blob/main/NGSI%20requests/solar%20request.txt

the common structure within each data entity must be standardized to promote reuse.
The data model for threephase measurement can be found in the following file: https://github.com/FiwareAtSupCom/3P_nrj_monitor/blob/main/data%20model/3%20phases%20data%20model.json extracted from: https://github.com/smart-data-models/dataModel.Energy/blob/master/ACMeasurement/schema.json

The data model for solar energy can be found in the following file:https://github.com/FiwareAtSupCom/3P_nrj_monitor/blob/main/data%20model/solar%20data%20model.json extracted from: (https://github.com/smart-data-models/dataModel.Energy/blob/master/SolarEnergy/schema.json)

The digital twin would be constantly updated in real time using data from the ESP. It would reflect fluctuations in power consumed. To enable easy interaction, the digital twin would have a graphical user interface that would provide intuitive visualizations and performance charts.

## Front End / Grafana ?
The choice of technology to use:
![image1](https://github.com/FiwareAtSupCom/3P_nrj_monitor/assets/93084127/58f27566-6620-4446-9689-2a541a62fe44)

• Ease of Use and Flexibility: Grafana provides a user-friendly interface for creating visualizations and dashboards without extensive coding. Its flexibility allows easy integration with various data sources, including CrateDB and Fiware solutions, making it a versatile choice.

• Real-Time Data Visualization: Grafana supports real-time data visualization, allowing you to create live dashboards that update dynamically as new data comes in. This is crucial for monitoring grid systems that require up-to-date information.

• Large Variety of Visualization Options: Grafana offers a wide array of visualization options. This variety enables you to choose the most suitable visualization type for different types of grid system data.

• Alerting and Monitoring Capabilities: Grafana provides alerting functionalities that can be set up based on the data from CrateDB and Fiware, allowing for proactive monitoring of the grid systems. This ensures timely responses to any anomalies or critical situations.

• Community Support and Integration: Grafana has a large community of users and developers, resulting in extensive documentation, plugins, and support. It integrates well with various data sources, making it easier to combine data from CrateDB and Fiware in a unified dashboard.

• Using a native frontend or other solutions might be more time-consuming and may lack the robustness and features that Grafana offers specifically for data visualization and monitoring. 
![image2](https://github.com/FiwareAtSupCom/3P_nrj_monitor/assets/93084127/11e5ede7-3e0e-46d0-bb30-d42cb9653795)

Grafana, in this context, appears to be a strong choice due to its ease of use, real-time capabilities, extensive visualization options, and community support, allowing you to focus more on the representation and analysis of the data rather than the complexities of building a visualization platform from scratch.

• Navigate to http://192.168.33.69:3000 and log in with default credentials which are: admin / admin

• Add a new data source using the following parameters:
![image](https://github.com/FiwareAtSupCom/3P_nrj_monitor/assets/93084127/05353534-45b8-4cff-b463-c2d188ed272d)

• Import the dashboard located in: grafana_dashboard folder:
![image](https://github.com/FiwareAtSupCom/3P_nrj_monitor/assets/93084127/bf60c68b-105e-4d09-82c8-1ff6f713de04)


• The final result should look like this:
![image](https://github.com/FiwareAtSupCom/3P_nrj_monitor/assets/93084127/bd22a2da-d7a3-49b0-a1c3-fc01284fa0b2)
