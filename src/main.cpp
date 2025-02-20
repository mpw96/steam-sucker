// SPDX-License-Identifier: GPL-3.0-only

/*
 * Steam Sucker - ESP8266 firmware to make an Elica cooker hood smart.
 * Copyright (C) 2025 Marc-Philip Werner
 */

#include <ELECHOUSE_CC1101_SRC_DRV.h>
#include <Homie.h>

// homie firmware
#define FW_NAME_HOMIE "steam-sucker-controller"
#define FW_VERSION_HOMIE "0.0.3"

// pins for CC1101
#define CC1101_GDO0 5
#define CC1101_GDO2 4

// cooker hood signals for light and fan, recorded via flipper zero from original remote control
#define LENGTH_LIGHT_TOGGLE 183
const int LIGHT_TOGGLE_DATA[LENGTH_LIGHT_TOGGLE] = {248556,-4600,509,-532,541,-822,235,-466,575,-486,575,-782,273,-464,571,-788,277,-796,279,-752,273,-446,609,-772,295,-408,641,-740,291,-15430,341,-760,287,-418,651,-720,307,-762,287,-406,639,-430,615,-752,307,-398,651,-418,613,-748,313,-408,647,-748,309,-724,315,-760,283,-406,649,-750,309,-418,613,-746,325,-15408,359,-728,319,-386,649,-742,299,-748,321,-378,655,-390,657,-748,309,-398,647,-386,679,-714,317,-402,659,-718,313,-722,329,-746,289,-406,655,-722,321,-416,653,-718,321,-15418,359,-730,309,-386,667,-712,327,-740,321,-374,669,-378,671,-708,321,-400,655,-392,653,-748,309,-398,647,-742,315,-732,317,-738,285,-418,639,-740,319,-396,653,-720,319,-256432,131,-1112,97,-2468,97,-2154,165,-2450,65,-366,365,-200,135,-438,305,-100,565,-66,265,-166,231,-166,127,-262,129,-100,63,-66,197,-298,421,-198,293,-66,131,-232,355,-130,2123};

#define LENGTH_LIGHT_OFF 275
const int LIGHT_OFF_DATA[LENGTH_LIGHT_OFF] = {664998,-13358,195,-844,193,-518,515,-566,507,-524,551,-460,579,-15466,309,-768,309,-400,641,-752,317,-728,321,-386,651,-386,679,-716,323,-380,655,-394,657,-748,309,-398,653,-710,349,-722,311,-730,317,-404,633,-428,637,-404,653,-386,635,-15426,355,-732,335,-356,667,-714,353,-702,319,-400,667,-380,665,-714,319,-400,667,-380,667,-712,319,-404,657,-722,321,-740,317,-726,317,-398,659,-388,659,-382,657,-382,683,-15412,345,-718,327,-410,635,-744,291,-734,321,-402,653,-392,669,-730,287,-420,653,-388,669,-728,287,-420,635,-742,319,-726,321,-728,321,-398,649,-406,653,-402,655,-400,621,-15442,393,-698,337,-384,637,-744,325,-704,321,-402,671,-378,669,-708,321,-398,669,-376,669,-710,321,-402,655,-722,321,-744,319,-722,321,-396,649,-406,653,-402,655,-398,621,-15442,393,-702,313,-424,643,-714,325,-740,291,-404,647,-408,653,-734,319,-392,655,-388,669,-714,317,-412,635,-736,321,-724,319,-730,319,-398,655,-390,655,-384,687,-382,645,-15444,351,-720,327,-412,637,-744,287,-732,321,-400,669,-378,667,-710,321,-402,655,-392,667,-734,317,-388,669,-708,321,-728,319,-730,319,-398,655,-392,667,-400,655,-384,635,-258300,65,-330,65,-98,97,-1244,167,-66,101,-334,99,-672,267,-200,265,-496,65,-268,99,-436,99,-198,199,-100,131,-66,567,-168,433,-66,903,-66,67};

