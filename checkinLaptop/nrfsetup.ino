#include <SPI.h>
#include "RF24.h"

                      // Set up nRF24L01 radio on SPI bus plus pins 7 & 8

const uint64_t pipes[2] = { 0xABCDABCD71LL, 0x544d52687CLL };   // Radio pipe addresses for the 2 nodes to communicate.

byte data[32];                           //Data buffer for testing data transfer speeds

unsigned long counter, rxTimer;          //Counter and timer for keeping track transfer info
unsigned long startTime, stopTime;  
bool TX=1,RX=0,role=0;

void nrfSetup(void) {

  Serial.begin(9600);

  radio.begin();                           // Setup and configure rf radio
  radio.setChannel(108);
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_250KBPS);
  radio.setAutoAck(1);                     // Ensure autoACK is enabled
  radio.setRetries(15,15);                  // Optionally, increase the delay between retries & # of retries
  radio.setCRCLength(RF24_CRC_8);          // Use 8-bit CRC for performance
  radio.openWritingPipe(pipes[0]);
  radio.openReadingPipe(1,pipes[1]);
  radio.startListening();                 // Start listening
  
  radio.powerUp();                        //Power up the radio
}

boolean receivedRFData(byte receivedData[], uint8_t receiveSize) { 
  if (radio.available()) {
    Sprintln("found data!");
    radio.read(receivedData, receiveSize);
    //dump_byte_array(receivedData, receiveSize); Sprintln();
    return true;
  }
  return false;  
}

boolean sendRFData(byte dataToSend[], uint8_t sendSize) {
  boolean sendStatus;
  radio.stopListening();
  sendStatus = radio.write(dataToSend, sendSize);
  radio.startListening();
  return sendStatus;
}
 
