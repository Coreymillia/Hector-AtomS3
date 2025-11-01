/*
  ESP32-Hector adaptation for M5 AtomS3
  
  Original ESP32-Hector is placed under the MIT license
  Copyleft (c+) 2020 tobozo 
  M5StickC Plus2 adaptation by Assistant
  AtomS3 adaptation by Assistant

  This project is heavily inspired from the work of Gerard Ferrandez
  http://codepen.io/ge1doot/details/eWzQBm/
*/

#include <M5Unified.h>
#include <math.h>

// Remove conflicting definitions
#ifdef PI
#undef PI
#endif
#ifdef TWO_PI
#undef TWO_PI
#endif

#define PI 3.14159265359
#define TWO_PI 6.28318530718

float romsin(float x) { return sin(x); }
float romcos(float x) { return cos(x); }
float romsqrt(float x) { return sqrt(x); }
float romlog(float x) { return log(x); }
float rompow(float x) { return x * x; }

// Scaled for AtomS3's smaller display (128x128)
#define SIZE 40
#define STEP 2
#define GRID_SIZE (SIZE/STEP)

// 3D animation parameters (scaled for AtomS3's smaller screen)
static float size = SIZE;
static float step = STEP * 1.2;
static float doublestep = STEP * 2;
static float speed = 0.15;
static float tsize = 0.85 * size;
static float halfsize = size * 0.5;
static float zoom = 1.33;
static float k = 0;
static float romcosav, romsinav, romcosah, romsinah;

static int num = GRID_SIZE;
static int fps = 0;

int red, green, blue;
int minrangecolor = 20;
int maxrangecolor = 255;
int rangemap = maxrangecolor - minrangecolor;

float fstart = 0;

struct Coord3D {
  int16_t x, y;
  float z;
};

Coord3D HectorGrid[GRID_SIZE][GRID_SIZE];

int pathindex = 0;
float frand = 0;

// Display dimensions - will be set dynamically
int display_width = 128;
int display_height = 128;
int display_center_x = 64;
int display_center_y = 64;

// Display modes for different visual styles
typedef enum {
  DISPLAY_GRID = 0,
  DISPLAY_SOLID = 1,
  DISPLAY_ZEBRA = 2,
  DISPLAY_CHECKBOARD = 3
} display_style_t;

// Wave pattern modes - Complete set from original
typedef enum {
  DRIP_WAVE = 0,
  SIN_WAVE = 1,
  FLAT_GRID = 2,
  TILT_REACTIVE = 3,
  SPIRAL_WAVE = 4,
  INTERFERENCE = 5,
  MOUNTAIN_RANGE = 6,
  RIPPLE_TANK = 7,
  PLASMA_FIELD = 8
} wave_style_t;

display_style_t displayStyle = DISPLAY_GRID;
wave_style_t waveStyle = DRIP_WAVE;  // Start with drip wave like original

bool paused = false;

// IMU data storage
float imu_accX = 0, imu_accY = 0, imu_accZ = 0;
float imu_gyroX = 0, imu_gyroY = 0, imu_gyroZ = 0;
unsigned long lastIMURead = 0;

// Shake detection variables
float lastAccelMagnitude = 0;
float shakeThreshold = 2.0;  // Threshold for shake detection  
unsigned long lastShakeTime = 0;
unsigned long shakeDelay = 1000;  // Minimum time between shake detections
unsigned long lastShakeCheck = 0;
const unsigned long shakeCheckInterval = 50; // Check every 50ms

// Function declarations
void setupScale();
void resetCoords();
void drawPath(int scan_y);
void project(float x, float y, float z, int scan_x, int scan_y);
void checkButtons();
void updateIMU();      // Read IMU data
void checkShakeDetection(); // Check for shake gesture to switch modes

// Function pointer to sine wave function
float (*surfaceFunction)(float x, float y, float k);

// f(x,y) equation for sin wave
float sinwave(float x, float y, float k) {
  float r = 0.001 * (rompow(x) + rompow(y));
  return 100 * romcos(-k + r) / (2 + r);
}

// f(x,y) equation for water drip wave
float dripwave(float x, float y, float k) {
  float r = 1.5 * romsqrt(rompow(x) + rompow(y));
  const float amplitude = 2.5;
  const float a = 200.0;
  const float b = (amplitude - fmod(k / 3, amplitude)) - amplitude / 2;
  return (a / (1 + r)) * romcos((b / romlog(r + 2)) * r);
}

// f(x,y) equation for flat grid (baseline)
float flatgrid(float x, float y, float k) {
  return 0; // Perfectly flat surface
}

