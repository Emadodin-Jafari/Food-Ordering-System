#include "MembershipLevel.h"


string NormalLevel::getLevelName() const {
    return "Normal";
}

double NormalLevel::calculateFinalPrice(double price) const {
    return price;
}

string SilverLevel::getLevelName() const {
    return "Silver";
}

double SilverLevel::calculateFinalPrice(double price) const {
    return (price * 0.95);
}

string GoldLevel::getLevelName() const {
    return "Gold";
}

double GoldLevel::calculateFinalPrice(double price) const {
    return (price * 0.90);
}

string VIPlevel::getLevelName() const {
    return "VIP";
}

double VIPlevel::calculateFinalPrice(double price) const {
    return (price * 0.85);
}