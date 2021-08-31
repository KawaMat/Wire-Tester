#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
 #include <SPI.h>
#include<Wire.h>
#include "PCF8575.h"

#define OLED_RESET 5
Adafruit_SSD1306 display(OLED_RESET);

#define BUTTON_TIME 150
#define BUTTON_OK 3 //PRZYCISK PULL_UP 
#define BUTTON_L 2
#define BUTTON_P 4

//ZMIENNE:
int expander,pin,n,wyj,wej,i,a,licznik,k,b,m,c,d;

int menu = 0;
int czas = 10;
int przewody = 48;

//tablice wyjsc i wyjsc
PCF8575 wyjscia[3];
PCF8575 wejscia[3];
//deklaracje funkcji

int wstrzymanie();
void wyswietl_czas();
void wyswietl_przewody();
void wyswietlanie_wartosci_czas(int);
void wyswietlanie_wartosci_przewody(int);
void wyswietl_start();
void testowanie(int, int);

void setup() {
  Serial.begin(9600);
 
//ustawienia WYSWIETLACZA
display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  
  //Adresowanie expanderow
wejscia[0].begin(0x20);            //PODAĆ ADRESY!!
wejscia[1].begin(0x22);
wejscia[2].begin(0x24);
wyjscia[0].begin(0x25);
wyjscia[1].begin(0x23);
wyjscia[2].begin(0x21);
 
 //ustawianie pinow na wejscia i wyjscia
for( i=0 ; i < 3;i++){
  for( a=0; a < 16 ;a++){
    wyjscia[i].pinMode(a,OUTPUT);
    wyjscia[i].digitalWrite(a,HIGH);
  }
}
for( k=0 ; k < 3 ;k++)
{
  for( b=0; b < 16; b++)
  {
    wejscia[k].pinMode(b,INPUT);
    wejscia[k].digitalWrite(b,HIGH);
  }
}
//przyciski
pinMode(BUTTON_OK, INPUT);
pinMode(BUTTON_L, INPUT);
pinMode(BUTTON_P, INPUT);
digitalWrite(BUTTON_OK,HIGH);
digitalWrite(BUTTON_L,HIGH);
digitalWrite(BUTTON_P,HIGH);

Serial.begin(9600);
 
}
void loop() {
  
  if(digitalRead(BUTTON_L) == LOW){
    delay(BUTTON_TIME);
    if(menu == 0){
      menu=2;
    }
    else{                               
      menu--;
    }
  }
  if(digitalRead(BUTTON_P) == LOW){
    delay(BUTTON_TIME);
    if(menu == 2){
      menu=0;
    }
    else{
      menu++;
    }
  }
  
  if(menu == 0){             // wybranie opcji START
    display.clearDisplay();
    wyswietl_start();
    if(digitalRead(BUTTON_OK) == LOW){
      delay(BUTTON_TIME);
      testowanie(czas,przewody);
    }
  }
  else if(menu == 2){                       //wyswietlenie opcji CZAS
    display.clearDisplay();
    wyswietl_czas();                        //funkcja wyswietlania opcji CZAS
    if(digitalRead(BUTTON_OK) == LOW){      //Wejscie w opcje CZAS
      delay(BUTTON_TIME);
      while(digitalRead(BUTTON_OK) == HIGH){
        if(digitalRead(BUTTON_L) == LOW){
          delay(BUTTON_TIME);
          if(czas == 0){
            czas=100;
          }
          else{                               
            czas=czas-10;
          }
        }
        if(digitalRead(BUTTON_P) == LOW){
          delay(BUTTON_TIME);
          if(czas == 100){
            czas=0;
          }
          else{                               
            czas=czas+10;
          }
        }
        display.clearDisplay();
        wyswietlanie_wartosci_czas(czas);      //funkcja wyswittlania wartosci czasu
      }
      delay(BUTTON_TIME);
    }
  }
    else if(menu == 1){
      display.clearDisplay();
      wyswietl_przewody();                     //funkcja wyswietlania opcji PRZEWODY
      if(digitalRead(BUTTON_OK) == LOW){       //Wejscie w opcje PRZEWODY
        delay(200);
        while(digitalRead(BUTTON_OK) == HIGH){
          if(digitalRead(BUTTON_L) == LOW){
            delay(BUTTON_TIME);
            if(przewody == 1){
              przewody=48;
            }
            else{                               
              przewody--;
            }
          }
          if(digitalRead(BUTTON_P) == LOW){
            delay(BUTTON_TIME);
            if(przewody == 48){
              przewody=1;
            }
            else{                               
              przewody++;
            }
          }
          display.clearDisplay();
          wyswietlanie_wartosci_przewody(przewody); //funkcja wyswittlania wartosci przewody
        }
        delay(BUTTON_TIME);
      }
    }
      
      
    
}

