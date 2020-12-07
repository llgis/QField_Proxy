/***************************************************************************
  bluetoothdevicemodel.h - BluetoothDeviceModel

 ---------------------
 begin                : 20.11.2020
 copyright            : (C) 2020 by David Signer
 email                : david (at) opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef BLUETOOTHDEVICEMODEL_H
#define BLUETOOTHDEVICEMODEL_H

#include <QAbstractListModel>
#include <QtBluetooth/QBluetoothServiceDiscoveryAgent>
#include <QtBluetooth/QBluetoothServiceInfo>
#include <QtBluetooth/QBluetoothLocalDevice>

/**
 * A model that manages the key/value pairs for a ValueMap widget.
 */
class BluetoothDeviceModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY( ScanningStatus scanningStatus READ scanningStatus WRITE setScanningStatus NOTIFY scanningStatusChanged )
    Q_PROPERTY( QString lastError READ lastError WRITE setLastError NOTIFY lastErrorChanged )

    Q_ENUMS( BluetoothDeviceRoles )

  public:
    //! The roles provided by this model
    enum BluetoothDeviceRoles
    {
        DeviceAddressRole = Qt::UserRole + 1,
        DisplayStringRole
    };

    enum ScanningStatus {
        Scanning,
        Succeeded,
        Failed,
        Canceled,
        NoStatus
    };
    Q_ENUM( ScanningStatus )

    /**
     * Create a new value map model
     */
    explicit BluetoothDeviceModel( QObject *parent = nullptr );


    int rowCount( const QModelIndex &parent = QModelIndex() ) const override;

    QVariant data( const QModelIndex &index, int role = Qt::DisplayRole ) const override;

    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void startServiceDiscovery();
    Q_INVOKABLE int findAddessIndex( const QString &address ) const;

    ScanningStatus scanningStatus() const { return mScanningStatus; };
    QString lastError() const { return mLastError; };

public slots:
    void setScanningStatus(ScanningStatus scanningStatus);
    void setLastError(QString lastError);

signals:
    void scanningStatusChanged(ScanningStatus scanningStatus);
    void lastErrorChanged(QString lastError);

private slots:
    void serviceDiscovered(const QBluetoothServiceInfo &service);

  private:
    std::unique_ptr<QBluetoothLocalDevice> mLocalDevice;
    QBluetoothServiceDiscoveryAgent mServiceDiscoveryAgent;
    QList<QPair<QString, QString>> mDiscoveredDevices;
    ScanningStatus mScanningStatus = NoStatus;
    QString mLastError;
};

#endif // BLUETOOTHDEVICEMODEL_H