// f(x,y) equation for tilt-reactive surface
float tiltwave(float x, float y, float k) {
  // Use IMU data to create tilted plane with ripples
  float tiltX = imu_accX * 15; // Scale tilt effect for smaller screen
  float tiltY = imu_accY * 15;
  float baseHeight = (x * tiltX + y * tiltY) * 0.4; // Tilted plane
  
  // Add subtle ripples based on gyro movement
  float gyroEffect = (imu_gyroX + imu_gyroY) * 8;
  float r = romsqrt(rompow(x) + rompow(y)) * 0.1;
  float ripple = gyroEffect * romcos(k + r) * 0.25;
  
  return baseHeight + ripple;
}

// Spiral wave pattern
float spiralwave(float x, float y, float k) {
  float r = romsqrt(rompow(x) + rompow(y));
  float theta = atan2(y, x);
  float spiral = romsin(theta * 3 + r * 0.2 - k * 2) * 25;
  return spiral / (1 + r * 0.1);
}

// Interference pattern
float interferencewave(float x, float y, float k) {
  // Multiple wave sources creating interference
  float wave1 = romsin(romsqrt(rompow(x - 10) + rompow(y)) * 0.3 - k) * 20;
  float wave2 = romsin(romsqrt(rompow(x + 10) + rompow(y)) * 0.3 - k * 1.1) * 20;
  float wave3 = romsin(romsqrt(rompow(x) + rompow(y - 10)) * 0.3 - k * 0.9) * 15;
  return wave1 + wave2 + wave3;
}

// Mountain range pattern
float mountainwave(float x, float y, float k) {
  float base = romsin(x * 0.1) * romcos(y * 0.1) * 40;
  float detail = romsin(x * 0.3 + k * 0.5) * romcos(y * 0.3 + k * 0.7) * 15;
  return base + detail;
}

// Ripple tank simulation
float ripplewave(float x, float y, float k) {
  float r1 = romsqrt(rompow(x - 15) + rompow(y - 15));
  float r2 = romsqrt(rompow(x + 15) + rompow(y + 15));
  float ripple1 = romsin(r1 * 0.5 - k * 3) * 25 / (1 + r1 * 0.1);
  float ripple2 = romsin(r2 * 0.5 - k * 3.2) * 25 / (1 + r2 * 0.1);
  return ripple1 + ripple2;
}

// Plasma field effect
float plasmawave(float x, float y, float k) {
  float plasma1 = romsin(x * 0.2 + k) * romcos(y * 0.2 + k * 1.1);
  float plasma2 = romsin((x + y) * 0.15 + k * 0.8) * romcos((x - y) * 0.15 + k * 0.9);
  return (plasma1 + plasma2) * 30;
}

void setupScale() {
  // Dynamically adjust based on actual display dimensions
  display_width = M5.Display.width();
  display_height = M5.Display.height();
  display_center_x = display_width / 2;
  display_center_y = display_height / 2;
}

void resetCoords() {
  for (int i = 0; i < GRID_SIZE; i++) {
    for (int j = 0; j < GRID_SIZE; j++) {
      HectorGrid[i][j].x = -9999;  // Use clearly invalid coordinates
      HectorGrid[i][j].y = -9999;
      HectorGrid[i][j].z = 0;
    }
  }
}

void project(float x, float y, float z, int scan_x, int scan_y) {
  float xx, yy, zz;
  
  xx = x * romcosah + z * romsinah;
  zz = z * romcosah - x * romsinah;
  
  yy = y * romcosav + zz * romsinav;
  zz = zz * romcosav - y * romsinav;
  
  if (scan_x >= 0 && scan_x < GRID_SIZE && scan_y >= 0 && scan_y < GRID_SIZE) {
    HectorGrid[scan_x][scan_y].x = (xx * zoom) + display_center_x;
    HectorGrid[scan_x][scan_y].y = (yy * zoom) + display_center_y;
    HectorGrid[scan_x][scan_y].z = zz;
  }
}

