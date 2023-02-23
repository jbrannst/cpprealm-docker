cmake_minimum_required(VERSION 3.22)
project(cpprealm_iot)

set(CMAKE_CXX_STANDARD 17)

add_executable(cpprealm_iot main.cpp)

find_package(cpprestsdk REQUIRED)

find_library(REALM_CPP_LIB NAMES cpprealm)
find_library(REALM_OS_LIB NAMES realm-object-store)
find_library(REALM_SYNC_LIB NAMES realm-sync)
find_library(REALM_LIB NAMES realm)
find_library(REALM_PARSER_LIB NAMES realm-parser)
find_library(SSL NAMES ssl)
find_library(CRYPTO NAMES crypto)

if (APPLE)
    find_library(FOUNDATION_LIB Foundation)
    find_library(SECURITY_LIB Security)
    include_directories(/usr/local/include/)
    link_directories(/usr/local/bin/)
    target_link_libraries(cpprealm_iot cpprestsdk::cpprest ${SSL} ${CRYPTO} ${REALM_CPP_LIB} ${REALM_OS_LIB} ${REALM_SYNC_LIB} ${REALM_LIB} ${REALM_PARSER_LIB} ${FOUNDATION_LIB} ${SECURITY_LIB} z -lcompression)
else()
    set(REALM_HAVE_UV 1)
    target_link_libraries(cpprealm_iot cpprestsdk::cpprest ${REALM_CPP_LIB} ${REALM_OS_LIB} ${REALM_SYNC_LIB} ${REALM_LIB} ${REALM_PARSER_LIB} z curl uv ssl crypto)
endif()