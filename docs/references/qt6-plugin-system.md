---
source: https://doc.qt.io/qt-6/plugins-howto.html
fetched: 2026-05-12
---

# How to Create Qt Plugins

Qt provides two APIs for creating plugins:

- A high-level API for writing extensions to Qt itself, such as custom database drivers, image formats, text codecs, and styles.
- A low-level API for extending Qt applications.

For example, if you want to write a custom [QStyle](qstyle.html) subclass and have Qt applications load it dynamically, you would use the higher-level API.

Since the higher-level API is built on top of the lower-level API, some issues are common to both.

If you want to provide plugins for use with Qt Widgets Designer, see [Creating Custom Widget Plugins](qtdesigner-components.html#creating-custom-widget-plugins).

## The High-Level API: Writing Qt Extensions

Writing a plugin that extends Qt itself is achieved by subclassing the appropriate plugin base class, implementing a few functions, and adding a macro.

There are several plugin base classes. Derived plugins are stored by default in sub-directories of the standard plugin directory. Qt will not find plugins if they are not stored in the appropriate directory.

The following table summarizes the plugin base classes. Some of the classes are private, and are therefore not documented. You can use them, but there is no compatibility promise with later Qt versions.

::: table
  Base Class                                                    Directory Name                            Qt Module                                       Key Case Sensitivity
  ------------------------------------------------------------- ----------------------------------------- ----------------------------------------------- ----------------------
  QAccessibleBridgePlugin                                       `accessiblebridge`        [Qt GUI](qtgui-index.html)                      Case Sensitive
  [QImageIOPlugin](qimageioplugin.html)         `imageformats`            [Qt GUI](qtgui-index.html)                      Case Sensitive
  QPictureFormatPlugin (obsolete)                               `pictureformats`          [Qt GUI](qtgui-index.html)                      Case Sensitive
  QBearerEnginePlugin                                           `bearer`                  [Qt Network](qtnetwork-index.html)              Case Sensitive
  QPlatformInputContextPlugin                                   `platforminputcontexts`   [Qt Platform Abstraction](qpa.html)             Case Insensitive
  QPlatformIntegrationPlugin                                    `platforms`               [Qt Platform Abstraction](qpa.html)             Case Insensitive
  QPlatformThemePlugin                                          `platformthemes`          [Qt Platform Abstraction](qpa.html)             Case Insensitive
  QPlatformPrinterSupportPlugin                                 `printsupport`            [Qt Print Support](qtprintsupport-index.html)   Case Insensitive
  QSGContextPlugin                                              `scenegraph`              [Qt Quick](topics-graphics2d.html#qt-quick)     Case Sensitive
  [QSqlDriverPlugin](qsqldriverplugin.html)     `sqldrivers`              [Qt SQL](qtsql-index.html)                      Case Sensitive
  [QIconEnginePlugin](qiconengineplugin.html)   `iconengines`             [Qt SVG](qtsvg-index.html)                      Case Insensitive
  [QAccessiblePlugin](qaccessibleplugin.html)   `accessible`              [Qt Widgets](qtwidgets-index.html)              Case Sensitive
  [QStylePlugin](qstyleplugin.html)             `styles`                  [Qt Widgets](qtwidgets-index.html)              Case Insensitive

If you have a new document viewer class called `JsonViewer` that you want to make available as a plugin, the class needs to be defined as follows (`jsonviewer.h`):

::: pre
``` {.cpp .prettyprint translate="no"}
class JsonViewer : public ViewerInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.Examples.DocumentViewer.ViewerInterface/1.0" FILE "jsonviewer.json")
    Q_INTERFACES(ViewerInterface)
public:
    JsonViewer();
    ~JsonViewer() override;
private:
    bool openJsonFile();

    QTreeView *m_tree;
    QListWidget *m_toplevel = nullptr;
    QJsonDocument m_root;
    QMenu *m_jsonMenu = nullptr;
    QToolBar *m_jsonToolBar = nullptr;
    QAction *m_expandAllAction = nullptr;
    QAction *m_collapseAllAction = nullptr;
};
```

Ensure that the class implementation is located in a `.cpp` file:

::: pre
``` {.cpp .prettyprint translate="no"}
JsonViewer::JsonViewer()

void JsonViewer::init(QFile *file, QWidget *parent, QMainWindow *mainWindow)

```

In addition, a json file (`jsonviewer.json`) containing meta data describing the plugin is required for most plugins. For document viewer plugins it simply contains the name of the viewer plugin.

::: pre
``` {.cpp .prettyprint translate="no"}

```

The type of information that needs to be provided in the json file is plugin dependent. See the class documentation for details on the information that needs to be contained in the file.

For database drivers, image formats, text codecs, and most other plugin types, no explicit object creation is required. Qt will find and create them as required.

Plugin classes may require additional functions to be implemented. See the class documentation for details of the virtual functions that must be reimplemented for each type of plugin.

The [Document Viewer Demo](qtdoc-demos-documentviewer-example.html) shows how to implement a plugin that displayes structured contents of a file. Each plugin therefore reimplements virtual functions, which

- identify the plugin
- return the MIME types it supports
- inform whether there is content to display and
- how contents are presented

::: pre
``` {.cpp .prettyprint translate="no"}
    QString viewerName() const override { return QLatin1StringView(staticMetaObject.className()); };
    QStringList supportedMimeTypes() const override;
    bool hasContent() const override;
    bool supportsOverview() const override { return true; }
```

## The Low-Level API: Extending Qt Applications

In addition to Qt itself, Qt applications can be extended through plugins. This requires the application to detect and load plugins using [QPluginLoader](qpluginloader.html). In that context, plugins may provide arbitrary functionality and are not limited to database drivers, image formats, text codecs, styles, and other types of plugins that extend Qt\'s functionality.

Making an application extensible through plugins involves the following steps:

1.  Define a set of interfaces (classes with only pure virtual functions) used to talk to the plugins.
2.  Use the [Q_DECLARE_INTERFACE](qtplugin.html#Q_DECLARE_INTERFACE)() macro to tell Qt\'s [meta-object system](metaobjects.html) about the interface.
3.  Use [QPluginLoader](qpluginloader.html) in the application to load the plugins.
4.  Use [qobject_cast](qobject.html#qobject_cast)() to test whether a plugin implements a given interface.

Writing a plugin involves these steps:

1.  Declare a plugin class that inherits from [QObject](qobject.html) and from the interfaces that the plugin wants to provide.
2.  Use the [Q_INTERFACES](qobject.html#Q_INTERFACES)() macro to tell Qt\'s [meta-object system](metaobjects.html) about the interfaces.
3.  Export the plugin using the [Q_PLUGIN_METADATA](qtplugin.html#Q_PLUGIN_METADATA)() macro.

For example, here\'s the definition of an interface class:

::: pre
``` {.cpp .prettyprint translate="no"}
class ViewerInterface : public AbstractViewer
{
public:
    virtual ~ViewerInterface() = default;
};
```

Here\'s the interface declaration:

::: pre
``` {.cpp .prettyprint translate="no"}
#define ViewerInterface_iid "org.qt-project.Qt.Examples.DocumentViewer.ViewerInterface/1.0"
Q_DECLARE_INTERFACE(ViewerInterface, ViewerInterface_iid)
```

See also [Creating Custom Widgets for Qt Widgets Designer](designer-creating-custom-widgets.html) for information about issues that are specific to Qt Widgets Designer. See the [Calendar Backend Plugin Example](qtcore-time-calendarbackendplugin-example.html) for an example of using a plugin to add support for a custom calendar system.

## Locating Plugins

Qt applications automatically know which plugins are available, because plugins are stored in the standard plugin subdirectories. Because of this, applications don\'t require any code to find and load plugins, since Qt handles them automatically.

During development, the directory for plugins is `QTDIR/plugins` (where `QTDIR` is the directory where Qt is installed), with each type of plugin in a subdirectory for that type, for example, `styles`. If you want your applications to use plugins and you don\'t want to use the standard plugins path, have your installation process determine the path you want to use for the plugins, and save the path, for example, by using [QSettings](qsettings.html), for the application to read when it runs. The application can then call [QCoreApplication::addLibraryPath](qcoreapplication.html#addLibraryPath)() with this path and your plugins will be available to the application. Note that the final part of the path (for example, `styles`) cannot be changed.

If you want the plugin to be loadable, one approach is to create a subdirectory under the application, and place the plugin in that directory. If you distribute any of the plugins that come with Qt (the ones located in the `plugins` directory), you must copy the subdirectory under `plugins` where the plugin is located to your applications root folder (i.e., do not include the `plugins` directory).

For more information about deployment, see the [Deploying Qt Applications](deployment.html) and [Deploying Plugins](deployment-plugins.html) documentation.

## Static Plugins

The normal and most flexible way to include a plugin with an application is to compile it into a dynamic library that is shipped separately, and detected and loaded at runtime.

Plugins can be linked statically into your application. If you build the static version of Qt, this is the only option for including Qt\'s predefined plugins. Using static plugins makes the deployment less error-prone, but has the disadvantage that no functionality from plugins can be added without a complete rebuild and redistribution of the application.

CMake and qmake automatically add the plugins that are typically needed by the Qt modules that are used, while more specialized plugins need to be added manually. The default list of automatically added plugins can be overridden per type.

The defaults are tuned towards an optimal out-of-the-box experience, but may unnecessarily bloat the application. It is recommended to inspect the linker command line and eliminate unnecessary plugins.

To cause static plugins actually being linked and instantiated, [Q_IMPORT_PLUGIN](qtplugin.html#Q_IMPORT_PLUGIN)() macros are also needed in application code, but those are automatically generated by the build system and added to your application project.

### Importing Static Plugins in CMake

To statically link plugins in a CMake project, you need to call the [qt_import_plugins](qt-import-plugins.html) command.

For example, the Linux `libinput` plugin is not imported by default. The following command imports it:

::: pre
``` {.cpp .prettyprint translate="no"}
qt_import_plugins(myapp INCLUDE Qt::QLibInputPlugin)
```

To link the minimal platform integration plugin instead of the default Qt platform adaptation plugin, use:

::: pre
``` {.cpp .prettyprint translate="no"}
qt_import_plugins(myapp
    INCLUDE_BY_TYPE platforms Qt::MinimalIntegrationPlugin
)
```

Another typical use case is to link only a certain set of `imageformats` plugins:

::: pre
``` {.cpp .prettyprint translate="no"}
qt_import_plugins(myapp
    INCLUDE_BY_TYPE imageformats Qt::QJpegPlugin Qt::QGifPlugin
)
```

If you want to prevent the linking of any `imageformats` plugin, use:

::: pre
``` {.cpp translate="no"}
qt_import_plugins(myapp
    EXCLUDE_BY_TYPE imageformats
)
```

If you want to turn off the addition of any default plugin, use the `NO_DEFAULT` option of [qt_import_plugins](qt-import-plugins.html).

### Importing Static Plugins in qmake

In a qmake project, you need to add the required plugins to your build using `QTPLUGIN`:

::: pre
``` {.cpp .prettyprint translate="no"}
QTPLUGIN += qlibinputplugin
```

For example, to link the minimal plugin instead of the default Qt platform adaptation plugin, use:

::: pre
``` {.cpp translate="no"}
QTPLUGIN.platforms = qminimal
```

If you want neither the default, nor the minimal QPA plugin to be linked automatically, use:

::: pre
``` {.cpp .prettyprint translate="no"}
QTPLUGIN.platforms = -
```

If you do not want all plugins added to QTPLUGIN to be automatically linked, remove `import_plugins` from the `CONFIG` variable:

::: pre
``` {.cpp .prettyprint translate="no"}
CONFIG -= import_plugins
```

### Creating Static Plugins

It is also possible to create your own static plugins by following these steps:

1.  Pass the `STATIC` option to the [qt_add_plugin](qt-add-plugin.html) command in your `CMakeLists.txt`. For a qmake project, add `CONFIG += static` to your plugin\'s `.pro` file.
2.  Use the [Q_IMPORT_PLUGIN](qtplugin.html#Q_IMPORT_PLUGIN)() macro in your application.
3.  Use the [Q_INIT_RESOURCE](qtresource.html#Q_INIT_RESOURCE)() macro in your application if the plugin ships qrc files.
4.  Link your application with your plugin library using [target_link_libraries](https://cmake.org/cmake/help/latest/command/target_link_libraries.html) in your `CMakeLists.txt` or `LIBS` in your `.pro` file.

See the [Plug & Paint](https://code.qt.io/cgit/qt/qtbase.git/tree/tests/manual/examples/widgets/tools/plugandpaint/app?h=6.11) example and the associated Basic Tools plugin for details on how to do this.

**Note:** If you are not using CMake or qmake to build your plugin, you need to make sure that the `QT_STATICPLUGIN` preprocessor macro is defined.

### Loading plugins

Plugin types (static or shared) and operating systems require specific approaches to locate and load plugins. It\'s useful to implement an abstraction for loading plugins.

::: pre
``` {.cpp .prettyprint translate="no"}
void ViewerFactory::loadViewerPlugins()

    // Load static plugins
    const QObjectList &staticPlugins = QPluginLoader::staticInstances();
    for (auto *plugin : staticPlugins)
        addViewer(plugin);
```

Shared plugins reside in their deployment directories, which may require OS-specific handling.

::: pre
``` {.cpp .prettyprint translate="no"}
    // Load shared plugins
    QDir pluginsDir = QDir(QApplication::applicationDirPath());
#if defined(Q_OS_DARWIN)
    if (pluginsDir.exists("../PlugIns"_L1)) { // installed build
        pluginsDir.cd("../PlugIns"_L1);
    } else {
        pluginsDir.cd("../../../../plugins"_L1); // non-installed build
    }
#elif defined(Q_OS_WIN)
    if (pluginsDir.exists("plugins"_L1)) { // non-installed build
        pluginsDir.cd("plugins"_L1);
    } else {
        pluginsDir.cd("../plugins"_L1); // installed build
    }
#else
    pluginsDir.cd("../plugins"_L1); // installed and non-installed build
#endif

    // qDebug("Loading plugins from %s...", qUtf8Printable(pluginsDir.path()));
    const auto entryList = pluginsDir.entryList(QDir::Files);
    for (const QString &fileName : entryList) {
        QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = loader.instance();
        if (plugin)
            addViewer(plugin);
#if 0
        else
            qDebug() << loader.errorString();
#endif
    }
}
```

## Deploying and Debugging Plugins

The [Deploying Plugins](deployment-plugins.html) document covers the process of deploying plugins with applications and debugging them when problems arise.

**See also** [QPluginLoader](qpluginloader.html) and [QLibrary](qlibrary.html).

[Best Practice Guides](best-practices.html){.prevPage} [Creating Shared Libraries](sharedlibrary.html){.nextPage}

[©]{.abbr title="Copyright"} 2026 The Qt Company Ltd. Documentation contributions included herein are the copyrights of their respective owners. The documentation provided herein is licensed under the terms of the [GNU Free Documentation License version 1.3](http://www.gnu.org/licenses/fdl.html) as published by the Free Software Foundation. Qt and respective logos are [trademarks](https://doc.qt.io/qt/trademarks.html) of The Qt Company Ltd. in Finland and/or other countries worldwide. All other trademarks are property of their respective owners.
