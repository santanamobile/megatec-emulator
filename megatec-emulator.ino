/*
** 2019-03-25
** @santanamobile
*/
#include <U8x8lib.h>
#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define SERBAUD       2400
#define PINVOLT_BATT  A0
#define PINVOLT_AC    A1
#define PINLED        13
#define PINSENSOR     2
#define TOTALWATT     300

#define INFO_COMPANY TSHARA
#define INFO_MODEL   POWER123
#define INFO_VERSION V1.0

U8X8_SH1106_128X64_NONAME_HW_I2C u8x8(U8X8_PIN_NONE);
OneWire ds(PINSENSOR);
DallasTemperature ds18x20(&ds);
float temperature = 0.0f;
uint8_t menu_redraw_required = 0;
bool testInProgress = false;
bool invalidCommand = false;
bool shutdownStarted = false;
unsigned int shutdownCounter = 0;

int StatusBit7 = 0, // Utility Fail (Immediate)
StatusBit6 = 0, // Battery Low
StatusBit5 = 0, // Bypass/Boost or Buck Active
StatusBit4 = 0, // UPS Failed
StatusBit3 = 0, // UPS Type is Standby (0 is On_line)
StatusBit2 = 0, // Test in Progress
StatusBit1 = 0, // Shutdown Active
StatusBit0 = 0; // Beeper On

void StatusDataStream();

enum testType {
  TEST_10SEC,
  TEST_UNTIL_BATTERY_LOW,
  TEST_FOR_X_MINUTES
} UPS_test;

void setup()
{
  Serial.begin(SERBAUD);
//  Serial.setTimeout(5000);
//  Serial.onReceive(MegatecProtocolParser);
  u8x8.begin();
  Wire.begin();
//  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.setFont(u8x8_font_amstrad_cpc_extended_f);
  u8x8.drawString(4, 2, "Megatec");
  u8x8.drawString(4, 3, "Protocol");
  u8x8.drawString(4, 4, "Simulator");
  menu_redraw_required = 1;

}

void loop()
{
  if(testInProgress) {
    
  }
}

void serialEvent()
{

  String MegatecBuffer = "";

  if (Serial.available() > 0) {
    MegatecBuffer = Serial.readStringUntil('\r');
  }

  MegatecBuffer.toUpperCase();
  char cmd = MegatecBuffer.charAt(0);
  int cmdLen = MegatecBuffer.length();

  u8x8.clear();
  u8x8.setCursor(0,1);
//  u8x8.println(MegatecBuffer);

  switch(cmd) {
    case 'Q': // Status Inquiry
      if (cmdLen == 1) { // Turn On/Off beep
        StatusBit0 = !StatusBit0;
        digitalWrite(PINLED, StatusBit0);
//        u8x8.setCursor(0,2);
        u8x8.print("Beep: ");
        u8x8.println(StatusBit0);
      } else {
        if (cmdLen == 2) {
          if (MegatecBuffer.charAt(1) == '1') {
          u8x8.print("Status");
            StatusDataStream();
          }
        }
      }
      break;

    case 'T': // 10 Seconds Test
      if (cmdLen == 1) {
        StatusBit2 = true;
        testInProgress = true;
        u8x8.println("Test for");
        u8x8.println("10 seconds");
      } else {
        if (cmdLen == 2) { // Test until Battery Low
          if (MegatecBuffer.charAt(1) == 'L') {
            u8x8.println("Test until");
            u8x8.println("Battery low");
          }
        }
        if (cmdLen == 3) { // Test for Specified Time Period
          if (MegatecBuffer.charAt(1) == 'L') {
            u8x8.println("Test until");
            u8x8.println("Time = n");
          }
        }
      }
      break;

    case 'S': // Shut Down Command
      if (cmdLen == 1) {
        notImplemented();
        shutdownStarted = true;
        StatusBit1 = 1;
      } else {
        if (cmdLen >= 2) {
          int rIndex = MegatecBuffer.indexOf('R');
//          if (MegatecBuffer.charAt(1) == 'R') {
//            testInProgress = false;
//          }
        }
      }
      break;

    case 'C': // Cancel Shut Down Command
      if (cmdLen == 1) { // Cancel the SN<n><cr> and SN<n>R<m><cr> command.
        u8x8.println("Cancel");
        u8x8.println("Shutdown");
        shutdownStarted = false;
        StatusBit1 = 0;
      } else {
        if (cmdLen == 2) {
          if (MegatecBuffer.charAt(1) == 'T') { // Cancel all test activity
            u8x8.println("Cancel");
            u8x8.println("All tests");
            testInProgress = false;
            StatusBit2 = 0;
          }
        }
      }
      break;

    case 'I': // UPS Information Command
      if (cmdLen == 1) {
        Serial.print("Company_Name UPS_Model Version\r");
        u8x8.println("INFO_COMPANY");
        u8x8.println("INFO_MODEL");
        u8x8.println("INFO_VERSION");
      } else {
        invalidCommand = true;
      }
      break;

    case 'F': // UPS Rating Information
      if (cmdLen == 1) { // #MMM.M QQQ SS.SS RR.R
        Serial.print("#127.0 090 12.21 60.1\r");
        u8x8.println("Rating");
        u8x8.println("Information");
        u8x8.println("Fake values");
      } else {
        invalidCommand = true;
      }
      break;

    default: // UnknownCommand
      invalidCommand = true;
      break;
  }

  if (invalidCommand) {
    Serial.print(MegatecBuffer);
    Serial.write('\r');
    u8x8.println("Invalid");
    u8x8.println("Command");
  }

  invalidCommand = false;
  Serial.flush();
}
