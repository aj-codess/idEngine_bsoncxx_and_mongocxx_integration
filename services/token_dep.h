#pragma once 

#include <iostream>
#include <string>
#include <memory>
#include <functional>

#include <jwt-cpp/jwt.h>

#include <session_manager.h>

using namespace std;

class token_dep{
    private:
    server_session session;

    public:
    token_dep()=default;
    bool token_validity(std::string token,std::function<void(std::string)> callback);
    std::string token_signer(std::string id);
};


bool token_dep::token_validity(std::string token,std::function<void(std::string)> callback){

     bool isValid=false;

    try {

        std::ifstream file("./../assets/env.txt");

        std::stringstream buffer;

        buffer << file.rdbuf();

        auto decoded = jwt::decode(token);

        auto verifier = jwt::verify()
                          .allow_algorithm(jwt::algorithm::hs256{buffer.str()})
                          .with_issuer("auth0");

        verifier.verify(decoded);

        std::string tokenGen_server_id = decoded.get_payload_claim("server_id").as_string();

        if(callback){

            if(this->session.isInSession(tokenGen_server_id)==true && this->session.check_expiry(tokenGen_server_id)==false){

                callback(tokenGen_server_id);

                isValid=true;

            } else {

                isValid=false;

            };
            
        };

    } catch (const std::exception& e) {

        std::cerr << "Token verification failed: " << e.what() << std::endl;

        isValid=false;

    };

    return isValid;

};




std::string token_dep::token_signer(std::string id){

    std::string token;

    try{

        auto env_read=[&](std::function<void(std::string)> callback) {

        std::ifstream file("./../assets/env.txt");

        std::stringstream buffer;

        buffer << file.rdbuf();

        if(callback){

            callback(buffer.str());

        };

    };

    env_read([&](std::string key_value){

        auto gen_token=jwt::create().set_issuer("auth0").set_type("JWT").set_payload_claim("server_id",jwt::claim(id)).sign(jwt::algorithm::hs256{key_value});

        token=gen_token;

    });


    } catch(const std::exception& e){

        std::cout << "Token signing failed: " << e.what() << std::endl;

    };

    return token;

};