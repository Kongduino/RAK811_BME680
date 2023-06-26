#include "Zanshin_BME680.h"  // Include the BME680 Sensor library
/**************************************************************************************************
** Declare all program constants                                                                 **
**************************************************************************************************/
const uint32_t SERIAL_SPEED {
  115200
}; ///< Set the baud rate for Serial I/O

/**************************************************************************************************
** Declare global variables and instantiate classes                                              **
**************************************************************************************************/
BME680_Class BME680; ///< Create an instance of the BME680 class
///< Forward function declaration with default value for sea level
float altitude(const int32_t press, const float seaLevel = 1013.25);
float altitude(const int32_t press, const float seaLevel) {
  /*!
    @brief     This converts a pressure measurement into a height in meters
    @details   The corrected sea-level pressure can be passed into the function if it is known, otherwise the standard atmospheric pressure of 1013.25hPa is used (see
             https://en.wikipedia.org/wiki/Atmospheric_pressure) for details.
    @param[in] press    Pressure reading from BME680
    @param[in] seaLevel Sea-Level pressure in millibars
    @return    floating point altitude in meters.
  */
  static float Altitude;
  Altitude = 44330.0 * (1.0 - pow(((float)press / 100.0) / seaLevel, 0.1903)); // Convert into meters
  return (Altitude);
}

void setup() {
  /*!
    @brief    Arduino method called once at startup to initialize the system
    @details  This is an Arduino IDE method which is called first upon boot or restart. It is only
            called one time and then control goes to the main "loop()" method, from which
            control never returns
    @return   void
  */
  Serial.begin(115200);
  delay(1000);
  Serial.print(F("Starting I2CDemo example program for BME680\n"));
  Serial.println("------------------------------------------------------");
  Serial.print(F("- Initializing BME680 sensor\n"));
  while (!BME680.begin(I2C_STANDARD_MODE)) {
    // Start BME680 using I2C, use first device found
    Serial.print(F("-  Unable to find BME680. Trying again in 5 seconds.\n"));
    delay(5000);
  }
  Serial.print(F("- Setting 16x oversampling for all sensors\n"));
  BME680.setOversampling(TemperatureSensor, Oversample16); // Use enumerated type values
  BME680.setOversampling(HumiditySensor, Oversample16); // Use enumerated type values
  BME680.setOversampling(PressureSensor, Oversample16); // Use enumerated type values
  Serial.print(F("- Setting IIR filter to a value of 4 samples\n"));
  BME680.setIIRFilter(IIR4); // Use enumerated type values
  Serial.print(F("- Setting gas measurement to 320 Ohm for 150ms\n"));
  BME680.setGas(320, 150); // 320 Ohm for 150 milliseconds
}
void loop() {
  /*!
    @brief    Arduino method for the main program loop
    @details  This is the main program for the Arduino IDE, it is an infinite loop and keeps on
            repeating. The "sprintf()" function is to pretty-print the values, since floating
            point is not supported on the Arduino, split the values into those before and those
            after the decimal point.
    @return   void
  */
  static int32_t temp, humidity, pressure, gas; // BME readings
  static char buf[16]; // sprintf text buffer
  static float alt; // Temporary variable
  static uint16_t loopCounter = 0; // Display iterations
  BME680.getSensorData(temp, humidity, pressure, gas); // Get readings
  if (loopCounter++ != 0) {
    // Ignore first reading, might be incorrect
    sprintf(buf, "Temp: % 2.2f *C ", (float)(temp / 100.0));
    Serial.print(buf);
    sprintf(buf, "Humidity: % 2.2f%% ", (float)(humidity / 1000.0));
    Serial.print(buf);
    sprintf(buf, "Pressure % 4.2f hPa ", (float)(pressure / 100.0));
    Serial.print(buf);
    alt = altitude(pressure, 1008.1);
    sprintf(buf, "Altitude: % 3d m ", (int16_t)(alt));
    Serial.print(buf);
    sprintf(buf, "Gas: % 4d / % 02d\n", (int16_t)(gas / 100), (uint8_t)(gas % 100));
    Serial.print(buf);
    delay(10000); // Wait 10s
  }
}
