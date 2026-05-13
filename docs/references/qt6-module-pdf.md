---
source: https://doc.qt.io/qt-6/qtpdf-module.html
fetched: 2026-05-12
---

# Qt PDF C++ Classes

Renders pages from PDF documents. [More\...](#details)

## Classes

  [QPdfBookmarkModel](qpdfbookmarkmodel.html)                   Holds a tree of links (anchors) within a PDF document, such as the table of contents
  [QPdfDocument](qpdfdocument.html)                             Loads a PDF document and renders pages from it
  [QPdfDocumentRenderOptions](qpdfdocumentrenderoptions.html)   Holds the options to render a page from a PDF document
  [QPdfLink](qpdflink.html)                                     Defines a link between a region on a page (such as a hyperlink or a search result) and a destination (page, location on the page, and zoom level at which to view it)
  [QPdfLinkModel](qpdflinkmodel.html)                           Holds the geometry and the destination for each link that the specified page contains
  [QPdfPageNavigator](qpdfpagenavigator.html)                   Navigation history within a PDF document
  [QPdfPageRenderer](qpdfpagerenderer.html)                     Encapsulates the rendering of pages of a PDF document
  [QPdfPageSelector](qpdfpageselector.html)                     A widget for selecting a PDF page
  [QPdfSearchModel](qpdfsearchmodel.html)                       Searches for a string in a PDF document and holds the results
  [QPdfSelection](qpdfselection.html)                           Defines a range of text that has been selected on one page in a PDF document, and its geometric boundaries
  [QPdfView](qpdfview.html)                                     A PDF viewer widget

## Detailed Description

The Qt PDF module contains classes and functions for rendering PDF documents.

To link against the module, add this line to your qmake project file:

```cpp
QT += pdf
```
