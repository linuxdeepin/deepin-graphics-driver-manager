#ifndef UTILS_H
#define UTILS_H

#include <QPixmap>
#include <QImageReader>
#include <QApplication>
#include <QJsonObject>
#include <QJsonDocument>

namespace Utils {

QPixmap renderSVG(const QString &path, const QSize &size);

QJsonObject QStringToJson(QString &jsonString);

QString JsonToQString(QJsonObject &jsonObject);

const QPixmap hidpiPixmap(const QString &path, const QSize &sz);


}


#endif
