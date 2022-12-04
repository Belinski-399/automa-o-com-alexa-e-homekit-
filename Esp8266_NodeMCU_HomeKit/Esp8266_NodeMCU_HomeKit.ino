/*
 *     Criado: 2022-12-4
 *      Autor: M.Belinski e B.Rafael
 */
#include <Arduino.h>
#include <arduino_homekit_server.h>
#include "wifi_info.h"

#define LOG_D(fmt, ...)   printf_P(PSTR(fmt "\n") , ##__VA_ARGS__);

void setup() {
	Serial.begin(115200);
	wifi_connect(); // no wifi_info.h
	//homekit_storage_reset(); // para remover o armazenamento de emparelhamento do HomeKit anterior ao executar este novo exemplo do HomeKit pela primeira vez
	my_homekit_setup();
}

void loop() {
	my_homekit_loop();
	delay(10);
}
// acessa a char do HomeKit definidas em my_accessory.c
extern "C" homekit_server_config_t config;
extern "C" homekit_characteristic_t cha_switch_on;

static uint32_t next_heap_millis = 0;

#define PIN_SWITCH 16

//Chamado quando o valor do switch é alterado pelo iOS Home APP
void cha_switch_on_setter(const homekit_value_t value) {
	bool on = value.bool_value;
	cha_switch_on.value.bool_value = on;	//sincroniza o valor
	LOG_D("Switch: %s", on ? "ON" : "OFF");
	digitalWrite(PIN_SWITCH, on ? LOW : HIGH);
}

void my_homekit_setup() {
	pinMode(PIN_SWITCH, OUTPUT);
	digitalWrite(PIN_SWITCH, HIGH);

//Adicione a função .setter para obter o evento switch enviado do iOS Home APP.
// O .setter deve ser adicionado antes do arduino_homekit_setup.
// O servidor HomeKit usa o .setter_ex internamente, veja a função homekit_accessories_init.
// Talvez este seja um problema de design legacy na biblioteca original do esp-homekit,
// e não tenho motivos para modificar este "recurso".
	cha_switch_on.setter = cha_switch_on_setter;
	arduino_homekit_setup(&config);

//reporta o valor do switch para o HomeKit se ele for alterado (por exemplo, por um botão físico)
//bool switch_is_on = true/false;
//cha_switch_on.value.bool_value = switch_is_on;
//homekit_characteristic_notify(&cha_switch_on, cha_switch_on.value);
}

void my_homekit_loop() {
	arduino_homekit_loop();
	const uint32_t t = millis();
	if (t > next_heap_millis) {
		// mostra informações de heap a cada 5 segundos
		next_heap_millis = t + 5 * 1000;
		LOG_D("Free heap: %d, HomeKit clients: %d",
				ESP.getFreeHeap(), arduino_homekit_connected_clients_count());

	}
}
