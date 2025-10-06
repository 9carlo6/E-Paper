/*
 * E-Paper Display Configuration
 * 
 * SUPPORTED HARDWARE:
 * - Waveshare 5.65" e-paper display
 * - Resolution: 600x448 pixels
 * - Colors: 7 (white, black, red, green, blue, yellow, orange)
 * - Controller: ESP32
 *
 * Repository: https://github.com/9carlo6/E-Paper
 * @author 9carlo6
 * @date 2025
 * @version 1.0
 *
 * @copyright
 * Copyright (c) 2025 9carlo6  
 * All rights reserved.
 */

#ifndef DISPLAY_CONFIG_H
#define DISPLAY_CONFIG_H

#include <GxEPD2_7C.h>

/* ========================================
   DISPLAY CONFIGURATION
   ======================================== */

#define GxEPD2_DISPLAY_CLASS GxEPD2_7C
#define GxEPD2_DRIVER_CLASS GxEPD2_565c_GDEP0565D90

/* ========================================
   PIN CONNECTIONS ESP32 -> DISPLAY
   Modify these values for different wiring
   ======================================== */

#define EPD_CS   5    // Chip Select (SPI)
#define EPD_DC   17   // Data/Command
#define EPD_RST  16   // Reset
#define EPD_BUSY 4    // Busy Signal

/* ========================================
   MEMORY CONFIGURATION
   ESP32 has ~320KB RAM, using 64KB for display buffer
   ======================================== */

#define BUFFER_SIZE 5000ul

// Calculate maximum manageable height based on memory
// 7-color display uses 4 bits per pixel = 2 pixels per byte
#define MAX_HEIGHT(EPD) (EPD::HEIGHT <= BUFFER_SIZE / (EPD::WIDTH / 2) ? \
                        EPD::HEIGHT : BUFFER_SIZE / (EPD::WIDTH / 2))

/* ========================================
   GLOBAL DISPLAY INSTANCE
   ======================================== */

GxEPD2_DISPLAY_CLASS<GxEPD2_DRIVER_CLASS, MAX_HEIGHT(GxEPD2_DRIVER_CLASS)> 
    display(GxEPD2_DRIVER_CLASS(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY));

#endif