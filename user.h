#ifndef USER_H
#define USER_H

#include <string>
#include <string_view>
#include <sstream>

/*
 * User class that stores user credentials
*/
class User
{
private:
    int mUserID;
    std::string mFirstName;
    std::string mLastName;
    std::string mPosition;

public:
    User() = default;
    User(int, std::string_view, std::string_view, std::string_view);
    ~User() = default;
    void setUserId(int);
    void setFirstName(std::string_view);
    void setLastName(std::string_view);
    void setPosition(std::string_view);
    int getUserId() const;
    std::string getFirstName();
    std::string getLastName();
    std::string getPosition();
    virtual std::string toString();
};

#endif // USER_H
