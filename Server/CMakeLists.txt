add_executable (Server "Server.cpp" "Server.hpp" "database.h" "httplib.h" "json.hpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Server PROPERTY CXX_STANDARD 14)
endif()

