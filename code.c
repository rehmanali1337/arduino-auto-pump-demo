// These are included for rfid
#include <MFRC522Extended.h>
#include <SPI.h>
#include <MFRC522.h>


// For working with strings
#include <String.h>

// For handling input/output
#include <stdio.h>

// For handling the nextion Display
#include "EasyNextionLibrary.h"

// For flow meter reading
#include <FlowMeter.h>  // https://github.com/sekdiy/FlowMeter


// Now we define some global variables here.
#define LCD_OUTPUT_PIN   0
#define LCD_INPUT_PIN   1
#define FLOW_SENSOR_INPUT_PIN 2
#define RST_PIN         9           // Configurable, see typical pin layout above
#define SS_PIN          10          // Configurable, see typical pin layout above
#define MOTOR_RELAY_PIN  7
#define BUZZER_PIN  4



MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

MFRC522::MIFARE_Key key;

const int DEFAULT_BAUD_RATE = 9600;
// const String NEXTION_SERIEL = "NX4827T043";



// Initialize the nextion
EasyNex nextionDisplay(Serial);
FlowMeter *Meter;

// Some global variables
byte Card1ID[] = {74, 06, 246, 63};
char password[10];
char starPassword[10];

// Can define valid password here.
char validPassword[] = "243132";

// Program will set this to true when password will get matched
bool passwordMatched = false;

// Current page to display
int pageNow = 0;

// 
int lastPage = 0;

// 
char amount[10];
bool amountEntered = false;


uint32_t currentProgress = 0;
// set the measurement update period to 1s (1000 ms)
const unsigned long period = 1000;


long lastTime = 0;

// For stopping the motor
bool stopMotor = false;

float currentVolume = 0.0;
float lastTotalVolume = 0.0;


float lastProgressBarValue = 0.0;


int passwordLength() {
// For finding the current length of password
  int count = 0;
  int len = 10;
  for (int i = 0; i < len; i++) {
    if (password[i] != '\0') {
      count ++;
    };
  };
  return count;
}


void ringBuzzer(int t) {
  // Responsible for ringing the buzzer for t milli seconds
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, HIGH);
  delay(t);
  digitalWrite(BUZZER_PIN, LOW);
}

void resetStarPassword() {
  for (int i = 0; i < 10; i++) {
    starPassword[i] = '\0';
  }
}
void resetPasswordArray() {
  for (int i = 0; i < 10; i++) {
    password[i] = '\0';
  }
}

void resetPassword() {
  resetPasswordArray();
  resetStarPassword();

}

void showPassword() {
  int elements_in_x = passwordLength();
  resetStarPassword();
  for (int i = 0; i < elements_in_x; i++) {
    strcat(starPassword, "*");
  }
  nextionDisplay.writeStr("password.t1.txt", starPassword);
}


void showAmount() {
  nextionDisplay.writeStr("amount.t1.txt", amount);
}


// Trigger functions for nextion buttons
void trigger0() {
  // Will be executed when "printh 23 02 54 00" is executed on button click even in nextion buttons.
  // Keypad Button 1.
  Serial.print("Keypad 1 pressed!");
  strcat(password, "1");
};

void trigger1() {
  // Will be executed when "printh 23 02 54 00" is executed on button click even in nextion buttons.
  // Keypad Button 2.
  strcat(password, "2");


  //  nextionDisplay.writeStr("t1.txt", );
}

void trigger2() {
  // Will be executed when "printh 23 02 54 00" is executed on button click even in nextion buttons.
  // Keypad Button 3.
  Serial.print("Keypad 3 pressed!");
  strcat(password, "3");
}
void trigger3() {
  // Will be executed when "printh 23 02 54 00" is executed on button click even in nextion buttons.
  // Keypad Button 4.
  Serial.print("Keypad 4 pressed!");
  strcat(password, "4");
}
void trigger4() {
  // Will be executed when "printh 23 02 54 00" is executed on button click even in nextion buttons.
  // Keypad Button 5.
  Serial.print("Keypad 5 pressed!");
  strcat(password, "5");
}
void trigger5() {
  // Will be executed when "printh 23 02 54 00" is executed on button click even in nextion buttons.
  // Keypad Button 6.
  Serial.print("Keypad 6 pressed!");
  strcat(password, "6");
}

