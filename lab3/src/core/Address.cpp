#include "../../include/core/Address.h"
#include <cmath>
#include <algorithm>

Address::Address() : street(""), city(""), state(""), zipCode(""), country(""),
            latitude(0.0), longitude(0.0) {}
Address::Address(const std::string& fullAddress)
    : street(fullAddress), city(""), state(""), zipCode(""), country(""), latitude(0.0), longitude(0.0) {}

Address::Address(const std::string& street, const std::string& city,
                 const std::string& state, const std::string& zipCode,
                 const std::string& country, double lat, double lon)
    : street(street), city(city), state(state), zipCode(zipCode),
      country(country), latitude(lat), longitude(lon) {}
Address::Address(const std::string& street, const std::string& city,
                 const std::string& state, const std::string& zipCode,
                 const std::string& country)
    : street(street), city(city), state(state), zipCode(zipCode),
      country(country), latitude(0.0), longitude(0.0) {}

Address::Address(const Address& other)
    : street(other.street), city(other.city), state(other.state),
      zipCode(other.zipCode), country(other.country),
      latitude(other.latitude), longitude(other.longitude) {}
Address::Address(const std::string& street, const std::string& city,
                 const std::string& state, const std::string& zipCode)
    : street(street), city(city), state(state), zipCode(zipCode),
      country("USA"), latitude(0.0), longitude(0.0) {}

Address& Address::operator=(const Address& other) {
    if (this != &other) {
        street = other.street;
        city = other.city;
        state = other.state;
        zipCode = other.zipCode;
        country = other.country;
        latitude = other.latitude;
        longitude = other.longitude;
    }
    return *this;
}

Address::Address(Address&& other) noexcept
    : street(std::move(other.street)), city(std::move(other.city)),
      state(std::move(other.state)), zipCode(std::move(other.zipCode)),
      country(std::move(other.country)), latitude(other.latitude),
      longitude(other.longitude) {}

Address& Address::operator=(Address&& other) noexcept {
    if (this != &other) {
        street = std::move(other.street);
        city = std::move(other.city);
        state = std::move(other.state);
        zipCode = std::move(other.zipCode);
        country = std::move(other.country);
        latitude = other.latitude;
        longitude = other.longitude;
    }
    return *this;
}

Address::~Address() {}

std::string Address::getStreet() const { return street; }
std::string Address::getCity() const { return city; }
std::string Address::getState() const { return state; }
std::string Address::getZipCode() const { return zipCode; }
std::string Address::getCountry() const { return country; }
double Address::getLatitude() const { return latitude; }
double Address::getLongitude() const { return longitude; }

void Address::setStreet(const std::string& newStreet) {
    street = newStreet;
}

void Address::setCity(const std::string& newCity) {
    city = newCity;
}

void Address::setCoordinates(double lat, double lon) {
    latitude = lat;
    longitude = lon;
}

std::string Address::getFullAddress() const {
    return street + ", " + city + ", " + state + " " + zipCode + ", " + country;
}

bool Address::isValidUSZipCode() const {
    if (country != "USA" && country != "US") return false;
    if (zipCode.length() != 5 && zipCode.length() != 10) return false;
    
    for (char c : zipCode) {
        if (zipCode.length() == 5 && !std::isdigit(c)) return false;
        if (zipCode.length() == 10 && c != '-' && !std::isdigit(c)) return false;
    }
    return true;
}

bool Address::isInSameCity(const Address& other) const {
    return city == other.city && state == other.state && country == other.country;
}

double Address::calculateDistance(const Address& other) const {
    double lat1 = latitude * M_PI / 180.0;
    double lon1 = longitude * M_PI / 180.0;
    double lat2 = other.latitude * M_PI / 180.0;
    double lon2 = other.longitude * M_PI / 180.0;
    
    double dlat = lat2 - lat1;
    double dlon = lon2 - lon1;
    
    double a = std::sin(dlat/2) * std::sin(dlat/2) +
               std::cos(lat1) * std::cos(lat2) *
               std::sin(dlon/2) * std::sin(dlon/2);
    double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1-a));
    
    return 6371.0 * c;
}