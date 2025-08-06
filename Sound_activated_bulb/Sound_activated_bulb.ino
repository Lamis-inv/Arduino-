
#define Sensor 8
#define RELAY_PIN 7

int clap = 0;
long detection_range_start = 0;
long detection_range = 0;
boolean status_lights = false;
void setup() {
  pinMode(Sensor, INPUT);
  pinMode(13,OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);
}
void loop() {
int status_sensor = digitalRead(Sensor);
if (status_sensor == 0)
{
if (clap == 0)
{
detection_range_start = detection_range = millis();
clap++;
}
else if (clap > 0 && millis()-detection_range >= 50)
{
detection_range = millis();
clap++;
}
}
if (millis()-detection_range_start >= 400)
{
if (clap == 2)
{
if (!status_lights)
{
status_lights = true;
digitalWrite(13, HIGH);
digitalWrite(RELAY_PIN, LOW);
}
else if (status_lights)
{
status_lights = false;
digitalWrite(13, LOW);
digitalWrite(RELAY_PIN, HIGH);
}
}
clap = 0;
}
}
