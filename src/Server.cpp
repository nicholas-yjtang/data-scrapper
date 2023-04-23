#include "Server.h"
#include <boost/log/trivial.hpp>
#include <crow.h>
#include <memory>
#include "Controller.h"
#include <boost/log/expressions.hpp>
#include <boost/log/core.hpp>

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

Server::Server() {
    BOOST_LOG_TRIVIAL(debug) << "Creating Server";
}

Server::~Server() {
    BOOST_LOG_TRIVIAL(debug) << "Destroying Server";
}

void Server::start() {
    BOOST_LOG_TRIVIAL(debug) << "Starting Server";
    RedirectLogger logger;
    crow::logger::setHandler(&logger);
    crow::logger::setLogLevel(crow::LogLevel::Debug);
    auto app = make_shared<crow::SimpleApp>();
    Controller controller;
    CROW_ROUTE((*app), "/configure").methods("PUT"_method)([&] (const crow::request& req) {
        auto keys = req.url_params.keys();
        crow::response r;
        for (size_t i = 0; i < keys.size(); i++) {
            string option = keys[i] + "=" + req.url_params.get(keys[i]);
            controller.sendCommand("configure", "put", option);
        }
        r.set_header("Content-Type", "application/json");
        return r;
    });

    CROW_ROUTE((*app), "/configure").methods("POST"_method)([&] (const crow::request& req) {
        auto keys = req.url_params.keys();
        crow::response r;
        for (size_t i = 0; i < keys.size(); i++) {
            if (keys[i] == "action") {
                controller.sendCommand("configure", req.url_params.get(keys[i]), "");
            }

        }
        r.set_header("Content-Type", "application/json");
        return r;
    });

    CROW_ROUTE((*app), "/log").methods("POST"_method)([&] (const crow::request& req) {
        auto keys = req.url_params.keys();
        crow::response r;
        for (size_t i = 0; i < keys.size(); i++) {
            if (keys[i] == "level") {
                string level = req.url_params.get(keys[i]);
                if (level == "debug") {
                    boost::log::core::get()->set_filter(boost::log::trivial::severity >= boost::log::trivial::debug);
                }
                else if (level == "info") {
                    boost::log::core::get()->set_filter(boost::log::trivial::severity >= boost::log::trivial::info);
                }
                else if (level == "warning") {
                    boost::log::core::get()->set_filter(boost::log::trivial::severity >= boost::log::trivial::warning);
                }
                else if (level == "error") {
                    boost::log::core::get()->set_filter(boost::log::trivial::severity >= boost::log::trivial::error);
                }
                else if (level == "fatal") {
                    boost::log::core::get()->set_filter(boost::log::trivial::severity >= boost::log::trivial::fatal);
                }
            }
            string option = keys[i] + "=" + req.url_params.get(keys[i]);
            controller.sendCommand("configure", "put", option);
        }
        r.set_header("Content-Type", "application/json");
        return r;
    });

    CROW_ROUTE((*app), "/collect").methods("POST"_method)([&] (const crow::request& req) {
        auto keys = req.url_params.keys();
        crow::response r;
        for (size_t i = 0; i < keys.size(); i++) {
            if (keys[i] == "action") {
                string action = req.url_params.get(keys[i]);
                controller.sendCommand("collect", action, "");
            }
        }
        r.set_header("Content-Type", "application/json");
        return r;
    });
    app->port(18080).run();
}