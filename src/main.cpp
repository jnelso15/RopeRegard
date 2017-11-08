
#include <SPI.h>
#include <RH_RF69.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>

#ifdef __AVR__
  #include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN            5

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      2

//Buttons
#define kButtonOnePin   A2
#define kButtonTwoPin   A3
#define kButtonThreePin A4
#define kButtonFourPin  A5

// 434 is our frequency
#define RF69_FREQ 434.0

//ADAFRUIT FEATHER
/*#if defined (__AVR_ATmega32U4__) // Feather 32u4 w/Radio
  #define RFM69_CS      8
  #define RFM69_INT     7
  #define RFM69_RST     4
#endif
*/

//FEATHERWING
#define RFM69_CS  10   // "B"
#define RFM69_RST 11   // "A"
#define RFM69_INT 2    // "SDA" (only SDA/SCL/RX/TX have IRQ!)

// Singleton instance of the radio driver
RH_RF69 rf69(RFM69_CS, RFM69_INT);

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 500; // delay for half a second

void setup() {
    Serial.begin(115200);
    //while (!Serial) { delay(1); } // wait until serial console is open, remove if not teathered to computer
  
  pixels.begin(); // This initializes the NeoPixel library.

  //Buttons
  pinMode(kButtonOnePin, INPUT);
  pinMode(kButtonTwoPin, INPUT);
  pinMode(kButtonThreePin, INPUT);
  pinMode(kButtonFourPin, INPUT);

  //Radio
  Serial.println("Feather RFM69 RX/TX Test!");
  
  // manual reset
  digitalWrite(RFM69_RST, HIGH);
  delay(10);
  digitalWrite(RFM69_RST, LOW);
  delay(10);
    
  if (!rf69.init()) {
    Serial.println("RFM69 radio init failed");
    while (1);
    }
  Serial.println("RFM69 radio init OK!");
    
  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM (for low power module)
  // No encryption
  if (!rf69.setFrequency(RF69_FREQ)) {
    Serial.println("setFrequency failed");
  }
// The encryption key has to be the same as the one in the server
uint8_t key[] = { 0xAF, 0xDD, 0xFF, 0xAF, 0xDA, 0x0F, 0xD3, 0xC7,
  0xC3, 0x3F, 0x5E, 0x2F, 0x1C, 0x5E, 0x55, 0x89};
rf69.setEncryptionKey(key);

// Antenna Power Level
rf69.setTxPower(20, true);

Serial.print("RFM69 radio @");  Serial.print((int)RF69_FREQ);  Serial.println(" MHz");

  
}

void loop() {

  //TRANSMITER
  if(digitalRead(kButtonOnePin)) {

    Serial.println("Button A pressed!");
    
    char radiopacket[20] = "Button #";
    radiopacket[8] = 'A';
    radiopacket[9] = 0;

    Serial.print("Sending "); Serial.println(radiopacket);
    rf69.send((uint8_t *)radiopacket, strlen(radiopacket));
    rf69.waitPacketSent();


    pixels.setPixelColor(0, pixels.Color(150,0,100)); // Purple
    pixels.show(); 
  } else if (digitalRead(kButtonTwoPin)) {
    Serial.println("Button B pressed!");
    
    char radiopacket[20] = "Button #";
    radiopacket[8] = 'B';
    radiopacket[9] = 0;

    Serial.print("Sending "); Serial.println(radiopacket);
    rf69.send((uint8_t *)radiopacket, strlen(radiopacket));
    rf69.waitPacketSent();

    pixels.setPixelColor(1, pixels.Color(0,66,37)); // British Racing Green
    pixels.show(); 
    delay(delayval);
  } else if (digitalRead(kButtonTwoPin)) {
    Serial.println("Button C pressed!");
    
    char radiopacket[20] = "Button #";
    radiopacket[8] = 'C';
    radiopacket[9] = 0;

    Serial.print("Sending "); Serial.println(radiopacket);
    rf69.send((uint8_t *)radiopacket, strlen(radiopacket));
    rf69.waitPacketSent();

    pixels.setPixelColor(1, pixels.Color(255,246,0)); // Yellow.
    pixels.show();
    delay(delayval);
  } else if (digitalRead(kButtonTwoPin)) {
    Serial.println("Button D pressed!");
    
    char radiopacket[20] = "Button #";
    radiopacket[8] = 'D';
    radiopacket[9] = 0;

    Serial.print("Sending "); Serial.println(radiopacket);
    rf69.send((uint8_t *)radiopacket, strlen(radiopacket));
    rf69.waitPacketSent();

    pixels.setPixelColor(1, pixels.Color(42,82,190)); // Moderately bright green color.
    pixels.show(); 
    delay(delayval);
  } else {
    

    pixels.setPixelColor(0, pixels.Color(0,0,0)); // OFF
    pixels.show(); 
  }
  
  if (rf69.waitAvailableTimeout(100)) {
    // Should be a message for us now   
    uint8_t buf[RH_RF69_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    

    

    //RECIEVE
    if (!rf69.recv(buf, &len)) {
      Serial.println("Receive failed");
      return;
    }

    rf69.printBuffer("Received: ", buf, len);
    buf[len] = 0;
    
    Serial.print("Got: "); Serial.println((char*)buf);
    if((String((char*)buf)).indexOf("A") != -1) {
      pixels.setPixelColor(0, pixels.Color(150,0,100)); // Purple
      pixels.show(); // This sends the updated pixel color to the hardware.
      delay(delayval);
    } else if((String((char*)buf)).indexOf("B") != -1) {
      pixels.setPixelColor(1, pixels.Color(0,66,37)); // British Racing Green
      pixels.show(); // This sends the updated pixel color to the hardware.
      delay(delayval);
    } else if((String((char*)buf)).indexOf("C") != -1) {
      pixels.setPixelColor(1, pixels.Color(255,246,0)); // Yellow.
      pixels.show(); // This sends the updated pixel color to the hardware.
      delay(delayval);
    } else if((String((char*)buf)).indexOf("D") != -1) {
      pixels.setPixelColor(1, pixels.Color(42,82,190)); // Moderately bright green color.
      pixels.show(); // This sends the updated pixel color to the hardware.
      delay(delayval);
    } else {
      pixels.setPixelColor(1, pixels.Color(0,0,0)); // OFF
      pixels.show(); // This sends the updated pixel color to the hardware.
    }
    Serial.print("RSSI: "); Serial.println(rf69.lastRssi(), DEC);
  }

}