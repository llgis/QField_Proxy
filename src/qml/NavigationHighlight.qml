import QtQuick 2.12

import org.qgis 1.0
import org.qfield 1.0

import Theme 1.0

Item {
  id: navigationHighlight
  property Navigation navigation

  LinePolygonHighlight {
    visible: positionSource.active
    mapSettings: navigation.mapSettings
    geometry:   QgsGeometryWrapper {
      qgsGeometry: navigation.path
      crs: navigation.mapSettings.crs
    }
    color: Theme.navigationColorSemiOpaque
    width: positionSource.active && positionSource.positionInfo && positionSource.positionInfo.latitudeValid ? 8 : 3
  }

  Repeater {
    model: navigation.model

    delegate: NavigationRenderer {
      mapSettings: navigation.mapSettings
      geometryWrapper.qgsGeometry: model.Point
      geometryWrapper.crs: navigation.mapSettings.destinationCrs
      pointIndex: model.index
      pointType: model.PointType
    }
  }
}
