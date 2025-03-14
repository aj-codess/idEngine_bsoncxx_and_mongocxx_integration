#include <iostream>
#include <string>
#include <fstream>

#include <boost/asio.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/coroutine/all.hpp>

#include <global_dcl.h>
#include <connection.h>
#include <compressor_module.h>
#include <hash_module.h>

using namespace std;


class conn {
public:
    void start_server();
    bool get_server_status();

    conn(boost::asio::io_context& ctx, domain end_p_url)
        : server(ctx, end_p_url) {

        cout << "loading server " << endl;

    }

private:
    connections server;
    hasher_class hasher;
    shrink_operands shrink;
};


int main(){

    domain url{"localhost", "8080"};

    boost::asio::io_context context;

    conn server(context, url);

    server.start_server();

    context.run();
    
    return 0;
}