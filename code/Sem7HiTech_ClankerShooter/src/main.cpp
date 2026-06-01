#include <Arduino.h>
#include "GyroscoopLSM6DSOX.h"
#include "JoyStick.h"
#include "DisplayWaveshare.h"
#include "Button.h"
#include "Clanker.h"
#include "Radar.h"
#include "DISPLAY_SH1106.h"


/* planning on working on
zoom
laser beam
vibratie acturator
maak stand voor het geweer (geen code)
*/



#define JOY_X 34 // joystick
#define JOY_Y 35
#define TFT_CS 5 // display
#define TFT_DC 2
#define TFT_RST 4

DISPLAY_SH1106 oledKlein;
const int maxAmmo = 12;
int currentAmmo = maxAmmo;

IButton* btn_1; //btn
IButton* btn_2;
IButton* btn_3;
IButton* btn_4;

const int BTNPIN1 = 14;
const int BTNPIN2 = 27;
const int BTNPIN3 = 26;
const int BTNPIN4 = 25;

// Wereld data
bool playerAlive = true;
bool gameOverShown = false;
float playerX = 0, playerZ = 0;
float oldPlayerX = 0, oldPlayerZ = 0;
int killCount = 0;
bool gridDwarnOnes = true;

// enemys
unsigned long lastSpawnTime = 0;
const unsigned long spawnInterval = 2000;

const int MAX_ENEMIES = 5;
bool enemyActive[MAX_ENEMIES] = {false};

// Objecten
Clanker enemies[MAX_ENEMIES] = {
  Clanker(0,0), Clanker(0,0), Clanker(0,0),
  Clanker(0,0), Clanker(0,0)
};

GyroscoopLSM6DSOX gyro;
JoyStick joy(JOY_X, JOY_Y);
DisplayWaveshare screen(TFT_CS, TFT_DC, TFT_RST);

void I2cScanner(){
  Serial.println("I2C Scanner starting...");

  for (uint8_t address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    uint8_t error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address < 16) Serial.print("0");
      Serial.print(address, HEX);
      Serial.println(" !");
    } 
    else if (error == 4) {
      Serial.print("Unknown error at address 0x");
      if (address < 16) Serial.print("0");
      Serial.println(address, HEX);
    }
  }

  Serial.println("I2C scan complete.");
}

void setup() {
  Serial.begin(115200);
  Serial.println("baud rate set 115200");
  // I2cScanner();
  
  btn_1 = new Button(BTNPIN1, 50, true); // button object
  btn_2 = new Button(BTNPIN2, 50, true); // button object
  btn_3 = new Button(BTNPIN3, 50, true); // button object
  btn_4 = new Button(BTNPIN4, 50, true); // button object
  
  screen.begin();
  delay(10); // kleine vertraging voor het scherm om klaar te zijn
  oledKlein.begin();
  delay(10); // kleine vertraging voor het scherm om klaar te zijn
  joy.begin();


  if (!gyro.begin()) {
      Serial.println("Gyro Error!");
      while(1);
  }
}

void spawnEnemy() {
  for (int i = 0; i < MAX_ENEMIES; i++) {
    if (!enemyActive[i]) {
      float spawnX = random(-500, 500);
      float spawnZ = random(250, 300);  // spawn distance in front of the player

      enemies[i].reset(spawnX, spawnZ);
      enemyActive[i] = true;

      Serial.println("Enemy spawned!");
      break;
    }
  }
}

int reloadState = 0; // 0 = normaal, 1 = reload gestart, wacht op omhoog kijken
void handleReload() {
    float pitch = gyro.getPitch();

    // STAP 1: Detecteer of de speler naar de vloer kijkt
    // (Waarde > 45 graden betekent dat de voorkant van de controller naar beneden wijst)
    if (reloadState == 0 && pitch > 45.0f) {
        reloadState = 1;
        Serial.println("Laden gestart... kom nu omhoog!");
        screen.clearScreen();
        screen.drawReloadScreen();
    }

    // STAP 2: Detecteer of de speler weer recht vooruit kijkt
    if (reloadState == 1 && pitch < 30.0f) {
        currentAmmo = maxAmmo;
        reloadState = 0; // Reset naar normaal
        Serial.println("RELOAD VOLTOOID!");
        screen.clearScreen();

        // Update je OLED display direct
        oledKlein.drawHUD(currentAmmo, maxAmmo, killCount);
    }
}

void resetGyro() {
  Serial.println("IMU reset");
  gyro.resetGyro();
  gyro.update(); // update direct na reset om correcte waarden te hebben bij hervatten
  delay(10); // kleine vertraging om zeker te zijn dat reset is verwerkt
}



