// Project Cars 2 telemetry packet structures
// Enhanced UDP telemetry format for Project Cars 2

#pragma once

#pragma pack(push)
#pragma pack(1)

// Project Cars 2 Telemetry packet (enhanced from PC1)
struct ProjectCars2Packet {
  // Build/version info
  uint32_t m_packetType;              // 0 = telemetry, 1 = FFB
  uint16_t m_packetVersion;           // Version 2 for PC2
  
  // World position
  float m_worldPositionX;             // World position X (meters)
  float m_worldPositionY;             // World position Y (meters)
  float m_worldPositionZ;             // World position Z (meters)
  
  // Rotation (new in PC2)
  float m_worldRotationX;             // Rotation X (radians)
  float m_worldRotationY;             // Rotation Y (radians)
  float m_worldRotationZ;             // Rotation Z (radians)
  
  // Velocities
  float m_linearVelocityX;            // Linear velocity X
  float m_linearVelocityY;            // Linear velocity Y
  float m_linearVelocityZ;            // Linear velocity Z
  
  float m_angularVelocityX;           // Angular velocity X
  float m_angularVelocityY;           // Angular velocity Y
  float m_angularVelocityZ;           // Angular velocity Z
  
  // Acceleration (new in PC2)
  float m_linearAccelerationX;        // Linear accel X
  float m_linearAccelerationY;        // Linear accel Y
  float m_linearAccelerationZ;        // Linear accel Z
  
  float m_angularAccelerationX;       // Angular accel X
  float m_angularAccelerationY;       // Angular accel Y
  float m_angularAccelerationZ;       // Angular accel Z
  
  // Lap timing
  float m_lastTimeTrialLapTimeMS;     // Last lap time in milliseconds
  float m_currentTimeTrialLapTimeMS;  // Current lap time
  float m_currentSector1TimeMS;       // Sector 1 time (new in PC2)
  float m_currentSector2TimeMS;       // Sector 2 time (new in PC2)
  float m_currentSector3TimeMS;       // Sector 3 time (new in PC2)
  
  float m_fastestLapTimeMS;           // Best lap time
  float m_fastestSector1TimeMS;       // Best sector 1
  float m_fastestSector2TimeMS;       // Best sector 2
  float m_fastestSector3TimeMS;       // Best sector 3
  
  // Performance inputs
  float m_speed;                      // Speed in m/s
  float m_throttle;                   // Throttle 0.0-1.0
  float m_brakes;                     // Brake 0.0-1.0
  float m_clutch;                     // Clutch 0.0-1.0
  float m_steering;                   // Steering -1.0 to +1.0
  float m_handbrake;                  // Handbrake (new in PC2)
  
  // Engine
  uint32_t m_gearFlags;               // Gear number and flags
  float m_engineRate;                 // Engine RPM
  float m_engineTemperature;          // Engine temp (Celsius, new in PC2)
  float m_engineOilTemperature;       // Oil temp (new in PC2)
  float m_engineOilPressure;          // Oil pressure (new in PC2)
  
  // DRS/ERS (new in PC2)
  float m_drsAllowed;                 // DRS allowed (0.0 or 1.0)
  float m_ersDeployMode;              // ERS deployment mode
  float m_ersStoreEnergy;             // ERS energy stored (0-100%)
  float m_ersDeployedThisLap;         // Energy deployed this lap
  float m_ersHarvestedThisLap;        // Energy harvested this lap
  
  // Tires (expanded in PC2)
  float m_tyreTempFrontLeft;          // Tire temp front left (Celsius)
  float m_tyreTempFrontRight;         // Tire temp front right
  float m_tyreTempRearLeft;           // Tire temp rear left
  float m_tyreTempRearRight;          // Tire temp rear right
  
  float m_tyreWearFrontLeft;          // Tire wear 0.0-1.0
  float m_tyreWearFrontRight;
  float m_tyreWearRearLeft;
  float m_tyreWearRearRight;
  
  float m_tyrePressureFrontLeft;      // Tire pressure (kPa, new in PC2)
  float m_tyrePressureFrontRight;
  float m_tyrePressureRearLeft;
  float m_tyrePressureRearRight;
  
  float m_tyreCompound;               // Tire compound (0-5 or similar)
  float m_brakeTemperatureFrontLeft;  // Brake temp (new in PC2)
  float m_brakeTemperatureFrontRight;
  float m_brakeTemperatureRearLeft;
  float m_brakeTemperatureRearRight;
  
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
  float m_fuelConsumption;            // Fuel consumption per lap estimate
  
  // Session/Race info
  uint32_t m_raceState;               // 0=invalid, 1=practice, 2=qualify, 3=race
  uint32_t m_lapNumber;               // Current lap
  uint32_t m_racePosition;            // Position in race
  uint32_t m_totalLaps;               // Total laps in race
  uint32_t m_viewedParticipantIndex;  // Participant index
  
  // Timing
  float m_eventTimeRemaining;         // Time remaining (seconds)
  float m_sessionTimeRemaining;       // Session time remaining
  
  // Session UID
  uint64_t m_sessionUID;              // Unique session identifier
  
  // Environment (new in PC2)
  float m_ambientTemperature;         // Ambient temp (Celsius)
  float m_trackTemperature;           // Track temp (Celsius)
  float m_windSpeedX;                 // Wind speed X (new in PC2)
  float m_windSpeedY;                 // Wind speed Y (new in PC2)
  float m_windSpeedZ;                 // Wind speed Z (new in PC2)
  
  // Rain/weather
  float m_rainIntensity;              // Rain intensity (new in PC2)
  float m_wetnessFront;               // Wetness at front (new in PC2)
  float m_wetnessRear;                // Wetness at rear (new in PC2)
  float m_wetnessLeft;                // Wetness on left (new in PC2)
  float m_wetnessRight;               // Wetness on right (new in PC2)
  
  // Penalties (expanded in PC2)
  uint8_t m_penalties;                // Total penalties
  uint8_t m_numCars;                  // Number of cars
  uint8_t m_playerCarPosition;        // Player car position
  uint8_t m_drsState;                 // DRS state (0=off, 1=on, 2=ready, etc)
};

#pragma pack(pop)

#endif
