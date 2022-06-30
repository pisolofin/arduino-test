#include <ESP8266WiFi.h>
#include <string>

// WiFi
std::string		_wifiSSID		= "";
std::string		_wifiPassword	= "";

void setup() {
	Serial.begin(9600);

	Serial.println("setup");
	Serial.println("WiFi.mode");
	// Set WiFi to station mode
	WiFi.mode(WIFI_STA);

	Serial.println("WiFi.begin");
	// Connect to WiFi
	WiFi.begin(_wifiSSID.c_str(), _wifiPassword.c_str());

	long timeoutSeconds = 30;
	long timeoutLoop = timeoutSeconds * 2;
	Serial.println("Waiting connection");
	// Waiting connection
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);

		timeoutLoop--;
		// Check time loop for timeout
		if (timeoutSeconds && !timeoutLoop) {
			break;
		}
	}
	bool wifiIsConnected = WiFi.status() == WL_CONNECTED;

	Serial.print("WiFi connection status: ");
	Serial.println(wifiIsConnected ? "true" : "false");
	// If connected show IP
	if (wifiIsConnected) {
		Serial.println(WiFi.localIP().toString().c_str());
	}
}

bool _configureTime = true;
// Time configuration
int _timezone_sec		= 1 * 3600;
int _daylightOffset_sec	= 0 * 3600;

void loop() {
	Serial.println("loop");
	if (!WiFi.isConnected()) {
		Serial.println("No connection");
		// Re check time
		_configureTime = true;
	}

	// Update system time
	if (_configureTime) {
		Serial.print("System time: ");
		dateTimeToSerial();

		Serial.println("Update system time");
		// Set time via NTP, as required for x.509 validation
		configTime(/* timezone_sec */_timezone_sec, /* daylightOffset_sec */_daylightOffset_sec, /* server1 */"pool.ntp.org", /* server2 */"time.nist.gov" /* server3 */);
		Serial.println("System time updated");

		_configureTime = false;
	}else {
		Serial.print("System time: ");
		dateTimeToSerial();
	}

	// Wait time before next update
	delay(3000);
}

/** Print date time to log */
void dateTimeToSerial() {
	time_t		rawtime;
	struct tm	*timeinfo;
	// Get current time
	time(&rawtime);
	timeinfo = localtime(&rawtime);

	char buffer[21];
	// Create buffer with data
	strftime(buffer, 21, "%F %T", timeinfo);

	Serial.println(buffer);
}
