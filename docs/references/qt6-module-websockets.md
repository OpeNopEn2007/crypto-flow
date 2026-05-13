---
source: https://doc.qt.io/qt-6/qtwebsockets-module.html
fetched: 2026-05-12
module: Qt WebSockets
---

# Qt WebSockets C++ Classes

List of C++ classes that enable WebSocket-based communication. [More\...](#details)

## Namespaces

  [QWebSocketProtocol](qwebsocketprotocol.html)   Contains constants related to the WebSocket standard

## Classes

  [QMaskGenerator](qmaskgenerator.html)                             Abstract base for custom 32-bit mask generators
  [QWebSocket](qwebsocket.html)                                     Implements a TCP socket that talks the WebSocket protocol
  [QWebSocketCorsAuthenticator](qwebsocketcorsauthenticator.html)   Authenticator object for Cross Origin Requests (CORS)
  [QWebSocketHandshakeOptions](qwebsockethandshakeoptions.html)     Collects options for the WebSocket handshake
  [QWebSocketServer](qwebsocketserver.html)                         Implements a WebSocket-based server

## Detailed Description

To use these classes in your application, use the following include statement:

```
#include <QtWebSockets/QtWebSockets>
```

To link against the module, add this line to your [qmake](qmake-manual.html) `.pro` file:

```
QT += websockets
```
