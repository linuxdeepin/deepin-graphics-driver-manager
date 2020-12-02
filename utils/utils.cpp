#include "utils.h"


namespace Utils {

QPixmap renderSVG(const QString &path, const QSize &size)
{
    QImageReader reader;
    QPixmap pixmap;
    reader.setFileName(path);
    if (reader.canRead()) {
        const qreal ratio = qApp->devicePixelRatio();
        reader.setScaledSize(size * ratio);
        pixmap = QPixmap::fromImage(reader.read());
        pixmap.setDevicePixelRatio(ratio);
    }
    else {
        pixmap.load(path);
    }
    return pixmap;
    }


QJsonObject QStringToJson(QString &jsonString)
{
    QJsonObject jsonObject;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonString.toLocal8Bit().data());
    if(jsonDocument.isNull()) {
        return  jsonObject;
    }
    jsonObject = jsonDocument.object();
    return jsonObject;

}

QString JsonToQString(QJsonObject &jsonObject)
{
     return QString(QJsonDocument(jsonObject).toJson());
}

const QPixmap hidpiPixmap(const QString &path, const QSize &sz)
{
    const auto ratio = qApp->devicePixelRatio();
    QPixmap iconPix = Utils::renderSVG(path, sz);
    iconPix.setDevicePixelRatio(ratio);

    return iconPix;
}

}
