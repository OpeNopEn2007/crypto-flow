# Qt XML Module

> **Source**: <https://doc.qt.io/qt-6/qtxml-module.html>
> **Fetched**: 2026-05-12

---

# Qt XML C++ Classes

The Qt XML module provides a C++ implementation of the DOM standard for XML. [More\...](#details)

## Classes

  [QDomAttr](qdomattr.html)                                     Represents one attribute of a QDomElement
  [QDomCDATASection](qdomcdatasection.html)                     Represents an XML CDATA section
  [QDomCharacterData](qdomcharacterdata.html)                   Represents a generic string in the DOM
  [QDomComment](qdomcomment.html)                               Represents an XML comment
  [QDomDocument](qdomdocument.html)                             Represents an XML document
  [QDomDocument::ParseResult](qdomdocument-parseresult.html)    Struct is used to store the result of QDomDocument::setContent()
  [QDomDocumentFragment](qdomdocumentfragment.html)             Tree of QDomNodes which is not usually a complete QDomDocument
  [QDomDocumentType](qdomdocumenttype.html)                     The representation of the DTD in the document tree
  [QDomElement](qdomelement.html)                               Represents one element in the DOM tree
  [QDomEntity](qdomentity.html)                                 Represents an XML entity
  [QDomEntityReference](qdomentityreference.html)               Represents an XML entity reference
  [QDomImplementation](qdomimplementation.html)                 Information about the features of the DOM implementation
  [QDomNamedNodeMap](qdomnamednodemap.html)                     Contains a collection of nodes that can be accessed by name
  [QDomNode](qdomnode.html)                                     The base class for all the nodes in a DOM tree
  [QDomNodeList](qdomnodelist.html)                             List of QDomNode objects
  [QDomNotation](qdomnotation.html)                             Represents an XML notation
  [QDomProcessingInstruction](qdomprocessinginstruction.html)   Represents an XML processing instruction
  [QDomText](qdomtext.html)                                     Represents text data in the parsed XML document

## Detailed Description

The [Qt XML](qtxml-index.html) page contains information about how to use the module.

**Note:** Qt XML will no longer receive additional features. For reading or writing XML documents iteratively (SAX), use the [QXmlStreamReader](qxmlstreamreader.html) and [QXmlStreamWriter](qxmlstreamwriter.html) classes. The classes are both easier to use and more compliant with the XML standard.
