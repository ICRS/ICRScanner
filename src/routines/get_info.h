#include "./../nfc.h"
#include "./../wifi.h"
#include "./../display.h"

int lastRead = 0;

void displayUserInfo(UserInfo user){
    clearScreen();
    tft.setCursor(12, 12);

    tft.print(card_uid);
    tft.drawWideLine(12.0f, 40.0f, 228.0f, 40.0f, 2.0f, TFT_WHITE);

    tft.setCursor(12, 50);
    if (user.found)
    {
        tft.print("Shortcode: " + user.shortcode);
        tft.setCursor(12, 82);
        String v = user.inducted ? "True" : "False";
        tft.print("Inducted:  " + v);
    }
    else
    {
        tft.print("User not found");
        Serial.println("User not found");
    }
};

void getInfoRoutine(){
    if (lastRead < millis() - 2000)
    {
        waitingScreen();
    }

    String uid = readNFC();

    if (uid == "")
    {
        Serial.println("No NFC detected");
        return;
    }

    lastRead = millis();
    UserInfo user = getUserInfo(uid);
    displayUserInfo(user);
}
