# Qt GUI

> **来源**: [https://doc.qt.io/qt-6/qtgui-module.html](https://doc.qt.io/qt-6/qtgui-module.html)
> **获取日期**: 2026-05-12
> **描述**: GUI 基础模块，提供窗口系统集成、事件处理、OpenGL 集成、字体处理和图像处理

# Qt GUI C++ Classes

The Qt GUI module provides the basic enablers for graphical applications written with Qt.

## Namespaces

  [QColorConstants](qcolorconstants.html)   Contains QColor predefined constants

## Classes

  [QAbstractFileIconProvider](qabstractfileiconprovider.html)                                            File icons for the QFileSystemModel class
  [QAbstractTextDocumentLayout](qabstracttextdocumentlayout.html)                                        Abstract base class used to implement custom layouts for QTextDocuments
  [QAbstractTextDocumentLayout::PaintContext](qabstracttextdocumentlayout-paintcontext.html)             QAbstractTextDocumentLayout::PaintContext class is a convenience class defining the parameters used when painting a document's layout
  [QAbstractTextDocumentLayout::Selection](qabstracttextdocumentlayout-selection.html)                   QAbstractTextDocumentLayout::Selection class is a convenience class defining the parameters of a selection
  [QAccessibilityHints](qaccessibilityhints.html)                                                        Contains platform specific accessibility hints and settings
  [QAccessible](qaccessible.html)                                                                        Enums and static functions related to accessibility
  [QAccessible::State](qaccessible-state.html)
  [QAccessibleActionInterface](qaccessibleactioninterface.html)                                          Implements support for invocable actions in the interface
  [QAccessibleAnnouncementEvent](qaccessibleannouncementevent.html)                                      Used to request the announcement of a given message by assistive technologies
  [QAccessibleAttributesInterface](qaccessibleattributesinterface.html)                                  Implements support for reporting attributes for an accessible object
  [QAccessibleEditableTextInterface](qaccessibleeditabletextinterface.html)                              Implements support for objects with editable text
  [QAccessibleEvent](qaccessibleevent.html)                                                              The base class for accessibility notifications
  [QAccessibleInterface](qaccessibleinterface.html)                                                      Defines an interface that exposes information about accessible objects
  [QAccessibleObject](qaccessibleobject.html)                                                            Implements parts of the QAccessibleInterface for QObjects
  [QAccessiblePlugin](qaccessibleplugin.html)                                                            Abstract base class for plugins provinding accessibility information for user interface elements
  [QAccessibleSelectionInterface](qaccessibleselectioninterface.html)                                    Implements support for selection handling
  [QAccessibleStateChangeEvent](qaccessiblestatechangeevent.html)                                        Notfies the accessibility framework that the state of an object has changed
  [QAccessibleTableCellInterface](qaccessibletablecellinterface.html)                                    Implements support for the IAccessibleTable2 Cell interface
  [QAccessibleTableInterface](qaccessibletableinterface.html)                                            Implements support for the IAccessibleTable2 interface
  [QAccessibleTableModelChangeEvent](qaccessibletablemodelchangeevent.html)                              Signifies a change in a table, list, or tree where cells are added or removed. If the change affected a number of rows, firstColumn and lastColumn will return -1. Likewise for columns, the row functions may return -1
  [QAccessibleTextCursorEvent](qaccessibletextcursorevent.html)                                          Notifies of cursor movements
  [QAccessibleTextInsertEvent](qaccessibletextinsertevent.html)                                          Notifies of text being inserted
  [QAccessibleTextInterface](qaccessibletextinterface.html)                                              Implements support for text handling
  [QAccessibleTextRemoveEvent](qaccessibletextremoveevent.html)                                          Notifies of text being deleted
  [QAccessibleTextSelectionEvent](qaccessibletextselectionevent.html)                                    Signals a change in the text selection of an object
  [QAccessibleTextUpdateEvent](qaccessibletextupdateevent.html)                                          Notifies about text changes. This is for accessibles that support editable text such as line edits. This event occurs for example when a portion of selected text gets replaced by pasting a new text or in override mode of editors
  [QAccessibleValueChangeEvent](qaccessiblevaluechangeevent.html)                                        Describes a change in value for an accessible object
  [QAccessibleValueInterface](qaccessiblevalueinterface.html)                                            Implements support for objects that manipulate a value
  [QAction](qaction.html)                                                                                Abstraction for user commands that can be added to different user interface components
  [QActionEvent](qactionevent.html)                                                                      Event that is generated when a QAction is added, removed, or changed
  [QActionGroup](qactiongroup.html)                                                                      Groups actions together
  [QBackingStore](qbackingstore.html)                                                                    Drawing area for QWindow
  [QBitmap](qbitmap.html)                                                                                Monochrome (1-bit depth) pixmaps
  [QBrush](qbrush.html)                                                                                  Defines the fill pattern of shapes drawn by QPainter
  [QChildWindowEvent](qchildwindowevent.html)                                                            Contains event parameters for child window changes
  [QClipboard](qclipboard.html)                                                                          Access to the window system clipboard
  [QCloseEvent](qcloseevent.html)                                                                        Contains parameters that describe a close event
  [QColor](qcolor.html)                                                                                  Colors based on RGB, HSV or CMYK values
  [QColorSpace](qcolorspace.html)                                                                        Color space abstraction
  [QColorSpace::PrimaryPoints](qcolorspace-primarypoints.html)                                           Struct contains four primary color space points
  [QColorTransform](qcolortransform.html)                                                                Transformation between color spaces
  [QConicalGradient](qconicalgradient.html)                                                              Used in combination with QBrush to specify a conical gradient brush
  [QContextMenuEvent](qcontextmenuevent.html)                                                            Contains parameters that describe a context menu event
  [QCursor](qcursor.html)                                                                                Mouse cursor with an arbitrary shape
  [QDesktopServices](qdesktopservices.html)                                                              Methods for accessing common desktop services
  [QDoubleValidator](qdoublevalidator.html)                                                              Range checking of floating-point numbers
  [QDrag](qdrag.html)                                                                                    Support for MIME-based drag and drop data transfer
  [QDragEnterEvent](qdragenterevent.html)                                                                Event which is sent to a widget when a drag and drop action enters it
  [QDragLeaveEvent](qdragleaveevent.html)                                                                Event that is sent to a widget when a drag and drop action leaves it
  [QDragMoveEvent](qdragmoveevent.html)                                                                  Event which is sent while a drag and drop action is in progress
  [QDropEvent](qdropevent.html)                                                                          Event which is sent when a drag and drop action is completed
  [QEnterEvent](qenterevent.html)                                                                        Contains parameters that describe an enter event
  [QEventPoint](qeventpoint.html)                                                                        Information about a point in a QPointerEvent
  [QExposeEvent](qexposeevent.html)                                                                      Contains event parameters for expose events
  [QFileOpenEvent](qfileopenevent.html)                                                                  Event that will be sent when there is a request to open a file or a URL
  [QFileSystemModel](qfilesystemmodel.html)                                                              Data model for the local filesystem
  [QFocusEvent](qfocusevent.html)                                                                        Contains event parameters for widget focus events
  [QFont](qfont.html)                                                                                    Specifies a query for a font used for drawing text
  [QFont::Tag](qfont-tag.html)                                                                           QFont::Tag type provides access to advanced font features
  [QFontDatabase](qfontdatabase.html)                                                                    Information about the fonts available in the underlying window system
  [QFontInfo](qfontinfo.html)                                                                            General information about fonts
  [QFontMetrics](qfontmetrics.html)                                                                      Font metrics information
  [QFontMetricsF](qfontmetricsf.html)                                                                    Font metrics information
  [QFontVariableAxis](qfontvariableaxis.html)                                                            Represents a variable axis in a font
  [QGenericMatrix](qgenericmatrix.html)                                                                  Template class that represents a NxM transformation matrix with N columns and M rows
  [QGenericPlugin](qgenericplugin.html)                                                                  Abstract base class for plugins
  [QGenericPluginFactory](qgenericpluginfactory.html)                                                    Creates plugin drivers
  [QGlyphRun](qglyphrun.html)                                                                            Direct access to the internal glyphs in a font
  [QGradient](qgradient.html)                                                                            Used in combination with QBrush to specify gradient fills
  [QGuiApplication](qguiapplication.html)                                                                Manages the GUI application's control flow and main settings
  [QHelpEvent](qhelpevent.html)                                                                          Event that is used to request helpful information about a particular point in a widget
  [QHideEvent](qhideevent.html)                                                                          Event which is sent after a widget is hidden
  [QHoverEvent](qhoverevent.html)                                                                        Contains parameters that describe a mouse event
  [QIcon](qicon.html)                                                                                    Scalable icons in different modes and states
  [QIconDragEvent](qicondragevent.html)                                                                  Indicates that a main icon drag has begun
  [QIconEngine](qiconengine.html)                                                                        Abstract base class for QIcon renderers
  [QIconEngine::ScaledPixmapArgument](qiconengine-scaledpixmapargument.html)
  [QIconEnginePlugin](qiconengineplugin.html)                                                            Abstract base for custom QIconEngine plugins
  [QImage](qimage.html)                                                                                  Hardware-independent image representation that allows direct access to the pixel data, and can be used as a paint device
  [QImageIOHandler](qimageiohandler.html)                                                                Defines the common image I/O interface for all image formats in Qt
  [QImageIOPlugin](qimageioplugin.html)                                                                  Defines an interface for writing an image format plugin
  [QImageReader](qimagereader.html)                                                                      Format independent interface for reading images from files or other devices
  [QImageWriter](qimagewriter.html)                                                                      Format independent interface for writing images to files or other devices
  [QInputDevice](qinputdevice.html)                                                                      Describes a device from which a QInputEvent originates
  [QInputEvent](qinputevent.html)                                                                        The base class for events that describe user input
  [QInputMethod](qinputmethod.html)                                                                      Access to the active text input method
  [QInputMethodEvent](qinputmethodevent.html)                                                            Parameters for input method events
  [QInputMethodEvent::Attribute](qinputmethodevent-attribute.html)                                       QInputMethodEvent::Attribute class stores an input method attribute
  [QInputMethodQueryEvent](qinputmethodqueryevent.html)                                                  Event sent by the input context to input objects
  [QIntValidator](qintvalidator.html)                                                                    Validator that ensures a string contains a valid integer within a specified range
  [QKeyEvent](qkeyevent.html)                                                                            Describes a key event
  [QKeySequence](qkeysequence.html)                                                                      Encapsulates a key sequence as used by shortcuts
  [QLinearGradient](qlineargradient.html)                                                                Used in combination with QBrush to specify a linear gradient brush
  [QMatrix4x4](qmatrix4x4.html)                                                                          Represents a 4x4 transformation matrix in 3D space
  [QMouseEvent](qmouseevent.html)                                                                        Contains parameters that describe a mouse event
  [QMoveEvent](qmoveevent.html)                                                                          Contains event parameters for move events
  [QMovie](qmovie.html)                                                                                  Convenience class for playing movies with QImageReader
  [QNativeGestureEvent](qnativegestureevent.html)                                                        Contains parameters that describe a gesture event
  [QNativeInterface::QAndroidOffscreenSurface](qnativeinterface-qandroidoffscreensurface.html)           Native interface to a offscreen surface on Android
  [QNativeInterface::QAndroidScreen](qnativeinterface-qandroidscreen.html)                               Native interface to a screen
  [QNativeInterface::QCocoaGLContext](qnativeinterface-qcocoaglcontext.html)                             Native interface to an NSOpenGLContext on macOS
  [QNativeInterface::QCocoaScreen](qnativeinterface-qcocoascreen.html)                                   Native interface to a screen on macOS
  [QNativeInterface::QEGLContext](qnativeinterface-qeglcontext.html)                                     Native interface to an EGL context
  [QNativeInterface::QGLXContext](qnativeinterface-qglxcontext.html)                                     Native interface to a GLX context
  [QNativeInterface::QWGLContext](qnativeinterface-qwglcontext.html)                                     Native interface to a WGL context on Windows
  [QNativeInterface::QWaylandApplication](qnativeinterface-qwaylandapplication.html)                     Native interface to a Wayland application
  [QNativeInterface::QWaylandScreen](qnativeinterface-qwaylandscreen.html)                               Native interface to a screen on Wayland
  [QNativeInterface::QWindowsScreen](qnativeinterface-qwindowsscreen.html)                               Native interface to a screen
  [QNativeInterface::QX11Application](qnativeinterface-qx11application.html)                             Native interface to an X11 application
  [QOffscreenSurface](qoffscreensurface.html)                                                            Represents an offscreen surface in the underlying platform
  [QOpenGLContext](qopenglcontext.html)                                                                  Represents a native OpenGL context, enabling OpenGL rendering on a QSurface
  [QOpenGLContextGroup](qopenglcontextgroup.html)                                                        Represents a group of contexts sharing OpenGL resources
  [QOpenGLExtraFunctions](qopenglextrafunctions.html)                                                    Cross-platform access to the OpenGL ES 3.0, 3.1 and 3.2 API
  [QOpenGLFunctions](qopenglfunctions.html)                                                              Cross-platform access to the OpenGL ES 2.0 API
  [QOpenGLTexture](qopengltexture.html)                                                                  Encapsulates an OpenGL texture object
  [QPageLayout](qpagelayout.html)                                                                        Describes the size, orientation and margins of a page
  [QPageRanges](qpageranges.html)                                                                        Represents a collection of page ranges
  [QPageRanges::Range](qpageranges-range.html)                                                           QPageRanges::Range struct holds the from and to endpoints of a range
  [QPageSize](qpagesize.html)                                                                            Describes the size and name of a defined page size
  [QPagedPaintDevice](qpagedpaintdevice.html)                                                            Represents a paint device that supports multiple pages
  [QPaintDevice](qpaintdevice.html)                                                                      The base class of objects that can be painted on with QPainter
  [QPaintDeviceWindow](qpaintdevicewindow.html)                                                          Convenience subclass of QWindow that is also a QPaintDevice
  [QPaintEngine](qpaintengine.html)                                                                      Abstract definition of how QPainter draws to a given device on a given platform
  [QPaintEngineState](qpaintenginestate.html)                                                            Information about the active paint engine's current state
  [QPaintEvent](qpaintevent.html)                                                                        Contains event parameters for paint events
  [QPainter](qpainter.html)                                                                              Performs low-level painting on widgets and other paint devices
  [QPainter::PixmapFragment](qpainter-pixmapfragment.html)                                               This class is used in conjunction with the QPainter::drawPixmapFragments() function to specify how a pixmap, or sub-rect of a pixmap, is drawn
  [QPainterPath](qpainterpath.html)                                                                      Container for painting operations, enabling graphical shapes to be constructed and reused
  [QPainterPath::Element](qpainterpath-element.html)                                                     QPainterPath::Element class specifies the position and type of a subpath
  [QPainterPathStroker](qpainterpathstroker.html)                                                        Used to generate fillable outlines for a given painter path
  [QPainterStateGuard](qpainterstateguard.html)                                                          RAII convenience class for balanced QPainter::save() and QPainter::restore() calls
  [QPalette](qpalette.html)                                                                              Contains color groups for each widget state
  [QPdfOutputIntent](qpdfoutputintent.html)
  [QPdfWriter](qpdfwriter.html)                                                                          Class to generate PDFs that can be used as a paint device
  [QPen](qpen.html)                                                                                      Defines how a QPainter should draw lines and outlines of shapes
  [QPicture](qpicture.html)                                                                              Paint device that records and replays QPainter commands
  [QPixelFormat](qpixelformat.html)                                                                      Class for describing different pixel layouts in graphics buffers
  [QPixmap](qpixmap.html)                                                                                Off-screen image representation that can be used as a paint device
  [QPixmapCache](qpixmapcache.html)                                                                      Application-wide cache for pixmaps
  [QPixmapCache::Key](qpixmapcache-key.html)                                                             QPixmapCache::Key class can be used for efficient access to the QPixmapCache
  [QPlatformSurfaceEvent](qplatformsurfaceevent.html)                                                    Used to notify about native platform surface events
  [QPointerEvent](qpointerevent.html)                                                                    A base class for pointer events
  [QPointingDevice](qpointingdevice.html)                                                                Describes a device from which mouse, touch or tablet events originate
  [QPointingDeviceUniqueId](qpointingdeviceuniqueid.html)                                                Identifies a unique object, such as a tagged token or stylus, which is used with a pointing device
  [QPolygon](qpolygon.html)                                                                              List of points using integer precision
  [QPolygonF](qpolygonf.html)                                                                            List of points using floating point precision
  [QQuaternion](qquaternion.html)                                                                        Represents a quaternion consisting of a vector and scalar
  [QQuaternion::Axes](qquaternion-axes.html)
  [QQuaternion::Axis](qquaternion-axis.html)
  [QQuaternion::EulerAngles](qquaternion-eulerangles.html)
  [QRadialGradient](qradialgradient.html)                                                                Used in combination with QBrush to specify a radial gradient brush
  [QRasterWindow](qrasterwindow.html)                                                                    Convenience class for using QPainter on a QWindow
  [QRawFont](qrawfont.html)                                                                              Access to a single physical instance of a font
  [QRegion](qregion.html)                                                                                Specifies a clip region for a painter
  [QRegularExpressionValidator](qregularexpressionvalidator.html)                                        Used to check a string against a regular expression
  [QResizeEvent](qresizeevent.html)                                                                      Contains event parameters for resize events
  [QRgba64](qrgba64.html)                                                                                Struct contains a 64-bit RGB color
  [QRgbaFloat](qrgbafloat.html)                                                                          Struct contains a four part RGBA floating-point color
  [QRhiVulkanQueueSubmitParams](qrhivulkanqueuesubmitparams.html)                                        References additional Vulkan API objects that get passed to vkQueueSubmit()
  [QScreen](qscreen.html)                                                                                Used to query screen properties
  [QScrollEvent](qscrollevent.html)                                                                      Sent when scrolling
  [QScrollPrepareEvent](qscrollprepareevent.html)                                                        Sent in preparation of scrolling
  [QSessionManager](qsessionmanager.html)                                                                Access to the session manager
  [QShader](qshader.html)                                                                                Contains multiple versions of a shader translated to multiple shading languages, together with reflection metadata
  [QShader::NativeShaderInfo](qshader-nativeshaderinfo.html)                                             Additional metadata about the native shader code
  [QShader::SeparateToCombinedImageSamplerMapping](qshader-separatetocombinedimagesamplermapping.html)   Mapping metadata for sampler uniforms
  [QShaderCode](qshadercode.html)                                                                        Contains source or binary code for a shader and additional metadata
  [QShaderDescription](qshaderdescription.html)                                                          Describes the interface of a shader
  [QShaderDescription::BlockVariable](qshaderdescription-blockvariable.html)                             Describes a member of a uniform or push constant block
  [QShaderDescription::BuiltinVariable](qshaderdescription-builtinvariable.html)                         Describes a built-in variable
  [QShaderDescription::InOutVariable](qshaderdescription-inoutvariable.html)                             Describes an input or output variable in the shader
  [QShaderDescription::PushConstantBlock](qshaderdescription-pushconstantblock.html)                     Describes a push constant block
  [QShaderDescription::StorageBlock](qshaderdescription-storageblock.html)                               Describes a shader storage block
  [QShaderDescription::UniformBlock](qshaderdescription-uniformblock.html)                               Describes a uniform block
  [QShaderKey](qshaderkey.html)                                                                          Specifies the shading language, the version with flags, and the variant
  [QShaderVersion](qshaderversion.html)                                                                  Specifies the shading language version
  [QShortcut](qshortcut.html)                                                                            Used to create keyboard shortcuts
  [QShortcutEvent](qshortcutevent.html)                                                                  Event which is generated when the user presses a key combination
  [QShowEvent](qshowevent.html)                                                                          Event that is sent when a widget is shown
  [QSinglePointEvent](qsinglepointevent.html)                                                            A base class for pointer events containing a single point, such as mouse events
  [QStandardItem](qstandarditem.html)                                                                    Item for use with the QStandardItemModel class
  [QStandardItemModel](qstandarditemmodel.html)                                                          Generic model for storing custom data
  [QStaticText](qstatictext.html)                                                                        Enables optimized drawing of text when the text and its layout is updated rarely
  [QStatusTipEvent](qstatustipevent.html)                                                                Event that is used to show messages in a status bar
  [QStyleHints](qstylehints.html)                                                                        Contains platform specific hints and settings
  [QSupportedWritingSystems](qsupportedwritingsystems.html)                                              Used when registering fonts with the internal Qt fontdatabase
  [QSurface](qsurface.html)                                                                              Abstraction of renderable surfaces in Qt
  [QSurfaceFormat](qsurfaceformat.html)                                                                  Represents the format of a QSurface
  [QSyntaxHighlighter](qsyntaxhighlighter.html)                                                          Allows you to define syntax highlighting rules, and in addition you can use the class to query a document's current formatting or user data
  [QTabletEvent](qtabletevent.html)                                                                      Contains parameters that describe a Tablet event
  [QTextBlock](qtextblock.html)                                                                          Container for text fragments in a QTextDocument
  [QTextBlock::iterator](qtextblock-iterator.html)                                                       QTextBlock::iterator class provides an iterator for reading the contents of a QTextBlock
  [QTextBlockFormat](qtextblockformat.html)                                                              Formatting information for blocks of text in a QTextDocument
  [QTextBlockGroup](qtextblockgroup.html)                                                                Container for text blocks within a QTextDocument
  [QTextBlockUserData](qtextblockuserdata.html)                                                          Used to associate custom data with blocks of text
  [QTextCharFormat](qtextcharformat.html)                                                                Formatting information for characters in a QTextDocument
  [QTextCursor](qtextcursor.html)                                                                        Offers an API to access and modify QTextDocuments
  [QTextDocument](qtextdocument.html)                                                                    Holds formatted text
  [QTextDocumentFragment](qtextdocumentfragment.html)                                                    Represents a piece of formatted text from a QTextDocument
  [QTextDocumentWriter](qtextdocumentwriter.html)                                                        Format-independent interface for writing a QTextDocument to files or other devices
  [QTextFormat](qtextformat.html)                                                                        Formatting information for a QTextDocument
  [QTextFragment](qtextfragment.html)                                                                    Holds a piece of text in a QTextDocument with a single QTextCharFormat
  [QTextFrame](qtextframe.html)                                                                          Represents a frame in a QTextDocument
  [QTextFrameFormat](qtextframeformat.html)                                                              Formatting information for frames in a QTextDocument
  [QTextImageFormat](qtextimageformat.html)                                                              Formatting information for images in a QTextDocument
  [QTextInlineObject](qtextinlineobject.html)                                                            Represents an inline object in a QAbstractTextDocumentLayout and its implementations
  [QTextItem](qtextitem.html)                                                                            All the information required to draw text in a custom paint engine
  [QTextLayout](qtextlayout.html)                                                                        Used to lay out and render text
  [QTextLayout::FormatRange](qtextlayout-formatrange.html)                                               QTextLayout::FormatRange structure is used to apply extra formatting information for a specified area in the text layout's content
  [QTextLength](qtextlength.html)                                                                        Encapsulates the different types of length used in a QTextDocument
  [QTextLine](qtextline.html)                                                                            Represents a line of text inside a QTextLayout
  [QTextList](qtextlist.html)                                                                            Decorated list of items in a QTextDocument
  [QTextListFormat](qtextlistformat.html)                                                                Formatting information for lists in a QTextDocument
  [QTextObject](qtextobject.html)                                                                        Base class for different kinds of objects that can group parts of a QTextDocument together
  [QTextObjectInterface](qtextobjectinterface.html)                                                      Allows drawing of custom text objects in QTextDocuments
  [QTextOption](qtextoption.html)                                                                        Description of general rich text properties
  [QTextOption::Tab](qtextoption-tab.html)
  [QTextTable](qtexttable.html)                                                                          Represents a table in a QTextDocument
  [QTextTableCell](qtexttablecell.html)                                                                  Represents the properties of a cell in a QTextTable
  [QTextTableCellFormat](qtexttablecellformat.html)                                                      Formatting information for table cells in a QTextDocument
  [QTextTableFormat](qtexttableformat.html)                                                              Formatting information for tables in a QTextDocument
  [QTouchEvent](qtouchevent.html)                                                                        Contains parameters that describe a touch event
  [QTransform](qtransform.html)                                                                          Specifies 2D transformations of a coordinate system
  [QUndoCommand](qundocommand.html)                                                                      The base class of all commands stored on a QUndoStack
  [QUndoGroup](qundogroup.html)                                                                          Group of QUndoStack objects
  [QUndoStack](qundostack.html)                                                                          Stack of QUndoCommand objects
  [QUtiMimeConverter](qutimimeconverter.html)                                                            Converts between a MIME type and a Uniform Type Identifier (UTI) format
  [QValidator](qvalidator.html)                                                                          Validation of input text
  [QVector2D](qvector2d.html)                                                                            Represents a vector or vertex in 2D space
  [QVector3D](qvector3d.html)                                                                            Represents a vector or vertex in 3D space
  [QVector4D](qvector4d.html)                                                                            Represents a vector or vertex in 4D space
  [QVulkanDeviceFunctions](qvulkandevicefunctions.html)                                                  Cross-platform access to the device level core Vulkan 1.3 API
  [QVulkanExtension](qvulkanextension.html)                                                              Represents information about a Vulkan extension
  [QVulkanFunctions](qvulkanfunctions.html)                                                              Cross-platform access to the instance level core Vulkan 1.3 API
  [QVulkanInfoVector](qvulkaninfovector.html)                                                            A specialized QList for QVulkanLayer and QVulkanExtension
  [QVulkanInstance](qvulkaninstance.html)                                                                Represents a native Vulkan instance, enabling Vulkan rendering onto a QSurface
  [QVulkanLayer](qvulkanlayer.html)                                                                      Represents information about a Vulkan layer
  [QVulkanWindow](qvulkanwindow.html)                                                                    Convenience subclass of QWindow to perform Vulkan rendering
  [QVulkanWindowRenderer](qvulkanwindowrenderer.html)                                                    Used to implement the application-specific rendering logic for a QVulkanWindow
  [QWhatsThisClickedEvent](qwhatsthisclickedevent.html)                                                  Event that can be used to handle hyperlinks in a \"What's This?\" text
  [QWheelEvent](qwheelevent.html)                                                                        Contains parameters that describe a wheel event
  [QWindow](qwindow.html)                                                                                Represents a window in the underlying windowing system
  [QWindowStateChangeEvent](qwindowstatechangeevent.html)                                                The window state before a window state change
  [QWindowsMimeConverter](qwindowsmimeconverter.html)                                                    Maps open-standard MIME to Window Clipboard formats

## Detailed Description

The [Qt GUI](qtgui-index.html) page contains information about how to use the module.
