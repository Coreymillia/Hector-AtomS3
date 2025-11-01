# Troubleshooting Guide - M5AtomS3 Hector

## 🔧 **Common Issues & Solutions**

### 🚫 **Flashing Issues**

#### **Problem**: Device not recognized
```
Error: Failed to connect to ESP32-S3
```
**Solutions**:
1. **Hold Reset Button**: Press and hold reset during upload
2. **Check USB Cable**: Use data cable, not charge-only
3. **Driver Issues**: Install CP210x or CH340 drivers
4. **Port Selection**: Try different USB ports
```bash
# Check available ports
pio device list

# Force specific port
pio run --target upload --upload-port /dev/ttyACM0
```

#### **Problem**: Upload fails partway through
```
Error: Timed out waiting for packet header
```
**Solutions**:
1. **Lower Baud Rate**: Change `upload_speed = 921600` in platformio.ini
2. **Boot Mode**: Hold reset until green LED appears
3. **Power Cycle**: Disconnect and reconnect USB

---

### 📱 **Display Issues**

#### **Problem**: Black screen on boot
**Symptoms**: Device boots but display stays black
**Solutions**:
1. **Check Serial Output**:
```bash
pio device monitor --baud 115200
```
Look for initialization messages

2. **Display Brightness**: Try adjusting in code:
```cpp
M5.Display.setBrightness(255); // Maximum brightness
```

3. **Hard Reset**: Power cycle the device

#### **Problem**: Corrupted graphics/spiderweb lines
**Symptoms**: Lines drawn from corners, garbled display
**Solutions**:
✅ **Fixed in current version**, but if you see this:
1. **Check Version**: Ensure you're using latest code
2. **Memory Issue**: Monitor free heap in serial
3. **Coordinate Overflow**: Review wave function bounds

---

### 🤝 **Shake Detection Issues**

#### **Problem**: Shake not switching patterns
**Symptoms**: Shaking device doesn't change visualization
**Diagnosis**:
```cpp
// Check serial output for:
"IMU initialized for shake detection" // Should see this on boot
"Shake detected! Delta: X.XX" // Should see when shaking
```

**Solutions**:
1. **IMU Not Initialized**:
```cpp
// Check this appears in serial:
if (M5.Imu.begin()) {
  Serial.println("IMU initialized for shake detection");
} else {
  Serial.println("IMU initialization failed");
}
```

2. **Adjust Sensitivity**:
```cpp
const float shakeThreshold = 1.5; // Lower = more sensitive
const float shakeThreshold = 3.0; // Higher = less sensitive
```

3. **Check IMU Data**:
Monitor accelerometer values in serial output. Should show changes when moved.

#### **Problem**: Too sensitive (switches constantly)
**Solutions**:
1. **Increase Threshold**:
```cpp
const float shakeThreshold = 3.0; // Increase from 2.0
```

2. **Increase Debounce Time**:
```cpp
const unsigned long shakeDebounceTime = 2000; // 2 seconds instead of 1
```

---

### 📐 **Tilt Interaction Issues**

#### **Problem**: Tilt mode not responding
**Symptoms**: Surface doesn't change when tilting device in TILT_REACTIVE mode
**Solutions**:
1. **Verify Mode**: Ensure you're in TILT_REACTIVE pattern (shows "TILT" on screen)

2. **Check IMU Data**: Monitor serial for accelerometer values:
```
Frame X, Mode: 3, Accel: 0.02,-0.15,0.98, Gyro: 0.01,0.02
```
Values should change when tilting

3. **Increase Sensitivity**:
```cpp
float tiltX = imu_accX * 30; // Increase from 20
float tiltY = imu_accY * 30; // Increase from 20
```

#### **Problem**: Tilt too sensitive/jittery
**Solutions**:
1. **Improve Filtering**:
```cpp
smoothAccX = smoothAccX * 0.9 + imu_accX * 0.1; // More smoothing
```

2. **Reduce Sensitivity**:
```cpp
float tiltX = imu_accX * 10; // Decrease from 20
```

---

### ⚡ **Performance Issues**

#### **Problem**: Low frame rate/choppy animation
**Symptoms**: Jerky movement, poor responsiveness
**Solutions**:
1. **Check Frame Rate**: Monitor serial output for timing info

2. **Reduce Grid Size**:
```cpp
#define SIZE 30        // Reduce from 40
#define GRID_SIZE 15   // Reduce from 20
```

3. **Simplify Wave Functions**: Remove complex calculations in inner loops

4. **Memory Check**:
```cpp
Serial.printf("Free heap: %d bytes\n", ESP.getFreeHeap());
```