#define LENGTH_LIGHT_ON 255
const int LIGHT_ON_DATA[LENGTH_LIGHT_ON] = {643950,-34338,205,-848,241,-480,559,-806,269,-776,263,-472,577,-472,611,-742,289,-436,615,-428,607,-764,319,-394,651,-718,319,-744,319,-724,321,-398,651,-722,319,-744,319,-724,321,-15426,359,-732,321,-388,647,-720,327,-714,323,-404,657,-392,665,-728,319,-392,653,-390,657,-712,345,-398,653,-712,311,-730,347,-734,287,-416,669,-708,319,-728,319,-732,319,-15426,361,-700,355,-388,647,-720,325,-712,323,-404,657,-392,655,-744,295,-402,665,-402,653,-716,317,-408,637,-738,317,-724,321,-726,321,-400,655,-722,317,-744,319,-728,319,-15464,313,-760,319,-392,657,-718,321,-740,319,-392,653,-404,653,-714,317,-412,635,-410,655,-718,321,-412,635,-736,321,-726,319,-728,319,-398,655,-720,319,-746,319,-726,321,-15462,355,-692,355,-374,675,-702,337,-728,313,-400,665,-364,661,-750,309,-396,651,-386,679,-714,315,-400,661,-724,319,-734,313,-754,295,-414,647,-710,321,-764,291,-732,319,-15464,353,-694,357,-372,677,-700,337,-728,315,-398,665,-368,693,-710,315,-374,697,-378,659,-714,315,-410,665,-712,317,-730,321,-728,321,-398,653,-724,321,-744,319,-722,321,-215910,65,-2026,131,-422,197,-426,161,-520,129,-588,195,-1220,65,-332,201,-236,65,-298,97,-196,921,-66,801,-68,433};

#define LENGTH_FAN_OFF 199
const int FAN_OFF_DATA[LENGTH_FAN_OFF] = {520856,-720,321,-374,649,-740,319,-726,319,-396,649,-408,651,-730,321,-388,667,-378,649,-738,321,-392,651,-736,319,-392,651,-734,319,-392,653,-732,319,-392,651,-734,319,-15420,359,-700,339,-388,633,-752,321,-708,321,-400,649,-408,651,-732,319,-390,651,-402,653,-730,319,-390,651,-734,319,-392,649,-732,319,-394,649,-738,319,-392,647,-740,319,-15420,359,-704,351,-356,681,-718,321,-708,321,-400,647,-410,651,-734,317,-390,651,-404,651,-732,319,-388,669,-710,319,-396,649,-738,319,-392,649,-736,319,-392,649,-738,319,-15394,357,-732,321,-392,653,-714,331,-748,321,-374,667,-382,647,-738,317,-396,667,-378,669,-710,321,-400,647,-740,319,-394,651,-732,321,-392,651,-736,319,-392,647,-738,319,-290100,65,-164,63,-96,225,-128,129,-96,161,-98,387,-130,259,-436,133,-200,335,-66,65,-66,163,-296,393,-132,455,-200,267,-100,99,-132,133,-470,201,-68,101,-68,163,-98,301,-102,1295,-100,571,-100,525,-100,299};

#define LENGTH_FAN_MINUS 293
const int FAN_MINUS_DATA[LENGTH_FAN_MINUS] = {60654,-70,2665,-66,143031,-66,254185,-738,335,-382,637,-746,321,-740,287,-402,669,-378,667,-710,319,-402,655,-392,669,-730,319,-388,635,-740,321,-396,653,-720,317,-414,637,-408,653,-388,669,-398,621,-15444,311,-754,299,-416,637,-744,291,-736,319,-400,669,-378,669,-710,321,-398,655,-392,669,-730,319,-392,651,-720,317,-412,637,-736,319,-394,655,-390,653,-384,653,-414,649,-15412,327,-748,325,-376,655,-724,321,-744,319,-394,653,-390,669,-730,287,-418,653,-390,669,-724,287,-418,637,-740,321,-396,651,-720,321,-414,635,-408,651,-388,669,-400,619,-15410,389,-714,299,-420,645,-710,321,-730,319,-402,653,-392,655,-714,329,-402,637,-428,635,-734,317,-394,669,-706,319,-398,667,-708,321,-400,667,-378,669,-378,669,-378,667,-15438,355,-726,317,-372,687,-718,313,-722,327,-416,645,-378,655,-720,321,-412,637,-406,653,-718,319,-414,653,-714,319,-412,635,-738,319,-394,653,-390,667,-400,651,-384,637,-15458,359,-700,341,-384,637,-744,325,-742,289,-404,653,-392,653,-748,309,-398,649,-386,679,-714,313,-404,647,-742,315,-372,667,-722,319,-412,637,-408,653,-388,653,-384,677,-15442,355,-716,299,-418,647,-710,323,-730,321,-400,651,-392,653,-748,309,-398,647,-386,681,-714,315,-404,659,-724,319,-386,653,-718,329,-414,647,-412,613,-408,649,-404,653,-285094,67,-1132,561,-98,97,-66,131,-132,99,-332,231,-330,65,-196,885,-166,267,-134,133,-100,399};

