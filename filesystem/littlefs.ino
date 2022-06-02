// LittleFS

#include <LittleFS.h> // LittleFS is declared
#include <string>

File myfile; // create a SPIFFS Handling Variable

struct fileCheckedInfo {
	size_t	fileTotalSize;
	int		fileCounter;
	int		directoryCounter;

	fileCheckedInfo(size_t fileTotalSize, int fileCounter, int directoryCounter) {
		fileCheckedInfo::fileTotalSize 		= fileTotalSize;
		fileCheckedInfo::fileCounter 		= fileCounter;
		fileCheckedInfo::directoryCounter	= directoryCounter;
	}
};

boolean initalizeFileSystem() {
	bool isInitialized = LittleFS.begin();
	// If FileSystem is not ok, I try to format it
	if (!isInitialized) {
		Serial.println("FileSystem initialized");
		isInitialized = LittleFS.format();
		isInitialized = isInitialized && LittleFS.begin();
	}

	if (isInitialized) {
		Serial.println("LittleFS: OK");
	}else {
		Serial.println("LittleFS: Error");
	}

	return isInitialized;
}

void setup() {
	Serial.begin(9600);
	Serial.println("Setup");

	initalizeFileSystem();
}

fileCheckedInfo* iterateDirectory(std::string directoryPath) {
	fs::Dir	directory			= LittleFS.openDir(directoryPath.c_str());
	size_t	fileTotalSize		= 0;
	int		fileCounter			= 0;
	int		directoryCounter	= 0;

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
		}else if (directory.isDirectory()) {
			fileCheckedInfo* fileCheckedInfoinside = iterateDirectory(directoryPath + directory.fileName().c_str() + "/");
			fileTotalSize += fileCheckedInfoinside->fileTotalSize;
			fileCounter += fileCheckedInfoinside->fileCounter;
			directoryCounter += fileCheckedInfoinside->directoryCounter;
			directoryCounter++;

			free(fileCheckedInfoinside);
		}
	}

	return new fileCheckedInfo(
		/* fileTotalSize */fileTotalSize,
		/* fileCounter */fileCounter,
		/* directoryCounter */directoryCounter
	);
}

void loop() {
	fileCheckedInfo* fileCheckedInfo = iterateDirectory("/");
	Serial.println();

	Serial.print  ("fileTotalSize: ");
	Serial.println(fileCheckedInfo->fileTotalSize);

	Serial.print  ("fileCounter: ");
	Serial.println(fileCheckedInfo->fileCounter);

	Serial.print  ("directoryCounter: ");
	Serial.println(fileCheckedInfo->directoryCounter);

	Serial.print  ("totalCounter: ");
	Serial.println(fileCheckedInfo->fileCounter + fileCheckedInfo->directoryCounter);

	Serial.println("--------------------------------------------------------------------------------");

	free(fileCheckedInfo);

	delay(3000);
}
