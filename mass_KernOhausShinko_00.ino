#include <SPI.h>            // Hack to use the SPI library
#include <Wire.h>
//=========================================================================
#include <iarduino_KB.h>   
iarduino_KB KB(  41, 43, 45, 39,    37, 35, 33, 31   );   
             // iarduino_KB KB(43, 41, 31, 39,      37, 35, 33, 45  );   // // - эластичная 4х4, тип KB1    http://iarduino.ru/shop/Sensory-Datchiki/klaviatura-4-4-knopki.html
             // iarduino_KB KB(31, 39, 37, 35, 33);                      // // - эластичная 1х4, тип KB4    http://iarduino.ru/shop/Expansion-payments/elastichnaya-klaviatura-4-knopki.html 
//==========================================================================
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>

//==========================================================================
#include <DS3231.h>

//==========================================================================
//#include "USBPrinter.h"
//#include "ESC_POS_Printer.h"
//#include "qrcode.h"
//==========================================================================
//#include <SoftwareSerial.h>
//#define MAX_LEN 255

//==========================================================================
#define OLED_RESET 4
Adafruit_SH1106 display(OLED_RESET);

//============= RTC DS3231 + EEPROM 24C32 32K ==============================
DS3231  rtc(SDA, SCL);

//==========================================================================
int n1=0;   int n2=0;   int n3=0;
int k1=0;   int k2=0;   
int x1=0;

//class PrinterOper : public USBPrinterAsyncOper {
//    public:
//    uint8_t OnInit(USBPrinter *pPrinter);
//};
//
//uint8_t PrinterOper::OnInit(USBPrinter *pPrinter) {
//  Serial.println(F("USB Printer OnInit"));
//  Serial.print(F("Bidirectional="));
//  Serial.println(pPrinter->isBidirectional());
//  return 0;
//} 
//========================================================================    
//USB myusb;
//PrinterOper AsyncOper;
//USBPrinter uprn (&myusb, &AsyncOper);

//====================================================================================================================================================
const int butPin0 = 30;     // Rejim
int j=-1;

//====================================================================================================================================================
char fio[17][11+1]={ "___________",   
                     "User1      ",  
                     "User2      ",   
                     "User3      ",
                     "User4      ",
                     "User5      ",
                     "User6      ",
                     "User7      ",
                     "User8      ",   
                     "User9      ",
                     "User10     ",
                     "User11     ",
                     "User12     ",
                     "User13     ",
                     "User14     ",
                     "User15     ",
                     "User16     "     };
//====================================================================================================================================================
// Parallel port  =  arduino   //Centronics Connector  
const int nStrobe =  2; //1       nStrobe (input)
const int data_0  =  3; //2       Data pin 0  (input)
const int data_1  =  4; //3       Data pin 1  (input)
const int data_2  =  5; //4       Data pin 2  (input)
const int data_3  =  6; //5       Data pin 3  (input)
const int data_4  =  7; //6       Data pin 4  (input)
const int data_5  =  8; //7       Data pin 5  (input)
const int data_6  =  9; //8       Data pin 6  (input)
const int data_7  = 10; //9       Data pin 7  (input)
const int nAck    = 11; //10      nAck        (output)
const int busy    = 12; //11      busy        (output)
                        //18-25   19-30 Ground (-)
const int strobeWait = 2;    // microseconds to strobe for

//=============================================================================================================================    
//String xop[10] = {  };
//char u1[MAX_LEN] = { 0 };
char  str1[200] = { 0 }; 
char  str2[200] = { 0 }; 
char  str3[200] = { 0 }; 
int m =5;

//_______________________________________________________________________________________________________________________________________________________________________
void setup() {
  Serial.begin(9600);
  while (!Serial) ;

  Serial.println("Hardware Serial: 9600 8N1 (USB)");
//  pinMode(0, INPUT);
//  pinMode(1, OUTPUT);
//  
//  pinMode(19, INPUT);
//  pinMode(18, OUTPUT);
//  pinMode(17, INPUT);
//  pinMode(16, OUTPUT);
//  pinMode(15, INPUT);
//  pinMode(14, OUTPUT);
  
  Serial1.begin(9600);
  Serial2.begin(9600);
  Serial3.begin(9600);

  Serial.println("Hardware Serial: 9600 8N1 (RS232)");
  Serial.println();

//---------------------------------------------------------------------------------------------------
 
//---------------------------------------------------------------------------------------------------
  digitalWrite(butPin0, LOW);
  pinMode(butPin0, INPUT);

  Serial.println("SH1106");
  display.begin(SH1106_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  delay(500);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Analytical balance !");
  display.display();
  delay(500);

//------------------------------------------------- --------------------------------------------------
  Serial.println("RTC-1");
  rtc.begin();
  //////////////////////////////rtc.setDOW(WEDNESDAY);     // Set Day-of-Week to SUNDAY
  //rtc.setDate(25, 07, 2019);    // Set the date to January 1st, 2014
  //rtc.setTime(14, 07, 00);      // Set the time to 12:00:00 (24hr format)  
  Serial.print(rtc.getDateStr( FORMAT_SHORT));
  Serial.print(" ");
  Serial.println(rtc.getTimeStr( FORMAT_SHORT));  
  Serial.println("RTC-2");
//-----------------------------------------------------------------------------
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,10);
  display.println(rtc.getDateStr( FORMAT_SHORT));
  display.setCursor(70,10);
  display.println(rtc.getTimeStr( FORMAT_SHORT));
  display.display();
  delay(500); 

  Serial.println(fio[0]);
//-------------------------------------------------------------------------------------------------------
  
  KB.begin(KB1);     // эластичная 4х4, тип KB1

//-------------------------------------------------------------------------------------------------------
// Parallel port  =  arduino   //Centronics Connector  
  pinMode(nStrobe, OUTPUT); digitalWrite(nStrobe, HIGH);     // is active LOW    // set HIGH
  pinMode(data_0,  OUTPUT);
  pinMode(data_1,  OUTPUT);
  pinMode(data_2,  OUTPUT);
  pinMode(data_3,  OUTPUT);
  pinMode(data_4,  OUTPUT);
  pinMode(data_5,  OUTPUT);
  pinMode(data_6,  OUTPUT);
  pinMode(data_7,  OUTPUT);
  pinMode(nAck,     INPUT);     // is active LOW
  pinMode(busy,     INPUT);  
  pinMode(23,       INPUT);   digitalWrite(23, HIGH);  // analog pin 0 on duemilanove and uno
  pinMode(24,      OUTPUT);
  pinMode(25,      OUTPUT);
  
  delay(1000);
  resetPrinter();
  Serial.println("Delay for 5 sec .... printer");
  delay(5000);
  Serial.println("Startup PRINTER complete");

//  if (myusb.Init()) {
//     delay( 200 );
//     Serial.println(F("USB host failed to initialize"));
//  }else{
//     Serial.println(F("USB Host init OK"));
//  }
//   uint8_t tmpbyte1;
//   tmpbyte1 = myusb.Init();
//   Serial.print("\t");
//   Serial.print(tmpbyte1, HEX);
//   Serial.print("...");
//   tmpbyte1 = myusb.regRd(rREVISION);
//   Serial.print(tmpbyte1, HEX);
//   Serial.println("");
//
//  if (uprn.isReady()) {
//    Serial.println(F("PrinterYES..."));
//  }else{
//    Serial.println("Printer Not Ready!...");
//  }
//******************************************************************************************************************************

}