int wstrzymanie(){
  
  display.fillTriangle(128,28,   118,26,   118,30, WHITE);
  display.fillTriangle(64,26,   69,30,   59,30, WHITE);
  display.display();
  delay(1);
  
   int exit = 0;
  while(digitalRead(BUTTON_P) == HIGH){
    if(digitalRead(BUTTON_OK) == LOW){
      delay(BUTTON_TIME);
      exit = 1;
      return exit;
    }
  }
  delay(BUTTON_TIME);
  return exit;
}

void testowanie(int czas, int przewody){
 
 int z,x,o,p,m,c,d;
 int flaga = 0;
 float stan = 0;
 int licznik;
 /*sprawdzanie czy odpowiadajacym pinom podawane sa stany wysokie,
 zapisywanie ich do tablicy i porownywanie ze stanem spodziewanym*/
 Serial.println(przewody);
 Serial.println(czas);
  for( x=0 ; x < 3; x++){        
    for( z=0 ; z<16 ; z++){                    // jesli stan rowny ilosci przewodow wyjdz z funkcji testowanie()
      int tabl[2];
      flaga = 0;
      wyjscia[x].digitalWrite(z,LOW);
      delay(czas);
      
      licznik = ((stan/przewody)*100);
      display.clearDisplay();
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(30,10);
      display.println(licznik);
      display.setCursor(95,10);
      display.println("%");
      display.display();
      delay(1);
      
      for( o=0 ; o <= 2 ; o++){
        for(p = 0; p <16 ; p++){
          if(wejscia[o].digitalRead(p) == LOW){
            
            flaga++;
            tabl[0] = o;
            tabl[1] = p;
           }
         
        }
       
      }
       
      
     
      if(flaga == 1){                               //jesli jest stan wysoki na jednym wyjsciu
          if( (x == tabl[0]) && (z == tabl[1])){    //jesli jest stan wysoki na odpowiednim pinie
            stan++;
            wyjscia[x].digitalWrite(z,HIGH);
            delay(czas);
          }
          else{                                     //jesli jest stan wysoki na nieodpowiednim pinie
            wyj = ((x * 16) + z + 1);
            wej = ((tabl[0]*16)+tabl[1] + 1);
            
            display.clearDisplay();
            display.setTextSize(1);
            display.setTextColor(WHITE);
            display.setCursor(8,5);
            display.println(wyj);                    //DRUKOWANIE ZAMIANY PRZEWODÓW
            display.setCursor(16,5);
            display.println(" zamienione z ");
            display.setCursor(100,5);
            display.println(wej);
            display.display();
            delay(1);
            
            wyjscia[x].digitalWrite(z,HIGH);
            delay(czas);
            if(wstrzymanie() == 1) return;
            display.clearDisplay();
            stan++;
          }
          
          
          flaga = 0;          
          if(stan == przewody) break;
          continue;
        }
        else if(flaga >1){                            //zwarcie na wielu wejsciach
          int blad_exp[flaga];
          int blad_pin[flaga];
          int licznik_tablic = 0;
          for( int m =0 ; m < 3 ; m++){
            for(int c = 0; c < 16 ; c++){
              if(wejscia[m].digitalRead(c) == LOW){
                blad_exp[licznik_tablic] = m;
                blad_pin[licznik_tablic] = c;
                licznik_tablic++;
              }
            }
          }
          wyj = (x*16+z+1);
          
          display.clearDisplay();
          display.setTextSize(1);
          display.setTextColor(WHITE);
          display.setCursor(24,0);                     //DRUKOWANIE ZWARCIA
          display.println(wyj);
          display.setCursor(32,0);
          display.println(" zwarte z: ");
          display.setCursor(0,13);
          
          int spacja = 5;
          for(int d = 0; d <flaga ;d++){
            display.setCursor(spacja,13);
            display.println(blad_exp[d] * 16 + blad_pin[d] + 1);
            spacja+=20; 
          }
          display.display();
          delay(1);
           wyjscia[x].digitalWrite(z,HIGH);
          delay(czas);
          if(wstrzymanie() == 1) return;         
          display.clearDisplay();
          flaga=0;
          stan++;
          if(stan == przewody) break;;
          continue;
        }
        else{                                            //nie ma sygnalu na zadnym wejsciu
          wyj = (x*16+z+1);
          
          display.clearDisplay();
          display.setTextSize(1);
          display.setTextColor(WHITE);
          display.setCursor(8,5);                        //DRUKOWANIE ZWARCIA
          display.println(wyj);
          display.setCursor(16,5);
          display.println(" - brak przeplywu!");
          display.display();
          delay(1);
          
          
          
          
         wyjscia[x].digitalWrite(z,HIGH);
         delay(czas);
         flaga = 0;
         stan++;
         if(wstrzymanie() == 1) return;
         if(stan == przewody) break;
         
        }
        
        
        }
        if(stan == przewody) break;
      }
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(5,10);                                 //ZAKONCZENIE
  display.println("UKONCZONO!");
  display.display();
  delay(1500);     
    
}
void wyswietl_start(void) {
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(32,5);
  display.clearDisplay();
  display.println("START!");
  display.fillTriangle(0,28,   10,26,   10,30, WHITE);
  display.fillTriangle(128,28,   118,26,   118,30, WHITE);
  display.fillTriangle(64,30,   69,26,   59,26, WHITE);
  display.display();
  delay(1);
}