void trigger6() {
  // Will be executed when "printh 23 02 54 00" is executed on button click even in nextion buttons.
  // Keypad Button 7.
  Serial.print("Keypad 7 pressed!");
  strcat(password, "7");
}

void trigger7() {
  // Will be executed when "printh 23 02 54 00" is executed on button click even in nextion buttons.
  // Keypad Button 8.
  Serial.print("Keypad 8 pressed!");
  strcat(password, "8");
}


void trigger8() {
  // Will be executed when "printh 23 02 54 00" is executed on button click even in nextion buttons.
  // Keypad Button 9.
  Serial.print("Keypad 9 pressed!");
  strcat(password, "9");
}

void trigger9() {
  // Will be executed when "printh 23 02 54 00" is executed on button click even in nextion buttons.
  // Keypad Button 0.
  Serial.print("Keypad 0 pressed!");
  strcat(password, "0");
}
void trigger10() {
  // Will be executed when "printh 23 02 54 00" is executed on button click even in nextion buttons.
  // Keypad Button Enter.
  Serial.print("Keypad Enter pressed!");
  Serial.println();
  Serial.println();
  Serial.print(password);
  Serial.print(validPassword);
  int result = strcmp(password , validPassword);
  //  Serial.print(result);
  if (result == 0) {
    Serial.print("Correct Password");
    Serial.println();
    passwordMatched = true;
  }
  else {
    Serial.print("Invalid Password!");
    ringBuzzer(3000);
  }
  Serial.println();
  resetPassword();
}

void trigger11() {
  // Will be executed when "printh 23 02 54 00" is executed on button click even in nextion buttons.
  // Keypad Button Clear.
  Serial.print("Keypad Clear pressed!");
  resetPassword();
}


// Trigger functions for nextion amount buttons
void trigger12() {
  // Will be executed when "printh 23 02 54 00" is executed on button click even in nextion buttons.
  // Keypad Button 1.
  Serial.print("Keypad 1 pressed!");
  strcat(amount, "1");
};

void trigger13() {
  // Will be executed when "printh 23 02 54 00" is executed on button click even in nextion buttons.
  // Keypad Button 2.
  strcat(amount, "2");
}

void trigger14() {
  // Will be executed when "printh 23 02 54 00" is executed on button click even in nextion buttons.
  // Keypad Button 3.
  Serial.print("Keypad 3 pressed!");
  strcat(amount, "3");
}
void trigger15() {
  // Will be executed when "printh 23 02 54 00" is executed on button click even in nextion buttons.
  // Keypad Button 4.
  Serial.print("Keypad 4 pressed!");
  strcat(amount, "4");
}
void trigger16() {
  // Will be executed when "printh 23 02 54 00" is executed on button click even in nextion buttons.
  // Keypad Button 5.
  Serial.print("Keypad 5 pressed!");
  strcat(amount, "5");
}
void trigger17() {
  // Will be executed when "printh 23 02 54 00" is executed on button click even in nextion buttons.
  // Keypad Button 6.
  Serial.print("Keypad 6 pressed!");
  strcat(amount, "6");
}

void trigger18() {
  // Will be executed when "printh 23 02 54 00" is executed on button click even in nextion buttons.
  // Keypad Button 7.
  Serial.print("Keypad 7 pressed!");
  strcat(amount, "7");
}

void trigger19() {
  // Will be executed when "printh 23 02 54 00" is executed on button click even in nextion buttons.
  // Keypad Button 8.
  Serial.print("Keypad 8 pressed!");
  strcat(amount, "8");
}


