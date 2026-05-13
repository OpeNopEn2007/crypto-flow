# Qt 6 网络编程 (REST API, WebSocket)

> 来源: https://doc.qt.io/qt-6/qtnetwork-programming.html, https://doc.qt.io/qt-6/qrestaccessmanager.html
> 获取日期: 2026-05-12

---

## 一、模块概览

Qt 6 通过 `Qt Network` 模块提供网络编程支持：

| 类名 | 用途 |
|------|------|
| `QNetworkAccessManager` | 高层 HTTP 操作管理器 |
| `QRestAccessManager` | REST API 便捷封装 (Qt 6.7+) |
| `QRestReply` | REST 响应处理 (Qt 6.7+) |
| `QNetworkRequest` | 网络请求封装 |
| `QNetworkReply` | 网络响应处理 |
| `QTcpSocket` | TCP 客户端 |
| `QTcpServer` | TCP 服务器 |
| `QUdpSocket` | UDP 通信 |
| `QWebSocket` | WebSocket 客户端 |
| `QWebSocketServer` | WebSocket 服务器 |

### CMake 配置

```cmake
find_package(Qt6 REQUIRED COMPONENTS Network)
target_link_libraries(myapp PRIVATE Qt6::Network)
```

---

## 二、QNetworkAccessManager - HTTP 请求

### GET 请求

```cpp
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>

QNetworkAccessManager *manager = new QNetworkAccessManager(this);

QNetworkRequest request(QUrl("https://api.example.com/data"));
request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

QNetworkReply *reply = manager->get(request);

connect(reply, &QNetworkReply::finished, [reply]() {
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray data = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(data);
        qDebug() << "Response:" << doc.object();
    } else {
        qWarning() << "Error:" << reply->errorString();
    }
    reply->deleteLater();
});
```

### POST 请求

```cpp
QNetworkRequest request(QUrl("https://api.example.com/users"));
request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

QJsonObject userData;
userData["name"] = "John Doe";
userData["email"] = "john@example.com";

QNetworkReply *reply = manager->post(request, QJsonDocument(userData).toJson());

connect(reply, &QNetworkReply::finished, [reply]() {
    QByteArray response = reply->readAll();
    qDebug() << "Status:" << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    qDebug() << "Response:" << response;
    reply->deleteLater();
});
```

### PUT / DELETE 请求

```cpp
// PUT
QNetworkReply *putReply = manager->put(request, QJsonDocument(updatedData).toJson());

// DELETE
QNetworkReply *deleteReply = manager->deleteResource(request);
```

---

## 三、QRestAccessManager - REST API 便捷封装 (Qt 6.7+)

`QRestAccessManager` 是 `QNetworkAccessManager` 的便捷封装，提供类型安全的 REST 操作和回调式 API。

### 基本用法

```cpp
#include <QRestAccessManager>
#include <QRestReply>

// 创建
QRestAccessManager restManager(networkAccessManager);

// GET 请求（回调方式）
QNetworkRequest request(QUrl("https://api.example.com/users"));
restManager.get(request, this, [](QRestReply &reply) {
    if (reply.isSuccess()) {
        auto data = reply.readJson();  // 直接解析 JSON
        qDebug() << data.object();
    } else {
        qWarning() << "Request failed";
    }
});

// POST 请求（直接发送 JSON）
QNetworkRequest postRequest(QUrl("https://api.example.com/users"));
QJsonObject userData = {{"name", "Alice"}, {"email", "alice@test.com"}};
restManager.post(postRequest, QJsonDocument(userData), this, [](QRestReply &reply) {
    if (reply.isSuccess()) {
        qDebug() << "User created";
    }
});

// PUT 请求
restManager.put(request, QJsonDocument(updateData), this, callback);

// DELETE 请求
restManager.deleteResource(request, this, callback);
```

### 支持的数据类型

`QRestAccessManager` 支持多种请求体类型：
- `QJsonDocument` - JSON 数据
- `QVariantMap` - 键值对（自动转 JSON）
- `QByteArray` - 原始字节
- `QIODevice` - 流式数据
- `QHttpMultiPart` - multipart 表单（文件上传）

---

## 四、TCP 编程

### TCP 客户端

