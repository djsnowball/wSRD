// Project Cars telemetry packet structures
// UDP telemetry format for Project Cars 1

#pragma once

#pragma pack(push)
#pragma pack(1)

// Project Cars 1 telemetry packet
struct ProjectCarsPacket {
  // Version info
  uint32_t m_packetType;              // 0 = telemetry, 1 = FFB
  uint16_t m_packetVersion;           // Version 1 or 2 for PC1
  
  // Car state
  float m_worldPositionX;             // World position X (meters)
  float m_worldPositionY;             // World position Y (meters)
  float m_worldPositionZ;             // World position Z (meters)
  
  float m_lastTimeTrialLapTimeMS;     // Last lap time in milliseconds
  float m_currentTimeTrialLapTimeMS;  // Current lap time in milliseconds
  
  // Performance
  float m_speed;                      // Speed in m/s
  float m_throttle;                   // Throttle 0.0-1.0
  float m_brakes;                     // Brake 0.0-1.0
  float m_clutch;                     // Clutch 0.0-1.0
  float m_steering;                   // Steering -1.0 to +1.0
  float m_gearFlags;                  // Gear number (1-6) or special bits
  
  // Engine
  float m_engineRate;                 // Engine RPM
  float m_drsAllowed;                 // DRS allowed (boolean as float)
  float m_ersDeployMode;              // ERS mode (0=off, 1=deploy, etc)
  
  // Tires
  float m_tyreTempFrontLeft;          // Tire temp front left (Celsius)
  float m_tyreTempFrontRight;         // Tire temp front right
  float m_tyreTempRearLeft;           // Tire temp rear left
  float m_tyreTempRearRight;          // Tire temp rear right
  
  float m_tyreWearFrontLeft;          // Tire wear 0.0-1.0
  float m_tyreWearFrontRight;
  float m_tyreWearRearLeft;
  float m_tyreWearRearRight;
  
  // Suspension & damage
  float m_suspensionDamageFL;         // Suspension damage 0.0-1.0
  float m_suspensionDamageFR;
  float m_suspensionDamageRL;
  float m_suspensionDamageRR;
  
  float m_engineDamage;               // Engine damage 0.0-1.0
  float m_aeroDamage;                 // Aero damage 0.0-1.0
  
  // Fuel
  float m_fuelAmount;                 // Current fuel (liters)
  float m_fuelCapacity;               // Fuel tank capacity
  
  // Session/Race info
  uint32_t m_raceState;               // 0=invalid, 1=practice, 2=qualify, 3=race, etc
  uint32_t m_lapNumber;               // Current lap
  uint32_t m_racePosition;            // Position in race
  uint32_t m_totalLaps;               // Total laps in race
  uint32_t m_viewedParticipantIndex;  // Participant index being viewed
  
  // Timing
  float m_eventTimeRemaining;         // Time remaining in session (seconds)
  float m_sessionTimeRemaining;       // Session time remaining
  
  // Session UID
  uint64_t m_sessionUID;              // Unique session identifier
  
  // Additional fields
  float m_maxInclineAngle;            // Car inclination angle
  
  // Air temperature
  float m_ambientTemperature;         // Ambient temp (Celsius)
  float m_trackTemperature;           // Track temp (Celsius)
  
  // Penalties
  uint8_t m_penalties;                // Total penalties
  uint8_t m_numCars;                  // Number of cars in session
  uint8_t m_playerCarPosition;        // Player car position
};

#pragma pack(pop)

#endif
