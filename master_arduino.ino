 #include <Wire.h>
 #include <DHT.h>
 int pirPin = 2;  // Pin for PIR motion sensor
 int irrigationMotorIn1 = 8;  // Motor driver input 1 for irrigation motor
 int irrigationMotorIn2 = 9;  // Motor driver input 2 for irrigation motor
 int irrigationMotorEn = 10;  // Motor driver enable pin for irrigation motor
 int pestControlMotorIn1 = 11;  // Motor driver input 1 for pest control motor
 int pestControlMotorIn2 = 12;  // Motor driver input 2 for pest control motor
 int pestControlMotorEn = 13;  // Motor driver enable pin for pest control motor
 int soilMoisturePin = A0;  // Pin for soil moisture sensor
 int moistureThreshold = 600;  // Threshold for soil moisture level
 const int slaveAddress = 8;  // I2C address of the slave
 #define DHTPIN 3          
// Pin connected to the DHT sensor
 #define DHTTYPE DHT11     // Type of DHT sensor (DHT11)
 DHT dht(DHTPIN, DHTTYPE);
 void setup() {
 pinMode(pirPin, INPUT);  // Set PIR pin as input
 pinMode(irrigationMotorIn1, OUTPUT);  // Set irrigation motor driver input 1 as output
 pinMode(irrigationMotorIn2, OUTPUT);  // Set irrigation motor driver input 2 as output
 pinMode(irrigationMotorEn, OUTPUT);  // Set irrigation motor driver enable as output
 pinMode(pestControlMotorIn1, OUTPUT);  // Set pest control motor driver input 1 as output
 pinMode(pestControlMotorIn2, OUTPUT);  // Set pest control motor driver input 2 as output
 pinMode(pestControlMotorEn, OUTPUT);  // Set pest control motor driver enable as output
Serial.begin(9600);  // Initialize serial communication for debugging
 Wire.begin();        
// Join the I2C bus as master
 dht.begin();         
}
 void loop() {
 // Initialize DHT sensor
 int motionDetected = digitalRead(pirPin);  // Read motion detection from PIR sensor
 int soilMoistureValue = analogRead(soilMoisturePin);  // Read soil moisture level
 float temperature = dht.readTemperature(); // Read temperature from DHT sensor
 float humidity = dht.readHumidity();       
// Read humidity from DHT sensor
 Serial.print("Soil Moisture: ");
 Serial.println(soilMoistureValue);
 Serial.print("Temperature: ");
 Serial.println(temperature);
 Serial.print("Humidity: ");
 Serial.println(humidity);
 // Adjust moisture threshold based on humidity
 int adjustedMoistureThreshold = moistureThreshold;
 if (humidity > 80) {
 adjustedMoistureThreshold -= 50;  // Reduce threshold to avoid overwatering in high humidity
 }
// Control irrigation motor based on soil moisture
  if (soilMoistureValue < adjustedMoistureThreshold) {
    Serial.println("Soil Moisture Low. Irrigating...");
    startIrrigationMotor();  // Start irrigation motor
    sendI2CSignal(1);  // Signal to slave Arduino: Irrigation ON
  } else {
    Serial.println("Soil Moisture Sufficient. Irrigation OFF");
    stopIrrigationMotor();  // Stop irrigation motor
    sendI2CSignal(0);  // Signal to slave Arduino: Irrigation OFF
  }
  // Control pest control motor based on motion detection
  if (motionDetected == HIGH) {
    Serial.println("Motion Detected! Activating Pest Control...");
    startPestControlMotor();  // Start pest control motor
    sendI2CSignal(2);  // Signal to slave Arduino: Pest Control ON
  } else {
    Serial.println("No Motion Detected. Pest Control OFF");
    stopPestControlMotor();  // Stop pest control motor
    sendI2CSignal(0);  // Signal to slave Arduino: Pest Control OFF
  }
  delay(500);  // Wait before next loop iteration
 }
 void startIrrigationMotor() {
  digitalWrite(irrigationMotorIn1, HIGH);  // Set irrigation motor direction
  digitalWrite(irrigationMotorIn2, LOW);
  analogWrite(irrigationMotorEn, 255);  // Set irrigation motor speed to full
 }
void stopIrrigationMotor() {
 digitalWrite(irrigationMotorIn1, LOW);  // Stop irrigation motor
 digitalWrite(irrigationMotorIn2, LOW);
 analogWrite(irrigationMotorEn, 0);  // Set motor speed to zero
 }
 void startPestControlMotor() {
 digitalWrite(pestControlMotorIn1, HIGH);  // Set pest control motor direction
 digitalWrite(pestControlMotorIn2, LOW);
 analogWrite(pestControlMotorEn, 255);  // Set pest control motor speed to full
 }
 void stopPestControlMotor() {
 digitalWrite(pestControlMotorIn1, LOW);  // Stop pest control motor
 digitalWrite(pestControlMotorIn2, LOW);
 analogWrite(pestControlMotorEn, 0);  // Set motor speed to zero
 }
 void sendI2CSignal(int signal) {
 Wire.beginTransmission(slaveAddress);
 Wire.write(signal);  // Send signal to slave Arduino (1 = Irrigation ON, 2 = Pest Control ON, 0 = OFF)
 Wire.endTransmission();
 }
