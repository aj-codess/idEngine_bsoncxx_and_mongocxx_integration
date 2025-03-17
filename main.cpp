#include <iostream>
#include <string>
#include <fstream>

#include <boost/asio.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/coroutine/all.hpp>

#include <db_connector.h>

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


void conn::start_server() {

    connectToDatabase();

     auto env_writter=[&](){

            std::ofstream path(ENV);

            if (path.is_open()){

                try{

                    std::string gen=hasher.hash(this->shrink.compress(GLOBAL));

                    path<<gen<<endl;

                    path.close();

                } catch(const std::exception e){

                    cout<<"unable to write env - "<<e.what()<<endl;

                };

            };
        };

        env_writter();

    server.start_server();

}




bool conn::get_server_status() {

    return server.get_status();

};


int main(){

    domain url{"localhost", "8080"};

    boost::asio::io_context context;

    conn server(context, url);

    server.start_server();

    context.run();
    
    return 0;
}