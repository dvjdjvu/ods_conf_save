#ifndef ODS_CONF_SAVE_H
#define ODS_CONF_SAVE_H

#include <OdsAppSrvInterface.h>

class OdsConfSave : public QObject, public OdsAppSrvInterface
{
	Q_OBJECT
	Q_INTERFACES(OdsAppSrvInterface)

public:
	OdsConfSave(); 
	~OdsConfSave(); 
	bool getResponce(const HttpRequest& _req, HttpResponce* _res);
};

#endif //ODS_CONF_SAVE_H
