// F1 2023 packet structures (based on official EA F1 2023 UDP telemetry spec)
// Key changes from 2022: Updated packet format identifiers, revised field offsets

#pragma once

#pragma pack(push)
#pragma pack(1)

struct PacketHeader2023 {
  uint16_t    m_packetFormat;            // 2023
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

// Lap data structure for F1 2023 (expanded from 2022)
struct LapData2023 {
  uint32_t    m_lastLapTimeInMS;         // Last lap time in milliseconds
  uint32_t    m_currentLapTimeInMS;      // Current lap time in milliseconds
  
  uint16_t    m_sectorTimesInMS[3];      // Sector times
  uint16_t    m_deltaToCarInFrontInMS;   // Delta to car in front
  uint16_t    m_deltaToRaceLeaderInMS;   // Delta to race leader
  
  float       m_lapDistance;             // Distance through lap in meters
  float       m_totalDistance;           // Total distance in session
  
  float       m_safetyCarDelta;          // Delta to safety car
  uint8_t     m_carPosition;             // Position in race
  uint8_t     m_currentLapNum;           // Current lap number
  uint8_t     m_penaltySeconds;          // Penalty time
  
  uint8_t     m_penaltyType;             // Type of penalty
  uint8_t     m_pitLaneTimerActive;      // Boolean
  uint16_t    m_pitLaneTimeInLaneInMS;   // Time in pit lane
  
  uint16_t    m_pitStopTimerInMS;        // Pit stop timer
  uint8_t     m_pitStopShouldServePenalty;
  
  uint8_t     m_drsAllowedOnThisLap;     // Boolean
  uint16_t    m_cumulativeLapDistanceInMetres;
};

struct PacketLapData2023 {
  PacketHeader2023  m_header;
  LapData2023       m_lapData[22];        // Data for all cars
};

// Session data for F1 2023 (expanded info)
struct SessionData2023 {
  uint8_t     m_weather;                 // 0=clear, 1=light cloud, 2=overcast, 
                                         // 3=light rain, 4=heavy rain, 5=storm
  int8_t      m_trackTemperature;        // Track temp in Celsius
  int8_t      m_airTemperature;          // Air temp in Celsius
  uint8_t     m_totalLaps;               // Total laps in session
  uint16_t    m_sessionDuration;         // Duration in minutes
  uint8_t     m_sessionType;             // 0=unknown, 1=P1, 2=P2, 3=P3...
  int8_t      m_trackId;                 // -1=unknown, 0=Albert Park... 
  uint8_t     m_sessionTimeLeft;         // Time remaining in seconds (capped at 255)
  uint16_t    m_sessionDurationRemaining; // Duration remaining in seconds
};

#pragma pack(pop)

#endif
