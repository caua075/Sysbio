#include <SPI.h>
#include <MFRC522.h>
#include "WiFiEsp.h" //INCLUSÃO DA BIBLIOTECA MÓDULO WIFI
#include "SoftwareSerial.h"

#define SS_PIN 10
#define RST_PIN 9
const int buzzer = 8; 
//const int motor = 9; 
char st[20];
String readString = String(2);
bool printWebData = true;
unsigned long beginMicros, endMicros;
unsigned long byteCount = 0;


SoftwareSerial Serial1(3, 2); // RX, TX

char ssid[] = "exemplo";            // your network SSID (name) - Nome da Rede Wi-fi
char pass[] = "exemplo";        // your network password - Senha da Rede Wi-fi

IPAddress servidor_pagina(XXX,XXX,X,X); //Endereço IP do servidor - http_site
IPAddress placa_local(XXX,XXX,X,X); //IP do ESP8266
IPAddress myDns(XXX, XXX, X, X);
MFRC522 mfrc522(SS_PIN, RST_PIN);
int status = WL_IDLE_STATUS; //STATUS TEMPORÁRIO ATRIBUÍDO QUANDO O WIFI É INICIALIZADO E PERMANECE ATIVO ATÉ QUE O NÚMERO DE TENTATIVAS EXPIRE (RESULTANDO EM WL_NO_SHIELD) OU QUE UMA CONEXÃO SEJA ESTABELECIDA (RESULTANDO EM WL_CONNECTED)
  
// Site remoto - Coloque aqui os dados do site que vai receber a requisição GET
const char http_site[] = "XXX.XXX.X.X";
const int http_port = XX;
WiFiEspClient client;

String endereco_pagina = "exemplo/index.php"; //Endereco onde está o site que recebe os dados e grava no banco de dados
String parametros  = "";

int outputValue = 0;
unsigned long lastConnectionTime = 0;
const unsigned long postingInterval = 10*1000;

//------------------------------------------------------ SETUP ---------------------------------------

void setup() {

  Serial.begin(9600);
  SPI.begin(); 
  Serial1.begin(9600);
  mfrc522.PCD_Init();
  Serial.println("Aproxime o seu cartao/TAG do leitor");
  Serial.println();

    WiFi.init(&Serial1);
   // WiFi.config(IPAddress(placa_local));

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }

  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }
  delay(1000);
}


//------------------------------------------------------ FIM SETUP ---------------------------------------
//-------------------------------------------------------- LOOP ------------------------------------------
void loop() {
        int l = 0; 
        char c = client.read();
       while(client.available()){
    //Serial.write(c);
          String line = client.readStringUntil('\r');
          l = l+1;
//        Serial.println(line);
          //Serial.println(l);
          if (l==9){
            line.replace("\n", "");
            line.replace(" ", "");
              Serial.print(line + "\n");        
            if (line=="0"){
              Serial.print("Usuário Não Encontrado \n \n");
              tone(buzzer,5000); 
              delay(2000);     
              noTone(buzzer);
              
            }
            
            if (line=="1"){
              Serial.print("Usuário Encontrado \n \n");
              tone(buzzer,1000); 
              delay(500);     
              noTone(buzzer);
              //outputValue = map(0, 0, 1023, 0, 255);
           //   analogWrite(motor, outputValue);
            //  analogWrite(9, 200);
               
           }
          }
        }


  if (millis() - lastConnectionTime > postingInterval) {
    httpRequest();
  }



}
//-------------------------------------------------------- FIM LOOP -------------------------------------------------

//-------------------------------------------------------- VOID httpRequest ------------------------------------------
void httpRequest() {

if ( ! mfrc522.PICC_IsNewCardPresent()){
    return;
  }
  // Seleciona um catão a ser lido
  if ( ! mfrc522.PICC_ReadCardSerial()){
    return;
  }

Serial.print("ID da tag:");

  
  tone(buzzer,2000);  
  delay(100); 
 
  noTone(buzzer);
 
  
  String conteudo = "";
  byte letra;
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    conteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Mensagem : ");
  conteudo.toUpperCase();
  
  //client.stop();
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  
 //if ( !client.connect(http_site, http_port) ) {
  if (client.connect(http_site, http_port) == true){
    Serial.print("connected to ");
    Serial.println(client.remoteIP());
   String a;
   a = String(conteudo);
   a.replace(" ", "");
    
     String param = "cod=" + String(a);
    Serial.println(param);
    client.println("GET exemplo/index.php?" + param + " HTTP/1.1");
    client.println("Host: XXX.XXX.X.X");
    client.println("Connection: close");
    client.println();
  
    
    lastConnectionTime = millis();
  
    
  } else {
    Serial.println("connection failed");
  }
}
//-------------------------------------------------------- FIM VOID httpRequest ------------------------------------------
