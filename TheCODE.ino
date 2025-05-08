/*
Projektnamn: Kodlos^3
Skapare: Robin Almström TE23C
Beskrivning: Ett kodlos maximerat för flexibilitet och möjlighet till expansion, detta är ett kodlos med möjlighet till en flerstegsprocess med i teorin oändligt lång kod.
*/

// Inkluderar några nödvändiga libraries
#include <Keypad.h> // behövs för keypad
#include "U8glib.h" // behövs för OLED-skärm
#include "pitches.h" // behövs endast för Piezo-låtar

#define BUZZER_PIN 2

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK); //magi som behövs för OLED

//definerar storleken på keypaden
const int ROW_NUM = 4; //antalet rader på keypaden(4)
const int COLUMN_NUM = 3; //antalet kolumner på keypaden(3)

// Variabler för tracking av vilket steg i processen som körs
int game = 0; 
const int totgamenum = 1;

// Variabler för steg noll
int korrektkod[] = {"5","3","6","6","9"};
int kodlength = 5;
int i = 0;
int seq = 0;
String j;
String k;
String h;
String g;
String f;
String e;
String d;
String c;
String b;
String a;

// Variabler för steg ett
int g1sequence[] = {String(random(1,10)), String(random(1,10)), String(random(1,10)), String(random(1,10)), String(random(1,10))};
int turn = 1;
int gi = 0;



// Wallace & Gromit temasång
int melody[] = {
  NOTE_C7, NOTE_AS6, NOTE_A6, NOTE_C7, NOTE_AS6, NOTE_A6, NOTE_C7, NOTE_G6, REST, REST/*,
  NOTE_D7, NOTE_C7, NOTE_D7, NOTE_E7, NOTE_D7, NOTE_C7  , NOTE_AS6, NOTE_A6, REST, REST,
  NOTE_C7, NOTE_AS6, NOTE_A6, NOTE_C7, NOTE_AS6, NOTE_A6, NOTE_C7, NOTE_G6, REST, REST,
  NOTE_D7, NOTE_C7, NOTE_D7, NOTE_E7, NOTE_D7, NOTE_E7, NOTE_F7, NOTE_C7, NOTE_F7, NOTE_F7, NOTE_E7, NOTE_D7,

  NOTE_F7, NOTE_E7, NOTE_D7, NOTE_F7, NOTE_E7, NOTE_D7, NOTE_F7, NOTE_D7, NOTE_C7, NOTE_C7, NOTE_B6, NOTE_C7, 
  NOTE_D7, NOTE_C7, NOTE_D7, NOTE_C7, NOTE_A6, NOTE_GS6, NOTE_A6, NOTE_GS6, REST, NOTE_A6, NOTE_A6,
  NOTE_A7, NOTE_GS6, REST, NOTE_A6, NOTE_A6, NOTE_G6, NOTE_E6, NOTE_E6, NOTE_DS6, REST, NOTE_E6, NOTE_G6, NOTE_F6, NOTE_D6,
  NOTE_D6, NOTE_CS6, REST, NOTE_D6, NOTE_G6, NOTE_D6, NOTE_G6, NOTE_A6, NOTE_D6, NOTE_A6, NOTE_C7, NOTE_C7, NOTE_C7, REST,
  NOTE_DS7, NOTE_E7, REST, NOTE_AS6, NOTE_B6, REST, NOTE_FS6, NOTE_G6, NOTE_DS6, REST, NOTE_E6, NOTE_C6,

  NOTE_C7, NOTE_AS6, NOTE_A6, NOTE_C7, NOTE_AS6, NOTE_A6, NOTE_C7, NOTE_G6, REST, REST,
  NOTE_D7, NOTE_C7, NOTE_D7, NOTE_E7, NOTE_D7, NOTE_C7  , NOTE_AS6, NOTE_A6, REST, REST,
  NOTE_C7, NOTE_AS6, NOTE_A6, NOTE_C7, NOTE_AS6, NOTE_A6, NOTE_C7, NOTE_G6, REST, REST,
  NOTE_D7, NOTE_C7, NOTE_D7, NOTE_E7, NOTE_D7, NOTE_E7, NOTE_F7, NOTE_C7, NOTE_F7*/
};
int durations[] = {
  4, 8, 8, 4, 8, 8, 8, 2, 8, 8/*,
  8, 8, 8, 4, 8, 8, 8, 2, 4, 8,
  4, 8, 8, 4, 8, 8, 8, 2, 8, 8,
  8, 8, 8, 4, 4, 8, 4, 4, 8, 8, 8, 8,

  4, 8, 4, 8, 8, 8, 4, 8, 4, 8, 8, 8,
  8, 8, 8, 8, 16, 8, 16, 16, 16, 8, 2,
  8, 16, 16, 8, 4, 8, 4, 8, 16, 16, 8, 4, 8, 4,
  8, 16, 16, 8, 8, 8, 8, 4, 4, 8, 16, 8, 8, 4,
  16, 8, 16, 16, 8, 16, 16, 8, 16, 16, 8, 8,

  4, 8, 8, 4, 8, 8, 8, 2, 8, 8,
  8, 8, 8, 4, 8, 8, 8, 2, 4, 8,
  4, 8, 8, 4, 8, 8, 8, 2, 8, 8,
  8, 8, 8, 4, 4, 8, 4, 4, 8*/
};


