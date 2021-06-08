/***************************************************************************
 digitizinglogger.h - DigitizingLogger
  ---------------------
 begin                : 7.6.2021
 copyright            : (C) 2021 by Mathieu Pellerin
 email                : mathieu (at) opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef DIGITIZINGLOGGER_H
#define DIGITIZINGLOGGER_H

#include "gnsspositioninformation.h"
#include "qfieldcloudconnection.h"
#include "snappingresult.h"

#include <qgspoint.h>
#include <qgsproject.h>
#include <qgsvectorlayer.h>

#include <QObject>

class DigitizingLogger : public QObject
{
    Q_OBJECT

    Q_PROPERTY( QString category READ category WRITE setCategory NOTIFY categoryChanged )
    Q_PROPERTY( QgsProject *project READ project WRITE setProject NOTIFY projectChanged )
    Q_PROPERTY( QgsVectorLayer *digitizingLayer READ digitizingLayer WRITE setDigitizingLayer NOTIFY digitizingLayerChanged )
    Q_PROPERTY( GnssPositionInformation positionInformation READ positionInformation WRITE setPositionInformation NOTIFY positionInformationChanged )
    Q_PROPERTY( bool positionLocked READ positionLocked WRITE setPositionLocked NOTIFY positionLockedChanged )
    Q_PROPERTY( SnappingResult topSnappingResult READ topSnappingResult WRITE setTopSnappingResult NOTIFY topSnappingResultChanged )
    Q_PROPERTY( CloudUserInformation cloudUserInformation WRITE setCloudUserInformation );

  public:
    explicit DigitizingLogger();

    //! Returns the digitizing logs category
    QString category() const { return mCategory; }

    //! Sets the digitizing logs \a category
    void setCategory( const QString &category );

    //! Returns the current project from which the digitizing logs will be sought
    QgsProject *project() const { return mProject; }

    //! Sets the \a project used to find the digitizing logs layer
    void setProject( QgsProject *project );

    //! Returns the current vector layer used to digitize features
    QgsVectorLayer *digitizingLayer() const { return mDigitizingLayer; }

    //! Sets the current \a vectorLayer used to digitze features
    void setDigitizingLayer( QgsVectorLayer *vectorLayer );

    /**
     * Returns position information generated by the TransformedPositionSource according to its provider
     */
    GnssPositionInformation positionInformation() const { return mPositionInformation; }

    /**
     * Sets position information generated by the TransformedPositionSource according to its provider
     * \param positionInformation the position information
     */
    void setPositionInformation( const GnssPositionInformation &positionInformation );

    /**
     * Returns whether the position is locked to the GNSS
     */
    bool positionLocked() const { return mPositionLocked; }

    /**
     * Sets whether the position is locked to the GNSS
     * \param positionLocked set to TRUE if the position is locked
     */
    void setPositionLocked( bool positionLocked );

    /**
     * Returns the top snapping result of the coordinate locator
     */
    SnappingResult topSnappingResult() const { return mTopSnappingResult; }

    /**
     * Sets the top snapping result of the coordinate locator
     * \param topSnappingResult the top snapping result object
     */
    void setTopSnappingResult( const SnappingResult &topSnappingResult );

    /**
     * Sets the current cloud user information
     * \param cloudUserInformation the cloud user information
     */
    void setCloudUserInformation( const CloudUserInformation &cloudUserInformation ) { mCloudUserInformation = cloudUserInformation; }

    /**
     * Adds a \a point into the digitizing logs' buffer.
     */
    Q_INVOKABLE void addCoordinate( const QgsPoint &point );

    /**
     * Writes the points buffer to the digitizing logs layer.
     */
    Q_INVOKABLE void writeCoordinates();

    /**
     * Clear the points buffer from the digitizing logs.
     */
    Q_INVOKABLE void clearCoordinates();

  signals:
  
    void categoryChanged();
    void projectChanged();
    void digitizingLayerChanged();
    void positionInformationChanged();
    void positionLockedChanged();
    void topSnappingResultChanged();
    void currentCoordinateChanged();

  private:

    //! Finds and link to the logs layer in present in the project
    void findLogsLayer();

    QString mCategory;

    QgsProject *mProject = nullptr;
    QgsVectorLayer *mLogsLayer = nullptr;

    QgsVectorLayer *mDigitizingLayer = nullptr;

    GnssPositionInformation mPositionInformation;
    bool mPositionLocked = false;
    SnappingResult mTopSnappingResult;
    CloudUserInformation mCloudUserInformation;

    QList<QgsFeature> mPointFeatures;

};

#endif // DIGITIZINGLOGGER_H
