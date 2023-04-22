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
    auto server = make_unique<Server>();
    server->start();
    return 0;
}