project (DataScrapper)
cmake_minimum_required(VERSION 3.13)
set (CMAKE_CXX_STANDARD 17)
find_package(CURL REQUIRED)
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost REQUIRED COMPONENTS log)
find_package(Threads REQUIRED)
include_directories(${Boost_INCLUDE_DIRS} include src)
set(datascrapper_sources 
    main.cpp CurlData.cpp CurlScrapper.cpp CurlSettings.cpp LocalDataStorage.cpp Server.cpp Utils.cpp DataSettingsFactory.cpp DataStorageFactory.cpp DataCollectorFactory.cpp Controller.cpp Command.cpp CommandQueue.cpp DataCollectingThread.cpp
)
list(TRANSFORM datascrapper_sources PREPEND "src/")
add_executable(DataScrapper ${datascrapper_sources})
target_link_libraries(DataScrapper pthread ${Boost_LIBRARIES} ${CURL_LIBRARIES})