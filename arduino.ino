#include <dht.h> //dhtlibrary
#include <SoftwareSerial.h>
SoftwareSerial ArduinoUno(3, 2);

///////////dht-pin-and-variable-initialization////////////////
#define dht_apin A0 // Analog Pin sensor is connected to
int h, t;
dht DHT;
//////////////////////////////////////////////////////////////

///////////////ultrasonic-sensor-pin-initialization///////////
int trigger1 = 5;
int echo1 = 4;
long duration1 = 0;
int cm1 = 0;
//////////////////////////////////////////////////////////////

///////////////////ph-sensor-initialization//////////////////
#define SensorPin A1        // the pH meter Analog output is connected with the Arduino’s Analog
unsigned long int avgValue; // Store the average value of the sensor feedback
float b;
int buf[10], temp;
/////////////////////////////////////////////////////////////

/////////////////////////led-initialization//////////////////
int led = 8;
/////////////////////////////////////////////////////////////

/////////////////////////mainpump-initialization//////////////////
int mainpump = 7;
/////////////////////////////////////////////////////////////

/////////////////////////minipump-initialization//////////////////
int minipump1 = 9;
int minipump2 = 10;
int minipump3 = 11;
int minipump4 = 12;
/////////////////////////////////////////////////////////////

///////////////////ec-sensor-initialization//////////////////
#define TdsSensorPin A3
#define VREF 5.0          // analog reference voltage(Volt) of the ADC
#define SCOUNT 30         // sum of sample point
int analogBuffer[SCOUNT]; // store the analog value in the array, read from ADC
int analogBufferTemp[SCOUNT];
int analogBufferIndex = 0, copyIndex = 0;
float averageVoltage = 0, tdsValue = 0, temperature = 25, ecValue = 0;
/////////////////////////////////////////////////////////////

/////////////////user-input-initialization///////////////////
float phValuemax, phValuemin, tdsValuemax, tdsValuemin;
/////////////////////////////////////////////////////////////

void setup()
{

    Serial.begin(9600);
    // delay(500);//Delay to let system boot
    ArduinoUno.begin(9600);
    Serial.println("DHT11 Humidity & temperature Sensor\n\n");
    // delay(1000);//Wait before accessing Sensor

    ///////////////ultrasonic-sensor-pin-setup///////////////////
    pinMode(trigger1, OUTPUT);
    pinMode(echo1, INPUT);
    //////////////////////////////////////////////////////////////

    ///////////////ph-sensor-pin-setup////////////////////////////
    pinMode(13, OUTPUT);
    /////////////////////////////////////////////////////////////

    ///////////////led-pin-setup/////////////////////////////////
    pinMode(8, OUTPUT);
    /////////////////////////////////////////////////////////////

    ///////////////mainpump-pin-setup////////////////////////////
    pinMode(7, OUTPUT);
    /////////////////////////////////////////////////////////////

    ///////////////minipump-pin-setup////////////////////////////
    pinMode(9, OUTPUT);
    pinMode(10, OUTPUT);
    pinMode(11, OUTPUT);
    pinMode(12, OUTPUT);
    /////////////////////////////////////////////////////////////

    ///////////////ec-sensor-pin-setup////////////////////////////
    pinMode(TdsSensorPin, INPUT);
    /////////////////////////////////////////////////////////////

} // end "setup()"