void trigger20() {
  // Will be executed when "printh 23 02 54 00" is executed on button click even in nextion buttons.
  // Keypad Button 9.
  Serial.print("Keypad 9 pressed!");
  strcat(amount, "9");
}

void trigger21() {
  // Will be executed when "printh 23 02 54 00" is executed on button click even in nextion buttons.
  // Keypad Button 0.
  Serial.print("Keypad 0 pressed!");
  strcat(amount, "0");
}
void trigger22() {
  // Will be executed when "printh 23 02 54 00" is executed on button click even in nextion buttons.
  // Keypad Button Enter.
  Serial.print("Keypad Enter pressed on amount!");
  Serial.println();
  amountEntered = true;

}

void trigger23() {
  // Will be executed when "printh 23 02 54 00" is executed on button click even in nextion buttons.
  // Keypad Button Clear.
  Serial.print("Keypad Clear pressed on amount!");
  amount[0] = 0;
}

bool checkValidCardUID(byte *buffer) {
  // Check if card uid is a valid/authorized ID.
  bool result = false;
  for (byte i = 0; i < sizeof(buffer); i++) {
    if (buffer[i] != Card1ID[i]) {
      return false;
    };
  };
  return true;
};

void dump_byte_array(byte *buffer, byte bufferSize) {
  // Print all the bytes array elements to serial monitor
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i]);
    //        Serial.print(buffer[i], HEX);
  };
  Serial.println();
};
bool RFIDPresentAndValid() {
  //  Check if we have an RFID available then return true/false
  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return false;
  }
  Serial.print("Card detected!");
  Serial.println();


  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial())
    return false;

  // Show some details of the PICC (that is: the tag/card)
  Serial.print(F("Card UID:"));
  dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
  //      Serial.println();
  if (checkValidCardUID(mfrc522.uid.uidByte)) {
    Serial.print("Card authorized!");
    Serial.println();
    return true;
  }
  Serial.print("Unauthorized Card!");
  Serial.println();
  return false;
}


void waitForRFID() {
  // Wait until someone scans the RFID card.
  bool check = true;
  while (check == true) {
    if ( ! mfrc522.PICC_IsNewCardPresent()) {
      check = true;
      continue;
    }
    // Select one of the cards
    if ( ! mfrc522.PICC_ReadCardSerial()) {
      check = true;
      continue;
    }
    check = false;
  }

    // A card has been detected. Valid/Invalid

    // Check if the card is valid
  if (checkValidCardUID(mfrc522.uid.uidByte)) {
    // The card is valid or authorized
    Serial.println("Card authorized!");
    nextionDisplay.writeStr("page 1");
    pageNow = 1;
    return;
  }

  // Card is invalid

  nextionDisplay.writeStr("home.t0.txt", "Invalid RFID!");
  nextionDisplay.writeStr("home.t0.txt", "Invalid RFID!");


  ringBuzzer(1000);
  delay(3000);


  nextionDisplay.writeStr("home.t0.txt", "Scan Your RFID");
  return waitForRFID();

}


void turnOnMotor() {
  digitalWrite(MOTOR_RELAY_PIN, LOW);
}


void turnOffMotor() {
  digitalWrite(MOTOR_RELAY_PIN, HIGH);
}

void renderCurrentPage() {

    // Show the currently selected page

  if (pageNow != lastPage) {

    switch (pageNow) {
      case 0:
        nextionDisplay.writeStr("page 0");
        break;

      case 1:
        nextionDisplay.writeStr("page 1");
        break;

      case 2:
        nextionDisplay.writeStr("page 2");
        break;

      case 3:
        //Show the progressbar page
        nextionDisplay.writeStr("page 3");
        break;

    }
    lastPage = pageNow;
  }
}

void updateProgressBar() {
  int value = currentVolume * 100.0;

  Serial.println(String(value));


  nextionDisplay.writeNum("n0.val", value);
  nextionDisplay.writeNum("n0.val", value);

  float fAmount = ((float)atof(amount)) / 100.0;
  int pValue = value / fAmount;

  Serial.println(pValue);


  nextionDisplay.writeNum("j0.val", pValue);
  nextionDisplay.writeNum("j0.val", pValue);
  nextionDisplay.writeNum("j0.val", pValue);
}

