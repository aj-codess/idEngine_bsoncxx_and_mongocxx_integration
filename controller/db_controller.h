#pragma once

#include <iostream>
#include <string>
#include <memory>

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
                cout<<"Database Connected and Seleted!"<<endl;
            }
        } catch(std::exception& error){
            cout<<"Internal Server Error - "<<error.what()<<endl;
        }
    };
    bool create_user(std::string user_id);
    bool create_space(std::string space_id);
    bool pushUser_ugc_id(std::string user_id,std::string ugc_id);
    bool pushUser_ownedSpace_id(std::string user_id,std::string space_id);
    bool pushSpace_userId(std::string space_id,std::string user_id);
    bool pushSpace_spaceUgc(std::string space_id,std::string spaceUgc);
};


bool db_controller::create_user(std::string user_id){
    bool isSaved;
    try{

        bsoncxx::builder::basic::document data{};
        bsoncxx::builder::basic::array ugc_id;
        bsoncxx::builder::basic::array ownedSpace_id;

        data.append(
            bsoncxx::builder::basic::kvp("user_id",user_id),
            bsoncxx::builder::basic::kvp("ugc_id",ugc_id),
            bsoncxx::builder::basic::kvp("ownedSpace_id",ownedSpace_id)
        );

        auto result=this->users.insert_one(data.view());

        if(result && result->result().inserted_count() > 0){
            isSaved=true;
        } else{
            isSaved=false;
        }

    } catch(std::exception& error){
        cout<<"Internal Server Error - "error.what()<<endl;
    };

    return isSaved;
};



bool db_controller::create_space(std::string space_id){
    bool isSaved;

    try{

        bsoncxx::builder::basic::document data{};
        bsoncxx::builder::basic::array user_id;
        bsoncxx::builder::basic::array space_ugc;

        data.append(
            bsoncxx::builder::basic::kvp("space_id",space_id),
            bsoncxx::builder::basic::kvp("user_id",user_id),
            bsoncxx::builder::basic::kvp("space_ugc",space_ugc)
        );

        auto result=this->space.insert_one(data.view());

        if(result && result->result().inserted_count() > 0){
            isSaved=true;
        } else{
            isSaved=false;
        }

    } catch(std::exception& error){
        cout<<"Internal Server Error - "error.what()<<endl;
    };

    return isSaved;
};



bool db_controller::pushUser_ugc_id(std::string user_id,std::string ugc_id){
    bool isPushed;
    try{

        bsoncxx::builder::stream::document filter{};
        filter<<"user_id"<<user_id;

        bsoncxx::builder::stream::document update{};
        update<<"$push"<<bsoncxx::builder::stream::open_document
        <<"ugc_id"<<ugc_id<<bsoncxx::builder::stream::close_document;

        auto result=this->users.update_one(filter.view(),update.vew());

        if(result && result->modified_count() > 0){
            isPushed=true;
        } else{
            isPushed=false;
        };

    } catch(std::exception& error){
        cout<<"Error pushing user ugc id - "<<error.what()<<endl;
    };

    return isPushed;
};




bool db_controller::pushUser_ownedSpace_id(std::string user_id,std::string space_id){
    bool isPushed;
    try{

        bsoncxx::builder::stream::document filter{};
        filter<<"user_id"<<user_id;

        bsoncxx::builder::stream::document update{};
        update<<"$push"<<bsoncxx::builder::stream::open_document
        <<"space_id"<<space_id<<bsoncxx::builder::stream::close_document;

        auto result=this->user.update_one(filter.view(),update.view());

        if(result && result->modified_count() > 0){
            isPushed=true;
        } else{
            isPushed=false;
        };

    } catch(std::exception& error){
        cout<<"Error pushing to owned space id - "<<error.what()<<endl;
    };

    return isPushed;
};



bool db_controller::pushSpace_userId(std::string space_id,std::string user_id){
    bool isPushed;
    try{

        bsoncxx::builder::stream::document filter{};
        filter<<"space_id"<<space_id;

        bsoncxx::builder::stream::document update{};
        update<<"$push"<<bsoncxx::builder::stream::open_document
        <<"user_id"<<user_id<<bsoncxx::builder::stream::close_document;

        auto result=this->space.update_one(filter.vew(),update.view());

        if(result && result->modified_count() > 0){
            isPushed=true;
        } else{
            isPushed=false;
        };

    } catch(std::exception& error){
        cout<<"Error Pushing User Id To space - "<<error.what()<<endl;
    };

    return isPushed;
};




bool db_controller::pushSpace_spaceUgc(std::string space_id,std::string spaceUgc){
    bool isPushed;
    try{

        bsoncxx::builder::stream::document filter{};
        filter<<"space_id"<<space_id;

        bsoncxx::builder::stream::document update{};
        update<<"$push"<<bsoncxx::builder::stream::open_document
        <<"space_ugc"<<spaceUgc<<bsoncxx::builder::stream::close_document;

        auto result=this->space.update_one(filter.view(),update.view());

        if(result && result->modified_count() > 0){
            isPushed=true;
        } else{
            isPushed=false;
        }

    } catch(std::exception& error){
        cout<<"Error pushing space ugc id to space - "<<error.what()<<endl;
    };

    return isPushed;
};