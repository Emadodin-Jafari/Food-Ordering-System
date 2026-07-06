#ifndef MEMBERSHIP_LEVEL_H
#define MEMBERSHIP_LEVEL_H
#include <string>

using namespace std;


class MembershipLevel{

public:
    virtual ~MembershipLevel() {}
    virtual string getLevelName() const = 0;
    virtual double calculateFinalPrice(double price) const = 0;

};


class NormalLevel : public MembershipLevel{
public:
    string getLevelName() const override;
    double calculateFinalPrice(double price) const override;
};


class SilverLevel : public MembershipLevel{
public:
    string getLevelName() const override;
    double calculateFinalPrice(double price) const override;
};

class GoldLevel : public MembershipLevel{
public:
    string getLevelName() const override;
    double calculateFinalPrice(double price) const override;
};

class VIPlevel : public MembershipLevel{
public:
    string getLevelName() const override;
    double calculateFinalPrice(double price) const override;
};




#endif