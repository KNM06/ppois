#ifndef ADDRESS_H
#define ADDRESS_H

#include <string>

class Address {
private:
    std::string street;
    std::string city;
    std::string state;
    std::string zipCode;
    std::string country;
    double latitude;
    double longitude;

public:
    Address();
    Address(const std::string& fullAddress);
    Address(const std::string& street,
        const std::string& city,
        const std::string& state,
        const std::string& zipCode,
        const std::string& country);

    Address(const std::string& street, const std::string& city,
            const std::string& state, const std::string& zipCode,
            const std::string& country, double lat, double lon);
    Address(const std::string& street, const std::string& city,
            const std::string& state, const std::string& zipCode);
    Address(const Address& other);
    Address(Address&& other) noexcept;
    Address& operator=(const Address& other);
    Address& operator=(Address&& other) noexcept;
    ~Address();

    std::string getStreet() const;
    std::string getCity() const;
    std::string getState() const;
    std::string getZipCode() const;
    std::string getCountry() const;
    double getLatitude() const;
    double getLongitude() const;

    void setStreet(const std::string& newStreet);
    void setCity(const std::string& newCity);
    void setCoordinates(double lat, double lon);

    std::string getFullAddress() const;
    bool isValidUSZipCode() const;
    bool isInSameCity(const Address& other) const;
    double calculateDistance(const Address& other) const;
};

#endif
