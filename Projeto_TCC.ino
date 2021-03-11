#include "Var_Ini.h"

//Declaração das funções 
void wifi_init();
void BrokerIni();
void BrokerLoop();
void Processos();

void setup() 
{
  //Inicializa saida serial  
    Serial.begin(115200);

  //Inicializa Wifi
    wifi_init();

  //Conecta ao Broker
    BrokerIni();
}

void loop() 
{
  //Chama o loop da rotina
   BrokerLoop();
   
   //executa processos
   Processos();
}
