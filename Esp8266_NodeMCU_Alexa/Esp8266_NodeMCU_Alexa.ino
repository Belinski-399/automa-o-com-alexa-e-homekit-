/*
 *     Criado: 2022-12-4
 *      Autor: M.Belinski e B.Rafael
 */
#include <Arduino.h>
#include <IRremoteESP8266.h> //INCLUSÃO DE BIBLIOTECA
#include <IRsend.h>
 
IRsend irsend(14); //FUNÇÃO RESPONSÁVEL PELO MÉTODO DE ENVIO DO SINAL IR / UTILIZA O GPIO14(D5)
      const uint32_t kPanasonicPower = 0x100BCBD;
      const uint32_t kPanasonicVolume = 0x1000405;
      const uint32_t kPanasonicVolum = 0x1008485;
      const uint16_t kPanasonicAddress = 0x4004;   // Panasonic endereço (Pre data)
#ifdef ESP32
#include <WiFi.h>
//#define RELAY_PIN_1 12
//#define RELAY_PIN_2 14

#else
#include <ESP8266WiFi.h>
#define RELAY_PIN_1 D1
#define RELAY_PIN_2 D2

#endif
#include "fauxmoESP.h"

#define SERIAL_BAUDRATE 115200

#define WIFI_SSID "BELINSKI"
#define WIFI_PASS "@gugu1970"

#define LAMP_1 "TV"
#define LAMP_2 "Volume"
#define LAMP_3 "LED 2"
#define LAMP_4 "LED 2"

fauxmoESP fauxmo;

// Wi-Fi Conexão
void wifiSetup() {
  // Define o como STA
  WiFi.mode(WIFI_STA);
  // Conecta
  Serial.printf("[WIFI] Conectado ao %s ", WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  // Espera
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
  Serial.println();
  // Conectado
  Serial.printf("[WIFI] STATION Mode, SSID: %s, IP address: %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());
}

void setup() {
  // Inicia a Serial
  Serial.begin(SERIAL_BAUDRATE);
  Serial.println();
  irsend.begin();

  // Conexão Wi-Fi
  wifiSetup();

  // Para configuração de estado de um LED
 // pinMode(RELAY_PIN_1, OUTPUT);
  //digitalWrite(RELAY_PIN_1, LOW);

  //pinMode(RELAY_PIN_2, OUTPUT);
  //digitalWrite(RELAY_PIN_2, LOW);



  // Por padrão, fauxmoESP cria seu próprio servidor web na porta definida
  // A porta TCP deve ser 80 para dispositivos gen3 (o padrão é 1901)
  // Isso deve ser feito antes da chamada enable()
  fauxmo.createServer(true); // não é necessário, este é o valor padrão
  fauxmo.setPort(80); // Isso é necessário para dispositivos gen3

  // Você deve chamar enable(true) assim que tiver uma conexão WiFi
  // Você pode ativar ou desativar a biblioteca a qualquer momento
  // Desativá-lo impedirá que os dispositivos sejam descobertos e trocados
  fauxmo.enable(true);

  // Você pode usar diferentes maneiras de chamar a Alexa para modificar o estado dos dispositivos:
  // "Alexa, ligar TV"
  // "Alexa, desligar TV"

  // Adiciona os dispositivos
  fauxmo.addDevice(LAMP_1);
  fauxmo.addDevice(LAMP_2);



  fauxmo.onSetState([](unsigned char device_id, const char * device_name, bool state, unsigned char value) {
    // Retorno de chamada quando um comando da Alexa é recebido.
    // Você pode usar device_id ou device_name para escolher o elemento no qual realizar uma ação (relé, LED, ...)
    // O state é um booleano (ON / OFF) e value um número de 0 a 255 (se você disser "definir a luz da cozinha para 50%", receberá 128 aqui).

    Serial.printf("[MAIN] Device #%d (%s) state: %s value: %d\n", device_id, device_name, state ? "ON" : "OFF", value);
    if ( (strcmp(device_name, LAMP_1) == 0) ) {
      Serial.println("RELAY 1 switched by Alexa");
      //digitalWrite(RELAY_PIN_1, !digitalRead(RELAY_PIN_1));
      if (state) {
               irsend.sendPanasonic(kPanasonicAddress, kPanasonicPower);  // PARÂMETROS NECESSÁRIOS PARA ENVIO DO SINAL IR
        Serial.println("Comando enviado: Liga / Desliga");
        delay(50); // TEMPO(EM MILISEGUNDOS) DE INTERVALO ENTRE UM COMANDO E OUTRO
        } else {
               irsend.sendPanasonic(kPanasonicAddress, kPanasonicPower);  // PARÂMETROS NECESSÁRIOS PARA ENVIO DO SINAL IR
        Serial.println("Comando enviado: Liga / Desliga");
        delay(50); // TEMPO(EM MILISEGUNDOS) DE INTERVALO ENTRE UM COMANDO E OUTRO
        }
    }
    if ( (strcmp(device_name, LAMP_2) == 0) ) {
      // this just sets a variable that the main loop() does something about
      Serial.println("RELAY 2 switched by Alexa");
       if (state) {
               irsend.sendPanasonic(kPanasonicAddress, kPanasonicVolume);  // PARÂMETROS NECESSÁRIOS PARA ENVIO DO SINAL IR
        Serial.println("Comando enviado: Liga / Desliga");
        delay(50); // TEMPO(EM MILISEGUNDOS) DE INTERVALO ENTRE UM COMANDO E OUTRO
        } else {
               irsend.sendPanasonic(kPanasonicAddress, kPanasonicVolum);  // PARÂMETROS NECESSÁRIOS PARA ENVIO DO SINAL IR
        Serial.println("Comando enviado: Liga / Desliga");
        delay(50); // TEMPO(EM MILISEGUNDOS) DE INTERVALO ENTRE UM COMANDO E OUTRO
        }
    }
       
  });

}

void loop() {
  // fauxmoESP usa um servidor TCP assíncrono, mas um servidor UDP sincronizado
  // Portanto, temos que pesquisar manualmente os pacotes UDP
  fauxmo.handle();

  static unsigned long last = millis();
  if (millis() - last > 5000) {
    last = millis();
    Serial.printf("[MAIN] Free heap: %d bytes\n", ESP.getFreeHeap());
  }

}
