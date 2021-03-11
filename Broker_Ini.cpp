#include <WiFi.h>
#include <PubSubClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "Var_Ini.h"

//Funçoes estruturadas
//void SensorIni();
void Buzzer(int bipe, int tempo);

//Declaração para publicar e assinar MQTT
boolean MQTTPublish(const char* topic, char* payload);
boolean MQTTSubscribe(const char* topicToSubscribe);

//Configuração de comunicação com o sensores
OneWire oneWireP1(SensorP1);
OneWire oneWireP2(SensorP2);
OneWire oneWireP3(SensorP3);

//Passa referência oneWire para o sensor de temperatura Dallas
DallasTemperature sensorsP1(&oneWireP1);
DallasTemperature sensorsP2(&oneWireP2);
DallasTemperature sensorsP3(&oneWireP3);

//Passa dados do wifi para conectar ao MQTT
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

//Variaveis do sistema
boolean mqttInitCompleted = false;
String clientId = "tcc";
bool *isBotaoLigar;
bool BotaoLigar_press;

char buf[640];
char Led_On[640];
char Led_Off[640];


float valord =10;
float valord1 =0;



char mensagem[30];

int ligado = 2;
int Processo = 0;

char buft[640];

float temperatureC;
float TemperaturaP1;
float TemperaturaP2;
float TemperaturaP3;

unsigned long hiProcesso2 = millis();
long tProcesso2 = 60000 * 0.5;

unsigned long hiProcesso3 = millis();
long tProcesso3 = 60000 * 1.5; //Tempo em minutos
long tProcesso3D2 = tProcesso3/2;
int Processo3Ini = 0;

unsigned long hiProcesso4 = millis();
long tProcesso4 = 60000 * 0.5;

unsigned long hiProcesso5 = millis();
long tProcesso5 = 60000 * 0.5;

unsigned long hiProcesso6 = millis();
long tProcesso6 = 60000 * 0.5;

unsigned long hiProcesso7 = millis();
long tProcesso7 = 60000 * 1.5;
int Processo7Ini = 0;


/* ------------RECEBE AS MENSAGENS DO BROKER-------------- */

void dataCallback(char* topic, byte* payload, unsigned int length)
{
  String strTopic = String((char*)topic);
  char payloadStr[length + 1];
  memset(payloadStr, 0, length + 1);
  strncpy(payloadStr, (char*)payload, length);
  String strPayload = String((char*)payloadStr);

  if (strTopic == Topico_BtLigar)
  {
    
    if (isBotaoLigar != NULL)
    {
    
      if (strPayload == "True")
      {
        *isBotaoLigar = "True";
        ligado=1;
        Processo = 1;
        //sprintf(buf, "%i",Led_On);
        MQTTPublish(Topico_LedLigar, Led_On);
      } 
      else
      {
        *isBotaoLigar = "False";
        ligado=2;
        Processo = 0;

        //Finaliza pinos desligado
        digitalWrite(RL_R1, LOW);
        digitalWrite(RL_R2, LOW);
        digitalWrite(RL_R3, LOW);
        digitalWrite(RL_V1, LOW);
        digitalWrite(RL_V2, LOW);
        digitalWrite(RL_V3, LOW);
        digitalWrite(RL_Bomba, LOW);
        
        //Finaliza topicos como desligado
        MQTTPublish(Topico_BtLigar_Status, "False");
        MQTTPublish(Topico_LedLigar, Led_Off);
        MQTTPublish(Topico_LedBomba, Led_Off);
        MQTTPublish(Topico_LedR1, Led_Off);
        MQTTPublish(Topico_LedR2, Led_Off);
        MQTTPublish(Topico_LedR3, Led_Off);
        MQTTPublish(Topico_LedV1, Led_Off);
        MQTTPublish(Topico_LedV2, Led_Off);
        MQTTPublish(Topico_LedV3, Led_Off);
        sprintf(buf, "%.2f",0);
        MQTTPublish(Topico_DisplayS1, buf);
        MQTTPublish(Topico_DisplayS2, buf);
        MQTTPublish(Topico_DisplayS3, buf);
        MQTTPublish(Topico_MensagemP, "Processo Desligado");
        MQTTPublish(Topico_Mensagem,"Para iniciar o processo novamente higienize os equipamentos, adicione agua na Panela 1 e Malte na Panela 2"); 
        
        //Buzzer de desligado
        Buzzer(1,1000);
      }  
    }
  }

  Serial.print("Função dataCallback. strTopic ");
  Serial.println(strTopic);
  //Serial.printf("Função dataCallback. Topic : [%s]\n", topic);
  Serial.printf("Função dataCallback. Payload : %s\n", payloadStr);
}

