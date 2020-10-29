
#ifndef RESOLUTIONWIDGET_H_
#define RESOLUTIONWIDGET_H_


#include <QFrame>
#include <QDebug>
#include <QProcess>
#include <QDir>
#include <QLineEdit>


class QLabel;
class ResolutionWidget: public QFrame
{
    Q_OBJECT

public:
    explicit ResolutionWidget(QWidget *parent = nullptr);


};

#endif
