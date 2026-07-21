// Game adapter layer - converts game-specific telemetry to unified format
// Handles detection and conversion for F1 2022-2024, ACC, Project Cars 1&2

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
    
    // Project Cars detection - based on packet size and structure
    // Project Cars packets are typically 300-350 bytes (PC1) or 400+ bytes (PC2)
    if (bufferSize >= 300 && bufferSize < 500) {
      uint32_t packetType = *((uint32_t*)&buffer[0]);
      uint16_t packetVersion = *((uint16_t*)&buffer[4]);
      
      // Check for Project Cars packet signatures
      if (packetType <= 1) {  // Type 0=telemetry, 1=FFB
        if (packetVersion == 1) {
          detection.gameType = GAME_PROJECT_CARS;
          detection.packetFormat = 101;
          return detection;
        } else if (packetVersion == 2) {
          detection.gameType = GAME_PROJECT_CARS_2;
          detection.packetFormat = 102;
          return detection;
        }
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
  
  static bool ConvertProjectCarsToUnified(const uint8_t* buffer, UnifiedTelemetry& output) {
    const ProjectCarsPacket* pc = (const ProjectCarsPacket*)buffer;
    
    output.speed = pc->m_speed * 3.6f;  // m/s to km/h
    output.throttle = pc->m_throttle;
    output.brake = pc->m_brakes;
    output.steering = pc->m_steering;
    output.engineRPM = pc->m_engineRate;
    output.topSpeed = 0;  // Will be updated by telemetry handler
    
    output.fuelRemaining = pc->m_fuelAmount;
    output.fuelCapacity = pc->m_fuelCapacity;
    
    output.drsAllowed = pc->m_drsAllowed > 0.5f;
    
    // Tires
    output.tireTemp[0] = pc->m_tyreTempRearLeft;
    output.tireTemp[1] = pc->m_tyreTempRearRight;
    output.tireTemp[2] = pc->m_tyreTempFrontLeft;
    output.tireTemp[3] = pc->m_tyreTempFrontRight;
    
    output.tireWear[0] = pc->m_tyreWearRearLeft * 100;
    output.tireWear[1] = pc->m_tyreWearRearRight * 100;
    output.tireWear[2] = pc->m_tyreWearFrontLeft * 100;
    output.tireWear[3] = pc->m_tyreWearFrontRight * 100;
    
    output.carPosition = pc->m_racePosition;
    output.currentLapNum = pc->m_lapNumber;
    
    return true;
  }
  
  static bool ConvertProjectCars2ToUnified(const uint8_t* buffer, UnifiedTelemetry& output) {
    const ProjectCars2Packet* pc2 = (const ProjectCars2Packet*)buffer;
    
    output.speed = pc2->m_speed * 3.6f;  // m/s to km/h
    output.throttle = pc2->m_throttle;
    output.brake = pc2->m_brakes;
    output.steering = pc2->m_steering;
    output.engineRPM = pc2->m_engineRate;
    output.topSpeed = 0;
    
    output.fuelRemaining = pc2->m_fuelAmount;
    output.fuelCapacity = pc2->m_fuelCapacity;
    
    output.drsAllowed = pc2->m_drsAllowed > 0.5f;
    
    // Tires
    output.tireTemp[0] = pc2->m_tyreTempRearLeft;
    output.tireTemp[1] = pc2->m_tyreTempRearRight;
    output.tireTemp[2] = pc2->m_tyreTempFrontLeft;
    output.tireTemp[3] = pc2->m_tyreTempFrontRight;
    
    output.tireWear[0] = pc2->m_tyreWearRearLeft * 100;
    output.tireWear[1] = pc2->m_tyreWearRearRight * 100;
    output.tireWear[2] = pc2->m_tyreWearFrontLeft * 100;
    output.tireWear[3] = pc2->m_tyreWearFrontRight * 100;
    
    output.carPosition = pc2->m_racePosition;
    output.currentLapNum = pc2->m_lapNumber;
    output.lastLapTimeMS = pc2->m_lastTimeTrialLapTimeMS;
    output.currentLapTimeMS = pc2->m_currentTimeTrialLapTimeMS;
    
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
