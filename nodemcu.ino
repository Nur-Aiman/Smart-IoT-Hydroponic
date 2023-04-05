#define BLYNK_PRINT Serial
#include <SoftwareSerial.h>
#include <BlynkSimpleEsp8266.h>
#include <ESP8266WiFi.h>
BlynkTimer timer;

char auth[] = "";
char ssid[] = "";
char pass[] = "";
float v, tem, hum, distance, ph, tds, ec;
float b, c, d, e, f, g;

SoftwareSerial NodeMCU(D2, D1);

void a()
{

    if (NodeMCU.available() > 0)
    {
        v = NodeMCU.parseFloat();
        if (NodeMCU.read() == '\n')
        {
            Serial.println(v);
            ///////////////////dht11-begin//////////////////
            if (v >= 100 && v <= 200)
            {
                float hum = v;
                hum = hum - 100;
                Serial.print(" Humidity : ");
                Serial.println(hum);
                Blynk.virtualWrite(V1, hum);
            }

            if (v >= 200 && v <= 300)
            {
                float tem = v;
                tem = tem - 200;
                Serial.print(" Temperature : ");
                Serial.println(tem);
                Blynk.virtualWrite(V2, tem);
            }
            ///////////////////dht11-end//////////////////

            //////////ultrasonic-sensor-begin/////////////
            if (v >= 300 && v <= 500)
            {
                float distance = v;
                distance = distance - 300;
                Serial.print(" Temperature : ");
                Serial.println(distance);
                Blynk.virtualWrite(V3, distance);
            }
            //////////ultrasonic-sensor-end///////////////

            //////////ph-sensor-begin/////////////////////
            if (v >= 500 && v <= 600)
            {
                float ph = v;
                ph = ph - 500;
                Serial.print(" pH : ");
                Serial.println(ph);
                Blynk.virtualWrite(V4, ph);
            }
            //////////ph-sensor-end///////////////////////

            //////////ec-sensor-begin/////////////////////
            if (v >= 1000 && v < 2000)
            {
                tds = v;
                tds = tds - 1000;
                Serial.print("TDS : ");
                Serial.println(tds);
                Blynk.virtualWrite(V7, tds);
            }

            if (v >= 2000 && v <= 3000)
            {
                ec = v;
                ec = ec - 2000;
                Serial.print(" EC : ");
                Serial.println(ec);
                Blynk.virtualWrite(V8, ec);
            }

            //////////ec-sensor-end///////////////////////
        }
    }
}
void setup()
{
    Serial.begin(115200);
    NodeMCU.begin(9600);
    Blynk.begin(auth, ssid, pass);
    pinMode(D2, INPUT);
    pinMode(D1, OUTPUT);
}

////////////////led-begin////////////////////
BLYNK_WRITE(V5)
{
    b = param.asFloat();
    b = b + 600;
    NodeMCU.print(b);
    NodeMCU.println("\n");
    Serial.println(b);
}
////////////////led-end///////////////////////

////////////////mainpump-begin////////////////
BLYNK_WRITE(V6)
{
    c = param.asFloat();
    c = c + 4000;
    NodeMCU.print(c);
    NodeMCU.println("\n");
    Serial.println(c);
}
////////////////mainpump-end////////////////////

////////////////phValuemax-begin////////////////
BLYNK_WRITE(V9)
{
    d = param.asFloat();
    d = d + 5000;
    NodeMCU.print(d);
    NodeMCU.println("\n");
    Serial.println(d);
}
////////////////phValuemax-end////////////////////

////////////////phValuemin-begin////////////////
BLYNK_WRITE(V10)
{
    e = param.asFloat();
    e = e + 6000;
    NodeMCU.print(e);
    NodeMCU.println("\n");
    Serial.println(e);
}
////////////////phValuemin-end////////////////////

////////////////tdsValuemax-begin////////////////
BLYNK_WRITE(V11)
{
    f = param.asFloat();
    f = f + 7000;
    NodeMCU.print(f);
    NodeMCU.println("\n");
    Serial.println(f);
}
////////////////tdsValuemax-end////////////////////

////////////////tdsValuemin-begin////////////////
BLYNK_WRITE(V12)
{
    g = param.asFloat();
    g = g + 8000;
    NodeMCU.print(g);
    NodeMCU.println("\n");
    Serial.println(g);
}
////////////////tdsValuemin-end////////////////////

void loop()
{
    Blynk.run();
    timer.run();
    timer.setInterval(400L, a);
}
