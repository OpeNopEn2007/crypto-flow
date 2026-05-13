# Qt 6 - NFC Module

> **来源**: https://doc.qt.io/qt-6/qtnfc-module.html
> **获取日期**: 2026-05-12

# Qt NFC C++ Classes

An API for accessing NFC Forum Tags. [More\...](#details)

## Classes(#classes "Direct link to this headline")

  [QNdefFilter](qndeffilter.html)                                  Filter for matching NDEF messages
  [QNdefFilter::Record](qndeffilter-record.html)                   QNdefFilter::Record struct contains the information about a filter record
  [QNdefMessage](qndefmessage.html)                                NFC NDEF message
  [QNdefNfcIconRecord](qndefnfciconrecord.html)                    NFC MIME record to hold an icon
  [QNdefNfcSmartPosterRecord](qndefnfcsmartposterrecord.html)      NFC RTD-SmartPoster
  [QNdefNfcTextRecord](qndefnfctextrecord.html)                    NFC RTD-Text
  [QNdefNfcUriRecord](qndefnfcurirecord.html)                      NFC RTD-URI
  [QNdefRecord](qndefrecord.html)                                  NFC NDEF record
  [QNearFieldManager](qnearfieldmanager.html)                      Access to notifications for NFC events
  [QNearFieldTarget](qnearfieldtarget.html)                        Interface for communicating with a target device
  [QNearFieldTarget::RequestId](qnearfieldtarget-requestid.html)   A request id handle

## Detailed Description(#details "Direct link to this headline")

The [Qt NFC](qtnfc-index.html) C++ API enables an application to access NFC Forum Tags.

### Using the Module(#using-the-module "Direct link to this headline")

Using a Qt module requires linking against the module library, either directly or through other dependencies. Several build tools have dedicated support for this, including [CMake](https://cmake.org/cmake/help/latest/) and [qmake](qmake-manual.html).

#### Building with CMake(#building-with-cmake "Direct link to this headline")

Use the `find_package()` command to locate the needed module components in the `Qt6` package:

``` {.cpp .plain translate="no"}
find_package(Qt6 REQUIRED COMPONENTS Nfc)
target_link_libraries(mytarget PRIVATE Qt::Nfc)
```

#### Building with qmake(#building-with-qmake "Direct link to this headline")

To configure the module for building with qmake, add the module as a value of the `QT` variable in the project's .pro file:

``` {.cpp .plain translate="no"}
QT += nfc
```
