void setup() {
	Serial.begin(9600);
}

void loop() {
	Serial.print("System time: ");
	dateTimeToSerial();

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
