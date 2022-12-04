/*
 * my_accessory.c
 * Defina o acessório em linguagem C usando a Macro em características.h
*/
#include <homekit/homekit.h>
#include <homekit/characteristics.h>

void my_accessory_identify(homekit_value_t _value) {
	printf("identificação do acessório\n");
}
// Mudar (seção HAP 8.38)
// obrigatório: ATIVADO
// opcional: NOME
// formato: bool; HAP section 9.70; escreva a função .setter para obter o evento switch enviado do iOS Home APP.
homekit_characteristic_t cha_switch_on = HOMEKIT_CHARACTERISTIC_(ON, false);
// formato: string; HAP section 9.62; comprimento máximo 64
homekit_characteristic_t cha_name = HOMEKIT_CHARACTERISTIC_(NAME, "Switch");
//Esta é basicamente a função .setter para obter o evento switch enviado do dispositivo iOS da Apple.
homekit_accessory_t *accessories[] = {
    HOMEKIT_ACCESSORY(.id=1, .category=homekit_accessory_category_switch, .services=(homekit_service_t*[]) {
        HOMEKIT_SERVICE(ACCESSORY_INFORMATION, .characteristics=(homekit_characteristic_t*[]) {
            HOMEKIT_CHARACTERISTIC(NAME, "Switch"),
            HOMEKIT_CHARACTERISTIC(MANUFACTURER, "Arduino HomeKit"),
            HOMEKIT_CHARACTERISTIC(SERIAL_NUMBER, "0123456"),
            HOMEKIT_CHARACTERISTIC(MODEL, "ESP8266/ESP32"),
            HOMEKIT_CHARACTERISTIC(FIRMWARE_REVISION, "1.0"),
            HOMEKIT_CHARACTERISTIC(IDENTIFY, my_accessory_identify),
            NULL
        }),
		HOMEKIT_SERVICE(SWITCH, .primary=true, .characteristics=(homekit_characteristic_t*[]){
			&cha_switch_on,
			&cha_name,
			NULL
		}),
        NULL
    }),
    NULL
};
//Você pode usar um código QR para emparelhar com acessórios. 
//Para habilitar esse recurso, você precisa configurar o acessório para usar a senha estática e definir algum ID de configuração:
// Para mais informações como gera um QR sync, segue o link https://github.com/maximkulkin/esp-homekit
homekit_server_config_t config = {
		.accessories = accessories,
		.password = "111-11-111"
};
