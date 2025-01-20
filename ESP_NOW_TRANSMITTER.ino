#include <esp_now.h>
#include <WiFi.h>
//#include <ResponsiveAnalogRead.h>

// Setting Analog pins for reading potentiometer values
int pot_X = A0;
int pot_Y = A1;
int pot_Z = A2; 
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
// Setting var for storing previous button states
int but_1_prev_state = LOW;
int but_2_prev_state = LOW;
int but_3_prev_state = LOW;
int but_4_prev_state = LOW;
int but_5_prev_state = LOW;

// Defining button pins
#define but_1 D2
#define but_2 D3
#define but_3 D4
#define but_4 D5
#define but_5 D6

// Receiver MAC address
uint8_t receiver_address[] = {0xEC, 0xDA, 0x3B, 0x55, 0x0B, 0xA4};

// Packet structure for sending data
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
PacketData data;


// callback for sent data
void onDataSent (const uint8_t *mac_addr, esp_now_send_status_t status)
{
  Serial.print("\r\nLast Packet Send Status:\t ");
  Serial.println(status);
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Message sent" : "Message failed");
}

void setup () {
  // Initiate Serial
  Serial.begin(115200);
  // Set WiFi Mode
  WiFi.mode(WIFI_STA);
  // Initiate pin modes for buttons
  pinMode(but_1, INPUT_PULLUP);
  pinMode(but_2, INPUT_PULLUP);
  pinMode(but_3, INPUT_PULLUP);
  pinMode(but_4, INPUT_PULLUP);
  pinMode(but_5, INPUT_PULLUP);

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) 
  {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  else
  {
    Serial.println("Successfully Initialized ESP-NOW");
  }

  esp_now_register_send_cb(onDataSent);

  // Register Peer
  esp_now_peer_info_t peer_info;
  memset(&peer_info, 0, sizeof(peer_info));
  memcpy(peer_info.peer_addr, receiver_address, 6);
  peer_info.channel = 0;
  peer_info.encrypt = false;
  // Add Peer
  if (esp_now_add_peer(&peer_info) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
  else
  {
    Serial.println("Successfully added peer");
  }
}

void loop () {
  //read value from pin and map to 8bit range (must be mapped, otherwise will send weird values when in struct PacketData)
  data.pot_X_val = map(analogRead(pot_X),0,4096,0,255);
  data.pot_Y_val = map(analogRead(pot_Y),0,4096,0,255);
  data.pot_Z_val = map(analogRead(pot_Z),0,4096,0,255);


  // Read the state of the button, and check against previous state
  int but_1_state = !digitalRead(but_1);
  if (but_1_state != but_1_prev_state){
    data.but_1_state = but_1_state;
    but_1_prev_state = but_1_state;

  }
  int but_2_state = !digitalRead(but_2);
  if (but_2_state != but_2_prev_state){
    data.but_2_state = but_2_state;
    but_2_prev_state = but_2_state;
  }
  int but_3_state = !digitalRead(but_3);
  if (but_3_state != but_3_prev_state){
    data.but_3_state = but_3_state;
    but_3_prev_state = but_3_state;
  }
  int but_4_state = !digitalRead(but_4);
  if (but_4_state != but_4_prev_state){
    data.but_4_state = but_4_state;
    but_4_prev_state = but_4_state;
  }
  int but_5_state = !digitalRead(but_5);
  if (but_5_state != but_5_prev_state){
    data.but_5_state = but_5_state;
    but_5_prev_state = but_5_state;
  }
// Print data values to serial for reference
  Serial.print("Pot X = ");
  Serial.println(data.pot_X_val);
  Serial.print("Pot Y = ");
  Serial.println(data.pot_Y_val);
  Serial.print("Pot Z = ");
  Serial.println(data.pot_Z_val);
  Serial.print("But 1 = ");
  Serial.println(data.but_1_state);
  Serial.print("But 2 = ");
  Serial.println(data.but_2_state);
  Serial.print("But 3 = ");
  Serial.println(data.but_3_state);
  Serial.print("But 4 = ");
  Serial.println(data.but_4_state);
  Serial.print("But 5 = ");
  Serial.println(data.but_5_state);
//Send Data to ESP-NOW
  esp_err_t result = esp_now_send(receiver_address, (uint8_t *) &data, sizeof(data));
  if (result == ESP_OK) {
    Serial.println("Sent Successfully");
  } else {
    Serial.println("Error sending data");
  }
  delay(50);
}
