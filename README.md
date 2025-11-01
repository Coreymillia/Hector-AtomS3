# Hector 3D Visualization - AtomS3 Version

This is an adaptation of the Hector 3D visualization project for the M5Stack AtomS3.

## Hardware Features

- **M5Stack AtomS3**: ESP32-S3 based controller
- **Display**: 0.85" IPS screen (128x128 pixels)
- **Sensors**: Built-in MPU6886 6-axis IMU
- **Input**: Single programmable button
- **Connectivity**: WiFi, Bluetooth

## Key Changes from M5StickC Plus2 Version

### Display Adaptations
- Scaled rendering for 128x128 display (vs 135x240)
- Adjusted grid size and step parameters for smaller screen
- Dynamic display dimension detection
- Optimized UI elements for compact space

### Hardware Differences
- **No built-in microphone**: Sound-reactive modes removed
- **Single button**: Long press for wave pattern cycling
- **M5Unified library**: Modern M5Stack library stack
- **ESP32-S3**: More powerful processor with better performance

### Control Scheme
- **Short Press Button A**: Cycle through display styles (Grid → Solid → Zebra → Checkerboard)
- **Long Press Button A (1s)**: Cycle through wave patterns
- **Long Press Power (2s)**: Pause/Resume animation

### Wave Patterns Available
1. **Flat Grid**: Static baseline
2. **Tilt Reactive**: Responds to device tilting via IMU
3. **Sine Wave**: Classic mathematical sine wave
4. **Drip Wave**: Water droplet effect
5. **Spiral Wave**: Rotating spiral pattern
6. **Interference**: Multiple wave interference
7. **Mountain Range**: Terrain-like surface
8. **Ripple Tank**: Dual-source ripples
9. **Plasma Field**: Smooth plasma-like effect

### Display Styles
1. **Grid**: Wireframe rendering
2. **Solid**: Filled triangular surfaces
3. **Zebra**: Alternating stripe pattern
4. **Checkerboard**: Rectangular pattern

## Installation

1. Install PlatformIO
2. Clone/download this project
3. Open in PlatformIO
4. Build and upload to AtomS3

```bash
cd HectorAtomS3
pio run --target upload
```

## Libraries Used

- M5Unified (modern M5Stack base library)
- M5GFX (graphics and display driver)

## Performance

The AtomS3 version is optimized for the smaller display and more powerful ESP32-S3 processor:
- Reduced grid complexity for smooth 30+ FPS
- Efficient memory usage
- Real-time IMU integration for interactive effects

## Original Credits

- Original ESP32-Hector by tobozo (MIT License)
- Inspired by Gerard Ferrandez's work: http://codepen.io/ge1doot/details/eWzQBm/
- M5StickC Plus2 adaptation
- AtomS3 adaptation

## Future Enhancements

Possible additions for the AtomS3 version:
- WiFi connectivity features
- Web interface for remote control
- Custom wave pattern programming
- Export/import of visualization settings