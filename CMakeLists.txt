cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
set(CMAKE_TOOLCHAIN_FILE /home/aj/code/Berita/vcpkg/scripts/buildsystems/vcpkg.cmake)

project(id_engine VERSION 1.0)

add_executable(main main.cpp)
target_include_directories(main PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/core)
target_include_directories(main PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/dependencies)
target_include_directories(main PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/config)
target_include_directories(main PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/services)
target_include_directories(main PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/ws_handlers)
target_include_directories(main PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/models)
target_include_directories(main PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/middleware)
target_include_directories(main PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/controller)
target_include_directories(main PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/routes)

find_package(boost_asio REQUIRED CONFIG)
target_include_directories(main PUBLIC ${boost_asio_INCLUDE_DIR})
target_link_libraries(main PUBLIC ${boost_asio_LIBRARIES} ${boost_asio_DEPENDENCIES})

find_package(boost_beast REQUIRED CONFIG)
target_include_directories(main PUBLIC ${boost_beast_INCLUDE_DIR})
target_link_libraries(main PUBLIC ${boost_beast_LIBRARIES} ${boost_beast_DEPENDENCIES})

find_package(boost_coroutine REQUIRED CONFIG)
target_link_libraries(main PUBLIC Boost::coroutine)

find_package(Boost_url REQUIRED CONFIG)
target_link_libraries(main PUBLIC Boost::url)

find_package(ZLIB REQUIRED)
target_include_directories(main PUBLIC ${ZLIB_INCLUDE_DIR})
target_link_libraries(main PUBLIC ${ZLIB_LIBRARIES} ${ZLIB_DEPENDENCIES})

find_package(nlohmann_json REQUIRED)
target_include_directories(main PUBLIC ${nlohmann_json_INCLUDE_DIR})
target_link_libraries(main PUBLIC ${nlohmann_json_LIBRARIES} ${nlohmann_json_DEPENDENCIES})

find_package(libpqxx REQUIRED)
target_include_directories(main PUBLIC ${libpqxx_INCLUDE_DIR})
target_link_libraries(main PUBLIC libpqxx::pqxx ${libpqxx_LIBRARIES} ${libpqxx_DEPENDENCIES})

find_package(OpenSSL REQUIRED)
target_include_directories(main PUBLIC ${OpenSSL_INCLUDE_DIR})
target_link_libraries(main PUBLIC ${OpenSSL_LIBRARIES} ${OpenSSL_DEPENDENCIES})

find_package(bsoncxx CONFIG REQUIRED)
target_include_directories(main PUBLIC ${bsoncxx_INCLUDE_DIR})
target_link_libraries(main PRIVATE $<IF:$<TARGET_EXISTS:mongo::bsoncxx_static>,mongo::bsoncxx_static,mongo::bsoncxx_shared>)

find_package(mongocxx CONFIG REQUIRED)
target_include_directories(main PUBLIC ${mongocxx_INCLUDE_DIR})
target_link_libraries(main PRIVATE $<IF:$<TARGET_EXISTS:mongo::mongocxx_static>,mongo::mongocxx_static,mongo::mongocxx_shared>)


target_include_directories(main PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../vcpkg/installed/x64-linux/include/jwt-cpp)

set(GLOBAL "GenuTech->id_engine")
set(ENV_PATH "./assets/env.txt")

target_compile_definitions(main PRIVATE GLOBAL="${GLOBAL}")
target_compile_definitions(main PRIVATE ENV="${ENV_PATH}")