// Assetto Corsa Competizione UDP telemetry structures
// All structs must be packed

#pragma once

#pragma pack(push)
#pragma pack(1)

// ACC Physics packet (most detailed)
struct ACCPhysicsPacket {
  uint32_t packetId;                // Packet identifier
  
  float speed;                      // Speed in m/s
  float throttle;                   // 0.0 to 1.0
  float brake;                      // 0.0 to 1.0
  float clutch;                     // 0.0 to 1.0
  
  float steerAngle;                 // -1.0 to +1.0
  
  float wheelSlip[4];               // RL, RR, FL, FR - slip ratio
  float wheelLoad[4];               // Normalized load 0-1
  float wheelsPressure[4];          // kPa
  float airTemp;                    // Celsius
  float roadTemp;                   // Celsius
  
  float enginerpm;                  // RPM
  float engHoursnorm;               // Hours (normalized to 0-2)
  
  float fuelRemaining;              // Liters
  float fuelConsumption;            // Liters per lap estimate
  
  float carsOnTrack;                // Number of cars
  
  float drsAllowed;                 // Boolean
  float drsActivated;               // Boolean
  
  float wheelAngularVelocity[4];    // rad/s
  
  float aBS;                        // Boolean
  
  float turboBoost;                 // Boost amount
  
  float airDensity;                 // kg/m³
  float airPressure;                // mbar
  float windSpeed;                  // m/s
  float windDirection;              // Degrees (0-360)
  
  int32_t ambientTemp;              // Celsius (int)
  float trackTemp;                  // Celsius
  
  // Additional data
  float brakeBias;                  // 0.0-1.0
  float localAngularVelocity[3];   // X, Y, Z rad/s
  
  // Tire compound
  int32_t tireCompound;             // 0=soft, 1=medium, 2=hard, etc
  
  uint32_t paused;                  // Boolean
  uint32_t headphoneVolume;         // 0-100
};

// ACC Graphics packet (lighter weight, race info)
struct ACCGraphicsPacket {
  uint32_t packetId;
  uint32_t packetType;              // 0=graphics, 1=physics, etc
  
  int32_t status;                   // 0=off, 1=replay, 2=live, 3=pause
  
  int32_t session;                  // Session type
  char currentTime[15];             // HH:MM:SS.mmm
  char lastTime[15];                // Last lap time
  char bestTime[15];                // Best lap time
  char split[15];                   // Split time
  
  uint32_t completePercentage;      // Lap progress %
  float position;                   // Car position number
  uint32_t carPosition;             // Position in race
  uint32_t splinePosition;          // Position on track (0-1)
  
  float kmh;                        // Speed
  
  int32_t sessionIndex;             // Current session index
  float sessionTimeLeft;            // Time remaining (s)
  float sessionTimeElapsed;         // Time elapsed (s)
  
  int32_t simulationPaused;         // Boolean
  int32_t penaltyTime;              // Penalty time remaining (s)
  
  int32_t penalties;                // Total penalties
  int32_t speedTrap;                // Speed trap speed
  
  uint32_t isLocalPlayer;           // Boolean
  
  // Car status
  int32_t carModel;                 // Car identifier
  int32_t numCars;                  // Total cars in session
  
  char playerName[100];             // Player name
  
  float speedMS;                    // Speed m/s
  
  uint32_t isRPlaying;              // Replay active
  float replaySessionTime;          // Replay time
  
  uint32_t rainLights;              // Boolean
  uint32_t fogLights;               // Boolean
  
  uint32_t wiper;                   // Wiper active
};

// ACC Static info packet (player data)
struct ACCStaticPacket {
  uint32_t packetId;
  
  char track[33];                   // Track name
  char playerName[100];             // Player name
  char playerSurname[100];          // Player surname
  
  int32_t playerNationality;        // Country code
  
  char carModel[33];                // Car model
  char skinName[33];                // Livery/skin name
  
  int32_t seatIndex;                // Seat position
  
  int32_t classID;                  // Class identifier
  
  int32_t driverCategory;           // Driver skill level
  
  int32_t mgUIDNumber;              // Multiplayer UID
  
  int32_t isAIControlled;           // Boolean
};

#pragma pack(pop)

#endif
