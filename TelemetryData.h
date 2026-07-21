// Unified telemetry data structure for multi-game support
// Normalizes telemetry from F1 2022-2024, ACC, and other racing sims

#pragma once

// Game identifiers
enum GameType {
  GAME_F1_2022 = 2022,
  GAME_F1_2023 = 2023,
  GAME_F1_2024 = 2024,
  GAME_ACC = 100,
  GAME_UNKNOWN = 0
};

// Normalized vehicle telemetry
struct UnifiedTelemetry {
  // Session info
  uint64_t sessionUID;
  uint32_t frameIdentifier;
  float sessionTime;
  
  // Player info
  uint8_t playerCarIndex;
  uint8_t totalCars;
  
  // Lap data
  uint8_t carPosition;
  uint32_t currentLapNum;
  float lastLapTimeMS;
  float currentLapTimeMS;
  float totalDistance;
  
  // Performance
  float speed;
  float throttle;
  float brake;
  float steering;
  uint8_t gear;
  uint8_t engineRPM;
  uint16_t topSpeed;
  
  // Pit info
  bool pitLaneTimerActive;
  uint32_t pitLaneTimeMS;
  
  // DRS (F1 only)
  bool drsAllowed;
  bool drsActivated;
  
  // Fuel (if available)
  float fuelRemaining;
  float fuelCapacity;
  
  // Tire temps and wear (if available)
  float tireTemp[4];  // RL, RR, FL, FR
  uint8_t tireWear[4];
  uint8_t tireCompound;  // 0=soft, 1=medium, 2=hard, 3=intermediate, 4=wet
  
  // Damage (if available)
  uint8_t engineDamage;
  uint8_t gearboxDamage;
};

// Game detection info
struct GameDetection {
  GameType gameType;
  uint8_t gameMajorVersion;
  uint8_t gameMinorVersion;
  uint16_t packetFormat;
};

#endif
