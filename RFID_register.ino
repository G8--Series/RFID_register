#include <SPI.h>         // needed for Arduino versions later than 0018
#include <Ethernet.h>
#include <EthernetUdp.h>         // UDP library from: bjoern@cs.stanford.edu 12/30/2008


// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {  
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 177);

unsigned int localPort = 8888;      // local port to listen on

// buffers for receiving and sending data
char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet,
char  ReplyBuffer[] = "acknowledged";       // a string to send back
// An EthernetUDP instance to let us send and receive packets over UDP
EthernetUDP Udp;

void S_UDP(){
  // start the Ethernet and UDP:
  Ethernet.begin(mac,ip);
  Udp.begin(localPort);

}

void UDP_RX(){
  // if there's data available, read a packet
  int packetSize = Udp.parsePacket();
  if(packetSize)
  {
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    Serial.print("From ");
    IPAddress remote = Udp.remoteIP();
    for (int i =0; i < 4; i++)
    {
      Serial.print(remote[i], DEC);
      if (i < 3)
      {
        Serial.print(".");
      }
    }

    // read the packet into packetBufffer
    Udp.read(packetBuffer,UDP_TX_PACKET_MAX_SIZE);
    Serial.println("Contents:");
    Serial.println(packetBuffer);
    comando=packetBuffer;
  }
  delay(10); 
}


void impresion(int tipo){
  Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
  Serial.println();

  switch(tipo){
  case 1:

    for(int f=0;f<21;f++){
      for(int c=0;c<3;c++){
        Serial.print(Registro[f][c]+" | ");  
        delay(10);
        Udp.print(Registro[f][c]+" | ");
      }
      Serial.println();
      Udp.println();
      if(Registro[f+1][0].equals("")){
        f=21;
      }
    }
    break;

  case 2:
    String Consulta[3][2]={
      "Usuario","Cant"                                };
    Consulta[1][0]=Usuarios[0];
    Consulta[2][0]=Usuarios[1];
    int contA=0;
    int contB=0;

    for(int f=0;f<21;f++){

      if(Registro[f][0]==Usuarios[0]){
        contA++;
      }
      else{
        if(Registro[f][0]==Usuarios[1]){
          contB++;
        }
      }

      if(Registro[f+1][0].equals("")){
        f=21;
        Consulta[1][1]="  "+(String)contA+" ";
        Consulta[2][1]="  "+(String)contB+" ";
        for(int i=0;i<3;i++){
          for(int c=0;c<2;c++){
            Serial.print(Consulta[i][c]+" | ");
            delay(10);
            Udp.print(Consulta[i][c]+" | ");
          }
          Serial.println();
          Udp.println();
        }

      }
    }

    break;

  }
  mensaje="";
  Udp.endPacket();
}


void UDP_TX(String mensaje){
  // send a reply, to the IP address and port that sent us the packet we received

  Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
  Udp.print(mensaje);
  Udp.endPacket();
}

