project (DataScrapperTest)
set (CMAKE_CXX_STANDARD 17)
find_package(GTest REQUIRED)
find_package(CURL REQUIRED)
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost REQUIRED COMPONENTS log)
find_package(Threads REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS} ../include ../src)
set(datascrapper_sources 
    CurlData.cpp CurlScrapper.cpp CurlSettings.cpp LocalDataStorage.cpp Server.cpp Utils.cpp DataSettingsFactory.cpp DataStorageFactory.cpp DataCollectorFactory.cpp Controller.cpp Command.cpp CommandQueue.cpp DataCollectingThread.cpp
)
list(TRANSFORM datascrapper_sources PREPEND "../src/")
add_executable(DataScrapperTest DataScrapperTest.cpp ${datascrapper_sources})
target_link_libraries(DataScrapperTest ${GTEST_LIBRARIES} pthread ${Boost_LIBRARIES} ${CURL_LIBRARIES})