void drawPath(int scan_y) {
  if (scan_y < 1 || scan_y >= GRID_SIZE) return;
  
  for (pathindex = 1; pathindex < GRID_SIZE; pathindex++) {
    int16_t x0 = HectorGrid[pathindex-1][scan_y-1].x;
    int16_t y0 = HectorGrid[pathindex-1][scan_y-1].y;
    int16_t x1 = HectorGrid[pathindex][scan_y-1].x;
    int16_t y1 = HectorGrid[pathindex][scan_y-1].y;
    int16_t x2 = HectorGrid[pathindex][scan_y].x;
    int16_t y2 = HectorGrid[pathindex][scan_y].y;

    // Check if coordinates are valid (not the invalid marker)
    bool validCoords = (x0 > -9000 && y0 > -9000 && x1 > -9000 && y1 > -9000 && x2 > -9000 && y2 > -9000);
    
    if (validCoords) {
      // Additional bounds check - only draw if coordinates are reasonable
      bool inBounds = (x0 > -200 && x0 < display_width + 200 && y0 > -200 && y0 < display_height + 200 &&
                       x1 > -200 && x1 < display_width + 200 && y1 > -200 && y1 < display_height + 200 &&
                       x2 > -200 && x2 < display_width + 200 && y2 > -200 && y2 < display_height + 200);
      
      if (inBounds) {
        uint16_t color = M5.Display.color565(red, green, blue);
        
        switch (displayStyle) {
          case DISPLAY_GRID:
            // Draw wireframe grid - only if both endpoints are somewhat reasonable
            if (abs(x1 - x0) < display_width && abs(y1 - y0) < display_height) {
              M5.Display.drawLine(x0, y0, x1, y1, color);
            }
            if (abs(x2 - x1) < display_width && abs(y2 - y1) < display_height) {
              M5.Display.drawLine(x1, y1, x2, y2, color);
            }
            break;
            
          case DISPLAY_SOLID:
            // Draw filled triangles for solid surface effect
            if (pathindex > 1 && scan_y > 1) {
              int16_t x3 = HectorGrid[pathindex-1][scan_y-1].x;
              int16_t y3 = HectorGrid[pathindex-1][scan_y-1].y;
              
              if (x3 > -9000 && y3 > -9000 && x3 > -200 && x3 < display_width + 200 && 
                  y3 > -200 && y3 < display_height + 200) {
                // Draw triangulated surface only if triangle is reasonable size
                int maxDist = max(max(abs(x1-x0), abs(x2-x1)), max(abs(y1-y0), abs(y2-y1)));
                if (maxDist < display_width) {
                  M5.Display.fillTriangle(x0, y0, x1, y1, x2, y2, color);
                  M5.Display.fillTriangle(x0, y0, x2, y2, x3, y3, color);
                }
              }
            } else {
              // Fallback to lines for edge cases
              if (abs(x1 - x0) < display_width && abs(y1 - y0) < display_height) {
                M5.Display.drawLine(x0, y0, x1, y1, color);
              }
            }
            break;
            
          case DISPLAY_ZEBRA:
            // Draw lines only for alternating patterns (zebra stripes)
            if (scan_y % 2 == pathindex % 2) {
              if (abs(x1 - x0) < display_width && abs(y1 - y0) < display_height) {
                M5.Display.drawLine(x0, y0, x1, y1, color);
              }
              if (abs(x2 - x1) < display_width && abs(y2 - y1) < display_height) {
                M5.Display.drawLine(x1, y1, x2, y2, color);
              }
            }
            break;
            
          case DISPLAY_CHECKBOARD:
            // Draw filled rectangles in checkboard pattern
            if ((pathindex + scan_y) % 2 == 0) {
              // Draw a visible rectangle instead of just a pixel
              int rectSize = 3; // Smaller for AtomS3's small screen
              if (x1 >= 0 && x1 < display_width && y1 >= 0 && y1 < display_height) {
                int centerX = constrain(x1, rectSize/2, display_width - 1 - rectSize/2);
                int centerY = constrain(y1, rectSize/2, display_height - 1 - rectSize/2);
                M5.Display.fillRect(centerX - rectSize/2, centerY - rectSize/2, rectSize, rectSize, color);
                
                // Also add connecting lines for structure - only if reasonable
                if (abs(x1 - x0) < display_width/2 && abs(y1 - y0) < display_height/2) {
                  M5.Display.drawLine(x0, y0, centerX, centerY, color);
                }
              }
            }
            break;
        }
      }
    }
  }
}

