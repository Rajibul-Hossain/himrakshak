#include "esp_camera.h"
#include <WiFi.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include "esp_http_server.h"
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
#define SERVO_PIN 14
#define PWM_CHANNEL 2 
#define PWM_FREQ 50  
#define PWM_RES 16    
// UDP Settings
WiFiUDP udp;
const int udpPort = 8888;
char packetBuffer[255];
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22
httpd_handle_t stream_httpd = NULL;

void setServoAngle(int angle) {
  // Map angle (0-180) to duty cycle for 16-bit resolution (0-65535)
  // 0 deg = 1ms = ~5% duty cycle = 3276
  // 180 deg = 2ms = ~10% duty cycle = 6553
  uint32_t duty = map(angle, 0, 180, 3276, 6553);
  ledcWrite(PWM_CHANNEL, duty);
}

// MJPEG Stream Handler (Runs automatically on a separate background task)
static esp_err_t stream_handler(httpd_req_t *req) {
  camera_fb_t * fb = NULL;
  esp_err_t res = ESP_OK;
  size_t _jpg_buf_len = 0;
  uint8_t * _jpg_buf = NULL;
  char * part_buf[64];

  res = httpd_resp_set_type(req, "multipart/x-mixed-replace;boundary=123456789000000000000987654321");
  if (res != ESP_OK) return res;

  while (true) {
    fb = esp_camera_fb_get();
    if (!fb) {
      Serial.println("Camera capture failed");
      res = ESP_FAIL;
    } else {
      _jpg_buf_len = fb->len;
      _jpg_buf = fb->buf;
    }
    
    if (res == ESP_OK) {
      size_t hlen = snprintf((char *)part_buf, 64, "\r\n--123456789000000000000987654321\r\nContent-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n", _jpg_buf_len);
      res = httpd_resp_send_chunk(req, (const char *)part_buf, hlen);
    }
    if (res == ESP_OK) {
      res = httpd_resp_send_chunk(req, (const char *)_jpg_buf, _jpg_buf_len);
    }
    if (fb) {
      esp_camera_fb_return(fb);
      fb = NULL;
      _jpg_buf = NULL;
    }
    if (res != ESP_OK) break;
  }
  return res;
}