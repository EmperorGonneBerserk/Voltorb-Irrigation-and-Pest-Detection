 #include <Wire.h>
 #include <Servo.h>
 Servo servoHorizontal;  // Horizontal servo
 int ldrLeft = A0;  // Left LDR
 int ldrRight = A1; // Right LDR
 const int slaveAddress = 8;  // I2C address of this slave
 int receivedData = 0;
 // Pin for ultrasonic buzzer
 int ultrasonicBuzzerPin = 7;  // Pin connected to the ultrasonic buzzer
 void setup() {
 servoHorizontal.attach(9); // Attach the servo to pin 9
 servoHorizontal.write(90); // Start at the center position
 Serial.begin(9600);  // Initialize serial communication for debugging
 Wire.begin(slaveAddress);  // Join I2C bus as slave with address 8
 Wire.onReceive(receiveEvent);  // Register event function to handle incoming data
 pinMode(ultrasonicBuzzerPin, OUTPUT);  // Set buzzer pin as output
 }
 void loop() {
 if (receivedData == 1) {
 // Solar Tracking Activated
 int leftValue = analogRead(ldrLeft);   // Read the left LDR
 int rightValue = analogRead(ldrRight); // Read the right LDR
 // Compare the LDR values and adjust the servo position
    if (leftValue > rightValue + 10) {
      servoHorizontal.write(servoHorizontal.read() - 1); // Move servo to the left
    } else if (rightValue > leftValue + 10) {
      servoHorizontal.write(servoHorizontal.read() + 1); // Move servo to the right
    }
    delay(100);  // Small delay to stabilize the movement
  } else if (receivedData == 0) {
    // Motor is OFF, no tracking
    Serial.println("Solar Tracking Paused");
  }
  if (receivedData == 2) {
    // Activate ultrasonic buzzer to repel pests
    Serial.println("Activating Pest Control - Ultrasonic Buzzer");
    tone(ultrasonicBuzzerPin, 40000); // Emit a high-frequency sound (e.g., 40 kHz)
  } else if (receivedData != 2) {
    // Deactivate ultrasonic buzzer
    Serial.println("Deactivating Pest Control - Ultrasonic Buzzer");
    noTone(ultrasonicBuzzerPin); // Stop emitting sound
  }
 }
 void receiveEvent(int howMany) {
  if (Wire.available() > 0) {
    receivedData = Wire.read();  // Read the data from the master
    Serial.print("Received: ");  // Debug line
    Serial.println(receivedData); // Debug line
  }
 }
