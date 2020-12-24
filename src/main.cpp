/*
 * This sketch programs Ardunio to communicate with Artisan using MODBUS protocol and
 * an inexpensive thermocouple amplifier.
 *
 * Hardware:
 *  Arduino UNO
 *  Thermocouple amplifier (MAX6675)
 *  K-type thermocouple
 *
 * Libraries needed:
 *  https://github.com/adafruit/MAX6675-library and;
 *  https://github.com/smarmengol/Modbus-Master-Slave-for-Arduino
 *
 * Based on:
 *  serialthermocouple arduino example
 *  https://github.com/hallgeirl/coffee-roaster/blob/4d4fa73c9831b0ff3f28752c454011ab70410de8/src/roaster/roaster.ino#L112-L136
 */

#include <max6675.h>
#include <ModbusRtu.h>

// data array for modbus network sharing:
uint16_t au16data[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1 };

Modbus slave;

// declare variables for Arduino pins connected to the MAX data pins:
int thermoDO_bt = 6;  // SO
int thermoCS_bt = 5;
int thermoCLK_bt = 4;  // SCK
int thermoVCC_bt = 3;
int thermoGND_bt = 2;

MAX6675 thermocouple_bt(thermoCLK_bt, thermoCS_bt, thermoDO_bt);

int thermoDO_amb = 12;
int thermoCS_amb = 11;
int thermoCLK_amb = 10;  // SCK
int thermoVCC_amb = 9;
int thermoGND_amb = 8;

MAX6675 thermocouple_amb(thermoCLK_amb, thermoCS_amb, thermoDO_amb);

void setup() {
  // configure the pins to power MAX6675
  pinMode(thermoVCC_bt, OUTPUT); digitalWrite(thermoVCC_bt, HIGH);
  pinMode(thermoGND_bt, OUTPUT); digitalWrite(thermoGND_bt, LOW);

  pinMode(thermoVCC_amb, OUTPUT); digitalWrite(thermoVCC_amb, HIGH);
  pinMode(thermoGND_amb, OUTPUT); digitalWrite(thermoGND_amb, LOW);

  slave = Modbus(1, 0, 0); // MODBUS object declaration: (1 = slave #1, 0 = RS232, 0 = RS232)
  slave.begin(19200); // 19200 baud, 8-bits, none, 1-bit stop

  delay(500);
}

void loop() {
   au16data[2] = ((uint16_t) thermocouple_bt.readCelsius() * 100);
   au16data[3] = ((uint16_t) thermocouple_amb.readCelsius() * 100);

   slave.poll(au16data, 16);

   // For the MAX6675 to update, you must delay AT LEAST 250ms between reads!
   delay(200);
}
