#include <SPI.h>
#include "LCD_Driver.h"
#include "GUI_Paint.h"
#include "LCD_DrawFace.h"

#define TRIG_PIN 2
#define ECHO_PIN 3

// Tumbler Specifications
#define RADIUS 32.33
#define HEIGHT 180.0
#define MAX_VOLUME 591.0

// Time Calculations
#define SECONDS_IN_A_MINUTE 60
#define SECONDS_IN_AN_HOUR 3600
#define HOURS_IN_A_DAY 24

unsigned long startMillis = 0;      // Arduino startup time in millis()
unsigned long currentMillis = 0;    // Current millis()
unsigned long lastUpdateMillis = 0; // Last update time in millis()
unsigned long lastDrinkMillis = 0;
unsigned long lastScreenSwitchMillis = 0; // To track the last screen switch time

unsigned long elapsedTimeInSeconds = 0;
unsigned long totalSeconds = 0;
unsigned long totalMinutes = 0;
unsigned long totalHours = 0;

unsigned long lastDrinkElapsed = 0;
unsigned long lastDrinkSeconds = 0;
unsigned long lastDrinkMinutes = 0;
unsigned long lastDrinkHours = 0;

unsigned long elapsedSinceLastDrink = 0;
unsigned long elapsedLastDrinkSeconds = 0;

unsigned long totalElapsedSeconds = 0;
unsigned long totalElapsedMinutes = 0;
unsigned long totalElapsedHours = 0;

int initialHours = 0, initialMinutes = 0, initialSeconds = 0; // Compile time variables
int currentHours = 0, currentMinutes = 0, currentSeconds = 0; // Current time variables

int currentScreen = 0; // 0 for elapsedState screen, 1 for drankVolume screen

// Water level and volume variables
long duration = 0;
float distance = 0.0;
float waterHeight = 0.0;
float waterVolume_ml = 0.0;
float waterVolume_mm3 = 0.0;

float previousVolume_ml = 0;
float currentVolume_ml = 0.0;
float drankVolume_ml = 0.0;
float drankAmount = 0.0;
bool firstMeasurement = true;

// State management
int elapsedState = 1;
int prevElapsedState = 0;

// Function to calculate water height and volume
void calculateWaterVolume() {
    // Measure distance
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(5);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    // Calculate distance
    duration = pulseIn(ECHO_PIN, HIGH);
    distance = duration * 0.34 / 2;
    distance = constrain(distance, 2, HEIGHT); // 거리 값을 2mm ~ HEIGHT로 제한

    // Calculate water height
    waterHeight = HEIGHT - distance;
    waterHeight = constrain(waterHeight, 0, HEIGHT);

    // Calculate water volume
    waterVolume_mm3 = 3.141592 * RADIUS * RADIUS * waterHeight;
    waterVolume_ml = waterVolume_mm3 / 1000.0;
}

// Function to update drinking stats
void updateDrinkingStats() {
    if (firstMeasurement) {
        currentVolume_ml = waterVolume_ml;
        previousVolume_ml = waterVolume_ml;
        firstMeasurement = false;
        lastDrinkMillis = millis(); // Initialize only once during the first measurement
        // Serial.print("FirstMeasurement, lastDrinkMillis: ");
        // Serial.println(lastDrinkMillis);
    } else {
        drankAmount = previousVolume_ml - currentVolume_ml;
        // Update only if a significant drinking event is detected
        if (drankAmount >= 120.0 && drankAmount > 0) {
            drankVolume_ml += drankAmount;
            lastDrinkMillis = millis(); // Update lastDrinkTime only for drinking events
            previousVolume_ml = currentVolume_ml; // Update previous volume for further comparisons
            // drawDrankVolume(drankVolume_ml);
            Serial.println("Drinking event detected. Updating lastDrinkTime.");
        } else {
            // Serial.println("No significant drinking event. lastDrinkTime unchanged.");
        }
    }

    currentVolume_ml = waterVolume_ml; // Always update the current volume
}

