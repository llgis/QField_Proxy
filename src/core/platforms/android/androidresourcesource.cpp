/***************************************************************************
  androidresourcesource.cpp - AndroidResourceSource

 ---------------------
 begin                : 5.7.2016
 copyright            : (C) 2016 by Matthias Kuhn
 email                : matthias@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "androidresourcesource.h"
#include "qgsapplication.h"
#include "qgsmessagelog.h"

#include <QAndroidJniEnvironment>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QSettings>
#include <QtAndroid>

AndroidResourceSource::AndroidResourceSource( const QString &prefix )
  : ResourceSource( nullptr, prefix )
  , QAndroidActivityResultReceiver()
  , mPrefix( prefix )
{
}

void AndroidResourceSource::handleActivityResult( int receiverRequestCode, int resultCode, const QAndroidJniObject &data )
{
  if ( receiverRequestCode == 171 )
  {
    const bool nativeCameraLaunched = QSettings().value( QStringLiteral( "QField/nativeCameraLaunched" ), false ).toBool();
    if ( nativeCameraLaunched )
    {
      QSettings().setValue( QStringLiteral( "QField/nativeCameraLaunched" ), false );
    }

    jint RESULT_OK = QAndroidJniObject::getStaticField<jint>( "android/app/Activity", "RESULT_OK" );
    if ( resultCode == RESULT_OK )
    {
      QAndroidJniObject extras = data.callObjectMethod( "getExtras", "()Landroid/os/Bundle;" );


      QAndroidJniObject media_path = QAndroidJniObject::fromString( "RESOURCE_FILENAME" );
      media_path = extras.callObjectMethod( "getString", "(Ljava/lang/String;)Ljava/lang/String;",
                                            media_path.object<jstring>() );

      QString media_relative_path = media_path.toString().remove( mPrefix );

      emit resourceReceived( media_relative_path );
    }
    else
    {
      emit resourceReceived( QString() );
    }
  }
}