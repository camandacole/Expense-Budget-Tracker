#include "user.h"
#include <string>
#include <sstream>

User::User(int userID, std::string_view firstName, std::string_view lastName, std::string_view position)
    : mUserID{ userID }, mFirstName{ firstName.data() }, mLastName{ lastName.data() }, mPosition{ position } {

}


void User::setUserId(int ID) {
    mUserID = ID;
}

void User::setFirstName(std::string_view firstName) {
    mFirstName = firstName.data();
}

void User::setLastName(std::string_view lastName) {
    mLastName = lastName.data();
}

void User::setPosition(std::string_view position) {
    mPosition = position.data();
}

int User::getUserId() const{

    return mUserID;
}

std::string User::getFirstName() {
    return mFirstName;
}

std::string User::getLastName() {
    return mLastName;
}

std::string User::getPosition() {
    return mPosition;
}

/*
 * Gets the string representation of this user object
 * Members are: userID, firstname, lastname, position
 * @returns the string
*/
std::string User::toString() {
    std::stringstream obj{};
    obj << getUserId() << "," << getLastName() << "," << getFirstName() << getPosition();
    return obj.str();
}