//==============================================================================================================================
//=================================================  L O O P ===================================================================  
//==============================================================================================================================
void loop() {
  
//  myusb.Task();
//  
//  if (uprn.isReady()) {
//     if (k1==0){
//         Serial.println("Printer YES");
//         k1=1;
//         k2=0;
//     }     
//  }else{
//     if (k2==0){
//         Serial.println("Printer Not Ready!...");
//         k2=1;
//         k1=0;
//     }    
//  }

  //___________________________________________________________________________________

  
  if (digitalRead(butPin0) == HIGH) {    //============= LED ON  ================
      //Serial.print(  digitalRead(butPin0)  );    // turn LED on:
      //Serial.println(" - "); 
      if(KB.check(KEY_DOWN )){          // если нажимается   кнопка, то ...
         //Serial.print("\n");
         Serial.print(KB.getNum);  
         Serial.print(" = ");
         Serial.print(KB.getChar); 
         Serial.print(" ");
         
         switch (KB.getNum) {
         case 10:    //-A-
              //for (int k=0; k< m; k++){
                   //Serial.println(xop[k]);
                   //const char *chv = xop[k].c_str();
                   //prnMsg1(  chv,   xop[k].length()    );
              //}
              //resetPrinter();
              //xop[40]=String();
              //m=5;
              break;
         default:
              break;           
         }
      }          
  }else{                                 //============= LED OFF ================ 
      if(KB.check(KEY_DOWN )){          // если нажимается   кнопка, то ...
         //Serial.print("\n");
         Serial.print(KB.getNum);  
         Serial.print(" = ");
         Serial.print(KB.getChar); 
         Serial.print(" ");
         
         switch (KB.getNum) {
          case 1:   //================= 1 ================    
               if (j <= 0){
                   j++;
                   Serial.print(j);    Serial.print("\ ");    Serial.println(fio[j]);
                   display.clearDisplay();
                   display.setTextSize(2);
                   display.setCursor(100,0);
                   display.println((j));
                   display.setCursor(0,20);
                   display.println(fio[j]);
                   display.display();
                   delay(100);
                   ///////////////j++;   
               }else if (j == 17-1  ){
                   j=0;
                   Serial.print(j);    Serial.print("\ ");    Serial.println(fio[j]);
                   display.clearDisplay();
                   display.setTextSize(2);
                   display.setCursor(100,0);
                   display.println((j));
                   display.setCursor(0,20);
                   display.println(fio[j]);
                   display.display();
                   delay(100);
               }else if (j < 17-1 ){
                   j++;
                   Serial.print(j);    Serial.print("> ");    Serial.println(fio[j]);
                   display.clearDisplay();
                   display.setTextSize(2);
                   display.setCursor(100,0);
                   display.println((j));
                   display.setCursor(0,20);
                   display.println(fio[j]);
                   display.display();
                   delay(100);    
               }else{   
                   Serial.print(j);
                   Serial.print("* ");
               }
               break; 
  
          case 2:  //================= 2 ================
               if (j <= 0){
                   j=17-1;
                   Serial.print(j);   Serial.print("\ ");   Serial.println(fio[j]);
                   display.clearDisplay();
                   display.setTextSize(2);
                   display.setCursor(100,0);
                   display.println((j));
                   display.setCursor(0,20);
                   display.println(fio[j]);
                   display.display();
                   delay(100);
               }else if (j == 17  ){
                   j=0;
                   Serial.print(j);   Serial.print("\ ");   Serial.println(fio[j]);
                   display.clearDisplay();
                   display.setTextSize(2);
                   display.setCursor(100,0);
                   display.println((j));
                   display.setCursor(0,20);
                   display.println(fio[j]);
                   display.display();
                   delay(100);
               }else if (j < 17 ){
                   j--;
                   Serial.print(j);   Serial.print("< ");   Serial.println(fio[j]);
                   display.clearDisplay();
                   display.setTextSize(2);
                   display.setCursor(100,0);
                   display.println((j));
                   display.setCursor(0,20);
                   display.println(fio[j]);
                   display.display();
                   delay(100);    
               }else{   
                   Serial.print(j);
                   Serial.print("*else*");
               }
               break;

          
          
          case 10:    //-A-         +++++++++++ K E R N +++++++++
              Serial.println("+A+");
              txtA();
              break;
          case 11:    //-B-         +++++++++++ S H I N K O ++++++++++++
              Serial.println("+B+");
              txtB();
              break;
          case 12:    //-C-         +++++++++++ O H A U S ++++++++++++
              Serial.println("+C+");
              txtC();
              break;
          case 13:    //-D-
              Serial.println("+D+");
              txtD();
              break;
          case 15:    //-#-
              Serial.println("+#+");
              txt3();
              break;
          case 14:    //-*-
              Serial.println(" ***** ");
              txt5();
              break;
          default:
              break; 
          
         }
      } 
  } 
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//==============================================================================================================================                   
//==============================================================================================================================
//==============================================================================================================================
//==============================================================================================================================
//==============================================================================================================================
//==============================================================================================================================
//==============================================================================================================================





//======================================== COM PORT =======================================================                   

//=========================================== 0 ===========================================================
void serialEvent() {
    Serial.println("");
    Serial.println("----------0----------");
    char  str0[200] = { 0 };
    int   i, len = 0;
    while (Serial.available())     {
      byte c = Serial.read();      
      str0[len] = c; len++;
    }
    for (int i=0; i<len; i++)     {
      Serial.print(str0[i], HEX);
      Serial.print("|");
    }    
}

//=========================================== 1 Kern ======================================================  
void serialEvent1() {
     Serial.println("");
     Serial.print(n1);
     Serial.println(" ----------1----------");
     if (n1>=111){ n1=0; }    
                     if (n1==0) {
                                //---------------------------Kern-------------------------
                                int i = 0;
                                char b1[25] = "\r----------------------\r\n";
                                //Serial.print( "sizeof_b1 = ");        Serial.println(sizeof(b1));   
                                for ( i = 0; i < 25; i++) {
                                     str1[i] = b1[i];
                                     //Serial.print(i);
                                     //Serial.print(".");
                                     //Serial.print( str1[i]);
                                     //Serial.print("\n");
                                }
                                Serial.print( "\t\t___i === ");
                                Serial.println(i);
                        
                                //----------------------------------------------------
                                char b2[16];
                                sprintf(b2, "%s %s\r\n", rtc.getDateStr( FORMAT_SHORT), rtc.getTimeStr( FORMAT_SHORT));
                                //Serial.print( "sizeof_b2 = ");         Serial.println(sizeof(b2));
                                
                                for ( i = 25; i < 25+16; i++) {
                                     str1[i] = b2[i-25];
                                     //Serial.print(i);
                                     //Serial.print(".");
                                     //Serial.print( str1[i]);
                                     //Serial.print("\n");
                                }
                                Serial.print( "\t\t___i === ");
                                Serial.println(i);
                        
                                //----------------------------------------------------
                                char b3[34] = "Kern ABT220-5DM\r\nS/N: B012020851\r\n";
                                //Serial.print( "sizeof_b3 = ");         Serial.println(sizeof(b3));
                                for (i = 41; i < 25+16+34; i++) {
                                     str1[i] = b3[i-41];
                                     //Serial.print(i);
                                     //Serial.print(".");
                                     //Serial.print( str1[i]);
                                     //Serial.print("\n");
                                }
                                Serial.print( "\t\t___i === ");
                                Serial.println(i);
                                
                                //----------------------------------------------------
                                if (j==-1){      j=0;     }
                                char b4[8+1+11+2];
                                sprintf(b4, "%s %s\r\n", "User ID:" ,  fio[j]);
                                //Serial.print( "sizeof_b4 = ");         Serial.println(sizeof(b4 ));
                                for (i = 75; i < 25+16+34+22; i++) {
                                     str1[i] = b4[i-75];
                                     //Serial.print(i);
                                     //Serial.print(".");
                                     //Serial.print( str1[i]);
                                     //Serial.print( "\n");
                                }
                                Serial.print( "\t\t**97**i === ");
                                Serial.println(i);
                                n1=i;
                     }
     int len = 0;
     while (Serial1.available())     {
            byte c = Serial1.read();      
            if ((c > 0x1F) && (c < 0x7B)) {
                 str1[n1] = c; n1++; len++;
            }            
     }
     for (int i=97; i<n1; i++)     {
          Serial.print(str1[i], HEX);
          Serial.print(".");        
     }
          Serial.print(" = ");        
          Serial.print(len);  
          Serial.print(" = ");        
          Serial.println(n1);  
          
     if (n1>=111){
                  //0x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x
                         byte u1[]={
                              0x1B,0x25,0x2D,0x31,0x32,0x33,0x34,0x35,0x58,0x40,0x50,0x4A,0x4C,0x20,0x4A,0x4F,0x42,  0x0A,
                              0x0D,
                              0x1B,0x45,0x1B,0x2A,0x74,0x36,0x30,0x30,0x52,0x1B,0x26,0x75,0x36,0x30,0x30,0x44,0x1B,0x2A,0x72,0x30,0x46,0x1B,0x26,0x6C,0x30,0x6F,0x1B,0x26,0x6C,0x37,0x31,0x41,0x1B,0x26,0x6C,0x37,0x48,0x1B,0x26,0x6C,0x30,0x53,0x1B,0x26,0x6C,0x31,0x58,0x1B,0x26,0x6C,0x38,0x63,0x31,0x45,0x1B,0x2A,0x70,0x30,0x78,0x30,0x59,0x1B,0x2A,0x63,0x30,0x74,0x32,0x34,0x39,0x34,0x78,0x33,0x39,0x35,0x35,0x59,0x1B,0x2A,0x62,0x30,0x4D,  0x0D,
                              0x1B,0x28,0x31,0x39,0x55,0x1B,0x28,0x73,0x34,0x30,0x39,0x39,0x74,0x30,0x62,0x30,0x73,0x31,0x32,0x2E,0x30,0x30,0x68,0x30,0x50,0x1B,0x26,0x64,0x40,0x1B,0x2A,0x76,0x30,0x6F,0x30,0x54,0x1B,0x2A,0x70,0x31,0x38,0x35,0x59,0x1B,0x2A,0x70,0x31,0x32,0x30,0x58
                         };
                         
                         //     0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,
                         byte u3[]={
                              0x1B,0x2A,0x70,0x33,0x32,0x39,0x36,0x59,0x0C,0x1B,0x45,0x1B,0x25,0x2D,0x31,0x32,0x33,0x34,0x35,0x58,0x40,0x50,0x4A,0x4C,0x20,0x45,0x4F,0x4A,  0x0A,
                              0x1B,0x25,0x2D,0x31,0x32,0x33,0x34,0x35,0x58
                         };
                  
                         byte wc3[ sizeof(u1)+n1+sizeof(u3) ]={0x00};
                         int k = 0;
                         for(k=0; k<sizeof(u1); k++){
                             wc3[k] = u1[k];
                             //Serial.print(k);
                             //Serial.print(".");
                             //Serial.print(wc3[k], HEX);
                             //Serial.print("\n");        
                         }
                         Serial.print( "\t\t151___k === ");
                         Serial.println(k); 
                         
                         for(k=sizeof(u1); k<sizeof(u1)+n1; k++){
                             wc3[k] = str1[k-sizeof(u1)];
                             //Serial.print(k);
                             //Serial.print(".");
                             //Serial.print(wc3[k]);
                             //Serial.print("\n");            
                         }
                         Serial.print( "\t\t262___k === ");
                         Serial.println(k); 
                         
                         for(k=(sizeof(u1)+n1); k<sizeof(u1)+n1+sizeof(u3); k++){
                             wc3[k] = u3[k-(sizeof(u1)+n1)];
                             //Serial.print(k);
                             //Serial.print(".");
                             //Serial.print(wc3[k], HEX);
                             //Serial.print("\n");    
                         }
                         Serial.print( "\t\t**300**k === ");
                         Serial.println(k); 
                  
                         prnMsg1(wc3             , sizeof(wc3)-1 );  
                         resetPrinter();
                  //0x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x





      
            for (int k=0; k<n1; k++){
                 Serial.print(str1[k]);
            }
            
        str1[200] = { 0 }; 
        n1=0;
     } 
}

//=========================================== 2 Shinko ====================================================  
void serialEvent2() {
     Serial.println("");
     Serial.print(n2);
     Serial.println(" ----------2----------");
     if (n2>=119){ n2=0; }
                    //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
                         if (n2==0) {    
                            //--------------------------Shinko--------------------------
                            int i = 0;
                            char b1[25] = "\r----------------------\r\n";
                            //Serial.print( "sizeof_b1 = ");        Serial.println(sizeof(b1));   
                            for ( i = 0; i < 25; i++) {
                                 str2[i] = b1[i];
                                 //Serial.print(i);
                                 //Serial.print(".");
                                 //Serial.print( str2[i]);
                                 //Serial.print("\n");
                            }
                            Serial.print( "\t\t___i === ");
                            Serial.println(i);
                    
                            //----------------------------------------------------
                            char b2[16];
                            sprintf(b2, "%s %s\r\n", rtc.getDateStr( FORMAT_SHORT), rtc.getTimeStr( FORMAT_SHORT));
                            //Serial.print( "sizeof_b2 = ");         Serial.println(sizeof(b2));
                            
                            for ( i = 25; i < 25+16; i++) {
                                 str2[i] = b2[i-25];
                                 //Serial.print(i);
                                 //Serial.print(".");
                                 //Serial.print( str2[i]);
                                 //Serial.print("\n");
                            }
                            Serial.print( "\t\t___i === ");
                            Serial.println(i);
                    
                            //----------------------------------------------------
                            char b3[42] = "Shinko Denshi AJ-620CE\r\nS/N: BL131063009\r\n";
                            //Serial.print( "sizeof_b3 = ");         Serial.println(sizeof(b3));
                            for (i = 41; i < 25+16+42; i++) {
                                 str2[i] = b3[i-41];
                                 //Serial.print(i);
                                 //Serial.print(".");
                                 //Serial.print( str2[i]);
                                 //Serial.print("\n");
                            }
                            Serial.print( "\t\t___i === ");
                            Serial.println(i);
                            
                            //----------------------------------------------------
                            if (j==-1){      j=0;     }
                            char b4[8+1+11+2];
                            sprintf(b4, "%s %s\r\n", "User ID:" ,  fio[j]);
                            //Serial.print( "sizeof_b4 = ");         Serial.println(sizeof(b4 ));
                            for (i = 83; i < 25+16+42+22; i++) {
                                 str2[i] = b4[i-83];
                                 //Serial.print(i);
                                 //Serial.print(".");
                                 //Serial.print(str2[i]);
                                 //Serial.print("\n");
                            }
                            Serial.print( "\t\t**105**i === ");
                            Serial.println(i);
        n2=i;
     }
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
     
     int len = 0;
     while (Serial2.available())     {
           byte c = Serial2.read(); 
           if ((c > 0x1F) && (c < 0x7B)) {
                str2[n2] = c; n2++; len++;
           }
     }     
     for (int i=105; i<n2; i++)     {
          Serial.print(str2[i], HEX);
          Serial.print(".");        
     }
          Serial.print(" = ");        
          Serial.print(len);  
          Serial.print(" = ");        
          Serial.println(n2);  
          //105+(14÷16)
     if (n2>=119){
                  //0x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x
                         byte u1[]={
                              0x1B,0x25,0x2D,0x31,0x32,0x33,0x34,0x35,0x58,0x40,0x50,0x4A,0x4C,0x20,0x4A,0x4F,0x42,  0x0A,
                              0x0D,
                              0x1B,0x45,0x1B,0x2A,0x74,0x36,0x30,0x30,0x52,0x1B,0x26,0x75,0x36,0x30,0x30,0x44,0x1B,0x2A,0x72,0x30,0x46,0x1B,0x26,0x6C,0x30,0x6F,0x1B,0x26,0x6C,0x37,0x31,0x41,0x1B,0x26,0x6C,0x37,0x48,0x1B,0x26,0x6C,0x30,0x53,0x1B,0x26,0x6C,0x31,0x58,0x1B,0x26,0x6C,0x38,0x63,0x31,0x45,0x1B,0x2A,0x70,0x30,0x78,0x30,0x59,0x1B,0x2A,0x63,0x30,0x74,0x32,0x34,0x39,0x34,0x78,0x33,0x39,0x35,0x35,0x59,0x1B,0x2A,0x62,0x30,0x4D,  0x0D,
                              0x1B,0x28,0x31,0x39,0x55,0x1B,0x28,0x73,0x34,0x30,0x39,0x39,0x74,0x30,0x62,0x30,0x73,0x31,0x32,0x2E,0x30,0x30,0x68,0x30,0x50,0x1B,0x26,0x64,0x40,0x1B,0x2A,0x76,0x30,0x6F,0x30,0x54,0x1B,0x2A,0x70,0x31,0x38,0x35,0x59,0x1B,0x2A,0x70,0x31,0x32,0x30,0x58
                         };
                         
                         //     0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,
                         byte u3[]={
                              0x1B,0x2A,0x70,0x33,0x32,0x39,0x36,0x59,0x0C,0x1B,0x45,0x1B,0x25,0x2D,0x31,0x32,0x33,0x34,0x35,0x58,0x40,0x50,0x4A,0x4C,0x20,0x45,0x4F,0x4A,  0x0A,
                              0x1B,0x25,0x2D,0x31,0x32,0x33,0x34,0x35,0x58
                         };
                  
                         byte wc3[ sizeof(u1)+n2+sizeof(u3) ]={0x00};
                         int k = 0;
                         for(k=0; k<sizeof(u1); k++){
                             wc3[k] = u1[k];
                             //Serial.print(k);
                             //Serial.print(".");
                             //Serial.print(wc3[k], HEX);
                             //Serial.print("\n");        
                         }
                         Serial.print( "\t\t___k === ");
                         Serial.println(k); 
                         
                         for(k=sizeof(u1); k<sizeof(u1)+n2; k++){
                             wc3[k] = str2[k-sizeof(u1)];
                             //Serial.print(k);
                             //Serial.print(".");
                             //Serial.print(wc3[k]);
                             //Serial.print("\n");            
                         }
                         Serial.print( "\t\t___k === ");
                         Serial.println(k); 
                         
                         for(k=(sizeof(u1)+n2); k<sizeof(u1)+n2+sizeof(u3); k++){
                             wc3[k] = u3[k-(sizeof(u1)+n2)];
                             //Serial.print(k);
                             //Serial.print(".");
                             //Serial.print(wc3[k], HEX);
                             //Serial.print("\n");    
                         }
                         Serial.print( "\t\t**308**k === ");
                         Serial.println(k); 
                  
                         prnMsg1(wc3             , sizeof(wc3)-1 );  
                         resetPrinter();
                  //0x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x








      
            for (int k=0; k<n2; k++){
                 Serial.print(str2[k]);
            }

        str2[200] = { 0 }; 
        n2=0;
     }       
}

//=========================================== 3 Ohaus =====================================================  
void serialEvent3() {
     Serial.println("");
     Serial.print(n3);
     Serial.println("------------3----------");
     if (n3>=101){ n3=0; }     
                         if (n3==0) {
                                    //------------------------Ohaus----------------------------
                                    int i = 0;
                                    char b1[25] = "\r----------------------\r\n";
                                    //Serial.print( "sizeof_b1 = ");        Serial.println(sizeof(b1));   
                                    for ( i = 0; i < 25; i++) {
                                         str3[i] = b1[i];
                                         //Serial.print(i);
                                         //Serial.print(".");
                                         //Serial.print(str3[i]);
                                         //Serial.print("\n");
                                    }
                                    Serial.print( "\t\t___i === ");
                                    Serial.println(i);
                            
                                    //----------------------------------------------------
                                    char b2[16];
                                    sprintf(b2, "%s %s\r\n" , rtc.getDateStr( FORMAT_SHORT), rtc.getTimeStr( FORMAT_SHORT));
                                    //Serial.print( "sizeof_b2 = ");         Serial.println(sizeof(b2));
                                    
                                    for ( i = 25; i < 25+16 ; i++) {
                                         str3[i] = b2[i-25];
                                         //Serial.print(i);
                                         //Serial.print(".");
                                         //Serial.print(str3[i]);
                                         //Serial.print("\n");
                                    }
                                    Serial.print( "\t\t___i === ");
                                    Serial.println(i);
                            
                                    //----------------------------------------------------
                                    char b3[31] = "Ohaus PA214C\r\nS/N: B247553310\r\n";
                                    //Serial.print( "sizeof_b3 = ");         Serial.println(sizeof(b3));
                                    for (i = 41; i < 25+16+31 ; i++) {
                                         str3[i] = b3[i-41];
                                         //Serial.print(i);
                                         //Serial.print(".");
                                         //Serial.print(str3[i]);
                                         //Serial.print("\n");
                                    }
                                    Serial.print( "\t\t___i === ");
                                    Serial.println(i);
                                    
                                    //----------------------------------------------------
                                    if (j==-1){      j=0;     }
                                    char b4[8+1+11+2 ];
                                    sprintf(b4, "%s %s\r\n", "User ID:" ,  fio[j]);
                                    //Serial.print( "sizeof_b4 = ");         Serial.println(sizeof(b4 )); //Serial.print(b4);
                                    for (i = 72; i < 25+16+31+22 ; i++) {
                                         str3[i] = b4[i-72];
                                         //Serial.print(i);
                                         //Serial.print(".");
                                         //Serial.print(str3[i]);
                                         //Serial.print("\n");
                                    }
                                    Serial.print( "\t\t**94**i === ");
                                    Serial.println(i); 
                           n3=i;
                         }

     int  len = 0;
     while (Serial3.available())     {
            byte c = Serial3.read(); 
            if ((c > 0x1F) && (c < 0x7B)) {
                 str3[n3] = c; n3++; len++;
            }
     }
     for (int i=94; i<n3; i++)     {
          Serial.print(str3[i], HEX);
          Serial.print(".");        
     }
          Serial.print(" = ");        
          Serial.println(n3);        
     
     //---------------------------------------------------------------
     if (n3>=101){
//0x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x
       byte u1[]={
            0x1B,0x25,0x2D,0x31,0x32,0x33,0x34,0x35,0x58,0x40,0x50,0x4A,0x4C,0x20,0x4A,0x4F,0x42,  0x0A,
            0x0D,
            0x1B,0x45,0x1B,0x2A,0x74,0x36,0x30,0x30,0x52,0x1B,0x26,0x75,0x36,0x30,0x30,0x44,0x1B,0x2A,0x72,0x30,0x46,0x1B,0x26,0x6C,0x30,0x6F,0x1B,0x26,0x6C,0x37,0x31,0x41,0x1B,0x26,0x6C,0x37,0x48,0x1B,0x26,0x6C,0x30,0x53,0x1B,0x26,0x6C,0x31,0x58,0x1B,0x26,0x6C,0x38,0x63,0x31,0x45,0x1B,0x2A,0x70,0x30,0x78,0x30,0x59,0x1B,0x2A,0x63,0x30,0x74,0x32,0x34,0x39,0x34,0x78,0x33,0x39,0x35,0x35,0x59,0x1B,0x2A,0x62,0x30,0x4D,  0x0D,
            0x1B,0x28,0x31,0x39,0x55,0x1B,0x28,0x73,0x34,0x30,0x39,0x39,0x74,0x30,0x62,0x30,0x73,0x31,0x32,0x2E,0x30,0x30,0x68,0x30,0x50,0x1B,0x26,0x64,0x40,0x1B,0x2A,0x76,0x30,0x6F,0x30,0x54,0x1B,0x2A,0x70,0x31,0x38,0x35,0x59,0x1B,0x2A,0x70,0x31,0x32,0x30,0x58
       };
       
       //     0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,
       byte u3[]={
            0x1B,0x2A,0x70,0x33,0x32,0x39,0x36,0x59,0x0C,0x1B,0x45,0x1B,0x25,0x2D,0x31,0x32,0x33,0x34,0x35,0x58,0x40,0x50,0x4A,0x4C,0x20,0x45,0x4F,0x4A,  0x0A,
            0x1B,0x25,0x2D,0x31,0x32,0x33,0x34,0x35,0x58
       };

       byte wc3[ sizeof(u1)+n3+sizeof(u3) ]={0x00};
       int k = 0;
       for(k=0; k<sizeof(u1); k++){
           wc3[k] = u1[k];
           //Serial.print(k);
           //Serial.print(",0x");
           //Serial.print(wc3[k], HEX);
           //Serial.print("\n");        
       }
       Serial.print( "\t\t___k === ");
       Serial.println(k); 
       
       for(k=sizeof(u1); k<sizeof(u1)+n3; k++){
           wc3[k] = str3[k-sizeof(u1)];
           //Serial.print(k);
           //Serial.print(".");
           //Serial.print(wc3[k]);
           //Serial.print("\n");            
       }
       Serial.print( "\t\t___k === ");
       Serial.println(k); 
       
       for(k=(sizeof(u1)+n3); k<sizeof(u1)+n3+sizeof(u3); k++){
           wc3[k] = u3[k-(sizeof(u1)+n3)];
           //Serial.print(k);
           //Serial.print(",0x");
           //Serial.print(wc3[k], HEX);
           //Serial.print("\n");    
       }
       Serial.print( "\n\t\t**292**k === ");
       Serial.println(k); 

       prnMsg1(wc3             , sizeof(wc3)-1 );  
       resetPrinter();
//0x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x

       
       for (int k=0; k<n3; k++){
              Serial.print(str3[k]);
       }

        str3[200] = { 0 }; 
        n3=0;
     }
     //---------------------------------------------------------------

  
}
//-------------------------------------------------------------------------------------------






//---------------------------------------------------------------------------------------------------------------------------
///////////////////////////////////////// KERN //////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------------------------------------------------------
void txtA(){
        //---------------------------Kern-------------------------
        int i = 0;
        char b1[25] = "\r----------------------\r\n";
        Serial.print( "sizeof_b1 = ");        Serial.println(sizeof(b1));   
        for ( i = 0; i < 25; i++) {
             str1[i] = b1[i];
             //Serial.print(i);
             //Serial.print(".");
             Serial.print( str1[i]);
             //Serial.print("\n");
        }
        Serial.print( "___i === ");
        Serial.println(i);

        //----------------------------------------------------
        char b2[16];
        sprintf(b2, "%s %s\r\n", rtc.getDateStr( FORMAT_SHORT), rtc.getTimeStr( FORMAT_SHORT));
        Serial.print( "sizeof_b2 = ");         Serial.println(sizeof(b2));
        
        for ( i = 25; i < 25+16; i++) {
             str1[i] = b2[i-25];
             //Serial.print(i);
             //Serial.print(".");
             Serial.print( str1[i]);
             //Serial.print("\n");
        }
        Serial.print( "___i === ");
        Serial.println(i);

        //----------------------------------------------------
        char b3[34] = "Kern ABT220-5DM\r\nS/N: B012020851\r\n";
        Serial.print( "sizeof_b3 = ");         Serial.println(sizeof(b3));
        for (i = 41; i < 25+16+34; i++) {
             str1[i] = b3[i-41];
             //Serial.print(i);
             //Serial.print(".");
             Serial.print( str1[i]);
             //Serial.print("\n");
        }
        Serial.print( "___i === ");
        Serial.println(i);
        
        //----------------------------------------------------
        if (j==-1){      j=0;     }
        char b4[8+1+11+2];
        sprintf(b4, "%s %s\r\n", "User ID:" ,  fio[j]);
        Serial.print( "sizeof_b4 = ");         Serial.println(sizeof(b4 ));
        for (i = 75; i < 25+16+34+22; i++) {
             str1[i] = b4[i-75];
             //Serial.print(i);
             //Serial.print(".");
             Serial.print( str1[i]);
             //Serial.print( "\n");
        }
        Serial.print( "**97**i === ");
        Serial.println(i);
 
//0x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x
       byte u1[]={
            0x1B,0x25,0x2D,0x31,0x32,0x33,0x34,0x35,0x58,0x40,0x50,0x4A,0x4C,0x20,0x4A,0x4F,0x42,  0x0A,
            0x0D,
            0x1B,0x45,0x1B,0x2A,0x74,0x36,0x30,0x30,0x52,0x1B,0x26,0x75,0x36,0x30,0x30,0x44,0x1B,0x2A,0x72,0x30,0x46,0x1B,0x26,0x6C,0x30,0x6F,0x1B,0x26,0x6C,0x37,0x31,0x41,0x1B,0x26,0x6C,0x37,0x48,0x1B,0x26,0x6C,0x30,0x53,0x1B,0x26,0x6C,0x31,0x58,0x1B,0x26,0x6C,0x38,0x63,0x31,0x45,0x1B,0x2A,0x70,0x30,0x78,0x30,0x59,0x1B,0x2A,0x63,0x30,0x74,0x32,0x34,0x39,0x34,0x78,0x33,0x39,0x35,0x35,0x59,0x1B,0x2A,0x62,0x30,0x4D,  0x0D,
            0x1B,0x28,0x31,0x39,0x55,0x1B,0x28,0x73,0x34,0x30,0x39,0x39,0x74,0x30,0x62,0x30,0x73,0x31,0x32,0x2E,0x30,0x30,0x68,0x30,0x50,0x1B,0x26,0x64,0x40,0x1B,0x2A,0x76,0x30,0x6F,0x30,0x54,0x1B,0x2A,0x70,0x31,0x38,0x35,0x59,0x1B,0x2A,0x70,0x31,0x32,0x30,0x58
       };
       
       //     0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,
       byte u3[]={
            0x1B,0x2A,0x70,0x33,0x32,0x39,0x36,0x59,0x0C,0x1B,0x45,0x1B,0x25,0x2D,0x31,0x32,0x33,0x34,0x35,0x58,0x40,0x50,0x4A,0x4C,0x20,0x45,0x4F,0x4A,  0x0A,
            0x1B,0x25,0x2D,0x31,0x32,0x33,0x34,0x35,0x58
       };

       byte wc3[ sizeof(u1)+i+sizeof(u3) ]={0x00};
       int k = 0;
       for(k=0; k<sizeof(u1); k++){
           wc3[k] = u1[k];
           //Serial.print(k);
           //Serial.print(".");
           Serial.print(wc3[k], HEX);
           //Serial.print("\n");        
       }
       Serial.print( "___k === ");
       Serial.println(k); 
       
       for(k=sizeof(u1); k<sizeof(u1)+i; k++){
           wc3[k] = str1[k-sizeof(u1)];
           //Serial.print(k);
           //Serial.print(".");
           Serial.print(wc3[k]);
           //Serial.print("\n");            
       }
       Serial.print( "___k === ");
       Serial.println(k); 
       
       for(k=(sizeof(u1)+i); k<sizeof(u1)+i+sizeof(u3); k++){
           wc3[k] = u3[k-(sizeof(u1)+i)];
           //Serial.print(k);
           //Serial.print(".");
           Serial.print(wc3[k], HEX);
           //Serial.print("\n");    
       }
       Serial.print( "**282**k === ");
       Serial.println(k); 

       //prnMsg1(wc3             , sizeof(wc3)-1 );  
       //resetPrinter();
//0x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x

        str1[200] = { 0 };
        

}

//---------------------------------------------------------------------------------------------------------------------------
///////////////////////////////////////// Shinko ////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------------------------------------------------------
void txtB(){  
        //--------------------------Shinko--------------------------
        int i = 0;
        char b1[25] = "\r----------------------\r\n";
        Serial.print( "sizeof_b1 = ");        Serial.println(sizeof(b1));   
        for ( i = 0; i < 25; i++) {
             str2[i] = b1[i];
             //Serial.print(i);
             //Serial.print(".");
             Serial.print( str2[i]);
             //Serial.print("\n");
        }
        Serial.print( "___i === ");
        Serial.println(i);

        //----------------------------------------------------
        char b2[16];
        sprintf(b2, "%s %s\r\n", rtc.getDateStr( FORMAT_SHORT), rtc.getTimeStr( FORMAT_SHORT));
        Serial.print( "sizeof_b2 = ");         Serial.println(sizeof(b2));
        
        for ( i = 25; i < 25+16; i++) {
             str2[i] = b2[i-25];
             //Serial.print(i);
             //Serial.print(".");
             Serial.print( str2[i]);
             //Serial.print("\n");
        }
        Serial.print( "___i === ");
        Serial.println(i);

        //----------------------------------------------------
        char b3[42] = "Shinko Denshi AJ-620CE\r\nS/N: BL131063009\r\n";
        Serial.print( "sizeof_b3 = ");         Serial.println(sizeof(b3));
        for (i = 41; i < 25+16+42; i++) {
             str2[i] = b3[i-41];
             //Serial.print(i);
             //Serial.print(".");
             Serial.print( str2[i]);
             //Serial.print("\n");
        }
        Serial.print( "___i === ");
        Serial.println(i);
        
        //----------------------------------------------------
        if (j==-1){      j=0;     }
        char b4[8+1+11+2];
        sprintf(b4, "%s %s\r\n", "User ID:" ,  fio[j]);
        Serial.print( "sizeof_b4 = ");         Serial.println(sizeof(b4 ));
        for (i = 83; i < 25+16+42+22; i++) {
             str2[i] = b4[i-83];
             //Serial.print(i);
             //Serial.print(".");
             Serial.print(str2[i]);
             //Serial.print("\n");
        }
        Serial.print( "**105**i === ");
        Serial.println(i);
        
//0x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x
       byte u1[]={
            0x1B,0x25,0x2D,0x31,0x32,0x33,0x34,0x35,0x58,0x40,0x50,0x4A,0x4C,0x20,0x4A,0x4F,0x42,  0x0A,
            0x0D,
            0x1B,0x45,0x1B,0x2A,0x74,0x36,0x30,0x30,0x52,0x1B,0x26,0x75,0x36,0x30,0x30,0x44,0x1B,0x2A,0x72,0x30,0x46,0x1B,0x26,0x6C,0x30,0x6F,0x1B,0x26,0x6C,0x37,0x31,0x41,0x1B,0x26,0x6C,0x37,0x48,0x1B,0x26,0x6C,0x30,0x53,0x1B,0x26,0x6C,0x31,0x58,0x1B,0x26,0x6C,0x38,0x63,0x31,0x45,0x1B,0x2A,0x70,0x30,0x78,0x30,0x59,0x1B,0x2A,0x63,0x30,0x74,0x32,0x34,0x39,0x34,0x78,0x33,0x39,0x35,0x35,0x59,0x1B,0x2A,0x62,0x30,0x4D,  0x0D,
            0x1B,0x28,0x31,0x39,0x55,0x1B,0x28,0x73,0x34,0x30,0x39,0x39,0x74,0x30,0x62,0x30,0x73,0x31,0x32,0x2E,0x30,0x30,0x68,0x30,0x50,0x1B,0x26,0x64,0x40,0x1B,0x2A,0x76,0x30,0x6F,0x30,0x54,0x1B,0x2A,0x70,0x31,0x38,0x35,0x59,0x1B,0x2A,0x70,0x31,0x32,0x30,0x58
       };
       
       //     0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,
       byte u3[]={
            0x1B,0x2A,0x70,0x33,0x32,0x39,0x36,0x59,0x0C,0x1B,0x45,0x1B,0x25,0x2D,0x31,0x32,0x33,0x34,0x35,0x58,0x40,0x50,0x4A,0x4C,0x20,0x45,0x4F,0x4A,  0x0A,
            0x1B,0x25,0x2D,0x31,0x32,0x33,0x34,0x35,0x58
       };

       byte wc3[ sizeof(u1)+i+sizeof(u3) ]={0x00};
       int k = 0;
       for(k=0; k<sizeof(u1); k++){
           wc3[k] = u1[k];
           //Serial.print(k);
           //Serial.print(".");
           Serial.print(wc3[k], HEX);
           //Serial.print("\n");        
       }
       Serial.print( "___k === ");
       Serial.println(k); 
       
       for(k=sizeof(u1); k<sizeof(u1)+i; k++){
           wc3[k] = str2[k-sizeof(u1)];
           //Serial.print(k);
           //Serial.print(".");
           Serial.print(wc3[k]);
           //Serial.print("\n");            
       }
       Serial.print( "___k === ");
       Serial.println(k); 
       
       for(k=(sizeof(u1)+i); k<sizeof(u1)+i+sizeof(u3); k++){
           wc3[k] = u3[k-(sizeof(u1)+i)];
           //Serial.print(k);
           //Serial.print(".");
           Serial.print(wc3[k], HEX);
           //Serial.print("\n");    
       }
       Serial.print( "**282**k === ");
       Serial.println(k); 

       //prnMsg1(wc3             , sizeof(wc3)-1 );  
       //resetPrinter();
//0x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x

       str2[200] = { 0 };


}

//---------------------------------------------------------------------------------------------------------------------------
/////////////////////////////////////////Ohaus //////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------------------------------------------------------
void txtC(){
        //------------------------Ohaus----------------------------
        int i = 0;
        char b1[25] = "\r----------------------\r\n";
        Serial.print( "sizeof_b1 = ");        Serial.println(sizeof(b1));   
        for ( i = 0; i < 25; i++) {
             str3[i] = b1[i];
             //Serial.print(i);
             //Serial.print(".");
             Serial.print(str3[i]);
             //Serial.print("\n");
        }
        Serial.print( "\t\t___i === ");
        Serial.println(i);

        //----------------------------------------------------
        char b2[16];
        sprintf(b2, "%s %s\r\n" , rtc.getDateStr( FORMAT_SHORT), rtc.getTimeStr( FORMAT_SHORT));
        Serial.print( "sizeof_b2 = ");         Serial.println(sizeof(b2));
        
        for ( i = 25; i < 25+16 ; i++) {
             str3[i] = b2[i-25];
             //Serial.print(i);
             //Serial.print(".");
             Serial.print(str3[i]);
             //Serial.print("\n");
        }
        Serial.print( "\t\t___i === ");
        Serial.println(i);

        //----------------------------------------------------
        char b3[31] = "Ohaus PA214C\r\nS/N: B247553310\r\n";
        Serial.print( "sizeof_b3 = ");         Serial.println(sizeof(b3));
        for (i = 41; i < 25+16+31 ; i++) {
             str3[i] = b3[i-41];
             //Serial.print(i);
             //Serial.print(".");
             Serial.print(str3[i]);
             //Serial.print("\n");
        }
        Serial.print( "\t\t___i === ");
        Serial.println(i);
        
        //----------------------------------------------------
        if (j==-1){      j=0;     }
        char b4[8+1+11+2 ];
        sprintf(b4, "%s %s\r\n", "User ID:" ,  fio[j]);
        Serial.print( "sizeof_b4 = ");         Serial.println(sizeof(b4 )); //Serial.print(b4);
        for (i = 72; i < 25+16+31+22 ; i++) {
             str3[i] = b4[i-72];
             //Serial.print(i);
             //Serial.print(".");
             Serial.print(str3[i]);
             //Serial.print("\n");
        }
        Serial.print( "\t\t**94**i === ");
        Serial.println(i); 

//0x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x
       byte u1[]={
            0x1B,0x25,0x2D,0x31,0x32,0x33,0x34,0x35,0x58,0x40,0x50,0x4A,0x4C,0x20,0x4A,0x4F,0x42,  0x0A,
            0x0D,
            0x1B,0x45,0x1B,0x2A,0x74,0x36,0x30,0x30,0x52,0x1B,0x26,0x75,0x36,0x30,0x30,0x44,0x1B,0x2A,0x72,0x30,0x46,0x1B,0x26,0x6C,0x30,0x6F,0x1B,0x26,0x6C,0x37,0x31,0x41,0x1B,0x26,0x6C,0x37,0x48,0x1B,0x26,0x6C,0x30,0x53,0x1B,0x26,0x6C,0x31,0x58,0x1B,0x26,0x6C,0x38,0x63,0x31,0x45,0x1B,0x2A,0x70,0x30,0x78,0x30,0x59,0x1B,0x2A,0x63,0x30,0x74,0x32,0x34,0x39,0x34,0x78,0x33,0x39,0x35,0x35,0x59,0x1B,0x2A,0x62,0x30,0x4D,  0x0D,
            0x1B,0x28,0x31,0x39,0x55,0x1B,0x28,0x73,0x34,0x30,0x39,0x39,0x74,0x30,0x62,0x30,0x73,0x31,0x32,0x2E,0x30,0x30,0x68,0x30,0x50,0x1B,0x26,0x64,0x40,0x1B,0x2A,0x76,0x30,0x6F,0x30,0x54,0x1B,0x2A,0x70,0x31,0x38,0x35,0x59,0x1B,0x2A,0x70,0x31,0x32,0x30,0x58
       };
       
       //     0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,
       byte u3[]={
            0x1B,0x2A,0x70,0x33,0x32,0x39,0x36,0x59,0x0C,0x1B,0x45,0x1B,0x25,0x2D,0x31,0x32,0x33,0x34,0x35,0x58,0x40,0x50,0x4A,0x4C,0x20,0x45,0x4F,0x4A,  0x0A,
            0x1B,0x25,0x2D,0x31,0x32,0x33,0x34,0x35,0x58
       };

       byte wc3[ sizeof(u1)+i+sizeof(u3) ]={0x00};
       int k = 0;
       for(k=0; k<sizeof(u1); k++){
           wc3[k] = u1[k];
           //Serial.print(k);
           //Serial.print(",0x");
           Serial.print(wc3[k], HEX);
           //Serial.print("\n");        
       }
       Serial.print( "\t\t___k === ");
       Serial.println(k); 
       
       for(k=sizeof(u1); k<sizeof(u1)+i; k++){
           wc3[k] = str3[k-sizeof(u1)];
           //Serial.print(k);
           //Serial.print(".");
           Serial.print(wc3[k]);
           //Serial.print("\n");            
       }
       Serial.print( "\t\t___k === ");
       Serial.println(k); 
       
       for(k=(sizeof(u1)+i); k<sizeof(u1)+i+sizeof(u3); k++){
           wc3[k] = u3[k-(sizeof(u1)+i)];
           //Serial.print(k);
           //Serial.print(",0x");
           Serial.print(wc3[k], HEX);
           //Serial.print("\n");    
       }
       Serial.print( "\n\t\t**283**k === ");
       Serial.println(k); 

       //prnMsg1(wc3             , sizeof(wc3)-1 );  
       //resetPrinter();
//0x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x
        
       //prnMsg1(  str1,   sizeof(str1)    );
       //prnMsg1("Mecmesin BFG500N"             , strlen("Mecmesin BFG500N") );
       //resetPrinter();
       str3[200] = { 0 }; 
        
}





//////////////////////////////////////////DDDDDDD/////////////////////////////////////////////////////////////////////////////
void txtD(){
       byte u1[]={
            0x1B,0x25,0x2D,0x31,0x32,0x33,0x34,0x35,0x58,0x40,0x50,0x4A,0x4C,0x20,0x4A,0x4F,0x42,  0x0A,
            0x0D,
            0x1B,0x45,0x1B,0x2A,0x74,0x36,0x30,0x30,0x52,0x1B,0x26,0x75,0x36,0x30,0x30,0x44,0x1B,0x2A,0x72,0x30,0x46,0x1B,0x26,0x6C,0x30,0x6F,0x1B,0x26,0x6C,0x37,0x31,0x41,0x1B,0x26,0x6C,0x37,0x48,0x1B,0x26,0x6C,0x30,0x53,0x1B,0x26,0x6C,0x31,0x58,0x1B,0x26,0x6C,0x38,0x63,0x31,0x45,0x1B,0x2A,0x70,0x30,0x78,0x30,0x59,0x1B,0x2A,0x63,0x30,0x74,0x32,0x34,0x39,0x34,0x78,0x33,0x39,0x35,0x35,0x59,0x1B,0x2A,0x62,0x30,0x4D,  0x0D,
            0x1B,0x28,0x31,0x39,0x55,0x1B,0x28,0x73,0x34,0x30,0x39,0x39,0x74,0x30,0x62,0x30,0x73,0x31,0x32,0x2E,0x30,0x30,0x68,0x30,0x50,0x1B,0x26,0x64,0x40,0x1B,0x2A,0x76,0x30,0x6F,0x30,0x54,0x1B,0x2A,0x70,0x31,0x38,0x35,0x59,0x1B,0x2A,0x70,0x31,0x32,0x30,0x58,
            0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,
            0x1B,0x2A,0x70,0x33,0x32,0x39,0x36,0x59,0x0C,0x1B,0x45,0x1B,0x25,0x2D,0x31,0x32,0x33,0x34,0x35,0x58,0x40,0x50,0x4A,0x4C,0x20,0x45,0x4F,0x4A,  0x0A,
            0x1B,0x25,0x2D,0x31,0x32,0x33,0x34,0x35,0x58
                        
       };

  prnMsg1(u1             , sizeof(u1)-1 );  
  resetPrinter();
 
}

/////////////////////////////////// --#-- ////////////////////////////////////////////////////////////////////////////////////////
void txt3(){
  char c1[]="12345678";
  prnMsg1(c1             , sizeof(c1)-1 );  
  resetPrinter();
 
}

//////////////////////////////////    ****  /////////////////////////////////////////////////////////////////////////////////////
void txt5(){
        //------------------------Ohaus----------------------------
        int i = 0;
        char b1[25] = "\r----------------------\r\n";
        Serial.print( "sizeof_b1 = ");        Serial.println(sizeof(b1));   
        for ( i = 0; i < 25; i++) {
             str3[i] = b1[i];            //Serial.print(i);
             //Serial.print(".");
             Serial.print(str3[i]);
             //Serial.print("\n");
        }
        Serial.print( "___i === ");
        Serial.println(i);

        //----------------------------------------------------
        char b2[16];
        sprintf(b2, "%s %s\r\n" , rtc.getDateStr( FORMAT_SHORT), rtc.getTimeStr( FORMAT_SHORT));
        Serial.print( "sizeof_b2 = ");         Serial.println(sizeof(b2));
        
        for ( i = 25; i < 25+16 ; i++) {
             str3[i] = b2[i-25];
             //Serial.print(i);
             //Serial.print(".");
             Serial.print(str3[i]);
             //Serial.print("\n");
        }
        Serial.print( "___i === ");
        Serial.println(i);

        //----------------------------------------------------
        char b3[31] = "Ohaus PA214C\r\nS/N: B247553310\r\n";
        Serial.print( "sizeof_b3 = ");         Serial.println(sizeof(b3));
        for (i = 41; i < 25+16+31 ; i++) {
             str3[i] = b3[i-41];
             //Serial.print(i);
             //Serial.print(".");
             Serial.print(str3[i]);
             //Serial.print("\n");
        }
        Serial.print( "___i === ");
        Serial.println(i);
        
        //----------------------------------------------------
        if (j==-1){      j=0;     }
        char b4[8+1+11+2 ];
        sprintf(b4, "%s %s\r\n", "User ID:" ,  fio[j]);
        Serial.print( "sizeof_b4 = ");         Serial.println(sizeof(b4 )); //Serial.print(b4);
        for (i = 72; i < 24+16+31+22 ; i++) {
             str3[i] = b4[i-72];
             //Serial.print(i);
             //Serial.print(".");
             Serial.print(str3[i]);
             //Serial.print("\n");
        }
        Serial.print( "**94**i === ");
        Serial.println(i); 

       //0x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x
       byte u1[]={
            0x1B,0x25,0x2D,0x31,0x32,0x33,0x34,0x35,0x58,0x40,0x50,0x4A,0x4C,0x20,0x4A,0x4F,0x42,  0x0A,
            0x0D,
            0x1B,0x45,0x1B,0x2A,0x74,0x36,0x30,0x30,0x52,0x1B,0x26,0x75,0x36,0x30,0x30,0x44,0x1B,0x2A,0x72,0x30,0x46,0x1B,0x26,0x6C,0x30,0x6F,0x1B,0x26,0x6C,0x37,0x31,0x41,0x1B,0x26,0x6C,0x37,0x48,0x1B,0x26,0x6C,0x30,0x53,0x1B,0x26,0x6C,0x31,0x58,0x1B,0x26,0x6C,0x38,0x63,0x31,0x45,0x1B,0x2A,0x70,0x30,0x78,0x30,0x59,0x1B,0x2A,0x63,0x30,0x74,0x32,0x34,0x39,0x34,0x78,0x33,0x39,0x35,0x35,0x59,0x1B,0x2A,0x62,0x30,0x4D,  0x0D,
            0x1B,0x28,0x31,0x39,0x55,0x1B,0x28,0x73,0x34,0x30,0x39,0x39,0x74,0x30,0x62,0x30,0x73,0x31,0x32,0x2E,0x30,0x30,0x68,0x30,0x50,0x1B,0x26,0x64,0x40,0x1B,0x2A,0x76,0x30,0x6F,0x30,0x54,0x1B,0x2A,0x70,0x31,0x38,0x35,0x59,0x1B,0x2A,0x70,0x31,0x32,0x30,0x58
       };
       
       //     0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,
       byte u3[]={
            0x1B,0x2A,0x70,0x33,0x32,0x39,0x36,0x59,0x0C,0x1B,0x45,0x1B,0x25,0x2D,0x31,0x32,0x33,0x34,0x35,0x58,0x40,0x50,0x4A,0x4C,0x20,0x45,0x4F,0x4A,  0x0A,
            0x1B,0x25,0x2D,0x31,0x32,0x33,0x34,0x35,0x58
       };

       byte wc3[ sizeof(u1)+i+sizeof(u3) ]={0x00};
       int k = 0;
       for(k=0; k<sizeof(u1); k++){
           wc3[k] = u1[k];
           //Serial.print(k);
           //Serial.print(",0x");
           Serial.print(wc3[k], HEX);
           //Serial.print("\n");        
       }
       Serial.print( "___k === ");
       Serial.println(k); 
       
       for(k=sizeof(u1); k<sizeof(u1)+i; k++){
           wc3[k] = str3[k-sizeof(u1)];
           //Serial.print(k);
           //Serial.print(".");
           Serial.print(wc3[k]);
           //Serial.print("\n");            
       }
       Serial.print( "___k === ");
       Serial.println(k); 
       
       for(k=(sizeof(u1)+i); k<sizeof(u1)+i+sizeof(u3); k++){
           wc3[k] = u3[k-(sizeof(u1)+i)];
           //Serial.print(k);
           //Serial.print(",0x");
           Serial.print(wc3[k], HEX);
           //Serial.print("\n");    
       }
       Serial.print( "**282**k === ");
       Serial.println(k); 

       prnMsg1(wc3             , sizeof(wc3)-1 );  
       resetPrinter();
       //0x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x000x

                                                                                      //prnMsg1(  str1,   sizeof(str1)    );
                                                                                      //prnMsg1("Mecmesin BFG500N"             , strlen("Mecmesin BFG500N") );
                                                                                      //resetPrinter();

        

        
        
        str3[200] = { 0 }; 
        
}



//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//======================================================================================================================================
//======================================================================================================================================
//======================================================= P R I N T ====================================================================
//======================================================================================================================================
//======================================================================================================================================
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void printByte(byte inByte) {
  while(digitalRead(busy) == HIGH) {
    // wait for busy to go low
  }

  int b0 = bitRead(inByte, 0);
  int b1 = bitRead(inByte, 1);
  int b2 = bitRead(inByte, 2);
  int b3 = bitRead(inByte, 3);
  int b4 = bitRead(inByte, 4);
  int b5 = bitRead(inByte, 5);
  int b6 = bitRead(inByte, 6);
  int b7 = bitRead(inByte, 7);

  digitalWrite(data_0, b0);        // set data bit pins
  digitalWrite(data_1, b1);
  digitalWrite(data_2, b2);
  digitalWrite(data_3, b3);
  digitalWrite(data_4, b4);
  digitalWrite(data_5, b5);
  digitalWrite(data_6, b6);
  digitalWrite(data_7, b7);

  digitalWrite(nStrobe, LOW);       // strobe nStrobe to input data bits
  delayMicroseconds(strobeWait);
  digitalWrite(nStrobe, HIGH);

  while(digitalRead(busy) == HIGH) {
    // wait for busy line to go low
  } 
}
//-------------------------------------------------------------------------------------------
void resetPrinter() {
  Serial.println("\nReseting printer...");
  printByte(27); // reset printer
  printByte('E');
  Serial.println("Printer Reset"); 
}
//-------------------------------------------------------------------------------------------
void prnMsg1(byte msg1[], int zs1) {
  digitalWrite(24, HIGH);

  //for(int line = 0; line < num_lines; line++) {
  for (int i = 0; i < zs1; i++) {
    byte character = msg1[i];
    printByte(character);
    delay(1);
  }
  printByte(10); // new line
  printByte(13); // carriage return
  //}
  
  digitalWrite(24, LOW);
}
