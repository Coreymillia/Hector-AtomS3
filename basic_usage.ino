/*
 * M5AtomS3 Hector - Basic Usage Example
 * 
 * This simplified example demonstrates the core concepts of the Hector 3D engine
 * Perfect for learning and experimenting with custom wave patterns
 */

#include <M5Unified.h>

// Simple wave function
float simpleWave(float x, float y, float time) {
  float distance = sqrt(x*x + y*y);
  return 50 * sin(distance * 0.2 - time);
}

void setup() {
  M5.begin();
  
  // Initialize display
  M5.Display.setBrightness(128);
  M5.Display.fillScreen(BLACK);
  
  Serial.begin(115200);
  Serial.println("Basic Hector Example Started!");
}

void loop() {
  static float time = 0;
  time += 0.1;
  
  M5.Display.fillScreen(BLACK);
  
  // Simple 2D visualization of wave function
  for (int screen_x = 0; screen_x < 128; screen_x += 4) {
    for (int screen_y = 0; screen_y < 128; screen_y += 4) {
      // Convert screen coordinates to mathematical space
      float x = (screen_x - 64) * 0.5;  // -32 to +32
      float y = (screen_y - 64) * 0.5;  // -32 to +32
      
      // Calculate wave height
      float z = simpleWave(x, y, time);
      
      // Convert height to color intensity
      int intensity = constrain(128 + z, 0, 255);
      uint16_t color = M5.Display.color565(intensity, intensity/2, 255-intensity);
      
      // Draw pixel
      M5.Display.fillRect(screen_x, screen_y, 4, 4, color);
    }
  }
  
  delay(50); // ~20 FPS
}