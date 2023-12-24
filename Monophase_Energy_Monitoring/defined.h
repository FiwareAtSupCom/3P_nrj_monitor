#define PowertimerFactor  1000000 //1 sec
#define AccumulatedPowertimerFactor   1000000//100000000 //100 seconds
#define EnergytimerFactor 10000000//3600000000 //1 hour 
#define VCFtimerFactor  1000000 //15 minutes
#define THDtimerFactor  5000000 //15 minutes

#define PowerConversionConstant 3.6097 //(mWATT/mVAR/mVA)/LSB

#define EnergyConversionConstant 1.0268 //( WATTHr/ VARHr/ VAHr)/LSB

#define VoltageConversionConstant 13.4225

#define CurrentConversionConstant 2.0036 

#define Type "ACMeasurement"

#define phaseType "singlePhase"

#define deviceID "urn:ngsi-v2:SolarEnergy"
#define deviceName "AirPort-NCE-T1-F01-TR05-ACTP"
#define alternateName "AirPort global Observation"
#define description "Measurement corresponding to the ventilation machine of the technical rooms Terminal 1 T1 Floor 01 Technical Room 05 for Triphase alternating current"
#define areaServed "Nice Aeroport"
#define refDevice "urn:ngsi-ld:Device:T1-F01-TR05-ACTP"