```cpp
#include <QTcpSocket>

QTcpSocket *socket = new QTcpSocket(this);

connect(socket, &QTcpSocket::connected, [socket]() {
    qDebug() << "Connected to server";
    socket->write("Hello, server!");
});

connect(socket, &QTcpSocket::readyRead, [socket]() {
    QByteArray data = socket->readAll();
    qDebug() << "Received:" << data;
});

connect(socket, &QTcpSocket::disconnected, []() {
    qDebug() << "Disconnected";
});

socket->connectToHost("127.0.0.1", 9090);
```

### TCP 服务器

```cpp
#include <QTcpServer>
#include <QTcpSocket>

QTcpServer *server = new QTcpServer(this);

connect(server, &QTcpServer::newConnection, [server]() {
    while (server->hasPendingConnections()) {
        QTcpSocket *client = server->nextPendingConnection();
        qDebug() << "New client:" << client->peerAddress().toString();

        connect(client, &QTcpSocket::readyRead, [client]() {
            QByteArray data = client->readAll();
            client->write("Echo: " + data);
        });

        connect(client, &QTcpSocket::disconnected, [client]() {
            client->deleteLater();
        });
    }
});

if (!server->listen(QHostAddress::Any, 9090)) {
    qWarning() << "Server could not start:" << server->errorString();
} else {
    qDebug() << "Server started on port 9090";
}
```

---

## 五、WebSocket 编程

### WebSocket 客户端

```cpp
#include <QWebSocket>

QWebSocket *ws = new QWebSocket("wss://echo.websocket.org");

connect(ws, &QWebSocket::connected, [ws]() {
    qDebug() << "WebSocket connected";
    ws->sendTextMessage("Hello, WebSocket!");
});

connect(ws, &QWebSocket::textMessageReceived, [](const QString &message) {
    qDebug() << "Received:" << message;
});

connect(ws, &QWebSocket::disconnected, []() {
    qDebug() << "WebSocket disconnected";
});

ws->open(QUrl("wss://echo.websocket.org"));
```

### WebSocket 服务器

```cpp
#include <QWebSocketServer>
#include <QWebSocket>
#include <QList>

class ChatServer : public QObject {
    Q_OBJECT
public:
    ChatServer(quint16 port, QObject *parent = nullptr) : QObject(parent) {
        m_server = new QWebSocketServer(
            "Chat Server",
            QWebSocketServer::NonSecureMode,
            this
        );

        if (m_server->listen(QHostAddress::Any, port)) {
            connect(m_server, &QWebSocketServer::newConnection,
                    this, &ChatServer::onNewConnection);
            qDebug() << "WebSocket server listening on port" << port;
        }
    }

private slots:
    void onNewConnection() {
        while (m_server->hasPendingConnections()) {
            QWebSocket *socket = m_server->nextPendingConnection();
            m_clients.append(socket);

            connect(socket, &QWebSocket::textMessageReceived,
                    this, &ChatServer::processMessage);
            connect(socket, &QWebSocket::disconnected,
                    this, &ChatServer::socketDisconnected);
        }
    }

    void processMessage(const QString &message) {
        QWebSocket *sender = qobject_cast<QWebSocket*>(sender());
        // 广播给所有客户端
        for (QWebSocket *client : m_clients) {
            if (client != sender) {
                client->sendTextMessage(message);
            }
        }
    }

    void socketDisconnected() {
        QWebSocket *socket = qobject_cast<QWebSocket*>(sender());
        m_clients.removeOne(socket);
        socket->deleteLater();
    }

private:
    QWebSocketServer *m_server;
    QList<QWebSocket*> m_clients;
};
```

---

## 六、HTTPS/SSL 配置

```cpp
// 设置 SSL 配置
QSslConfiguration sslConfig = QSslConfiguration::defaultConfiguration();
sslConfig.setProtocol(QSsl::TlsV1_2OrLater);

QNetworkRequest request(QUrl("https://api.example.com/data"));
request.setSslConfiguration(sslConfig);
request.setRawHeader("Authorization", "Bearer YOUR_TOKEN");

QNetworkReply *reply = manager->get(request);
```

---

## 七、最佳实践

1. **总是检查 `reply->error()`** 再读取数据
2. **调用 `reply->deleteLater()`** 释放资源
3. **使用 `QRestAccessManager`** (Qt 6.7+) 替代手动 JSON 构造
4. **设置超时**: `QNetworkRequest::setTransferTimeout()` (Qt 6.6+)
5. **Token 认证**: 通过 `request.setRawHeader("Authorization", ...)` 传递
6. **线程安全**: `QNetworkAccessManager` 不是线程安全的，每个线程应创建独立实例
7. **WebSocket**: 使用 `wss://` 协议确保加密通信
