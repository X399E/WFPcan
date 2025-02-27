#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <SPI.h>
#include <mcp_can.h>

// WiFi設定
const char* ssid = "your_ssid";
const char* password = "your_password";

// HTTPSサーバ設定
const char* host = "example.com";  // ホスト名のみ（"https://"は不要）
const int httpsPort = 443;

// CAN設定とピン定義
const int CAN_CS_PIN = 5;    // MCP2515のCSピン（ESP32側GPIO5）
const int CAN_INT_PIN = 4;   // MCP2515のINTピン（ESP32側GPIO4）
MCP_CAN CAN(CAN_CS_PIN);     // MCP2515オブジェクトの生成

// HTTPS送信関数
void sendPayloadOverHTTPS(String payload) {
  WiFiClientSecure client;
  // 開発中は証明書検証を無効化（本番では適切な証明書設定を）
  client.setInsecure();

  if (!client.connect(host, httpsPort)) {
    Serial.println("サーバーへの接続に失敗しました。");
    return;
  }

  // HTTP POSTリクエスト（送信先パスは適宜変更）
  String request = String("POST /path HTTP/1.1\r\n") +
                   "Host: " + host + "\r\n" +
                   "Content-Type: text/plain\r\n" +
                   "Content-Length: " + payload.length() + "\r\n" +
                   "Connection: close\r\n\r\n" +
                   payload;
  client.print(request);

  // サーバーからの応答を取得
  while(client.connected()){
    String line = client.readStringUntil('\n');
    if(line == "\r") break;
  }
  String response = client.readString();
  Serial.println("サーバーからの応答:");
  Serial.println(response);
  client.stop();
}

void setup() {
  Serial.begin(115200);

  // WiFi接続
  WiFi.begin(ssid, password);
  Serial.print("WiFi接続中");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi接続完了");

  // CANバスの初期化（500kbps、8MHzオシレータの場合）
  if (CAN.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) == CAN_OK) {
    Serial.println("CAN初期化成功！");
  } else {
    Serial.println("CAN初期化失敗！");
    while (1); // 初期化失敗時は停止
  }
  pinMode(CAN_INT_PIN, INPUT);
}

void loop() {
  // MCP2515の割り込みピンがLOWならCANメッセージ受信可能
  if (!digitalRead(CAN_INT_PIN)) {
    long unsigned int rxId;
    unsigned char len = 0;
    unsigned char rxBuf[8];
    if (CAN.readMsgBuf(&rxId, &len, rxBuf) == CAN_OK) {
      Serial.print("受信したCAN ID: 0x");
      Serial.print(rxId, HEX);
      Serial.print(" データ: ");

      // 受信データを文字列に変換（例として16進数表記）
      String payload = "";
      for (int i = 0; i < len; i++) {
        Serial.print(rxBuf[i], HEX);
        Serial.print(" ");
        // 数値を16進数の文字列に変換し、間にスペースを挿入
        payload += String(rxBuf[i], HEX);
        if (i < len - 1) {
          payload += " ";
        }
      }
      Serial.println();

      // HTTPSサーバにCANデータを送信
      sendPayloadOverHTTPS(payload);
    }
  }
  delay(10);  // 少し待機
}
