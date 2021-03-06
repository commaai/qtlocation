/****************************************************************************
**
** Copyright (C) 2016 Vlad Seryakov <vseryakov@gmail.com>
** Copyright (C) 2016 Aaron McCarthy <mccarthy.aaron@gmail.com>
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtLocation module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QGEOROUTINGMANAGERENGINEMAPBOX_H
#define QGEOROUTINGMANAGERENGINEMAPBOX_H

#include <QtLocation/QGeoServiceProvider>
#include <QtLocation/QGeoRoutingManagerEngine>

QT_BEGIN_NAMESPACE

class QNetworkAccessManager;
class QGeoRouteParser;

class QGeoRoutingManagerEngineMapbox : public QGeoRoutingManagerEngine
{
    Q_OBJECT

public:
    QGeoRoutingManagerEngineMapbox(const QVariantMap &parameters,
                                QGeoServiceProvider::Error *error,
                                QString *errorString);
    ~QGeoRoutingManagerEngineMapbox();

    QGeoRouteReply *calculateRoute(const QGeoRouteRequest &request);
    const QGeoRouteParser *routeParser() const;

private Q_SLOTS:
    void replyFinished();
    void replyError(QGeoRouteReply::Error errorCode, const QString &errorString);

private:
    QNetworkAccessManager *m_networkManager;
    QByteArray m_userAgent;
    QString m_accessToken;
    QString m_api_url;
    bool m_useMapboxText = false;
    QGeoRouteParser *m_routeParser = nullptr;
};

QT_END_NAMESPACE

#endif // QGEOROUTINGMANAGERENGINEOSM_H

