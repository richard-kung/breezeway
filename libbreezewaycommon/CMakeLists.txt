################# dependencies #################
### Qt/KDE
find_package(Qt5 ${QT_MIN_VERSION} REQUIRED CONFIG COMPONENTS Widgets)

################# breezewaystyle target #################
set(breezewaycommon_LIB_SRCS
    breezewayboxshadowrenderer.cpp
)

add_library(breezewaycommon5 ${breezewaycommon_LIB_SRCS})

generate_export_header(breezewaycommon5
    BASE_NAME breezewaycommon
    EXPORT_FILE_NAME breezewaycommon_export.h)

target_link_libraries(breezewaycommon5
    PUBLIC
        Qt::Core
        Qt::Gui)

set_target_properties(breezewaycommon5 PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR})

install(TARGETS breezewaycommon5 ${KDE_INSTALL_TARGETS_DEFAULT_ARGS} LIBRARY NAMELINK_SKIP)
