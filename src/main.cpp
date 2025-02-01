#include <ELECHOUSE_CC1101_SRC_DRV.h>

byte transmit_bytes[11] = {72,101,108,108,111,32,87,111,114,108,100};
char *transmit_char = "Hello World";

void setup() {
    Serial.begin(115200);

    if (ELECHOUSE_cc1101.getCC1101()) {        // Check the CC1101 Spi connection.
        Serial.println("Connection OK");
    }
    else {
        Serial.println("Connection Error");
    }

    ELECHOUSE_cc1101.Init();              // must be set to initialize the cc1101!
    ELECHOUSE_cc1101.setCCMode(1);       // set config for internal transmission mode.
    ELECHOUSE_cc1101.setModulation(2);  // set modulation mode. 0 = 2-FSK, 1 = GFSK, 2 = ASK/OOK, 3 = 4-FSK, 4 = MSK.
    ELECHOUSE_cc1101.setMHZ(433.92);   // Here you can set your basic frequency. The lib calculates the frequency automatically (default = 433.92).The cc1101 can: 300-348 MHZ, 387-464MHZ and 779-928MHZ. Read More info from datasheet.
    ELECHOUSE_cc1101.setSyncMode(2);  // Combined sync-word qualifier mode. 0 = No preamble/sync. 1 = 16 sync word bits detected. 2 = 16/16 sync word bits detected. 3 = 30/32 sync word bits detected. 4 = No preamble/sync, carrier-sense above threshold. 5 = 15/16 + carrier-sense above threshold. 6 = 16/16 + carrier-sense above threshold. 7 = 30/32 + carrier-sense above threshold.
 // ELECHOUSE_cc1101.setPA(10);      // set TxPower. The following settings are possible depending on the frequency band.  (-30  -20  -15  -10  -6    0    5    7    10   11   12) Default is max!
    ELECHOUSE_cc1101.setCrc(1);     // 1 = CRC calculation in TX and CRC check in RX enabled. 0 = CRC disabled for TX and RX.

    Serial.println("Tx Mode");
}

void loop() {

    Serial.println("Transmit byte format.");
    ELECHOUSE_cc1101.SendData(transmit_bytes, 11, 100);
    delay(2000);

    Serial.println("Transmit from char format.");
    ELECHOUSE_cc1101.SendData(transmit_char, 100);
    delay(1000);

    Serial.println("Transmit from char format directly.");
    ELECHOUSE_cc1101.SendData("Hello World", 100);
    delay(2000);
}
