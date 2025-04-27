#include "./../nfc.h"
#include "./../wifi.h"
#include "./../display.h"

int NFClastRead = 0;

void displayPrintAuth(int status){
    clearScreen();
    tft.setCursor(12, 12);

    tft.print(card_uid);
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
    if (NFClastRead < millis() - 2000)
    {
        waitingScreen();
    }

    String uid = readNFC();

    if (uid == "")
    {
        Serial.println("No NFC detected");
        return;
    }

    NFClastRead = millis();
    int status = sendToPrintWindow(uid);
    displayPrintAuth(status);
}
