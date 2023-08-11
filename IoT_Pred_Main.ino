#define APP_DEBUG
#define USE_NODE_MCU_BOARD
#include "DHT.h"  //--> Include the DHT Sensor Library
#define FREQUENCY_HZ        50
#define INTERVAL_MS         (1000 / (FREQUENCY_HZ + 1))
//OG
//TEMP
const int DHTPin = 16;//TEMPERATURE
#define DHTTYPE DHT11
DHT dht(DHTPin, DHTTYPE);
//HALL
  int hall_pin = 2;
//VIB
int led = 15; // GREEN FOR VIB
const int vs =4; // vibration sensor
//SOUND
 const int OUT_PIN = 5;
 const int SAMPLE_TIME = 10;
 unsigned long millisCurrent;
 unsigned long millisLast = 0;
 unsigned long millisElapsed = 0;
  int sampleBufferValue = 0;
//BUZZER
const int buzzer = 12; //buzzer
//NEW TEMP
#define sensorPin 0

//SEND TO CLOUD 
#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif

#include <Firebase_ESP_Client.h>

// Provide the token generation process info.
#include <addons/TokenHelper.h>

/* 1. Define the WiFi credentials */
#define WIFI_SSID "Vivo 1801"
#define WIFI_PASSWORD "$IOeRs3@z^aSw4Il?"

/* 2. Define the API Key */
#define API_KEY "AIzaSyC1bw0Xh9IacEMhxlPRLhryhLeCY4rAy8s"

/* 3. Define the project ID */
#define FIREBASE_PROJECT_ID "clouddb-177f5"

/* 4. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "bibatralph@gmail.com"
#define USER_PASSWORD "Gadesa2000"

// Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long dataMillis = 0;
int count = 0;
int randomNumber1 = 0;
int randomNumber2 = 0;

void fcsUploadCallback(CFS_UploadStatusInfo info)
{
    if (info.status == fb_esp_cfs_upload_status_init)
    {
        Serial.printf("\nUploading data (%d)...\n", info.size);
    }
    else if (info.status == fb_esp_cfs_upload_status_upload)
    {
        Serial.printf("Uploaded %d%s\n", (int)info.progress, "%");
    }
    else if (info.status == fb_esp_cfs_upload_status_complete)
    {
        Serial.println("Upload completed ");
    }
    else if (info.status == fb_esp_cfs_upload_status_process_response)
    {
        Serial.print("Processing the response... ");
    }
    else if (info.status == fb_esp_cfs_upload_status_error)
    {
        Serial.printf("Upload failed, %s\n", info.errorMsg.c_str());
    }
}
void setup()
{
    //OG
      pinMode(vs, INPUT);  // vibration sensor
  pinMode(hall_pin, INPUT); //hall Sensor
                          // SOUND Sensor
  pinMode(led, OUTPUT);// SAMPLE RESULT
  pinMode(buzzer, OUTPUT); // BUZZER
  pinMode(DHTPin, INPUT); 

    //OG END
    Serial.begin(115200);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(300);
    }
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();

    Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

    /* Assign the api key (required) */
    config.api_key = API_KEY;

    /* Assign the user sign in credentials */
    auth.user.email = USER_EMAIL;
    auth.user.password = USER_PASSWORD;

    /* Assign the callback function for the long running token generation task */
    config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h

#if defined(ESP8266)
    // In ESP8266 required for BearSSL rx/tx buffer for large data handle, increase Rx size as needed.
    fbdo.setBSSLBufferSize(2048 /* Rx buffer size in bytes from 512 - 16384 */, 2048 /* Tx buffer size in bytes from 512 - 16384 */);
#endif
    
    // Limit the size of response payload to be collected in FirebaseData
    fbdo.setResponseSize(2048);

    Firebase.begin(&config, &auth);

    Firebase.reconnectWiFi(true);

    // For sending payload callback
    // config.cfs.upload_callback = fcsUploadCallback;
}

