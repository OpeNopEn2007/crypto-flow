---
source: https://doc.qt.io/qt-6/metaobjects.html
fetched: 2026-05-12
---

# The Meta-Object System

Qt\'s meta-object system provides the signals and slots mechanism for inter-object communication, run-time type information, and the dynamic property system.

The meta-object system is based on three things:

1.  The [QObject](qobject.html) class provides a base class for objects that can take advantage of the meta-object system.
2.  The [Q_OBJECT](qobject.html#Q_OBJECT) macro is used to enable meta-object features, such as dynamic properties, signals, and slots.
3.  The [Meta-Object Compiler](moc.html) (`moc`) supplies each [QObject](qobject.html) subclass with the necessary code to implement meta-object features.

The `moc` tool reads a C++ source file. If it finds one or more class declarations that contain the [Q_OBJECT](qobject.html#Q_OBJECT) macro, it produces another C++ source file which contains the meta-object code for each of those classes. This generated source file is either `#include`\'d into the class\'s source file or, more usually, compiled and linked with the class\'s implementation.

In addition to providing the [signals and slots](signalsandslots.html) mechanism for communication between objects (the main reason for introducing the system), the meta-object code provides the following additional features:

- [QObject::metaObject](qobject.html#metaObject)() returns the associated [meta-object](qmetaobject.html) for the class.
- [QMetaObject::className](qmetaobject.html#className)() returns the class name as a string at run-time, without requiring native run-time type information (RTTI) support through the C++ compiler.
- [QObject::inherits](qobject.html#inherits)() function returns whether an object is an instance of a class that inherits a specified class within the [QObject](qobject.html) inheritance tree.
- [QObject::tr](qobject.html#tr)() translates strings for [internationalization](internationalization.html).
- [QObject::setProperty](qobject.html#setProperty)() and [QObject::property](qobject.html#property)() dynamically set and get properties by name.
- [QMetaObject::newInstance](qmetaobject.html#newInstance)() constructs a new instance of the class.

[]

It is also possible to perform dynamic casts using [qobject_cast](qobject.html#qobject_cast)() on [QObject](qobject.html) classes. The [qobject_cast](qobject.html#qobject_cast)() function behaves similarly to the standard C++ `dynamic_cast()`, with the advantages that it doesn\'t require RTTI support and it works across dynamic library boundaries. It attempts to cast its argument to the pointer type specified in angle-brackets, returning a non-zero pointer if the object is of the correct type (determined at run-time), or `nullptr` if the object\'s type is incompatible.

For example, let\'s assume `MyWidget` inherits from [QWidget](qwidget.html) and is declared with the [Q_OBJECT](qobject.html#Q_OBJECT) macro:

::: pre
``` {.cpp .prettyprint translate="no"}
    QObject *obj = new MyWidget;
```

The `obj` variable, of type `QObject *`, actually refers to a `MyWidget` object, so we can cast it appropriately:

::: pre
``` {.cpp .prettyprint translate="no"}
    QWidget *widget = qobject_cast<QWidget *>(obj);
```

The cast from [QObject](qobject.html) to [QWidget](qwidget.html) is successful, because the object is actually a `MyWidget`, which is a subclass of [QWidget](qwidget.html). Since we know that `obj` is a `MyWidget`, we can also cast it to `MyWidget *`:

::: pre
``` {.cpp .prettyprint translate="no"}
    MyWidget *myWidget = qobject_cast<MyWidget *>(obj);
```

The cast to `MyWidget` is successful because [qobject_cast](qobject.html#qobject_cast)() makes no distinction between built-in Qt types and custom types.

::: pre
``` {.cpp .prettyprint translate="no"}
    QLabel *label = qobject_cast<QLabel *>(obj);
    // label is nullptr
```

The cast to [QLabel](qlabel.html), on the other hand, fails. The pointer is then set to `nullptr`. This makes it possible to handle objects of different types differently at run-time, based on the type:

::: pre
``` {.cpp .prettyprint translate="no"}
    if (QLabel *label = qobject_cast<QLabel *>(obj)) {
        label->setText(tr("Ping"));
    } else if (QPushButton *button = qobject_cast<QPushButton *>(obj)) {
        button->setText(tr("Pong!"));
    }
```

While it is possible to use [QObject](qobject.html) as a base class without the [Q_OBJECT](qobject.html#Q_OBJECT) macro and without meta-object code, neither signals and slots nor the other features described here will be available if the [Q_OBJECT](qobject.html#Q_OBJECT) macro is not used. From the meta-object system\'s point of view, a [QObject](qobject.html) subclass without meta code is equivalent to its closest ancestor with meta-object code. This means for example, that [QMetaObject::className](qmetaobject.html#className)() will not return the actual name of your class, but the class name of this ancestor.

Therefore, we strongly recommend that all subclasses of [QObject](qobject.html) use the [Q_OBJECT](qobject.html#Q_OBJECT) macro regardless of whether or not they actually use signals, slots, and properties.

**See also** [QMetaObject](qmetaobject.html), [Qt\'s Property System](properties.html), and [Signals and Slots](signalsandslots.html).

[The Property System](properties.html){.nextPage}

[©]{.abbr title="Copyright"} 2026 The Qt Company Ltd. Documentation contributions included herein are the copyrights of their respective owners. The documentation provided herein is licensed under the terms of the [GNU Free Documentation License version 1.3](http://www.gnu.org/licenses/fdl.html) as published by the Free Software Foundation. Qt and respective logos are [trademarks](https://doc.qt.io/qt/trademarks.html) of The Qt Company Ltd. in Finland and/or other countries worldwide. All other trademarks are property of their respective owners.
