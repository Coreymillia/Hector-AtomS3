# Wave Mathematics - M5AtomS3 Hector 🌊

## 🧮 **Mathematical Foundation**

The Hector visualization engine transforms mathematical equations into real-time 3D surfaces. Each wave pattern represents a different mathematical concept, making abstract math visually tangible.

### **Coordinate System**
- **X, Y**: Spatial coordinates (-20 to +20 units)
- **Z**: Surface height (function output)  
- **K**: Time parameter (continuously increasing)
- **Grid**: 20×20 sample points for real-time rendering

### **Function Signature**
```cpp
float wave_function(float x, float y, float k) {
    // Mathematical transformation
    return height_value;  // Typically -100 to +100
}
```

---

## 🌊 **Wave Pattern Equations**

### **1. Sine Wave**
```cpp
float sinwave(float x, float y, float k) {
  float r = 0.001 * (x*x + y*y);
  return 100 * cos(-k + r) / (2 + r);
}
```
**Mathematical Concept**: Radial sine wave with distance-based frequency modulation
- **Equation**: `z = 100 * cos(-t + r²/1000) / (2 + r²/1000)`
- **Physics**: Similar to water ripples spreading from a point source
- **Education**: Demonstrates wave propagation, frequency, and amplitude decay

---

### **2. Drip Wave**
```cpp
float dripwave(float x, float y, float k) {
  float r = 1.5 * sqrt(x*x + y*y);
  const float amplitude = 2.5;
  const float a = 200.0;
  const float b = (amplitude - fmod(k/3, amplitude)) - amplitude/2;
  return (a / (1 + r)) * cos((b / log(r + 2)) * r);
}
```
**Mathematical Concept**: Modulated wave with logarithmic frequency variation
- **Equation**: `z = (200/(1+r)) * cos((b/ln(r+2)) * r)`
- **Physics**: Models water droplet impact creating expanding ripples
- **Education**: Complex wave modulation, logarithmic functions

---

### **3. Flat Grid**
```cpp
float flatgrid(float x, float y, float k) {
  return 0; // Perfectly flat surface
}
```
**Mathematical Concept**: Null function baseline
- **Equation**: `z = 0`
- **Purpose**: Reference plane for comparison with other patterns
- **Education**: Demonstrates coordinate system and grid structure

---

### **4. Tilt-Reactive Surface** ⭐
```cpp
float tiltwave(float x, float y, float k) {
  // IMU input creates tilted plane
  float tiltX = imu_accX * 20;
  float tiltY = imu_accY * 20;
  float baseHeight = (x * tiltX + y * tiltY) * 0.5;
  
  // Add gyroscope ripples
  float gyroEffect = (imu_gyroX + imu_gyroY) * 10;
  float r = sqrt(x*x + y*y) * 0.1;
  float ripple = gyroEffect * cos(k + r) * 0.3;
  
  return baseHeight + ripple;
}
```
**Mathematical Concept**: Linear plane with sensor-driven modulation
- **Base Equation**: `z = (x·αₓ + y·αᵧ) / 2` (tilted plane)
- **Ripple Addition**: `z += γ·cos(t + r/10)` (gyro ripples)
- **Physics**: Demonstrates accelerometer integration and real-time response
- **Education**: Linear algebra (plane equations), sensor fusion

---

### **5. Spiral Wave**
```cpp
float spiralwave(float x, float y, float k) {
  float r = sqrt(x*x + y*y);
  float theta = atan2(y, x);
  float spiral = sin(theta * 3 + r * 0.2 - k * 2) * 25;
  return spiral / (1 + r * 0.1);
}
```
**Mathematical Concept**: Polar coordinate transformation with angular modulation
- **Equation**: `z = sin(3θ + 0.2r - 2t) · 25 / (1 + 0.1r)`
- **Conversion**: `r = √(x² + y²)`, `θ = atan2(y,x)`
- **Physics**: Rotational wave patterns, spiral galaxies
- **Education**: Polar coordinates, trigonometric functions, coordinate transforms

---

