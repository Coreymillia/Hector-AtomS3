# Contributing to M5AtomS3-Hector

Thank you for your interest in contributing to the M5AtomS3-Hector 3D visualization project! 🎉

## 🎯 **Ways to Contribute**

### 🐛 **Bug Reports**
- Check existing [issues](https://github.com/YourUsername/M5AtomS3-Hector/issues) first
- Use the bug report template
- Include device info and steps to reproduce
- Add screenshots/videos if visual issues

### ✨ **Feature Requests**
- Describe the feature and use case
- Explain how it fits with existing functionality
- Consider performance impact on AtomS3 hardware

### 🌊 **New Wave Patterns**
We love new mathematical visualizations! Consider:
- Performance (must maintain 30+ FPS)
- Visual appeal on 128×128 display
- Mathematical interest/educational value

### 📚 **Documentation**
- Fix typos and improve clarity
- Add examples and tutorials
- Translate documentation
- Create educational content

## 🛠️ **Development Setup**

### **Prerequisites**
- PlatformIO or Arduino IDE
- M5Stack AtomS3 hardware
- Git for version control

### **Getting Started**
```bash
# Fork and clone
git clone https://github.com/YourUsername/M5AtomS3-Hector.git
cd M5AtomS3-Hector

# Create feature branch
git checkout -b feature/your-feature-name

# Build and test
pio run --target upload
```

## 📋 **Coding Standards**

### **Code Style**
- Use consistent indentation (2 spaces)
- Meaningful variable names
- Comment complex mathematical functions
- Follow existing code patterns

### **Wave Function Guidelines**
```cpp
// Good wave function example
float mywave(float x, float y, float k) {
  // Brief description of mathematical concept
  float frequency = 0.1;
  float amplitude = 30.0;
  
  // Calculate wave with performance in mind
  float r = sqrt(x*x + y*y);
  return amplitude * sin(r * frequency - k);
}
```

### **Performance Requirements**
- Maintain 30+ FPS on AtomS3
- Keep mathematical calculations simple
- Use approximations where appropriate
- Test on actual hardware

## 🔄 **Pull Request Process**

### **Before Submitting**
1. Test on actual AtomS3 hardware
2. Verify all wave patterns still work
3. Check memory usage hasn't increased significantly
4. Update documentation if needed

### **PR Template**
```
## Description
Brief description of changes

## Type of Change
- [ ] Bug fix
- [ ] New wave pattern
- [ ] Performance improvement
- [ ] Documentation update

## Testing
- [ ] Tested on AtomS3 hardware
- [ ] All existing patterns still work
- [ ] Frame rate remains smooth

## Screenshots/Videos
(If visual changes)
```

### **Review Process**
- Maintainer will review within 48-72 hours
- May request changes or testing
- Approved PRs will be merged to main branch

## 🌊 **Adding New Wave Patterns**

### **Step 1: Create Wave Function**
Add to `src/main.cpp`:
```cpp
float mynewwave(float x, float y, float k) {
  // Your mathematical visualization
  return /* your calculation */;
}
```

### **Step 2: Add to Enum**
```cpp
typedef enum {
  // ... existing patterns ...
  MY_NEW_WAVE = 9
} wave_style_t;
```

### **Step 3: Update Switch Statements**
Add cases in:
- `checkShakeDetection()`
- `sinLoop()` status display

### **Step 4: Test Performance**
- Monitor frame rate
- Check memory usage
- Test shake detection still works

## 📊 **Performance Guidelines**

### **Frame Rate Requirements**
- Target: 30+ FPS sustained
- Minimum: 20 FPS acceptable
- Test with complex patterns active

### **Memory Constraints**
- Flash: Keep under 500KB total
- RAM: Don't exceed 30KB usage
- Avoid dynamic allocation in loops

### **Mathematical Optimization**
- Use `romsin()`, `romcos()` for approximations
- Prefer integer math where possible  
- Cache expensive calculations
- Avoid division in inner loops

## 🎓 **Educational Content**

### **Wave Pattern Documentation**
When adding patterns, include:
- Mathematical equation
- Physical/visual concept
- Educational applications
- Parameter explanations

### **Example Documentation**
```markdown
## Interference Wave
**Equation**: `wave1 + wave2 + wave3`
**Concept**: Multiple wave sources creating interference patterns
**Education**: Physics - wave superposition, constructive/destructive interference
**Parameters**: 
- Source positions: (±20, ±20, 0, ±30)
- Frequencies: 0.3, 0.3×1.2, 0.25×0.8
```

## 🐛 **Bug Fix Guidelines**

### **Critical Bugs** (Fix Immediately)
- Device crashes or resets
- Rendering artifacts (spiderweb lines)
- IMU/shake detection failures
- Memory leaks

### **Non-Critical Bugs**
- Minor visual glitches
- Performance optimizations
- Documentation errors

### **Testing Checklist**
- [ ] Device boots correctly
- [ ] All 9 patterns render cleanly
- [ ] Shake detection responsive
- [ ] Tilt reaction works in TILT mode
- [ ] No memory leaks over time
- [ ] Serial output clean

## 📞 **Getting Help**

### **Questions?**
- Open a [GitHub Discussion](https://github.com/YourUsername/M5AtomS3-Hector/discussions)
- Check existing documentation in `docs/`
- Review closed issues for similar problems

### **Stuck on Math?**
- Include your equation attempts
- Describe the visual effect you want
- We're happy to help with mathematical concepts!

## 🙏 **Recognition**

Contributors will be:
- Added to README credits
- Mentioned in release notes
- Invited as project collaborators (for significant contributions)

## 📄 **License**

By contributing, you agree that your contributions will be licensed under the MIT License.

---

**Happy coding and thank you for making AtomS3-Hector even more amazing!** 🌊✨