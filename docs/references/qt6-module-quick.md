---
source: https://doc.qt.io/qt-6/qtquick-module.html
fetched: 2026-05-12
---

# Qt Quick C++ Classes

The Qt Quick module provides classes for embedding Qt Quick in Qt/C++ applications. [More\...](#details)

## Namespaces

  [QQuickOpenGLUtils](qquickopenglutils.html)   Contains utilities for Qt Quick when used with an OpenGL backend

## Classes

  [QNativeInterface::QSGD3D11Texture](qnativeinterface-qsgd3d11texture.html)                 Provides access to and enables adopting Direct3D 11 texture objects
  [QNativeInterface::QSGD3D12Texture](qnativeinterface-qsgd3d12texture.html)                 Provides access to and enables adopting Direct3D 12 texture objects
  [QNativeInterface::QSGMetalTexture](qnativeinterface-qsgmetaltexture.html)                 Provides access to and enables adopting Metal texture objects
  [QNativeInterface::QSGOpenGLTexture](qnativeinterface-qsgopengltexture.html)               Provides access to and enables adopting OpenGL texture objects
  [QNativeInterface::QSGVulkanTexture](qnativeinterface-qsgvulkantexture.html)               Provides access to and enables adopting Vulkan image objects
  [QQuickAsyncImageProvider](qquickasyncimageprovider.html)                                  Interface for asynchronous control of QML image requests
  [QQuickFramebufferObject](qquickframebufferobject.html)                                    Convenience class for integrating OpenGL rendering using a framebuffer object (FBO) with Qt Quick
  [QQuickFramebufferObject::Renderer](qquickframebufferobject-renderer.html)
  [QQuickGraphicsConfiguration](qquickgraphicsconfiguration.html)                            Controls lower level graphics settings for the QQuickWindow
  [QQuickGraphicsDevice](qquickgraphicsdevice.html)                                          Opaque container for native graphics objects representing graphics devices or contexts
  [QQuickImageProvider](qquickimageprovider.html)                                            Interface for supporting pixmaps and threaded image requests in QML
  [QQuickImageResponse](qquickimageresponse.html)                                            Interface for asynchronous image loading in QQuickAsyncImageProvider
  [QQuickItem](qquickitem.html)                                                              The most basic of all visual items in Qt Quick
  [QQuickItem::ItemChangeData](qquickitem-itemchangedata.html)                               Adds supplementary information to the QQuickItem::itemChange() function
  [QQuickItemGrabResult](qquickitemgrabresult.html)                                          Contains the result from QQuickItem::grabToImage()
  [QQuickPaintedItem](qquickpainteditem.html)                                                Way to use the QPainter API in the QML Scene Graph
  [QQuickRenderControl](qquickrendercontrol.html)                                            Mechanism for rendering the Qt Quick scenegraph onto an offscreen render target in a fully application-controlled manner
  [QQuickRenderTarget](qquickrendertarget.html)                                              Opaque container for native graphics resources specifying a render target, and associated metadata
  [QQuickRhiItem](qquickrhiitem.html)                                                        Portable alternative to QQuickFramebufferObject that is not tied to OpenGL, but rather allows integrating rendering with the QRhi APIs with Qt Quick
  [QQuickRhiItemRenderer](qquickrhiitemrenderer.html)                                        A QQuickRhiItemRenderer implements the rendering logic of a QQuickRhiItem
  [QQuickTextDocument](qquicktextdocument.html)                                              Access to the QTextDocument of QQuickTextEdit
  [QQuickTextureFactory](qquicktexturefactory.html)                                          Interface for loading custom textures from QML
  [QQuickView](qquickview.html)                                                              Window for displaying a Qt Quick user interface
  [QQuickWindow](qquickwindow.html)                                                          The window for displaying a graphical QML scene
  [QQuickWindow::GraphicsStateInfo](qquickwindow-graphicsstateinfo.html)                     Describes some of the RHI's graphics state at the point of a beginExternalCommands() call
  [QSGBasicGeometryNode](qsgbasicgeometrynode.html)                                          Serves as a baseclass for geometry based nodes
  [QSGClipNode](qsgclipnode.html)                                                            Implements the clipping functionality in the scene graph
  [QSGDynamicTexture](qsgdynamictexture.html)                                                Serves as a baseclass for dynamically changing textures, such as content that is rendered to FBO's
  [QSGFlatColorMaterial](qsgflatcolormaterial.html)                                          Convenient way of rendering solid colored geometry in the scene graph
  [QSGGeometry](qsggeometry.html)                                                            Low-level storage for graphics primitives in the Qt Quick Scene Graph
  [QSGGeometry::Attribute](qsggeometry-attribute.html)                                       QSGGeometry::Attribute describes a single vertex attribute in a QSGGeometry
  [QSGGeometry::AttributeSet](qsggeometry-attributeset.html)                                 QSGGeometry::AttributeSet describes how the vertices in a QSGGeometry are built up
  [QSGGeometry::ColoredPoint2D](qsggeometry-coloredpoint2d.html)                             QSGGeometry::ColoredPoint2D struct is a convenience struct for accessing 2D Points with a color
  [QSGGeometry::Point2D](qsggeometry-point2d.html)                                           QSGGeometry::Point2D struct is a convenience struct for accessing 2D Points
  [QSGGeometry::TexturedPoint2D](qsggeometry-texturedpoint2d.html)                           QSGGeometry::TexturedPoint2D struct is a convenience struct for accessing 2D Points with texture coordinates
  [QSGGeometryNode](qsggeometrynode.html)                                                    Used for all rendered content in the scene graph
  [QSGImageNode](qsgimagenode.html)                                                          Provided for convenience to easily draw textured content using the QML scene graph
  [QSGMaterial](qsgmaterial.html)                                                            Encapsulates rendering state for a shader program
  [QSGMaterialShader](qsgmaterialshader.html)                                                Represents a graphics API independent shader program
  [QSGMaterialShader::GraphicsPipelineState](qsgmaterialshader-graphicspipelinestate.html)   Describes state changes that the material wants to apply to the currently active graphics pipeline state
  [QSGMaterialType](qsgmaterialtype.html)                                                    Used as a unique type token in combination with QSGMaterial
  [QSGNode](qsgnode.html)                                                                    The base class for all nodes in the scene graph
  [QSGOpacityNode](qsgopacitynode.html)                                                      Used to change opacity of nodes
  [QSGOpaqueTextureMaterial](qsgopaquetexturematerial.html)                                  Convenient way of rendering textured geometry in the scene graph
  [QSGRectangleNode](qsgrectanglenode.html)                                                  Convenience class for drawing solid filled rectangles using scenegraph
  [QSGRenderNode](qsgrendernode.html)                                                        Represents a set of custom rendering commands targeting the graphics API that is in use by the scenegraph
  [QSGRenderNode::RenderState](qsgrendernode-renderstate.html)                               Provides information about the projection matrix and clipping
  [QSGRendererInterface](qsgrendererinterface.html)                                          An interface providing access to some of the graphics API specific internals of the scenegraph
  [QSGSimpleRectNode](qsgsimplerectnode.html)                                                Convenience class for drawing solid filled rectangles using scenegraph
  [QSGSimpleTextureNode](qsgsimpletexturenode.html)                                          Provided for convenience to easily draw textured content using the QML scene graph
  [QSGTextNode](qsgtextnode.html)                                                            Class for drawing text layouts and text documents in the Qt Quick scene graph
  [QSGTexture](qsgtexture.html)                                                              The base class for textures used in the scene graph
  [QSGTextureMaterial](qsgtexturematerial.html)                                              Convenient way of rendering textured geometry in the scene graph
  [QSGTextureProvider](qsgtextureprovider.html)                                              Encapsulates texture based entities in QML
  [QSGTransformNode](qsgtransformnode.html)                                                  Implements transformations in the scene graph
  [QSGVertexColorMaterial](qsgvertexcolormaterial.html)                                      Convenient way of rendering per-vertex colored geometry in the scene graph

## Detailed Description

To link against the module, add this line to your [qmake](qmake-manual.html) `.pro` file:

``` 
QT += quick
```

For more information on the Qt Quick module, see the [Qt Quick](qtquick-index.html) module documentation.
