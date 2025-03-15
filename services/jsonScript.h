#pragma once

#include <iostream>
#include <string>
#include <memory>

#include <logic.h>

#include <nlohmann/json.hpp>


class jsonScript{
    private:

    public:
    jsonScript()=default;
    nlohmann::json id_2_json(std::string id);
    nlohmann::json convert_persistent(persist data);
    nlohmann::json delete_bool(bool isDeleted);
};


nlohmann::json jsonScript::id_2_json(std::string id) {

    nlohmann::json cast = { {"id", id} };

    return cast;
};



nlohmann::json jsonScript::convert_persistent(persist data){

    nlohmann::json persist_json = {
        {"current_user_n",data.current_user_n},
        {"current_space_n",data.current_space_n},
        {"current_time_len",data.current_time_len}
    };

    return persist_json;
};



nlohmann::json jsonScript::delete_bool(bool isDeleted){

    nlohmann::json cast_bool = {
        {"isDeleted", isDeleted}
    };

    return cast_bool;
}