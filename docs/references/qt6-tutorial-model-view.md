# Qt 6 Model/View Programming

> Source: https://doc.qt.io/qt-6/model-view-programming.html
> Fetched: 2026-05-12

---

## Introduction to Model/View Programming

Qt contains a set of item view classes that use a model/view architecture to manage the relationship between data and the way it is presented to the user. The separation of functionality introduced by this architecture gives developers greater flexibility to customize the presentation of items, and provides a standard model interface to allow a wide range of data sources to be used with existing item views.

## The Model/View Architecture

Model-View-Controller (MVC) is a design pattern originating from Smalltalk that is often used when building user interfaces. In Design Patterns, Gamma et al. write:

> MVC consists of three kinds of objects. The Model is the application object, the View is its screen presentation, and the Controller defines the way the user interface reacts to user input. Before MVC, user interface designs tended to lump these objects together. MVC decouples them to increase flexibility and reuse.

If the view and the controller objects are combined, the result is the **model/view architecture**. This still separates the way that data is stored from the way that it is presented to the user, but provides a simpler framework based on the same principles. This separation makes it possible to display the same data in several different views, and to implement new types of views, without changing the underlying data structures. To allow flexible handling of user input, we introduce the concept of the **delegate**.

### Architecture Diagram

```
+--------+     +-------+     +----------+
|  Data  | --> | Model | --> |   View   | --> Screen
+--------+     +---+---+     +----+-----+
                      |            |
                      v            v
                 +-----------+ +----------+
                 | Delegate  | | Rendering|
                 | (Editing) | |          |
                 +-----------+ +----------+
```

| Component | Role |
|-----------|------|
| **Model** | Communicates with a source of data, providing an interface for the other components |
| **View** | Obtains model indexes from the model; retrieves items of data from the data source |
| **Delegate** | Renders items of data; when an item is edited, communicates with the model directly using model indexes |

Models, views, and delegates communicate with each other using **signals and slots**:

- Signals from the **model** inform the view about changes to the data held by the data source.
- Signals from the **view** provide information about the user's interaction with the items being displayed.
- Signals from the **delegate** are used during editing to tell the model and view about the state of the editor.

## Models

All item models are based on the `QAbstractItemModel` class. This class defines an interface that is used by views and delegates to access data. The data itself does not have to be stored in the model; it can be held in a data structure or repository provided by a separate class, a file, a database, or some other application component.

`QAbstractItemModel` provides an interface to data that is flexible enough to handle views that represent data in the form of tables, lists, and trees. However, when implementing new models for list and table-like data structures, the `QAbstractListModel` and `QAbstractTableModel` classes are better starting points because they provide appropriate default implementations of common functions.

### Built-in Models

| Model | Description |
|-------|-------------|
| `QStringListModel` | Stores a simple list of QString items |
| `QStandardItemModel` | Manages complex tree structures of items, each can contain arbitrary data |
| `QFileSystemModel` | Provides information about files and directories in the local filing system |
| `QSqlQueryModel` | Read-only data model for SQL queries |
| `QSqlTableModel` | Read/write data model for SQL tables |
| `QSqlRelationalTableModel` | Data model with foreign key support |

## Views

Complete implementations are provided for different kinds of views:

| View | Description |
|------|-------------|
| `QListView` | Displays a list of items |
| `QTableView` | Displays data from a model in a table |
| `QTreeView` | Shows model items of data in a hierarchical list |
| `QComboBox` | Drop-down list combined with a button |
| `QColumnView` | Shows data in a column-based model |

Each of these classes is based on the `QAbstractItemView` abstract base class.

## Delegates

`QAbstractItemDelegate` is the abstract base class for delegates in the model/view framework. The default delegate implementation is provided by `QStyledItemDelegate`, and is used as the default delegate by Qt's standard views.

We recommend using `QStyledItemDelegate` as the base class when implementing custom delegates or when working with Qt style sheets.

## Convenience Classes

A number of convenience classes are derived from the standard view classes:

- `QListWidget` - Item-based list
- `QTreeWidget` - Item-based tree
- `QTableWidget` - Item-based table

These classes are less flexible than the view classes, and cannot be used with arbitrary models. We recommend that you use a model/view approach to handling data in item views unless you strongly need an item-based set of classes.

## Using Models and Views

### Using QFileSystemModel with Views

```cpp
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QSplitter *splitter = new QSplitter;

    QFileSystemModel *model = new QFileSystemModel;
    model->setRootPath(QDir::currentPath());

    QTreeView *tree = new QTreeView(splitter);
    tree->setModel(model);
    tree->setRootIndex(model->index(QDir::currentPath()));

    QListView *list = new QListView(splitter);
    list->setModel(model);
    list->setRootIndex(model->index(QDir::currentPath()));

    splitter->setWindowTitle("Two views onto the same file system model");
    splitter->show();
    return app.exec();
}
```

## Model Classes

### Basic Concepts

