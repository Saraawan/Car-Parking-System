#include <TaskScheduler.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <ArduinoQueue.h>
Scheduler scheduler;
LiquidCrystal_I2C lcd(0x27, 20, 4);
Servo myservo1;
#define ir_s1 2 // Entrance sensor
#define ir_s2 4 // Exit sensor
#define Total 4 // Limited parking slots
#define ir_car1 5 // Car sensor for slot 1
#define ir_car2 6 // Car sensor for slot 2
#define ir_car3 7 // Car sensor for slot 3
#define ir_car4 8 // Car sensor for slot 4
int Space;
int flag1 = 0;
int flag2 = 0;
// Sensor variables
int S1 = 0, S2 = 0, S3 = 0, S4 = 0;
// Declare Read_Sensor function
void Read_Sensor();
// Define callback functions for tasks
void task1();
void task2();
void task3();
// Define Task objects
Task t1(1000, TASK_FOREVER, &task1);
Task t2(2000, TASK_FOREVER, &task2);
Task t3(100, TASK_FOREVER, &task3);
// FIFO Queue for managing cars waiting to enter
ArduinoQueue<int> carQueue;
void setup() {
pinMode(ir_s1, INPUT);
pinMode(ir_s2, INPUT);
pinMode(ir_car1, INPUT);
pinMode(ir_car2, INPUT);
pinMode(ir_car3, INPUT);
pinMode(ir_car4, INPUT);
myservo1.attach(3);
myservo1.write(100);
lcd.init();
lcd.backlight();
lcd.setCursor(0, 1);
lcd.print(" Hi Welcome To ");
lcd.setCursor(0, 2);
lcd.print(" JustDoElectronics");
delay(5000);
lcd.clear();
lcd.setCursor(0, 0);
lcd.print(" Today's Project ");
lcd.setCursor(0, 1);
lcd.print(" Car Parking ");
lcd.setCursor(0, 2);
lcd.print(" System ");
delay(5000);
lcd.clear();
Space = Total;
Serial.begin(9600); // Initialize serial communication
scheduler.init();
scheduler.addTask(t1);
scheduler.addTask(t2);
scheduler.addTask(t3);
scheduler.enableAll(); // Start all tasks
}
void loop() {
scheduler.execute();
// Your existing loop logic for parking system
if (digitalRead(ir_s1) == LOW && flag1 == 0) {
// Existing logic for car entrance
// Enqueue the car ID or any identifier
if (Space > 0) {
flag1 = 1;
if (flag2 == 0) {
myservo1.write(0);
Space = Space - 1;
carQueue.enqueue(1); // Enqueue the car ID or any identifier
}
} else {
lcd.setCursor(0, 0);
lcd.print(" Parking Full ");
lcd.setCursor(0, 1);
lcd.print(" No Entry Allowed ");
delay(1000);
lcd.clear();
}
}
if (digitalRead(ir_s2) == LOW && flag2 == 0) {
// Existing logic for car exit
// Dequeue the car that is exiting
flag2 = 1;
if (flag1 == 0) {
if (!carQueue.isEmpty()) {
myservo1.write(0);
Space = Space + 1;
int carId = carQueue.dequeue(); // Dequeue the car ID or any identifier
Serial.print("Car exited with ID: ");
Serial.println(carId);
}
}
}
if (flag1 == 1 && flag2 == 1) {
// Existing logic when both flags are set
delay(1000);
myservo1.write(100);
flag1 = 0, flag2 = 0;
}
lcd.setCursor(0, 0);
lcd.print("Total Slots: ");
lcd.print(Total);
lcd.setCursor(0, 1);
lcd.print("Available: ");
lcd.print(Space);
}
void task1() {
// Car entrance logic
Serial.println("Car entered the parking lot");
lcd.clear();
lcd.setCursor(0, 0);
lcd.print("Car Entered: Slot ");
lcd.print(Total - Space);
lcd.setCursor(0, 1);
lcd.print("Available: ");
lcd.print(Space);
}
void task2() {
// Car exit logic
Serial.println("Car exited the parking lot");
lcd.clear();
lcd.setCursor(0, 0);
lcd.print("Car Exited: Slot ");
lcd.print(Total - Space + 1);
lcd.setCursor(0, 1);
lcd.print("Available: ");
lcd.print(Space);
}
void task3() {
// Read sensors and update display
Read_Sensor();
lcd.setCursor(0, 0);
lcd.print(" Available Slot: ");
lcd.print(Space);
lcd.print(" ");
lcd.setCursor(0, 1);
lcd.print("S1:");
lcd.print(S1 == 1 ? "Full " : "Empty");
lcd.setCursor(11, 1);
lcd.print("S2:");
lcd.print(S2 == 1 ? "Full " : "Empty");
lcd.setCursor(0, 2);
lcd.print("S3:");
lcd.print(S3 == 1 ? "Full " : "Empty");
lcd.setCursor(11, 2);
lcd.print("S4:");
lcd.print(S4 == 1 ? "Full " : "Empty");
}
// Function to read sensor values
void Read_Sensor() {
S1 = digitalRead(ir_car1);
S2 = digitalRead(ir_car2);
S3 = digitalRead(ir_car3);
S4 = digitalRead(ir_car4);
}
