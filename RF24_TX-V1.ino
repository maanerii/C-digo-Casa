/* 
 * https://blogmasterwalkershop.com.br/arduino/como-usar-com-arduino-modulo-transceptor-wireless-2-4ghz-nrf24l01
 * https://components101.com/wireless/nrf24l01-pinout-features-datasheet
 *  Ligação dos pinos:

    Vcc -> 3V3;
    GND -> GND;
    CE -> 9;
    CSN -> 10;
    SCK -> 13;
    MOSI -> 11;
    MISO -> 12;
    IRQ -> NÃO SERÁ UTILIZADO
*/ 

#include <SPI.h> 
#include <nRF24L01.h> 
#include <RF24.h> 

RF24 radio(9, 10);                  //CRIA UMA INSTÂNCIA UTILIZANDO OS PINOS (CE, CSN)     
const byte endereco[6] = "00001";   //CRIA UM ENDEREÇO PARA ENVIO DOS DADOS (O TRANSMISSOR E O RECEPTOR DEVEM SER CONFIGURADOS COM O MESMO ENDEREÇO)

int pinoBotao = 2;                  //PINO DIGITAL UTILIZADO PELO BOTÃO
int pinoLed = 3;                    // PINO DIGITAL DO LED
int estadoBotao = 0;                // VARIÁVEL PARA ARMAZENAR O ESTADO DO BOTÃO

void setup() {
  pinMode(pinoBotao, INPUT_PULLUP); // DEFINE O PINO COMO ENTRADA / "_PULLUP" É PARA ATIVAR O RESISTOR INTERNO
  pinMode(pinoLed, OUTPUT);         // DEFINE O PINO COMO SAIDA / LIGAR O LED
  
  // ABERTURA DO RADIO PARA ENVIAR
  radio.begin();                   // INICIALIZA A COMUNICAÇÃO SEM FIO
  radio.openWritingPipe(endereco); // DEFINE O ENDEREÇO PARA ENVIO DE DADOS AO RECEPTOR
  radio.setPALevel(RF24_PA_HIGH);  // DEFINE O NÍVEL DO AMPLIFICADOR DE POTÊNCIA
  radio.stopListening();           // DEFINE O MÓDULO COMO TRANSMISSOR (NÃO RECEBE DADOS)
}

void loop(){

  if(digitalRead(pinoBotao) == HIGH){                   // SE A LEITURA DO PINO FOR IGUAL A HIGH, FAZ
    digitalWrite(pinoLed, LOW);                         // DESLIGA O LED
    estadoBotao = 1;                                    // REINICIALIZA O VALOR DA VARIÁVEL
    radio.write(&estadoBotao, sizeof(estadoBotao));     // ENVIA AO RECEPTOR OS DADOS
    
  }else{                                                // SENÃO, FAZ
        if(digitalRead(pinoBotao) == LOW){              // SE A LEITURA DO PINO FOR IGUAL A LOW, FAZ
        digitalWrite(pinoLed, HIGH);                    // LIGA O LED
        estadoBotao = 0;                                // VARIÁVEL RECEBE VALOR 0
        radio.write(&estadoBotao, sizeof(estadoBotao)); // ENVIA AO RECEPTOR OS DADOS
        delay(300);
        }
  }
}