void wyswietl_przewody(void) {
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(18,5);
  display.clearDisplay();
  display.println("PRZEWODY");
  display.fillTriangle(0,28,   10,26,   10,30, WHITE);
  display.fillTriangle(128,28,   118,26,   118,30, WHITE);
  display.fillTriangle(64,30,   69,26,   59,26, WHITE);
  display.display();
  delay(1);
}

void wyswietl_czas(void) {
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(40,5);
  display.clearDisplay();
  display.println("CZAS");
  display.fillTriangle(0,28,   10,26,   10,30, WHITE);
  display.fillTriangle(128,28,   118,26,   118,30, WHITE);
  display.fillTriangle(64,30,   69,26,   59,26, WHITE);
  display.display();
  delay(1);
}
void wyswietlanie_wartosci_czas(int czas) {
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(10,7);
  display.println("CZAS:");
  display.setTextSize(2);
  display.setCursor(45,3);
  display.println(czas);
  display.setTextSize(1);
  display.setCursor(90,7);
  display.println("MS");
  display.fillTriangle(0,28,   10,26,   10,30, WHITE);
  display.fillTriangle(128,28,   118,26,   118,30, WHITE);
  display.fillTriangle(64,26,   69,30,   59,30, WHITE);
  display.display();
  delay(1);
}
 void wyswietlanie_wartosci_przewody(int przewody) {
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(15,7);
  display.clearDisplay();
  display.println("PRZEWODY:");
  display.setTextSize(2);
  display.setCursor(80,3);
  display.println(przewody);
  display.fillTriangle(0,28,   10,26,   10,30, WHITE);
  display.fillTriangle(128,28,   118,26,   118,30, WHITE);
  display.fillTriangle(64,26,   69,30,   59,30, WHITE);
  display.display();
  delay(1);
}
