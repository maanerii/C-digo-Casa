/* 
 * https://blogmasterwalkershop.com.br/arduino/como-usar-com-arduino-modulo-transceptor-wireless-2-4ghz-nrf24l01
 * https://components101.com/wireless/nrf24l01-pinout-features-datasheet
 * https://blog.eletrogate.com/servo-motor-para-aplicacoes-com-arduino/
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

#include <SPI.h>                  // INCLUSÃO DE BIBLIOTECA SPI
#include <nRF24L01.h>             // INCLUSÃO DE BIBLIOTECA RF24 - RADIO FREQUENCIA
#include <RF24.h>                 // INCLUSÃO DE BIBLIOTECA RF24 - RADIO FREQUENCIA
#include <Servo.h>                // INCLUSÃO DE BIBLIOTECA SERVO MOTOR
  
RF24 radio(9, 10);                // CRIA UMA INSTÂNCIA UTILIZANDO OS PINOS (CE, CSN)
const byte endereco[6] = "00001"; // CRIA UM ENDEREÇO PARA ENVIO DOS DADOS (O TRANSMISSOR E O RECEPTOR DEVEM SER CONFIGURADOS COM O MESMO ENDEREÇO)

int pinoBotao = 2;                // PINO DIGITAL UTILIZADO PELO BOTÃO
int pinoLed = 3;                  // PINO DIGITAL UTILIZADO PELO LED
int servoPin = 4;                 // Especifica o pino usado para controlar o servo

int estadoBotao = 1;              // VARIÁVEL PARA ARMAZENAR O ESTADO DO BOTÃO
int Abertura = 75;                // DEFINE O GRAU DE ABERTURA DO MOTOR
int Fechamento = 0;               // DEFINE O GRAU DE FECHAMENTO DO MOTOR
int UltimoEstado = 0;             // GUARDA O ÚLTIMO ESTADO DO MOTOR

Servo servomotor;                 // Cria objeto para controlar o servo

void setup() {
  
  pinMode(pinoLed, OUTPUT);          // DEFINE O PINO COMO SAÍDA
  pinMode(pinoBotao, INPUT_PULLUP);  // DEFINE O PINO COMO ENTRADA / "_PULLUP" É PARA ATIVAR O RESISTOR INTERNO
  digitalWrite(pinoLed, LOW);        // LED INICIA DESLIGADO

  // ABERTURA DO RADIO PARA RECEBER
  radio.begin();                      // INICIALIZA A COMUNICAÇÃO SEM FIO
  radio.openReadingPipe(0, endereco); // DEFINE O ENDEREÇO PARA RECEBIMENTO DE DADOS VINDOS DO TRANSMISSOR
  radio.setPALevel(RF24_PA_HIGH);     // DEFINE O NÍVEL DO AMPLIFICADOR DE POTÊNCIA
  radio.startListening();             // DEFINE O MÓDULO COMO RECEPTOR (NÃO ENVIA DADOS)

  // INICIAL O SERVO MOTOR E POSICIONA COMO FECHADO
  servomotor.attach(servoPin);       // Associa o pino 4 ao objeto servomotor
  servomotor.write(Fechamento);      // Inicia servomotor na posição zero
}
 
void loop(){
  if (radio.available()){                          // SE A COMUNICAÇÃO ESTIVER HABILITADA, FAZ
    radio.read(&estadoBotao, sizeof(estadoBotao)); // LÊ OS DADOS RECEBIDOS
    
  if(estadoBotao == 0){                            // SE O PARÂMETRO RECEBIDO (ESTADO ATUAL DO BOTÃO) FOR IGUAL A 0 (PRESSIONADO), FAZ
    digitalWrite(pinoLed, HIGH);                   // LIGA O LED
    ControleMotor();                               // CHAMA O CONTROLE DO MOTOR
    estadoBotao = 1;                               // REINICIALIZA A VARIAVEL DO BOTÃO
    delay(300);                                    // APLICA UMA PAUSA DE 300 MS
  }else{
        if(estadoBotao == 1){                      // SE O PARÂMETRO RECEBIDO (ESTADO ATUAL DO BOTÃO) FOR IGUAL A 1 (NÃO PRESSIONADO), FAZ
        digitalWrite(pinoLed, LOW);                // DESLIGA O LED
        }
    }
  }

  if(digitalRead(pinoBotao) == HIGH){              // SE A LEITURA DO PINO FOR IGUAL A HIGH, FAZ
     digitalWrite(pinoLed, LOW);                   // Desliga O LED
  }
  else {
    digitalWrite(pinoLed, HIGH);                   // LIGA O LED
    ControleMotor();                               // CHAMA O CONTROLE DO MOTOR
    delay(5);                                      // APLICA UMA PAUSA DE 5 MS
  }
   
  delay(5);                                        //INTERVALO DE 5 MILISSEGUNDOS
}

void ControleMotor (){                             // CONTROLE DO MOTOR
   if (UltimoEstado == 0){                         // VERIFICA O ULTIMO ESTADO DO MOTOR COMO 0 - FECHADO
    servomotor.write(Abertura);                    // ABRE O MOTOR
    UltimoEstado = 1;                              // DEFINE O ÚLTIME ESTADO DO MOTOR
    delay(300);                                    // APLICA UMA PAUSA DE 300 MS
  }else if (UltimoEstado == 1){                    // VERIFICA O ULTIMO ESTADO DO MOTOR COMO 1 - ABERTO
    servomotor.write(Fechamento);                  // FECHA O MOTOR
    UltimoEstado = 0;                              // DEFINE O ÚLTIME ESTADO DO MOTOR
    delay(300);                                    // APLICA UMA PAUSA DE 300 MS
  }
}
