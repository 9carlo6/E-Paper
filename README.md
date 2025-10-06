# 🖼️ E-Paper Photo Frame

A modern web-based photo frame controller for Waveshare 5.65" 7-color e-paper displays. Upload and display images wirelessly with an intuitive web interface featuring interactive cropping, multiple dithering algorithms, and image rotation.

![Version](https://img.shields.io/badge/version-1.0-blue)
![Platform](https://img.shields.io/badge/platform-ESP32-green)
![License](https://img.shields.io/badge/license-MIT-orange)

## 📸 Screenshots

### Display Boot Screen
![Boot Screen](images/boot_screen.jpg)
*Initial screen showing QR code for easy connection*

### Web Interface
![Web Interface](images/web_interface.jpg)
*Clean, modern upload interface*

### Interactive Crop Editor
![Crop Editor](images/crop_editor.jpg)
*Drag, zoom, rotate - perfect positioning every time*

### Algorithm Selection
![Algorithm Selection](images/algorithm_selection.jpg)
*Compare 5 different conversion methods side-by-side*

### Final Result on Display
![Display Result](images/display_result.jpg)
*Your photo beautifully rendered in 7 colors*

## ✨ Features

### Web Interface
- **Interactive Crop Editor** - Drag to reposition, pinch/scroll to zoom
- **90° Rotation** - Rotate images to fit any orientation
- **Background Selection** - Choose white or black backgrounds
- **5 Dithering Algorithms** - Preview and compare conversion methods:
  - Floyd-Steinberg (balanced, natural look)
  - Atkinson (softer, less noise)
  - Ordered/Bayer (retro, patterned)
  - No Dithering (pure colors, cartoon style)
  - Black & White (classic 2-color)
- **Live Preview** - See exactly how your image will look before uploading
- **Touch-Friendly** - Works seamlessly on mobile devices

### Connectivity
- **WiFi Station Mode** - Connect to your existing network
- **Access Point Fallback** - Automatic AP mode if no network configured
- **QR Code Display** - Scan to connect instantly
- **Web Configuration** - Save WiFi credentials through browser

### Storage & Performance
- **SPIFFS Storage** - No PSRAM required
- **Optimized Memory Usage** - ~64KB buffer for 600x448 display
- **Fast Uploads** - Progress tracking and efficient transfer

## 🛠️ Hardware Requirements

### Required Components
- **ESP32 Development Board** (any variant with SPI support)
- **Waveshare 5.65" 7-Color E-Paper Display** (600x448 pixels)
  - Model: GDEP0565D90
  - Colors: Black, White, Red, Green, Blue, Yellow, Orange

### Pin Connections

| ESP32 Pin | Display Pin | Function |
|-----------|-------------|----------|
| GPIO 5    | CS          | Chip Select |
| GPIO 17   | DC          | Data/Command |
| GPIO 16   | RST         | Reset |
| GPIO 4    | BUSY        | Busy Signal |
| 3.3V      | VCC         | Power |
| GND       | GND         | Ground |

> **Note:** Modify pin assignments in `display_config.h` if using different wiring.

## 📦 Installation

### Prerequisites
Install the following libraries through Arduino IDE Library Manager:

```
- GxEPD2 (by Jean-Marc Zingg)
- QRCode (by Richard Moore)
```

### Upload Steps

1. **Clone Repository**
   ```bash
   git clone https://github.com/9carlo6/E-Paper.git
   cd E-Paper
   ```

2. **Open in Arduino IDE**
   - Open `E-Paper_Photo_Frame.ino`

3. **Configure Board**
   - Board: ESP32 Dev Module
   - Upload Speed: 921600
   - Flash Size: 4MB (with SPIFFS)
   - Partition Scheme: Default 4MB with spiffs

4. **Verify Pin Configuration**
   - Check `display_config.h` matches your wiring

5. **Upload**
   - Click Upload and wait for completion

## 🚀 Usage

### First Time Setup

1. **Power On** - ESP32 boots and displays welcome screen with QR code
2. **Connect to WiFi** - Two options:
   - **Option A:** Scan QR code on display
   - **Option B:** Connect to `E-Paper WiFi` (password: `epaper2024`)
3. **Open Web Interface** - Navigate to displayed IP address
4. **Configure WiFi** (optional) - Visit `/config` to save your network credentials

### Uploading Images

1. **Open Web Interface** - Go to `http://[IP-ADDRESS]`
2. **Upload Image** - Click or drag image (JPG, PNG, BMP supported)
3. **Crop & Position**
   - Drag to move image
   - Use zoom controls or pinch to scale
   - Click "Rotate" to rotate 90°
   - Select white or black background
   - Use "Fit" to auto-scale, "Center" to center image
4. **Choose Algorithm** - Preview all conversion methods and select preferred one
5. **Upload to Display** - Click "Upload to Display" and wait for refresh

### Subsequent Uses

After initial WiFi configuration, the frame automatically connects to your network on boot. Simply open the web interface at the saved IP address.

## 📁 Project Structure

```
E-Paper/
├── E-Paper_Photo_Frame.ino    # Main program file
├── display_config.h            # Display hardware configuration
├── web_interface.h             # Complete web interface (HTML/CSS/JS)
└── README.md                   # This file
```

## ⚙️ Configuration

### WiFi Settings
Default AP credentials (modify in `E-Paper_Photo_Frame.ino`):
```cpp
#define AP_SSID "E-Paper WiFi"
#define AP_PASS "epaper2025"
```

### Display Settings
Pin configuration (modify in `display_config.h`):
```cpp
#define EPD_CS   5
#define EPD_DC   17
#define EPD_RST  16
#define EPD_BUSY 4
```

### Image Settings
Resolution (modify in `E-Paper_Photo_Frame.ino`):
```cpp
#define IMAGE_WIDTH 448
#define IMAGE_HEIGHT 600
```

## 🔧 Troubleshooting

### Display Not Updating
- Check pin connections
- Verify SPIFFS is initialized (check Serial Monitor)
- Ensure image file size is correct (134,400 bytes)

### WiFi Connection Failed
- Check SSID and password
- Verify signal strength
- Try AP mode by removing saved credentials

### Web Interface Not Loading
- Confirm correct IP address
- Check ESP32 is powered and connected
- Try accessing `/config` endpoint

### Upload Errors
- Ensure image file is valid format
- Check SPIFFS has available space
- Verify network connection is stable

## 🎨 About Dithering Algorithms

The web interface offers 5 different algorithms to convert your photos for the 7-color display:

- **Floyd-Steinberg**: Best for photographs, provides natural gradients
- **Atkinson**: Softer alternative, less grainy than Floyd-Steinberg
- **Ordered (Bayer)**: Creates retro halftone patterns, good for graphics
- **No Dithering**: Pure color quantization, cartoon-like effect
- **Black & White**: Classic 2-color dithering for monochrome look

Each algorithm provides a live preview so you can choose the best result for your image.

## 📝 Serial Monitor Output

The system provides detailed logging:
- SPIFFS initialization status
- WiFi connection progress
- IP address assignment
- Web server status
- Upload progress
- Display update timing

## 🤝 Contributing

Contributions are welcome! Feel free to:
- Report bugs
- Suggest features
- Submit pull requests
- Improve documentation

## 📄 License

This project is licensed under the MIT License - see the LICENSE file for details.

## 🙏 Acknowledgments

- [GxEPD2](https://github.com/ZinggJM/GxEPD2) library by Jean-Marc Zingg
- [QRCode](https://github.com/ricmoo/qrcode) library by Richard Moore
- Waveshare for e-paper display documentation

## 📧 Contact

For questions or support, please open an issue on GitHub.

---

**Project Link:** [https://github.com/9carlo6/E-Paper](https://github.com/9carlo6/E-Paper)