void loop()
{
//OG

//TEMP CODE 
  int humidity = dht.readHumidity();  //--> Read humidity
  int temperature = dht.readTemperature();  //--> Read temperature as Celsius (the default). If you want to read the temperature sensor in Fahrenheit, use this -> float t = dht.readTemperature (true);
   //Serial.print(humidity);
   //Serial.print(temperature);
// MACHINE TEMP CODE


//Serial.println(humidity); 
//Serial.println(temperature); 

//SOund CODE
   
//VIB CODE

// HALL CODE
 //NOT USED
  int sensorValue2 = analogRead(2);  
  // print out the value you read:
  //Serial.println("sensorValue: ");
  //Serial.println(sensorValue2);

  
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  float voltage2 = sensorValue2 * (5.0 / 1023.0);
  //Serial.println("voltage: ");
  //Serial.println(voltage2);  


// END

//OG END

//SEND TO CLOUD START
randomNumber1=random(100,500);
randomNumber2=random(100,500);

    // Firebase.ready() should be called repeatedly to handle authentication tasks.
    //DHT 11
    if (Firebase.ready() && (millis() - dataMillis > 1000 || dataMillis == 0))
    {
    
        dataMillis = millis();
        FirebaseJson content;
        std::vector<struct fb_esp_firestore_document_write_t> writes;
        struct fb_esp_firestore_document_write_t transform_write;
        transform_write.type = fb_esp_firestore_document_write_type_transform;   
        transform_write.document_transform.transform_document_path = "Dht11/current";
        struct fb_esp_firestore_document_write_field_transforms_t field_transforms;
        field_transforms.fieldPath = "humidity";
        field_transforms.transform_type = fb_esp_firestore_transform_type_set_to_server_value;
        field_transforms.transform_content = "REQUEST_TIME";  
        transform_write.document_transform.field_transforms.push_back(field_transforms);  
        writes.push_back(transform_write);

        struct fb_esp_firestore_document_write_t update_write;
        update_write.type = fb_esp_firestore_document_write_type_update;
        String documentPath = "Dht11/current";
        content.set("fields/humidity/doubleValue", randomNumber1);
        content.set("fields/temperature/doubleValue", randomNumber2);
        update_write.update_document_content = content.raw();
        update_write.update_document_path = documentPath.c_str();
        writes.push_back(update_write);
        if (Firebase.Firestore.commitDocument(&fbdo, FIREBASE_PROJECT_ID, "" /* databaseId can be (default) or empty */, writes /* dynamic array of fb_esp_firestore_document_write_t */, "" /* transaction */))
           
            Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());
        
        else
        
            Serial.println(fbdo.errorReason());
    }
    //VIB
     if (Firebase.ready() && (millis() - dataMillis > 1000 || dataMillis == 0))
    {
 long measurement = vibration();
 Serial.println(measurement);
  if (measurement > threshold){
    digitalWrite(led, HIGH);
  tone(buzzer, 1000); // Send 1KHz sound signal...
  delay(1000);        // ...for 1 sec
  noTone(buzzer);     // Stop sound...
  delay(1000);        // ...for 1sec

  }
  else{
    digitalWrite(led, LOW); 
   
  }
        dataMillis = millis();
        FirebaseJson content;
        std::vector<struct fb_esp_firestore_document_write_t> writes;
        struct fb_esp_firestore_document_write_t transform_write;
        transform_write.type = fb_esp_firestore_document_write_type_transform;   
        transform_write.document_transform.transform_document_path = "SW420/current";
        struct fb_esp_firestore_document_write_field_transforms_t field_transforms;
        field_transforms.fieldPath = "Vibration";
        field_transforms.transform_type = fb_esp_firestore_transform_type_set_to_server_value;
        field_transforms.transform_content = "REQUEST_TIME";  
        transform_write.document_transform.field_transforms.push_back(field_transforms);  
        writes.push_back(transform_write);

        struct fb_esp_firestore_document_write_t update_write;
        update_write.type = fb_esp_firestore_document_write_type_update;
        String documentPath = "SW420/current";
        content.set("fields/Vibration/doubleValue", measurement);
       
        update_write.update_document_content = content.raw();
        update_write.update_document_path = documentPath.c_str();
        writes.push_back(update_write);
        if (Firebase.Firestore.commitDocument(&fbdo, FIREBASE_PROJECT_ID, "" /* databaseId can be (default) or empty */, writes /* dynamic array of fb_esp_firestore_document_write_t */, "" /* transaction */))
           
            Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());
        
        else
        
            Serial.println(fbdo.errorReason());
    }
    // INTERNAL TEMP
      if (Firebase.ready() && (millis() - dataMillis > 1000 || dataMillis == 0))
    {
    
      // Get the voltage reading from the LM35
  int reading = analogRead(sensorPin);
  float voltage = reading * (5.0 / 1024.0);
  // Convert the voltage into the temperature in Celsius
  float temperatureC = voltage * 100;


        dataMillis = millis();
        FirebaseJson content;
        std::vector<struct fb_esp_firestore_document_write_t> writes;
        struct fb_esp_firestore_document_write_t transform_write;
        transform_write.type = fb_esp_firestore_document_write_type_transform;   
        transform_write.document_transform.transform_document_path = "LM35/current";
        struct fb_esp_firestore_document_write_field_transforms_t field_transforms;
        field_transforms.fieldPath = "ITemp";
        field_transforms.transform_type = fb_esp_firestore_transform_type_set_to_server_value;
        field_transforms.transform_content = "REQUEST_TIME";  
        transform_write.document_transform.field_transforms.push_back(field_transforms);  
        writes.push_back(transform_write);

        struct fb_esp_firestore_document_write_t update_write;
        update_write.type = fb_esp_firestore_document_write_type_update;
        String documentPath = "LM35/current";
        content.set("fields/ITemp/doubleValue", temperatureC);
       
        update_write.update_document_content = content.raw();
        update_write.update_document_path = documentPath.c_str();
        writes.push_back(update_write);
        if (Firebase.Firestore.commitDocument(&fbdo, FIREBASE_PROJECT_ID, "" /* databaseId can be (default) or empty */, writes /* dynamic array of fb_esp_firestore_document_write_t */, "" /* transaction */))
           
            Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());
        
        else
        
            Serial.println(fbdo.errorReason());
    
    }}
   
long vibration(){
  long measurement=pulseIn (vs, HIGH); 
  return measurement;
}
