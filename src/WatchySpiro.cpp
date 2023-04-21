#include "WatchySpiro.h"
#include <Fonts/FreeMonoBold12pt7b.h>

const uint8_t BATTERY_DISPLAY_START_Y = 34;
const uint8_t BATTERY_DISPLAY_START_X = 40;
const uint8_t BATTERY_DISPLAY_HEIGHT = 4;
const uint8_t BATTERY_SEGMENT_WIDTH = 20;

const bool DARKMODE = false;

void WatchySpiro::drawWatchFace(){
    display.fillScreen(DARKMODE ? GxEPD_BLACK : GxEPD_WHITE);
    display.setTextColor(DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    display.setFont(&FreeMonoBold12pt7b); // ? width, 15px high

    syncTime();
    drawBackground();
    drawTime();
    drawDate();
    drawSteps();
    drawBattery();

    //turn off radios
    WiFi.mode(WIFI_OFF);
    btStop();
}

void WatchySpiro::syncTime(){
    if(currentTime.Hour == 5 && currentTime.Minute == 0) {
        if(connectWiFi()){
            syncNTP();
        }
    }
}

void WatchySpiro::drawBackground(){
    Serial.begin(9600);
    drawSpiral(3, 75, 20);
    drawSpiral(5, 50, 10);
}

void WatchySpiro::drawSpiral(int strokeSize, int maxRadius, int maxRatio) {
    int cx = 100;
    int cy = 90;
    int r1 = random(5, maxRadius);
    int r2 = maxRadius - r1;
    int ratio1 = random(1, 5);
    int ratio2 = random(1, maxRatio);
    float pi = 3.14159;
    
    for (float i = 0; i < 2*pi; i += 0.001) {
        int x = cx + r1 * cos(i * ratio1) + r2 * cos(-i * ratio2);
        int y = cy + r1 * sin(i * ratio1) + r2 * sin(-i * ratio2);
        display.drawRect(x, y, strokeSize, strokeSize, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    }
    
    char dimensions[50];
    sprintf(dimensions, "Dimensions: r1=%d r2=%d ratio1=%d ratio2=%d", r1, r2, ratio1, ratio2);
    Serial.println(dimensions);
}

void WatchySpiro::drawTime(){
    display.setCursor(0, 195);
    if(currentTime.Hour < 10){
        display.print('0');
    }
    display.print(currentTime.Hour);
    display.print(':');
    if(currentTime.Minute < 10){
        display.print('0');
    }
    display.print(currentTime.Minute);
}

void WatchySpiro::drawDate(){
    display.setCursor(131, 195);
    uint8_t day = currentTime.Day;
    if (day < 10){
        display.print("0");
    }
    display.print(day);
    display.print("/");
    uint8_t month = currentTime.Month;
    if (month < 10){
        display.print("0");
    }
    display.print(month);
}

void WatchySpiro::drawSteps(){
    // reset step counter at midnight
    if (currentTime.Hour == 0 && currentTime.Minute == 0){
      sensor.resetStepCounter();
    }
    display.setCursor(0, 15);
    display.println(sensor.getCounter());
}

void WatchySpiro::drawBattery(){
    float batt = (getBatteryVoltage()-3.3)/0.9;
    display.drawLine(0,199,200*batt,199,GxEPD_BLACK);
}