#define LENGTH_FAN_PLUS 195
const int FAN_PLUS_DATA[LENGTH_FAN_PLUS] = {43506,-68,1233,-100,1483,-712,301,-416,649,-742,289,-764,289,-434,617,-404,639,-766,287,-430,613,-428,613,-746,317,-412,635,-734,319,-392,653,-720,319,-414,639,-738,319,-754,289,-730,321,-15426,359,-706,313,-420,615,-748,327,-740,291,-436,617,-394,653,-750,307,-400,649,-386,643,-750,313,-400,629,-772,317,-370,653,-750,309,-418,613,-744,293,-772,291,-768,287,-15432,359,-702,315,-418,615,-748,327,-740,291,-436,619,-394,665,-718,317,-412,637,-406,655,-720,317,-412,637,-736,321,-394,653,-718,317,-414,637,-736,317,-728,321,-728,321,-15424,359,-702,353,-386,649,-716,325,-746,289,-404,649,-408,653,-720,321,-410,637,-404,653,-718,317,-412,637,-738,319,-396,651,-720,317,-414,637,-736,321,-724,319,-732,321,-270246,65,-3110,523,-166,199,-134,133,-170,303,-332,395,-134,403,-168,531,-396,131,-98,195,-66,393,-296,129,-98,165,-164,231,-66,99,-98,63,-164,1057,-64,353,-98,1077,-66,393};

void initCC1101() {
    ELECHOUSE_cc1101.Init();
    ELECHOUSE_cc1101.setGDO(CC1101_GDO0, CC1101_GDO2);
    ELECHOUSE_cc1101.setMHZ(433.92);
    ELECHOUSE_cc1101.SetTx();
    ELECHOUSE_cc1101.setModulation(2); // OOK (on-off-keying)
    ELECHOUSE_cc1101.setPA(5);  // -30, -20, -15, -10, 0, 5, 7, 10 (default is 10, which is max)

    if (ELECHOUSE_cc1101.getCC1101()) {
        Serial.println("Connection to CC1101 OK");
    }
    else {
        Serial.println("Connection to CC1101 Error");
    }
}

// sending data to CC1101
void sendData(const int data[], const int dataLength) {
    // if the number is positive, we send a 1 and delay for the amount
    // if the number is negative, we send a 0 and delay for the amount
    for (int i=0; i < dataLength; ++i) {
        byte sendMe = 1;
        int delay = data[i];
        if(delay < 0) {
            // DONT TRANSMIT
            sendMe = 0;
        }
        delay = abs(delay);

        digitalWrite(CC1101_GDO0, sendMe);
        delayMicroseconds(delay);
    }

    // STOP TRANSMITTING
    digitalWrite(CC1101_GDO0, 0);
}

void sendLightToggle() {
    Serial.println("cc1101-sending toggle light.");
    sendData(LIGHT_TOGGLE_DATA, LENGTH_LIGHT_TOGGLE);
    Serial.println("Done.");
}

void sendLightOn() {
    Serial.println("cc1101-sending light on.");
    sendData(LIGHT_ON_DATA, LENGTH_LIGHT_ON);
    Serial.println("Done.");
}

void sendLightOff() {
    Serial.println("cc1101-sending light off.");
    sendData(LIGHT_OFF_DATA, LENGTH_LIGHT_OFF);
    Serial.println("Done.");
}

void sendFanOff() {
    Serial.println("cc1101-sending fan off.");
    sendData(FAN_OFF_DATA, LENGTH_FAN_OFF);
    Serial.println("Done.");
}

void sendFanMinus() {
    Serial.println("cc1101-sending fan minus.");
    sendData(FAN_MINUS_DATA, LENGTH_FAN_MINUS);
    Serial.println("Done.");
}

