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
    if (status == 200)
    {
        tft.print("Authenticated!");
        Serial.println("Authenticated!");
    }
    else
    {
        tft.print("User not found");
        Serial.println("User not found");
    }
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
    displayPrintAuth(status, uid);
}
