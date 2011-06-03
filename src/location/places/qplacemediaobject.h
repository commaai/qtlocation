#ifndef QPLACEMEDIAOBJECT_H
#define QPLACEMEDIAOBJECT_H

#include <QSharedDataPointer>
#include <QString>
#include "qmobilityglobal.h"
#include "qplacesupplier.h"

QTM_BEGIN_NAMESPACE

class QPlaceMediaObjectPrivate;

class Q_LOCATION_EXPORT QPlaceMediaObject
{
public:
    QPlaceMediaObject();
    QPlaceMediaObject(const QPlaceMediaObject &other);

    virtual ~QPlaceMediaObject();

    QPlaceMediaObject &operator=(const QPlaceMediaObject &other);

    bool operator==(const QPlaceMediaObject &other) const;
    bool operator!=(const QPlaceMediaObject &other) const {
        return !(other == *this);
    }

    QString url() const;
    void setUrl(const QString &data);
    QString thumbnailUrl() const;
    void setThumbnailUrl(const QString &data);
    QString id() const;
    void setId(const QString &data);
    QString metaInfo() const;
    void setMetaInfo(const QString &data);
    QString mimeType() const;
    void setMimeType(const QString &data);
    QPlaceSupplier supplier() const;
    void setSupplier(const QPlaceSupplier &data);

private:
    QSharedDataPointer<QPlaceMediaObjectPrivate> d;
};

QTM_END_NAMESPACE

#endif // QPLACEMEDIAOBJECT_H