In the model/view architecture, the model provides a standard interface that views and delegates use to access data. In Qt, the standard interface is defined by the `QAbstractItemModel` class. No matter how the items of data are stored in any underlying data structure, all subclasses of `QAbstractItemModel` represent the data as a **hierarchical structure** containing tables of items.

### Model Indexes

To ensure that the representation of the data is kept separate from the way it is accessed, the concept of a **model index** is introduced. Each piece of information that can be obtained via a model is represented by a model index. Views and delegates use these indexes to request items of data to display.

Model indexes provide temporary references to pieces of information, and can be used to retrieve or modify data via the model. Since models may reorganize their internal structures from time to time, model indexes may become invalid, and should not be stored. If a long-term reference to a piece of information is required, a **persistent model index** must be created.

To obtain a model index that corresponds to an item of data, three properties must be specified to the model: a **row number**, a **column number**, and the **model index of a parent item**.

```cpp
QModelIndex index = model->index(row, column, parent);
```

### Rows and Columns

In its most basic form, a model can be accessed as a simple table in which items are located by their row and column numbers:

```cpp
QModelIndex indexA = model->index(0, 0, QModelIndex());
QModelIndex indexB = model->index(1, 1, QModelIndex());
QModelIndex indexC = model->index(2, 1, QModelIndex());
```

Top level items in a model are always referenced by specifying `QModelIndex()` as their parent item.

### Parents of Items

The table-like interface to item data provided by models is ideal when using data in a table or list view. However, structures such as tree views require the model to expose a more flexible interface:

```cpp
// Top-level items
QModelIndex indexA = model->index(0, 0, QModelIndex());
QModelIndex indexC = model->index(2, 1, QModelIndex());

// Item "B" is a child of item "A"
QModelIndex indexB = model->index(1, 0, indexA);
```

### Item Roles

Items in a model can perform various roles for other components, allowing different kinds of data to be supplied for different situations:

```cpp
QVariant value = model->data(index, role);
```

| Role | Description |
|------|-------------|
| `Qt::DisplayRole` | String displayed as text in a view |
| `Qt::EditRole` | Data used by editors |
| `Qt::ToolTipRole` | Tooltip text |
| `Qt::StatusTipRole` | Status bar text |
| `Qt::WhatsThisRole` | "What's This?" text |
| `Qt::FontRole` | Font used for items |
| `Qt::ForegroundRole` | Brush for text rendering |
| `Qt::BackgroundRole` | Brush for background rendering |
| `Qt::TextAlignmentRole` | Text alignment |
| `Qt::CheckStateRole` | Check state |
| `Qt::DecorationRole` | Icon or pixmap |
| `Qt::SizeHintRole` | Size hint |

### Using Model Indexes - Example

```cpp
auto *model = new QFileSystemModel;

auto onDirectoryLoaded = [model, layout, &window](const QString &directory) {
    QModelIndex parentIndex = model->index(directory);
    const int numRows = model->rowCount(parentIndex);
    for (int row = 0; row < numRows; ++row) {
        QModelIndex index = model->index(row, 0, parentIndex);

        QString text = model->data(index, Qt::DisplayRole).toString();

        auto *label = new QLabel(text, &window);
        layout->addWidget(label);
    }
};

QObject::connect(model, &QFileSystemModel::directoryLoaded, onDirectoryLoaded);
model->setRootPath(QDir::currentPath());
```

## View Classes

### Using a Model with QListView

```cpp
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QStringList numbers;
    numbers << "One" << "Two" << "Three" << "Four" << "Five";

    QAbstractItemModel *model = new StringListModel(numbers);

    QListView *view = new QListView;
    view->setModel(model);

    view->show();
    return app.exec();
}
```

### Using Multiple Views

Providing multiple views onto the same model is simply a matter of setting the same model for each view:

```cpp
QTableView *firstTableView = new QTableView;
QTableView *secondTableView = new QTableView;

firstTableView->setModel(model);
secondTableView->setModel(model);
```

### Sharing Selections Among Views

```cpp
secondTableView->setSelectionModel(firstTableView->selectionModel());
```

Both views now operate on the same selection model, keeping both the data and the selected items synchronized.

## Delegate Classes

### Creating a Custom Delegate

```cpp
class SpinBoxDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    SpinBoxDelegate(QObject *parent = nullptr);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;

    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const override;

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                              const QModelIndex &index) const override;
};
```

#### Providing an Editor

```cpp
QWidget *SpinBoxDelegate::createEditor(QWidget *parent,
                                       const QStyleOptionViewItem &/* option */,
                                       const QModelIndex &/* index */) const
{
    QSpinBox *editor = new QSpinBox(parent);
    editor->setFrame(false);
    editor->setMinimum(0);
    editor->setMaximum(100);

    return editor;
}
```

#### Setting Editor Data from Model

```cpp
void SpinBoxDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const
{
    int value = index.data(Qt::EditRole).toInt();

    QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
    spinBox->setValue(value);
}
```

