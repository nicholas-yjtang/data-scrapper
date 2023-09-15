#include "Server.h"
#include <memory>
#include <boost/log/trivial.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
using namespace std;

int main(int argc, char *argv[])
{
    boost::log::core::get()->set_filter(boost::log::trivial::severity  >=  boost::log::trivial::info);
    BOOST_LOG_TRIVIAL(info) << "Starting application";
    int port = getenv("DS_PORT") ? stoi(getenv("DS_PORT")) : 18080;
    auto server = make_unique<Server>(port);
    server->start();
    return 0;
}