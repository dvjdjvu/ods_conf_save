#include "ods_conf_save_impl.h"
#include "ods_conf_save.h"

#include <Kernel/OdsInterface.h>

#include <QThread>
#include <QString>
#include <QDebug>

#include <QDomDocument>
#include <QDomElement>


OdsConfSaveImpl::OdsConfSaveImpl()
{
    
}

OdsConfSaveImpl::~OdsConfSaveImpl()
{

}

QString 
OdsConfSaveImpl::getURIFunct(QString uri)
{
    QRegExp rx("\\/(\\w+)[\\?|\\z|$|\\Z]");
    
    int pos = rx.indexIn(uri);
    if (pos > -1) {
        return rx.cap(1);
    }
    
    return "/";
}

QString
OdsConfSaveImpl::getURIParam(const HttpRequest& _req, QString par)
{
    QHash<QString, QString> hash = _req.getParameters("utf8");
    QHash<QString, QString>::const_iterator i = hash.constBegin();
    while (i != hash.constEnd()) {
        if (i.key() == par) {
            return QString(i.value().toLocal8Bit());
        }    
        ++i;
    }
    
    return "";
}

bool 
OdsConfSaveImpl::getResponce(const HttpRequest& _req, HttpResponce* _res)
{
    PLOG_DBG("get responce in plugin\n");
    QString str = "";
    
    do {

        if (!_req.isValid()) 
        {
            break;
        }
        
        if (!_req.getSession()->isConnected(QString("192.168.5.198").toAscii(), QString("iksori_db").toAscii()))
        {
            _req.getSession()->createConnection(QString("postgres").toAscii(), QString("1").toAscii(),
                                                QString("ods_conf_save").toAscii(),
                                                QString("192.168.5.198").toAscii(),
                                                QString("iksori_db").toAscii());
        }

        this->odsIface = _req.getSession()->getConnection(QString("192.168.5.198").toAscii(), QString("iksori_db").toAscii());
        if (this->odsIface == NULL)
        {
            _res->addHeader("Content-Language",  "en,ru");
            _res->addHeader("Content-Type",  "application/json; charset=utf8");
            str = "this->odsIface == NULL";
            break;
        }
        
        this->Conf.init(QString("192.168.5.198"), QString("iksori_db"), QString("postgres"), QString("1"), QString("$$$Системные ИО"), QString("Настройка"));
        this->Conf.connect(this->odsIface);
        
        QString funct = getURIFunct(QString(_req.getURI()) + "?");
        
        do {
            if (funct == "/") {
                str += "It's work!";
                break;
            }
            if (funct == "get_all") {
                
                str += "var records = { \"records\": [ ";
                
                QStringList All = this->Conf.getAll();
                for (int i = 0; i < All.count(); i++) {
                    str += All[i];
                    if (i != All.count() - 1) {
                        str +=  ",";
                    }
                }
                
                str += " ]};";
                
                break;
            }
            if (funct == "get_task") {
                QString task = getURIParam(_req, "task");
                QStringList Task = this->Conf.getTask(task);
                
                str += "var records = { \"records\": [ ";
                
                for (int i = 0; i < Task.count(); i++) {
                    str += Task[i];
                    if (i != Task.count() - 1) {
                        str +=  ",";
                    }
                }
                
                str += " ]};";
                
                break;
            }
            if (funct == "get_task_list") {
                QStringList taskList = this->Conf.getTaskList();
                
                str += "var tasks = [ ";
                
                for (int i = 0; i < taskList.count(); i++) {
                    str += taskList[i];
                    if (i != taskList.count() - 1) {
                        str +=  ",";
                    }
                }
                
                str += " ];";
                
                break;
            }
            if (funct == "get_task_key") {
                QString task = getURIParam(_req, "task");
                QString key  = getURIParam(_req, "key");
                QString taskKey = this->Conf.getTaskKey(task, key);
                
                str += "var val = \"" + taskKey + "\";";
                
                break;
            }
            if (funct == "update_task_key") {
                QString task = getURIParam(_req, "task");
                QString key  = getURIParam(_req, "key");
                QString val  = getURIParam(_req, "val");
                
                str += "var op = \"" + QString::number(this->Conf.updateTaskKey(task, val, key)) + "\";";
                
                break;
            }
            if (funct == "add_record") {
                QString task = getURIParam(_req, "task");
                QString key  = getURIParam(_req, "key");
                QString val  = getURIParam(_req, "val");
                
                str += "var op = \"" + QString::number(this->Conf.addRecord(task, val, key)) + "\";";
                
                break;
            }
            
        } while (0);
        
        
        //obj.dumpInfo();
        
        _res->addHeader("Content-Language",  "en,ru");
        _res->addHeader("Content-Type",  "application/json; charset=utf8");
        _res->addBody(str);
        
        return false;
        
    } while (0);
    
    _res->addHeader("Content-Language",  "en,ru");
    _res->addHeader("Content-Type",  "application/json; charset=utf8");
    _res->addBody(str);

    return false;
}

