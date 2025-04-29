#include "./../nfc.h"
#include "./../wifi.h"
#include "./../display.h"

int lastRead = 0;
int WAIT_DELAY = 5000;
bool cleared = false;

void displayUserInfo(UserInfo user, String uid){
    clearScreen();
    tft.setCursor(12, 12);
    tft.print(uid);
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
    Serial.println("getInfoRoutine");

    if (lastRead < millis() - WAIT_DELAY) {
        if (!cleared)
        {
            clearScreen();
            cleared = true;
        }
        waitingScreen("INFO");
    }

    String uid = readNFC();
    if (uid == "") return;
    displayLoading(uid);

    UserInfo user = getUserInfo(uid);

    displayUserInfo(user, uid);
    lastRead = millis();
    cleared = false;

    Serial.println("F");

}
