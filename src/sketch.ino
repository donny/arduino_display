#include <WiServer.h>
#include <SoftwareSerial.h>

// Wireless configuration --------------------------------------------

#define WIRELESS_MODE_INFRA	1
#define WIRELESS_MODE_ADHOC	2

#define WIRELESS_SEC_OPEN	0
#define WIRELESS_SEC_WEP	1
#define WIRELESS_SEC_WPA	2
#define WIRELESS_SEC_WPA2	3

unsigned char local_ip[] = {192,168,1,220}; // IP address of WiShield.
unsigned char gateway_ip[] = {192,168,1,1}; // Gateway IP.
unsigned char subnet_mask[] = {255,255,255,0}; // Subnet mask.
const char ssid[] PROGMEM = {"SSID"}; // SSID, max 32 bytes.

unsigned char security_type = WIRELESS_SEC_WPA2;

// Passphrase for WPA/WPA2, max 64 characters.
const char security_passphrase[] PROGMEM = {"PASSWORD"};

const unsigned char wep_keys[] PROGMEM = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

// Setup the wireless mode.
// Infrastructure - connect to AP.
// Adhoc - connect to another WiFi device.
unsigned char wireless_mode = WIRELESS_MODE_INFRA;

unsigned char ssid_len;
unsigned char security_passphrase_len;



// Display configuration --------------------------------------------

#define LCD_RX_PIN	4
#define LCD_TX_PIN	7

SoftwareSerial LCD(LCD_RX_PIN, LCD_TX_PIN); // They refer to the Arduino pins.



// Utility functions --------------------------------------------

void clearScreen() {
	// Clears the screen.
	LCD.write(0xFE);
	LCD.write(0x01); 
}

// Change the LCD backlight.
// 128 = OFF, 157 = Fully ON, everything inbetween = varied brightness. 
int backlight(int brightness) {
	LCD.write(0x7C); // Note the different command flag = 124 dec.
	LCD.write(brightness); // Any value between 128 and 157 or 0x80 and 0x9D.
	delay(100);
	clearScreen();
}

void selectLineOne() { 
	// Puts the cursor at line 1 char 0.
	LCD.write(0xFE); // Command flag.
	LCD.write(128); // Position.
}

void selectLineTwo() { 
	// Puts the cursor at line 2 char 0.
	LCD.write(0xFE); // Command flag.
	LCD.write(192); // Position.
}



// Miscellaneous functions --------------------------------------------

void processInfo(char *data, int len) {
	String dataStr = String(data);

	int hash1 = -1;
	int hash2 = -1;
	int hash3 = -1;
	
	hash1 = dataStr.indexOf("#");
	if (hash1 == -1) return;
	hash2 = dataStr.indexOf("#", hash1 + 1);
	if (hash2 == -1) return;
	hash3 = dataStr.indexOf("#", hash2 + 1);
	if (hash3 == -1) return;

	//Serial.println(dataStr.substring(hash1 + 1, hash2));
	//Serial.println(dataStr.substring(hash2 + 1, hash3));

	clearScreen();
	selectLineOne();
	LCD.print(dataStr.substring(hash1 + 1, hash2));
	selectLineTwo();
	LCD.print(dataStr.substring(hash2 + 1, hash3));
}



//  Global variables --------------------------------------------

uint8 ip[] = {127, 0, 0, 1}; // IP address for the web site.
GETrequest getInfo(ip, 80, "www.example.com", "/~donny/cgi-bin/info.cgi");

// 10 minutes. 1000 * 60 * 10 (1 second = 1000 millis).
unsigned long interval = 600000;

// Time (in millis) when the data should be retrieved.
unsigned long updateTime = 0;



//  Setup function --------------------------------------------

void setup() {
	// Enable Serial output and ask WiServer to generate log messages.
	Serial.begin(57600);
	// WiServer.enableVerboseMode(true);

	LCD.begin(9600); // All SerLCDs come at 9600 baud by default.
	backlight(150); // Change the backlight.

	clearScreen();
	selectLineOne();
	LCD.print("Initializing");
	selectLineTwo();
	LCD.print("Wi-Fi...");

	// Initialize WiServer.
	// We'll pass NULL for the page serving function since we don't need
	// to serve web pages. 
	WiServer.init(NULL);
  
	// Have the processInfo function called when data is returned by the server.
	getInfo.setReturnFunc(processInfo);
}



//  Loop function --------------------------------------------

void loop() {
	// Check if it's time to get an update
	if (millis() >= updateTime) {
		clearScreen();
		selectLineOne();
		LCD.print("Getting data...");
		selectLineTwo();
		LCD.print("");

		// Get the info.
		getInfo.submit();    

		updateTime += interval;
	}
  
	// Run WiServer
	WiServer.server_task();
}
