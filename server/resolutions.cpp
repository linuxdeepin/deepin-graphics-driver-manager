
#include "resolutions.h"

#include <QFile>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>



Resolutions::Resolutions()
{

}

// int Resolutions::type()
// {
//     int flag = m_devInfo.deviceFlag();
//     switch(flag)
//     {
//         case GraphicsDeviceInfo::AMD:
//             return  AMD; 
//             break;
//         case GraphicsDeviceInfo::INTEL:
//             return INTEL;
//             break;
//         case GraphicsDeviceInfo::NVIDIA:
//             return NVIDIA;
//             break;
//         case (GraphicsDeviceInfo::NVIDIA | GraphicsDeviceInfo::INTEL):
//             if (m_devInfo.isNotebook()){
//                 return INTEL_NVIDIA;
//             }else{
//                 if (GraphicsDeviceInfo::INTEL == m_devInfo.CurrWorkingDevice())
//                     return  INTEL_NVIDIA_USE_INTEL;
//                 else if (GraphicsDeviceInfo::NVIDIA == m_devInfo.CurrWorkingDevice())
//                     return INTEL_NVIDIA_USE_NVIDIA;
//                 else return NoResolution;
//             }
//             break;
//         default:
//             break;
//     }
// }

ResolutionsBuilder::ResolutionsBuilder(const GraphicsDeviceInfo &devInfo) :
    m_config(RESOLUTIONS_DIR"/resolutions.json"),
    m_devInfo(devInfo)
{

}

ResolutionsBuilder ResolutionsBuilder::config(const QString &config)
{
    m_config = config;

    return *this;
}

Resolutions ResolutionsBuilder::build()
{
    QFile config(m_config);
    const bool b = config.open(QIODevice::ReadOnly);
    Q_ASSERT_X(b, Q_FUNC_INFO, "read config file error");

    const int deviceType = m_devInfo.deviceFlag();
    const auto doc = QJsonDocument::fromJson(config.readAll());

    Resolutions r;
    for (const auto res : doc.array())
    {
        const auto object = res.toObject();
        const int type = object["type"].toInt();
        if (type != deviceType)
            continue;
        //r.m_type=type();
        r.m_name = object["name"].toString();
        r.m_iconName = object["icon_name"].toString();
        r.m_statusScript = object["status"].toString();
        for (const auto resl : object["resolutions"].toArray()) {
            Resolution solution(resl.toObject());
            switch (deviceType)
            {
            case GraphicsDeviceInfo::AMD:
                if(r.m_type == Resolutions::NoResolution){
                    r.m_type = Resolutions::AMD;
                }
                if(object["name"].toString() == "amd"){
                    //根据白名单确认使用amdgpu还是raedon，目前这里默认使用amdgpu
                    if(solution.name() == "amdgpu"){
                        r.m_resolutions.append(solution);
                    }
                }
                break;
            case GraphicsDeviceInfo::INTEL:
                if(r.m_type == Resolutions::NoResolution){
                    r.m_type = Resolutions::INTEL;
                }
                if(object["name"].toString() == "intel"){
                    //根据白名单确认使用glamor、uxa、sna三者中的一个，目前这了默认使用glamor
                    if(solution.name() == "glamor"){
                        r.m_resolutions.append(solution);
                    }
                }
                break;
            case GraphicsDeviceInfo::NVIDIA:
                if(r.m_type == Resolutions::NoResolution){
                    r.m_type = Resolutions::NVIDIA;
                }
                if(object["name"].toString() == "nvidia"){
                    if(solution.name() == "nvidia" || solution.name() == "nouveau"){
                        r.m_resolutions.append(solution);
                    }
                }
                break;
            case (GraphicsDeviceInfo::INTEL | GraphicsDeviceInfo::NVIDIA):
                if(!m_devInfo.isNotebook())
                {
                    if (GraphicsDeviceInfo::INTEL == m_devInfo.curDeviceFlag())
                    {
                        if(r.m_type == Resolutions::NoResolution){
                            r.m_type = Resolutions::INTEL_NVIDIA_USE_INTEL;
                        }
                        if(object["name"].toString() == "intel"){
                            //根据白名单确认使用glamor、uxa、sna三者中的一个，目前这了默认使用glamor
                            if(solution.name() == "glamor"){
                                r.m_resolutions.append(solution);
                            }
                        }
                    }else if (GraphicsDeviceInfo::NVIDIA == m_devInfo.curDeviceFlag()){
                        if(r.m_type == Resolutions::NoResolution){
                            r.m_type = Resolutions::INTEL_NVIDIA_USE_NVIDIA;
                        }
                        if(object["name"].toString() == "nvidia"){
                            if(solution.name() == "nvidia" || solution.name() == "nouveau"){
                                r.m_resolutions.append(solution);
                            }
                        }
                    }
                }else{
                    if(r.m_type == Resolutions::NoResolution){
                        r.m_type = Resolutions::INTEL_NVIDIA;
                    }
                    if(object["name"].toString() == "intel"){
                        //根据白名单确认使用glamor、uxa、sna三者中的一个，目前这了默认使用glamor
                        if(solution.name() == "glamor"){
                            r.m_resolutions.append(solution);
                        }
                    }
                    if(object["name"].toString() == "nvidia_intel"){
                        if(solution.name() == "bumblebee" || solution.name() == "prime"){
                            r.m_resolutions.append(solution);
                        }
                    }
                }
                break;
            }
        }
    }
    return r;
}
