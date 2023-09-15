#include <gtest/gtest.h>
#include <boost/log/trivial.hpp>
#include "crow.h"
#include <memory>
#include <future>
#include "Utils.h"
#include "CurlScrapper.h"
#include "CurlSettings.h"
#include "LocalDataStorage.h"
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>
#include "DataCollectorFactory.h"
#include "DataSettingsFactory.h"
#include "DataStorageFactory.h"
#include "Controller.h"

using namespace std;

class RedirectLogger: public crow::ILogHandler {
    public:
        void log(std::string message, crow::LogLevel level) override {
            switch (level) {
                case crow::LogLevel::Debug:
                    BOOST_LOG_TRIVIAL(debug) << message;
                    break;
                case crow::LogLevel::Warning:
                    BOOST_LOG_TRIVIAL(warning) << message;
                    break;
                case crow::LogLevel::Error:
                    BOOST_LOG_TRIVIAL(error) << message;
                    break;
                case crow::LogLevel::Info:
                    BOOST_LOG_TRIVIAL(info) << message;
                    break;
                case crow::LogLevel::Critical:
                    BOOST_LOG_TRIVIAL(fatal) << message;
                    break;
            }
        }
};

class MockServer: public::testing::Test {
    protected:
        int port = 18080;
        MockServer() {
            BOOST_LOG_TRIVIAL(debug) << "Creating MockServer";
            app = nullptr;
            app_thread = nullptr;
        }
        ~MockServer() {
            BOOST_LOG_TRIVIAL(debug) << "Destroying MockServer";
            stop();
        }

        void SetUp() override {
            BOOST_LOG_TRIVIAL(debug) << "Setting up MockServer";
        }

        void run() {
            BOOST_LOG_TRIVIAL(debug) << "Running MockServer";
            app = make_shared<crow::SimpleApp>();
            auto f1 = [this] () {
                CROW_ROUTE((*app), "/data").methods("GET"_method)([this]() {
                    crow::response r;
                    r.set_header("Content-Type", "application/json");
                    r.write(Utils::readFile("test_geo.json"));
                    return r;
                });
                app->port(port).multithreaded().run();
            };
            app_thread = make_shared<thread>(f1);           
            BOOST_LOG_TRIVIAL(debug) << "Waiting for server to start";
            app->wait_for_server_start();
            BOOST_LOG_TRIVIAL(debug) << "Server started";
        }

        void stop() {
            BOOST_LOG_TRIVIAL(debug) << "Stopping MockServer";
            if (app != nullptr) app->stop();
            Utils::closeThread(app_thread);
            app = nullptr;
            app_thread = nullptr;            
        }

        void TearDown() override {
            BOOST_LOG_TRIVIAL(debug) << "Tearing down MockServer";
        }
        shared_ptr<crow::SimpleApp> app;
        shared_ptr<thread> app_thread;
};

TEST (TestSettings, TestSettings) {
    auto settings = DataSettingsFactory::getInstance().createDataSettings("curl");
    string data_url = "http://localhost:18080/data";
    string data_name = "test";
    string data_type = "json";
    settings->set("data.url", data_url);
    settings->set("data.name", data_name);
    settings->set("data.type", data_type);
    ASSERT_EQ(settings->get("data.url"), data_url);
    ASSERT_EQ(settings->get("data.name"), data_name);
    ASSERT_EQ(settings->get("data.type"), data_type);
    settings->save();
    settings->clear();
    ASSERT_EQ(settings->get("data.url"), "");
    ASSERT_EQ(settings->get("data.name"), "");
    ASSERT_EQ(settings->get("data.type"), "");
    settings->load();
    ASSERT_EQ(settings->get("data.url"), data_url);
    ASSERT_EQ(settings->get("data.name"), data_name);
    ASSERT_EQ(settings->get("data.type"), data_type);
    settings->clear();
    settings->save();
    settings->load();
    ASSERT_EQ(settings->get("data.url"), "");
    ASSERT_EQ(settings->get("data.name"), "");
    ASSERT_EQ(settings->get("data.type"), "");
}

