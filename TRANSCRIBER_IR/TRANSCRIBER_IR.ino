/*
 *     Criado: 2022-12-4
 *      Autor: M.Belinski e B.Rafael
 */
// transcriber dos dados recebidos de um transmissor IR
#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>

// um detector/demodulador IR está conectado ao pino 2 do GPIO
uint16_t RECV_PIN = 2;

IRrecv irrecv(RECV_PIN);
decode_results results;

void setup() {
  Serial.begin(115200);
  irrecv.enableIRIn();  // Inicia o receiver
}

void dump(decode_results *results) {
    // Envia a estrutura decode_results.
   // Chama isso depois de IRrecv::decode()
   // no void dump, serve para enviar um dado para lib para encontrar qual é protocolo
   // e se esse protocolo esta na lib
   // se não ele fica em dado bruto RAW
  uint16_t count = results->rawlen;
  if (results->decode_type == UNKNOWN) {
    Serial.print("Unknown encoding: ");
  } else if (results->decode_type == NEC) {
    Serial.print("Decoded NEC: ");
  } else if (results->decode_type == SONY) {
    Serial.print("Decoded SONY: ");
  } else if (results->decode_type == RC5) {
    Serial.print("Decoded RC5: ");
  } else if (results->decode_type == RC5X) {
    Serial.print("Decoded RC5X: ");
  } else if (results->decode_type == RC6) {
    Serial.print("Decoded RC6: ");
  } else if (results->decode_type == RCMM) {
    Serial.print("Decoded RCMM: ");
  } else if (results->decode_type == PANASONIC) {
    Serial.print("Decoded PANASONIC - Address: ");
    Serial.print(results->address, HEX);
    Serial.print(" Value: ");
  } else if (results->decode_type == LG) {
    Serial.print("Decoded LG: ");
  } else if (results->decode_type == JVC) {
    Serial.print("Decoded JVC: ");
  } else if (results->decode_type == AIWA_RC_T501) {
    Serial.print("Decoded AIWA RC T501: ");
  } else if (results->decode_type == WHYNTER) {
    Serial.print("Decoded Whynter: ");
  } else if (results->decode_type == NIKAI) {
    Serial.print("Decoded Nikai: ");
  }
  serialPrintUint64(results->value, 16);
  Serial.print(" (");
  Serial.print(results->bits, DEC);
  Serial.println(" bits)");
  Serial.print("Raw (");
  Serial.print(count, DEC);
  Serial.print("): {");

  for (uint16_t i = 1; i < count; i++) {
    if (i % 100 == 0)
      yield();  // Rendimento preventivo a cada 100 entradas para alimentar o WDT.
    if (i & 1) {
      Serial.print(results->rawbuf[i] * kRawTick, DEC);
    } else {
      Serial.print(", ");
      Serial.print((uint32_t) results->rawbuf[i] * kRawTick, DEC);
    }
  }
  Serial.println("};");
}
// caso o dump esteja desatualizado, utilize o IRrecvDumpV2.ino
// nesse segundo sistema de exemplo, ele utiliza melhor a lib e coleta mais tipos de dados atualizados 
void loop() {
  if (irrecv.decode(&results)) {
    dump(&results);
    Serial.println("OBSOLETO: Por favor, use IRrecvDumpV2.ino!");
    irrecv.resume();  // Recebe o próximo valor
  }
}
