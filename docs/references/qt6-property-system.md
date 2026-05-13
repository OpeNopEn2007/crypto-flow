---
source: https://doc.qt.io/qt-6/properties.html
fetched: 2026-05-12
---

# The Property System

Qt provides a sophisticated property system similar to the ones supplied by some compiler vendors. However, as a compiler- and platform-independent library, Qt does not rely on non-standard compiler features like `__property` or `[property]`. The Qt solution works with *any* standard C++ compiler on every platform Qt supports. It is based on the [Meta-Object System](metaobjects.html) that also provides inter-object communication via [signals and slots](signalsandslots.html).

## Requirements for Declaring Properties

To declare a property, use the [Q_PROPERTY](qobject.html#Q_PROPERTY)() macro in a class that inherits [QObject](qobject.html).

::: pre
``` {.cpp .prettyprint translate="no"}
Q_PROPERTY(type name
           (READ getFunction [WRITE setFunction] |
            MEMBER memberName [(READ getFunction | WRITE setFunction)])
           [RESET resetFunction]
           [NOTIFY notifySignal]
           [REVISION int | REVISION(int[, int])]
           [DESIGNABLE bool]
           [SCRIPTABLE bool]
           [STORED bool]
           [USER bool]
           [BINDABLE bindableProperty]
           [CONSTANT]
           [FINAL]
           [VIRTUAL]
           [OVERRIDE]
           [REQUIRED])
```

Here are some typical examples of property declarations taken from class [QWidget](qwidget.html).

::: pre
``` {.cpp translate="no"}
Q_PROPERTY(bool focus READ hasFocus)
Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled)
Q_PROPERTY(QCursor cursor READ cursor WRITE setCursor RESET unsetCursor)
```

Here is an example showing how to export member variables as Qt properties using the `MEMBER` keyword. Note that a `NOTIFY` signal must be specified to allow QML property bindings.

::: pre
``` {.cpp .prettyprint translate="no"}
    Q_PROPERTY(QColor color MEMBER m_color NOTIFY colorChanged)
    Q_PROPERTY(qreal spacing MEMBER m_spacing NOTIFY spacingChanged)
    Q_PROPERTY(QString text MEMBER m_text NOTIFY textChanged)
    //...
signals:
    void colorChanged();
    void spacingChanged();
    void textChanged(const QString &newText);

private:
    QColor  m_color;
    qreal   m_spacing;
    QString m_text;
```

A property behaves like a class data member, but it has additional features accessible through the [Meta-Object System](metaobjects.html).

- A `READ` accessor function is required if no `MEMBER` variable was specified. It is for reading the property value. Ideally, a const function is used for this purpose, and it must return either the property\'s type or a const reference to that type. e.g., [QWidget::focus](qwidget.html#focus-prop) is a read-only property with `READ` function, [QWidget::hasFocus](qwidget.html#focus-prop)(). If a `BINDABLE` is specified, you can write `READ default` to have the `READ` accessor generated from the `BINDABLE`.
- A `WRITE` accessor function is optional. It is for setting the property value. It must return void and must take exactly one argument, either of the property\'s type or a pointer or reference to that type. e.g., [QWidget::enabled](qwidget.html#enabled-prop) has the `WRITE` function [QWidget::setEnabled](qwidget.html#enabled-prop)(). Read-only properties do not need `WRITE` functions. e.g., [QWidget::focus](qwidget.html#focus-prop) has no `WRITE` function. If you specify both a `BINDABLE` and `WRITE default`, a `WRITE` accessor will be generated from the `BINDABLE`. The generated `WRITE` accessor will *not* explicitly emit any signal declared with `NOTIFY`. You should register the signal as change handler to the `BINDABLE`, for example using [Q_OBJECT_BINDABLE_PROPERTY](qobjectbindableproperty.html#Q_OBJECT_BINDABLE_PROPERTY).
- A `MEMBER` variable association is required if no `READ` accessor function is specified. This makes the given member variable readable and writable without the need of creating `READ` and `WRITE` accessor functions. It\'s still possible to use `READ` or `WRITE` accessor functions in addition to `MEMBER` variable association (but not both), if you need to control the variable access.
- A `RESET` function is optional. It is for setting the property back to its context specific default value. e.g., [QWidget::cursor](qwidget.html#cursor-prop) has the typical `READ` and `WRITE` functions, [QWidget::cursor](qwidget.html#cursor-prop)() and [QWidget::setCursor](qwidget.html#cursor-prop)(), and it also has a `RESET` function, [QWidget::unsetCursor](qwidget.html#cursor-prop)(), since no call to [QWidget::setCursor](qwidget.html#cursor-prop)() can mean *reset to the context specific cursor*. The `RESET` function must return void and take no parameters.
- A `NOTIFY` signal is optional. If defined, it should specify one existing signal in that class that is emitted whenever the value of the property changes. `NOTIFY` signals for `MEMBER` variables must take zero or one parameter, which must be of the same type as the property. The parameter will take the new value of the property. The `NOTIFY` signal should only be emitted when the property has really been changed, to avoid bindings being unnecessarily re-evaluated in QML, for example. The signal is emitted automatically when the property is changed via the Qt API ([QObject::setProperty](qobject.html#setProperty), [QMetaProperty](qmetaproperty.html), etc.), but not when the MEMBER is changed directly.
- A `REVISION` number or `REVISION()` macro is optional. If included, it defines the property and its notifier signal to be used in a particular revision of the API (usually for exposure to QML). If not included, it defaults to 0.
- The `DESIGNABLE` attribute indicates whether the property should be visible in the property editor of GUI design tool (e.g., [Qt Widgets Designer](qtdesigner-manual.html)). Most properties are `DESIGNABLE` (default true). Valid values are true and false.
- The `SCRIPTABLE` attribute indicates whether this property should be accessible by a scripting engine (default true). Valid values are true and false.
- The `STORED` attribute indicates whether the property should be thought of as existing on its own or as depending on other values. It also indicates whether the property value must be saved when storing the object\'s state. Most properties are `STORED` (default true), but e.g., [QWidget::minimumWidth](qwidget.html#minimumWidth-prop)() has `STORED` false, because its value is just taken from the width component of property [QWidget::minimumSize](qwidget.html#minimumSize-prop)(), which is a [QSize](qsize.html).
- The `USER` attribute indicates whether the property is designated as the user-facing or user-editable property for the class. Normally, there is only one `USER` property per class (default false). e.g., [QAbstractButton::checked](qabstractbutton.html#checked-prop) is the user editable property for (checkable) buttons. Note that [QItemDelegate](qitemdelegate.html) gets and sets a widget\'s `USER` property.
- The `BINDABLE bindableProperty` attribute indicates that the property supports [bindings](bindableproperties.html), and that it is possible to set and inspect bindings to this property via the meta object system ([QMetaProperty](qmetaproperty.html)). `bindableProperty` names a class member of type [QBindable](qbindable.html)\<T\>, where T is the property type. This attribute was introduced in Qt 6.0.
- The presence of the `CONSTANT` attribute indicates that the property value is constant. For a given object instance, the READ method of a constant property must return the same value every time it is called. This constant value may be different for different instances of the object. A constant property cannot have a WRITE method or a NOTIFY signal.
- `FINAL`, `VIRTUAL`, `OVERRIDE` modifiers mirror the semantics of their C++ and [QML counterparts](qtqml-syntax-objectattributes.html#override-semantics), allowing to make property overriding explicit at the meta-object level.

  ::: {.admonition .note}
  **Note:** At present, these modifiers are not enforced by moc. They are recognized syntactically and are primarily used for QML runtime enforcement and tooling diagnostics. Future versions may introduce stricter compile-time validation and warnings for invalid overrides across modules.
  :::

  ::: {.admonition .note}
  **Note:** If you want to change accessing behaviour for a property, use the polymorphism provided by C++.
  :::
- The presence of the `REQUIRED` attribute indicates that the property should be set by a user of the class. This is not enforced by moc, and is mostly useful for classes exposed to QML. In QML, classes with REQUIRED properties cannot be instantiated unless all REQUIRED properties have been set.

The `READ`, `WRITE`, and `RESET` functions can be inherited. They can also be virtual. When they are inherited in classes where multiple inheritance is used, they must come from the first inherited class.

The property type can be any type supported by [QVariant](qvariant.html), or it can be a user-defined type. In this example, class [QDate](qdate.html) is considered to be a user-defined type.

::: pre
``` {.cpp .prettyprint translate="no"}
Q_PROPERTY(QDate date READ getDate WRITE setDate)
```

Because [QDate](qdate.html) is user-defined, you must include the `<QDate>` header file with the property declaration.

For historical reasons, *QMap* and *QList* as property types are synonym of *QVariantMap* and *QVariantList*.

## Reading and Writing Properties with the Meta-Object System

A property can be read and written using the generic functions [QObject::property](qobject.html#property)() and [QObject::setProperty](qobject.html#setProperty)(), without knowing anything about the owning class except the property\'s name. In the code snippet below, the call to [QAbstractButton::setDown](qabstractbutton.html#down-prop)() and the call to [QObject::setProperty](qobject.html#setProperty)() both set property \"down\".

::: pre
``` {.cpp .prettyprint translate="no"}
QPushButton *button = new QPushButton;
QObject *object = button;

button->setDown(true);
object->setProperty("down", true);
```

Accessing a property through its `WRITE` accessor is the better of the two, because it is faster and gives better diagnostics at compile time, but setting the property this way requires that you know about the class at compile time. Accessing properties by name lets you access classes you don\'t know about at compile time. You can *discover* a class\'s properties at run time by querying its [QObject](qobject.html), [QMetaObject](qmetaobject.html), and [QMetaProperties](qmetaproperty.html).

::: pre
``` {.cpp .prettyprint translate="no"}
QObject *object = new QObject;
const QMetaObject *metaobject = object->metaObject();
int count = metaobject->propertyCount();
for (int i=0; i<count; ++i) {
    QMetaProperty metaproperty = metaobject->property(i);
    const char *name = metaproperty.name();
    QVariant value = object->property(name);
    //...
}
```

In the above snippet, [QMetaObject::property](qmetaobject.html#property)() is used to get [metadata](qmetaproperty.html) about each property defined in some unknown class. The property name is fetched from the metadata and passed to [QObject::property](qobject.html#property)() to get the [value](qvariant.html) of the property in the current [object](qobject.html).

## A Simple Example

Suppose we have a class `MyClass`, which is derived from [QObject](qobject.html) and which uses the [Q_OBJECT](qobject.html#Q_OBJECT) macro. We want to declare a property in `MyClass` to keep track of a priority value. The name of the property will be `priority`, and its type will be an enumeration type named `Priority`, which is defined in `MyClass`.

We declare the property with the [Q_PROPERTY](qobject.html#Q_PROPERTY)() macro in the private section of the class. The required `READ` function is named `priority`, and we include a `WRITE` function named `setPriority`. The enumeration type must be registered with the [Meta-Object System](metaobjects.html) using the [Q_ENUM](qobject.html#Q_ENUM)() macro. Registering an enumeration type makes the enumerator names available for use in calls to [QObject::setProperty](qobject.html#setProperty)(). We must also provide our own declarations for the `READ` and `WRITE` functions. The declaration of `MyClass` then might look like this:

::: pre
``` {.cpp .prettyprint translate="no"}
class MyClass : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Priority priority READ priority WRITE setPriority NOTIFY priorityChanged)

public:
    MyClass(QObject *parent = nullptr);
    ~MyClass();

    enum Priority { High, Low, VeryHigh, VeryLow };
    Q_ENUM(Priority)

    void setPriority(Priority priority)

    Priority priority() const

signals:
    void priorityChanged(Priority);

private:
    Priority m_priority;
};
```

The `READ` function is const and returns the property type. The `WRITE` function returns void and has exactly one parameter of the property type. The meta-object compiler enforces these requirements. The equality check in the `WRITE` function, while not mandatory, is good practice as there is no point in notifying and potentially forcing re-evaluation in other places if nothing has changed.

Given a pointer to an instance of `MyClass` or a pointer to a [QObject](qobject.html) that is an instance of `MyClass`, we have two ways to set its priority property:

::: pre
``` {.cpp .prettyprint translate="no"}
MyClass *myinstance = new MyClass;
QObject *object = myinstance;

myinstance->setPriority(MyClass::VeryHigh);
object->setProperty("priority", "VeryHigh");
```

In the example, the enumeration type that is the property type is declared in `MyClass` and registered with the [Meta-Object System](metaobjects.html) using the [Q_ENUM](qobject.html#Q_ENUM)() macro. This makes the enumeration values available as strings for use as in the call to [setProperty](qobject.html#setProperty)(). Had the enumeration type been declared in another class, its fully qualified name (i.e., OtherClass::Priority) would be required, and that other class would also have to inherit [QObject](qobject.html) and register the enumeration type there using the [Q_ENUM](qobject.html#Q_ENUM)() macro.

A similar macro, [Q_FLAG](qobject.html#Q_FLAG)(), is also available. Like [Q_ENUM](qobject.html#Q_ENUM)(), it registers an enumeration type, but it marks the type as being a set of *flags*, i.e., values that can be OR\'d together. An I/O class might have enumeration values `Read` and `Write` and then [QObject::setProperty](qobject.html#setProperty)() could accept `Read | Write`. [Q_FLAG](qobject.html#Q_FLAG)() should be used to register this enumeration type.

## Dynamic Properties

[QObject::setProperty](qobject.html#setProperty)() can also be used to add *new* properties to an instance of a class at runtime. When it is called with a name and a value, if a property with the given name exists in the [QObject](qobject.html), and if the given value is compatible with the property\'s type, the value is stored in the property, and true is returned. If the value is *not* compatible with the property\'s type, the property is *not* changed, and false is returned. But if the property with the given name doesn\'t exist in the [QObject](qobject.html) (i.e., if it wasn\'t declared with [Q_PROPERTY](qobject.html#Q_PROPERTY)()), a new property with the given name and value is automatically added to the [QObject](qobject.html), but false is still returned. This means that a return of false can\'t be used to determine whether a particular property was actually set, unless you know in advance that the property already exists in the [QObject](qobject.html).

Note that *dynamic* properties are added on a per instance basis, i.e., they are added to [QObject](qobject.html), not [QMetaObject](qmetaobject.html). A property can be removed from an instance by passing the property name and an invalid [QVariant](qvariant.html) value to [QObject::setProperty](qobject.html#setProperty)(). The default constructor for [QVariant](qvariant.html) constructs an invalid [QVariant](qvariant.html).

Dynamic properties can be queried with [QObject::property](qobject.html#property)(), just like properties declared at compile time with [Q_PROPERTY](qobject.html#Q_PROPERTY)().

## Properties and Custom Types

Custom types used by properties need to be registered using the [Q_DECLARE_METATYPE](qmetatype.html#Q_DECLARE_METATYPE)() macro so that their values can be stored in [QVariant](qvariant.html) objects. This makes them suitable for use with both static properties declared using the [Q_PROPERTY](qobject.html#Q_PROPERTY)() macro in class definitions and dynamic properties created at run-time.

## Adding Additional Information to a Class

Connected to the property system is an additional macro, [Q_CLASSINFO](qobject.html#Q_CLASSINFO)(), that can be used to attach additional *name*--*value* pairs to a class\'s meta-object. This is used for instance to mark a property as the *default* one in the context of [QML Object Types](qtqml-typesystem-objecttypes.html):

::: pre
``` {.cpp .prettyprint translate="no"}
Q_CLASSINFO("DefaultProperty", "content")
```

Like other meta-data, class information is accessible at run-time through the meta-object; see [QMetaObject::classInfo](qmetaobject.html#classInfo)() for details.

## Using Bindable Properties

Three different types can be used to implement bindable properties:

- [QProperty](qproperty.html)
- [QObjectBindableProperty](qobjectbindableproperty.html)
- [QObjectComputedProperty](qobjectcomputedproperty.html).

The first one is a general class for bindable properties. The latter two can only be used inside a [QObject](qobject.html).

For more information, including examples, see the classes mentioned above and the general tips on implementing and using [bindable properties](bindableproperties.html).

**See also** [Meta-Object System](metaobjects.html), [Signals and Slots](signalsandslots.html), [Q_DECLARE_METATYPE](qmetatype.html#Q_DECLARE_METATYPE)(), [QMetaType](qmetatype.html), [QVariant](qvariant.html), [Qt Bindable Properties](bindableproperties.html), and [Defining QML Types from C++](qtqml-cppintegration-definetypes.html).

[The Meta-Object System](metaobjects.html){.prevPage} [Object Model](object.html){.nextPage}

[©]{.abbr title="Copyright"} 2026 The Qt Company Ltd. Documentation contributions included herein are the copyrights of their respective owners. The documentation provided herein is licensed under the terms of the [GNU Free Documentation License version 1.3](http://www.gnu.org/licenses/fdl.html) as published by the Free Software Foundation. Qt and respective logos are [trademarks](https://doc.qt.io/qt/trademarks.html) of The Qt Company Ltd. in Finland and/or other countries worldwide. All other trademarks are property of their respective owners.
