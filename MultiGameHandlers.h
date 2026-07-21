// Multi-game packet handling functions
// Routes telemetry packets to the correct game handler

#pragma once
#include "GameAdapter.h"

void ProcessGamePacket(const uint8_t* buffer, uint16_t packetSize) {
  if (packetSize < 5) return;
  
  // Detect game type from packet signature
  GameDetection detection = GameAdapter::DetectGame(buffer, packetSize);
  
  if (detection.gameType == GAME_UNKNOWN) {
    DEBUG_SERIAL.println("Unknown game format");
    return;
  }
  
  // Update current game if changed
  if (detection.gameType != currentGameType) {
    currentGameType = detection.gameType;
    gameDetection = detection;
    
    String gameStr;
    switch (currentGameType) {
      case GAME_F1_2022:
        gameStr = "F1 2022";
        break;
      case GAME_F1_2023:
        gameStr = "F1 2023";
        break;
      case GAME_F1_2024:
        gameStr = "F1 2024";
        break;
      case GAME_ACC:
        gameStr = "ACC";
        break;
      default:
        gameStr = "Unknown";
    }
    
    DEBUG_SERIAL.print("Game detected: ");
    DEBUG_SERIAL.println(gameStr);
    myNex.writeStr("page main");  // Reset to main page on game change
  }
  
  // Route to appropriate game handler
  switch (detection.gameType) {
    case GAME_F1_2022:
      ProcessF1Packet(buffer, packetSize, GAME_F1_2022);
      break;
    case GAME_F1_2023:
      ProcessF1Packet(buffer, packetSize, GAME_F1_2023);
      break;
    case GAME_F1_2024:
      ProcessF1Packet(buffer, packetSize, GAME_F1_2024);
      break;
    case GAME_ACC:
      ProcessACCPacket(buffer, packetSize);
      break;
    default:
      break;
  }
}

void ProcessF1Packet(const uint8_t* buffer, uint16_t packetSize, GameType gameType) {
  // All F1 versions use compatible header structure
  // Determine minimum header size
  if (packetSize < sizeof(PacketHeader)) return;
  
  struct PacketHeader *header = (struct PacketHeader *)buffer;
  HandleSessionUid(header->m_sessionUID);
  
  uint8_t myCar = header->m_playerCarIndex;
  
  // Handle lap data packets (packet ID = 2 for all F1 versions)
  if (header->m_packetId == 2) {
    if (packetSize >= sizeof(PacketLapData)) {
      struct PacketLapData *p = (struct PacketLapData *)buffer;
      struct LapData *telemetry = &(p->m_lapData[myCar]);
      HandleF1LapData(telemetry, p, myCar, gameType);
    }
  }
  
  // Car telemetry packets (packet ID = 6 for all F1 versions)
  if (header->m_packetId == 6) {
    if (packetSize >= sizeof(PacketCarTelemetryData)) {
      struct PacketCarTelemetryData *p = (struct PacketCarTelemetryData *)buffer;
      struct CarTelemetryData *telemetry = &(p->m_carTelemetryData[myCar]);
      HandleF1CarTelemetry(telemetry, p, myCar);
    }
  }
}

void ProcessACCPacket(const uint8_t* buffer, uint16_t packetSize) {
  // ACC has multiple packet types - need to identify which one
  if (packetSize < sizeof(ACCPhysicsPacket)) return;
  
  ACCPhysicsPacket *physics = (ACCPhysicsPacket *)buffer;
  
  // Process physics data
  HandleACCPhysics(physics);
}

