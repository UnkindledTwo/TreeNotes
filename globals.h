#pragma once
#include "appconfig.h"
#include <QIcon>
#include <functional>

namespace Globals {
    extern app_config appConfig;
    extern app_config appConfig_default;
    extern QVector<QIcon> iconVector;
    extern QVector<QPair<QString, std::function<QString()>>> macroVector;
}
