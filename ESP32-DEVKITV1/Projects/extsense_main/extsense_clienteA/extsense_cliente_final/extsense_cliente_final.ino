#include <SPI.h>
#include <ESP8266WiFi.h>          // The Basic Function Of The ESP NODEMCU
//------------------------------------------------------------------------------------
// Definindo os pinos I/O (Configuração)
//------------------------------------------------------------------------------------
#define   LedBoard   2            //LED do módulo WIFI                 
#define   SOS_BUTTON 16           //D0 = (GPIO16) - Botão NodeMCU 
#define   SOUND_VELOCITY 0.034    //343 m/s: Corresponde a 1234,8 Km/h em 20 Graus Celsius

const int button = 5;             //D1 = (GPIO5)  Botão liga e desliga     
const int led_on = 4;             //D2 = (GPIO4)  Led indicador de ativação do dispositivo
const int vibraCall = 13;         //D7 = (GPIO13) Motor de vibração 
const int sensTrigger = 14;       //D5 = (GPIO5) Pino do sensor TRIGGER (TRANSMISSOR DE FREQUENCIAS PARA OS OBJETOS)
const int sensEcho = 12;          //D6 = (GPIO4) Pino do sensor ECHO (RECEPTOR QUE RECEBE O PULSO DAS FREQUENCIAS RETORNADAS DO OBJETO )

long  pulse_signal;               //VARIAVEL QUE ARMAZENARÁ O TEMPO DE PERCURSO DAS ONDAS SONORAS DO SENSOR, 
                                  //QUE É O TEMPO DECORRIDO DESDE O MOMENTO DA TRANSMISSÃO E RECEPÇÃO DO PULSO.
float distance;                   //VARIÁVEL QUE ARMAZENA OS VALORES DA DISTANCIA DOS OBJETOS
float tmp = 50;

int  turn_on_off  = 0;            //VARIÁVEL USADA PARA A LÓGICA DE LIGA E DESLIGA, DE UM MESMO BOTÃO
//------------------------------------------------------------------------------------
// Váriaveis de autenticação da rede WIFI
//------------------------------------------------------------------------------------
char ssid[] = "Casa_01";                               //SSID (Nome) DO ESP SERVIDOR 
char pass[] = "fvml1234";                              //A SENHA DE AUTENTICAÇÃO PARA A CONEXÃO COM O SERVIDOR
  
//------------------------------------------------------------------------------------
// WIFI Module Mode & IP
//------------------------------------------------------------------------------------
IPAddress server(192,168,10,40);                      //INDEREÇO IP FIXO DO SERVIDOR
WiFiClient client;

//====================================================================================
// Configuração dos pinos , para cada componente instalado 
//====================================================================================
void setup() {
  
  Serial.begin(115200);                                // Inicialização da velocidade de transmissão 
  pinMode(vibraCall, OUTPUT);                          // Configuração do pino vibracall
  pinMode(LedBoard, OUTPUT);                           // Configura a Led imbutida da placa
  pinMode(led_on, OUTPUT);                             // Configura a Led de ativação do dispositivo
  pinMode(SOS_BUTTON,INPUT);                           // Configuração do botão de acionamento S.O.S
  pinMode(button,INPUT);                               // Configuração do botão de ativação do dispositivo
  
  pinMode(sensTrigger,OUTPUT);                         // Configura o pino TRIGGER como SAÍDA (TRANSMISSOR)
  pinMode(sensEcho,INPUT);                             // Configura o pino ECHO como ENTRADA  (RECEPTOR)
  
  digitalWrite(LedBoard, HIGH);                        // Inicializa a Led da placa desligada 
  
 //========================================================================================================//
 // Configurações de conexão 
 //=======================================================================================================//                              
  Serial.println("");
  Serial.print("Aguardando conexao: ");
  
  WiFi.begin(ssid, pass);                             
    while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    digitalWrite(LedBoard, LOW);
    delay(250);
    digitalWrite(LedBoard, HIGH);
    delay(250);
    }
  digitalWrite(LedBoard, HIGH);
  
//------------------------------------------------------------------------------------
// Parametros de REDE - Apenas para depuração 
//------------------------------------------------------------------------------------
  Serial.println("");
  Serial.println("Cliente Conectado: extsense device");
  Serial.print("IP: ");       Serial.println(WiFi.softAPIP());
  Serial.print("SSID: ");     Serial.println(WiFi.SSID());
  Serial.print("Sinal: ");    Serial.println(WiFi.RSSI());
}
//====================================================================================//
// Função loop principal
//====================================================================================//
void loop() {
    
    Extsense_main();        //FUNÇÃO QUE SE REFERE AO FUNCIONAMENTO DO DISPOSITIVO
    ContinuousConnection(); //FUNÇÃO QUE SE REFERE AO ESTADO DE CONEXÃO DO DISPOSITIVO
}
//====================================================================================//

 void ContinuousConnection(){
  client.connect(server, 80);                                  //PORTA DE CONEXÃO COM O SERVIDOR
   ReadButton();                                               //REALIZA A LEITURA DO BOTÃO DO TRANSMISSOR
 }
