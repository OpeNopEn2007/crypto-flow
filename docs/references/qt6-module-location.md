# Qt 6 - Location Module

> **来源**: https://doc.qt.io/qt-6/qtlocation-module.html
> **获取日期**: 2026-05-12

# Qt Location C++ Classes

Provides C++ interfaces to retrieve location and navigational information. [More\...](#details)

## Namespaces(#namespaces "Direct link to this headline")

  [QLocation](qlocation.html)   Contains miscellaneous identifiers used throughout the QtLocation module

## Classes(#classes "Direct link to this headline")

  [QGeoCodeReply](qgeocodereply.html)                               Manages an operation started by an instance of QGeoCodingManager
  [QGeoCodingManager](qgeocodingmanager.html)                       Support for geocoding operations
  [QGeoCodingManagerEngine](qgeocodingmanagerengine.html)           Interface and convenience methods to implementers of QGeoServiceProvider plugins who want to provide support for geocoding operations
  [QGeoManeuver](qgeomaneuver.html)                                 Represents the information relevant to the point at which two QGeoRouteSegments meet
  [QGeoRoute](qgeoroute.html)                                       Represents a route between two points
  [QGeoRouteReply](qgeoroutereply.html)                             Manages an operation started by an instance of QGeoRoutingManager
  [QGeoRouteRequest](qgeorouterequest.html)                         Represents the parameters and restrictions which define a request for routing information
  [QGeoRouteSegment](qgeoroutesegment.html)                         Represents a segment of a route
  [QGeoRoutingManager](qgeoroutingmanager.html)                     Support for geographic routing operations
  [QGeoRoutingManagerEngine](qgeoroutingmanagerengine.html)         Interface and convenience methods to implementers of QGeoServiceProvider plugins who want to provide access to geographic routing information
  [QGeoServiceProvider](qgeoserviceprovider.html)                   Aggregates access to services which provide geographical information
  [QPlace](qplace.html)                                             Represents a set of data about a place
  [QPlaceAttribute](qplaceattribute.html)                           Represents generic attribute information about a place
  [QPlaceCategory](qplacecategory.html)                             Represents a category that a QPlace can be associated with
  [QPlaceContactDetail](qplacecontactdetail.html)                   Represents a contact detail such as a phone number or website url
  [QPlaceContent](qplacecontent.html)                               Holds content about places
  [QPlaceContentReply](qplacecontentreply.html)                     Manages a content retrieval operation started by an instance of QPlaceManager
  [QPlaceContentRequest](qplacecontentrequest.html)                 Represents the parameters of a content request
  [QPlaceDetailsReply](qplacedetailsreply.html)                     Manages a place details fetch operation started by an instance of QPlaceManager
  [QPlaceIcon](qplaceicon.html)                                     Represents an icon
  [QPlaceIdReply](qplaceidreply.html)                               Manages operations which return an identifier such as saving and removal operations of places and categories
  [QPlaceManager](qplacemanager.html)                               The interface which allows clients to access places stored in a particular backend
  [QPlaceManagerEngine](qplacemanagerengine.html)                   Interface for implementers of QGeoServiceProvider plugins who want to provide access to place functionality
  [QPlaceMatchReply](qplacematchreply.html)                         Manages a place matching operation started by an instance of QPlaceManager
  [QPlaceMatchRequest](qplacematchrequest.html)                     Used to find places from one manager that match those from another. It represents a set of request parameters
  [QPlaceProposedSearchResult](qplaceproposedsearchresult.html)     Represents a search result containing a proposed search
  [QPlaceRatings](qplaceratings.html)                               Holds rating information about a place
  [QPlaceReply](qplacereply.html)                                   Manages an operation started by an instance of QPlaceManager and serves as a base class for more specialized replies
  [QPlaceResult](qplaceresult.html)                                 Represents a search result containing a place
  [QPlaceSearchReply](qplacesearchreply.html)                       Manages a place search operation started by an instance of QPlaceManager
  [QPlaceSearchRequest](qplacesearchrequest.html)                   Represents the set of parameters for a search request
  [QPlaceSearchResult](qplacesearchresult.html)                     The base class for search results
  [QPlaceSearchSuggestionReply](qplacesearchsuggestionreply.html)   Manages a search suggestion operation started by an instance of QPlaceManager
  [QPlaceUser](qplaceuser.html)                                     Represents an individual user

## Detailed Description(#details "Direct link to this headline")

The C++ API provides access to geocoding and navigation information, and also place search. Use the [QML](location-maps-qml.html) API to render this information on an interactive map that supports touch gestures, overlays, and so on.

Include the appropriate header in your C++ code. For example, applications using routes can include:

``` {.cpp .prettyprint translate="no"}
#include
```

Add the *location* keyword in the project file to link against the Qt Location library:

``` {.cpp .plain translate="no"}
QT += location
```

See more in the [Qt Location Overview](qtlocation-index.html).