void performConnect()
{
  uint16_t connectionDelay = 5000;
  
  while (!mqttClient.connected())
  {
    Serial.printf("Conectando ao Broker...\n");
    
    if (mqttClient.connect(clientId.c_str(), mqttUser, mqttPassword))
    {
      Serial.printf("Conectado ao Broker!\n");

      //Assinaturas
      MQTTSubscribe(Topico_BtLigar);

      //Buzzer de inicio
      Buzzer(3,100);

      //Cores dos leds
      sprintf(Led_On, "%i", Led_Lig);
      sprintf(Led_Off, "%i", Led_Des);
      
      //Inicializa topicos como desligado
      MQTTPublish(Topico_BtLigar_Status, "False");
      MQTTPublish(Topico_LedLigar, Led_Off);
      MQTTPublish(Topico_LedBomba, Led_Off);
      MQTTPublish(Topico_LedR1, Led_Off);
      MQTTPublish(Topico_LedR2, Led_Off);
      MQTTPublish(Topico_LedR3, Led_Off);
      MQTTPublish(Topico_LedV1, Led_Off);
      MQTTPublish(Topico_LedV2, Led_Off);
      MQTTPublish(Topico_LedV3, Led_Off);
      sprintf(buf, "%.2f",0);
      MQTTPublish(Topico_DisplayS1, buf);
      MQTTPublish(Topico_DisplayS2, buf);
      MQTTPublish(Topico_DisplayS3, buf);
      MQTTPublish(Topico_Mensagem, "Coloque agua na PANELA 1 e Adicione malte na PANELA 2, apos pressione Botão Inicio");
      MQTTPublish(Topico_MensagemP,"Aguardando..."); 
    }
    else
    {
      Serial.printf("Erro! Falha ao conectar MQTT, rc = %d\n", mqttClient.state());
      Serial.printf("Tente novamente em %d msec.\n", connectionDelay);
      delay(connectionDelay);
    }
  }
}

//Retorna se MQTT esta conectado
boolean MQTTIsConnected()
{
  return mqttClient.connected();
}

  //Declaração do botão liga//
  void setMQTTBotaoLigar(bool *BotaoLigar)
  {
    Serial.println("Entrou em setMQTTBotaoLigar");
    isBotaoLigar = BotaoLigar;
      
    if (isBotaoLigar != NULL)
      *isBotaoLigar = false;  
  }

//Inicializa valores 
void BrokerIni()
{
  //Inicializa os pinos de saidas
  pinMode(RL_R1, OUTPUT);
  pinMode(RL_V1, OUTPUT);
  pinMode(RL_R2, OUTPUT);
  pinMode(RL_V2, OUTPUT);
  pinMode(RL_R3, OUTPUT);
  pinMode(RL_V3, OUTPUT);
  pinMode(RL_Bomba, OUTPUT);
  pinMode(RL_Buzzer, OUTPUT);
     
  //Inicializa velocidade da serial    
  Serial.println("BrokerIni!");
  
  //Conecta ao Broker
  mqttClient.setServer(mqttServer, mqttPort);
  
  //Inicializa o retorno das chamadas
  mqttClient.setCallback(dataCallback);
  
  //Define MQTT true
  mqttInitCompleted = true;
  
  //Inicializa sensor de temperatura
  sensorsP1.begin();
  sensorsP2.begin();
  sensorsP3.begin();

  //Carrega o botão de ligar
  setMQTTBotaoLigar(&BotaoLigar_press);
}

