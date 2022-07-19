#include "appconfig.h"
#include <QIcon>
#include <functional>
#include <QStyle>

namespace Globals {
    app_config appConfig;
    app_config appConfig_default;
    QVector<QIcon> iconVector;
    QVector<QPair<QString, std::function<QString()>>> macroVector;
}
