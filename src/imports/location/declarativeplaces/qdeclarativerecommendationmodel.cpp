/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the QtLocation module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qdeclarativerecommendationmodel_p.h"
#include "qdeclarativeplace_p.h"

#include <QtDeclarative/QDeclarativeInfo>
#include <QtLocation/QGeoServiceProvider>
#include <QtLocation/QPlaceManager>

#include <qplacesearchreply.h>

QT_USE_NAMESPACE

/*!
    \qmlclass RecommendationModel QDeclarativeRecommenadationModel
    \brief The RecommendationModel element provides access to place recommendations.
    \inherits QAbstractListModel
    \ingroup qml-places

    RecommendationModel provides a model of place recommendation results within the \l searchArea,
    that are similiar to the place identified by the \l placeId property.

    The \l executing property indicates whether a query is currently executing.

    The model returns data for the following roles:

    \table
        \header
            \o Role
            \o Type
            \o Description
        \row
            \o distance
            \o real
            \o The distance to the place.
        \row
            \o place
            \o Place
            \o The Place.
    \endtable

    The following example shows how to use the RecommendationModel to search for recommendations
    within a 5km radius:

    \code
    import QtLocation 5.0

    Place {
        id: place
        ...
    }

    RecommendationModel {
        id: recommendationModel

        placeId: place.placeId
        searchArea: BoundingCircle {
            center: Coordinate {
                longitude: 53
                latitude: 100
            }
            radius:5000
        }
    }

    ...
    resultModel.execute()
    ...

    ListView {
        model: recommendationModel
        delegate: Text { text: 'Name: ' + place.name }
    }
    \endcode

    \sa SearchResultModel, SupportedCategoryModel, {QPlaceManager}
*/

/*!
    \qmlproperty GeoCoordinate RecommendationModel::searchArea

    This element holds the search area.

    Note: this property's changed() signal is currently emitted only if the
    whole element changes, not if only the contents of the element change.
*/

/*!
    \qmlproperty int RecommendationModel::offset

    This element holds offset for items that would be returned.
    Less then 0 means that it is undefined.
*/

/*!
    \qmlproperty int RecommendationModel::limit

    This element holds limit of items that would be returned.
    Less then -1 means that limit is undefined.
*/

/*!
    \qmlproperty bool RecommendationModel::executing

    This property indicates whether a search query is currently being executed.
*/



/*!
    \qmlmethod RecommendationModel::execute()
    Parameter placeId should contain string for which search should be
    started.
    Updates the items represented by the model from the underlying proivider.
*/


/*!
    \qmlmethod RecommendationModel::cancel()
    Cancels ongoing request.
*/

QDeclarativeRecommendationModel::QDeclarativeRecommendationModel(QObject *parent)
:   QDeclarativeSearchModelBase(parent)
{
    QHash<int, QByteArray> roles = roleNames();
    roles.insert(DistanceRole, "distance");
    roles.insert(PlaceRole, "place");
    setRoleNames(roles);
}

QDeclarativeRecommendationModel::~QDeclarativeRecommendationModel()
{
}

/*!
    \qmlproperty string RecommendationModel::placeId

    This element holds place id used in query.
*/

QString QDeclarativeRecommendationModel::placeId() const
{
    return m_request.searchTerm();
}

void QDeclarativeRecommendationModel::setPlaceId(const QString &placeId)
{
    if (m_request.searchTerm() == placeId)
        return;

    m_request.setSearchTerm(placeId);
    emit placeIdChanged();
}

void QDeclarativeRecommendationModel::clearData()
{
    qDeleteAll(m_places);
    m_places.clear();
    m_results.clear();
}

void QDeclarativeRecommendationModel::updateSearchRequest()
{
    QDeclarativeSearchModelBase::updateSearchRequest();
}

void QDeclarativeRecommendationModel::processReply(QPlaceReply *reply)
{
    QPlaceSearchReply *searchReply = qobject_cast<QPlaceSearchReply *>(reply);
    if (!searchReply)
        return;

    m_results = searchReply->results();

    foreach (const QPlaceSearchResult &result, m_results) {
        QDeclarativePlace *place = new QDeclarativePlace(result.place(), this);
        place->setPlugin(plugin());
        m_places.insert(result.place().placeId(), place);
    }
}

int QDeclarativeRecommendationModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return m_results.count();
}

QVariant QDeclarativeRecommendationModel::data(const QModelIndex& index, int role) const
{
    if (index.row() > m_results.count())
        return QVariant();

    const QPlaceSearchResult &result = m_results.at(index.row());

    if (result.type() != QPlaceSearchResult::PlaceResult)
        return QVariant();

    switch (role) {
    case Qt::DisplayRole:
        return result.place().name();
    case DistanceRole:
        return result.distance();
    case PlaceRole:
        return QVariant::fromValue(static_cast<QObject *>(m_places.value(result.place().placeId())));
    default:
        return QVariant();
    }

    return QVariant();
}

QPlaceReply *QDeclarativeRecommendationModel::sendQuery(QPlaceManager *manager,
                                                        const QPlaceSearchRequest &request)
{
    QPlace target;
    target.setPlaceId(request.searchTerm());
    return manager->recommendations(target, request);
}
