#include <WiFi.h>
#include <WebSocketsServer.h>

// Tên và mật khẩu WiFi do ESP32 tạo ra
const char* ssid = "ESP32-AccessPoint";
const char* password = "12345678";

// Khởi tạo WebSocket server trên port 81
WebSocketsServer webSocket = WebSocketsServer(81);

// Hàm xử lý khi có tin nhắn WebSocket
void handleWebSocketMessage(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  switch(type) {
    case WStype_TEXT:
      Serial.printf("Client %u gửi: %s\n", num, payload);
      // Gửi lại phản hồi cho client
      webSocket.sendTXT(num, "ESP32 đã nhận: " + String((char*)payload));
      break;
  }
}

void setup() {
  Serial.begin(115200);

  // Cấu hình ESP32 thành AP
  WiFi.softAP(ssid, password);
  Serial.println("Đang khởi tạo Access Point...");
  Serial.print("Địa chỉ IP: ");
  Serial.println(WiFi.softAPIP());

  // Bắt đầu WebSocket server
  webSocket.begin();
  webSocket.onEvent(handleWebSocketMessage);
}

void loop() {
  webSocket.loop();
}
