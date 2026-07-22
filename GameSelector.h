// Game selection and configuration management
// Allows users to manually select which game is expected

#pragma once
#include "TelemetryData.h"

// Game selector state
GameType selectedGame = GAME_UNKNOWN;
bool autoDetectGame = true;  // Allow auto-detect as fallback

// Get game name string
String GetGameName(GameType game) {
  switch (game) {
    case GAME_F1_2022:
      return "F1 2022";
    case GAME_F1_2023:
      return "F1 2023";
    case GAME_F1_2024:
      return "F1 2024";
    case GAME_ACC:
      return "ACC";
    case GAME_PROJECT_CARS:
      return "Project Cars";
    case GAME_PROJECT_CARS_2:
      return "Project Cars 2";
    case GAME_UNKNOWN:
      return "Auto Detect";
    default:
      return "Unknown";
  }
}

// Set the selected game
void SetSelectedGame(GameType game) {
  selectedGame = game;
  if (game == GAME_UNKNOWN) {
    autoDetectGame = true;
    DEBUG_SERIAL.println("Game selection: Auto Detect enabled");
    myNex.writeStr("gamesel.txt", "Auto Detect");
  } else {
    autoDetectGame = false;
    DEBUG_SERIAL.print("Game selection: ");
    DEBUG_SERIAL.println(GetGameName(game));
    myNex.writeStr("gamesel.txt", GetGameName(game));
  }
  SaveGameSelection();
}

// Get the currently selected game
GameType GetSelectedGame() {
  return selectedGame;
}

// Check if we should accept packets from a specific game
bool IsGameAllowed(GameType detectedGame) {
  if (autoDetectGame) {
    return true;  // Accept any game in auto-detect mode
  }
  return (detectedGame == selectedGame);  // Only accept selected game
}

// Save game selection to config file
void SaveGameSelection() {
  // Store in config.json
  File configFile = SD.open(confFile, FILE_WRITE);
  if (configFile) {
    DynamicJsonDocument doc(1024);
    
    // Read existing config
    if (SD.exists(confFile)) {
      String content = "";
      while (configFile.available()) {
        content += (char)configFile.read();
      }
      deserializeJson(doc, content);
      configFile.close();
      configFile = SD.open(confFile, FILE_WRITE);
      configFile.seek(0);
    }
    
    // Update game selection
    doc["selectedGame"] = (int)selectedGame;
    doc["autoDetect"] = autoDetectGame;
    
    serializeJson(doc, configFile);
    configFile.close();
    DEBUG_SERIAL.println("Game selection saved to config");
  }
}

// Load game selection from config file
void LoadGameSelection() {
  if (SD.exists(confFile)) {
    File configFile = SD.open(confFile, FILE_READ);
    if (configFile) {
      String content = "";
      while (configFile.available()) {
        content += (char)configFile.read();
      }
      configFile.close();
      
      DynamicJsonDocument doc(1024);
      if (deserializeJson(doc, content) == DeserializationError::Ok) {
        if (doc.containsKey("selectedGame")) {
          int gameId = doc["selectedGame"];
          selectedGame = (GameType)gameId;
        }
        if (doc.containsKey("autoDetect")) {
          autoDetectGame = doc["autoDetect"];
        }
        DEBUG_SERIAL.print("Game selection loaded: ");
        DEBUG_SERIAL.println(GetGameName(selectedGame));
      }
    }
  }
}

// Display game selector on Nextion (to be called from button handler)
void ShowGameSelector() {
  myNex.writeStr("page gameselector");
}

// Handle game selection from Nextion buttons
// Call these from your Nextion button handlers
void SelectGame_F1_2022() {
  SetSelectedGame(GAME_F1_2022);
  myNex.writeStr("page main");
}

void SelectGame_F1_2023() {
  SetSelectedGame(GAME_F1_2023);
  myNex.writeStr("page main");
}

void SelectGame_F1_2024() {
  SetSelectedGame(GAME_F1_2024);
  myNex.writeStr("page main");
}

void SelectGame_ACC() {
  SetSelectedGame(GAME_ACC);
  myNex.writeStr("page main");
}

void SelectGame_ProjectCars() {
  SetSelectedGame(GAME_PROJECT_CARS);
  myNex.writeStr("page main");
}

void SelectGame_ProjectCars2() {
  SetSelectedGame(GAME_PROJECT_CARS_2);
  myNex.writeStr("page main");
}

void SelectGame_AutoDetect() {
  SetSelectedGame(GAME_UNKNOWN);
  myNex.writeStr("page main");
}

#endif
