#include <esp_now.h>
#include <WiFi.h>
#include <Joystick_ESP32S2.h>
#include <ResponsiveAnalogRead.h>

// Setting var for reading potentiometer values
int pot_X_val = 0;
int pot_Y_val = 0;
int pot_Z_val = 0;
// Setting var for storing current button
int but_1_state = 0;
int but_2_state = 0;
int but_3_state = 0;
int but_4_state = 0;
int but_5_state = 0;

struct PacketData
{
  byte pot_X_val;
  byte pot_Y_val;
  byte pot_Z_val;
  byte but_1_state;
  byte but_2_state;
  byte but_3_state;
  byte but_4_state;
  byte but_5_state;
};
PacketData received_data;

// Configuriung Joystick HID Device Layout
Joystick_ Joystick(0x03,JOYSTICK_TYPE_JOYSTICK,
5,      // Button Count: Number of Buttons shown to the HID system
0,      // Hat Switch Count: Number of Hat switches.  Max is 2
false,   // Include X Axis
false,   // Include Y Axis
false,   // Include Z Axis
true,  // Include X Rotation
true,  // Include Y Rotation
true,  // Include Z Rotation
false,  // Include Rudder
false,  // Include Throttle
false,  // Include Accelerator
false,  // Include Brake
false); // Include Steering

const bool initAutoSendState = true; // Sends state immediately, otherwise Joystick.sendState would need to be called to update the game controller state


//callback for received data
void onDataReceive(const uint8_t * mac, const uint8_t *incoming_data, int len)
{
  if (len ==0)
  {
    return;
  }
  memcpy(&received_data, incoming_data, sizeof(received_data));
  String input_data ;
  input_data = input_data + 
  "Pot X =  " + received_data.pot_X_val  + "\n" + 
  "Pot Y =  " + received_data.pot_Y_val  + "\n" + 
  "Pot Z =  " + received_data.pot_Z_val  + "\n" + 
  "But 1 =  " + received_data.but_1_state + "\n" +
  "But 2 =  " + received_data.but_2_state + "\n" +
  "But 3 =  " + received_data.but_3_state + "\n" +
  "But 4 =  " + received_data.but_4_state + "\n" +
  "But 5 =  " + received_data.but_5_state + "\n";
  //Serial.print(input_data);

  pot_X_val = received_data.pot_X_val;
  pot_Y_val = received_data.pot_Y_val;
  pot_Z_val = received_data.pot_Z_val;
  but_1_state = received_data.but_1_state;
  but_2_state = received_data.but_2_state;
  but_3_state = received_data.but_3_state;
  but_4_state = received_data.but_4_state;
  but_5_state = received_data.but_5_state;

}

void setup() {
  //initiate Serial
  Serial.begin(115200);
  // Initialize Joystick Library
  Joystick.begin();
  //WiFi Setup
  WiFi.mode(WIFI_STA);

  //ESP-NOW init
  if(esp_now_init() !=ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  //Receive Callback
  esp_now_register_recv_cb(onDataReceive);
}

void loop() {
  Joystick.setRxAxis(pot_X_val);
  Joystick.setRyAxis(pot_Y_val);
  Joystick.setRzAxis(pot_Z_val);
  Joystick.setButton(0, but_1_state);
  Joystick.setButton(1, but_2_state);
  Joystick.setButton(2, but_3_state);
  Joystick.setButton(3, but_4_state);
  Joystick.setButton(4, but_5_state);
}
