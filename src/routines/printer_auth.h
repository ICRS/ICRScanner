#include "./../nfc.h"
#include "./../wifi.h"
#include "./../display.h"

int NFClastRead = 0;

void displayPrintAuth(int status, String uid){
    clearScreen();
    tft.setCursor(12, 12);

    tft.print(uid);
    tft.drawWideLine(12.0f, 40.0f, 228.0f, 40.0f, 2.0f, TFT_WHITE);

    tft.setCursor(12, 50);

    String message = String(status) + ": Unknown Error";

    switch (status)
    {
    case 200:
        message = "Authenticated!";
        break;
    case -1:
        message = "-1: Can't Find Server";
        break;
    case 204:
        message = "204: Can't Find Card!";
        break;
    case 401:
        message = "401: Not Inducted!";
        break;
    }

    tft.print(message);
    Serial.println(message);
};

void printerAuthRoutine(){
    Serial.println("printerAuthRoutine");

    if (NFClastRead < millis() - WAIT_DELAY) {
        if (!cleared)
        {
            clearScreen();
            cleared = true;
        }
        waitingScreen("AUTH");
    }

    String uid = readNFC();
    if (uid == "") return;
    displayLoading(uid);

    int status = sendToPrintWindow(uid);

    NFClastRead = millis();
    cleared = false;
    displayPrintAuth(status, uid);
}