//====================================================================================//
void ReadButton() {
  
  int reading = digitalRead( SOS_BUTTON );                     //LÊ O ESTADO DO BOTÃO DE S.O.S
  
  if (reading == HIGH) {                                       //SE O BOTÃO FOR PRESSIONADO:
    for(int vibfeedback = 1; vibfeedback <= 3; vibfeedback++){ //LOOP DE REPETIÇÃO PRÉ-DEFINIDO SOBRE QUANTAS VEZES O MOTOR DE VIBRAÇÃO DEVE SER ATIVADO E DESATIVADO 
    digitalWrite(vibraCall,HIGH);
    delay(800);
    digitalWrite(vibraCall,LOW);
    delay(800);
    }
    client.print("I am Transmitter\r");                        //Send messege "I am Transmitter" To Server
    delay(200);
    }else{
    ClientContinue(); 
    }
}
//====================================================================================
void ClientContinue(){
  client.println("Transmmiter");                      // ENVIA A MENSAGEM PARA O SERVIDOR
  String answer = client.readStringUntil('\r');       // RECEBEA RESPOSTA DO SERVIDOR
  client.flush();
}
void Extsense_main(){                                 //FUNÇÃO PRINCIPAL DO SENSOR 
    
     if (digitalRead(button) == HIGH){                //SE O BOTÃO FOR PRESSIONADO
     delay(400);
     
     if(turn_on_off == HIGH){
     turn_on_off = LOW; 
     Serial.println("Dispositivo desativado!");      //IMPRIME NA TELA QUE O DISPOSITVO FOI DESLIGADO
     digitalWrite(led_on,LOW);
     }
     else{
     turn_on_off = HIGH;
     }
    }
   if(turn_on_off == HIGH){                           //CONDIÇÃO QUE VERIFICA SE O ESTADO DO BOTÃO É ALTO
    
   digitalWrite(led_on,HIGH);                         //ATIVA O LED INDICADOR DE ATIVAÇÃO DO DISPOSITIVO
   
   //INICIA O PINO TRIGGER COMO BAIXO POR 5 MICROSSEGUNDO(LIMPEZA DE VALORES INICIAIS DE LEITURA)
   digitalWrite(sensTrigger,LOW);                  
   delayMicroseconds(5);
   //ATIVA O PINO TRIGGER POR 10 MICROSEGUNDOS
   digitalWrite(sensTrigger,HIGH);                  
   delayMicroseconds(10);
   digitalWrite(sensTrigger,LOW);

   pulse_signal = pulseIn(sensEcho,HIGH);             //USAMOS A FUNÇÃO pulseIn() PARA OBTER O TEMPO DE VIAGEM DA ONDA SONORA
   distance = calc_distanceCm(pulse_signal);          //Variavel que recebe e armazena os resultados calculados pela a função
  
   if(distance <= 150){
   Serial.println("Aproximação detectada!");
   digitalWrite(vibraCall,HIGH);
   delay(600);
    }
   }
   digitalWrite(vibraCall,LOW);
   digitalWrite(led_on,LOW);                           //MANTÉM A LED INDICADOR DE ATIVAÇÃO DESATIVADA CASO O DISPOSITIVO NÃO TENHA SIDO ATIVADO COM O PRESSIONAMENTO DO BOTÃO
}
long calc_distanceCm(long pulse_calcCm){               //FUNÇÃO QUE RECEBE OS VALORES DA VARIÁVEL "pulse_signal" E RETORNA O CALCULO COM OS VALORES ARMAZENADOS NA VARIAVEL "distanceCm"
  float distanceCm = pulse_calcCm * SOUND_VELOCITY /2; //CALCULA OS VALORES ARMAZENADOS NA VARIAVEL pulse_calcCm COM O VALOR DA CONSTANTE SOUND_VELOCITY, PARA ENTREGAR O RESULTADO DE DISTANCIAMENTO DO OBJETO
  Serial.print("Distancia do objeto : ");              //IMPRIME NA TELA "Distancia do objeto : "
  Serial.print(distanceCm);                            //IMPRIME OS VALORES DO CALCULO ARMAZENADOS NA VARIAVEL "distanceCm"
  Serial.println(" cm ");
  return distanceCm;                                   //RETORNA OS VALORES CALCULADOS PARA A FUNÇÃO PRINCIPAL 
}
