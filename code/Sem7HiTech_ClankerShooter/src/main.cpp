#include <Arduino.h>
#include "gyroscoop.h"
#include "joyStick.h"
#include "display.h"
#include "Button.h"
#include "Clanker.h"

/* planning on working on
enemy indicator
zoom
laser beam
vibratie acturator
enemy kill count
imu reset
bullets show
ammo en reload
adding good sprits
update enemys zoals grid verwijder ze en dan over nieuw tekenen
maak stand voor het geweer (geen code)
*/

/* working on now
plane grid, horizontale and schijn
*/


#define JOY_X 34 // joystick
#define JOY_Y 35
#define TFT_CS 5 // display
#define TFT_DC 2
#define TFT_RST 4

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

gyroscoop gyro;
joyStick joy(JOY_X, JOY_Y);
display screen(TFT_CS, TFT_DC, TFT_RST);

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
  joy.begin();
  
  if (!gyro.begin()) {
      Serial.println("Gyro Error!");
      while(1);
  }
}

void spawnEnemy() {
  for (int i = 0; i < MAX_ENEMIES; i++) {
    if (!enemyActive[i]) {
      float spawnX = random(-200, 200);
      float spawnZ = random(100, 300);

      enemies[i].reset(spawnX, spawnZ);
      enemyActive[i] = true;

      Serial.println("Enemy spawned!");
      break;
    }
  }
}


void loop() {
  // imu

  gyro.update();
  float angle = gyro.getAngle();

  if (!playerAlive) {
    // Laat game-over scherm maar 1x zien
    static bool printed = false;
    if (!printed) {
      screen.gameOverScreen();
      Serial.println("GAME OVER!");
      printed = true;
    }

    // redo play again
    if (btn_4->readButton()) {
      Serial.println("Game redo");
      playerAlive = true;
      printed = false;
      playerX = 0;
      playerZ = 0;
      oldPlayerX = 0;
      oldPlayerZ = 0; 

      // Kill alle enemies
      for (int i = 0; i < MAX_ENEMIES; i++) {
        enemyActive[i] = false;
        enemies[i].takeDamage(999999); // force kill
        enemies[i].lastSize = 0;
      }

      lastSpawnTime = millis();
      screen.clearScreen();
    }

    return; // stop playing the game
  }

  if(btn_2->readButton()){
    Serial.println("Button 2 pressed");
  }
  
  if(btn_3->readButton()){
    Serial.println("Button 3 pressed");
  }

  // Joystick uitlezen voor beweging speler
  float moveForward = joy.getY(); // -1.0 tot 1.0
  float moveSide = joy.getX();    // -1.0 tot 1.0

  // Beweeg speler in de richting waarin hij kijkt
  if(abs(moveForward) > 0.2) { // speed
    oldPlayerX = playerX;
    oldPlayerZ = playerZ;
    
    playerX += sin(angle) * moveForward * 2.0f;
    playerZ += cos(angle) * moveForward * 2.0f;
  }
  
  // test
  screen.drawGrid(playerX, playerZ, oldPlayerX, oldPlayerZ);

  // spawner
  unsigned long currentTime = millis();
  if (currentTime - lastSpawnTime > spawnInterval) {
    spawnEnemy();
    lastSpawnTime = currentTime;
  }

  // colision
  for (int i = 0; i < MAX_ENEMIES; i++) {
    if (!enemyActive[i]) continue;

    enemies[i].update(playerX, playerZ);

    float dx = enemies[i].getX() - playerX;
    float dz = enemies[i].getZ() - playerZ;
    float distance = sqrt(dx*dx + dz*dz);

    if (distance < 15.0f && !enemies[i].isDead()  ) {
      playerAlive = false;
      Serial.println("GAME OVER!");
    }
  }

  // schieten
   if (btn_1->readButton()) {
    screen.drawShootEffect();
    Serial.println("FIRE!");

    for (int i = 0; i < MAX_ENEMIES; i++) {
      if (!enemyActive[i] || enemies[i].isDead()) continue;

      ScreenPos pos = enemies[i].calculateProjection(playerX, playerZ, angle);

      if (pos.visible) {
        int halfSize = pos.size / 2;

        if (160 > (pos.x - halfSize) && 160 < (pos.x + halfSize) &&
            120 > (pos.y - halfSize) && 120 < (pos.y + halfSize)) {
          
          Serial.println("HIT enemy " + String(i));
          enemies[i].takeDamage(100);
        }
      }
    }
  }

  // tekenen
  for (int i = 0; i < MAX_ENEMIES; i++) {
    if (!enemyActive[i]) continue;

    ScreenPos pos = enemies[i].calculateProjection(playerX, playerZ, angle);

    if (!enemies[i].isDead()) {
      screen.drawEnemy(pos.x, pos.y, pos.size, enemies[i].lastSx, enemies[i].lastSy, enemies[i].lastSize);

      if (pos.visible) {
        enemies[i].lastSx = pos.x;
        enemies[i].lastSy = pos.y;
        enemies[i].lastSize = pos.size;
      } else {
        enemies[i].lastSize = 0;
      }
    } else {
      screen.drawEnemy(0, 0, 0, enemies[i].lastSx, enemies[i].lastSy, enemies[i].lastSize);

      enemyActive[i] = false;
    }
  }

  // vezier
  screen.drawUI();

  delay(10); 
}