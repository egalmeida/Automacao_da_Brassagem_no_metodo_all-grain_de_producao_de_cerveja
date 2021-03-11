#include <WiFi.h>

#define ssid "****"
#define password "****"

//inicializa wifi//
void wifi_init()
{
  WiFi.begin(ssid, password);
  Serial.print("Iniciando conexao com a rede WiFi");
  
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
   
    Serial.println();
    Serial.println("Conectado na rede WiFi!");
    Serial.print("Endereço IP: ");
    Serial.println(WiFi.localIP());
}
