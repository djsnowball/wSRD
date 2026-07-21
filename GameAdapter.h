// Game adapter layer - converts game-specific telemetry to unified format
// Handles detection and conversion for F1 2022, F1 2023, F1 2024, and ACC

#pragma once
#include "TelemetryData.h"

class GameAdapter {
public:
  static GameDetection DetectGame(const uint8_t* buffer, uint16_t bufferSize) {
    GameDetection detection;
    detection.gameType = GAME_UNKNOWN;
    
    if (bufferSize < 5) return detection;
    
    // F1 games have packetFormat at offset 0-1
    uint16_t format = *((uint16_t*)&buffer[0]);
    
    if (format == 2022) {
      detection.packetFormat = 2022;
      detection.gameMajorVersion = buffer[2];
      detection.gameMinorVersion = buffer[3];
      detection.gameType = GAME_F1_2022;
      return detection;
    }
    
    if (format == 2023) {
      detection.packetFormat = 2023;
      detection.gameMajorVersion = buffer[2];
      detection.gameMinorVersion = buffer[3];
      detection.gameType = GAME_F1_2023;
      return detection;
    }
    
    if (format == 2024) {
      detection.packetFormat = 2024;
      detection.gameMajorVersion = buffer[2];
      detection.gameMinorVersion = buffer[3];
      detection.gameType = GAME_F1_2024;
      return detection;
    }
    
    // ACC detection - check for known ACC packet signatures
    // ACC packets are identified by their structure and port (default 9996)
    if (bufferSize >= 100) {
      // ACC physics packet is larger, check for consistency markers
      uint32_t packetId = *((uint32_t*)&buffer[0]);
      if (packetId > 0 && packetId < 1000000) {  // Heuristic for ACC packetId
        detection.gameType = GAME_ACC;
        detection.packetFormat = 100;
        return detection;
      }
    }
    
    return detection;
  }
  
  static bool ConvertF1ToUnified(const uint8_t* buffer, UnifiedTelemetry& output, 
                                 GameType gameType, uint8_t playerCarIndex) {
    if (gameType == GAME_F1_2022) {
      return ConvertF12022ToUnified(buffer, output, playerCarIndex);
    } else if (gameType == GAME_F1_2023) {
      return ConvertF12023ToUnified(buffer, output, playerCarIndex);
    } else if (gameType == GAME_F1_2024) {
      return ConvertF12024ToUnified(buffer, output, playerCarIndex);
    }
    return false;
  }
  
  static bool ConvertACCToUnified(const uint8_t* buffer, UnifiedTelemetry& output) {
    // ACC sends multiple packet types - this handles physics packets
    // Additional handling needed for graphics/static packets
    
    const ACCPhysicsPacket* physics = (const ACCPhysicsPacket*)buffer;
    
    output.speed = physics->speed * 3.6f;  // m/s to km/h
    output.throttle = physics->throttle;
    output.brake = physics->brake;
    output.steering = physics->steerAngle;
    output.engineRPM = physics->enginerpm;
    
    output.fuelRemaining = physics->fuelRemaining;
    output.fuelCapacity = output.fuelRemaining + physics->fuelConsumption;
    
    output.drsAllowed = physics->drsAllowed > 0.5f;
    output.drsActivated = physics->drsActivated > 0.5f;
    
    // Tires
    output.tireTemp[0] = physics->airTemp;  // Placeholder - ACC sends road temp
    output.tireTemp[1] = physics->airTemp;
    output.tireTemp[2] = physics->airTemp;
    output.tireTemp[3] = physics->airTemp;
    
    output.tireCompound = physics->tireCompound;
    
    return true;
  }

private:
  static bool ConvertF12022ToUnified(const uint8_t* buffer, UnifiedTelemetry& output, 
                                     uint8_t playerCarIndex);
  
  static bool ConvertF12023ToUnified(const uint8_t* buffer, UnifiedTelemetry& output, 
                                     uint8_t playerCarIndex);
  
  static bool ConvertF12024ToUnified(const uint8_t* buffer, UnifiedTelemetry& output, 
                                     uint8_t playerCarIndex);
};

// Inline implementations for F1 conversion (uses existing packet structures)
inline bool GameAdapter::ConvertF12022ToUnified(const uint8_t* buffer, 
                                                UnifiedTelemetry& output, 
                                                uint8_t playerCarIndex) {
  // This will be implemented using the existing F1Structs2022.h
  // For now, return true to indicate it's a valid F1 2022 packet
  // The main sketch will continue using the existing conversion logic
  return true;
}

inline bool GameAdapter::ConvertF12023ToUnified(const uint8_t* buffer, 
                                                UnifiedTelemetry& output, 
                                                uint8_t playerCarIndex) {
  // F1 2023 has slightly different packet format
  // Will be updated once F1 2023 structures are added
  return true;
}

inline bool GameAdapter::ConvertF12024ToUnified(const uint8_t* buffer, 
                                                UnifiedTelemetry& output, 
                                                uint8_t playerCarIndex) {
  // F1 2024 packet format
  // Will be updated once F1 2024 structures are added
  return true;
}

#endif
