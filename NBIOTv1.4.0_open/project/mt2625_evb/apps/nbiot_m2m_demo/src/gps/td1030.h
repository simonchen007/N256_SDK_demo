#ifndef __TD1030_H__
#define __TD1030_H__

typedef struct {
	char DatumCode[4];
	char DatumCodeFlag;
	char LatitudinalOff[4];
	char NorthSouth;
	char LongitudeOff[4];
	char EastWest;
	char HeightOff[4];
	char ReferenceCode[4];
}TD1030_DTM;

typedef struct {
	char UTCTime[10];
	char LatitudinalErr[4];
	char LongitudeErr[4];
	char HeightOff[4];
	char FaultMark[3];
	char FaultMarkPro[4];
	char DistanceEstimation[4];
	char PartialValue[4];
	char GnssSysFlag;
	char GnssSigFlag;
}TD1030_GBS;

typedef struct {
	char UTCTime[10];
	char Latitudinal[25];
	char NorthSouth;
	char Longitude[25];
	char EastWest;
	char Model;
	char SatelliteNum[3];
	char LevelFactor[6];
	char HighEarth[8];
	char M1;
	char GeoidalSurface[8];
	char M2;
	char DataAge[5];
	char IdentNumber[6];
}TD1030_GGA;

typedef struct {
	char Latitudinal[25];
	char NorthSouth;
	char Longitude[25];
	char EastWest;
	char UTCTime[10];
	char Status;	
	char Model;
}TD1030_GLL;

typedef struct {
	char UTCTime[20];
	char Latitudinal[25];
	char NorthSouth;
	char Longitude[25];
	char EastWest;
	char Model[8];
	char SatelliteNum[10];
	char LevelFactor[10];
	char HighEarth[20];
	char GeoidalSurface[20];
	char DataAge[10];
	char IdentNumber[20];
	char IsPass;
}TD1030_GNS;

typedef struct {
	char UTCTime[20];
	char State;
	char Latitudinal[25];
	char NorthSouth;
	char Longitude[25];
	char EastWest;
	char GroundSpeed[20];
	char GroundGuide[20];
	char DataTime[20];
	char MagneticDeflection[15];
	char MEastWest;
	char mode;
	char NavigationState;
	char IsPass;
}TD1030_RMC;

typedef struct {
	char TotalNum;
	char SerialNum;
	int  TotalSatellite;
	char SatelliteNum;
	int  Angle;
	char Azimuth;
	char CN;
	char IsPass;
}TD1030_GSV;
#endif