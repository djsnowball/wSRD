// F1 2024 packet structures (based on official EA F1 2024 UDP telemetry spec)
// Further expanded from 2023 with additional telemetry fields

#pragma once

#pragma pack(push)
#pragma pack(1)

struct PacketHeader2024 {
  uint16_t    m_packetFormat;            // 2024
  uint8_t     m_gameMajorVersion;        // Game major version
  uint8_t     m_gameMinorVersion;        // Game minor version
  uint8_t     m_packetVersion;           // Version of packet type
  uint8_t     m_packetId;                // Packet identifier
  uint64_t    m_sessionUID;              // Unique session identifier
  float       m_sessionTime;             // Session timestamp
  uint32_t    m_frameIdentifier;         // Frame identifier
  uint8_t     m_playerCarIndex;          // Player car index
  uint8_t     m_secondaryPlayerCarIndex; // Secondary player index (splitscreen)
};

// Enhanced lap data for F1 2024
struct LapData2024 {
  uint32_t    m_lastLapTimeInMS;         // Last lap time in milliseconds
  uint32_t    m_currentLapTimeInMS;      // Current lap time in milliseconds
  
  uint16_t    m_sectorTimesInMS[3];      // Sector times
  uint16_t    m_deltaToCarInFrontInMS;   // Delta to car in front
  uint16_t    m_deltaToRaceLeaderInMS;   // Delta to race leader
  
  float       m_lapDistance;             // Distance through lap
  float       m_totalDistance;           // Total distance
  
  float       m_safetyCarDelta;          // Delta to safety car
  uint8_t     m_carPosition;             // Race position
  uint8_t     m_currentLapNum;           // Current lap
  uint8_t     m_penaltySeconds;          // Penalty time
  
  uint8_t     m_penaltyType;             // Type of penalty
  uint8_t     m_pitLaneTimerActive;      // Boolean
  uint16_t    m_pitLaneTimeInLaneInMS;   // Pit lane time
  
  uint16_t    m_pitStopTimerInMS;        // Pit stop timer
  uint8_t     m_pitStopShouldServePenalty; // Boolean
  
  uint8_t     m_drsAllowedOnThisLap;     // Boolean
  uint8_t     m_drsActivationsRemaining; // DRS uses remaining (2024 addition)
  
  uint16_t    m_cumulativeLapDistanceInMetres;
  
  uint8_t     m_estimatedLapTimeRemaining; // Estimated time to complete lap (seconds)
  uint8_t     m_racingLine;              // 0=no racing line, 1=racing line active
  uint8_t     m_racingLineType;          // 0=2D, 1=3D
};

struct PacketLapData2024 {
  PacketHeader2024  m_header;
  LapData2024       m_lapData[22];       // All car data
};

// Car telemetry for F1 2024 (per-car performance data)
struct CarTelemetryData2024 {
  uint16_t    m_speed;                   // Speed in kph
  float       m_throttle;                // 0.0 to 1.0
  float       m_steer;                   // -1.0 to +1.0
  float       m_brake;                   // 0.0 to 1.0
  uint8_t     m_clutch;                  // 0 to 100
  int8_t      m_gear;                    // -1=reverse, 0=neutral, 1+=forward
  uint16_t    m_engineRPM;               // RPM
  uint8_t     m_drs;                     // Boolean
  
  uint8_t     m_revLightsPercent;        // Rev limiter percentage
  uint16_t    m_revLightsBitValue;       // Rev lights as bitmask
  
  uint16_t    m_brakesTemperature[4];    // Celsius (RL, RR, FL, FR)
  uint8_t     m_tyresSurfaceTemperature[4]; // Celsius
  uint8_t     m_tyresInnerTemperature[4];   // Celsius
  
  uint16_t    m_engineTemperature;       // Celsius
  float       m_tyresPressure[4];        // kPa
  uint8_t     m_surfaceType[4];          // 0=tarmac, 1=rumble strip, 2=concrete, 3=rock, 
                                         // 4=dirt, 5=grass, 6=gravel, 7=mud
};

struct PacketCarTelemetryData2024 {
  PacketHeader2024       m_header;
  CarTelemetryData2024   m_carTelemetryData[22];
  uint8_t                m_mfdPanelIndex;
  uint8_t                m_mfdPanelIndexSecondaryPlayer;
  int8_t                 m_suggestedGear;
};

#pragma pack(pop)

#endif
