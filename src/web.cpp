#include "web.h"

Web::Web() : server(80) {}

Web::~Web() {}

void Web::begin()
{
    server.on("/", [this]()
              {
        String html = R"rawliteral(
<!DOCTYPE html>
<html lang="vi">

<head>
    <meta charset="UTF-8" />
    <meta name="viewport"
        content="width=device-width, initial-scale=1.0, user-scalable=no, maximum-scale=1.0, minimum-scale=1.0" />
    <title>Điều Khiển ESP32</title>

    <style>
        html,
        body {
            margin: 0;
            padding: 0;
            background-color: #1a1a1a;
            color: white;
            font-family: Arial, sans-serif;
            height: 100%;
            width: 100%;
            overflow: hidden;
        }

        .main {
            position: relative;
            width: 100vw;
            height: 100vh;
        }

        #run-slider {
            position: absolute;
            left: 5%;
            top: 40%;
            transform: rotate(-90deg) translateX(-50%);
            transform-origin: left center;
            width: 80vh;
            height: 100px;
            -webkit-appearance: none;
            background: #444;
            border-radius: 15px;
        }

        #run-slider::-webkit-slider-thumb {
            -webkit-appearance: none;
            width: 100px;
            height: 100px;
            background: #28a745;
            border-radius: 50%;
            cursor: pointer;
        }

        .right-panel {
            position: absolute;
            left: 20%;
            top: 0;
            width: 78%;
            height: 100%;
            display: flex;
            flex-direction: column;
            justify-content: space-between;
            align-items: center;
            padding: 20px;
            box-sizing: border-box;
        }

        .switch {
            position: relative;
            display: inline-block;
            width: 60px;
            height: 34px;
        }

        .switch input {
            opacity: 0;
            width: 0;
            height: 0;
        }

        .slider {
            position: absolute;
            cursor: pointer;
            top: 0;
            left: 0;
            right: 0;
            bottom: 0;
            background-color: #ccc;
            transition: .4s;
        }

        .slider:before {
            position: absolute;
            content: "";
            height: 26px;
            width: 26px;
            left: 4px;
            bottom: 4px;
            background-color: white;
            transition: .4s;
        }

        input:checked+.slider {
            background-color: #2196F3;
        }

        input:checked+.slider:before {
            transform: translateX(26px);
        }

        .slider.round {
            border-radius: 34px;
        }

        .slider.round:before {
            border-radius: 50%;
        }

        .control-box {
            padding: 5px 30px;
            background-color: #444;
            border-radius: 15px;
        }

        .turn-slider {
            width: 100%;
            height: 100px;
            -webkit-appearance: none;
            background: #444;
            border-radius: 15px;
        }

        .turn-slider::-webkit-slider-thumb {
            -webkit-appearance: none;
            width: 100px;
            height: 100px;
            background: #28a745;
            border-radius: 50%;
            cursor: pointer;
        }

        .line-status {
            display: flex;
            justify-content: space-around;
            align-items: center;
            width: 150px;
            padding: 20px 10px;
            background-color: #444;
            border-radius: 15px;
        }

        .line-item {
            width: 10px;
            height: 50px;
            border: 2px solid #28a745;
            border-radius: 10px;
            display: flex;
            align-items: end;
        }

        .line-value {
            width: 10px;
            border-radius: 10px;
        }
    </style>
</head>

<body>
    <div class="main">
        <!-- Thanh trượt dọc -->
        <input id="run-slider" type="range" min="-100" max="100" value="0" disabled />

        <!-- Khu vực bên phải -->
        <div class="right-panel">
            <div class="control-box">
                <div style="display: flex; align-items: center;">
                    <h3 style="margin-right: 10px;">Connect the robot:</h3>
                    <label class="switch">
                        <input id="ws-toggle" type="checkbox" />
                        <span class="slider round"></span>
                    </label>
                </div>
            </div>
            <div class="line-status">
                <div class="line-item">
                    <div id="line1" class="line-value"></div>

                </div>
                <div class="line-item">
                    <div id="line2" class="line-value"></div>

                </div>
                <div class="line-item">
                    <div id="line3" class="line-value"></div>
                </div>
                <div class="line-item">
                    <div id="line4" class="line-value"></div>
                </div>
                <div class="line-item">
                    <div id="line5" class="line-value"></div>
                </div>


            </div>

            <!-- Thanh trượt ngang -->
            <input class="turn-slider" type="range" min="-100" max="100" value="0" disabled />
        </div>
    </div>

    <script>




        let lowLine = [0, 0, 0, 0, 0];
        let highLine = [0, 0, 0, 0, 0];
        let ws = null;
        let debounceTimer;
        const wsToggle = document.getElementById("ws-toggle");
        const runSlider = document.getElementById("run-slider");
        const turnSlider = document.querySelector(".turn-slider");
        const line = [
            document.getElementById("line1"),
            document.getElementById("line2"),
            document.getElementById("line3"),
            document.getElementById("line4"),
            document.getElementById("line5")
        ];
        function setSlidersEnabled(enabled) {
            runSlider.disabled = !enabled;
            turnSlider.disabled = !enabled;
        }
        const setLineValue = (line) => {
            line.forEach((lineItem, index) => {
                value = Math.max(lowLine[index], Math.min(value, highLine[index]));
                const persen = (value - lowLine[index]) / (highLine[index] - lowLine[index]) * 100;
                lineItem.style.height = `${persen}%`;
                lineItem.style.backgroundColor = persen > 50 ? "#dfd446" : "#28a745";
            });
        };
        const lineInit = (data) => {
            lowLine = data.lowLine;
            highLine = data.highLine;
            setLineValue(data.line)

        }
        function resetSliders() {
            runSlider.value = 0;
            turnSlider.value = 0;
        }

        function debounce(func, delay) {
            return function (...args) {
                clearTimeout(debounceTimer);
                debounceTimer = setTimeout(() => {
                    func(...args);
                }, delay);
            };
        }

        function sendSliderData() {
            if (ws && ws.readyState === WebSocket.OPEN) {
                const data = {
                    run: parseInt(runSlider.value),
                    turn: parseInt(turnSlider.value)
                };
                ws.send(JSON.stringify(data));
            }
        }

        function disconnectWS() {
            if (ws) {
                ws.close();
                ws = null;
            }
            setSlidersEnabled(false);
            resetSliders();
            wsToggle.checked = false;
        }

        wsToggle.addEventListener("change", () => {
            if (wsToggle.checked) {
                ws = new WebSocket("ws://192.168.4.1:81"); 
                ws.onopen = () => {
                    console.log("WebSocket connected");
                    setSlidersEnabled(true);
                };

                ws.onmessage = (event) => {
                    console.log("Received:", event.data);
                    const data = JSON.parse(event.data);
                    switch (data.type) {
                        case "line":
                            setLineValue(data.value);
                            break;
                        case "connect":
                            lineInit(data.value);
                            break;
                    }

                };

                ws.onclose = () => {
                    console.warn("WebSocket closed");
                    disconnectWS();
                };

                ws.onerror = (err) => {
                    console.error("WebSocket error:", err);
                    disconnectWS();
                };
            } else {
                disconnectWS();
            }
        });

        // Gửi dữ liệu khi kéo slider
        [runSlider, turnSlider].forEach(slider => {
            slider.addEventListener("input", sendSliderData);
            slider.addEventListener("change", () => {
                slider.value = 0;
                sendSliderData();
            });
        });
        window.addEventListener("load", () => {
            document.body.addEventListener("click", () => {
                const el = document.documentElement;
                if (el.requestFullscreen) el.requestFullscreen();
                else if (el.webkitRequestFullscreen) el.webkitRequestFullscreen();
            }, { once: true });

            if (screen.orientation && screen.orientation.lock) {
                screen.orientation.lock("landscape").catch(() => {
                    console.warn("Thiết bị không hỗ trợ lock landscape.");
                });
            }
        });
    </script>
</body>

</html>
)rawliteral";

        server.send(200, "text/html", html); });

    server.begin();
}

void Web::handle()
{
    server.handleClient();
}
