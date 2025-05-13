#include <Arduino.h>  
#include <Arduino_FreeRTOS.h>  
#include <task.h>  
#include <queue.h>  

// Queue handle (global)  
QueueHandle_t xQueue;  

// Task 1: Blink LED (unchanged)  
void TaskBlink(void *pvParam) {  
  pinMode(13, OUTPUT);  
  while (1) {  
    digitalWrite(13, !digitalRead(13));  
    vTaskDelay(500 / portTICK_PERIOD_MS);  
  }  
}  

// Task 2: Print received data from queue  
void TaskPrint(void *pvParam) {
  int receivedValue = 0;
  while (1) {
    if (xQueueReceive(xQueue, &receivedValue, portMAX_DELAY) == pdTRUE) 
    { 
      Serial.print("Received: ");
      Serial.println(receivedValue);
    }
  }
} 

// NEW Task 3: Generate "sensor data"  
void TaskSensor(void *pvParam) {  
  int counter = 0;  
  while (1) {  
    xQueueSend(xQueue, &counter, 0);  // Send data to queue  
    counter++;  
    vTaskDelay(1000 / portTICK_PERIOD_MS);  
  }  
}  

void setup() {  
  Serial.begin(9600);  
  xQueue = xQueueCreate(5, sizeof(int));  // Queue for 5 integers  
  
  xTaskCreate(TaskBlink, "Blink", 128, NULL, 1, NULL);  
  xTaskCreate(TaskPrint, "Print", 128, NULL, 1, NULL);  
  xTaskCreate(TaskSensor, "Sensor", 128, NULL, 1, NULL);  // New task  
}  

void loop() {}  