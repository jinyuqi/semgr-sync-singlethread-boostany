find_package(GTest REQUIRED)

add_library(GMock::GMock STATIC IMPORTED)
set_target_properties(GMock::GMock PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/gmock/lib/libgmock.a")

add_library(GMock::Main STATIC IMPORTED)
set_target_properties(GMock::Main PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/gmock/lib/libgmock_main.a")
set_target_properties(GMock::Main PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/gmock/include")
set_target_properties(GMock::Main PROPERTIES INTERFACE_LINK_LIBRARIES GMock::GMock)
set_property(TARGET GMock::Main APPEND PROPERTY INTERFACE_LINK_LIBRARIES GTest::GTest)

add_executable(ut_semgr ut_semgr.cpp)
target_link_libraries(ut_semgr GMock::Main semgr)