void loop()
{
    // Start of Program

    //////////////////////////////////led-program-begin////////////////////////////
    delay(1000);
    while (ArduinoUno.available() > 0)
    {
        float val = ArduinoUno.parseFloat();
        if (ArduinoUno.read() == '\n')
        {

            if (val >= 600 && val < 700)
            {

                Serial.println(val);
                val = (int)val;
                if (val == 601)
                {
                    digitalWrite(led, HIGH);
                }
                else
                {
                    digitalWrite(led, LOW);
                }
            }

            //////////////////////////////////mainpump-program-begin///////////////////////////
            if (val >= 4000 && val < 5000)
            {

                Serial.println(val);
                val = (int)val;
                if (val == 4001)
                {
                    digitalWrite(mainpump, HIGH);
                }
                else
                {
                    digitalWrite(mainpump, LOW);
                }
            }
            //////////////////////////////////mainpump-program-end//////////////////////////////

            /////////////////////////////user-input-program-begin///////////////////////////////
            if (val >= 5000 && val < 6000)
            {
                phValuemax = val - 5000;
                Serial.print("pH max = ");
                Serial.println(phValuemax);
            }

            if (val >= 6000 && val < 7000)
            {
                phValuemin = val - 6000;
                Serial.print("pH min = ");
                Serial.println(phValuemin);
            }

            if (val >= 7000 && val < 8000)
            {
                tdsValuemax = val - 7000;
                Serial.print("tds max = ");
                Serial.println(tdsValuemax);
            }

            if (val >= 8000 && val < 9000)
            {
                tdsValuemin = val - 8000;
                Serial.print("tds min = ");
                Serial.println(tdsValuemin);
            }
            /////////////////////////////user-input-program-end/////////////////////////////////
        }
    }
    delay(1000);
    //////////////////////////////////led-program-end//////////////////////////////

    ////////////////////////////////dht-program-begin////////////////////////////////////
    DHT.read11(dht_apin);

    Serial.print("Current humidity = ");
    h = DHT.humidity;
    h = h + 100;
    ArduinoUno.print(h);
    ArduinoUno.println("\n");
    Serial.print(h);
    Serial.print("%  ");

    Serial.print("temperature = ");
    t = DHT.temperature;
    t = t + 200;
    ArduinoUno.print(t);
    ArduinoUno.println("\n");
    Serial.print(t);
    Serial.println("C  ");
    // delay(1000);//Wait 5 seconds before accessing sensor again.
    ////////////////////////////////dht-program-end////////////////////////////////////

    ////////////////////////////////ultrasonic-sensor-program-begin////////////////////
    digitalWrite(trigger1, LOW);
    digitalWrite(trigger1, HIGH);
    digitalWrite(trigger1, LOW);

    duration1 = pulseIn(echo1, HIGH);
    cm1 = duration1 * 0.034 / 2;
    cm1 = cm1 + 300;

    ArduinoUno.print(cm1);
    ArduinoUno.println("\n");
    Serial.print("Cm: ");
    Serial.println(cm1);

    // delay(1000);
    ////////////////////////////////ultrasonic-sensor-program-end////////////////////

    ////////////////////////////////ph-sensor-program-begin//////////////////////////

    for (int i = 0; i < 10; i++) // Get 10 sample value from the sensor for smooth the value
    {
        buf[i] = analogRead(SensorPin);
        delay(10);
    }
    for (int i = 0; i < 9; i++) // sort the analog from small to large
    {
        for (int j = i + 1; j < 10; j++)
        {
            if (buf[i] > buf[j])
            {
                temp = buf[i];
                buf[i] = buf[j];
                buf[j] = temp;
            }
        }
    }
    avgValue = 0;
    for (int i = 2; i < 8; i++) // take the average value of 6 center sample
        avgValue += buf[i];
    float phValue = (float)avgValue * 5.0 / 1024 / 6; // convert the analog into millivolt
    phValue = 3.5 * phValue;                          // convert the millivolt into pH value
    phValue = phValue + 500;
    Serial.print("    pH:");
    Serial.print(phValue, 2);
    ArduinoUno.print(phValue);
    ArduinoUno.println("\n");
    Serial.println(" ");
    digitalWrite(13, HIGH);
    delay(300);
    digitalWrite(13, LOW);

    ////////////////////////////////ph-sensor-program-end//////////////////////////

    ////////////////////////////////ec-sensor-program-begin//////////////////////////
    temperature = t - 200;
    static unsigned long analogSampleTimepoint = millis();
    if (millis() - analogSampleTimepoint > 10U) // every 40 milliseconds,read the analog value from the ADC
    {
        analogSampleTimepoint = millis();
        analogBuffer[analogBufferIndex] = analogRead(TdsSensorPin); // read the analog value and store into the buffer
        analogBufferIndex++;
        if (analogBufferIndex == SCOUNT)
            analogBufferIndex = 0;
    }
    static unsigned long printTimepoint = millis();
    if (millis() - printTimepoint > 10U)
    {
        printTimepoint = millis();
        for (copyIndex = 0; copyIndex < SCOUNT; copyIndex++)
            analogBufferTemp[copyIndex] = analogBuffer[copyIndex];
        averageVoltage = getMedianNum(analogBufferTemp, SCOUNT) * (float)VREF / 1024.0;                                                                                                  // read the analog value more stable by the median filtering algorithm, and convert to voltage value
        float compensationCoefficient = 1.0 + 0.02 * (temperature - 25.0);                                                                                                               // temperature compensation formula: fFinalResult(25^C) = fFinalResult(current)/(1.0+0.02*(fTP-25.0));
        float compensationVolatge = averageVoltage / compensationCoefficient;                                                                                                            // temperature compensation
        tdsValue = (133.42 * compensationVolatge * compensationVolatge * compensationVolatge - 255.86 * compensationVolatge * compensationVolatge + 857.39 * compensationVolatge) * 0.5; // convert voltage value to tds value
        // Serial.print("voltage:");
        // Serial.print(averageVoltage,2);
        // Serial.print("V   ");
        Serial.print("TDS Value:");
        Serial.print(tdsValue, 2);
        Serial.println("ppm");

        ecValue = tdsValue * 1 / 700;
        Serial.print("EC Value:");
        Serial.print(ecValue, 2);

        tdsValue = tdsValue + 1000;
        ArduinoUno.print(tdsValue);
        ArduinoUno.println("\n");
        ecValue = ecValue + 2000;
        ArduinoUno.print(ecValue);
        ArduinoUno.println("\n");
    }

    ////////////////////////////////minipump-automation-program-begin//////////////
    phValue = phValue - 500;
    if (phValue < phValuemin)
    {
        digitalWrite(minipump1, LOW);
        digitalWrite(minipump2, HIGH);
        delay(5000);
        digitalWrite(minipump1, LOW);
        digitalWrite(minipump2, LOW);
    }
    else if (phValue > phValuemax)
    {
        digitalWrite(minipump1, HIGH);
        digitalWrite(minipump2, LOW);
        delay(5000);
        digitalWrite(minipump1, LOW);
        digitalWrite(minipump2, LOW);
    }

    tdsValue = tdsValue - 1000;
    if (tdsValue < tdsValuemin)
    {
        digitalWrite(minipump3, LOW);
        digitalWrite(minipump4, HIGH);
        delay(5000);
        digitalWrite(minipump3, LOW);
        digitalWrite(minipump4, LOW);
    }
    else if (tdsValue > tdsValuemax)
    {
        digitalWrite(minipump3, HIGH);
        digitalWrite(minipump4, LOW);
        delay(5000);
        digitalWrite(minipump3, LOW);
        digitalWrite(minipump4, LOW);
    }

    ////////////////////////////////minipump-automation-program-end////////////////
}

int getMedianNum(int bArray[], int iFilterLen)
{
    int bTab[iFilterLen];
    for (byte m = 0; m < iFilterLen; m++)
        bTab[m] = bArray[m];
    int m, n, bTemp;
    for (n = 0; n < iFilterLen - 1; n++)
    {
        for (m = 0; m < iFilterLen - n - 1; m++)
        {
            if (bTab[m] > bTab[m + 1])
            {
                bTemp = bTab[m];
                bTab[m] = bTab[m + 1];
                bTab[m + 1] = bTemp;
            }
        }
    }
    if ((iFilterLen & 1) > 0)
        bTemp = bTab[(iFilterLen - 1) / 2];
    else
        bTemp = (bTab[iFilterLen / 2] + bTab[iFilterLen / 2 - 1]) / 2;
    return bTemp;
}

////////////////////////////////ec-sensor-program-end//////////////////////////
