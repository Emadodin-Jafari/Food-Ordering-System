#ifndef MEMBERSHIPLEVEL_H
#define MEMBERSHIPLEVEL_H

#include <string>

class MembershipLevel {
public:
    virtual ~MembershipLevel() {}
    virtual std::string getLevelName() const = 0;
    virtual double calculateFinalPrice(double price) const = 0;
    virtual double getPointsMultiplier() const = 0;
    virtual double calculateDeliveryFee(double totalPrice) const = 0;
    virtual int getMonthlyCouponCount() const = 0;
    virtual std::string getNextLevelRequirement(int currentPoints) const = 0;
};

class NormalLevel : public MembershipLevel {
public:
    std::string getLevelName() const override;
    double calculateFinalPrice(double price) const override;
    double getPointsMultiplier() const override;
    double calculateDeliveryFee(double totalPrice) const override;
    virtual int getMonthlyCouponCount() const override;
    virtual std::string getNextLevelRequirement(int currentPoints) const override;
};

class SilverLevel : public MembershipLevel {
public:
    std::string getLevelName() const override;
    double calculateFinalPrice(double price) const override;
    double getPointsMultiplier() const override;
    double calculateDeliveryFee(double totalPrice) const override;
    virtual int getMonthlyCouponCount() const override;
    virtual std::string getNextLevelRequirement(int currentPoints) const override;

};

class GoldLevel : public MembershipLevel {
public:
    std::string getLevelName() const override;
    double calculateFinalPrice(double price) const override;
    double getPointsMultiplier() const override;
    double calculateDeliveryFee(double totalPrice) const override;
    virtual int getMonthlyCouponCount() const override;
    virtual std::string getNextLevelRequirement(int currentPoints) const override;

};

class VIPlevel : public MembershipLevel {
public:
    std::string getLevelName() const override;
    double calculateFinalPrice(double price) const override;
    double getPointsMultiplier() const override;
    double calculateDeliveryFee(double totalPrice) const override;
    virtual int getMonthlyCouponCount() const override;
    virtual std::string getNextLevelRequirement(int currentPoints) const override;

};

#endif