void sinLoop() {
  if (paused) return;

  k += speed;
  
  // Update sensors for interactive modes
  updateIMU();
  checkShakeDetection();

  setupScale();

  // Use real IMU data instead of simulated gyro for camera angle
  float ah = imu_accY * 2.0; // Use real tilt for camera
  float av = imu_accX * 1.5;

  romcosav = romcos(av);
  romsinav = romsin(av);
  romcosah = romcos(ah);
  romsinah = romsin(ah);

  M5.Display.fillScreen(BLACK);

  // Reset grid coordinates before each frame
  resetCoords();

  int scan_y = 0;

  for (float x = halfsize; x >= -halfsize; x -= doublestep) {
    blue = map(x, -halfsize, halfsize, minrangecolor, maxrangecolor);

    int scan_x = 0;

    for (float y = -halfsize; y <= halfsize; y += step) {
      if (scan_x >= GRID_SIZE || scan_y >= GRID_SIZE) break;

      float z = surfaceFunction(x, y, k);
      green = map(y, -halfsize, halfsize, minrangecolor, maxrangecolor);
      float brightnessfactor = float(map(int(z), -50, 50, 100, 20)) / 100.0;
      red = maxrangecolor - (green - minrangecolor);
      green *= brightnessfactor;
      red *= brightnessfactor;
      blue *= brightnessfactor;

      project(x, y, z, scan_x, scan_y);
      scan_x++;
    }

    drawPath(scan_y);
    scan_y++;
  }

  // Show status information (compact for small screen)
  M5.Display.setTextSize(1);
  M5.Display.setTextColor(WHITE);
  M5.Display.setCursor(2, 2);

  // Show current mode (compact for small screen)
  switch (waveStyle) {
    case DRIP_WAVE: M5.Display.printf("DRIP"); break;
    case SIN_WAVE: M5.Display.printf("SINE"); break;
    case FLAT_GRID: M5.Display.printf("FLAT"); break;
    case TILT_REACTIVE: M5.Display.printf("TILT"); break;
    case SPIRAL_WAVE: M5.Display.printf("SPRL"); break;
    case INTERFERENCE: M5.Display.printf("INTRF"); break;
    case MOUNTAIN_RANGE: M5.Display.printf("MNTN"); break;
    case RIPPLE_TANK: M5.Display.printf("RIPP"); break;
    case PLASMA_FIELD: M5.Display.printf("PLSM"); break;
  }
  
  // Debug info for tracking issues
  static int frameCount = 0;
  frameCount++;
  if (frameCount % 120 == 0) {  // Every 120 frames (about every 2 seconds)
    Serial.printf("Frame %d, Mode: %d, Accel: %.2f,%.2f,%.2f, Gyro: %.2f,%.2f\n", 
                  frameCount, waveStyle, imu_accX, imu_accY, imu_accZ, imu_gyroX, imu_gyroY);
  }
}

void checkButtons() {
  M5.update();
  // No button functionality - shake only for mode switching
}

void updateIMU() {
  unsigned long currentTime = millis();
  
  // Update IMU data at 50Hz like the original M5StickC Plus2
  if (currentTime - lastIMURead >= 20) {
    lastIMURead = currentTime;
    
    // Use the M5StickC Plus2 method - get data into variables
    M5.Imu.getAccelData(&imu_accX, &imu_accY, &imu_accZ);
    M5.Imu.getGyroData(&imu_gyroX, &imu_gyroY, &imu_gyroZ);
    
    // Apply the same smoothing filter as M5StickC Plus2
    static float smoothAccX = 0, smoothAccY = 0;
    static float smoothGyroX = 0, smoothGyroY = 0;
    
    smoothAccX = smoothAccX * 0.8 + imu_accX * 0.2;
    smoothAccY = smoothAccY * 0.8 + imu_accY * 0.2;
    smoothGyroX = smoothGyroX * 0.8 + imu_gyroX * 0.2;
    smoothGyroY = smoothGyroY * 0.8 + imu_gyroY * 0.2;
    
    imu_accX = smoothAccX;
    imu_accY = smoothAccY;
    imu_gyroX = smoothGyroX;
    imu_gyroY = smoothGyroY;
  }
}

