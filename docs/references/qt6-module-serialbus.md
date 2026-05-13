# Qt Serial Bus Module

> **Source**: <https://doc.qt.io/qt-6/qtserialbus-module.html>
> **Fetched**: 2026-05-12

---

# Qt Serial Bus C++ Classes

Provides classes to read and write serial bus data. [More\...](#details)

## Namespaces

  [QtCanBus](qtcanbus.html)   Some commons enums that are used in the CAN bus handling part of the QtSerialPort module

## Classes

  [QCanBus](qcanbus.html)                                                                        Handles registration and creation of bus plugins
  [QCanBusDevice](qcanbusdevice.html)                                                            The interface class for CAN bus
  [QCanBusDevice::Filter](qcanbusdevice-filter.html)                                             QCanBusDevice::Filter struct defines a filter for CAN bus frames
  [QCanBusDeviceInfo](qcanbusdeviceinfo.html)                                                    Information about CAN bus interfaces
  [QCanBusFactory](qcanbusfactory.html)                                                          Factory class used as the plugin interface for CAN bus plugins
  [QCanBusFrame](qcanbusframe.html)                                                              Container class representing a single CAN frame
  [QCanBusFrame::TimeStamp](qcanbusframe-timestamp.html)                                         Timestamp information with microsecond precision
  [QCanDbcFileParser](qcandbcfileparser.html)                                                    Can be used to parse DBC files
  [QCanFrameProcessor](qcanframeprocessor.html)                                                  Can be used to decode a QCanBusFrame or to convert the input data into a QCanBusFrame that is ready to be sent to the receiver
  [QCanFrameProcessor::ParseResult](qcanframeprocessor-parseresult.html)                         Struct is used as a return value for the QCanFrameProcessor::parseFrame() method
  [QCanMessageDescription](qcanmessagedescription.html)                                          Describes the rules to process a CAN message and represent it in an application-defined format
  [QCanSignalDescription](qcansignaldescription.html)                                            Describes the rules to extract one value out of the CAN frame and represent it in an application-defined format
  [QCanSignalDescription::MultiplexValueRange](qcansignaldescription-multiplexvaluerange.html)   Defines a range of values for a multiplexor signal
  [QCanUniqueIdDescription](qcanuniqueiddescription.html)                                        Describes the rules for accessing a unique identifier in a QCanBusFrame
  [QModbusClient](qmodbusclient.html)                                                            The interface to send Modbus requests
  [QModbusDataUnit](qmodbusdataunit.html)                                                        Container class representing single bit and 16 bit word entries in the Modbus register
  [QModbusDevice](qmodbusdevice.html)                                                            The base class for Modbus classes, QModbusServer and QModbusClient
  [QModbusDeviceIdentification](qmodbusdeviceidentification.html)                                Container class representing the physical and functional description of a Modbus server
  [QModbusExceptionResponse](qmodbusexceptionresponse.html)                                      Container class containing the function and error code inside a Modbus ADU
  [QModbusPdu](qmodbuspdu.html)                                                                  Abstract container class containing the function code and payload that is stored inside a Modbus ADU
  [QModbusReply](qmodbusreply.html)                                                              Contains the data for a request sent with a QModbusClient derived class
  [QModbusRequest](qmodbusrequest.html)                                                          Container class containing the function code and payload that is stored inside a Modbus ADU
  [QModbusResponse](qmodbusresponse.html)                                                        Container class containing the function code and payload that is stored inside a Modbus ADU
  [QModbusRtuSerialClient](qmodbusrtuserialclient.html)                                          Represents a Modbus client that uses a serial bus for its communication with the Modbus server
  [QModbusRtuSerialServer](qmodbusrtuserialserver.html)                                          Represents a Modbus server that uses a serial port for its communication with the Modbus client
  [QModbusServer](qmodbusserver.html)                                                            The interface to receive and process Modbus requests
  [QModbusTcpClient](qmodbustcpclient.html)                                                      The interface class for Modbus TCP client device
  [QModbusTcpConnectionObserver](qmodbustcpconnectionobserver.html)                              Represents the interface for objects that can be passed to QModbusTcpServer::installConnectionObserver
  [QModbusTcpServer](qmodbustcpserver.html)                                                      Represents a Modbus server that uses a TCP server for its communication with the Modbus client

## Detailed Description

Provides classes to access various serial buses.

For C++ projects include the header appropriate for the current use case, for example applications using the CAN bus device may use

``` cpp
#include <QCanBusDevice>
```

To use the module with CMake, use the `find_package()` command to locate the needed module components in the `Qt6` package:

``` cpp
find_package(Qt6 COMPONENTS SerialBus REQUIRED)
target_link_libraries(mytarget PRIVATE Qt6::SerialBus)
```

To use the module for building with qmake, add the module as a value of the `QT` variable in the project's .pro file:

``` cpp
QT += serialbus
```

See more in the [Qt Serial Bus Overview](qtserialbus-index.html).
