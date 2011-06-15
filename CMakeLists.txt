cmake_minimum_required(VERSION 2.6)

include(FindPkgConfig)
find_package(Qt4 REQUIRED)
find_package(X11 REQUIRED)

set(SOURCES
  qcategorizedview.cpp
  qcategorydrawer.cpp
  qcategorizedsortfilterproxymodel.cpp
)

qt4_automoc(${SOURCES})

include_directories(.
  ${QT_QTGUI_INCLUDE_DIR}
  ${QT_QTCORE_INCLUDE_DIR}
  ${QT_HEADERS_DIR})

add_library(qcategorizedview ${SOURCES})
target_link_libraries(qcategorizedview ${QT_QTGUI_LIBRARIES} ${QT_QTCORE_LIBRARIES})

install(TARGETS qcategorizedview DESTINATION lib)
