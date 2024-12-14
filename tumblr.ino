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

unsigned long startMillis;      // Arduino startup time in millis()
unsigned long currentMillis;    // Current millis()
unsigned long lastUpdateMillis; // Last update time in millis()
unsigned long lastDrinkMillis;

unsigned long elapsedTimeInSeconds;
unsigned long totalSeconds;
unsigned long totalMinutes;
unsigned long totalHours;

unsigned long lastDrinkElapsed;
unsigned long lastDrinkSeconds;
unsigned long lastDrinkMinutes;
unsigned long lastDrinkHours;

int initialHours, initialMinutes, initialSeconds; // Compile time variables
int currentHours, currentMinutes, currentSeconds; // Current time variables

// Water level and volume variables
long duration;
float distance;
float waterHeight;
float waterVolume_ml;
float waterVolume_mm3;

float previousVolume_ml = 0;
float currentVolume_ml = 0.0;
float drankVolume_ml = 0.0;
float drankAmount = 0.0;
bool firstMeasurement = true;

// State management
int state = 1;
int previousState = 0;

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
        Serial.print("FirstMeasurement, lastDrinkMillis: ");
        Serial.println(lastDrinkMillis);
    } else {
        drankAmount = previousVolume_ml - currentVolume_ml;

        // Update only if a significant drinking event is detected
        if (drankAmount >= 100.0) {
            drankVolume_ml += drankAmount;
            lastDrinkMillis = millis(); // Update lastDrinkTime only for drinking events
            previousVolume_ml = currentVolume_ml; // Update previous volume for further comparisons
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
}

// Function to update state based on last drink time
void updateState() {
    unsigned long elapsedSinceLastDrink = (currentMillis - lastDrinkMillis) / 1000; // Seconds since last drink

    if (elapsedSinceLastDrink >= (8 * SECONDS_IN_A_MINUTE)) {
        state = 4;
    } else if (elapsedSinceLastDrink >= (4 * SECONDS_IN_A_MINUTE)) {
        state = 3;
    } else if (elapsedSinceLastDrink >= (2 * SECONDS_IN_A_MINUTE)) {
        state = 2;
    } else {
        state = 1;
    }

    if (state != previousState) {
        LCD_DrawFace(state);
        previousState = state;
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

    // Calculate elapsed time in seconds since the start
    unsigned long elapsedLastDrinkSeconds = (lastDrinkMillis - startMillis) / 1000;

    // Add elapsed time to compile time
    unsigned long totalElapsedSeconds = initialSeconds + elapsedLastDrinkSeconds;
    unsigned long totalElapsedMinutes = initialMinutes + totalElapsedSeconds / SECONDS_IN_A_MINUTE;
    unsigned long totalElapsedHours = initialHours + totalElapsedMinutes / SECONDS_IN_A_MINUTE;

    // Calculate HH:MM:SS for last drink time
    lastDrinkSeconds = totalElapsedSeconds % SECONDS_IN_A_MINUTE;
    lastDrinkMinutes = totalElapsedMinutes % SECONDS_IN_A_MINUTE;
    lastDrinkHours = totalElapsedHours % HOURS_IN_A_DAY;

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
}
