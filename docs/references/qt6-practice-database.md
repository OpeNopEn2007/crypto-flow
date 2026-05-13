# Qt 6 数据库编程 (SQLite/MySQL 集成)

> 来源: https://doc.qt.io/qt-6/sql-connecting.html, https://doc.qt.io/qt-6/sql-sqlstatements.html
> 获取日期: 2026-05-12

---

## 一、模块概览

Qt 6 通过 `Qt Sql` 模块提供数据库支持，核心类：

| 类名 | 用途 |
|------|------|
| `QSqlDatabase` | 数据库连接管理 |
| `QSqlQuery` | 执行 SQL 语句和遍历结果集 |
| `QSqlQueryModel` | SQL 查询结果的只读数据模型 |
| `QSqlTableModel` | 可编辑的 SQL 数据模型（支持 View 绑定） |
| `QSqlError` | 数据库错误信息 |
| `QSqlDriver` | 底层数据库驱动接口 |

### 支持的数据库驱动

| 驱动名 | 数据库 |
|--------|--------|
| QSQLITE | SQLite |
| QMYSQL | MySQL |
| QPSQL | PostgreSQL |
| QODBC | ODBC (SQL Server 等) |

---

## 二、CMake 配置

```cmake
find_package(Qt6 REQUIRED COMPONENTS Sql)
target_link_libraries(myapp PRIVATE Qt6::Sql)
```

---

## 三、连接数据库

### SQLite 连接

```cpp
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

// 创建 SQLite 连接（内置驱动，无需额外配置）
QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
db.setDatabaseName("my_database.db");

if (!db.open()) {
    qWarning() << "Cannot open database:" << db.lastError().text();
    return;
}

qDebug() << "Database connected successfully";
```

### MySQL 连接

```cpp
QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
db.setHostName("localhost");
db.setDatabaseName("mydb");
db.setUserName("root");
db.setPassword("password");
db.setPort(3306);

if (!db.open()) {
    qWarning() << "MySQL connection failed:" << db.lastError().text();
    return;
}
```

### 多连接管理

```cpp
// 命名连接（支持多个连接）
QSqlDatabase firstDB = QSqlDatabase::addDatabase("QSQLITE", "first");
firstDB.setDatabaseName("db1.db");
firstDB.open();

QSqlDatabase secondDB = QSqlDatabase::addDatabase("QSQLITE", "second");
secondDB.setDatabaseName("db2.db");
secondDB.open();

// 通过连接名获取连接
QSqlDatabase defaultDB = QSqlDatabase::database();        // 默认连接
QSqlDatabase first = QSqlDatabase::database("first");     // 命名连接
```

### 关闭和移除连接

```cpp
db.close();
// 移除连接前必须先关闭
QSqlDatabase::removeDatabase("connection_name");
```

---

## 四、执行 SQL 语句

### 基本查询

```cpp
QSqlQuery query;
query.exec("SELECT name, salary FROM employee WHERE salary > 50000");

// 遍历结果集
while (query.next()) {
    QString name = query.value(0).toString();
    int salary = query.value(1).toInt();
    qDebug() << name << salary;
}
```

### 插入记录

```cpp
QSqlQuery query;
query.exec("INSERT INTO employee (id, name, salary) "
           "VALUES (1001, 'Thad Beaumont', 65000)");
```

### 命名绑定（推荐）

```cpp
QSqlQuery query;
query.prepare("INSERT INTO employee (id, name, salary) "
              "VALUES (:id, :name, :salary)");
query.bindValue(":id", 1001);
query.bindValue(":name", "Thad Beaumont");
query.bindValue(":salary", 65000);
query.exec();
```

### 位置绑定

```cpp
QSqlQuery query;
query.prepare("INSERT INTO employee (id, name, salary) "
              "VALUES (?, ?, ?)");
query.addBindValue(1001);
query.addBindValue("Thad Beaumont");
query.addBindValue(65000);
query.exec();
```

### 更新和删除

```cpp
// 更新
QSqlQuery query;
query.exec("UPDATE employee SET salary = 70000 WHERE id = 1003");

// 删除
query.exec("DELETE FROM employee WHERE id = 1007");
```

---

## 五、事务处理

```cpp
// 启动事务
QSqlDatabase::database().transaction();

QSqlQuery query;
query.exec("SELECT id FROM employee WHERE name = 'Torild Halvorsen'");
if (query.next()) {
    int employeeId = query.value(0).toInt();
    query.exec("INSERT INTO project (id, name, ownerid) "
               "VALUES (201, 'Manhattan Project', "
               + QString::number(employeeId) + ')');
}

// 提交事务
QSqlDatabase::database().commit();

// 或回滚
// QSqlDatabase::database().rollback();
```

---

## 六、性能优化

### 只向前遍历

```cpp
QSqlQuery query;
query.setForwardOnly(true);  // 只允许向前遍历，大幅提升性能
query.exec("SELECT * FROM large_table");
while (query.next()) {
    // 处理数据
}
```

### 检查驱动特性

```cpp
QSqlQuery query;
query.exec("SELECT name, salary FROM employee WHERE salary > 50000");

QSqlDatabase defaultDB = QSqlDatabase::database();
int numRows;
if (defaultDB.driver()->hasFeature(QSqlDriver::QuerySize)) {
    numRows = query.size();
} else {
    query.last();
    numRows = query.at() + 1;
}
```

---

## 七、完整 SQLite 示例：创建表并操作

```cpp
#include <QCoreApplication>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // 连接 SQLite
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("app_data.db");

    if (!db.open()) {
        qCritical() << "Database error:" << db.lastError().text();
        return -1;
    }

    // 创建表
    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS users ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT,"
               "name TEXT NOT NULL,"
               "email TEXT UNIQUE,"
               "created_at DATETIME DEFAULT CURRENT_TIMESTAMP"
               ")");

    // 插入数据
    query.prepare("INSERT INTO users (name, email) VALUES (:name, :email)");
    query.bindValue(":name", "Alice");
    query.bindValue(":email", "alice@example.com");
    if (!query.exec()) {
        qWarning() << "Insert failed:" << query.lastError().text();
    }

    // 查询数据
    query.exec("SELECT id, name, email FROM users");
    while (query.next()) {
        qDebug() << "ID:" << query.value(0).toInt()
                 << "Name:" << query.value(1).toString()
                 << "Email:" << query.value(2).toString();
    }

    db.close();
    return 0;
}
```

---

## 八、最佳实践

1. **使用命名绑定**而非字符串拼接，防止 SQL 注入
2. **调用 `setForwardOnly(true)`** 优化大结果集遍历
3. **检查 `lastError()`** 处理数据库错误
4. **事务批量操作**：多条 INSERT/UPDATE 在事务中执行更快
5. **SQLite 连接字符串**: 生产环境建议使用完整路径 `db.setDatabaseName("/path/to/db.sqlite")`
6. **关闭连接**：程序退出前调用 `db.close()`
7. **多线程**: 每个线程应有独立的数据库连接
