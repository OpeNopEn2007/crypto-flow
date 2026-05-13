# Qt WebEngine Core

> **来源**: [https://doc.qt.io/qt-6/qtwebenginecore-module.html](https://doc.qt.io/qt-6/qtwebenginecore-module.html)
> **获取日期**: 2026-05-12
> **描述**: Web 引擎核心模块，提供 QtWebEngineQuick 和 QtWebEngineWidgets 共享的公共 API

# Qt WebEngine Core C++ Classes

Provides public API shared by both [QtWebEngineQuick](qtwebenginequick-module.html) and [QtWebEngineWidgets](qtwebenginewidgets-module.html).

## Namespaces

  [QWebEngineGlobalSettings](qwebengineglobalsettings.html)   Holds global settings of the web engine

## Classes

  [QWebEngineCertificateError](qwebenginecertificateerror.html)                               Information about a certificate error
  [QWebEngineClientCertificateSelection](qwebengineclientcertificateselection.html)           QWebEngineClientCertSelection class wraps a client certificate selection
  [QWebEngineClientCertificateStore](qwebengineclientcertificatestore.html)                   In-memory store for client certificates
  [QWebEngineClientHints](qwebengineclienthints.html)                                         Object to customize User-Agent Client Hints used by a profile
  [QWebEngineContextMenuRequest](qwebenginecontextmenurequest.html)                           Request for populating or extending a context menu with actions
  [QWebEngineCookieStore](qwebenginecookiestore.html)                                         Access to Chromium's cookies
  [QWebEngineCookieStore::FilterRequest](qwebenginecookiestore-filterrequest.html)            QWebEngineCookieStore::FilterRequest struct is used in conjunction with QWebEngineCookieStore::setCookieFilter() and is the type filterCallback operates on
  [QWebEngineDesktopMediaRequest](qwebenginedesktopmediarequest.html)                         A request for populating a dialog with available sources for screen capturing
  [QWebEngineDownloadRequest](qwebenginedownloadrequest.html)                                 Information about a download
  [QWebEngineExtensionInfo](qwebengineextensioninfo.html)                                     Information about a browser extension
  [QWebEngineExtensionManager](qwebengineextensionmanager.html)                               Allows applications to install and load Chrome extensions from the filesystem
  [QWebEngineFileSystemAccessRequest](qwebenginefilesystemaccessrequest.html)                 Enables accepting or rejecting requests for local file system access from JavaScript applications
  [QWebEngineFindTextResult](qwebenginefindtextresult.html)                                   Encapsulates the result of a string search on a page
  [QWebEngineFrame](qwebengineframe.html)                                                     Gives information about and control over a page frame
  [QWebEngineFullScreenRequest](qwebenginefullscreenrequest.html)                             Enables accepting or rejecting requests for entering and exiting the fullscreen mode
  [QWebEngineGlobalSettings::DnsMode](qwebengineglobalsettings-dnsmode.html)                  Struct provides means to specify the DNS host resolution mode
  [QWebEngineHistory](qwebenginehistory.html)                                                 Represents the history of a web engine page
  [QWebEngineHistoryItem](qwebenginehistoryitem.html)                                         Represents one item in the history of a web engine page
  [QWebEngineHistoryModel](qwebenginehistorymodel.html)                                       A data model that represents the history of a web engine page
  [QWebEngineHttpRequest](qwebenginehttprequest.html)                                         Holds a request to be sent with WebEngine
  [QWebEngineLoadingInfo](qwebengineloadinginfo.html)                                         A utility type for the WebEngineView::loadingChanged signal
  [QWebEngineNavigationRequest](qwebenginenavigationrequest.html)                             A utility type for the QWebEnginePage::navigationRequested signal
  [QWebEngineNewWindowRequest](qwebenginenewwindowrequest.html)                               A utility type for the QWebEnginePage::newWindowRequested() signal
  [QWebEngineNotification](qwebenginenotification.html)                                       Encapsulates the data of an HTML5 web notification
  [QWebEnginePage](qwebenginepage.html)                                                       Object to view and edit web documents
  [QWebEnginePermission](qwebenginepermission.html)                                           A QWebEnginePermission is an object used to access and modify the state of a single permission that's been granted or denied to a specific origin URL
  [QWebEngineProfile](qwebengineprofile.html)                                                 Web engine profile shared by multiple pages
  [QWebEngineProfileBuilder](qwebengineprofilebuilder.html)                                   Way to construct QWebEngineProfile
  [QWebEngineRegisterProtocolHandlerRequest](qwebengineregisterprotocolhandlerrequest.html)   Enables accepting or rejecting requests from the registerProtocolHandler API
  [QWebEngineScript](qwebenginescript.html)                                                   Encapsulates a JavaScript program
  [QWebEngineScriptCollection](qwebenginescriptcollection.html)                               Represents a collection of user scripts
  [QWebEngineSettings](qwebenginesettings.html)                                               Object to store the settings used by QWebEnginePage
  [QWebEngineUrlRequestInfo](qwebengineurlrequestinfo.html)                                   Information about URL requests
  [QWebEngineUrlRequestInterceptor](qwebengineurlrequestinterceptor.html)                     Abstract base class for URL interception
  [QWebEngineUrlRequestJob](qwebengineurlrequestjob.html)                                     Represents a custom URL request
  [QWebEngineUrlScheme](qwebengineurlscheme.html)                                             Configures a custom URL scheme
  [QWebEngineUrlSchemeHandler](qwebengineurlschemehandler.html)                               Base class for handling custom URL schemes
  [QWebEngineWebAuthPinRequest](qwebenginewebauthpinrequest.html)                             Encapsulates the data of a PIN WebAuth UX request
  [QWebEngineWebAuthUxRequest](qwebenginewebauthuxrequest.html)                               Encapsulates the data of a WebAuth UX request

## Detailed Description

If you use qmake to build your projects, Qt WebEngine Core is usually indirectly included through the [Qt WebEngine Quick](qtwebengine-qmlmodule.html) or [Qt WebEngine Widgets](qtwebenginewidgets-module.html) modules.

To link against the module using build with qmake, add this line to your qmake project file:

```
QT += webenginecore
```

However, `webenginecore` is implied by adding `webenginequick` or `webenginewidgets`.

For build with CMake use the `find_package()` command to locate the needed module components in the Qt6 package and `target_link_libraries()` to link against the module:

```
find_package(Qt6 REQUIRED COMPONENTS WebEngineCore)
target_link_libraries(target PRIVATE Qt6::WebEngineCore)
```
