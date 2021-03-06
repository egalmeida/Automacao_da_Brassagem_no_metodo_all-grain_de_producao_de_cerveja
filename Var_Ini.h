//Definiçoes de Aacesso MQTT
#define mqttServer "maqiatto.com"
#define mqttPort 1883
#define mqttUser "USUARIO DO BROKER"
#define mqttPassword "SENHA DO BROKER"
#define mqttTopic "SEU TOPICO/Projeto_TCC"

//Definições dos topicos MQTT
// mensagens  
#define Topico_Mensagem "SEU TOPICO/Projeto_TCC/Mensagem"
#define Topico_MensagemP "SEU TOPICO/Projeto_TCC/MensagemP"
#define Topico_DisplayS1 "SEU TOPICO/Projeto_TCC/DisplayS1"
#define Topico_DisplayS2 "SEU TOPICO/Projeto_TCC/DisplayS2"
#define Topico_DisplayS3 "SEU TOPICO/Projeto_TCC/DisplayS3"
//Botão
#define Topico_BtLigar "SEU TOPICO/Projeto_TCC/BtLigar"
#define Topico_BtLigar_Status "SEU TOPICO/Projeto_TCC/BtLigarStatus"

//Leds
#define Topico_LedLigar "SEU TOPICO/Projeto_TCC/LedLigar"
#define Topico_LedBomba "SEU TOPICO/Projeto_TCC/LedBomba"

#define Topico_LedR1 "SEU TOPICO/Projeto_TCC/LedR1"
#define Topico_LedR2 "SEU TOPICO/Projeto_TCC/LedR2"
#define Topico_LedR3 "SEU TOPICO/Projeto_TCC/LedR3"
#define Topico_LedV1 "SEU TOPICO/Projeto_TCC/LedV1"
#define Topico_LedV2 "SEU TOPICO/Projeto_TCC/LedV2"
#define Topico_LedV3 "SEU TOPICO/Projeto_TCC/LedV3"

//Define cores dos Leds
const int Led_Lig = 2;
const int Led_Des = 4;

//Declarações dos pinos que serão utilizados - GPIO//Pino do esp32

const int RL_Buzzer = 12; // 19

//Definição da panela 1
const int RL_R1 = 16; // 6
const int RL_V1 = 17; // 7
const int SensorP1 = 15; // 3
const float vTemperaturaP1 = 30; //Graus Celsius

//Definição da panela 2
const int RL_R2 = 18; // 9
const int RL_V2 = 19; // 10
const int RL_Bomba = 21; // 11
const int SensorP2 = 14; // 20
const float vTemperaturaP2 = 30; //Graus Celsius


//Definição da panela 3
const int RL_R3 = 23; // 15
const int RL_V3 = 22; // 14
const int SensorP3 = 4; // 5
const float vTemperaturaP3 = 30; //Graus Celsius
