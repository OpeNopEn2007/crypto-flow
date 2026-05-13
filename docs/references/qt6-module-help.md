# Qt Help Module

> **Source**: <https://doc.qt.io/qt-6/qthelp-module.html>
> **Fetched**: 2026-05-12

---

# Qt Help C++ Classes

Provides classes for integrating online documentation in applications. [More\...](#details)

## Classes

  [QCompressedHelpInfo](qcompressedhelpinfo.html)               Access to the details about a compressed help file
  [QHelpContentItem](qhelpcontentitem.html)                     Item for use with QHelpContentModel
  [QHelpContentModel](qhelpcontentmodel.html)                   Model that supplies content to views
  [QHelpContentWidget](qhelpcontentwidget.html)                 Tree view for displaying help content model items
  [QHelpEngine](qhelpengine.html)                               Access to contents and indices of the help engine
  [QHelpEngineCore](qhelpenginecore.html)                       The core functionality of the help system
  [QHelpFilterData](qhelpfilterdata.html)                       Details for the filters used by QHelpFilterEngine
  [QHelpFilterEngine](qhelpfilterengine.html)                   Filtered view of the help contents
  [QHelpFilterSettingsWidget](qhelpfiltersettingswidget.html)   Widget that allows for creating, editing and removing filters
  [QHelpIndexModel](qhelpindexmodel.html)                       Model that supplies index keywords to views
  [QHelpIndexWidget](qhelpindexwidget.html)                     List view displaying the QHelpIndexModel
  [QHelpLink](qhelplink.html)                                   Struct provides the data associated with a help link
  [QHelpSearchEngine](qhelpsearchengine.html)                   Access to widgets reusable to integrate fulltext search as well as to index and search documentation
  [QHelpSearchEngineCore](qhelpsearchenginecore.html)           Access to index and search documentation
  [QHelpSearchQueryWidget](qhelpsearchquerywidget.html)         Simple line edit or an advanced widget to enable the user to input a search term in a standardized input mask
  [QHelpSearchResult](qhelpsearchresult.html)                   The data associated with the search result
  [QHelpSearchResultWidget](qhelpsearchresultwidget.html)       Text browser to display search results

## Detailed Description

The classes and tools supplied with Qt to enable developers to include online help and documentation in their applications are described in more detail in [The Qt Help Framework](qthelp-framework.html) overview document.

To link against the module, add this line to your [qmake](qmake-manual.html) `.pro` file:

``` cpp
QT += help
```
