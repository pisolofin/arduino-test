// SPIFFS

#include <FS.h> // Include the SPIFFS library
#include <string>

File myfile; // create a SPIFFS Handling Variable

boolean initalizeFileSystem() {
	bool isInitialized = SPIFFS.begin();
	// If FileSystem is not ok, I try to format it
	if (!isInitialized) {
		Serial.println("FileSystem initialized");
		isInitialized = SPIFFS.format();
		isInitialized = isInitialized && SPIFFS.begin();
	}

	if (isInitialized) {
		Serial.println("SPIFFS: OK");
	}else {
		Serial.println("SPIFFS: Error");
	}

	return isInitialized;
}

void setup() {
	Serial.begin(9600);
	Serial.println("Setup");

	initalizeFileSystem();
}

void loop() {
	fs::Dir directory = SPIFFS.openDir("/");
	size_t	fileTotalSize	= 0;
	int		fileCounter		= 0;

	while (directory.next()) {
//		Serial.println("fileName   : " + directory.fileName());
//		Serial.print  ("isDirectory: ");
//		Serial.println(directory.isDirectory() ? "YES" : "NO");
//		Serial.print  ("isFile     : ");
//		Serial.println(directory.isFile() ? "YES" : "NO");

		Serial.print(".");

		if (directory.isFile() && directory.fileSize()) {
			File file = directory.openFile("r");
//			Serial.println(file.size());
			fileTotalSize += file.size();
			fileCounter++;
		}
	}

	Serial.println();

	Serial.print  ("fileTotalSize: ");
	Serial.println(fileTotalSize);

	Serial.print  ("fileCounter: ");
	Serial.println(fileCounter);

	Serial.println("--------------------------------------------------------------------------------");

	delay(3000);
}