//Verifica conexão com o Broker
void BrokerLoop()
{
  if(mqttInitCompleted)
  {
  
    if (!MQTTIsConnected())
    {
      performConnect();
    }
    mqttClient.loop();
  }
}

//Executa Processos
void Processos()
{
  if (ligado == 1)
  {
    sensorsP1.requestTemperatures();
    TemperaturaP1 = sensorsP1.getTempCByIndex(0);
    sensorsP2.requestTemperatures();
    TemperaturaP2 = sensorsP2.getTempCByIndex(0);
    sensorsP3.requestTemperatures();
    TemperaturaP3 = sensorsP3.getTempCByIndex(0);

    if(Processo == 1)
    {
      sprintf(buf, "%.2f",TemperaturaP1);
      MQTTPublish(Topico_DisplayS1, buf);  
      MQTTPublish(Topico_Mensagem,"Aquecendo a agua para a Brassagem");
      MQTTPublish(Topico_MensagemP,"Aguarde... ");
      MQTTPublish(Topico_LedR1, Led_On);
     
      if (vTemperaturaP1 >= TemperaturaP1)
      {
        digitalWrite(RL_R1, HIGH);
        MQTTPublish(Topico_LedR1, Led_On);
      }
      else
      {
        digitalWrite(RL_R1, LOW);
        MQTTPublish(Topico_LedR1, Led_Off);
        digitalWrite(RL_V1, HIGH);
        MQTTPublish(Topico_LedV1, Led_On);
        Processo = 2;
        tProcesso2 += millis();
        MQTTPublish(Topico_Mensagem,"Valvula da Panela 1 aberta");
        MQTTPublish(Topico_MensagemP,"Aguarde... ");
        Buzzer(1,200);
      }
    }
    
    if(Processo == 2) // Retirar agua da Panela 1
    {
      hiProcesso2 = millis();
      sprintf(buf, "%.2f",0);
      MQTTPublish(Topico_DisplayS1, buf);
      
      if ( hiProcesso2 >= tProcesso2) //espera agua sair Panela 1
      {
        digitalWrite(RL_V1, LOW);
        MQTTPublish(Topico_LedV1, Led_Off);
        
        if (vTemperaturaP2 >= TemperaturaP2)//Aquece agua ate vTemperaturaP2 30
        {
          sprintf(buf, "%.2f",TemperaturaP2);
          MQTTPublish(Topico_DisplayS2, buf);  
          MQTTPublish(Topico_Mensagem,"Ajustando temperatura para Brassagem"); 
          Serial.println("vTemperaturaP2 >= TemperaturaP2 p2");
          digitalWrite(RL_R2, HIGH);
          MQTTPublish(Topico_LedR2, Led_On);
        }
        else //Alcançou vTemperaturaP2 passa para proximo preocesso 
        {
          Processo=3;
          Buzzer(1, 200);
        } 
      }
    }
    
    if(Processo == 3) //Brassagem
    {
      Serial.println("Processo 3");
      digitalWrite(RL_V2, HIGH);
      MQTTPublish(Topico_LedV2, Led_On);
      digitalWrite(RL_Bomba, HIGH);
      MQTTPublish(Topico_LedBomba, Led_On);
      hiProcesso3 = millis();
      MQTTPublish(Topico_Mensagem,"Brassagem iniciada");
      sprintf(buf, "%.2f",TemperaturaP2);
      MQTTPublish(Topico_DisplayS2, buf);  
      
      if (vTemperaturaP2 >= TemperaturaP2)//mantem a agua aquecida ate vTemperaturaP2 30
      {
        Serial.println("vTemperaturaP2 >= TemperaturaP2");
        digitalWrite(RL_R2, HIGH);
        MQTTPublish(Topico_LedR2, Led_On);
      }
      else //Alcançou 30 desliga 
      {
        Serial.println("vTemperaturaP2 >= TemperaturaP2 else");
        digitalWrite(RL_R2, LOW);
        MQTTPublish(Topico_LedR2, Led_Off);
      } 
      
      if (Processo3Ini==0)//Pega tempo para p3 e para aquecer agua novamente
      {
        Serial.println("Processo3Ini=0");
        tProcesso3 += millis();
        tProcesso3D2 += millis();
      }
        
      if(hiProcesso3 >= tProcesso3D2) //Preparação da agua da lavagem
      {
        Serial.println("hiProcesso3 >= tProcesso3D2");
        MQTTPublish(Topico_MensagemP,"Aquecendo agua para a Lavagem");
        sprintf(buf, "%.2f",TemperaturaP1);
        MQTTPublish(Topico_DisplayS1, buf);  
        
        if (vTemperaturaP1 >= TemperaturaP1) //Mantem aquecida a agua para lavagem
        {
          Serial.println("vTemperaturaP1 >= TemperaturaP1");
          digitalWrite(RL_R1, HIGH);
          MQTTPublish(Topico_LedR1, Led_On);
        }
        else 
        {
          Serial.println("vTemperaturaP1 >= TemperaturaP1 else");
          digitalWrite(RL_R1, LOW);
          MQTTPublish(Topico_LedR1, Led_Off);
        }
      }
     
      if(tProcesso3 >= hiProcesso3)
      {
        Serial.println("tProcesso3 >= hiProcesso3");
        Processo3Ini=1;      
      }
      else //Fim do processo de Brassagem
      {
        Serial.println("Processo 3 else");
        digitalWrite(RL_Bomba, LOW);
        MQTTPublish(Topico_LedBomba, Led_Off);
        digitalWrite(RL_R1, LOW);
        MQTTPublish(Topico_LedR1, Led_Off);
        digitalWrite(RL_V2, LOW);
        MQTTPublish(Topico_LedV2, Led_Off);
        digitalWrite(RL_R2, LOW);
        MQTTPublish(Topico_LedR2, Led_Off);
        digitalWrite(RL_V3, HIGH);
        MQTTPublish(Topico_LedV3, Led_On);
        Processo = 4;
        tProcesso4 += millis();
        MQTTPublish(Topico_MensagemP,"Fim da Brassagem"); 
        MQTTPublish(Topico_Mensagem,"Transfirindo Mosto para Panela de Fervura");      
        sprintf(buf, "%.2f",0);
        MQTTPublish(Topico_DisplayS1, buf);  
        MQTTPublish(Topico_DisplayS2, buf);  
        Buzzer(1, 200);
      }
    } //Fim processo 3 Brassagem
    
    if(Processo == 4) // Retirar agua da Panela 2 
    {
      Serial.println("Processo == 4");
      hiProcesso4 = millis();
           
      if ( hiProcesso4 >= tProcesso4) //espera agua sair Panela 2
      {
        Serial.println("hiProcesso4 >= tProcesso4");
        digitalWrite(RL_V1, HIGH);
        MQTTPublish(Topico_LedV1, Led_On);
        Processo = 5;
        tProcesso5 += millis();
        Buzzer(1, 200);
      }
    }
    
    if(Processo == 5) //Passa agua da Panela 1 para panela 2 
    {
      Serial.println("Processo == 5");
      hiProcesso5 = millis();
      MQTTPublish(Topico_MensagemP,"Lavagem do bagaço");
        
      if ( hiProcesso5 >= tProcesso5) //espera agua da Panela 1 passar para panela 2 
      {
        Serial.println("hiProcesso5 >= tProcesso5");
        digitalWrite(RL_V1, LOW);
        MQTTPublish(Topico_LedV1, Led_Off);
        Processo = 6;
        tProcesso6 += millis();
        Buzzer(1,200);
      }
    }
    
    if(Processo == 6) //Passa agua da Panela 2 para panela 3 
    {
      Serial.println("Processo == 6");
      hiProcesso6 = millis();
     
      if ( hiProcesso6 >= tProcesso6) //espera agua da Panela 2 passar para panela 3 
      {
        Serial.println("hiProcesso6 >= tProcesso6");
        digitalWrite(RL_V3, LOW);
        MQTTPublish(Topico_LedV3, Led_Off);

        MQTTPublish(Topico_MensagemP,"Lavagem finalizada");
        MQTTPublish(Topico_Mensagem,"Ajustando a temperatura para a Fervura");  
        
        if (vTemperaturaP3 >= TemperaturaP3) //Aquece a agua até vTemperatura 3 para fervura
        {
          sprintf(buf, "%.2f", TemperaturaP3);
          MQTTPublish(Topico_DisplayS3, buf);
          Serial.println("vTemperaturaP3 >= TemperaturaP3 p6");
          digitalWrite(RL_R3, HIGH);
          MQTTPublish(Topico_LedR3, Led_On);
        }
        else //Chegou a vTemperaturaP3 passa para o proximo processo
        {
          Serial.println("vTemperaturaP3 >= TemperaturaP3 p6 else");
          Processo = 7;
          tProcesso7 += millis();
          MQTTPublish(Topico_Mensagem,"Fervura iniciada"); 
          MQTTPublish(Topico_MensagemP,"Adicione Lúpulo na Panela 3"); 
          Buzzer(1,200);
        }
      }
    }

    if(Processo == 7) //Fervura
    {
      Serial.println("Processo == 7");
      hiProcesso7 = millis();
      sprintf(buf, "%.2f", TemperaturaP3);
      MQTTPublish(Topico_DisplayS3, buf);
                
      if (vTemperaturaP3 >= TemperaturaP3)
      {
        Serial.println("vTemperaturaP3 >= TemperaturaP3");
        digitalWrite(RL_R3, HIGH);
        MQTTPublish(Topico_LedR3, Led_On);
      }
      else
      {
        Serial.println("vTemperaturaP3 >= TemperaturaP3 else");
        digitalWrite(RL_R3, LOW);
        MQTTPublish(Topico_LedR3, Led_Off);
      }
      
      if (Processo7Ini=0)
      {
         tProcesso7 += millis();
      }
      
      if(tProcesso7 >= hiProcesso7)
      {
        Serial.println("tProcesso7 >= hiProcesso7");
         Processo7Ini=1;      
      }
      else //Fim do processo da fervura
      {
        Serial.println("tProcesso7 >= hiProcesso7 else");
        digitalWrite(RL_R3, LOW);
        MQTTPublish(Topico_LedR3, Led_Off);
        ligado=0;
        Buzzer(10,100);
        
        sprintf(buf, "%.2f","0");
        MQTTPublish(Topico_DisplayS1, buf);
        MQTTPublish(Topico_DisplayS2, buf);
        MQTTPublish(Topico_DisplayS3, buf);
        MQTTPublish(Topico_Mensagem, "Fim do Processo...");
        MQTTPublish(Topico_MensagemP,""); 
      }
    } //Fim processo 3 Brassagem
         
  }
  else //DESLIGA
  {
    
  } 
}

//Publica topicos no MQTT
boolean MQTTPublish(const char* topic, char* payload)
{
  boolean retval = false;
  
  if (mqttClient.connected())
  {
    retval = mqttClient.publish(topic, payload);
  }
  
  Serial.printf("Função MQTTPublish Topic: [%s]\n", topic);
  Serial.printf("Função MQTTPublish Payload : %s\n", payload);
 
  return retval;
}

//Assina topicos no MQTT
boolean MQTTSubscribe(const char* topicToSubscribe)
{
  boolean retval = false;
  
  if (mqttClient.connected())
  {
    retval = mqttClient.subscribe(topicToSubscribe);
  }
  
  Serial.printf("Função MQTTSubscribe: [%s]\n", topicToSubscribe);
 
  return retval;
}

void Buzzer(int bipe, int tempo)
{
  for (int i = 1; i <= bipe; i++) 
  {
    digitalWrite(RL_Buzzer, HIGH);
    delay(tempo);
    digitalWrite(RL_Buzzer, LOW);
    delay(tempo);
  }
}

//class Botao
//{
//  private:
//
//  public:
//    bool start_button_press;
//};
