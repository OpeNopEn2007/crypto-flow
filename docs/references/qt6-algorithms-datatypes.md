---
source: https://doc.qt.io/qt-6/datastreamformat.html
fetched: 2026-05-12
---

# Serializing Qt Data Types

The [QDataStream](qdatastream.html) class allows you to serialize the Qt data types listed in this section.

It is always best to cast integers to a Qt integer type, such as [qint16](qttypes.html#qint16-typedef) or [quint32](qttypes.html#quint32-typedef), when reading and writing. This ensures that you always know exactly what size integers you are reading and writing, no matter what the underlying platform and architecture the application happens to be running on.

- bool
- [qint8](qttypes.html#qint8-typedef)
- [qint16](qttypes.html#qint16-typedef)
- [qint32](qttypes.html#qint32-typedef)
- [qint64](qttypes.html#qint64-typedef)
- [quint8](qttypes.html#quint8-typedef)
- [quint16](qttypes.html#quint16-typedef)
- [quint32](qttypes.html#quint32-typedef)
- [quint64](qttypes.html#quint64-typedef)
- `float`
- `double`
- `const char *`
- [QBitArray](qbitarray.html)
- [QBrush](qbrush.html)
- [QByteArray](qbytearray.html)
- [QCborArray](qcborarray.html)
- [QCborMap](qcbormap.html)
- [QCborValue](qcborvalue.html)
- [QColor](qcolor.html)
- [QColorSpace](qcolorspace.html)
- [QCursor](qcursor.html)
- [QDate](qdate.html)
- [QDateTime](qdatetime.html)
- [QEasingCurve](qeasingcurve.html)
- [QFont](qfont.html)
- [QGenericMatrix](qgenericmatrix.html)
- [QHash](qhash.html#the-hashing-function)\<Key, T\>
- [QHostAddress](qhostaddress.html)
- [QIcon](qicon.html)
- [QImage](qimage.html)
- [QJsonArray](qjsonarray.html)
- [QJsonDocument](qjsondocument.html)
- [QJsonObject](qjsonobject.html)
- [QJsonValue](qjsonvalue.html)
- [QKeySequence](qkeysequence.html)
- [QLine](qline.html)
- [QLineF](qlinef.html)
- [QList](qlist.html)\<T\>
- [QListWidgetItem](qlistwidgetitem.html)
- [QLocale](qlocale.html)
- [QMap](qmap.html)\<Key, T\>
- [QMargins](qmargins.html)
- [QMarginsF](qmarginsf.html)
- [QMatrix4x4](qmatrix4x4.html)
- [QModelIndex](qmodelindex.html)
- [QModelIndexList](qmodelindex.html#QModelIndexList-typedef)
- [QMultiHash](qmultihash.html)\<Key
- [QMultiMap](qmultimap.html)\<Key
- [QNetworkCacheMetaData](qnetworkcachemetadata.html)
- [QNetworkCacheMetaData::AttributesMap](qnetworkcachemetadata.html#AttributesMap-typedef)
- [QPageRanges](qpageranges.html)
- [QPainterPath](qpainterpath.html)
- std::pair\<T1, T2\>
- [QPalette](qpalette.html)
- [QPen](qpen.html)
- [QPicture](qpicture.html)
- [QPixmap](qpixmap.html)
- [QPoint](qpoint.html)
- [QPointF](qpointf.html)
- [QPolygon](qpolygon.html)
- [QPolygonF](qpolygonf.html)
- [QQuaternion](qquaternion.html)
- [QRect](qrect.html)
- [QRectF](qrectf.html)
- [QRegularExpression](qregularexpression.html)
- [QRegion](qregion.html)
- [QSet](qset.html)
- [QSize](qsize.html)
- [QSizeF](qsizef.html)
- [QSizePolicy](qsizepolicy.html)
- [QStandardItem](qstandarditem.html)
- [QString](qstring.html)
- [QTableWidgetItem](qtablewidgetitem.html)
- [QTextBlockFormat](qtextblockformat.html)
- [QTextCharFormat](qtextcharformat.html)
- [QTextFormat](qtextformat.html)
- [QTextFrameFormat](qtextframeformat.html)
- [QTextLength](qtextlength.html)
- [QTextListFormat](qtextlistformat.html)
- [QTextTableCellFormat](qtexttablecellformat.html)
- [QTimeZone](qtimezone.html)
- [QTime](qtime.html)
- [QTransform](qtransform.html)
- [QTreeWidgetItem](qtreewidgetitem.html)
- [QTypeRevision](qtyperevision.html)
- [QUrl](qurl.html)
- [QUuid](quuid.html)
- [QVariant](qvariant.html)
- [QVector2D](qvector2d.html)
- [QVector3D](qvector3d.html)
- [QVector4D](qvector4d.html)
- [QVersionNumber](qversionnumber.html)

**See also** [JSON Support in Qt](json.html) and [CBOR Support in Qt](cbor.html).

[JSON Support in Qt](json.html){.prevPage} [Container Classes](containers.html){.nextPage}

[©]{.abbr title="Copyright"} 2026 The Qt Company Ltd. Documentation contributions included herein are the copyrights of their respective owners. The documentation provided herein is licensed under the terms of the [GNU Free Documentation License version 1.3](http://www.gnu.org/licenses/fdl.html) as published by the Free Software Foundation. Qt and respective logos are [trademarks](https://doc.qt.io/qt/trademarks.html) of The Qt Company Ltd. in Finland and/or other countries worldwide. All other trademarks are property of their respective owners.

###### **Next**

- [Container Classes](containers.html)

###### **Previous**

- [JSON Support in Qt](json.html)

::::::::::::::::: l-footer
:::::::::::::::: l-footer__container

::: l-footer__logo
[![](/images/qtgroup.svg)](https://www.qt.io/?hsLang=en){.c-logo-footer}

::: c-social-media-links
[](https://twitter.com/qtproject){.fm_button .fm_twitter target="_blank" rel="noopener"} [](https://www.facebook.com/qt/){.fm_button .fm_facebook target="_blank" rel="noopener"} [](https://www.youtube.com/user/QtStudios){.fm_button .fm_youtube target="_blank" rel="noopener"} [](https://www.linkedin.com/company/qtgroup/){.fm_button .fm_linkedin target="_blank" rel="noopener"}

::: l-footer__contact
[Contact Us](https://www.qt.io/contact-us?hsLang=en){.c-btn}

:::: c-footer-navigation

- [Qt Group](javascript:;){aria-haspopup="true" aria-expanded="false" role="menuitem"}
  - [Our Story](https://www.qt.io/group){role="menuitem"}
  - [Brand](https://www.qt.io/brand){role="menuitem"}
  - [News](https://www.qt.io/newsroom){role="menuitem"}
  - [Careers](https://www.qt.io/careers){role="menuitem"}
  - [Investors](https://www.qt.io/investors){role="menuitem"}
  - [Qt Products](https://www.qt.io/product){role="menuitem"}
  - [Software Quality Products](https://www.qt.io/product/quality-assurance){role="menuitem"}
- [Licensing](javascript:;){aria-haspopup="true" aria-expanded="false" role="menuitem"}
  - [License Agreement](https://www.qt.io/terms-conditions){role="menuitem"}
  - [Open Source](https://www.qt.io/licensing/open-source-lgpl-obligations){role="menuitem"}
  - [Plans and pricing](https://www.qt.io/pricing){role="menuitem"}
  - [Download](https://www.qt.io/download){role="menuitem"}
  - [FAQ](https://www.qt.io/faq/overview){role="menuitem"}
- [Learn Qt](javascript:;){aria-haspopup="true" aria-expanded="false" role="menuitem"}
  - [For Learners](https://www.qt.io/academy){role="menuitem"}
  - [For Students and Teachers](https://www.qt.io/qt-educational-license){role="menuitem"}
  - [Qt Documentation](https://doc.qt.io/){role="menuitem" target="_blank" rel="noopener"}
  - [Qt Forum](https://forum.qt.io/){role="menuitem" target="_blank" rel="noopener"}
- [Support & Services](javascript:;){aria-haspopup="true" aria-expanded="false" role="menuitem"}
  - [Professional Services](https://www.qt.io/qt-professional-services){role="menuitem"}
  - [Customer Success](https://www.qt.io/customer-success){role="menuitem"}
  - [Support Services](https://www.qt.io/qt-support/){role="menuitem"}
  - [Partners](https://www.qt.io/contact-us/partners){role="menuitem"}
  - [Qt World](https://www.qt.io/qt-world){role="menuitem" target="_blank" rel="noopener"}

::::::: l-footer__row

:::: c-footer-secondary-navigation

- [© 2026 The Qt Company](javascript:;){role="menuitem"}
- [Feedback](mailto:feedback@qt.io?Subject=Feedback%20about%20doc.qt.io%20site)

Qt Group includes The Qt Company Oy and its global subsidiaries and affiliates.
