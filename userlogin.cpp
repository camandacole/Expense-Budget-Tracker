#include "userlogin.h"

UserLogin::UserLogin(int loginID, int userID, std::string_view userName, std::string_view password, std::byte accessLevel)
    : mLoginID{ loginID }, mUserID{ userID }, mUserName{ userName.data() }, mPassword{ password.data() }, mAccessLevel{accessLevel}{

}

void UserLogin::setLoginId(int ID) {
    mLoginID = ID;
}

void UserLogin::setUserId(int ID) {
    mUserID = ID;
}

void UserLogin::setUserName(std::string_view userName) {
    mUserName = userName.data();
}

void UserLogin::setPassword(std::string_view password) {
    mPassword = password.data();
}

void UserLogin::setAccessLevel(std::byte accessLevel){
    mAccessLevel = accessLevel;
}

/*
 * Gets a string and coberts it to a byte then sets the access level of this object
 * @param bytestring the byte to convert
*/
void UserLogin::setAccessLevel(std::string byteString){
    char c{0b00000000};
    for (int i{ 7 }; i >= 0; i--) {
        char bit = 7 - i;
        if (byteString[bit] == '1') {
            c |= i << bit;
        }
    }
    mAccessLevel = (std::byte)c;
}

/*
 * converts byte to a string
 * @returns the string representation of the byte
*/
std::string UserLogin::convertByteToString() {
    std::string byteString{ "" };
    char c{ (char)mAccessLevel };
    for (int i{ 7 }; i >= 0; i--) {
        if ((c >> i & 1) == 1) {
            byteString += "1";
        }
        else {
            byteString += "0";
        }
    }
    return byteString;
}

int UserLogin::getLoginId() {

    return mLoginID;
}

int UserLogin::getUserId() {

    return mUserID;
}

std::string UserLogin::getUserName() {
    return mUserName;
}

std::string UserLogin::getPassword() {
    return mPassword;
}

std::byte UserLogin::getAccessLevel() {
    return mAccessLevel;
}

/*
 * Gets the string representation of this user login object
 * Members are: loginID, username, password
 * @returns the string
*/
std::string UserLogin::toString() {
    std::stringstream obj{};
    obj << getLoginId() << getUserId() << "," << getUserName() << "," << getPassword();
    return obj.str();
}
