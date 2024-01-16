/*#define PowertimerFactor  1000000 //1 sec
#define EnergytimerFactor 3600000000 //1 hour 
#define VCFtimerFactor  900000000 //15 minutes
#define THDtimerFactor  1800000000 //15 minutes
*/

#define PowertimerFactor  1000000 //1 sec
#define EnergytimerFactor 15000000 //1 hour 
#define VCFtimerFactor  5000000 //15 minutes
#define THDtimerFactor  5000000 //15 minutes

#define timerFactorChanger 3

#define AccumulatedPowertimerFactor   5000000 //100 seconds

#define PowerConversionConstant 3.6097 //(mWATT/mVAR/mVA)/LSB

#define EnergyConversionConstant 1.0268 //( WATTHr/ VARHr/ VAHr)/LSB

#define VoltageConversionConstant 13.4225

#define CurrentConversionConstant 2.0036 

#define Type "ACMeasurement"

#define phaseType "threePhase"

#define deviceID "urn:ngsi-v2:SolarEnergy"
#define deviceName "AirPort-NCE-T1-F01-TR05-ACTP"
#define alternateName "AirPort global Observation"
#define description "Measurement corresponding to the ventilation machine of the technical rooms Terminal 1 T1 Floor 01 Technical Room 05 for Triphase alternating current"
#define areaServed "Nice Aeroport"
#define refDevice "urn:ngsi-ld:Device:T1-F01-TR05-ACTP"