// Mappar ut vilken knapp som representerar vilken Char
char keys[ROW_NUM][COLUMN_NUM] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

byte pin_rows[ROW_NUM] = {9, 8, 7, 6}; //vilken pin raderna kopplas till
byte pin_column[COLUMN_NUM] = {5, 4, 3}; //vilken pin kolumnerna kopplas till

Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM ); //mappar respektive pin till sin korrekta Char




//definerar LED-pins och sätter defaultfärgen, samt sätter OLED-fonten(u8g's 4pxl-font för skojs skull)
void setup(){
  Serial.begin(9600);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  yellow();
  u8g.setFont(u8g_font_u8glib_4);
}


//loopen :)
void loop(){
  char key = keypad.getKey(); //Känner av om en knapp trycks
  if (key){ // Om en knapp trycks:
    if (String(key) == "#"){ // Om brädgård trycks ner:
      if (game == 0){ // Om det är steg noll: 
        if (seq == kodlength) { // Om alla knappar har tryckts korrekt
          dub(); //spela vinstsekvens
          game = 1; // sätt spel till nästa spel
        }else{ 
          ggnoob(); // spela förlustsekvens
        }
        emptyLastPressedHistory(); // ta bort knapphistoriken
        seq = 0; //resetta allt inför nästa försök
        i = 0;
      } else if (game == 1){ // annars, om det är steg ett:
      // Knapp används ej i steg ett. Ger ingen reaktion när nertryckt
      }
    }else if (String(key) == "*"){ // Om nertryckt knapp inte är # utan *:
      oledWrite("", "Code Reset!", "", "Sequence nr: 0", "", "", "", "", "", ""); // skriver att koden har resettats
      // Spelar en sekvens med blinkande rött och gult för att signalera att koden resettats.
      red();
      delay(200);
      yellow();
      delay(200);
      red();
      delay(200);
      yellow();
      seq = 0;
      i = 0; // Tilldelar relevanta variabler originella värden.
      game = 0;
    }else if (game == 0){ // Ifall spelet är noll och en siffra tryckts ner.

      k = j; 
      j = h;
      h = g;
      g = f;
      f = e;   // Updatera historik på det mest grundliga sättet någonsin för orka leka med arrayer.
      e = d;
      d = c;
      c = b;
      b = a;
      a = String(key); // sätter den senaste till nummret på knappen som trycktes, fungerar även med # & *.

      if (String(key) == korrektkod[i]) { // om den nedtryckta knappen är samma som knappen i den korrekta koden:
        seq += 1; //höj antalet korrekt tryckta knappar i sekvensen med ett
      }
      // Skriv ut vilka knappas som skrivits, hur många, och en ledtråd ifall man glömmer bort.
      oledWrite("", "Input:    " + a + " " + b + " " + c + " " + d + " " + e + " " + f + " " + g + " " + h  " " + j + " " + k, "", "Sequence nr: " + String(i + 1), "", "If I forgot code:", "The worst year ever, followed by", "the intergral of 24x^5 - 9x^2", "from -1.1 to 1.69445.", "");

      i += 1; // höjer antalet tryckta knappar i sekvensen med ett
    }else if (game == 1){ // ifall spelet är ett och en siffra trycktes ner:
      // skriver ut vad man ska göra
      oledWrite("", "Click the numbers in the correct order as shown below:", "", "", String(int g1sequence[gi]), "", String(gi + 1), "", "", "");
      if (int(key) != int(g1sequence[gi])){ // om man trycker fel knapp:
        Serial.println(String(key) + " Is not " + (String(g1sequence[gi]))); // skriv i seriellmonitorn att knappen man tryckte ner inte var samma som knappen man skulle trycka ner.
        ggnoob(); // ge förlust
        gi = 0; // återställ påverkad variabel
        Resetall(); //återställ alla globala variablar (aka börja om från noll)
      }else if (gi +1 >= sizeof(g1sequence)){ // annars, om man trycker rätt:
        dub(); // vinssekvens
        Resetall(); //återställ alla globala variablar inför nästa försök
      }
      gi += 1; //öka antalet knapptryck med ett
    }
    Serial.println("The future is string with this one");
    Serial.println(key);
    Serial.println(i);    // DEV: Skriv ut ett antal viktiga variabler om en knapp trycks ner.
    Serial.println(seq);
    Serial.println(" ");
  }
} // slut på loopen.


