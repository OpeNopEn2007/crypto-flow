# Qt 6 - Bluetooth Module

> **来源**: https://doc.qt.io/qt-6/qtbluetooth-module.html
> **获取日期**: 2026-05-12

# Qt Bluetooth C++ Classes

Enables basic Bluetooth operations like scanning for devices and connecting them. [More\...](#details)

## Namespaces(#namespaces "Direct link to this headline")

  [QBluetooth](qbluetooth.html)   Classes and functions related to Bluetooth

## Classes(#classes "Direct link to this headline")

  [QBluetoothAddress](qbluetoothaddress.html)                                                        Assigns an address to the Bluetooth device
  [QBluetoothDeviceDiscoveryAgent](qbluetoothdevicediscoveryagent.html)                              Discovers the Bluetooth devices nearby
  [QBluetoothDeviceInfo](qbluetoothdeviceinfo.html)                                                  Stores information about the Bluetooth device
  [QBluetoothHostInfo](qbluetoothhostinfo.html)                                                      Encapsulates the details of a local QBluetooth device
  [QBluetoothLocalDevice](qbluetoothlocaldevice.html)                                                Enables access to the local Bluetooth device
  [QBluetoothServer](qbluetoothserver.html)                                                          Uses the RFCOMM or L2cap protocol to communicate with a Bluetooth device
  [QBluetoothServiceDiscoveryAgent](qbluetoothservicediscoveryagent.html)                            Enables you to query for Bluetooth services
  [QBluetoothServiceInfo](qbluetoothserviceinfo.html)                                                Enables access to the attributes of a Bluetooth service
  [QBluetoothServiceInfo::Alternative](qbluetoothserviceinfo-alternative.html)                       Stores attributes of a Bluetooth Data Element Alternative
  [QBluetoothServiceInfo::Sequence](qbluetoothserviceinfo-sequence.html)                             Stores attributes of a Bluetooth Data Element Sequence
  [QBluetoothSocket](qbluetoothsocket.html)                                                          Enables connection to a Bluetooth device running a bluetooth server
  [QBluetoothUuid](qbluetoothuuid.html)                                                              Generates a UUID for each Bluetooth service
  [QLowEnergyAdvertisingData](qlowenergyadvertisingdata.html)                                        Represents the data to be broadcast during Bluetooth Low Energy advertising
  [QLowEnergyAdvertisingParameters](qlowenergyadvertisingparameters.html)                            Represents the parameters used for Bluetooth Low Energy advertising
  [QLowEnergyAdvertisingParameters::AddressInfo](qlowenergyadvertisingparameters-addressinfo.html)   QLowEnergyAdvertisingParameters::AddressInfo defines the elements of a white list
  [QLowEnergyCharacteristic](qlowenergycharacteristic.html)                                          Stores information about a Bluetooth Low Energy service characteristic
  [QLowEnergyCharacteristicData](qlowenergycharacteristicdata.html)                                  Used to set up GATT service data
  [QLowEnergyConnectionParameters](qlowenergyconnectionparameters.html)                              Used when requesting or reporting an update of the parameters of a Bluetooth LE connection
  [QLowEnergyController](qlowenergycontroller.html)                                                  Access to Bluetooth Low Energy Devices
  [QLowEnergyDescriptor](qlowenergydescriptor.html)                                                  Stores information about the Bluetooth Low Energy descriptor
  [QLowEnergyDescriptorData](qlowenergydescriptordata.html)                                          Used to create GATT service data
  [QLowEnergyService](qlowenergyservice.html)                                                        Represents an individual service on a Bluetooth Low Energy Device
  [QLowEnergyServiceData](qlowenergyservicedata.html)                                                Used to set up GATT service data

## Detailed Description(#details "Direct link to this headline")

The [Qt Bluetooth](qtbluetooth-index.html) C++ API enables an application to scan for devices and connect and interact with them in a more flexible way.

To use the C++ library in your application, add the following configuration option to your `.pro` file:

``` {.cpp .prettyprint translate="no"}
QT += bluetooth
```
