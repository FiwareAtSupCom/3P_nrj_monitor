
# Project Description

## Introduction:
The Three phases Energy analyse and monitor is project that aims to accurately measure and present the digital twin representation of power quantity and quality in three-phase systems. This system leverages the ADE9000 integrated circuit for precise measurements. The data is then transmitted via the ISP interface to an ESP8266, which communicates over Wi-Fi with a FIWARE context broker. The FIWARE context broker is responsible for data storage and communication with Grafana for visualization.

## Project Objectives:
L'objectif central de notre projet est de développer un système de smart metering qui permettra la mesure précise et complète des valeurs quantitatives et qualitatives associées aux systèmes de production et de consommation d'énergie électrique. Contrairement à la focalisation sur le ADE 9000, notre attention est portée sur la création d'un outil innovant et performant qui répondra aux besoins spécifiques liés à la surveillance et à l'optimisation des processus énergétiques.

## Systèmes monitorés:

Le système axé sur la mesure et la surveillance de l'énergie en trois phases, pourrait être utilisé pour surveiller divers systèmes de production et de consommation d'énergie électrique. Voici quelques exemples de systèmes que le système peut monitorer :


1. Réseau Électrique Local : Le système pourrait être utilisé pour surveiller la consommation d'énergie dans un bâtiment, permettant aux utilisateurs de prendre des décisions informées pour économiser de l'énergie.
Le système pourrait surveiller la consommation d'énergie pour l'éclairage, le chauffage, la climatisation, etc., afin d'optimiser l'efficacité énergétique.

2. Systèmes d'Énergie Renouvelable : Il pourrait être utilisé pour surveiller les systèmes de production d'énergie renouvelable tels que les parcs éoliens, les centrales solaires, etc.

# Architecture

![image](https://github.com/FiwareAtSupCom/3P_nrj_monitor/assets/93084127/8d696b53-4b43-4abd-801d-2c5109747c3d)

# Système embarqué

L'ESP32 effectue des mesures périodiques via des interruptions de timer. Les données sont transmises par Wi-Fi, et en cas d'interruption de la connexion, l'ESP32 stocke les informations dans une mémoire limitée de 512 octets. Elle surveille activement la connexion Wi-Fi, ajustant les timers pour minimiser le stockage local si la connexion est interrompue. Cette approche garantit une gestion efficace des données malgré les perturbations de la connectivité
![My Image](img/processes_sur_ESP-32.jpg)
![My Image](img/programme-principal.jpg)
![My Image](img/power_interrpt.jpg)
![My Image](img/Energy_interupt.jpg)
## Schema
...
## code
...
# NGSI / datamodeles 
Les données qu’on va utiliser sont :

• activeEnergyExport : Energie active exportée par phase depuis la date de début du comptage.

• activeEnergyImport : Énergie active importée consommée par phase depuis la date de début du comptage.

• activePower : Puissance active consommée par phase depuis la date de début du comptage.

• courant : 'Courant électrique.

• dateEnergyMeteringStarted : date de début du comptage de l'énergie.

• dateModified : Horodatage de la dernière modification de l'entité.

• phaseToPhaseVoltage : 'Tension entre phases.

• Facteur de puissance :

• refTargetDevice : appareil(s) pour lequel(s) la mesure a été effectuée


Les requétes  d’ectriture des systèmes IoT correspondant au NGSI sont trouvés dans le fichier  suivant : https://github.com/FiwareAtSupCom/3P_nrj_monitor/blob/main/REQUETE.txt 

la structure commune au sein de chaque entité de données doit être standardisée afin de favoriser la réutilisation.
Le modèle de données se trouve dans le fichier suivant : https://github.com/FiwareAtSupCom/3P_nrj_monitor/blob/main/data-model.txt

Le digital twin serait constamment mis à jour en temps réel à partir des données provenant de l’ESP. Il refléterait les fluctuations de la puissance consoméé. Pour permettre une interation facile, le digital twin aurait une interface utilisateur graphique qui fournirait des visualisations intuitives et des graphiques de performance.
# Base de données et registers
....
# Front End / Grafana ?
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