// F1 2022 lap data handler - full implementation with original functionality
void HandleF1LapData(struct LapData *telemetry, struct PacketLapData *p, 
                     uint8_t myCar, GameType gameType) {
  currentLap = telemetry->m_currentLapNum;
  
  myNex.writeNum("main.pos.val", telemetry->m_carPosition);
  myNex.writeNum("main.lap.val", telemetry->m_currentLapNum);

  if (telemetry->m_pitLaneTimerActive) {
    myNex.writeStr("rv.plt.txt", GetTimeFromMillis(telemetry->m_pitLaneTimeInLaneInMS));
  }

  if (lastLapTime != telemetry->m_lastLapTimeInMS) {  
    lastLapTime = telemetry->m_lastLapTimeInMS;
    myNex.writeStr("main.llap.txt", GetTimeFromMillis(telemetry->m_lastLapTimeInMS));

    tenLastLapTime.unshift(laptimes{ currentLap - 1, lastLapTime });

    using index_t = decltype(tenLastLapTime)::index_t;
    for (index_t i = 0; i < tenLastLapTime.size(); i++) {
      String myNexObjectLapTime = "ll.lt" + String(i) + ".txt";
      String myNexObjectLap = "ll.l" + String(i) + ".txt";
      myNex.writeStr(myNexObjectLapTime, GetTimeFromMillis(tenLastLapTime[i].laptime));
      myNex.writeStr(myNexObjectLap, String(tenLastLapTime[i].lap));
    }

    if (fastestLapTime == 0.00 || lastLapTime < fastestLapTime) {
      fastestLapTime = lastLapTime;
      myNex.writeStr("main.blap.txt", GetTimeFromMillis(fastestLapTime));
    }
  }

  if (currentPage == 9 || currentPage == 12) {
    if (checkPointCarInFrontSet == true && checkPointCarInFrontDistance <= telemetry->m_totalDistance) {
      myNex.writeStr("dd.dcarinfront.txt", GetTimeFromMillis(millis() - checkPointCarInFrontMillis));
      checkPointCarInFrontSet = false;
    }

    if (!checkPointYourCarSet) {
      checkPointYourCarDistance = telemetry->m_totalDistance;
      checkPointYourCarMillis = millis();
      checkPointYourCarSet = true;
    }

    for (int i = 0; i < numActiveCars; i++) {
      struct LapData *opponent = &(p->m_lapData[i]);

      if (telemetry->m_carPosition == 1) {
        myNex.writeStr("dd.dcarinfront.txt", "");
        myNex.writeStr("dd.llcarinfront.txt", "");
        myNex.writeStr("dd.blcarinfront.txt", "");
        IdxCarInFront = -1;
      } else if (telemetry->m_carPosition != 1 && opponent->m_carPosition == (telemetry->m_carPosition - 1)) {
        if (!checkPointCarInFrontSet) {
          checkPointCarInFrontDistance = opponent->m_totalDistance;
          checkPointCarInFrontMillis = millis();
          checkPointCarInFrontSet = true;
          IdxCarInFront = i;
          myNex.writeStr("dd.llcarinfront.txt", GetTimeFromMillis(opponent->m_lastLapTimeInMS));
        }
      }

      if (telemetry->m_carPosition == numActiveCars) {
        myNex.writeStr("dd.dcarbehind.txt", "");
        myNex.writeStr("dd.llcarbehind.txt", "");
        myNex.writeStr("dd.blcarbehind.txt", "");
        IdxCarBehind = -1;
      } else if (opponent->m_carPosition == (telemetry->m_carPosition + 1)) {
        if (checkPointYourCarSet == true && checkPointYourCarDistance <= opponent->m_totalDistance) {
          myNex.writeStr("dd.dcarbehind.txt", GetTimeFromMillis(millis() - checkPointYourCarMillis));
          myNex.writeStr("dd.llcarbehind.txt", GetTimeFromMillis(opponent->m_lastLapTimeInMS));
          checkPointYourCarSet = false;
          IdxCarBehind = i;
        }
      }
    }
  }
}

// F1 car telemetry handler for F1 2022 (and compatible versions)
void HandleF1CarTelemetry(struct CarTelemetryData *telemetry, 
                          struct PacketCarTelemetryData *p, uint8_t myCar) {
  LedRevCounter(telemetry->m_engineRPM);

  if (currentPage == 1 || currentPage == 7 || currentPage == 6 || currentPage == 12) {
    if (telemetry->m_speed > 220 && telemetry->m_speed > yourTopSpeed) {
      yourTopSpeed = telemetry->m_speed;
      myNex.writeStr("rv.ts.txt", String(yourTopSpeed));
      myNex.writeStr("rv.ev.txt", "New Top Speed");
    }

    if (currentPage != 12) {
      myNex.writeNum("speed.val", telemetry->m_speed);
      myNex.writeNum("rpm.val", telemetry->m_engineRPM);

      if (!showSuggestedGear) {
        myNex.writeStr("gear.txt", GEAR_NAMES[telemetry->m_gear & 0x0F]);
      } else {
        myNex.writeStr("gear.txt", String(p->m_suggestedGear));
      }
      myNex.writeNum("main.throttle.val", telemetry->m_throttle * 100);
      myNex.writeNum("main.brake.val", telemetry->m_brake * 100);
      myNex.writeStr("drs.txt", DRS_NAMES[telemetry->m_drs & 0x0F]);
      HandleTyreColors(telemetry->m_tyresInnerTemperature);
    }

    myNex.writeStr("temprl.txt", String(telemetry->m_tyresInnerTemperature[0]) + "\xB0C");
    myNex.writeStr("temprr.txt", String(telemetry->m_tyresInnerTemperature[1]) + "\xB0C");
    myNex.writeStr("tempfl.txt", String(telemetry->m_tyresInnerTemperature[2]) + "\xB0C");
    myNex.writeStr("tempfr.txt", String(telemetry->m_tyresInnerTemperature[3]) + "\xB0C");

    if (currentPage == 12) {
      HandleTyreColorsText(telemetry->m_tyresInnerTemperature);
    }
  }
}

// F1 2024 lap data handler (stub)
void HandleF1LapData2024(struct LapData2024 *telemetry, struct PacketLapData2024 *p, 
                         uint8_t myCar) {
  // Implementation for F1 2024 specific fields
  currentLap = telemetry->m_currentLapNum;
  myNex.writeNum("main.pos.val", telemetry->m_carPosition);
  myNex.writeNum("main.lap.val", telemetry->m_currentLapNum);
  
  // F1 2024 specific: DRS activations remaining
  if (telemetry->m_drsActivationsRemaining > 0) {
    DEBUG_SERIAL.print("DRS uses remaining: ");
    DEBUG_SERIAL.println(telemetry->m_drsActivationsRemaining);
  }
}

// F1 2024 car telemetry handler (stub)
void HandleF1CarTelemetry2024(struct PacketCarTelemetryData2024 *packet, uint8_t myCar) {
  // Handle rev lights, brake temperature, tire temps, etc.
  // This would update the LED strip and Nextion display
}

// ACC physics handler (stub)
void HandleACCPhysics(ACCPhysicsPacket *physics) {
  // Convert ACC telemetry to display format
  float speedKmh = physics->speed * 3.6f;
  DEBUG_SERIAL.print("ACC Speed: ");
  DEBUG_SERIAL.println(speedKmh);
  
  myNex.writeNum("main.speed.val", (uint16_t)speedKmh);
}

#endif
