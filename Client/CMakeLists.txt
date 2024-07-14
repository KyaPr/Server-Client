add_executable (Client "Client.cpp" "Client.hpp" "httplib.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Server PROPERTY CXX_STANDARD 14)
endif()
