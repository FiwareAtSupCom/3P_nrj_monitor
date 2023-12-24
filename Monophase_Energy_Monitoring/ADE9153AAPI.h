#ifndef ADE9153AAPI_h
#define ADE9153AAPI_h

#include "Arduino.h"
#include "ADE9153A.h"

/**************************************************************************
  Definitions
 **************************************************************************/
/* Configuration Registers */
#define ADE9153A_AI_PGAGAIN 0x000A		/*Signal on IAN, current channel gain=16x*/
#define ADE9153A_CONFIG0 0x00000000		/*Datapath settings at default*/
#define ADE9153A_CONFIG1 0x0300			/*Chip settings at default*/
#define ADE9153A_CONFIG2 0x0C00			/*High-pass filter corner, fc=0.625Hz*/
#define ADE9153A_CONFIG3 0x0000			/*Peak and overcurrent settings*/
#define ADE9153A_ACCMODE 0x0010			/*Energy accumulation modes, Bit 4, 0 for 50Hz, 1 for 60Hz*/

#define ADE9153A_VLEVEL 0x002C11E8		/*Assuming Vnom=1/2 of fullscale*/
#define ADE9153A_ZX_CFG 0x0000			/*ZX low-pass filter select*/
#define ADE9153A_MASK 0x00000100		/*Enable EGYRDY interrupt*/

#define ADE9153A_ACT_NL_LVL 0x000033C8
#define ADE9153A_REACT_NL_LVL 0x000033C8
#define ADE9153A_APP_NL_LVL 0x000033C8

/* Constant Definitions */
#define ADE9153A_RUN_ON 0x0001			/*DSP On*/
#define ADE9153A_COMPMODE 0x0005		/*Initialize for proper operation*/

#define ADE9153A_VDIV_RSMALL 0x03E8		/*Small resistor on board is 1kOhm=0x3E8*/

/* Energy Accumulation Settings */
#define ADE9153A_EP_CFG 0x0009			/*Energy accumulation configuration*/
#define ADE9153A_EGY_TIME 0x0F9F		/*Accumulate energy for 4000 samples*/

/* Temperature Sensor Settings */
#define ADE9153A_TEMP_CFG 0x000C		/*Temperature sensor configuration*/

/* Ideal Calibration Values for ADE9153A Shield Based on Sensor Values */
#define CAL_IRMS_CC		0.838190	// (uA/code)
#define CAL_VRMS_CC		13.41105	// (uV/code)	
#define CAL_POWER_CC 	1508.743	// (uW/code) Applicable for Active, reactive and apparent power
#define CAL_ENERGY_CC	0.858307	// (uWhr/xTHR_HI code)Applicable for Active, reactive and apparent energy

/**************************************************************************
  Structures and Global Variables
 **************************************************************************/

struct EnergyRegs {
	int32_t ActiveEnergyReg;
	int32_t FundReactiveEnergyReg;
	int32_t ApparentEnergyReg;
	float ActiveEnergyValue;
	float FundReactiveEnergyValue;
	float ApparentEnergyValue;
};

struct PowerRegs {
	int32_t ActivePowerReg;
	float ActivePowerValue;
	int32_t FundReactivePowerReg;
	float FundReactivePowerValue;
	int32_t ApparentPowerReg;
	float ApparentPowerValue;
};

struct RMSRegs {
	int32_t CurrentRMSReg;
	float CurrentRMSValue;
	int32_t VoltageRMSReg;
	float VoltageRMSValue;
};

struct HalfRMSRegs {
	int32_t HalfCurrentRMSReg;
	float HalfCurrentRMSValue;
	int32_t HalfVoltageRMSReg;
	float HalfVoltageRMSValue;
};

struct PQRegs {
	int32_t PowerFactorReg;
	float PowerFactorValue;
	int32_t PeriodReg;
	float FrequencyValue;
	int32_t AngleReg_AV_AI;
	float AngleValue_AV_AI;
};

struct AcalRegs {
	int32_t AcalAICCReg;
	float AICC;
	int32_t AcalAICERTReg;
	int32_t AcalAVCCReg;
	float AVCC;
	int32_t AcalAVCERTReg;
};

struct Temperature {
	uint16_t TemperatureReg;
	float TemperatureVal;
};


class ADE9153AClass
{
public:
	ADE9153AClass(void);
	void SetupADE9153A(void);

	/* SPI Functions */
	bool SPI_Init(uint32_t SPI_speed, uint8_t chipSelect_Pin);
	void SPI_Write_16(uint16_t Address, uint16_t Data );
	void SPI_Write_32(uint16_t Address, uint32_t Data );
	uint16_t SPI_Read_16(uint16_t Address);
	uint32_t SPI_Read_32(uint16_t Address);

	/* ADE9153A Calculated Paramter Read Functions */
	void ReadEnergyRegs(volatile EnergyRegs *Data);
	void ReadPowerRegs(volatile PowerRegs *Data);
	void ReadRMSRegs(volatile RMSRegs *Data);
	void ReadHalfRMSRegs(HalfRMSRegs *Data);
	void ReadPQRegs(volatile PQRegs *Data);

	void ReadAcalRegs(AcalRegs *Data);
	bool StartAcal_AINormal(void);
	bool StartAcal_AITurbo(void);
	bool StartAcal_AV(void);
	void StopAcal(void);
	bool ApplyAcal(float AICC, float AVCC);

	void ReadTemperature(Temperature *Data);

  /*Added Functions*/
    void InitActiveEnergy(volatile EnergyRegs* Energy);
    void AccumulateActiveEnergy(volatile EnergyRegs* Energy);
    void InitReactiveEnergy(volatile EnergyRegs* Energy);
    void AccumulateReactiveEnergy(volatile EnergyRegs* Energy);

private:
	uint8_t _chipSelect_Pin;
};
extern ADE9153AClass ade9153A;

#endif