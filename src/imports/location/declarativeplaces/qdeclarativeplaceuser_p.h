/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
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
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QDECLARATIVEPLACEUSER_P_H
#define QDECLARATIVEPLACEUSER_P_H

#include <QtCore/QObject>
#include <QtQml/qqml.h>
#include <QtLocation/QPlaceUser>

QT_BEGIN_NAMESPACE

class QDeclarativePlaceUser : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QPlaceUser user READ user WRITE setUser)
    Q_PROPERTY(QString userId READ userId WRITE setUserId NOTIFY userIdChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)

public:
    explicit QDeclarativePlaceUser(QObject *parent = 0);
    explicit QDeclarativePlaceUser(const QPlaceUser &src, QObject *parent = 0);
    ~QDeclarativePlaceUser();

    QPlaceUser user() const;
    void setUser(const QPlaceUser &src);

    QString userId() const;
    void setUserId(const QString &id);

    QString name() const;
    void setName(const QString &name);

Q_SIGNALS:
    void userIdChanged();
    void nameChanged();

private:
    QPlaceUser m_user;
};

QT_END_NAMESPACE

QML_DECLARE_TYPE(QDeclarativePlaceUser)

#endif
