//O CODIGO CONSISTE EM TESTAR O PROTOCOLO DE UM APARELHO COM INFRAVERMELHO POR DADO SERIAL.
#include <Arduino.h>
#include <IRremoteESP8266.h> //INCLUSÃO DE BIBLIOTECA
#include <IRsend.h>
 
IRsend irsend(14); //FUNÇÃO RESPONSÁVEL PELO MÉTODO DE ENVIO DO SINAL IR / UTILIZA O GPIO14(D5)
 //PARA TRABALHAR COM DADOS RAW, APENAS TIRAR OS COMENTARIOS E DEFINIR O TAMANHO DO BLOCO E FREQUÊNCIA
//int tamanho = 100; //TAMANHO DA LINHA RAW(100 BLOCOS)
//int frequencia = 48; //FREQUÊNCIA DO SINAL IR(48KHz)

const uint32_t kPanasonicPower = 0x100BCBD; // Panasonic desligar/ligar TV
const uint16_t kPanasonicAddress = 0x4004;   // Panasonic Endereço (Pre data)

// DADOS EM RAW
// BOTÃO LIGA / DESLIGA
//unsigned int LD[100] = {3550, 1650,450, 400,500, 1250,500, 350,450, 400,450, 400,500, 400,450, 400,450, 400,450, 400,450, 400,500, 400,450, 400,450, 400,450, 1300,450, 400,450, 400,450, 400,450, 400,500, 400,450, 400,450, 400,450, 400,450, 400,500, 1250,450, 400,500, 350,500, 400,450, 400,450, 400,500, 400,450, 400,500, 350,500, 1250,450, 400,450, 1300,450, 1250,500, 1250,450, 1250,500, 400,450, 400,500, 1200,500, 400,450, 1250,500, 1250,450, 1250,450, 1300,450, 400,450, 1250,500}; //COLE A LINHA RAW CORRESPONDENTE DENTRO DAS CHAVES
// BOTÃO MUTE
//unsigned int MT[100] = {3500 ,1700 ,450 ,400 ,500 ,1250 ,450 ,400 ,450 ,400 ,500 ,400 ,450 ,400 ,450 ,400 ,500 ,400 ,450 ,400 ,450 ,400 ,500 ,400 ,450 ,400 ,450 ,400 ,500 ,1250 ,450 ,400 ,450 ,400 ,500 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,500 ,400 ,450 ,400 ,450 ,1250 ,500 ,350 ,500 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,500 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,500 ,400 ,450 ,400 ,450 ,1250 ,500 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,450 ,400 ,500 ,400 ,450 ,400 ,450 ,1250 ,500 ,400 ,450 ,1250 ,500}; //COLE A LINHA RAW CORRESPONDENTE DENTRO DAS CHAVES
// BOTÃO PLAY / PAUSE
//unsigned int PP[100] = {3500,1700,500,400,450,1250,500,400,450,400,450,400,450,400,450,400,500,350,500,400,450,400,450,400,450,400,500,400,450,1250,500,350,450,400,500,400,450,400,450,400,450,400,450,400,500,400,450,400,450,1250,500,400,450,400,450,400,450,400,450,400,500,400,450,400,450,400,450,1250,500,400,450,400,450,400,500,400,450,1250,500,400,450,400,450,1250,500,350,500,400,450,400,450,400,450,1300,450,400,450,1250,500}; //COLE A LINHA RAW CORRESPONDENTE DENTRO DAS CHAVES
// BOTÃO VOLTAR
//unsigned int VT[100] = {3550,1650,500,400,450,1250,500,350,500,400,450,400,500,350,500,350,500,350,500,400,450,400,450,400,500,350,500,350,500,1250,450,400,500,400,450,400,500,350,500,400,450,400,500,350,500,400,450,400,500,1200,500,400,450,400,500,350,500,350,500,350,500,400,450,400,500,350,450,400,500,350,500,1250,500,350,450,1300,450,1250,500,400,450,400,500,350,500,350,500,1250,500,350,500,1250,450,1250,500,400,450,1250,450}; //COLE A LINHA RAW CORRESPONDENTE DENTRO DAS CHAVES
// BOTÃO ADIANTAR
//unsigned int AT[100] = {3550,1650,500,350,500,1250,450,400,450,400,500,400,450,400,450,400,450,400,500,350,500,400,450,400,450,400,450,400,450,1300,450,400,450,400,450,400,500,400,450,400,450,400,450,400,450,400,500,400,450,1250,450,400,500,400,450,400,450,400,500,400,450,400,450,400,500,400,450,1250,500,350,500,1250,500,350,450,1300,450,1250,450,400,500,400,450,1250,500,400,450,1250,450,400,500,1250,450,1250,500,400,450,1250,500}; //COLE A LINHA RAW CORRESPONDENTE DENTRO DAS CHAVES

void setup(){
  irsend.begin(); //INICIALIZA A FUNÇÃO
  Serial.begin(115200); //INICIALIZA A PORTA SERIAL
}
void loop(){
//INICIAR O COMANDO SERIAL   
    char c = Serial.read(); //VARIÁVEL RESPONSÁVEL POR RECEBER O CARACTER DIGITADO NA JANELA SERIAL
    
    if (c == 'a'){ //SE CARACTER DIGITADO FOR IGUAL A "a", FAZ
        irsend.sendPanasonic(kPanasonicAddress, kPanasonicPower);  // PARÂMETROS NECESSÁRIOS PARA ENVIO DO SINAL IR
        Serial.println("Comando enviado: Liga / Desliga");
        delay(50); // TEMPO(EM MILISEGUNDOS) DE INTERVALO ENTRE UM COMANDO E OUTRO
    }
}