### **6. Interference Pattern**
```cpp
float interferencewave(float x, float y, float k) {
  float wave1 = sin(sqrt((x+20)*(x+20) + y*y) * 0.3 - k) * 25;
  float wave2 = sin(sqrt((x-20)*(x-20) + y*y) * 0.3 - k*1.2) * 25;
  float wave3 = sin(sqrt(x*x + (y-10)*(y-10)) * 0.3 - k*0.8) * 20;
  return (wave1 + wave2 + wave3) * 0.6;
}
```
**Mathematical Concept**: Wave superposition from multiple sources
- **Wave Sources**: (-20,0), (20,0), (0,-10)
- **Equation**: `z = Σ Aᵢ·sin(0.3·rᵢ - fᵢ·t)`
- **Physics**: Constructive/destructive interference, Young's double slit
- **Education**: Wave superposition principle, interference patterns

---

### **7. Mountain Range**
```cpp
float mountainwave(float x, float y, float k) {
  // Create mountain-like terrain
  float mountain1 = 40 * cos((x + k*5) * 0.1) * cos(y * 0.15);
  float mountain2 = 25 * sin((x - k*3) * 0.08) * sin((y+10) * 0.12);
  float noise = 10 * sin(x*0.3 + k) * cos(y*0.25 + k*0.7);
  return mountain1 + mountain2 + noise;
}
```
**Mathematical Concept**: Multi-frequency terrain generation
- **Base Terrain**: `z₁ = 40·cos(0.1x)·cos(0.15y)`
- **Secondary Features**: `z₂ = 25·sin(0.08x)·sin(0.12y)`  
- **Surface Detail**: `z₃ = 10·sin(0.3x)·cos(0.25y)`
- **Physics**: Geological formations, topographic mapping
- **Education**: Function composition, terrain modeling

---

### **8. Ripple Tank**
```cpp
float ripplewave(float x, float y, float k) {
  // Multiple ripple sources
  float r1 = sqrt((x+15)*(x+15) + (y+10)*(y+10));
  float r2 = sqrt((x-20)*(x-20) + (y-15)*(y-15));
  float r3 = sqrt((x-10)*(x-10) + (y+25)*(y+25));
  
  float ripple1 = 30 * cos(-k*2 + r1*0.3) / (1 + r1*0.1);
  float ripple2 = 25 * cos(-k*2.5 + r2*0.28) / (1 + r2*0.12);
  float ripple3 = 20 * cos(-k*1.8 + r3*0.35) / (1 + r3*0.08);
  
  return ripple1 + ripple2 + ripple3;
}
```
**Mathematical Concept**: Multiple point sources with amplitude decay
- **Sources**: (-15,-10), (20,15), (10,-25)
- **Equation**: `zᵢ = Aᵢ·cos(-fᵢ·t + 0.3rᵢ) / (1 + 0.1rᵢ)`
- **Physics**: Laboratory ripple tank experiments, wave interference
- **Education**: Distance-based amplitude decay, phase relationships

---

### **9. Plasma Field**
```cpp
float plasmawave(float x, float y, float k) {
  // Plasma-like energy field
  float plasma1 = sin(x*0.2 + k) + cos(y*0.25 + k*1.3);
  float plasma2 = sin((x+y)*0.15 + k*0.8) + cos((x-y)*0.18 + k*1.1);
  float plasma3 = sin(sqrt(x*x + y*y)*0.1 + k*0.6);
  
  return (plasma1 + plasma2 + plasma3) * 15;
}
```
**Mathematical Concept**: Multi-dimensional harmonic oscillation
- **Linear Components**: `sin(0.2x + t) + cos(0.25y + 1.3t)`
- **Diagonal Waves**: `sin(0.15(x+y) + 0.8t) + cos(0.18(x-y) + 1.1t)`
- **Radial Component**: `sin(0.1√(x²+y²) + 0.6t)`
- **Physics**: Electromagnetic fields, plasma dynamics
- **Education**: Harmonic motion, multi-dimensional waves

---

## 🔬 **Mathematical Analysis**

### **Frequency Analysis**
Each pattern exhibits different frequency characteristics:

| Pattern | Spatial Freq | Temporal Freq | Complexity |
|---------|-------------|---------------|------------|
| SINE_WAVE | Variable | 1.0× | Low |
| DRIP_WAVE | Log scale | 0.33× | High |
| TILT_REACTIVE | 0 (static) | IMU-driven | Medium |
| SPIRAL_WAVE | 3×angular | 2.0× | Medium |
| INTERFERENCE | 0.3× | 1.0×,1.2×,0.8× | High |
| MOUNTAIN_RANGE | 0.1×,0.08× | 5×,3×,1× | Medium |
| RIPPLE_TANK | 0.3×,0.28×,0.35× | 2×,2.5×,1.8× | High |
| PLASMA_FIELD | 0.2×,0.25×,0.1× | 1×,1.3×,0.6× | High |

### **Amplitude Characteristics**
- **Peak Amplitude**: ±100 units (display-optimized)
- **Decay Functions**: `1/(1+αr)` for distance-based attenuation
- **Modulation**: Time-based (`k`) and spatial (`x,y`) components

### **Computational Complexity**
```cpp
// Performance ranking (CPU cycles per point)
FLAT_GRID < SIN_WAVE < TILT_REACTIVE < SPIRAL_WAVE < MOUNTAIN_RANGE < DRIP_WAVE < PLASMA_FIELD < RIPPLE_TANK < INTERFERENCE
```

---

## 🎓 **Educational Applications**

### **Mathematics Courses**

#### **Calculus**
- **Multivariable Functions**: `z = f(x,y,t)`
- **Partial Derivatives**: Surface slopes and gradients
- **Integration**: Area under curved surfaces

#### **Trigonometry**
- **Sine/Cosine Waves**: Periodic behavior visualization  
- **Phase Relationships**: Multiple wave interference
- **Amplitude Modulation**: Distance-based scaling

#### **Linear Algebra**
- **Coordinate Transforms**: 3D to 2D projection
- **Matrix Operations**: Rotation and translation
- **Vector Fields**: Gradient visualization

### **Physics Courses**

#### **Wave Mechanics**
- **Superposition Principle**: Multiple wave addition
- **Interference Patterns**: Constructive/destructive interference
- **Wave Propagation**: Radial and linear wave motion

#### **Oscillations**
- **Harmonic Motion**: Sine/cosine temporal evolution
- **Coupled Oscillators**: Multiple frequency components
- **Damped Oscillation**: Amplitude decay with distance

### **Computer Science**

#### **3D Graphics**
- **Surface Rendering**: Real-time mesh generation
- **Projection Mathematics**: 3D to 2D coordinate mapping
- **Performance Optimization**: Algorithm efficiency

#### **Numerical Methods**
- **Discrete Sampling**: Grid-based approximation
- **Real-time Computation**: Frame rate constraints
- **Approximation Functions**: `romsin()`, `romcos()`

---

## 🛠️ **Creating Custom Patterns**

### **Design Principles**
1. **Performance**: Keep calculations simple for 30+ FPS
2. **Visual Appeal**: Ensure interesting motion and patterns
3. **Mathematical Interest**: Demonstrate specific concepts
4. **Parameter Range**: Output ±100 for consistent scaling

### **Template Function**
```cpp
float mypattern(float x, float y, float k) {
  // Step 1: Calculate spatial relationships
  float r = sqrt(x*x + y*y);              // Distance from origin
  float theta = atan2(y, x);               // Angle from x-axis
  
  // Step 2: Apply mathematical transformation
  float base_wave = sin(/* your function */);
  
  // Step 3: Add time evolution
  float time_component = k * speed_factor;
  
  // Step 4: Scale and return
  return amplitude * base_wave;
}
```

### **Example: Hexagonal Pattern**
```cpp
float hexpattern(float x, float y, float k) {
  // Convert to hexagonal coordinates
  float hex_x = x;
  float hex_y = (2 * y - x) / sqrt(3);
  
  // Create hexagonal wave
  float wave1 = sin(hex_x * 0.3 - k);
  float wave2 = sin(hex_y * 0.3 - k * 1.1); 
  float wave3 = sin((hex_x - hex_y) * 0.3 - k * 0.9);
  
  return (wave1 + wave2 + wave3) * 20;
}
```

---

**The mathematical beauty of these equations comes alive through real-time visualization, making abstract concepts tangible and inspiring!** ✨📐