TEST_F(MockServer, testData) {
    BOOST_LOG_TRIVIAL(debug) << "Testing Data related classes";
    port = 18081;
    run();
    auto settings = DataSettingsFactory::getInstance().createDataSettings("curl");
    auto storage  = DataStorageFactory::getInstance().createDataStorage("local");
    auto scrapper = DataCollectorFactory::getInstance().createDataCollector("curl");
    settings->set("data.url", "http://localhost:" + to_string(port) + "/data");
    settings->set("data.name", "test");
    settings->set("data.type", "json");
    settings->set("store.audit", "true");
    storage->setSettings(settings);
    scrapper->setSettings(settings);
    scrapper->setStorage(storage);
    scrapper->build();
    for (int i = 0; i < 10; i++) {
        scrapper->collectData();
        auto storedUrls = storage->getStoredUrls();
        ASSERT_EQ(storedUrls.size(), 1);
        string lastStore = storedUrls.front();
        BOOST_LOG_TRIVIAL(debug) << "Last store: " << lastStore;
        ASSERT_TRUE(Utils::compareFiles("test_geo.json", lastStore));
    }
    settings->set("store.audit", "");
    scrapper->collectData();    
    auto storedUrls = storage->getStoredUrls();
    ASSERT_TRUE(storedUrls.size()==0);
    settings->set("data.url", "");
    settings->set("data.name", "");
    settings->set("data.type", "");
    settings->set("store.audit", "");

}

TEST_F(MockServer, testController) {
    BOOST_LOG_TRIVIAL(debug) << "Testing Controller";
    port = 18082;
    run();
    Controller controller;
    controller.sendCommand("configure", "put", "data.url=http://localhost:" + to_string(port) + "/data");
    controller.sendCommand("configure", "put", "data.name=test");
    controller.sendCommand("configure", "put", "data.type=json");
    controller.sendCommand("configure", "put", "store.audit=true");
    controller.sendCommand("collect", "start", "");
    this_thread::sleep_for(chrono::seconds(5));
    controller.sendCommand("collect", "stop", "");
    controller.closeThreads();
    auto storedUrls = controller.getStoredUrls();
    ASSERT_TRUE(storedUrls.size() > 0);
    while (!storedUrls.empty()) {
        string url = storedUrls.front();
        BOOST_LOG_TRIVIAL(debug) << "Stored url: " << url;
        ASSERT_TRUE(Utils::compareFiles("test_geo.json", url));
        storedUrls.pop();
    }
    controller.sendCommand("configure", "clear", "");
}

TEST_F(MockServer, testControllerRestart) {
    BOOST_LOG_TRIVIAL(debug) << "Testing Restarting Controller";
    port = 18083;
    run();
    Controller controller;
    controller.sendCommand("configure", "put", "data.url=http://localhost:" + to_string(port) + "/data");
    controller.sendCommand("configure", "put", "data.name=test");
    controller.sendCommand("configure", "put", "data.type=json");
    controller.sendCommand("configure" , "put", "data.timeout=500");
    controller.sendCommand("configure", "put", "store.audit=true");
    controller.sendCommand("collect", "start", "");
    BOOST_LOG_TRIVIAL(debug) << "Going to sleep";
    this_thread::sleep_for(chrono::seconds(5));
    BOOST_LOG_TRIVIAL(debug) << "Stopping the server";
    stop();
    BOOST_LOG_TRIVIAL(debug) << "Server stopped";
    auto storedUrls = controller.getStoredUrls();
    while (!storedUrls.empty()) {
        string url = storedUrls.front();
        BOOST_LOG_TRIVIAL(debug) << "Stored url: " << url;
        ASSERT_TRUE(Utils::compareFiles("test_geo.json", url));
        storedUrls.pop();
    }
    port = 18084;
    run();
    controller.sendCommand("configure", "put", "data.url=http://localhost:" + to_string(port) + "/data");
    this_thread::sleep_for(chrono::seconds(5));
    controller.sendCommand("collect", "stop", "");
    controller.closeThreads();
    storedUrls = controller.getStoredUrls();
    ASSERT_TRUE(!storedUrls.empty());
    while (!storedUrls.empty()) {
        string url = storedUrls.front();
        BOOST_LOG_TRIVIAL(debug) << "Stored url: " << url;
        ASSERT_TRUE(Utils::compareFiles("test_geo.json", url));
        storedUrls.pop();
    }
    controller.sendCommand("configure", "clear", "");
}


int main (int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    RedirectLogger logger;
    crow::logger::setHandler(&logger);
    crow::logger::setLogLevel(crow::LogLevel::Debug);
    //set boost severity level
    boost::log::add_file_log("output.log");
    boost::log::core::get()->set_filter(boost::log::trivial::severity  >=  boost::log::trivial::debug);
    return RUN_ALL_TESTS();
}