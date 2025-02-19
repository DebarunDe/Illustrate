#ifndef OAUTH2_H
#define OAUTH2_H

#include <iostream>
#include <string>
#include <unordered_map>
#include "functions.h"
#include "../keys/config.h"

class User {
    private:
        /**
         * @function getAuthCode
         * @brief Retrieves the authorization code from Yahoo Fantasy API.
         * 
         * This function is responsible for obtaining the authorization code required
         * to authenticate and access Yahoo Fantasy API endpoints.
         * 
         * @param User The user object to store the authorization code.
         * @return nothing.
         */
        template<typename T>
        void getAuthCode(T& User) {
            std::string authCodeResponse;
            std::string url = "https://api.login.yahoo.com/oauth2/request_auth?";
            // initialize curl
            CURL *curl;
            CURLcode res;
            std::unordered_map <std::string, std::string> postFields = {
                {"client_id", User.clientID},
                {"redirect_uri", User.redirectURI},
                {"response_type", "code"},
                {"language", "en-us"}
            };

            // generate postFields string
            std::string postFieldsStr = parseMapToPostString(postFields);

            // perform curl request
            std::cout << "Please enter the authorization code: ";
            curl = curl_easy_init();
            if (curl) { 
                curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
                curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postFieldsStr.c_str());
                std::string command = "open \"" + url+postFieldsStr + "\"";
                system(command.c_str());
            } else {
                std::cerr << "Curl failed to initialize." << std::endl;
                return;
            }

            std::cin >> authCodeResponse;
            User.authCode = authCodeResponse;
            curl_easy_cleanup(curl);
            return;
        }

        /**
        * @function getAccessToken
        * @brief Retrieves the access token from Yahoo Fantasy API.
        * 
        * This function is responsible for obtaining the access token required
        * to authenticate and access Yahoo Fantasy API endpoints.
        * 
        * @param clientId The client ID provided by Yahoo for API access.
        * @param clientSecret The client secret provided by Yahoo for API access.
        * @param redirectUri The redirect URI registered with Yahoo for API access.
        * @param authCode The authorization code provided by Yahoo for API access.
        * @return nothing.
        */
        template <typename T>
        void getAccessToken(T& User) {
            std::unordered_map<std::string, std::string> postFields = {
                {"client_id", User.clientID},
                {"client_secret", User.clientSecret},
                {"redirect_uri", User.redirectURI},
                {"code", User.authCode},
                {"grant_type", "authorization_code"}
            };
        
            auto response = call("LOGIN", "get_token", std::forward<std::unordered_map<std::string, std::string>>(postFields));
            User.accessToken = response["access_token"];
            User.refreshToken = response["refresh_token"];
        
            return;
        }


    public:
        std::string clientID;
        std::string clientSecret;
        std::string authCode;
        std::string accessToken;
        std::string refreshToken;
        std::string redirectURI;

        User() {
            clientID = CLIENT_ID;
            clientSecret = CLIENT_SECRET;
            redirectURI = REDIRECT_URI;
            authCode = "";
            accessToken = "";
            refreshToken = "";
        }
        
        ~User() {}

        void login() {
            //calls helper functions getAuthCode, getAccessToken to complete handshake with Yahoo Oauth2
            getAuthCode(*this);
            getAccessToken(*this);

            return;
        }
};

#endif  // OAUTH2_H