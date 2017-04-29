#ifndef RELAYSHIELDMQTTCONTROLLER_CURRENTSENSOR_CONFIG_H_
#define RELAYSHIELDMQTTCONTROLLER_CURRENTSENSOR_CONFIG_H_


const unsigned long CURRENT_UPDATE_INTERVAL = 10UL * 1000UL;

unsigned long currentPreviousMillis = 0UL;

// sensor specific variables
const int MV_PER_AMP = 66; // sensor scale factor
const float SENSOR_OFFSET = 5.0 / 2.0;  // offset since measuring both +/- currents

// Measurement topics
const char CURRENT_MEASUREMENT_TOPIC[] PROGMEM = "relayshield/measurement/current";

PGM_P const CURRENT_MEASUREMENT_TOPICS[] PROGMEM = {
    CURRENT_MEASUREMENT_TOPIC,     // idx = 0
};

/* STATUS_TOPICS indices, must match table above */
typedef enum {
  CURRENT_MEASUREMENT_TOPIC_IDX = 0,
} current_measurement_topics;

const byte CURRENT_SENSOR_PIN = A3;

void current_sensor_init() {
  pinMode(CURRENT_SENSOR_PIN, INPUT);
}

float current() {
  float reading = analogRead(CURRENT_SENSOR_PIN) * (5.0 / 1023.0); // pin voltage reading
  DEBUG_LOG(2, F("Current sensor:"));
  DEBUG_LOG(2, reading);
  DEBUG_LOG(2, F("V"));
  float current = (reading - SENSOR_OFFSET) * 1000.0 / MV_PER_AMP;
  DEBUG_LOG(2, current)
  DEBUG_LOG(2, F("A"));
  return abs(current);
}

void publish_current() {
  topicBuffer[0] = '\0';
  strcpy_P(topicBuffer, (char*)pgm_read_word(&(CURRENT_MEASUREMENT_TOPICS[CURRENT_MEASUREMENT_TOPIC_IDX])));
  payloadBuffer[0] = '\0';
  dtostrf(current(),1,FLOAT_DECIMAL_PLACES, payloadBuffer);
  DEBUG_LOG(1, topicBuffer);
  DEBUG_LOG(1, payloadBuffer);
  mqttClient.publish(topicBuffer, payloadBuffer);
}


#endif   /* RELAYSHIELDMQTTCONTROLLER_CURRENTSENSOR_CONFIG_H_ */