void MeterISR() {
  // let our flow meter count the pulses
  Meter->count();
}

void displayFlowSensorReading() {
  long currentTime = millis();
  long duration = currentTime - lastTime;


  // process the counted ticks
  Meter->tick(duration);
  float fAmount = ((float)atof(amount)) / 100.0;
  Serial.println(fAmount);


  // output some measurement result
  Serial.println("FlowMeter - current flow rate: " + String(Meter->getCurrentFlowrate()) + " l/min, " +
                 "nominal volume: " + String(Meter->getTotalVolume()) + " l, " +
                 "compensated error: " + String(Meter->getCurrentError()) + " %, " +
                 "duration: " + String(Meter->getTotalDuration() / 1000) + " s.");

  // prepare for next cycle
  lastTime = currentTime;

  currentVolume = String(Meter->getTotalVolume()).toFloat() - lastTotalVolume;
  if (currentVolume >= fAmount) {
    currentVolume = fAmount;
    turnOffMotor();
    lastTotalVolume = String(Meter->getTotalVolume()).toFloat();
//    currentVolume = 0.0;
    stopMotor = true;
    Meter->reset();
//    amount[0] = 0;
    return;
  }

  //  }
}

void setup() {
    // This function will be run once whenever the arduino starts
  //  Setup serial monitor for debugging
  Serial.begin(15200);
  while (!Serial);

//   Start the nextion display
  nextionDisplay.begin(9600);
//   Now we set the page 0 on the display
  nextionDisplay.writeStr("page 0");
  nextionDisplay.lastCurrentPageId = 100;


  Serial.print("Setting up arduino ..");
  Serial.println();

//   Start the SPI Bus (From docs)
  SPI.begin();        // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522 card

  pinMode(MOTOR_RELAY_PIN, OUTPUT);
  turnOffMotor();



  // Setup the flow sensor.
  FlowSensorProperties MySensor = {60, 8, {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};
  Meter = new FlowMeter(digitalPinToInterrupt(FLOW_SENSOR_INPUT_PIN), MySensor, MeterISR, RISING);


  Serial.print("Arduino setup complete!");
  Serial.println();
};


void loop() {
    // Responds to touch clicks (from docs)
  nextionDisplay.NextionListen();

  renderCurrentPage();

  switch (pageNow) {

    case 0:
    // It means in pageNow we have value of 0
      waitForRFID();
      break;

    case 1:
    // It means in pageNow we have value of 1
      showPassword();
      if (passwordMatched == true) {
        Serial.print("Password matched!");
        Serial.println();
        lastPage = pageNow;
        pageNow = 2;
        passwordMatched = false;
        nextionDisplay.writeStr("page 2");
      };
      break;

    case 2:
    // It means in pageNow we have value of 2
      // On amount enter page.
      showAmount();
      if (amountEntered == true) {
        amountEntered = false;
        Serial.print("Amount has been entered!");
        Serial.print(amount);
        Serial.println();

        lastPage = pageNow;
        pageNow = 3;
        nextionDisplay.writeStr("page 3");
        //        amount[0] = 0;
        stopMotor = false;
        turnOnMotor();
      }
      break;

    case 3: {
    // It means in pageNow we have value of 3

        if (stopMotor == true) {
            // We need to stop the motor now
          lastPage = pageNow;
          pageNow = 0;
          turnOffMotor();
          ringBuzzer(2000);
          nextionDisplay.writeStr("page 0");
          currentProgress = 0;
          amount[0] = 0;
          break;
        }
        else {
          Serial.println("Filling up..");
          displayFlowSensorReading();
          //          currentProgress += 5;
          updateProgressBar();
          //          delay(100);
        }
        break;
      }
  }
//   Out of switch case statement
}