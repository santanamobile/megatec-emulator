/*
void MegatecProtocolParser(String data)
{
  
}
*/

void StatusDataStream()
{
// (208.4 140.0 208.4 034 59.9 2.05 35.0  00110000
  String strDataStream = "";

  double inputVoltage = map(analogRead(PINVOLT_AC), 0, 1023, 90, 230);
  double batteryVoltage = map(analogRead(PINVOLT_BATT), 0, 1023, 9, 15);
//  float batteryVoltage = random(11, 12);
  double outputVoltage = 127;
  int faultVoltage = random(18, 20);
  int freq = random(58, 62);
  int percent = (inputVoltage - (inputVoltage - faultVoltage) / 2);
  ds18x20.requestTemperatures();
  temperature = ds18x20.getTempCByIndex(0);
  String stringStatusByte = String(StatusBit7) + String(StatusBit6) + String(StatusBit5) + String(StatusBit4) + String(StatusBit3) + String(StatusBit2) + String(StatusBit1) + String(StatusBit0);
  strDataStream += "(";
  strDataStream += String(inputVoltage) + " ";
  strDataStream += String(inputVoltage - faultVoltage) + " ";
  strDataStream += String(outputVoltage) + " ";
  strDataStream += String(percent) + " ";
  strDataStream += String(freq) + " ";
  strDataStream += String(batteryVoltage) + " ";
  strDataStream += String(temperature) + " ";
  strDataStream += String(stringStatusByte) + "\r";
  Serial.print(strDataStream);
}
