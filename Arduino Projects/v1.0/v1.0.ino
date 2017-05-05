// Written by Daniel Smith, 2012.01.30
// Modified by Hans Arizona to Digimundo Technlogics, 2017.05.04

#define MAX_BITS 100                 // max number of bits 
#define WEIGAND_WAIT_TIME  3000      // time to wait for another weigand pulse.  
 
unsigned char databits[MAX_BITS];    // stores all of the data bits
unsigned char bitCount;              // number of bits currently captured
unsigned char flagDone;              // goes low when data is currently being captured
unsigned int weigand_counter;        // countdown until we assume there are no more bits
 
unsigned long facilityCode=0;        // decoded facility code
unsigned long cardCode=0;            // decoded card code

int alarma=13;

// interrupt that happens when INTO goes low (0 bit)
void ISR_INT0()
{
  //Serial.print("0");   // uncomment this line to display raw binary
  bitCount++;
  flagDone = 0;
  weigand_counter = WEIGAND_WAIT_TIME;  
 
}
 
// interrupt that happens when INT1 goes low (1 bit)
void ISR_INT1()
{
  //Serial.print("1");   // uncomment this line to display raw binary
  databits[bitCount] = 1;
  bitCount++;
  flagDone = 0;
  weigand_counter = WEIGAND_WAIT_TIME;  
}
 
void setup()
{
  pinMode(alarma, OUTPUT);    // LED
  pinMode(2, INPUT);     // DATA0 (INT0)
  pinMode(3, INPUT);     // DATA1 (INT1)
 
  Serial.begin(9600);
  Serial.println("Tarjetas RFID \n\n");
 
  // binds the ISR functions to the falling edge of INTO and INT1
  attachInterrupt(0, ISR_INT0, FALLING);  
  attachInterrupt(1, ISR_INT1, FALLING);
 
 
  weigand_counter = WEIGAND_WAIT_TIME;
}

void led()
{
  digitalWrite(alarma, HIGH);
  delay(500);
  digitalWrite(alarma, LOW);
  delay(100);
}
 
void loop()
{
  // This waits to make sure that there have been no more data pulses before processing data
  if (!flagDone) {
    if (--weigand_counter == 0)
      flagDone = 1;  
  }
 
  // if we have bits and we the weigand counter went out
  if (bitCount > 0 && flagDone) {
    unsigned char i;
 
    // we will decode the bits differently depending on how many bits we have
    // see www.pagemac.com/azure/data_formats.php for mor info
    if (bitCount == 35) {
      // Serial.println("caso 35 bits");
      // 35 bit HID Corporate 1000 format
      // facility code = bits 2 to 14
      for (i=2; i<14; i++)
      {
         facilityCode <<=1;
         facilityCode |= databits[i];
      }
 
      // card code = bits 15 to 34
      for (i=14; i<34; i++)
      {
         cardCode <<=1;
         cardCode |= databits[i];
      }
      //led();
      printBits();
      // Serial.println("caso 35 bits");
    }
    else if (bitCount == 26) {
    //Serial.println("caso 26 bits");
      // standard 26 bit format
      // facility code = bits 2 to 9
      for (i=1; i<9; i++)
      {
         facilityCode <<=1;
         facilityCode |= databits[i];
      }
 
      // card code = bits 10 to 23
      for (i=9; i<25; i++)
      {
         cardCode <<=1;
         cardCode |= databits[i];
      }
      //led();
      printBits();  
      //Serial.println("caso 26 bits");
    }
    /*else if (bitCount == 32){
      // Serial.println("caso 32 bits");
      // standard 32 bit format
      // facility code = bits 2 to 9
      for (i=1; i<8; i++)
      {
         facilityCode <<=1;
         facilityCode |= databits[i];
      }
 
      // card code = bits 10 to 31
      for (i=8; i<32; i++)
      {
         cardCode <<=1;
         cardCode |= databits[i];
      }
      printBits();
      // Serial.println("caso 32 bits");
    }*/

    else {
      // you can add other formats if you want!
     // Serial.println("No se pudo leer, vuelva a intentarlo."); 
     //led();
     digitalWrite(alarma, HIGH);
     delay(150);
     digitalWrite(alarma, LOW);
     delay(150);
     digitalWrite(alarma, HIGH);
     delay(150);
     digitalWrite(alarma, LOW);
     delay(150);
     digitalWrite(alarma, HIGH);
     delay(150);
     digitalWrite(alarma, LOW);
     delay(150);
    }
 
     // cleanup and get ready for the next card
     bitCount = 0;
     facilityCode = 0;
     cardCode = 0;
     for (i=0; i<MAX_BITS; i++) 
     {
       databits[i] = 0;
     }
  }
}
 
void printBits()
{
      // I really hope you can figure out what this function does
      Serial.print(" RFID = ");
      Serial.println(cardCode);
      Serial.print(bitCount);
      Serial.println(" bits");
}
