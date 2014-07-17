#include "ods_conf_save.h"
#include "ods_conf_save_impl.h"

OdsConfSave::OdsConfSave()
{
}

OdsConfSave::~OdsConfSave()
{
}

bool OdsConfSave::getResponce(const HttpRequest& _req, HttpResponce* _res)
{
    OdsConfSaveImpl impl;
    return impl.getResponce(_req, _res);
}

Q_EXPORT_PLUGIN2(OdsConfSave, OdsConfSave)
