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

struct persist_struct{
    uint64_t current_user_n;
    uint64_t current_space_n;
    short current_time_len;
};


class db_controller{
    private:
    mongocxx::database db;
    mongocxx::collection users;
    mongocxx::collection space;
    mongocxx::collection persistent;
    void removeFromSpace(std::string user_id);
    void delete_associate(std::string space_id);
    void update_persistent(persist_struct persist);
    std::string get_persistentData();

    public:
    db_controller(){
        try{
            if(!client){
                 cerr << "Error: Database client is not initialized!" << endl;
            } else{
                db=(*client)["idEngine_database"];
                users = db["users"];
                space = db["space"];
                persistent = db["persistent"];
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
    bool delete_user(std::string user_id);
    bool delete_space(std::string space_id);
    bool delete_ugc(std::string user_id,std::string ugc_id);
    bool delete_userFromSpace(std::string space_id,std::string user_id);
    bool delete_ugcFromSpace(std::string space_id,std::string ugc_id);
    std::string get_userUgc(std::string user_id);
    std::string get_ownedSpace_id(std::string user_id);
    std::string get_usersInSpace(std::string space_id);
    std::string get_spaceUgc(std::string space_id);
};



void db_controller::update_persistent(persist_struct persist){
    try{

        bsoncxx::builder::stream::document update{};
        update<<"$set"<<bsoncxx::builder::stream::open_document
        <<"current_user_n"<<persist.current_user_n
        <<"current_space_n"<<persist.current_space_n
        <<"current_time_len"<<persist.current_time_len<<bsoncxx::builder::stream::close_document;

        this->persistent.insert_one(update.view());

    } catch(std::exception& error){
        cout<<"Error updating peristent data - "<<error.what()<<endl;
    };
};



std::string db_controller::get_persistentData(){
    try{
       
        auto result=this->persistent.find_one({});
        if(result){
            return bsoncxx::to_json(result->view());
        } else{
            return "{}";
        };

    } catch(std::exception& error){
        cout<<"Error Retrieving Persistent Data - "<<error.what()<<endl;
    };
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
        };

    } catch(std::exception& error){
        cout<<"Internal Server Error - "<<error.what()<<endl;
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
        };

    } catch(std::exception& error){
        cout<<"Internal Server Error - "<<error.what()<<endl;
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

        auto result=this->users.update_one(filter.view(),update.view());

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

        auto result=this->users.update_one(filter.view(),update.view());

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
        };

    } catch(std::exception& error){
        cout<<"Error pushing space ugc id to space - "<<error.what()<<endl;
    };

    return isPushed;
};



bool db_controller::delete_user(std::string user_id){
    bool isDeleted;
    try{

        bsoncxx::builder::stream::document filter{};
        filter<<"user_id"<<user_id;

        auto result=this->users.delete_one(filter.view());

        if(result && result->deleted_count() > 0){
            isDeleted=true;
        } else{
            isDeleted=false;
        };

        this->removeFromSpace(user_id);

    } catch(std::exception& error){
        cout<<"Error Deleting User id - "<<error.what()<<endl;
    };

    return isDeleted;
};



void db_controller::removeFromSpace(std::string user_id){
    try{
        bsoncxx::builder::stream::document filter{};
        filter<<"user_id"<<user_id;

        bsoncxx::builder::stream::document update{};
        update<<"$pull"<<bsoncxx::builder::stream::open_document
        <<"user_id"<<user_id<<bsoncxx::builder::stream::close_document;

        this->space.update_many(filter.view(),update.view());

    } catch(std::exception& error){
        cout<<"Error Removing from reference in space - "<<error.what()<<endl;
    };
};




bool db_controller::delete_space(std::string space_id){
    bool isDeleted;
    try{

        bsoncxx::builder::stream::document filter{};
        filter<<"space_id"<<space_id;

        auto result=this->space.delete_one(filter.view());

        if(result && result->deleted_count() > 0){
            isDeleted=true;
        } else{
            isDeleted=false;
        };

        this->delete_associate(space_id);

    } catch(std::exception& error){
        cout<<"Error deleting space id - "<<error.what()<<endl;
    };

    return isDeleted;
};



void db_controller::delete_associate(std::string space_id){
    try{

        bsoncxx::builder::stream::document filter{};
        filter<<"ownedSpace_id"<<space_id;

        bsoncxx::builder::stream::document update{};
        update<<"$pull"<<bsoncxx::builder::stream::open_document
        <<"ownedSpace_id"<<space_id<<bsoncxx::builder::stream::close_document;

        this->users.delete_one(filter.view(),update.view());

    } catch(std::exception& error){
        cout<<"Error Deleting space id Reference in user - "<<error.what()<<endl;
    };
};