void checkShakeDetection() {
  // Use the exact same method as the working AtomS3 project
  if (millis() - lastShakeCheck > shakeCheckInterval) {
    float accelX, accelY, accelZ;
    
    // Try to get accelerometer data using the AtomS3 method
    if (M5.Imu.getAccel(&accelX, &accelY, &accelZ)) {
      // Calculate total acceleration magnitude
      float accelMagnitude = sqrt(accelX * accelX + accelY * accelY + accelZ * accelZ);
      
      // Detect shake (sudden change in acceleration) - AtomS3 method
      if (lastAccelMagnitude > 0) {
        float accelDelta = abs(accelMagnitude - lastAccelMagnitude);
        
        if (accelDelta > shakeThreshold && millis() - lastShakeTime > shakeDelay) {
          // Shake detected! Change mode
          lastShakeTime = millis();
          
          // Debug output
          Serial.printf("Shake detected! Delta: %.2f, Magnitude: %.2f\n", accelDelta, accelMagnitude);
          
          // Cycle to next wave pattern
          switch (waveStyle) {
            case DRIP_WAVE: 
              waveStyle = SIN_WAVE;
              surfaceFunction = &sinwave;
              Serial.println("Switched to SIN_WAVE");
              break;
            case SIN_WAVE: 
              waveStyle = FLAT_GRID;
              surfaceFunction = &flatgrid;
              Serial.println("Switched to FLAT_GRID");
              break;
            case FLAT_GRID: 
              waveStyle = TILT_REACTIVE;
              surfaceFunction = &tiltwave;
              Serial.println("Switched to TILT_REACTIVE");
              break;
            case TILT_REACTIVE: 
              waveStyle = SPIRAL_WAVE;
              surfaceFunction = &spiralwave;
              Serial.println("Switched to SPIRAL_WAVE");
              break;
            case SPIRAL_WAVE:
              waveStyle = INTERFERENCE;
              surfaceFunction = &interferencewave;
              Serial.println("Switched to INTERFERENCE");
              break;
            case INTERFERENCE:
              waveStyle = MOUNTAIN_RANGE;
              surfaceFunction = &mountainwave;
              Serial.println("Switched to MOUNTAIN_RANGE");
              break;
            case MOUNTAIN_RANGE:
              waveStyle = RIPPLE_TANK;
              surfaceFunction = &ripplewave;
              Serial.println("Switched to RIPPLE_TANK");
              break;
            case RIPPLE_TANK:
              waveStyle = PLASMA_FIELD;
              surfaceFunction = &plasmawave;
              Serial.println("Switched to PLASMA_FIELD");
              break;
            case PLASMA_FIELD:
              waveStyle = DRIP_WAVE;
              size = SIZE * 0.8;
              step = STEP * 1.8;
              setupScale();
              surfaceFunction = &dripwave;
              Serial.println("Switched to DRIP_WAVE");
              break;
          }
          
          // Visual feedback - brief status display
          M5.Display.fillRect(0, display_height - 15, display_width, 15, BLACK);
          M5.Display.setCursor(2, display_height - 12);
          M5.Display.setTextColor(GREEN);
          M5.Display.setTextSize(1);
          
          switch (waveStyle) {
            case DRIP_WAVE: M5.Display.printf("DRIP"); break;
            case SIN_WAVE: M5.Display.printf("SINE"); break;
            case FLAT_GRID: M5.Display.printf("FLAT"); break;
            case TILT_REACTIVE: M5.Display.printf("TILT"); break;
            case SPIRAL_WAVE: M5.Display.printf("SPIRAL"); break;
            case INTERFERENCE: M5.Display.printf("INTERFERENCE"); break;
            case MOUNTAIN_RANGE: M5.Display.printf("MOUNTAIN"); break;
            case RIPPLE_TANK: M5.Display.printf("RIPPLE"); break;
            case PLASMA_FIELD: M5.Display.printf("PLASMA"); break;
          }
        }
      }
      
      lastAccelMagnitude = accelMagnitude;
    }
    
    lastShakeCheck = millis();
  }
}

void setup() {
  // Initialize M5 AtomS3
  M5.begin();
  
  // Initialize IMU for shake detection and tilt reaction (like working examples)
  if (M5.Imu.begin()) {
    Serial.println("IMU initialized for shake detection and tilt reaction");
  } else {
    Serial.println("IMU initialization failed - shake detection disabled");
  }
  
  // Set up display
  M5.Display.setBrightness(128);
  M5.Display.fillScreen(BLACK);

  M5.Display.setTextColor(WHITE);
  M5.Display.setTextSize(1);
  M5.Display.setCursor(10, 30);
  M5.Display.println("ESP32-S3");
  M5.Display.setCursor(10, 50);
  M5.Display.println("Hector");
  M5.Display.setCursor(10, 70);
  M5.Display.println("AtomS3");
  M5.Display.setCursor(10, 90);
  M5.Display.println("Shake to switch");

  delay(3000);

  surfaceFunction = &dripwave; // Start with drip wave like original
  setupScale();

  fstart = millis() - 1;

  M5.Display.fillScreen(BLACK);
  M5.Display.setCursor(2, display_height - 10);
  M5.Display.setTextSize(1);
  M5.Display.printf("Shake to switch modes");

  Serial.begin(115200);
  Serial.println("Hector AtomS3 initialized");
  Serial.println("Shake device to switch wave patterns");
  Serial.println("Tilt device in TILT mode for interactive effects");
  
  // Display dimensions info for debugging
  Serial.printf("Display: %dx%d\n", display_width, display_height);
}

void loop() {
  checkButtons();
  sinLoop();
  
  // Small delay to prevent overwhelming the system
  delay(30);
}