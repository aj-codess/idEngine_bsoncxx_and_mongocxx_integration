#pragma once


#include <iostream>
#include <string>

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/stream/document.hpp>

#include <db_connector.h>

using namespace std;

class db_controller{
    private:
    mongocxx::database db;
    mongocxx::collection users;
    mongocxx::collection space;

    public:
    db_controller(){
        try{
            if(!client){
                 cerr << "Error: Database client is not initialized!" << endl;
            } else{
                db=(*client)["idEngine_database"];
                users = db["users"];
                space = db["space"];
                cout<<"Database Seleted!"<<endl;
            }
        } catch(std::exception& error){
            cout<<"Internal Server Error - "<<error.what()<<endl;
        }
    }
};