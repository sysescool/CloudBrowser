#ifndef COMMON_H
#define COMMON_H

#include "src/bend/man/mancloud.h"
#include "src/bend/gateway.h"

#include "src/config/exceptions.h"
#include "src/config/errorcode.h"
#include "src/config/apis.h"
#include "src/config/globals.h"
#include "src/config/loggerproxy.h"

#include "src/middle/manmodels.h"
#include "src/middle/signals/mansignals.h"
#include "src/middle/manglobal.h"

#include "src/helper/filehelper.h"
#include "src/helper/bytehelper.h"

// Qt6 默认使用 UTF-8，直接用字符串字面量即可
// Qt5 需要显式转换为 UTF-8
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    #define STR(x) QString(x)
#else
    #define STR(x) QString::fromUtf8(x)
#endif

#endif // COMMON_H
