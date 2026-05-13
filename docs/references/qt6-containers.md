---
source: https://doc.qt.io/qt-6/containers.html
fetched: 2026-05-12
---

# Container Classes

## Introduction

The Qt library provides a set of general purpose template-based container classes. These classes can be used to store items of a specified type. For example, if you need a resizable array of [QString](qstring.html)s, use [QList](qlist.html)\<[QString](qstring.html)\>.

These container classes are designed to be lighter, safer, and easier to use than the STL containers. If you are unfamiliar with the STL, or prefer to do things the \"Qt way\", you can use these classes instead of the STL classes.

The container classes are [implicitly shared](implicit-sharing.html), they are [reentrant](threads-reentrancy.html), and they are optimized for speed, low memory consumption, and minimal inline code expansion, resulting in smaller executables. In addition, they are [thread-safe](threads-reentrancy.html) in situations where they are used as read-only containers by all threads used to access them.

The containers provide iterators for traversal. [STL-style iterators](containers.html#stl-style-iterators) are the most efficient ones and can be used together with Qt\'s and STL\'s [generic algorithms](qtalgorithms.html). [Java-style Iterators](java-style-iterators.html#java-style-iterators) are provided for backwards compatibility.

**Note:** Since Qt 5.14, range constructors are available for most of the container classes. [QMultiMap](qmultimap.html) is a notable exception. Their use is encouraged to replace of the various deprecated from/to methods of Qt 5. For example:

::: pre
``` {.cpp .prettyprint translate="no"}
QList<int> list = {1, 2, 3, 4, 4, 5};
QSet<int> set(list.cbegin(), list.cend());
/*
    Will generate a QSet containing 1, 2, 3, 4, 5.
*/
```

## The Container Classes

Qt provides the following sequential containers: [QList](qlist.html), [QStack](qstack.html), and [QQueue](qqueue.html). For most applications, [QList](qlist.html) is the best type to use. It provides very fast appends. If you really need a linked-list, use std::list. [QStack](qstack.html) and [QQueue](qqueue.html) are convenience classes that provide LIFO and FIFO semantics.

Qt also provides these associative containers: [QMap](qmap.html), [QMultiMap](qmultimap.html), [QHash](qhash.html#the-hashing-function), [QMultiHash](qmultihash.html), and [QSet](qset.html). The \"Multi\" containers conveniently support multiple values associated with a single key. The \"Hash\" containers provide faster lookup by using a hash function instead of a binary search on a sorted set.

As special cases, the [QCache](qcache.html) and [QContiguousCache](qcontiguouscache.html) classes provide efficient hash-lookup of objects in a limited cache storage.

::: table
  Class                                                                    Summary
  ------------------------------------------------------------------------ -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  [QList](qlist.html)\<T\>                                 This is by far the most commonly used container class. It stores a list of values of a given type (T) that can be accessed by index. Internally, it stores an array of values of a given type at adjacent positions in memory. Inserting at the front or in the middle of a list can be quite slow, because it can lead to large numbers of items having to be moved by one position in memory.
  [QVarLengthArray](qvarlengtharray.html)\<T, Prealloc\>   This provides a low-level variable-length array. It can be used instead of [QList](qlist.html) in places where speed is particularly important.
  [QStack](qstack.html)\<T\>                               This is a convenience subclass of [QList](qlist.html) that provides \"last in, first out\" (LIFO) semantics. It adds the following functions to those already present in [QList](qlist.html): [push](qstack.html#push)(), [pop](qstack.html#pop)(), and [top](qstack.html#top)().
  [QQueue](qqueue.html)\<T\>                               This is a convenience subclass of [QList](qlist.html) that provides \"first in, first out\" (FIFO) semantics. It adds the following functions to those already present in [QList](qlist.html): [enqueue](qqueue.html#enqueue)(), [dequeue](qqueue.html#dequeue)(), and [head](qqueue.html#head)().
  [QSet](qset.html)\<T\>                                   This provides a single-valued mathematical set with fast lookups.
  [QMap](qmap.html)\<Key, T\>                              This provides a dictionary (associative array) that maps keys of type Key to values of type T. Normally each key is associated with a single value. [QMap](qmap.html) stores its data in Key order; if order doesn\'t matter [QHash](qhash.html#the-hashing-function) is a faster alternative.
  [QMultiMap](qmultimap.html)\<Key, T\>                    This provides a dictionary, like [QMap](qmap.html), except it allows inserting multiple equivalent keys.
  [QHash](qhash.html#the-hashing-function)\<Key, T\>       This has almost the same API as [QMap](qmap.html), but provides significantly faster lookups. [QHash](qhash.html#the-hashing-function) stores its data in an arbitrary order.
  [QMultiHash](qmultihash.html)\<Key, T\>                  This provides a hash-table-based dictionary, like [QHash](qhash.html#the-hashing-function), except it allows inserting multiple equivalent keys.

Containers can be nested. For example, it is perfectly possible to use a [QMap](qmap.html)\<[QString](qstring.html), [QList](qlist.html)\<int\>\>, where the key type is [QString](qstring.html) and the value type [QList](qlist.html)\<int\>.

The containers are defined in individual header files with the same name as the container (e.g., `<QList>`). For convenience, the containers are forward declared in `<QtContainerFwd>`.

[][]

The values stored in the various containers can be of any *assignable data type*. To qualify, a type must provide a copy constructor, and an assignment operator. For some operations a default constructor is also required. This covers most data types you are likely to want to store in a container, including basic types such as `int` and `double`, pointer types, and Qt data types such as [QString](qstring.html), [QDate](qdate.html), and [QTime](qtime.html), but it doesn\'t cover [QObject](qobject.html) or any [QObject](qobject.html) subclass ([QWidget](qwidget.html), [QDialog](qdialog.html), [QTimer](qtimer.html), etc.). If you attempt to instantiate a [QList](qlist.html)\<[QWidget](qwidget.html)\>, the compiler will complain that [QWidget](qwidget.html)\'s copy constructor and assignment operators are disabled. If you want to store these kinds of objects in a container, store them as pointers, for example as [QList](qlist.html)\<[QWidget](qwidget.html) \*\>.

Here\'s an example custom data type that meets the requirement of an assignable data type:

::: pre
``` {.cpp .prettyprint translate="no"}
class Employee

    Employee(const Employee &other);

    Employee &operator=(const Employee &other);

private:
    QString myName;
    QDate myDateOfBirth;
};
```

If we don\'t provide a copy constructor or an assignment operator, C++ provides a default implementation that performs a member-by-member copy. In the example above, that would have been sufficient. Also, if you don\'t provide any constructors, C++ provides a default constructor that initializes its member using default constructors. Although it doesn\'t provide any explicit constructors or assignment operator, the following data type can be stored in a container:

::: pre
``` {.cpp .prettyprint translate="no"}
struct Movie
{
    int id;
    QString title;
    QDate releaseDate;
};
```

Some containers have additional requirements for the data types they can store. For example, the Key type of a [QMap](qmap.html)\<Key, T\> must provide `operator<()`. Such special requirements are documented in a class\'s detailed description. In some cases, specific functions have special requirements; these are described on a per-function basis. The compiler will always emit an error if a requirement isn\'t met.

Qt\'s containers provide operator\<\<() and operator\>\>() so that they can easily be read and written using a [QDataStream](qdatastream.html). This means that the data types stored in the container must also support operator\<\<() and operator\>\>(). Providing such support is straightforward; here\'s how we could do it for the Movie struct above:

::: pre
``` {.cpp .prettyprint translate="no"}
QDataStream &operator<<(QDataStream &out, const Movie &movie)

QDataStream &operator>>(QDataStream &in, Movie &movie)

```

[]

The documentation of certain container class functions refer to *default-constructed values*; for example, [QList](qlist.html) automatically initializes its items with default-constructed values, and [QMap::value](qmap.html#value)() returns a default-constructed value if the specified key isn\'t in the map. For most value types, this simply means that a value is created using the default constructor (e.g. an empty string for [QString](qstring.html)). But for primitive types like `int` and `double`, as well as for pointer types, the C++ language doesn\'t specify any initialization; in those cases, Qt\'s containers automatically initialize the value to 0.

## Iterating over Containers

### Range-based for

Range-based `for` should preferably be used for containers:

::: pre
``` {.cpp .prettyprint translate="no"}
QList<QString> list = {"A", "B", "C", "D"};
for (const auto &item : list) {
//...
}
```

Note that when using a Qt container in a non-const context, [implicit sharing](implicit-sharing.html) may perform an undesired detach of the container. To prevent this, use `std::as_const()`:

::: pre
``` {.cpp .prettyprint translate="no"}
QList<QString> list = {"A", "B", "C", "D"};
for (const auto &item : std::as_const(list)) {
    //...
}
```

For associative containers, this will loop over the values.

### Index-based

For sequential containers that store their items contiguously in memory (for example, [QList](qlist.html)), index-based iteration can be used:

::: pre
``` {.cpp .prettyprint translate="no"}
QList<QString> list = {"A", "B", "C", "D"};
for (qsizetype i = 0; i < list.size(); ++i) {
    const auto &item = list.at(i);
    //...
}
```

### The Iterator Classes

Iterators provide a uniform means to access items in a container. Qt\'s container classes provide two types of iterators: STL-style iterators and Java-style iterators. Iterators of both types are invalidated when the data in the container is modified or detached from [implicitly shared copies](implicit-sharing.html) due to a call to a non-const member function.

#### STL-Style Iterators

STL-style iterators have been available since the release of Qt 2.0. They are compatible with Qt\'s and STL\'s [generic algorithms](qtalgorithms.html) and are optimized for speed.

For each container class, there are two STL-style iterator types: one that provides read-only access and one that provides read-write access. Read-only iterators should be used wherever possible because they are faster than read-write iterators.

::: table
  Containers                                                                                                                         Read-only iterator                                                                   Read-write iterator
  ---------------------------------------------------------------------------------------------------------------------------------- ------------------------------------------------------------------------------------ ------------------------------------------------------------------------------
  [QList](qlist.html)\<T\>, [QStack](qstack.html)\<T\>, [QQueue](qqueue.html)\<T\>   [QList](qlist.html)\<T\>::const_iterator                             [QList](qlist.html)\<T\>::iterator
  [QSet](qset.html)\<T\>                                                                                             [QSet](qset.html)\<T\>::const_iterator                               [QSet](qset.html)\<T\>::iterator
  [QMap](qmap.html)\<Key, T\>, [QMultiMap](qmultimap.html)\<Key, T\>                                 [QMap](qmap.html)\<Key, T\>::const_iterator                          [QMap](qmap.html)\<Key, T\>::iterator
  [QHash](qhash.html#the-hashing-function)\<Key, T\>, [QMultiHash](qmultihash.html)\<Key, T\>        [QHash](qhash.html#the-hashing-function)\<Key, T\>::const_iterator   [QHash](qhash.html#the-hashing-function)\<Key, T\>::iterator

The API of the STL iterators is modelled on pointers in an array. For example, the `++` operator advances the iterator to the next item, and the `*` operator returns the item that the iterator points to. In fact, for [QList](qlist.html) and [QStack](qstack.html), which store their items at adjacent memory positions, the [iterator](qlist-iterator.html) type is just a typedef for `T *`, and the [const_iterator](qlist-iterator.html) type is just a typedef for `const T *`.

In this discussion, we will concentrate on [QList](qlist.html) and [QMap](qmap.html). The iterator types for [QSet](qset.html) have exactly the same interface as [QList](qlist.html)\'s iterators; similarly, the iterator types for [QHash](qhash.html#the-hashing-function) have the same interface as [QMap](qmap.html)\'s iterators.

Here\'s a typical loop for iterating through all the elements of a [QList](qlist.html)\<[QString](qstring.html)\> in order and converting them to lowercase:

::: pre
``` {.cpp .prettyprint translate="no"}
QList<QString> list = {"A", "B", "C", "D"};

for (auto i = list.begin(), end = list.end(); i != end; ++i)
    *i = (*i).toLower();
```

STL-style iterators point directly at items. The [begin](qlist.html#begin)() function of a container returns an iterator that points to the first item in the container. The [end](qlist.html#end)() function of a container returns an iterator to the imaginary item one position past the last item in the container. [end](qlist.html#end)() marks an invalid position; it must never be dereferenced. It is typically used in a loop\'s break condition. If the list is empty, [begin](qlist.html#begin)() equals [end](qlist.html#end)(), so we never execute the loop.

The diagram below shows the valid iterator positions as red arrows for a list containing four items:

![](images/stliterators1.png)

Iterating backward with an STL-style iterator is done with reverse iterators:

::: pre
``` {.cpp .prettyprint translate="no"}
QList<QString> list = {"A", "B", "C", "D"};

for (auto i = list.rbegin(), rend = list.rend(); i != rend; ++i)
    *i = i->toLower();
```

In the code snippets so far, we used the unary `*` operator to retrieve the item (of type [QString](qstring.html)) stored at a certain iterator position, and we then called [QString::toLower](qstring.html#toLower)() on it.

For read-only access, you can use const_iterator, [cbegin](qlist.html#cbegin)(), and [cend](qlist.html#cend)(). For example:

::: pre
``` {.cpp .prettyprint translate="no"}
for (auto i = list.cbegin(), end = list.cend(); i != end; ++i)
    qDebug() << *i;
```

The following table summarizes the STL-style iterators\' API:

::: table
  Expression                 Behavior
  -------------------------- -------------------------------------------------------------------------------------------
  `*i`       Returns the current item
  `++i`      Advances the iterator to the next item
  `i += n`   Advances the iterator by `n` items
  `--i`      Moves the iterator back by one item
  `i -= n`   Moves the iterator back by `n` items
  `i - j`    Returns the number of items between iterators `i` and `j`

The `++` and `--` operators are available both as prefix (`++i`, `--i`) and postfix (`i++`, `i--`) operators. The prefix versions modify the iterators and return a reference to the modified iterator; the postfix versions take a copy of the iterator before they modify it, and return that copy. In expressions where the return value is ignored, we recommend that you use the prefix operators (`++i`, `--i`), as these are slightly faster.

For non-const iterator types, the return value of the unary `*` operator can be used on the left side of the assignment operator.

For [QMap](qmap.html) and [QHash](qhash.html#the-hashing-function), the `*` operator returns the value component of an item. If you want to retrieve the key, call key() on the iterator. For symmetry, the iterator types also provide a value() function to retrieve the value. For example, here\'s how we would print all items in a [QMap](qmap.html) to the console:

::: pre
``` {.cpp .prettyprint translate="no"}
QMap<int, int> map;
//...
for (auto i = map.cbegin(), end = map.cend(); i != end; ++i)
    qDebug() << i.key() << ':' << i.value();
```

Thanks to [implicit sharing](implicit-sharing.html), it is very inexpensive for a function to return a container per value. The Qt API contains dozens of functions that return a [QList](qlist.html) or [QStringList](qstringlist.html) per value (e.g., [QSplitter::sizes](qsplitter.html#sizes)()). If you want to iterate over these using an STL iterator, you should always take a copy of the container and iterate over the copy. For example:

::: pre
``` {.cpp .prettyprint translate="no"}
// RIGHT
const QList<int> sizes = splitter->sizes();
for (auto i = sizes.begin(), end = sizes.end(); i != end; ++i)

// WRONG
for (auto i = splitter->sizes().begin();
        i != splitter->sizes().end(); ++i)

```

This problem doesn\'t occur with functions that return a const or non-const reference to a container.

##### Implicit sharing iterator problem

[Implicit sharing](implicit-sharing.html) has another consequence on STL-style iterators: you should avoid copying a container while iterators are active on that container. The iterators point to an internal structure, and if you copy a container you should be very careful with your iterators. E.g:

::: pre
``` {.cpp .prettyprint translate="no"}
QList<int> a, b;
a.resize(100000); // make a big list filled with 0.

QList<int>::iterator i = a.begin();
// WRONG way of using the iterator i:
b = a;
/*
    Now we should be careful with iterator i since it will point to shared data
    If we do *i = 4 then we would change the shared instance (both vectors)
    The behavior differs from STL containers. Avoid doing such things in Qt.
*/

a[0] = 5;
/*
    Container a is now detached from the shared data,
    and even though i was an iterator from the container a, it now works as an iterator in b.
    Here the situation is that (*i) == 0.
*/

b.clear(); // Now the iterator i is completely invalid.

int j = *i; // Undefined behavior!
/*
    The data from b (which i pointed to) is gone.
    This would be well-defined with STL containers (and (*i) == 5),
    but with QList this is likely to crash.
*/
```

The above example only shows a problem with [QList](qlist.html), but the problem exists for all the implicitly shared Qt containers.

#### Java-Style Iterators

[Java-Style iterators](java-style-iterators.html#java-style-iterators) are modelled on Java\'s iterator classes. New code should prefer [STL-Style Iterators](containers.html#stl-style-iterators).

## Qt containers compared with std containers

::: table
+------------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| Qt container                                                           | Closest std container                                                                                                                                                                                                                                                      |
+========================================================================+============================================================================================================================================================================================================================================================================+
| [QList](qlist.html)\<T\>                               | Similar to std::vector\<T\>                                                                                                                                                                                                                                                |
|                                                                        |                                                                                                                                                                                                                                                                            |
|                                                                        | [QList](qlist.html) and [QVector](qvector.html) were unified in Qt 6. Both use the datamodel from [QVector](qvector.html). [QVector](qvector.html) is now an alias to [QList](qlist.html). |
|                                                                        |                                                                                                                                                                                                                                                                            |
|                                                                        | This means that [QList](qlist.html) is not implemented as a linked list, so if you need constant time insert, delete, append or prepend, consider `std::list<T>`. See [QList](qlist.html) for details.                     |
+------------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| [QVarLengthArray](qvarlengtharray.html)\<T, Prealloc\> | Resembles a mix of std::array\<T\> and std::vector\<T\>.                                                                                                                                                                                                                   |
|                                                                        |                                                                                                                                                                                                                                                                            |
|                                                                        | For performance reasons, [QVarLengthArray](qvarlengtharray.html) lives on the stack unless resized. Resizing it automatically causes it to use the heap instead.                                                                                           |
+------------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| [QStack](qstack.html)\<T\>                             | Similar to std::stack\<T\>, inherits from [QList](qlist.html).                                                                                                                                                                                             |
+------------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| [QQueue](qqueue.html)\<T\>                             | Similar to std::queue\<T\>, inherits from [QList](qlist.html).                                                                                                                                                                                             |
+------------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| [QSet](qset.html)\<T\>                                 | Similar to std::unordered_set\<T\>. Internally, [QSet](qset.html) is implemented with a [QHash](qhash.html#the-hashing-function).                                                                                                          |
+------------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| [QMap](qmap.html)\<Key, T\>                            | Similar to std::map\<Key, T\>.                                                                                                                                                                                                                                             |
+------------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| [QMultiMap](qmultimap.html)\<Key, T\>                  | Similar to std::multimap\<Key, T\>.                                                                                                                                                                                                                                        |
+------------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| [QHash](qhash.html#the-hashing-function)\<Key, T\>     | Most similar to std::unordered_map\<Key, T\>.                                                                                                                                                                                                                              |
+------------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| [QMultiHash](qmultihash.html)\<Key, T\>                | Most similar to std::unordered_multimap\<Key, T\>.                                                                                                                                                                                                                         |
+------------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+

## Qt containers and std algorithms

You can use Qt containers with functions from `#include <algorithm>`.

::: pre
``` {.cpp .prettyprint translate="no"}
QList<int> list = {2, 3, 1};

std::sort(list.begin(), list.end());
/*
    Sort the list, now contains { 1, 2, 3 }
*/

std::reverse(list.begin(), list.end());
/*
    Reverse the list, now contains { 3, 2, 1 }
*/

int even_elements =
        std::count_if(list.begin(), list.end(), [](int element) { return (element % 2 == 0); });
/*
    Count how many elements that are even numbers, 1
*/
```

## Other Container-Like Classes

Qt includes other template classes that resemble containers in some respects. These classes don\'t provide iterators and cannot be used with the [foreach](qforeach.html#foreach) keyword.

- [QCache](qcache.html)\<Key, T\> provides a cache to store objects of a certain type T associated with keys of type Key.
- [QContiguousCache](qcontiguouscache.html)\<T\> provides an efficient way of caching data that is typically accessed in a contiguous way.

Additional non-template types that compete with Qt\'s template containers are [QBitArray](qbitarray.html), [QByteArray](qbytearray.html), [QString](qstring.html), and [QStringList](qstringlist.html).

## Algorithmic Complexity

Algorithmic complexity is concerned about how fast (or slow) each function is as the number of items in the container grow. For example, inserting an item in the middle of a std::list is an extremely fast operation, irrespective of the number of items stored in the list. On the other hand, inserting an item in the middle of a [QList](qlist.html) is potentially very expensive if the [QList](qlist.html) contains many items, since half of the items must be moved one position in memory.

To describe algorithmic complexity, we use the following terminology, based on the \"big Oh\" notation:

[][][][][]

- **Constant time:** O(1). A function is said to run in constant time if it requires the same amount of time no matter how many items are present in the container. One example is [QList::push_back](qlist.html#push_back)().
- **Logarithmic time:** O(log *n*). A function that runs in logarithmic time is a function whose running time is proportional to the logarithm of the number of items in the container. One example is the binary search algorithm.
- **Linear time:** O(*n*). A function that runs in linear time will execute in a time directly proportional to the number of items stored in the container. One example is [QList::insert](qlist.html#insert)().
- **Linear-logarithmic time:** O(*n* log *n*). A function that runs in linear-logarithmic time is asymptotically slower than a linear-time function, but faster than a quadratic-time function.
- **Quadratic time:** O(*n*²). A quadratic-time function executes in a time that is proportional to the square of the number of items stored in the container.

The following table summarizes the algorithmic complexity of the sequential container [QList](qlist.html)\<T\>:

::: table
                                             Index lookup   Insertion   Prepending   Appending
  ------------------------------------------ -------------- ----------- ------------ -------------
  [QList](qlist.html)\<T\>   O(1)           O(n)        O(n)         Amort. O(1)

In the table, \"Amort.\" stands for \"amortized behavior\". For example, \"Amort. O(1)\" means that if you call the function only once, you might get O(*n*) behavior, but if you call it multiple times (e.g., *n* times), the average behavior will be O(1).

The following table summarizes the algorithmic complexity of Qt\'s associative containers and sets:

::: table
+--------------------------------------------------------------------+--------------------------+--------------------------+
|                                                                    | Key lookup               | Insertion                |
|                                                                    +-------------+------------+-------------+------------+
|                                                                    | Average     | Worst case | Average     | Worst case |
+====================================================================+=============+============+=============+============+
| [QMap](qmap.html)\<Key, T\>                        | O(log *n*)  | O(log *n*) | O(log *n*)  | O(log *n*) |
+--------------------------------------------------------------------+-------------+------------+-------------+------------+
| [QMultiMap](qmultimap.html)\<Key, T\>              | O(log *n*)  | O(log *n*) | O(log *n*)  | O(log *n*) |
+--------------------------------------------------------------------+-------------+------------+-------------+------------+
| [QHash](qhash.html#the-hashing-function)\<Key, T\> | Amort. O(1) | O(*n*)     | Amort. O(1) | O(*n*)     |
+--------------------------------------------------------------------+-------------+------------+-------------+------------+
| [QSet](qset.html)\<Key\>                           | Amort. O(1) | O(*n*)     | Amort. O(1) | O(*n*)     |
+--------------------------------------------------------------------+-------------+------------+-------------+------------+

With [QList](qlist.html), [QHash](qhash.html#the-hashing-function), and [QSet](qset.html), the performance of appending items is amortized O(log *n*). It can be brought down to O(1) by calling [QList::reserve](qlist.html#reserve)(), [QHash::reserve](qhash.html#reserve)(), or [QSet::reserve](qset.html#reserve)() with the expected number of items before you insert the items. The next section discusses this topic in more depth.

## Optimizations for Primitive and Relocatable Types

Qt containers can use optimized code paths if the stored elements are relocatable or even primitive. However, whether types are primitive or relocatable cannot be detected in all cases. You can declare your types to be primitive or relocatable by using the [Q_DECLARE_TYPEINFO](qtypeinfo.html#Q_DECLARE_TYPEINFO) macro with the Q_PRIMITIVE_TYPE flag or the Q_RELOCATABLE_TYPE flag. See the documentation of [Q_DECLARE_TYPEINFO](qtypeinfo.html#Q_DECLARE_TYPEINFO) for further details and usage examples.

If you do not use [Q_DECLARE_TYPEINFO](qtypeinfo.html#Q_DECLARE_TYPEINFO), Qt will use [std::is_trivial_v\<T\>](https://en.cppreference.com/w/cpp/types/is_trivial) to identify primitive types and it will require both [std::is_trivially_copyable_v\<T\>](https://en.cppreference.com/w/cpp/types/is_trivially_copyable) and [std::is_trivially_destructible_v\<T\>](https://en.cppreference.com/w/cpp/types/is_destructible) to identify relocatable types. This is always a safe choice, albeit of maybe suboptimal performance.

## Growth Strategies

[QList](qlist.html)\<T\>, [QString](qstring.html), and [QByteArray](qbytearray.html) store their items contiguously in memory; [QHash](qhash.html#the-hashing-function)\<Key, T\> keeps a hash table whose size is proportional to the number of items in the hash. To avoid reallocating the data every single time an item is added at the end of the container, these classes typically allocate more memory than necessary.

Consider the following code, which builds a [QString](qstring.html) from another [QString](qstring.html):

::: pre
``` {.cpp .prettyprint translate="no"}
QString onlyLetters(const QString &in)

    return out;
}
```

We build the string `out` dynamically by appending one character to it at a time. Let\'s assume that we append 15000 characters to the [QString](qstring.html) string. Then the following 11 reallocations (out of a possible 15000) occur when [QString](qstring.html) runs out of space: 8, 24, 56, 120, 248, 504, 1016, 2040, 4088, 8184, 16376. At the end, the [QString](qstring.html) has 16376 Unicode characters allocated, 15000 of which are occupied.

The values above may seem a bit strange, but there is a guiding principle. It advances by doubling the size each time. More precisely, it advances to the next power of two, minus 16 bytes. 16 bytes corresponds to eight characters, as [QString](qstring.html) uses UTF-16 internally.

[QByteArray](qbytearray.html) uses the same algorithm as [QString](qstring.html), but 16 bytes correspond to 16 characters.

[QList](qlist.html)\<T\> also uses that algorithm, but 16 bytes correspond to 16/sizeof(T) elements.

[QHash](qhash.html#the-hashing-function)\<Key, T\> is a totally different case. [QHash](qhash.html#the-hashing-function)\'s internal hash table grows by powers of two, and each time it grows, the items are relocated in a new bucket, computed as [qHash](qhash.html#the-hashing-function)(*key*) % [QHash::capacity](qhash.html#capacity)() (the number of buckets). This remark applies to [QSet](qset.html)\<T\> and [QCache](qcache.html)\<Key, T\> as well.

For most applications, the default growing algorithm provided by Qt does the trick. If you need more control, [QList](qlist.html)\<T\>, [QHash](qhash.html#the-hashing-function)\<Key, T\>, [QSet](qset.html)\<T\>, [QString](qstring.html), and [QByteArray](qbytearray.html) provide a trio of functions that allow you to check and specify how much memory to use to store the items:

- [capacity](qstring.html#capacity)() returns the number of items for which memory is allocated (for [QHash](qhash.html#the-hashing-function) and [QSet](qset.html), the number of buckets in the hash table).
- [reserve](qstring.html#reserve)(*size*) explicitly preallocates memory for *size* items.
- [squeeze](qstring.html#squeeze)() frees any memory not required to store the items.

If you know approximately how many items you will store in a container, you can start by calling [reserve](qstring.html#reserve)(), and when you are done populating the container, you can call [squeeze](qstring.html#squeeze)() to release the extra preallocated memory.

[Serializing Qt Data Types](datastreamformat.html){.prevPage} [C++20 Overview](cpp20-overview.html){.nextPage}

[©]{.abbr title="Copyright"} 2026 The Qt Company Ltd. Documentation contributions included herein are the copyrights of their respective owners. The documentation provided herein is licensed under the terms of the [GNU Free Documentation License version 1.3](http://www.gnu.org/licenses/fdl.html) as published by the Free Software Foundation. Qt and respective logos are [trademarks](https://doc.qt.io/qt/trademarks.html) of The Qt Company Ltd. in Finland and/or other countries worldwide. All other trademarks are property of their respective owners.
