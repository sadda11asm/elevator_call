#include <SPI.h>
#include <Ethernet.h>
#include <Servo.h>

int devices[]={2,3};
#define MAX_STRING_LEN  20
char tagStr[MAX_STRING_LEN] = "";
char dataStr[MAX_STRING_LEN] = "";
char tmpStr[MAX_STRING_LEN] = "";
char endTag[3] = {'<', '/', '\0'};
int len;
int count=0;
boolean tagFlag = false;
boolean dataFlag = false;
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0D, 0xFE, 0xD2 };
IPAddress ip(10,1,235,208); 
byte gateway[] = { 192,168,0,1 }; 
IPAddress myDns(192, 168, 0, 1);


Servo myservo;  // create servo object to control a servo
Servo myservo2;

char server[] = "elevator.nudl.kz";

byte subnet[]={255,255,255,0};
EthernetClient client;
void setup()
{
Serial.begin(9600);
Ethernet.begin(mac);
//Ethernet.begin(mac, ip, myDns);
 for(int i=0;i<2;i++)
  {
   pinMode(devices[i],OUTPUT); 
  }
  myservo.attach(6); 
  myservo2.attach(5); 
}
void loop()
{
  if (client.connect(server, 80)) {  //starts client connection, checks for connection
    Serial.println("connected");
    client.println("GET /test.xml HTTP/1.1");
    client.println( "Host: elevator.nudl.kz");
    client.println(); 
    Serial.print(server);
    Serial.println("/test.xml");
   } 
  else {
    Serial.println("connection failed"); //error message if no client connect
    Serial.println();
   }
   while(client.connected() && !client.available()) delay(1); //waits for data
   while (client.connected() || client.available())
  { 
    xmlread();
  }


}


void xmlread() {
  
char inChar = client.read();
 
  if (inChar == '<') {
     addChar(inChar, tmpStr);
     tagFlag = true;
     dataFlag = false;
   } 
  
  
  else if (inChar == '>') {
     addChar(inChar, tmpStr);
     if (tagFlag) {      
        strncpy(tagStr, tmpStr, strlen(tmpStr)+1);
     }
     clearStr(tmpStr);
     tagFlag = false;
     dataFlag = true;         
   } 
  

  else if (inChar != 10)
  {
    if (tagFlag) 
     {
       addChar(inChar, tmpStr);
        if ( tagFlag && strcmp(tmpStr, endTag) == 0 )
         {
           clearStr(tmpStr);
           tagFlag = false;
           dataFlag = false;
         }
      }
      if (dataFlag) {
        addChar(inChar, dataStr);
     }
   }
   
   
   if (inChar == 10 ) {
    if (matchTag("<state>")) {
        
	Serial.print("state: ");
        Serial.println(dataStr);
        Serial.print("length--");
        Serial.println(strlen(dataStr));
        count=count+1;
        Serial.println(count);
       devicescontrol(count,dataStr);
       if(count==10)
       {
         count=0;
         
         clearStr(tmpStr);
         clearStr(tagStr);
         clearStr(dataStr);
         client.stop();
         client.flush();
         Serial.println("disconnecting.");
       }
        
     }
     clearStr(tmpStr);
     clearStr(tagStr);
     clearStr(dataStr);
     tagFlag = false;
     dataFlag = false;
  }
    


}


void clearStr (char* str) {
  int len = strlen(str);
  for (int c = 0; c < len; c++) {
     str[c] = 0;
  }
}


void addChar (char ch, char* str) {
  const char *tagMsg  = "<ERROR>";
  const char *dataMsg = "-ERROR";

 
  if (strlen(str) > MAX_STRING_LEN - 2) {
     if (tagFlag) {
        clearStr(tagStr);
        strcpy(tagStr,tagMsg);
     }
     if (dataFlag) {
        clearStr(dataStr);
        strcpy(dataStr,dataMsg);
     }
     clearStr(tmpStr);
     tagFlag = false;
     dataFlag = false;

  } 
  else {
     str[strlen(str)] = ch;
  }
}

boolean matchTag (const char* searchTag) {
  if ( strcmp(tagStr, searchTag) == 0 ) {
     return true;
  } else {
     return false;
  }
}


void devicescontrol(int devicescount, char *devicestate)
{
  if(!strncmp("ON",devicestate,2))
  {
    Serial.print("on..........on......");
    Serial.println(devicescount);
    if (devicescount-1==0) {
      myservo.write(20);
      delay(1000);
      myservo.write(90);
    } else {
      myservo2.write(10);
      delay(1000);
      myservo2.write(90);
    }
    digitalWrite(devices[devicescount-1],HIGH);
  }
  else if(!strncmp("OFF",devicestate,3))
  {
    Serial.print("off......off.......");
    Serial.println(devicescount);
    digitalWrite(devices[devicescount-1],LOW);
  }
  
} 