void sendFanPlus() {
    Serial.println("cc1101-sending fan plus.");
    sendData(FAN_PLUS_DATA, LENGTH_FAN_PLUS);
    Serial.println("Done.");
}

// this is representing the current state of the light and the fan
bool steamsuckerLightIsOn(false);
bool steamSuckerFanIsOn(false);
long steamSuckerFanSpeed(1); // 1 - slow, ..., 4 - max
long steamSuckerFanPercentage(25); // 1 - slow, ..., 100 - max

HomieNode steamsuckerLightNode("light", "Licht", "switch");
HomieNode steamsuckerFanNode("sucker", "Ventilator", "fan");

const String TRUE_STRING("true");
const String FALSE_STRING("false");

bool steamsuckerLightOnOffHandler(const HomieRange& range, const String& on) {
    Homie.getLogger() << "steamsuckerLightOnOffHandler called." << endl;
    if (on != TRUE_STRING && on != FALSE_STRING) {
        Homie.getLogger() << "unrecognised value " << on << ", doing nothing." << endl;
        return false;
    }

    steamsuckerLightIsOn = (on == TRUE_STRING);

    // this is the actual switching of the light
    if (steamsuckerLightIsOn) {
        sendLightOn();
    }
    else {
        sendLightOff();
    }

    // send the current light status
    Homie.getLogger() << "homie-sending light is now " << (steamsuckerLightIsOn ? "on" : "off") << "." << endl;
    steamsuckerLightNode.setProperty("on").send(steamsuckerLightIsOn ? TRUE_STRING : FALSE_STRING);

    return true;
}

bool manageFan(bool fanOnWanted, long fanSpeedWanted) {
    // either on/off changes or speed changes, never both
    if (!fanOnWanted && !steamSuckerFanIsOn) {
        // fan is off and not touched, just set new preset
        Homie.getLogger() << "just setting fan speed variable to " << fanSpeedWanted << ", because the fan is off and we leave it off." << endl;
        steamSuckerFanSpeed = fanSpeedWanted;
        steamsuckerFanNode.setProperty("speed").send(String(steamSuckerFanSpeed));
        return true;
    }

    long accelerationSteps(0);
    long decelerationSteps(0);

    if (fanOnWanted) {
        if (!steamSuckerFanIsOn) {
            // fan is being switched on, we must do all acceleration steps
            steamSuckerFanSpeed = 0;
        }

        if (fanSpeedWanted>steamSuckerFanSpeed) {
            accelerationSteps = fanSpeedWanted - steamSuckerFanSpeed;
            Homie.getLogger() << "accelerating fan speed from " << steamSuckerFanSpeed << " to " << fanSpeedWanted << "." << endl;
        }

        if (fanSpeedWanted<steamSuckerFanSpeed) {
            decelerationSteps = steamSuckerFanSpeed-fanSpeedWanted;
            Homie.getLogger() << "reducing fan speed from " << steamSuckerFanSpeed << " to " << fanSpeedWanted << "." << endl;
        }

        for (; accelerationSteps>0; --accelerationSteps) {
            sendFanPlus();
        }

        for (; decelerationSteps>0; --decelerationSteps) {
            sendFanMinus();
        }
        steamSuckerFanIsOn = true;
    }
    else {
        steamSuckerFanIsOn = false;
        sendFanOff();
    }

    steamsuckerFanNode.setProperty("on").send(steamSuckerFanIsOn ? TRUE_STRING : FALSE_STRING);
    steamSuckerFanSpeed = fanSpeedWanted;
    Homie.getLogger() << "reached desired fan speed " << steamSuckerFanSpeed << "." << endl;
    steamsuckerFanNode.setProperty("speed").send(String(steamSuckerFanSpeed));
    return true;
}

bool steamsuckerFanOnOffHandler(const HomieRange& range, const String& on) {
    Homie.getLogger() << "steamsuckerFanOnOffHandler called." << endl;
    if (on != TRUE_STRING && on != FALSE_STRING) {
        Homie.getLogger() << "unrecognised value " << on << ", doing nothing." << endl;
        return false;
    }

    bool fanOnWanted = (on == TRUE_STRING);
    return manageFan(fanOnWanted, steamSuckerFanSpeed);
}

