#include "MembershipLevel.h"
#include <string>

using namespace std;
string NormalLevel::getLevelName() const {
    return "Normal";
}
double NormalLevel::calculateFinalPrice(double price) const {
    return price;
}
double NormalLevel::getPointsMultiplier() const {
    return 1.0;
}
double NormalLevel::calculateDeliveryFee(double totalPrice) const {
    return 100.0;
}

int NormalLevel::getMonthlyCouponCount() const {
    return 0;
}
string NormalLevel::getNextLevelRequirement(int currentPoints) const {
    int nextPoints = 100 - currentPoints;
    return "You need " + std::to_string(nextPoints < 0 ? 0 : nextPoints) + " more points to upgrade to (Silver)!";
}

string SilverLevel::getLevelName() const {
    return "Silver";
}
double SilverLevel::calculateFinalPrice(double price) const {
    return price * 0.95;
}
double SilverLevel::getPointsMultiplier() const {
    return 1.2;
}
double SilverLevel::calculateDeliveryFee(double totalPrice) const {
    return (totalPrice > 500.0) ? 75.0 : 100.0;
}

int SilverLevel::getMonthlyCouponCount() const {
    return 1;
}

string SilverLevel::getNextLevelRequirement(int currentPoints) const {
    int nextPoints = 300 - currentPoints;
    return "You need " + std::to_string(nextPoints < 0 ? 0 : nextPoints) + " more points to upgrade to (Gold)!";
}

string GoldLevel::getLevelName() const {
    return "Gold";
}
double GoldLevel::calculateFinalPrice(double price) const {
    return price * 0.90;
}
double GoldLevel::getPointsMultiplier() const {
    return 1.5;
}
double GoldLevel::calculateDeliveryFee(double totalPrice) const {
    return 50.0;
}

int GoldLevel::getMonthlyCouponCount() const {
    return 1;
}

string GoldLevel::getNextLevelRequirement(int currentPoints) const {
    int nextPoints = 700 - currentPoints;
    return "You need " + std::to_string(nextPoints < 0 ? 0 : nextPoints) + " more points to upgrade to (VIP)!";
}

string VIPlevel::getLevelName() const {
    return "VIP";
}
double VIPlevel::calculateFinalPrice(double price) const {
    return price * 0.85;
}
double VIPlevel::getPointsMultiplier() const {
    return 2.0;
}
double VIPlevel::calculateDeliveryFee(double totalPrice) const {
    return 0.0;
}

int VIPlevel::getMonthlyCouponCount() const {
    return 3;
}
string VIPlevel::getNextLevelRequirement(int currentPoints) const {
    return "You are at the highest level (VIP Member)!";
}