#### Submitting Edited Data to Model

```cpp
void SpinBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
    QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
    spinBox->interpretText();
    int value = spinBox->value();

    model->setData(index, value, Qt::EditRole);
}
```

#### Updating Editor Geometry

```cpp
void SpinBoxDelegate::updateEditorGeometry(QWidget *editor,
                                           const QStyleOptionViewItem &option,
                                           const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}
```

## Handling Selections

### Current Item vs Selected Items

| Current Item | Selected Items |
|-------------|----------------|
| There can only be one current item | There can be multiple selected items |
| Changed with key navigation or mouse clicks | Set or unset depending on selection modes |
| Edited if F2 is pressed or double-clicked | Used with anchor to specify selection range |
| Indicated by focus rectangle | Indicated with selection rectangle |

### Selection Modes

| Mode | Description |
|------|-------------|
| `QItemSelectionModel::NoSelection` | Items cannot be selected |
| `QItemSelectionModel::SingleSelection` | Only one item can be selected |
| `QItemSelectionModel::MultiSelection` | Multiple items can be toggled |
| `QItemSelectionModel::ExtendedSelection` | Multiple items with Shift/Ctrl |
| `QItemSelectionModel::ContiguousSelection` | Multiple contiguous items with Shift |
| `QItemSelectionModel::ExtendedSelection` | Standard multi-selection |

### Using Selection Models

```cpp
QItemSelectionModel *selectionModel = view->selectionModel();

// Get selected indexes
QModelIndexList indexes = selectionModel->selectedIndexes();

// Iterate over selected items
for (const QModelIndex &index : indexes) {
    QString text = index.data(Qt::DisplayRole).toString();
    qDebug() << text;
}
```

## Sorting

### Using QSortFilterProxyModel

```cpp
QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel;
proxyModel->setSourceModel(model);

QTableView *view = new QTableView;
view->setModel(proxyModel);

// Sort by column 0, ascending
proxyModel->sort(0, Qt::AscendingOrder);

// Enable case-insensitive filtering
proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
proxyModel->setFilterFixedString("search term");
```

## Custom Model Implementation

### Subclassing QAbstractListModel

```cpp
class StringListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    StringListModel(const QStringList &strings, QObject *parent = nullptr)
        : QAbstractListModel(parent), m_strings(strings) {}

    int rowCount(const QModelIndex &parent = QModelIndex()) const override {
        if (parent.isValid())
            return 0;
        return m_strings.size();
    }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override {
        if (!index.isValid() || index.row() >= m_strings.size())
            return QVariant();

        if (role == Qt::DisplayRole || role == Qt::EditRole)
            return m_strings.at(index.row());

        return QVariant();
    }

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override {
        if (index.isValid() && role == Qt::EditRole) {
            m_strings[index.row()] = value.toString();
            emit dataChanged(index, index, {role});
            return true;
        }
        return false;
    }

    Qt::ItemFlags flags(const QModelIndex &index) const override {
        if (!index.isValid())
            return Qt::NoItemFlags;
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
    }

private:
    QStringList m_strings;
};
```

### Subclassing QAbstractTableModel

```cpp
class MyTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    MyTableModel(QObject *parent = nullptr) : QAbstractTableModel(parent) {}

    int rowCount(const QModelIndex &parent = QModelIndex()) const override {
        if (parent.isValid()) return 0;
        return m_data.size();
    }

    int columnCount(const QModelIndex &parent = QModelIndex()) const override {
        if (parent.isValid()) return 0;
        return 3; // Name, Value, Description
    }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override {
        if (!index.isValid()) return QVariant();

        if (role == Qt::DisplayRole || role == Qt::EditRole) {
            const auto &row = m_data[index.row()];
            switch (index.column()) {
            case 0: return row.name;
            case 1: return row.value;
            case 2: return row.description;
            }
        }
        return QVariant();
    }

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override {
        if (role != Qt::DisplayRole || orientation != Qt::Horizontal)
            return QVariant();

        switch (section) {
        case 0: return "Name";
        case 1: return "Value";
        case 2: return "Description";
        }
        return QVariant();
    }

private:
    struct Row { QString name; int value; QString description; };
    QList<Row> m_data;
};
```

## Best Practices

1. **Always subclass the appropriate base**: Use `QAbstractListModel` for lists, `QAbstractTableModel` for tables, `QAbstractItemModel` for trees
2. **Emit dataChanged()**: When data changes, emit `dataChanged()` with the affected indexes and roles
3. **Use persistent model indexes** for long-term references to data items
4. **Use QSortFilterProxyModel** for sorting and filtering instead of modifying the source model
5. **Share selection models** between views that should show consistent selections
6. **Custom delegates** should subclass `QStyledItemDelegate` for proper style integration
7. **Implement flags()**: Always return appropriate flags for items (editable, selectable, etc.)

## See Also

- Model Classes
- View Classes
- Delegate Classes
- Using Selections in Item Views
- Chart Example
