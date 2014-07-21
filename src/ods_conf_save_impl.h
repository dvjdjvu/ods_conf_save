#ifndef ODS_CONF_SAVE_IMPL_H
#define ODS_CONF_SAVE_IMPL_H

#include <QObject>
#include <QMap>
#include <QStringList>
#include <QRegExp>

#include <OdsAppSrvInterface.h>
#include "serializer.h"
#include "parser.h"

#include <OdsConf/OdsConf.h>

#include <IObjectScheme>

using namespace ODS;

class OdsConfSaveImpl : public QObject
{
    Q_OBJECT

public:
    OdsConfSaveImpl();
    ~OdsConfSaveImpl();
    bool getResponce(const HttpRequest& _req, HttpResponce* _res);
    
    QString getURIFunct(QString uri);
    QString getURIParam(const HttpRequest& _req, QString par);

private:
    ODS::OdsInterface* odsIface; 
    ODS::IObjectManager ioMgr;
    
    OdsConf Conf;
};

#endif // ODS_CONF_SAVE_IMPL_H
