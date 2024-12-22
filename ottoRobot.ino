#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include <Otto.h>


// Pin Servo dan Buzzer
#define K_kiri 23
#define K_kanan 26
#define T_kiri 25
#define T_kanan 5
#define Buzzer 27

// Variabel Sensor Ultrasonic
// Pin Sensor Ultrasonik
#define TRIGGER_PIN 4
#define ECHO_PIN 18
#define MAX_DISTANCE 200  // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

// Variabel Telegram
const char* ssid = "realme10";
const char* password = "anTonG8xxx";
#define BOTtoken "7886515241:AAG37uGmMCVNzxtKm_cQR9F16kbHy-Rf1OI"
#define CHAT_ID "1831457098"

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);
Otto Otto;

long lastTime = 0;
int botRequestDelay = 1000;

void setup() {
  Serial.begin(115200);
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  // Menghubungkan ke Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Menghubungkan ke Wi-Fi...");
  }
  Serial.println("Terhubung ke Wi-Fi");
  bot.sendMessage(CHAT_ID, "Akses diterima", "");

  // Setup Telegram Bot
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
  
  // Setup Robot Otto
  Otto.init(K_kiri, K_kanan, T_kiri, T_kanan, true, Buzzer);
  Otto.home();  // Atur Otto ke posisi awal
}

void loop() {
  if (millis() > lastTime + botRequestDelay) {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while (numNewMessages) {
      Serial.println("Pesan baru diterima!");
      Otto.sing(S_OhOoh2);
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTime = millis();
  }
}

// Fungsi untuk menangani pesan baru dari Telegram
void handleNewMessages(int numNewMessages) {
  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    String text = bot.messages[i].text;

    if (chat_id != CHAT_ID) {
      bot.sendMessage(chat_id, "Akses tidak diizinkan.", "");
      continue;
    }

    if (text == "/start") {
      bot.sendMessage(chat_id, "Halo! Robot Otto siap menerima perintah.", "");
    } else if (text == "/ottodance") {
      // Uji getaran
      Otto.sing(S_mode1);
      Otto.shakeLeg(1, 1000, 1);  // Getar kaki kanan
      delay(500);
      Otto.sing(S_OhOoh);
      Otto.shakeLeg(1, 1000, -1);  // Getar kaki kiri
      delay(500);
      Otto.sing(S_OhOoh);

      // Uji gerakan lain (misalnya memiringkan tubuh)
      Otto.swing(2, 1000, 20);  // Ayun tubuh ke kiri dan kanan
      delay(500);
      Otto.sing(S_mode1);
      Otto.crusaito(2, 1000, 20);  // Silang tubuh ke depan dan belakang
      delay(500);
      Otto.sing(S_mode1);
      bot.sendMessage(chat_id, "Robot berdansa.", "");
    } else if (text == "/ottonyanyi") {
      Otto.sing(S_connection);
      Otto.sing(S_mode1);
      Otto.sing(S_mode2);
      Otto.sing(S_happy);
      bot.sendMessage(chat_id, "itu Nyanyian ku", "");
    } else if (text == "/ottoakting") {
      Otto.playGesture(OttoFail);
      Otto.playGesture(OttoVictory);
      Otto.home();
      delay(100);
      bot.sendMessage(chat_id, "Bagaimana akting ku ?", "");
    } else if (text == "/ottomenari") {
      Otto.swing(1, 1000, 25);
      Otto.moonwalker(1, 1000, 25, -1);
      Otto.swing(1, 1000, 25);
      Otto.sing(S_OhOoh2);
      Otto.moonwalker(1, 1000, 25, +1);
      Otto.sing(S_OhOoh);
      bot.sendMessage(chat_id, "Itu tarian Mickael Jackson ku", "");
    } else if (text == "/ottojalan") {
      Otto.walk(2, 1000, 1);  // Maju 2 langkah
      Otto.walk(2, 1000, -1);
      Otto.sing(S_happy);
      bot.sendMessage(chat_id, "Itu Gerakanku!!", "");
    } else if (text == "/ottomaju") {
      Otto.sing(S_OhOoh);
      Otto.walk(6, 1000, 1);  // Maju 2 langkah
      Otto.sing(S_happy);
      bot.sendMessage(chat_id, "OK aku maju 5 langkah nih", "");
    } else if (text == "/ottomundur") {
      Otto.sing(S_OhOoh);
      Otto.walk(5, 1000, -1);  // Mundur 5 langkahh
      Otto.sing(S_happy);
      bot.sendMessage(chat_id, "OK aku mundur 5 langkah nih", "");
    } else if (text == "/ottopenguin") {
      Otto.shakeLeg(1, 1000, 1);  // Getar kaki kanan
      Otto.sing(S_superHappy);
      Otto.shakeLeg(1, 1000, -1);  // Getar kaki kiri
      Otto.sing(S_superHappy);
      delay(800);
      Otto.shakeLeg(2, 1000, 1);  // Getar kaki kanan
      Otto.sing(S_superHappy);
      Otto.shakeLeg(2, 1000, -1);  // Getar kaki kiri
      Otto.sing(S_superHappy);
      bot.sendMessage(chat_id, "Itu Gerakan Pinguin", "");
    } else if (text == "/cek_kondisi") {
      digitalWrite(TRIGGER_PIN, LOW);
      delayMicroseconds(2);
      digitalWrite(TRIGGER_PIN, HIGH);
      delayMicroseconds(10);
      digitalWrite(TRIGGER_PIN, LOW);
      int duration_us = pulseIn(ECHO_PIN, HIGH);
      int distance_cm = 0.017 * duration_us;
      Serial.println ("Jarak :" +String(distance_cm) + "cm");

      Otto.sing(S_connection);
      bot.sendMessage(chat_id, "Jarak ke objek: " + String(distance_cm) + " cm", "");
    } else {
      bot.sendMessage(chat_id, "Perintah tidak dikenali.", "");
    }
  }
}
