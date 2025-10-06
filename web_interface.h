/*
 * Web Interface for E-Paper Photo Frame
 * Complete HTML/CSS/JavaScript interface
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

#ifndef WEB_INTERFACE_H
#define WEB_INTERFACE_H

const char HTML_PAGE[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
    <title>🖼️ E-Paper Controller</title>
    <style>
        * { margin: 0; padding: 0; box-sizing: border-box; }
        body {
            font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            min-height: 100vh;
            display: flex;
            align-items: center;
            justify-content: center;
            padding: 20px;
        }
        .container {
            background: white;
            border-radius: 20px;
            box-shadow: 0 20px 60px rgba(0,0,0,0.3);
            padding: 40px;
            max-width: 600px;
            width: 100%;
        }
        h1 { text-align: center; color: #333; margin-bottom: 10px; font-size: 2em; }
        .subtitle { text-align: center; color: #666; margin-bottom: 30px; font-size: 0.9em; }
        .upload-area {
            border: 3px dashed #ccc;
            border-radius: 10px;
            padding: 40px;
            text-align: center;
            cursor: pointer;
            transition: all 0.3s;
            margin: 20px 0;
        }
        .upload-area:hover { border-color: #667eea; background: #f8f9ff; }
        .upload-area.dragover { border-color: #667eea; background: #e8ebff; }
        input[type="file"] { display: none; }
        .crop-editor {
            display: none;
            margin: 20px 0;
            padding: 20px;
            background: #f8f9fa;
            border-radius: 10px;
        }
        .crop-container {
            position: relative;
            width: 100%;
            max-width: 448px;
            aspect-ratio: 448/600;
            margin: 20px auto;
            border: 2px solid #667eea;
            border-radius: 10px;
            overflow: hidden;
            background: #000;
            cursor: move;
            touch-action: none;
        }
        .crop-container canvas {
            width: 100%;
            height: 100%;
            display: block;
        }
        .crop-frame {
            position: absolute;
            top: 0; left: 0; right: 0; bottom: 0;
            border: 2px dashed rgba(255, 255, 255, 0.8);
            pointer-events: none;
        }
        .zoom-controls-row {
            display: flex;
            align-items: center;
            justify-content: center;
            gap: 10px;
            margin: 20px 0;
            flex-wrap: wrap;
        }
        .zoom-btn-large {
            width: 50px !important;
            height: 50px;
            padding: 0 !important;
            margin: 0 !important;
            font-size: 24px;
            line-height: 50px;
            border-radius: 10px;
        }
        .zoom-slider {
            flex: 1;
            min-width: 200px;
            max-width: 400px;
        }
        .crop-action-buttons {
            display: flex;
            gap: 10px;
            justify-content: center;
            margin: 15px 0;
            flex-wrap: wrap;
        }
        .crop-action-buttons button {
            flex: 1;
            min-width: 120px;
            width: auto;
            padding: 12px 20px;
            margin: 0;
        }
        .confirm-button-large {
            width: 100%;
            padding: 20px !important;
            font-size: 18px !important;
            margin-top: 20px !important;
        }
        .background-controls {
            display: flex;
            gap: 10px;
            justify-content: center;
            margin: 15px 0;
            align-items: center;
        }
        .bg-btn {
            width: 80px;
            height: 40px;
            border: 3px solid #ddd;
            cursor: pointer;
            transition: all 0.3s;
            display: flex;
            align-items: center;
            justify-content: center;
            font-weight: 600;
            border-radius: 5px;
        }
        .bg-btn.selected {
            border-color: #667eea;
            box-shadow: 0 0 10px rgba(102, 126, 234, 0.5);
        }
        .bg-btn-white {
            background: white;
            color: black;
        }
        .bg-btn-black {
            background: black;
            color: white;
        }
        .algorithm-selection {
            display: none;
            margin: 20px 0;
            padding: 20px;
            background: #f8f9fa;
            border-radius: 10px;
        }
        .algorithm-grid {
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(180px, 1fr));
            gap: 15px;
            margin-top: 20px;
        }
        .algorithm-option {
            cursor: pointer;
            border: 3px solid #ddd;
            border-radius: 10px;
            padding: 10px;
            background: white;
            transition: all 0.3s;
        }
        .algorithm-option:hover {
            border-color: #667eea;
            transform: translateY(-5px);
            box-shadow: 0 8px 20px rgba(102, 126, 234, 0.3);
        }
        .algorithm-option.selected {
            border-color: #667eea;
            background: #f0f3ff;
        }
        .algorithm-option canvas {
            width: 100%;
            border-radius: 5px;
            image-rendering: pixelated;
        }
        .algorithm-name {
            font-weight: 600;
            margin-top: 10px;
            color: #333;
            text-align: center;
        }
        .algorithm-desc {
            font-size: 0.85em;
            color: #666;
            margin-top: 5px;
            text-align: center;
        }
        .preview { margin: 20px 0; text-align: center; }
        .preview canvas {
            max-width: 100%;
            border-radius: 10px;
            box-shadow: 0 4px 12px rgba(0,0,0,0.1);
            image-rendering: pixelated;
        }
        button {
            width: 100%;
            padding: 15px;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            color: white;
            border: none;
            border-radius: 10px;
            font-size: 16px;
            font-weight: 600;
            cursor: pointer;
            transition: transform 0.2s;
            margin-top: 10px;
        }
        button:hover:not(:disabled) {
            transform: translateY(-2px);
            box-shadow: 0 6px 20px rgba(102, 126, 234, 0.4);
        }
        button:disabled { background: #ccc; cursor: not-allowed; }
        button.secondary { background: #6c757d; }
        button.success { background: #28a745; }
        button.danger { background: #dc3545; }
        .progress-bar {
            width: 100%;
            height: 30px;
            background: #e0e0e0;
            border-radius: 15px;
            overflow: hidden;
            margin: 20px 0;
            display: none;
        }
        .progress-fill {
            height: 100%;
            background: linear-gradient(90deg, #667eea 0%, #764ba2 100%);
            transition: width 0.3s;
            display: flex;
            align-items: center;
            justify-content: center;
            color: white;
            font-weight: 600;
        }
        .info {
            background: #e3f2fd;
            padding: 15px;
            border-radius: 8px;
            margin: 20px 0;
            font-size: 0.9em;
            color: #1976d2;
        }
        .icon { font-size: 3em; margin-bottom: 10px; }
    </style>
</head>
<body>
    <div class="container">
        <h1>🖼️ E-Paper Controller</h1>
        <p class="subtitle">Upload and crop images for your display</p>
        
        <div class="upload-area" id="uploadArea" onclick="document.getElementById('fileInput').click()">
            <div class="icon">📸</div>
            <p style="font-weight: 600; margin-bottom: 5px;">Click or drag an image</p>
            <p style="color: #666; font-size: 0.9em;">JPG, PNG, BMP supported</p>
            <input type="file" id="fileInput" accept="image/*" onchange="handleFile(this.files[0])">
        </div>
        
        <div class="crop-editor" id="cropEditor">
            <h3 style="text-align: center; margin-bottom: 15px;">✂️ Crop & Position Image</h3>
            <p style="text-align: center; color: #666; font-size: 0.9em; margin-bottom: 15px;">
                Drag to move • Pinch or use controls to zoom
            </p>
            
            <div class="background-controls">
                <span style="font-weight: 600; margin-right: 10px;">Background:</span>
                <div class="bg-btn bg-btn-white selected" id="bgWhite" onclick="setBackground('white')">White</div>
                <div class="bg-btn bg-btn-black" id="bgBlack" onclick="setBackground('black')">Black</div>
            </div>
            
            <div class="crop-container" id="cropContainer">
                <canvas id="cropCanvas"></canvas>
                <div class="crop-frame"></div>
            </div>
            
            <div class="zoom-controls-row">
                <button onclick="zoomOut()" class="zoom-btn-large secondary">−</button>
                <button onclick="zoomIn()" class="zoom-btn-large secondary">+</button>
                <input type="range" id="zoomSlider" class="zoom-slider" min="10" max="300" value="100">
                <span id="zoomValue" style="font-weight: 600; min-width: 60px;">100%</span>
            </div>
            
            <div class="crop-action-buttons">
                <button onclick="cancelCrop()" class="danger">✕ Cancel</button>
                <button onclick="resetCrop()" class="secondary">🔄 Reset</button>
                <button onclick="rotateImage()" class="secondary">🔄 Rotate</button>
            </div>
            
            <div class="crop-action-buttons">
                <button onclick="centerImage()" class="secondary">⊕ Center</button>
                <button onclick="fitImage()" class="success">📐 Fit</button>
            </div>
            
            <button onclick="confirmCrop()" class="confirm-button-large">✓ Confirm Crop</button>
        </div>
        
        <div class="algorithm-selection" id="algorithmSelection">
            <h3 style="text-align: center; margin-bottom: 10px;">🎨 Choose Conversion Algorithm</h3>
            <p style="text-align: center; color: #666; font-size: 0.9em; margin-bottom: 15px;">
                Click on your preferred version
            </p>
            
            <div class="algorithm-grid" id="algorithmGrid"></div>
            
            <button onclick="confirmAlgorithm()" id="confirmAlgorithmBtn" disabled class="confirm-button-large">
                ✓ Use Selected Algorithm
            </button>
            
            <button onclick="loadNewImage()" class="secondary" style="margin-top: 10px;">
                📷 Load New Image
            </button>
        </div>
        
        <div class="preview" id="preview" style="display: none;">
            <h3 style="margin-bottom: 15px;">Final Preview</h3>
            <canvas id="previewCanvas"></canvas>
        </div>
        
        <div class="info" id="info" style="display: none;"></div>
        
        <div class="progress-bar" id="progressBar">
            <div class="progress-fill" id="progressFill">0%</div>
        </div>
        
        <button id="uploadBtn" onclick="uploadToDisplay()" style="display: none;">📤 Upload to Display</button>
        <button id="newImageBtn" onclick="loadNewImage()" style="display: none; background: #6c757d; margin-top: 10px;">📷 Load New Image</button>
    </div>

    <script>
        /* ========================================
           CONSTANTS & CONFIGURATION
           ======================================== */
        const TARGET_WIDTH = 448;
        const TARGET_HEIGHT = 600;
        const COLORS = [
            [0, 0, 0], [255, 255, 255], [0, 255, 0], [0, 0, 255],
            [255, 0, 0], [255, 255, 0], [255, 140, 0]
        ];
        
        /* ========================================
           STATE VARIABLES
           ======================================== */
        let convertedBinary = null;
        let originalImage = null;
        let cropCanvas, cropCtx;
        let cropScale = 1.0;
        let cropOffsetX = 0;
        let cropOffsetY = 0;
        let cropRotation = 0;
        let isDragging = false;
        let lastMouseX = 0;
        let lastMouseY = 0;
        let backgroundColor = 'white';
        
        let isPinching = false;
        let initialPinchDistance = 0;
        let initialPinchScale = 1.0;
        
        let croppedImageData = null;
        let algorithmResults = {};
        let selectedAlgorithm = null;
        
        /* ========================================
           BACKGROUND COLOR SELECTION
           ======================================== */
        function setBackground(color) {
            backgroundColor = color;
            
            document.getElementById('bgWhite').classList.remove('selected');
            document.getElementById('bgBlack').classList.remove('selected');
            
            if (color === 'white') {
                document.getElementById('bgWhite').classList.add('selected');
            } else {
                document.getElementById('bgBlack').classList.add('selected');
            }
            
            drawCropPreview();
        }
        
        /* ========================================
           DRAG & DROP FILE HANDLING
           ======================================== */
        const uploadArea = document.getElementById('uploadArea');
        uploadArea.addEventListener('dragover', (e) => {
            e.preventDefault();
            uploadArea.classList.add('dragover');
        });
        uploadArea.addEventListener('dragleave', () => {
            uploadArea.classList.remove('dragover');
        });
        uploadArea.addEventListener('drop', (e) => {
            e.preventDefault();
            uploadArea.classList.remove('dragover');
            if (e.dataTransfer.files.length > 0) {
                handleFile(e.dataTransfer.files[0]);
            }
        });
        
        /* ========================================
           FILE UPLOAD & IMAGE LOADING
           ======================================== */
        function handleFile(file) {
            if (!file || !file.type.startsWith('image/')) {
                alert('Please select a valid image!');
                return;
            }
            
            const reader = new FileReader();
            reader.onload = (e) => {
                originalImage = new Image();
                originalImage.onload = () => {
                    document.getElementById('uploadArea').style.display = 'none';
                    document.getElementById('cropEditor').style.display = 'block';
                    document.getElementById('preview').style.display = 'none';
                    document.getElementById('info').style.display = 'none';
                    document.getElementById('uploadBtn').style.display = 'none';
                    document.getElementById('newImageBtn').style.display = 'none';
                    initCropEditor();
                };
                originalImage.src = e.target.result;
            };
            reader.readAsDataURL(file);
        }
        
        /* ========================================
           CROP EDITOR INITIALIZATION
           ======================================== */
        function initCropEditor() {
            cropCanvas = document.getElementById('cropCanvas');
            cropCtx = cropCanvas.getContext('2d');
            
            cropCanvas.width = TARGET_WIDTH;
            cropCanvas.height = TARGET_HEIGHT;
            
            cropScale = 1.0;
            cropOffsetX = 0;
            cropOffsetY = 0;
            cropRotation = 0;
            
            fitImage();
            
            const zoomSlider = document.getElementById('zoomSlider');
            zoomSlider.value = Math.round(cropScale * 100);
            zoomSlider.addEventListener('input', (e) => {
                const oldScale = cropScale;
                cropScale = e.target.value / 100;
                document.getElementById('zoomValue').textContent = e.target.value + '%';
                
                const centerX = TARGET_WIDTH / 2;
                const centerY = TARGET_HEIGHT / 2;
                
                cropOffsetX = centerX - (centerX - cropOffsetX) * (cropScale / oldScale);
                cropOffsetY = centerY - (centerY - cropOffsetY) * (cropScale / oldScale);
                
                drawCropPreview();
            });
            
            setupCropDrag();
            drawCropPreview();
        }
        
        /* ========================================
           MOUSE & TOUCH GESTURE HANDLING
           ======================================== */
        function setupCropDrag() {
            const container = document.getElementById('cropContainer');
            
            container.addEventListener('mousedown', (e) => {
                isDragging = true;
                lastMouseX = e.clientX;
                lastMouseY = e.clientY;
                e.preventDefault();
            });
            
            container.addEventListener('mousemove', (e) => {
                if (!isDragging) return;
                
                const dx = e.clientX - lastMouseX;
                const dy = e.clientY - lastMouseY;
                
                cropOffsetX += dx;
                cropOffsetY += dy;
                
                lastMouseX = e.clientX;
                lastMouseY = e.clientY;
                
                drawCropPreview();
                e.preventDefault();
            });
            
            container.addEventListener('mouseup', () => {
                isDragging = false;
            });
            
            container.addEventListener('mouseleave', () => {
                isDragging = false;
            });
            
            container.addEventListener('touchstart', (e) => {
                if (e.touches.length === 1) {
                    const touch = e.touches[0];
                    isDragging = true;
                    isPinching = false;
                    lastMouseX = touch.clientX;
                    lastMouseY = touch.clientY;
                } else if (e.touches.length === 2) {
                    isDragging = false;
                    isPinching = true;
                    
                    const touch1 = e.touches[0];
                    const touch2 = e.touches[1];
                    
                    initialPinchDistance = Math.hypot(
                        touch2.clientX - touch1.clientX,
                        touch2.clientY - touch1.clientY
                    );
                    initialPinchScale = cropScale;
                }
                e.preventDefault();
            });
            
            container.addEventListener('touchmove', (e) => {
                if (e.touches.length === 1 && isDragging) {
                    const touch = e.touches[0];
                    const dx = touch.clientX - lastMouseX;
                    const dy = touch.clientY - lastMouseY;
                    
                    cropOffsetX += dx;
                    cropOffsetY += dy;
                    
                    lastMouseX = touch.clientX;
                    lastMouseY = touch.clientY;
                    
                    drawCropPreview();
                } else if (e.touches.length === 2 && isPinching) {
                    const touch1 = e.touches[0];
                    const touch2 = e.touches[1];
                    
                    const currentDistance = Math.hypot(
                        touch2.clientX - touch1.clientX,
                        touch2.clientY - touch1.clientY
                    );
                    
                    const scaleChange = currentDistance / initialPinchDistance;
                    const oldScale = cropScale;
                    let newScale = initialPinchScale * scaleChange;
                    
                    newScale = Math.max(0.1, Math.min(3.0, newScale));
                    
                    cropScale = newScale;
                    
                    const centerX = TARGET_WIDTH / 2;
                    const centerY = TARGET_HEIGHT / 2;
                    cropOffsetX = centerX - (centerX - cropOffsetX) * (cropScale / oldScale);
                    cropOffsetY = centerY - (centerY - cropOffsetY) * (cropScale / oldScale);
                    
                    const zoomPercent = Math.round(newScale * 100);
                    document.getElementById('zoomSlider').value = zoomPercent;
                    document.getElementById('zoomValue').textContent = zoomPercent + '%';
                    
                    drawCropPreview();
                }
                e.preventDefault();
            });
            
            container.addEventListener('touchend', (e) => {
                if (e.touches.length < 2) {
                    isPinching = false;
                }
                if (e.touches.length === 0) {
                    isDragging = false;
                }
                e.preventDefault();
            });
        }
        
        /* ========================================
           CANVAS DRAWING WITH ROTATION
           ======================================== */
        function drawCropPreview() {
            cropCtx.fillStyle = backgroundColor;
            cropCtx.fillRect(0, 0, TARGET_WIDTH, TARGET_HEIGHT);
            
            cropCtx.save();
            
            cropCtx.translate(TARGET_WIDTH / 2, TARGET_HEIGHT / 2);
            cropCtx.rotate(cropRotation * Math.PI / 180);
            cropCtx.translate(-TARGET_WIDTH / 2, -TARGET_HEIGHT / 2);
            
            const scaledWidth = originalImage.width * cropScale;
            const scaledHeight = originalImage.height * cropScale;
            
            cropCtx.drawImage(
                originalImage,
                cropOffsetX,
                cropOffsetY,
                scaledWidth,
                scaledHeight
            );
            
            cropCtx.restore();
        }
        
        /* ========================================
           IMAGE TRANSFORMATION FUNCTIONS
           ======================================== */
        function rotateImage() {
            cropRotation = (cropRotation + 90) % 360;
            drawCropPreview();
        }
        
        function fitImage() {
            const imgWidth = (cropRotation % 180 === 0) ? originalImage.width : originalImage.height;
            const imgHeight = (cropRotation % 180 === 0) ? originalImage.height : originalImage.width;
            
            const scaleX = TARGET_WIDTH / imgWidth;
            const scaleY = TARGET_HEIGHT / imgHeight;
            cropScale = Math.max(scaleX, scaleY);
            
            const scaledWidth = originalImage.width * cropScale;
            const scaledHeight = originalImage.height * cropScale;
            cropOffsetX = (TARGET_WIDTH - scaledWidth) / 2;
            cropOffsetY = (TARGET_HEIGHT - scaledHeight) / 2;
            
            document.getElementById('zoomSlider').value = Math.round(cropScale * 100);
            document.getElementById('zoomValue').textContent = Math.round(cropScale * 100) + '%';
            
            drawCropPreview();
        }
        
        function resetCrop() {
            cropScale = 1.0;
            cropOffsetX = 0;
            cropOffsetY = 0;
            cropRotation = 0;
            document.getElementById('zoomSlider').value = 100;
            document.getElementById('zoomValue').textContent = '100%';
            drawCropPreview();
        }
        
        function centerImage() {
            const scaledWidth = originalImage.width * cropScale;
            const scaledHeight = originalImage.height * cropScale;
            cropOffsetX = (TARGET_WIDTH - scaledWidth) / 2;
            cropOffsetY = (TARGET_HEIGHT - scaledHeight) / 2;
            drawCropPreview();
        }
        
        function zoomIn() {
            const slider = document.getElementById('zoomSlider');
            const oldScale = cropScale;
            let currentValue = parseInt(slider.value);
            currentValue = Math.min(300, currentValue + 10);
            slider.value = currentValue;
            cropScale = currentValue / 100;
            document.getElementById('zoomValue').textContent = currentValue + '%';
            
            const centerX = TARGET_WIDTH / 2;
            const centerY = TARGET_HEIGHT / 2;
            cropOffsetX = centerX - (centerX - cropOffsetX) * (cropScale / oldScale);
            cropOffsetY = centerY - (centerY - cropOffsetY) * (cropScale / oldScale);
            
            drawCropPreview();
        }
        
        function zoomOut() {
            const slider = document.getElementById('zoomSlider');
            const oldScale = cropScale;
            let currentValue = parseInt(slider.value);
            currentValue = Math.max(10, currentValue - 10);
            slider.value = currentValue;
            cropScale = currentValue / 100;
            document.getElementById('zoomValue').textContent = currentValue + '%';
            
            const centerX = TARGET_WIDTH / 2;
            const centerY = TARGET_HEIGHT / 2;
            cropOffsetX = centerX - (centerX - cropOffsetX) * (cropScale / oldScale);
            cropOffsetY = centerY - (centerY - cropOffsetY) * (cropScale / oldScale);
            
            drawCropPreview();
        }
        
        function cancelCrop() {
            document.getElementById('cropEditor').style.display = 'none';
            document.getElementById('uploadArea').style.display = 'block';
            document.getElementById('fileInput').value = '';
            originalImage = null;
        }
        
        /* ========================================
           CROP CONFIRMATION & ALGORITHM SELECTION
           ======================================== */
        function confirmCrop() {
            croppedImageData = cropCtx.getImageData(0, 0, TARGET_WIDTH, TARGET_HEIGHT);
            document.getElementById('cropEditor').style.display = 'none';
            showAlgorithmSelection();
        }
        
        function showAlgorithmSelection() {
            const algorithmSection = document.getElementById('algorithmSelection');
            algorithmSection.style.display = 'block';
            
            const grid = document.getElementById('algorithmGrid');
            grid.innerHTML = '<p style="text-align:center;padding:20px;">⏳ Processing with different algorithms...</p>';
            
            setTimeout(() => {
                processWithAllAlgorithms();
            }, 100);
        }
        
        /* ========================================
           ALGORITHM PROCESSING & COMPARISON
           ======================================== */
        function processWithAllAlgorithms() {
            const algorithms = [
                {
                    name: 'Floyd-Steinberg',
                    desc: 'Balanced, natural look',
                    func: floydSteinbergDithering
                },
                {
                    name: 'Atkinson',
                    desc: 'Softer, less noise',
                    func: atkinsonDithering
                },
                {
                    name: 'Ordered (Bayer)',
                    desc: 'Retro, patterned',
                    func: orderedDithering
                },
                {
                    name: 'No Dithering',
                    desc: 'Pure colors, cartoon style',
                    func: noDithering
                },
                {
                    name: 'Black & White',
                    desc: 'Classic B&W, 2 colors',
                    func: blackAndWhiteDithering
                }
            ];
            
            const grid = document.getElementById('algorithmGrid');
            grid.innerHTML = '';
            
            algorithms.forEach((algo) => {
                const quantizedPixels = algo.func(
                    new Uint8ClampedArray(croppedImageData.data),
                    TARGET_WIDTH,
                    TARGET_HEIGHT
                );
                
                algorithmResults[algo.name] = quantizedPixels;
                
                const option = document.createElement('div');
                option.className = 'algorithm-option';
                option.onclick = () => selectAlgorithm(algo.name);
                
                const canvas = document.createElement('canvas');
                canvas.width = TARGET_WIDTH;
                canvas.height = TARGET_HEIGHT;
                const ctx = canvas.getContext('2d');
                const imageData = ctx.createImageData(TARGET_WIDTH, TARGET_HEIGHT);
                
                for (let i = 0; i < quantizedPixels.length; i++) {
                    const [r, g, b] = COLORS[quantizedPixels[i]];
                    const idx = i * 4;
                    imageData.data[idx] = r;
                    imageData.data[idx + 1] = g;
                    imageData.data[idx + 2] = b;
                    imageData.data[idx + 3] = 255;
                }
                ctx.putImageData(imageData, 0, 0);
                
                const name = document.createElement('div');
                name.className = 'algorithm-name';
                name.textContent = algo.name;
                
                const desc = document.createElement('div');
                desc.className = 'algorithm-desc';
                desc.textContent = algo.desc;
                
                option.appendChild(canvas);
                option.appendChild(name);
                option.appendChild(desc);
                grid.appendChild(option);
            });
        }
        
        /* ========================================
           ALGORITHM SELECTION & CONFIRMATION
           ======================================== */
        function selectAlgorithm(algoName) {
            selectedAlgorithm = algoName;
            
            const options = document.querySelectorAll('.algorithm-option');
            options.forEach(opt => opt.classList.remove('selected'));
            event.currentTarget.classList.add('selected');
            
            document.getElementById('confirmAlgorithmBtn').disabled = false;
        }
        
        function confirmAlgorithm() {
            if (!selectedAlgorithm) {
                alert('Please select an algorithm!');
                return;
            }
            
            document.getElementById('algorithmSelection').style.display = 'none';
            
            const quantizedPixels = algorithmResults[selectedAlgorithm];
            processFinalImage(quantizedPixels);
        }
        
        /* ========================================
           FINAL IMAGE PROCESSING & UI UPDATE
           ======================================== */
        function processFinalImage(quantizedPixels) {
            showPreview(quantizedPixels);
            convertedBinary = generateBinary(quantizedPixels);
            
            document.getElementById('uploadBtn').style.display = 'block';
            document.getElementById('uploadBtn').disabled = false;
            document.getElementById('newImageBtn').style.display = 'block';
            document.getElementById('preview').style.display = 'block';
            
            const info = document.getElementById('info');
            info.style.display = 'block';
            info.innerHTML = `✅ Ready to upload<br>
                🎨 Algorithm: <strong>${selectedAlgorithm}</strong>`;
        }
        
        /* ========================================
           RESET & NEW IMAGE LOADING
           ======================================== */
        function loadNewImage() {
            document.getElementById('uploadArea').style.display = 'block';
            document.getElementById('preview').style.display = 'none';
            document.getElementById('info').style.display = 'none';
            document.getElementById('cropEditor').style.display = 'none';
            document.getElementById('algorithmSelection').style.display = 'none';
            document.getElementById('uploadBtn').style.display = 'none';
            document.getElementById('uploadBtn').disabled = true;
            document.getElementById('newImageBtn').style.display = 'none';
            document.getElementById('fileInput').value = '';
            convertedBinary = null;
            originalImage = null;
            selectedAlgorithm = null;
            algorithmResults = {};
            backgroundColor = 'white';
            cropRotation = 0;
            
            document.getElementById('bgWhite').classList.add('selected');
            document.getElementById('bgBlack').classList.remove('selected');
        }
        
        function findClosestColor(r, g, b) {
            let minDist = Infinity;
            let closestIndex = 0;
            for (let i = 0; i < COLORS.length; i++) {
                const [cr, cg, cb] = COLORS[i];
                const dist = Math.sqrt(Math.pow(r-cr,2) + Math.pow(g-cg,2) + Math.pow(b-cb,2));
                if (dist < minDist) {
                    minDist = dist;
                    closestIndex = i;
                }
            }
            return closestIndex;
        }
        
        function floydSteinbergDithering(pixels, width, height) {
            const quantized = new Uint8Array(width * height);
            const rgbData = new Uint8ClampedArray(pixels.length);
            
            for (let i = 0; i < pixels.length; i++) {
                rgbData[i] = pixels[i];
            }
            
            for (let y = 0; y < height; y++) {
                for (let x = 0; x < width; x++) {
                    const idx = (y * width + x) * 4;
                    const r = rgbData[idx];
                    const g = rgbData[idx + 1];
                    const b = rgbData[idx + 2];
                    
                    const colorIndex = findClosestColor(r, g, b);
                    quantized[y * width + x] = colorIndex;
                    
                    const [newR, newG, newB] = COLORS[colorIndex];
                    const errR = r - newR;
                    const errG = g - newG;
                    const errB = b - newB;
                    
                    const distributeError = (dx, dy, factor) => {
                        const nx = x + dx;
                        const ny = y + dy;
                        if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                            const nidx = (ny * width + nx) * 4;
                            rgbData[nidx] += errR * factor;
                            rgbData[nidx + 1] += errG * factor;
                            rgbData[nidx + 2] += errB * factor;
                        }
                    };
                    
                    distributeError(1, 0, 7/16);
                    distributeError(-1, 1, 3/16);
                    distributeError(0, 1, 5/16);
                    distributeError(1, 1, 1/16);
                }
            }
            return quantized;
        }
        
        function atkinsonDithering(pixels, width, height) {
            const quantized = new Uint8Array(width * height);
            const rgbData = new Uint8ClampedArray(pixels.length);
            
            for (let i = 0; i < pixels.length; i++) {
                rgbData[i] = pixels[i];
            }
            
            for (let y = 0; y < height; y++) {
                for (let x = 0; x < width; x++) {
                    const idx = (y * width + x) * 4;
                    const r = rgbData[idx];
                    const g = rgbData[idx + 1];
                    const b = rgbData[idx + 2];
                    
                    const colorIndex = findClosestColor(r, g, b);
                    quantized[y * width + x] = colorIndex;
                    
                    const [newR, newG, newB] = COLORS[colorIndex];
                    const errR = r - newR;
                    const errG = g - newG;
                    const errB = b - newB;
                    
                    const distributeError = (dx, dy, factor) => {
                        const nx = x + dx;
                        const ny = y + dy;
                        if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                            const nidx = (ny * width + nx) * 4;
                            rgbData[nidx] += errR * factor;
                            rgbData[nidx + 1] += errG * factor;
                            rgbData[nidx + 2] += errB * factor;
                        }
                    };
                    
                    distributeError(1, 0, 1/8);
                    distributeError(2, 0, 1/8);
                    distributeError(-1, 1, 1/8);
                    distributeError(0, 1, 1/8);
                    distributeError(1, 1, 1/8);
                    distributeError(0, 2, 1/8);
                }
            }
            return quantized;
        }
        
        function orderedDithering(pixels, width, height) {
            const quantized = new Uint8Array(width * height);
            
            const bayerMatrix = [
                [0, 8, 2, 10],
                [12, 4, 14, 6],
                [3, 11, 1, 9],
                [15, 7, 13, 5]
            ];
            
            for (let y = 0; y < height; y++) {
                for (let x = 0; x < width; x++) {
                    const idx = (y * width + x) * 4;
                    let r = pixels[idx];
                    let g = pixels[idx + 1];
                    let b = pixels[idx + 2];
                    
                    const threshold = (bayerMatrix[y % 4][x % 4] / 16 - 0.5) * 50;
                    r = Math.max(0, Math.min(255, r + threshold));
                    g = Math.max(0, Math.min(255, g + threshold));
                    b = Math.max(0, Math.min(255, b + threshold));
                    
                    const colorIndex = findClosestColor(r, g, b);
                    quantized[y * width + x] = colorIndex;
                }
            }
            return quantized;
        }
        
        function noDithering(pixels, width, height) {
            const quantized = new Uint8Array(width * height);
            
            for (let y = 0; y < height; y++) {
                for (let x = 0; x < width; x++) {
                    const idx = (y * width + x) * 4;
                    const r = pixels[idx];
                    const g = pixels[idx + 1];
                    const b = pixels[idx + 2];
                    
                    const colorIndex = findClosestColor(r, g, b);
                    quantized[y * width + x] = colorIndex;
                }
            }
            return quantized;
        }
        
        function blackAndWhiteDithering(pixels, width, height) {
            const quantized = new Uint8Array(width * height);
            const rgbData = new Uint8ClampedArray(pixels.length);
            
            for (let i = 0; i < pixels.length; i++) {
                rgbData[i] = pixels[i];
            }
            
            for (let y = 0; y < height; y++) {
                for (let x = 0; x < width; x++) {
                    const idx = (y * width + x) * 4;
                    const r = rgbData[idx];
                    const g = rgbData[idx + 1];
                    const b = rgbData[idx + 2];
                    
                    const gray = (r * 0.299 + g * 0.587 + b * 0.114);
                    
                    const colorIndex = gray > 127 ? 1 : 0;
                    quantized[y * width + x] = colorIndex;
                    
                    const targetGray = colorIndex === 0 ? 0 : 255;
                    const error = gray - targetGray;
                    
                    const distributeError = (dx, dy, factor) => {
                        const nx = x + dx;
                        const ny = y + dy;
                        if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                            const nidx = (ny * width + nx) * 4;
                            const errorAmount = error * factor;
                            rgbData[nidx] += errorAmount;
                            rgbData[nidx + 1] += errorAmount;
                            rgbData[nidx + 2] += errorAmount;
                        }
                    };
                    
                    distributeError(1, 0, 7/16);
                    distributeError(-1, 1, 3/16);
                    distributeError(0, 1, 5/16);
                    distributeError(1, 1, 1/16);
                }
            }
            return quantized;
        }
        
        function showPreview(quantizedPixels) {
            const canvas = document.getElementById('previewCanvas');
            canvas.width = TARGET_WIDTH;
            canvas.height = TARGET_HEIGHT;
            const ctx = canvas.getContext('2d');
            const imageData = ctx.createImageData(TARGET_WIDTH, TARGET_HEIGHT);
            
            for (let i = 0; i < quantizedPixels.length; i++) {
                const [r, g, b] = COLORS[quantizedPixels[i]];
                const idx = i * 4;
                imageData.data[idx] = r;
                imageData.data[idx + 1] = g;
                imageData.data[idx + 2] = b;
                imageData.data[idx + 3] = 255;
            }
            ctx.putImageData(imageData, 0, 0);
        }
        
        function generateBinary(quantizedPixels) {
            const buffer = new Uint8Array(TARGET_WIDTH * TARGET_HEIGHT / 2);
            let bufferIndex = 0;
            for (let y = 0; y < TARGET_HEIGHT; y++) {
                for (let x = 0; x < TARGET_WIDTH; x += 2) {
                    const p1 = quantizedPixels[y * TARGET_WIDTH + x] & 0x0F;
                    const p2 = quantizedPixels[y * TARGET_WIDTH + x + 1] & 0x0F;
                    buffer[bufferIndex++] = (p1 << 4) | p2;
                }
            }
            return buffer;
        }
        
        async function uploadToDisplay() {
            if (!convertedBinary) {
                alert('No image to upload!');
                return;
            }
            
            const progressBar = document.getElementById('progressBar');
            const progressFill = document.getElementById('progressFill');
            const uploadBtn = document.getElementById('uploadBtn');
            
            progressBar.style.display = 'block';
            uploadBtn.disabled = true;
            
            try {
                const formData = new FormData();
                const blob = new Blob([convertedBinary], { type: 'application/octet-stream' });
                formData.append('file', blob, 'image.bin');
                
                const xhr = new XMLHttpRequest();
                
                xhr.upload.addEventListener('progress', (e) => {
                    if (e.lengthComputable) {
                        const percent = Math.round((e.loaded / e.total) * 100);
                        progressFill.style.width = percent + '%';
                        progressFill.textContent = percent + '%';
                    }
                });
                
                xhr.addEventListener('load', () => {
                    if (xhr.status === 200) {
                        progressFill.textContent = '✓ Complete!';
                        setTimeout(() => {
                            progressBar.style.display = 'none';
                            alert('✅ Image uploaded successfully!');
                            uploadBtn.disabled = false;
                        }, 1500);
                    } else {
                        throw new Error('Upload failed');
                    }
                });
                
                xhr.addEventListener('error', () => {
                    throw new Error('Network error');
                });
                
                xhr.open('POST', '/upload');
                xhr.send(formData);
                
            } catch (error) {
                alert('❌ Error: ' + error.message);
                progressBar.style.display = 'none';
                uploadBtn.disabled = false;
            }
        }
    </script>
</body>
</html>
)rawliteral";

#endif