void loop() {
  gyro.update();
  float angle = gyro.getAngle();

  handleReload();

  // =========================
  // GAME OVER STATE
  // =========================
  if (!playerAlive) {
    static bool printed = false;

    if (!printed) {
      screen.gameOverScreen(killCount);
      Serial.println("GAME OVER!");
      printed = true;
    }

    if (btn_4->readButton()) {
      playerAlive = true;
      printed = false;

      playerX = 0;
      playerZ = 0;
      oldPlayerX = 0;
      oldPlayerZ = 0;

      for (int i = 0; i < MAX_ENEMIES; i++) {
        enemyActive[i] = false;
        enemies[i].takeDamage(999999);
        enemies[i].lastSize = 0;
      }

      killCount = 0;
      lastSpawnTime = millis();
    }

    return;
  }

  // =========================
  // PAUSE (toggle)
  // =========================
  static bool paused = false;

  if (btn_3->readButton()) {
    paused = !paused;

    if (paused) {
      screen.drawPauseScreen();
      return;
    } else {
      gyro.resetGyro();
      gyro.update();
      screen.clearScreen();
    }
  }

  if (paused) return;

  // =========================
  // imu reset
  // =========================
  if (btn_2->readButton()) {
    resetGyro();
  }

  // =========================
  // player movement
  // =========================
  float moveForward = joy.getY();

  // 1. deadzone (kills micro noise)
  if (abs(moveForward) < 0.08f) {
    moveForward = 0;
  }

  // 2. movement
  if (moveForward != 0) {
    oldPlayerX = playerX;
    oldPlayerZ = playerZ;

    playerX += sin(angle) * moveForward * 2.0f;
    playerZ += cos(angle) * moveForward * 2.0f;
  }

  // 3. snap (removes 1.000003 jitter)
  playerX = round(playerX * 100.0f) / 100.0f;
  playerZ = round(playerZ * 100.0f) / 100.0f;

  // =========================
  // SPAWN
  // =========================
  unsigned long now = millis();
  if (now - lastSpawnTime > spawnInterval) {
    spawnEnemy();
    lastSpawnTime = now;
  }

  // =========================
  // UPDATE ENEMIES / COLLISION
  // =========================
  for (int i = 0; i < MAX_ENEMIES; i++) {
    if (!enemyActive[i]) continue;

    enemies[i].update(playerX, playerZ);

    float dx = enemies[i].getX() - playerX;
    float dz = enemies[i].getZ() - playerZ;

    float d2 = dx * dx + dz * dz;

    if (d2 < 225.0f && !enemies[i].isDead()) { // 15^2
      playerAlive = false;
    }
  }

  // =========================
  // SHOOTING
  // =========================
  if (btn_1->readButton() && currentAmmo > 0) {
    currentAmmo--;
    screen.triggerShootEffect();

    for (int i = 0; i < MAX_ENEMIES; i++) {
      if (!enemyActive[i] || enemies[i].isDead()) continue;

      if (enemies[i].checkHit(160, 120, playerX, playerZ, angle)) {
        enemies[i].takeDamage(100);

        if (enemies[i].isDead()) {
          killCount++;
        }
      }
    }
  }
  screen.updateShootEffect();

  // =========================
  // tekenen
  // =========================

  if (reloadState == 0) {

    // only if player moved + small offset
    if(playerX != oldPlayerX || playerZ != oldPlayerZ) {
      // grid + UI mogen altijd als basis
      screen.drawGrid(playerX, playerZ, oldPlayerX, oldPlayerZ);
    } else { // eenamaalig ook
      if (gridDwarnOnes = true) {
        screen.drawGrid(playerX, playerZ, oldPlayerX, oldPlayerZ);
        gridDwarnOnes = false;
      }
    }

    for (int i = 0; i < MAX_ENEMIES; i++) {
      if (!enemyActive[i]) continue;

      ScreenPos pos = enemies[i].calculateProjection(playerX, playerZ, angle);

      if (enemies[i].isDead()) {
        enemyActive[i] = false;
        screen.drawEnemy(0, 0, 0, enemies[i].lastSx, enemies[i].lastSy, enemies[i].lastSize);
        continue;
      }

      screen.drawEnemy(
        pos.x, pos.y, pos.size,
        enemies[i].lastSx,
        enemies[i].lastSy,
        enemies[i].lastSize
      );

      if (pos.visible) {
        enemies[i].lastSx = pos.x;
        enemies[i].lastSy = pos.y;
        enemies[i].lastSize = pos.size;
      } else {
        enemies[i].lastSize = 0;
      }
    }

    Radar radar(MAX_ENEMIES);
    int side = radar.getClosestEnemySide(playerX, playerZ, angle, enemies, enemyActive);
    screen.drawEnemyIndicator(side);

    screen.drawUI(killCount);
    oledKlein.drawHUD(currentAmmo, maxAmmo, killCount);
  }
}