#include "ods_conf_save_impl.h"
#include "ods_conf_save.h"

#include <Kernel/OdsInterface.h>

#include <QThread>
#include <QString>
#include <QDebug>

#include <QDomDocument>
#include <QDomElement>


/* Коды ошибок
   0 - всё хорошо
   1 - bad request (request is not valid)
   2 - connection error (OdsInterface == NULL)
   3 - JSON query parse error
   4 - Unknown action
   5 - config file error
*/

OdsConfSaveImpl::OdsConfSaveImpl()
{
    
}

OdsConfSaveImpl::~OdsConfSaveImpl()
{

}

bool 
OdsConfSaveImpl::addRecord(QString task, QString value, QString key)
{   
    // Добавление записи.
    IObject obj = this->ioMgr.createIObject(QString("$$$Системные ИО.Настройка"));
    obj.setAttr("Задача", task);
    obj.setAttr("Значение", value);
    obj.setAttr("Ключ", key);
    return this->ioMgr.saveIObject(obj);
}

bool 
OdsConfSaveImpl::delTask(QString task)
{
    // ioMgr.deleteIObjectByCondition(QString("$$$Системные ИО.Настройка"), "\"Задача\" = '3ПК ВО'");
    QString del = "\"Задача\" = '" + task + "'";
    return this->ioMgr.deleteIObjectByCondition(QString("$$$Системные ИО.Настройка"), del);
}

bool 
OdsConfSaveImpl::delTaskKey(QString task, QString key)
{
    QString del = "\"Задача\" = '" + task + "'" + key + "' and \"Ключ\" = '" + key + "'";
    return this->ioMgr.deleteIObjectByCondition(QString("$$$Системные ИО.Настройка"), del);
}

bool 
OdsConfSaveImpl::delAll()
{
    
    IObjectCursor cursor = this->ioMgr.getIObjects(QString("$$$Системные ИО.Настройка"));
    if (!cursor.isValid()) {
        return false;
    }
        
    IObjectCursor::iterator it = cursor.begin();
    while (it != cursor.end()) {
        IObject obj = *it;
        this->ioMgr.deleteIObject(obj, IObject::Default);
        it++;
    }
    
    return true;
}

bool 
OdsConfSaveImpl::updateTaskKey(QString task, QString value, QString key)
{
    //return this->ioMgr.updateIObject(obj);
    //return this->ioMgr.updateIObject(obj, )
    
    delTaskKey(task, key);
    
    return addRecord(task, value, key);
}

QStringList 
OdsConfSaveImpl::getAll()
{
    QStringList All;
    
    do {
        IObjectCursor cursor = this->ioMgr.getIObjects(QString("$$$Системные ИО.Настройка"));
        if (!cursor.isValid()) {
            break;
        }
        
        QString::number(cursor.size());
        
        IObjectCursor::iterator it = cursor.begin();
        while (it != cursor.end()) {
            IObject obj = *it;
            All << (obj.getStringAttr("Задача") + ";" + obj.getStringAttr("Значение") + ";" + obj.getStringAttr("Ключ")); 
            it++;
        }
    } while(0);
    
    return All;
}
    
QStringList 
OdsConfSaveImpl::getTask(QString task)
{
    QStringList Task;
    
    do {
        IObjectCursor cursor = this->ioMgr.getIObjects(QString("$$$Системные ИО.Настройка"));
        if (!cursor.isValid()) {
            break;
        }
        
        QString::number(cursor.size());
        
        IObjectCursor::iterator it = cursor.begin();
        while (it != cursor.end()) {
            IObject obj = *it;
            if (obj.getStringAttr("Задача") == task) {
                Task << (obj.getStringAttr("Задача") + ";" + obj.getStringAttr("Значение") + ";" + obj.getStringAttr("Ключ")); 
            }
            it++;
        }
    } while(0);
    
    return Task;
}

QString 
OdsConfSaveImpl::getTaskKey(QString task, QString key)
{
    do {
        IObjectCursor cursor = this->ioMgr.getIObjects(QString("$$$Системные ИО.Настройка"));
        if (!cursor.isValid()) {
            break;
        }
        
        QString::number(cursor.size());
        
        IObjectCursor::iterator it = cursor.begin();
        while (it != cursor.end()) {
            IObject obj = *it;
            if (obj.getStringAttr("Задача") == task && obj.getStringAttr("Ключ") == key) {
                return obj.getStringAttr("Значение");
            }
            it++;
        }
    } while(0);
    
    return "";
}

QStringList 
OdsConfSaveImpl::getTaskList()
{
    QStringList taskList;
    do {
        IObjectCursor cursor = this->ioMgr.getIObjects(QString("$$$Системные ИО.Настройка"));
        if (!cursor.isValid()) {
            break;
        }
        
        QString::number(cursor.size());
        
        IObjectCursor::iterator it = cursor.begin();
        while (it != cursor.end()) {
            IObject obj = *it;
            
            taskList << obj.getStringAttr("Задача");
            
            it++;
        }
    } while(0);
    
    taskList.sort();
    taskList.removeDuplicates();
    
    return taskList;
}

