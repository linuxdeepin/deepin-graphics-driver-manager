
#include "resolutions.h"

#include <QFile>
#include <QDebug>

Resolutions::Resolutions()
{

}

Resolutions ResolutionsBuilder::build(const GraphicsDeviceInfo &devInfo)
{
    QFile config("/home/resolutions.json");
    Q_ASSERT_X(config.open(QIODevice::ReadOnly), Q_FUNC_INFO, "read config file error");

    qDebug() << config.readAll();


    Resolutions r;

    return r;
}