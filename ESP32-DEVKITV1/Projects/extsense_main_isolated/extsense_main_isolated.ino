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

void setup() {
  
 Serial.begin(115200);                               // Inicialização da velocidade de transmissão
 pinMode(vibraCall,OUTPUT);                          // Configuração do pino vibracall
 pinMode(led_on, OUTPUT);                            // Configura a Led de ativação do dispositivo
 pinMode(button,INPUT);                              // Configuração do botão de ativação do dispositivo
 pinMode(sensTrigger,OUTPUT);                        // Configura o pino TRIGGER como SAÍDA (TRANSMISSOR)
 pinMode(sensEcho,INPUT);                            // Configura o pino ECHO como ENTRADA  (RECEPTOR)
 
}
void loop() {
 Extsense_main(); //FUNÇÃO QUE SE REFERE AO FUNCIONAMENTO DO DISPOSITIVO
}
void Extsense_main(){                                 //FUNÇÃO PRINCIPAL DO SENSOR 
    
     if (digitalRead(button) == HIGH){                //SE O BOTÃO FOR PRESSIONADO
     delay(400);
     
     if(turn_on_off == HIGH){
     turn_on_off = LOW; 
     Serial.println("Dispositivo desativado!");       //IMPRIME NA TELA QUE O DISPOSITVO FOI DESLIGADO
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
   else if(distance > 150){
   digitalWrite(vibraCall,LOW);
   }
   digitalWrite(led_on,LOW);                           //MANTÉM A LED INDICADOR DE ATIVAÇÃO DESATIVADA CASO O DISPOSITIVO NÃO TENHA SIDO ATIVADO COM O PRESSIONAMENTO DO BOTÃO
}
long calc_distanceCm(long pulse_calcCm){               //FUNÇÃO QUE RECEBE OS VALORES DA VARIÁVEL "pulse_signal" E RETORNA O CALCULO COM OS VALORES ARMAZENADOS NA VARIAVEL "distanceCm"
  float distanceCm = pulse_calcCm * SOUND_VELOCITY /2; //CALCULA OS VALORES ARMAZENADOS NA VARIAVEL pulse_calcCm COM O VALOR DA CONSTANTE SOUND_VELOCITY, PARA ENTREGAR O RESULTADO DE DISTANCIAMENTO DO OBJETO
  Serial.print("Distancia do objeto : ");              //IMPRIME NA TELA "Distancia do objeto : "
  Serial.print(distanceCm);                            //IMPRIME OS VALORES DO CALCULO ARMAZENADOS NA VARIAVEL "distanceCm"
  Serial.println(" cm ");
  return distanceCm;                                   //RETORNA OS VALORES CALCULADOS PARA A FUNÇÃO PRINCIPAL 
}
