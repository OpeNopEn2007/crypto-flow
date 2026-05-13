# Qt 6 - Sensors Module

> **来源**: https://doc.qt.io/qt-6/qtsensors-module.html
> **获取日期**: 2026-05-12

# Qt Sensors C++ Classes

Provides classes for reading sensor data. [More\...](#details)

## Classes(#classes "Direct link to this headline")

  [QAccelerometer](qaccelerometer.html)                           Convenience wrapper around QSensor
  [QAccelerometerFilter](qaccelerometerfilter.html)               Convenience wrapper around QSensorFilter
  [QAccelerometerReading](qaccelerometerreading.html)             Reports on linear acceleration along the X, Y and Z axes
  [QAmbientLightFilter](qambientlightfilter.html)                 Convenience wrapper around QSensorFilter
  [QAmbientLightReading](qambientlightreading.html)               Represents one reading from the ambient light sensor
  [QAmbientLightSensor](qambientlightsensor.html)                 Convenience wrapper around QSensor
  [QAmbientTemperatureFilter](qambienttemperaturefilter.html)     Convenience wrapper around QSensorFilter
  [QAmbientTemperatureReading](qambienttemperaturereading.html)   Holds readings of the ambient temperature
  [QAmbientTemperatureSensor](qambienttemperaturesensor.html)     Convenience wrapper around QSensor
  [QCompass](qcompass.html)                                       Convenience wrapper around QSensor
  [QCompassFilter](qcompassfilter.html)                           Convenience wrapper around QSensorFilter
  [QCompassReading](qcompassreading.html)                         Represents one reading from a compass
  [QGyroscope](qgyroscope.html)                                   Convenience wrapper around QSensor
  [QGyroscopeFilter](qgyroscopefilter.html)                       Convenience wrapper around QSensorFilter
  [QGyroscopeReading](qgyroscopereading.html)                     Represents one reading from the gyroscope sensor
  [QHumidityFilter](qhumidityfilter.html)                         Convenience wrapper around QSensorFilter
  [QHumidityReading](qhumidityreading.html)                       Holds readings from the humidity sensor
  [QHumiditySensor](qhumiditysensor.html)                         Convenience wrapper around QSensor
  [QLightFilter](qlightfilter.html)                               Convenience wrapper around QSensorFilter
  [QLightReading](qlightreading.html)                             Represents one reading from the light sensor
  [QLightSensor](qlightsensor.html)                               Convenience wrapper around QSensor
  [QMagnetometer](qmagnetometer.html)                             Convenience wrapper around QSensor
  [QMagnetometerFilter](qmagnetometerfilter.html)                 Convenience wrapper around QSensorFilter
  [QMagnetometerReading](qmagnetometerreading.html)               Represents one reading from the magnetometer
  [QOrientationFilter](qorientationfilter.html)                   Convenience wrapper around QSensorFilter
  [QOrientationReading](qorientationreading.html)                 Represents one reading from the orientation sensor
  [QOrientationSensor](qorientationsensor.html)                   Convenience wrapper around QSensor
  [QPressureFilter](qpressurefilter.html)                         Convenience wrapper around QSensorFilter
  [QPressureReading](qpressurereading.html)                       Holds readings from the pressure sensor
  [QPressureSensor](qpressuresensor.html)                         Convenience wrapper around QSensor
  [QProximityFilter](qproximityfilter.html)                       Convenience wrapper around QSensorFilter
  [QProximityReading](qproximityreading.html)                     Represents one reading from the proximity sensor
  [QProximitySensor](qproximitysensor.html)                       Convenience wrapper around QSensor
  [QRotationFilter](qrotationfilter.html)                         Convenience wrapper around QSensorFilter
  [QRotationReading](qrotationreading.html)                       Represents one reading from the rotation sensor
  [QRotationSensor](qrotationsensor.html)                         Convenience wrapper around QSensor
  [QSensor](qsensor.html)                                         Represents a single hardware sensor
  [QSensorBackend](qsensorbackend.html)                           Sensor implementation
  [QSensorBackendFactory](qsensorbackendfactory.html)             Instantiates instances of QSensorBackend
  [QSensorChangesInterface](qsensorchangesinterface.html)         The pure virtual interface to sensor plugins
  [QSensorFilter](qsensorfilter.html)                             Efficient callback facility for asynchronous notifications of sensor changes
  [QSensorManager](qsensormanager.html)                           Handles registration and creation of sensor backends
  [QSensorPluginInterface](qsensorplugininterface.html)           The pure virtual interface to sensor plugins
  [QSensorReading](qsensorreading.html)                           Holds the readings from the sensor
  [QTiltFilter](qtiltfilter.html)                                 Convenience wrapper around QSensorFilter
  [QTiltReading](qtiltreading.html)                               Holds readings from the tilt sensor
  [QTiltSensor](qtiltsensor.html)                                 Convenience wrapper around QSensor
  [qoutputrange](qoutputrange.html)                               Holds the specifics of an output range

## Detailed Description(#details "Direct link to this headline")

Mobile devices contain sensor hardware that allow detecting changes in various physical properties of the device itself or its immediate environment. Examples of device properties include the angle at which the device is held, whereas environmental properties include for example the current level of ambient light.
