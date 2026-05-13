# Qt D-Bus Module

> **Source**: <https://doc.qt.io/qt-6/qtdbus-module.html>
> **Fetched**: 2026-05-12

---

# Qt D-Bus C++ Classes

The Qt D-Bus module is a Unix-only library that you can use to perform Inter-Process Communication using the [D-Bus](qtdbus-index.html) protocol. [More\...](#details)

## Namespaces

  [QDBus](qdbus.html)   Contains miscellaneous identifiers used throughout the Qt D-Bus module

## Classes

  [QDBusAbstractAdaptor](qdbusabstractadaptor.html)           The base class of D-Bus adaptor classes
  [QDBusAbstractInterface](qdbusabstractinterface.html)       The base class for all D-Bus interfaces in the Qt D-Bus binding, allowing access to remote interfaces
  [QDBusArgument](qdbusargument.html)                         Used to marshall and demarshall D-Bus arguments
  [QDBusConnection](qdbusconnection.html)                     Represents a connection to the D-Bus bus daemon
  [QDBusConnectionInterface](qdbusconnectioninterface.html)   Access to the D-Bus bus daemon service
  [QDBusContext](qdbuscontext.html)                           Allows slots to determine the D-Bus context of the calls
  [QDBusError](qdbuserror.html)                               Represents an error received from the D-Bus bus or from remote applications found in the bus
  [QDBusInterface](qdbusinterface.html)                       Proxy for interfaces on remote objects
  [QDBusMessage](qdbusmessage.html)                           Represents one message sent or received over the D-Bus bus
  [QDBusObjectPath](qdbusobjectpath.html)                     Enables the programmer to identify the OBJECT_PATH type provided by the D-Bus typesystem
  [QDBusPendingCall](qdbuspendingcall.html)                   Refers to one pending asynchronous call
  [QDBusPendingCallWatcher](qdbuspendingcallwatcher.html)     Convenient way for waiting for asynchronous replies
  [QDBusPendingReply](qdbuspendingreply.html)                 Contains the reply to an asynchronous method call
  [QDBusReply](qdbusreply.html)                               Stores the reply for a method call to a remote object
  [QDBusServer](qdbusserver.html)                             Peer-to-peer communication between processes on the same computer
  [QDBusServiceWatcher](qdbusservicewatcher.html)             Allows the user to watch for a bus service change
  [QDBusSignature](qdbussignature.html)                       Enables the programmer to identify the SIGNATURE type provided by the D-Bus typesystem
  [QDBusUnixFileDescriptor](qdbusunixfiledescriptor.html)     Holds one Unix file descriptor
  [QDBusVariant](qdbusvariant.html)                           Enables the programmer to identify the variant type provided by the D-Bus typesystem
  [QDBusVirtualObject](qdbusvirtualobject.html)               Used to handle several DBus paths with one class

## Detailed Description

Applications using the Qt D-Bus module can provide services to other, remote applications by exporting objects, as well as use services exported by those applications by placing calls and accessing properties.

The [Qt D-Bus](qtdbus-index.html) page contains information about how to use the module.
