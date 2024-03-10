#ifndef USERLOGIN_H
#define USERLOGIN_H

#include <string>
#include <string_view>
#include <sstream>

/*
 * UserLogin class that stores user credentials for loggin into the system
 * This includes most importantly the password and username
*/
class UserLogin
{
private:
    int mUserID;
    int mLoginID;
    std::string mUserName;
    std::string mPassword;
    std::byte mAccessLevel;

public:
    UserLogin() = default;
    UserLogin(int, int, std::string_view, std::string_view, std::byte);
    ~UserLogin() = default;
    void setUserId(int);
    void setLoginId(int);
    void setUserName(std::string_view);
    void setPassword(std::string_view);
    void setAccessLevel(std::byte);
    void setAccessLevel(std::string);
    std::string convertByteToString();
    int getUserId();
    int getLoginId();
    std::string getUserName();
    std::string getPassword();
    std::byte getAccessLevel();
    virtual std::string toString();

};

#endif // USERLOGIN_H