//=================Funktioner===================//

/*
Namn: oledWrite
Beskrivning: skriver ut 10 rader text på oledskärmen.
Input: 10 Strings
Returns: void 
"Output": Skriver ut på OLED-skärmen
*/
void oledWrite(String text, String text2, String text3, String text4, String text5, String text6, String text7, String text8, String text9, String text0){
  u8g.firstPage();

  do{
    u8g.drawStr(0, 6, text.c_str());
    u8g.drawStr(0, 12, text2.c_str());
    u8g.drawStr(0, 18, text3.c_str());
    u8g.drawStr(0, 24, text4.c_str());
    u8g.drawStr(0, 30, text5.c_str());
    u8g.drawStr(0, 36, text6.c_str());
    u8g.drawStr(0, 42, text7.c_str());
    u8g.drawStr(0, 48, text8.c_str());
    u8g.drawStr(0, 54, text9.c_str());
    u8g.drawStr(0, 60, text0.c_str());
  }while (u8g.nextPage());
}


/*
Namn: green, yellow, red
Beskrivning: tre funktioner som sätter på LEDen vars färg matchar namnet och stänger av de andra. Detta för att minska onödig text i loopen.
Input: Inget
Retunrerar: Inget
"Output": Sätter på en LED och stänger av resten. (aka byter lampa)
*/
void green() {
  digitalWrite(11, HIGH);
  digitalWrite(12, LOW);
  digitalWrite(13, LOW);
}

void yellow() {
  digitalWrite(12, HIGH);
  digitalWrite(11, LOW);
  digitalWrite(13, LOW);
}

void red() {
  digitalWrite(13, HIGH);
  digitalWrite(12, LOW);
  digitalWrite(11, LOW);
}


/*
Namn: Playsong
Beskrivning: Spelar musik på Piezot (direkt stulet från internet)
Input: Inget
Returnerar: Inget
"Output": Får piezon att börja spela.
*/
void Playsong() {
  int size = sizeof(durations) / sizeof(int); // calculates the length of the song.

  for (int note = 0; note < size; note++) { // for every note of the song:
    int duration = 1832 / durations[note]; // calculates the duration of the note. 1000 for songs from GitHub, otherwise: 120BPM = 2000ms, 131BPM = 1832ms, 262BPM = 916ms
    tone(BUZZER_PIN, melody[note], duration); //plays the current note value for the current duration.

    int pauseBetweenNotes = duration * 1.30; // calculates the pause between notes.
    delay(pauseBetweenNotes); // delays by that amount of time.

    noTone(BUZZER_PIN); // Stops playing the note.
  }
}


/*
Namn: dub
Beskrivning: Kod som ska spelas när man klarar av ett steg i sekvensen.
Input: Inget
Returnerar: Inget
"Output": Spelar en sekvens reserverad till när någon klarar ett steg eller låser upp låset.
*/
void dub() {
  green(); // säter ljuset till grönt
  if (game != totgamenum){ // om det inte är det sista steget.
    oledWrite("", "Code Correct!", "", "Sequence nr: 0", "", "", "", "", "", "");
    delay(4000);
    oledWrite("", "Click the numbers in the correct order as shown below:", "", "", String(g1sequence[0]), "", "", "", "", "");
  }else{ // om det ÄR sista steget:
    oledWrite("", "Code Correct!", "", "Acess granted. Welcome home, Master.", "", "", "", "", "", "");
    delay(8000);
  }
  yellow(); // sätter tillbaka till gult
}


/*
Namn: ggnoob
Beskrivning: Kod för ifall du gör fel
Input: Inget
Returnerar: Inget
"Output": Skriver på OLED-skärmen och spelar W&G-temat
*/
void ggnoob(){
  oledWrite("", "Incorrect button pressed", "", "Sequence nr: 0", "", "", "", "", "", "");
  red();
  Playsong();
  yellow();
}


/*
Namn: emptyLastPressedHistory
Beskrivning: Tömmer en massa variabler
Input: Inget
Returnerar: Inget
"Output": Ändra en massa variabler till ''.
*/
void emptyLastPressedHistory() {
  a = "";
  b = "";
  c = "";
  d = "";
  e = "";
  f = "";
  g = "";
  k = "";
  h = "";
  j = "";
}


/*
Namn: Resetall
Beskrivning: Återställer alla nödvändiga variabler för att köra om koden.
Input: Inget
Returnerar: Inget
"Output": ändrar några variabler och blinkar lampor
*/
void Resetall() {
  turn = 1;
  gi = 0;
  emptyLastPressedHistory();
  game = 0;
  kodlength = 5;
  i = 0;
  seq = 0;
  red();
  delay(200);
  yellow();
  delay(200);   //blinkar lampor för att indikera
  red();
  delay(200);
  yellow();
}