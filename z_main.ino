
void loop() {

  check_ota();

  check_mqtt_reset();

  delay(200);                      // wait one second

  if (myDFPlayer.available()) {
    printDetail(myDFPlayer.readType(), myDFPlayer.read()); //Print the detail message from DFPlayer to handle different errors and states.
  }

  check_DFPlayer();

}