QString 
OdsConfSaveImpl::getURIFunct(QString uri)
{
    QRegExp rx("\\/(\\w+)[\\?|\\z]");
    
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

void
OdsConfSaveImpl::handler()
{
    
}

bool 
OdsConfSaveImpl::getResponce(const HttpRequest& _req, HttpResponce* _res)
{
    PLOG_DBG("get responce in plugin\n");
    QString str = "";
    
    
    do {
        if (!_req.isValid()) {
            break;
        }
        
        //QByteArray userLogin = _req.getUserLogin();
        //QByteArray userPwd   = _req.getUserPassword();
        
        QByteArray userLogin = QString("postgres").toAscii();
        QByteArray userPwd   = QString("1").toAscii();
        

        if (!_req.getSession()->isConnected(QString("192.168.5.198").toAscii(), QString("iksori_db").toAscii()))
        {
            _req.getSession()->createConnection(userLogin, userPwd,
                                                QString("ods_conf_save").toAscii(),
                                                QString("192.168.5.198").toAscii(),
                                                QString("iksori_db").toAscii());
        }

        this->odsIface = _req.getSession()->getConnection(QString("192.168.5.198").toAscii(), QString("iksori_db").toAscii());
        if (this->odsIface == NULL)
        {
            _res->addHeader("Content-Language",  "en,ru");
            _res->addHeader("Content-Type",  "application/json; charset=utf8");
            _res->addBody("this->odsIface == NULL");
            return false;
        }
        
        IObjectScheme scheme =  this->odsIface->structureManager().getScheme("$$$Системные ИО");
        if (!scheme.isValid())
        {
            _res->addHeader("Content-Language",  "en,ru");
            _res->addHeader("Content-Type",  "application/json; charset=utf8");
            _res->addBody("scheme == NULL");
            return false;
        }
        
        IObjectType type = scheme.getType("Настройка");
        if (!type.isValid())
        {
            _res->addHeader("Content-Language",  "en,ru");
            _res->addHeader("Content-Type",  "application/json; charset=utf8");
            _res->addBody("type == NULL");
            return false;
        }
        
        IObjectAttrCursor attrs = type.getAttrs();
        if (!attrs.isValid())
        {
            _res->addHeader("Content-Language",  "en,ru");
            _res->addHeader("Content-Type",  "application/json; charset=utf8");
            _res->addBody("attrs == NULL");
            return false;
        }
        
        
        /*
        IObjectAttrCursor::iterator attrIt = attrs.begin();
        while (attrIt != attrs.end() ) {
            IObjectAttr attr = *attrIt;
            if (!attr.isValid()) {
                continue;
            }
            
            str += attr.name() + " ! ";
            
            attrIt++;
        }
        */
        
        this->ioMgr = this->odsIface->iobjectManager();
        if (!this->ioMgr.isValid())
        {
            _res->addHeader("Content-Language",  "en,ru");
            _res->addHeader("Content-Type",  "application/json; charset=utf8");
            _res->addBody("ioMgr == NULL");
            return false;
        }
        
        // Добавление записи.
        //addRecord("ПК ВО2", "120", "Пороговое время актуальности данных");
        
        //delTaskKey("ПК ВО", "Пороговое время актуальности данных");
        
        //delAll();
        
        QString funct = getURIFunct(QString(_req.getURI()));
    
        do {
            if (funct == "/") {
                break;
            }
            if (funct == "get_all") {
                
                QStringList All = getAll();
                for (int i = 0; i < All.count(); i++) {
                    str += All[i] + "\n";
                }
                break;
            }
            if (funct == "get_task") {
                QString task = getURIParam(_req, "task");
                str += task + "\n";
                QStringList Task = getTask(task);
                
                for (int i = 0; i < Task.count(); i++) {
                    str += Task[i] + "\n";
                }
                break;
            }
            if (funct == "get_task_list") {
                QStringList taskList = getTaskList();
                
                for (int i = 0; i < taskList.count(); i++) {
                    str += taskList[i] + "\n";
                }
                break;
            }
            if (funct == "get_task_key") {
                QString task = getURIParam(_req, "task");
                QString key  = getURIParam(_req, "key");
                QString taskKey = getTaskKey(task, key);
                
                str += taskKey + "\n";
                
                break;
            }
            if (funct == "update_task_key") {
                QString task = getURIParam(_req, "task");
                QString key  = getURIParam(_req, "key");
                QString val  = getURIParam(_req, "val");
                //QString taskKey = getTaskKey(task, key);
                
                //updateTaskKey(task, val, key);
                
                //str += taskKey + "\n";
                
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
    //_res->addBody("{\"cats\": \"all ok\"}");
    
    
    _res->addBody(str);

    return false;
}
