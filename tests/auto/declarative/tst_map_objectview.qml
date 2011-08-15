/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Mobility Components.
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

import QtQuick 2.0
import QtTest 1.0
import Qt.location 5.0
import Qt.location.test 5.0

Item {
    id: masterItem
    width: 200
    height: 350
    // General-purpose elements for the test:
    Plugin { id: testPlugin; name : "nokia"; PluginParameter {name: "mapping.host"; value: "for.nonexistent"}}
    Coordinate{ id: mapDefaultCenter; latitude: 10; longitude: 30}

    MapCircle {
        id: externalCircle
        radius: 2000000
        center: mapDefaultCenter
    }

    TestModel {
        id: testModel
        datatype: 'coordinate'
        datacount: 7
        delay: 0
    }

    TestModel {
        id: testModel2
        datatype: 'coordinate'
        datacount: 3
        delay: 0
    }

    Map {
        id: mapWithPlugin; center: mapDefaultCenter; plugin: testPlugin;
        anchors.fill: parent; size.width: parent.width; size.height: parent.height; zoomLevel: 2
        MapCircle {
            id: internalCircle
            radius: 2000000
            center: mapDefaultCenter
        }
        MapObjectView {
            id: theObjectView
            model: testModel
            delegate: Component {
                MapCircle {
                    radius: 1500000
                    center: Coordinate {
                        latitude: modeldata.coordinate.latitude;
                        longitude: modeldata.coordinate.longitude;
                    }
                }
            }
        }
    }
    Map {
        id: mapWithoutPlugin; center: mapDefaultCenter;
        anchors.fill: parent; size.width: parent.width; size.height: parent.height; zoomLevel: 2
        MapCircle {
            id: internalCircle2
            radius: 2000000
            center: mapDefaultCenter
        }
        MapObjectView {
            id: theObjectView2
            model: testModel
            delegate: Component {
                MapCircle {
                    radius: 1500000
                    center: Coordinate {
                        latitude: modeldata.coordinate.latitude;
                        longitude: modeldata.coordinate.longitude;
                    }
                }
            }
        }
    }

    TestCase {
        name: "MapObjectView"
        function test_a_add_and_remove() {
            // Basic adding and removing of static object
            compare(mapWithPlugin.testGetDeclarativeMapObjectCount(), 8)
            mapWithPlugin.addMapObject(internalCircle)
            compare(mapWithPlugin.testGetDeclarativeMapObjectCount(), 8)
            mapWithPlugin.removeMapObject(internalCircle)
            compare(mapWithPlugin.testGetDeclarativeMapObjectCount(), 7)
            mapWithPlugin.removeMapObject(internalCircle)
            compare(mapWithPlugin.testGetDeclarativeMapObjectCount(), 7)
            // Basic adding and removing of dynamic object
            var dynamicCircle = Qt.createQmlObject( "import QtQuick 2.0; import Qt.location 5.0; MapCircle {radius: 4000; center: mapDefaultCenter}", mapWithPlugin, "");
            mapWithPlugin.addMapObject(dynamicCircle)
            compare(mapWithPlugin.testGetDeclarativeMapObjectCount(), 8)
            mapWithPlugin.removeMapObject(dynamicCircle)
            compare(mapWithPlugin.testGetDeclarativeMapObjectCount(), 7)
            mapWithPlugin.removeMapObject(dynamicCircle)
            compare(mapWithPlugin.testGetDeclarativeMapObjectCount(), 7)
        }

        SignalSpy {id: model1Spy; target: testModel; signalName: "modelChanged"}
        SignalSpy {id: model2Spy; target: testModel2; signalName: "modelChanged"}
        function test_b_model_change() {
            // Change the model of an MapObjectView on the fly
            // and verify that object counts change accordingly.
            compare(mapWithPlugin.testGetDeclarativeMapObjectCount(), 7)
            testModel.datacount += 2
            testModel2.datacount += 1
            compare(mapWithPlugin.testGetDeclarativeMapObjectCount(), 9)
            theObjectView.model = testModel
            compare(mapWithPlugin.testGetDeclarativeMapObjectCount(), 9)
            theObjectView.model = testModel2
            compare(mapWithPlugin.testGetDeclarativeMapObjectCount(), 4)
        }

        SignalSpy {id: pluginChangedSpy; target: mapWithoutPlugin; signalName: "pluginChanged"}
        function test_c_plugin_set_later() {
            compare(mapWithoutPlugin.testGetDeclarativeMapObjectCount(), 0)
            mapWithoutPlugin.plugin = testPlugin
            tryCompare(pluginChangedSpy, "count", 1, 1000)
            compare(mapWithoutPlugin.testGetDeclarativeMapObjectCount(), 10) // 9 from testModel, + 1 from mapcircle
        }
    }
}