// Function to update elapsed time and current time
void updateTime() {
    elapsedTimeInSeconds = (currentMillis - startMillis) / 1000;
    totalSeconds = initialSeconds + elapsedTimeInSeconds;
    totalMinutes = initialMinutes + totalSeconds / SECONDS_IN_A_MINUTE;
    totalHours = initialHours + totalMinutes / SECONDS_IN_A_MINUTE;

    currentSeconds = totalSeconds % SECONDS_IN_A_MINUTE;
    currentMinutes = totalMinutes % SECONDS_IN_A_MINUTE;
    currentHours = totalHours % HOURS_IN_A_DAY;

    // Calculate elapsed time in seconds since the start
    elapsedLastDrinkSeconds = (lastDrinkMillis - startMillis) / 1000;

    // Add elapsed time to compile time
    totalElapsedSeconds = initialSeconds + elapsedLastDrinkSeconds;
    totalElapsedMinutes = initialMinutes + totalElapsedSeconds / SECONDS_IN_A_MINUTE;
    totalElapsedHours = initialHours + totalElapsedMinutes / SECONDS_IN_A_MINUTE;

    // Calculate HH:MM:SS for last drink time
    lastDrinkSeconds = totalElapsedSeconds % SECONDS_IN_A_MINUTE;
    lastDrinkMinutes = totalElapsedMinutes % SECONDS_IN_A_MINUTE;
    lastDrinkHours = totalElapsedHours % HOURS_IN_A_DAY;
}

// Function to update state based on last drink time
void updateState() {
    elapsedSinceLastDrink = (currentMillis - lastDrinkMillis) / 1000; // Seconds since last drink

    if (elapsedSinceLastDrink >= (8 * SECONDS_IN_A_MINUTE)) {
        elapsedState = 4;
    } else if (elapsedSinceLastDrink >= (4 * SECONDS_IN_A_MINUTE)) {
        elapsedState = 3;
    } else if (elapsedSinceLastDrink >= (2 * SECONDS_IN_A_MINUTE)) {
        elapsedState = 2;
    } else {
        elapsedState = 1;
    }

    if (elapsedState != prevElapsedState) {
        // LCD_DrawFace(elapsedState);
        prevElapsedState = elapsedState;
    }
}


// Function to print debug information
void printDebugInfo() {
    lastDrinkElapsed = (currentMillis - lastDrinkMillis) / 1000; // Convert millis to seconds
    if (lastDrinkElapsed < 0) lastDrinkElapsed = 0; // Prevent negative values due to timing issues

    Serial.print("Distance (mm): ");
    Serial.println(distance);
    Serial.print("Water Height (mm): ");
    Serial.println(waterHeight);
    Serial.print("Water Volume (ml): ");
    Serial.println(waterVolume_ml);
    Serial.print("Drank Volume (ml): ");
    Serial.println(drankVolume_ml);

    // Debug Current Time
    Serial.print("Current Time (HH:MM:SS): ");
    Serial.print(currentHours);
    Serial.print(":");
    Serial.print(currentMinutes);
    Serial.print(":");
    Serial.println(currentSeconds);

    // Debug print for last drink time
    Serial.print("Last Drink Time (HH:MM:SS): ");
    Serial.print(lastDrinkHours);
    Serial.print(":");
    Serial.print(lastDrinkMinutes);
    Serial.print(":");
    Serial.println(lastDrinkSeconds);

    Serial.print("Elapsed Time Since Last Drink (seconds): ");
    Serial.println(lastDrinkElapsed);
    Serial.println("");
}

void setup() {
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    Serial.begin(9600);

    // Initialize time from compile time
    sscanf(__TIME__, "%d:%d:%d", &initialHours, &initialMinutes, &initialSeconds);

    // Initialize millis()
    startMillis = millis();
    lastUpdateMillis = millis();

    // Initialize LCD
    Config_Init();
    LCD_Init();
    LCD_SetBacklight(1000);
    Paint_NewImage(LCD_WIDTH, LCD_HEIGHT, 0, BLACK);

    // Print start time
    Serial.print("Start Time (HH:MM:SS): ");
    Serial.print(initialHours);
    Serial.print(":");
    Serial.print(initialMinutes);
    Serial.print(":");
    Serial.println(initialSeconds);
}

void loop() {
    currentMillis = millis();

    // Update every 5 seconds
    if (currentMillis - lastUpdateMillis >= 5000) {
        lastUpdateMillis = currentMillis;

        // Update water volume and drinking stats
        calculateWaterVolume();
        updateDrinkingStats();

        // Update time and state
        updateTime();
        updateState();

        // Print debug information
        printDebugInfo();
    }

    // Check if it's time to switch screens (every 15 seconds)
    if (currentMillis - lastScreenSwitchMillis >= 15000) {
        lastScreenSwitchMillis = currentMillis; // Update last screen switch time

        if (currentScreen == 0) {
            // Switch to drank volume screen
            // float sample_drankVolume_ml = 2000.0;
            // drawDrankVolume(sample_drankVolume_ml);
            drawDrankVolume(waterVolume_ml);
            currentScreen = 1; // Update current screen state
        } else {
            // Switch to elapsedState face screen
            LCD_DrawFace(elapsedState);
            currentScreen = 0; // Update current screen state
        }
    }
}