#### **Problem**: Device resets during operation
**Symptoms**: Random reboots, especially during pattern switching
**Solutions**:
1. **Stack Overflow**: Reduce recursion or large local variables
2. **Memory Fragmentation**: Avoid dynamic allocation
3. **Power Issues**: Use quality USB cable and power source

---

### 🔊 **Serial Communication Issues**

#### **Problem**: No serial output
**Solutions**:
1. **Check Baud Rate**: Ensure monitor matches code (115200)
2. **USB Driver**: Install proper drivers for your system
3. **Cable Quality**: Try different USB cable

#### **Problem**: Garbled serial output
**Solutions**:
1. **Baud Rate Mismatch**: Double-check both ends
2. **Buffer Overflow**: Reduce serial output frequency
3. **Hardware Issue**: Try different USB port

---

### 🔋 **Power Issues**

#### **Problem**: Device won't power on
**Solutions**:
1. **USB Cable**: Ensure data+power cable, not charge-only
2. **Power Source**: Try different USB port/charger
3. **Hardware Fault**: Check for physical damage

#### **Problem**: Random shutdowns
**Solutions**:
1. **Power Supply**: Use high-quality 5V supply
2. **Current Draw**: Check for excessive current usage
3. **Temperature**: Ensure adequate ventilation

---

## 🛠️ **Development Issues**

### **Build Errors**

#### **Library Not Found**
```
Error: Could not find the package with 'M5GFX' requirements
```
**Solution**:
```bash
pio lib install "m5stack/M5GFX@^0.1.4"
pio lib install "m5stack/M5Unified@^0.1.4"
```

#### **Compilation Errors**
```
'M5' was not declared in this scope
```
**Solutions**:
1. **Include Headers**: Ensure `#include <M5Unified.h>`
2. **Library Version**: Check compatible versions
3. **Platform**: Verify ESP32-S3 platform installed

### **Upload Issues**

#### **Out of Memory**
```
Error: File too large for available flash space
```
**Solutions**:
1. **Check Partition**: Verify 8MB flash configuration
2. **Reduce Size**: Remove unused features
3. **Optimize**: Enable compiler optimizations

---

## 📊 **Diagnostic Commands**

### **System Information**
```cpp
void printSystemInfo() {
  Serial.printf("Chip: %s\n", ESP.getChipModel());
  Serial.printf("Flash: %d MB\n", ESP.getFlashChipSize() / 1024 / 1024);
  Serial.printf("Free Heap: %d bytes\n", ESP.getFreeHeap());
  Serial.printf("CPU Freq: %d MHz\n", ESP.getCpuFreqMHz());
}
```

### **IMU Test**
```cpp
void testIMU() {
  float ax, ay, az, gx, gy, gz;
  if (M5.Imu.getAccel(&ax, &ay, &az) && M5.Imu.getGyro(&gx, &gy, &gz)) {
    Serial.printf("Accel: %.2f,%.2f,%.2f Gyro: %.2f,%.2f,%.2f\n", 
                  ax, ay, az, gx, gy, gz);
  } else {
    Serial.println("IMU read failed!");
  }
}
```

### **Performance Test**
```cpp
void performanceTest() {
  unsigned long start = millis();
  for (int i = 0; i < 1000; i++) {
    sinLoop(); // Render 1000 frames
  }
  unsigned long elapsed = millis() - start;
  Serial.printf("1000 frames in %lu ms (%.1f FPS)\n", elapsed, 1000000.0/elapsed);
}
```

---

## 🆘 **Getting More Help**

### **Still Having Issues?**

1. **Check Issues**: [GitHub Issues Page](https://github.com/YourUsername/M5AtomS3-Hector/issues)
2. **Discussions**: [GitHub Discussions](https://github.com/YourUsername/M5AtomS3-Hector/discussions)
3. **M5Stack Community**: [M5Stack Forum](https://community.m5stack.com/)

### **Reporting Bugs**

Include this information:
- **Hardware**: M5Stack AtomS3 version
- **Software**: PlatformIO version, library versions
- **Symptoms**: Detailed description
- **Serial Output**: Copy relevant console output
- **Steps**: How to reproduce the issue

### **Emergency Recovery**

If device becomes unresponsive:
1. **Hold Reset**: For 10+ seconds
2. **Flash Bootloader**: Using esptool
3. **Factory Reset**: Re-flash complete firmware
4. **Hardware Check**: Verify physical connections

---

**Most issues can be resolved with the solutions above. Don't hesitate to ask for help if you're still stuck!** 🤝✨