bool db_controller::delete_ugc(std::string user_id,std::string ugc_id){
    bool isDeleted;
    try{

        bsoncxx::builder::stream::document filter{};
        filter<<"user_id"<<user_id;

        bsoncxx::builder::stream::document update{};
        update<<"$pull"<<bsoncxx::builder::stream::open_document
        <<"ugc_id"<<ugc_id<<bsoncxx::builder::stream::close_document;

        auto result=this->users.delete_one(filter.view(),update.view());

        if(result && result->deleted_count() > 0){
            isDeleted=true;
        } else{
            isDeleted=false;
        };

    } catch(std::exception& error){
        cout<<"Error Deleting Ugc  - "<<error.what()<<endl;
    };

    return isDeleted;
};



bool db_controller::delete_userFromSpace(std::string space_id,std::string user_id){
    bool isDeleted;
    try{

        bsoncxx::builder::stream::document filter{};
        filter<<"space_id"<<space_id;

        bsoncxx::builder::stream::document update{};
        update<<"$pull"<<bsoncxx::builder::stream::open_document
        <<"user_id"<<user_id<<bsoncxx::builder::stream::close_document;

        auto result=this->space.delete_one(filter.view(),update.view());

        if(result && result->deleted_count() > 0){
            isDeleted=true;
        } else{
            isDeleted=false;
        };

    } catch(std::exception& error){
        cout<<"Error Deleting User from space  - "<<error.what()<<endl;
    };

    return isDeleted;
};




bool db_controller::delete_ugcFromSpace(std::string space_id,std::string ugc_id){
    bool isDeleted;
    try{

        bsoncxx::builder::stream::document filter{};
        filter<<"space_id"<<space_id;

        bsoncxx::builder::stream::document update{};
        update<<"$pull"<<bsoncxx::builder::stream::open_document
        <<"space_ugc"<<ugc_id<<bsoncxx::builder::stream::close_document;

        auto result=this->space.delete_one(filter.view(),update.view());

        if(result && result->deleted_count() > 0){
            isDeleted=true;
        } else{
            isDeleted=false;
        };

    } catch(std::exception& error){
        cout<<"Error Deleting User from space  - "<<error.what()<<endl;
    };

    return isDeleted;
};



std::string db_controller::get_userUgc(std::string user_id){
    try{

        bsoncxx::builder::stream::document filter{};
        filter<<"user_id"<<user_id;

        auto result=this->users.find_one(filter.view());

        if(result){
            auto view=result->view();
            if(view["ugc_id"] && view["ugc_id"].type() == bsoncxx::type::k_array){
                return bsoncxx::to_json(view["ugc_id"].get_array().value);
            };
        };

        return "[]";

    } catch(std::exception& error){
        cout<< "Error retrieving user ugc id: " << error.what()<<endl;
        return "{}";
    };
};




std::string db_controller::get_ownedSpace_id(std::string user_id){
    try{

        bsoncxx::builder::stream::document filter{};
        filter<<"user_id"<<user_id;

        auto result=this->users.find_one(filter.view());

        if(result){
            auto view=result->view();
            if(view["ownedSpace_id"] && view["ownedSpace_id"].type() == bsoncxx::type::k_array){
                return bsoncxx::to_json(view["ownedSpace_id"].get_array().value);
            };
        };

        return "[]";

    } catch(std::exception& error){
        cout<< "Error retrieving user owned space id: " << error.what() <<endl;
        return "{}";
    };
};




std::string db_controller::get_usersInSpace(std::string space_id){
    try{

        bsoncxx::builder::stream::document filter{};
        filter<<"space_id"<<space_id;

        auto result=this->space.find_one(filter.view());

        if(result){
            auto view=result->view();
            if(view["user_id"] && view["user_id"].type() == bsoncxx::type::k_array){
                return bsoncxx::to_json(view["user_id"].get_array().value);
            };
        };
        return "[]";
    } catch(std::exception& error){
        cout<< "Error retrieving users id in space : " << error.what() <<endl;
        return "{}";
    };
};
    
    
    

std::string db_controller::get_spaceUgc(std::string space_id){
    try{

        bsoncxx::builder::stream::document filter{};
        filter<<"space_id"<<space_id;

        auto result=this->space.find_one(filter.view());

        if(result){
            auto view=result->view();
            if(view["space_ugc"] && view["space_ugc"].type() == bsoncxx::type::k_array ){
                return bsoncxx::to_json(view["space_ugc"].get_array().value);
            };
        };
        return "[]";
    } catch(std::exception& error){
        cout<< "Error retrieving space ugc id: " << error.what() <<endl;
        return "{}";
    };
};