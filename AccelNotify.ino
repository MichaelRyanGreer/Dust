/*
 * Copyright (c) 2016 Intel Corporation.  All rights reserved.
 * See the bottom of this file for the license terms.
 */

#include <CurieBLE.h>
#include <CurieIMU.h>

const int ledPin = 13; // set ledPin to on-board LED

BLEService accelService("19B10000-E8F2-537E-4F6C-D104768A1214"); // create service

// create button characteristic and allow remote device to get notifications
BLECharCharacteristic accelCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite); // allows remote device to get notifications

float prev;


void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT); // use the LED on pin 13 as an output

  CurieIMU.begin();
  CurieIMU.setAccelerometerRange(2);

  // begin initialization
  BLE.begin();

  // set the local name peripheral advertises
  BLE.setLocalName("accelNotify");
  // set the UUID for the service this peripheral advertises:
  BLE.setAdvertisedService(accelService);

  // add the characteristics to the service
  accelService.addCharacteristic(accelCharacteristic);

  // add the service
  BLE.addService(accelService);

  accelCharacteristic.setValue(1);

  // start advertising
  BLE.advertise();

  Serial.println("Bluetooth device active, waiting for connections...");

  digitalWrite(13, HIGH);

  prev = getVector();
}

void loop() {
  // poll for BLE events
  BLE.poll();

  float vec = getVector();
  if (vec > prev + 0.5 || vec < prev - 0.5) {
        accelCharacteristic.setValue(1);
  }
}

float getVector() {
  float ax, ay, az;
  CurieIMU.readAccelerometerScaled(ax, ay, az);
  float next = sqrt(10*(ax*ax + ay*ay + az*az));
  Serial.print("Vector: ");
  Serial.println(next);
  return next;
}

/*
  Copyright (c) 2016 Intel Corporation. All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-
  1301 USA
*/
  
