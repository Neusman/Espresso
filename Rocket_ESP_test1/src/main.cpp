#include <Arduino.h>
#include <main.h>


//create SSD1306 object here and pass a pointer to Display class
//SSD1306Wire oled(0x3c, 21, 22); //SDA pin 27, SCL pin 33
//Display Display1(&oled);

//Deep sleep:
bool _sleep=false;
unsigned long timedelay;

//Deep sleep:
void IRAM_ATTR isr_stop(){
  detachInterrupt(digitalPinToInterrupt(32)); //because later used for wake up
  //setting flag instead to get a little bit of delay
  _sleep=true;
  timedelay=millis();
}

//create Coffee Output pointer
Coffee coffee(25, 26, 0); // Example pins: Pump=D2(25), Valve=D3(26), LED=D5(0)

// Define the current state
Coffee_state currentState = NORMAL;

//create Temp sensor Kmeter
TempSens tempSens(&Wire, 21, 22); // SDA=21, SCL=22

//create 7seg
seg_counter seg7; // I2C address: 0x70


void setup() {
    Serial.begin(115200);

    // Initialize I2C FIRST (before any I2C devices)
    Wire.begin(21, 22); // SDA=21, SCL=22 (FireBeetle 2 ESP32-E)
    delay(100);

    // General setup
    LeverInterrupt::setup(17, CHANGE);
    Tank::setup(36);
    Boiler::setup(39);
    const uint8_t Sleep_pin = 15;
    coffee.NormalState();



    // Deep sleep setup
    _sleep = false;
    esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_ALL);
    pinMode(Sleep_pin, INPUT_PULLUP);
    Serial.println("Wake up!");
    attachInterrupt(digitalPinToInterrupt(Sleep_pin), isr_stop, FALLING);
    delay(500);


    // Initialize 7-segment display (AFTER Wire.begin)
    seg7.setupTube();
    Serial.println("7-segment display initialized.");
    delay(100);

    // Initialize KMeterISO (commented out for now)

    if (!tempSens.begin()) {
        Serial.println("Failed to initialize KMeterISO. Check connections!");
        while (1);
    }
    Serial.println("KMeterISO initialized successfully!");
    delay(100);


}


void loop() {
 
	//Deep sleep:
	if (_sleep && millis()-timedelay>1000) {
		Serial.println("Going to sleep now");
		//Display1.displayOff();
		esp_sleep_enable_ext0_wakeup(GPIO_NUM_15,0); //1 = High, 0 = Low connected to GPIO32
		esp_deep_sleep_start();
	}
	
	// Check for MAKING_COFFEE conditions first
    if (LeverInterrupt::checkTriggeredRising() && Tank::WaterIsOK()) {
        currentState = MAKING_COFFEE;
        coffee.MakeCoffee();
		Serial.println("Making coffee");
    }
 	// Handle state transitions
    switch (currentState) {
        case NORMAL:
            // Transition to FillBoiler if boiler water is NOK and tank water is OK
            if (Boiler::WaterIsNOK() && Tank::WaterIsOK()) {
                currentState = FILLING_BOILER;
                coffee.FillBoiler();
				Serial.println("Fill Boiler");
            }
            break;

        case MAKING_COFFEE:
            //start counting
			seg7.displayNumber(millis());
            delay(10);
			// Return to NormalState if lever falling
            if (LeverInterrupt::checkTriggeredFalling()) {
                currentState = NORMAL;
                coffee.NormalState();
                seg7.clear();
				Serial.println("----");
            }
            break;

        case FILLING_BOILER:
            // Return to NormalState if boiler water is OK
            if (Boiler::WaterIsOK()) {
                currentState = NORMAL;
                coffee.NormalState();
				Serial.println("----1");
            };
            if (Tank::WaterIsNOK()) {
                currentState = NORMAL;
                coffee.NormalState();
				Serial.println("Water empty during boiler fill");
            }
            break;
    }

    // Blink LED if tank water is NOK (regardless of state)
    // Always call WaterLevel_LED() and pass the blink condition
    coffee.WaterLevel_LED(Tank::WaterIsNOK());

  	// Read temperature in Celsius
    tempSens.checkAndPrintTemperature(millis());


}