bool steamsuckerFanPresetHandler(const HomieRange& range, const String& preset) {
    Homie.getLogger() << "steamsuckerFanPresetHandler called." << endl;
    for (byte i = 0; i < preset.length(); ++i) {
        if (!isDigit(preset.charAt(i))) {
            Homie.getLogger() << "unrecognised value " << preset << ", doing nothing." << endl;
            return false;
        }
    }
    long wantedSpeed = preset.toInt();
    if (wantedSpeed<1 || wantedSpeed>4) {
        Homie.getLogger() << "value " << wantedSpeed << " unknown, must be in interval [1,4], doing nothing." << endl;
        return false;
    }

    steamSuckerFanPercentage = wantedSpeed * 25;
    steamsuckerFanNode.setProperty("percentage").send(String(steamSuckerFanPercentage));
    return manageFan(steamSuckerFanIsOn, wantedSpeed);
}

bool steamsuckerFanPercentageHandler(const HomieRange& range, const String& percentage) {
    Homie.getLogger() << "steamsuckerFanPercentageHandler called." << endl;
    for (byte i = 0; i < percentage.length(); ++i) {
        if (!isDigit(percentage.charAt(i))) {
            Homie.getLogger() << "unrecognised value " << percentage << ", doing nothing." << endl;
            return false;
        }
    }
    long wantedPercentage = percentage.toInt();
    if (wantedPercentage<1 || wantedPercentage>100) {
        Homie.getLogger() << "value " << wantedPercentage << " is not a valid percentage [1,100], doing nothing." << endl;
        return false;
    }

    // convert percentage [1,100] to preset [1,4]
    long wantedSpeed(-1);
    if (wantedPercentage < 38) {
        wantedSpeed = 1;
    }
    else if (wantedPercentage < 63) {
        wantedSpeed = 2;
    }
    else if (wantedPercentage < 88) {
        wantedSpeed = 3;
    }
    else {
        wantedSpeed = 4;
    }

    Homie.getLogger() << "mapping " << wantedPercentage << " percent to speed " << wantedSpeed << "." << endl;
    steamSuckerFanPercentage = wantedPercentage;
    steamsuckerFanNode.setProperty("percentage").send(String(steamSuckerFanPercentage));
    if (steamSuckerFanSpeed == wantedSpeed) {
        // the fan already has the speed where it's wanted to be
        Homie.getLogger() << "fan has already speed " << steamSuckerFanSpeed << ", doing nothing." << endl;
        return true;
    }
    return manageFan(steamSuckerFanIsOn, wantedSpeed);
}

bool initialStateSet(false);
void homieLoopFunction() {
    if (initialStateSet) {
        return;
    }
    steamsuckerLightNode.setProperty("on").send(steamsuckerLightIsOn ? TRUE_STRING : FALSE_STRING);
    steamsuckerFanNode.setProperty("on").send(steamSuckerFanIsOn ? TRUE_STRING : FALSE_STRING);
    steamsuckerFanNode.setProperty("speed").send(String(steamSuckerFanSpeed));
    steamsuckerFanNode.setProperty("percentage").send(String(steamSuckerFanPercentage));
    initialStateSet = true;
}

void setup() {
    Serial.begin(115200);
    Serial.println();
    Serial.println();
    Serial.println("Starting setup...");

    initCC1101();
    Homie_setFirmware(FW_NAME_HOMIE, FW_VERSION_HOMIE);
    Homie.setLoopFunction(homieLoopFunction);

    steamsuckerLightNode.advertise("on").setName("On").setDatatype("boolean").settable(steamsuckerLightOnOffHandler);
    steamsuckerFanNode.advertise("on").setName("On").setDatatype("boolean").settable(steamsuckerFanOnOffHandler);
    steamsuckerFanNode.advertise("speed").setName("Speed").setDatatype("integer").setFormat("1:4").settable(steamsuckerFanPresetHandler);
    steamsuckerFanNode.advertise("percentage").setName("Percentage").setDatatype("integer").setFormat("1:100").settable(steamsuckerFanPercentageHandler);
    Homie.setup();
    sendLightOff();
    sendFanOff();
    Serial.println("Setup done.");
}

void loop() {
    Homie.loop();
}
