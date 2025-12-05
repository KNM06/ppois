#include <gtest/gtest.h>
#include "../../include/core/Address.h"
#include <cmath>

class AddressTest : public ::testing::Test {
protected:
    std::unique_ptr<Address> createBasicAddress() {
        return std::make_unique<Address>("123 Main St", "Springfield", "IL", 
                                        "62704", "USA", 39.7817, -89.6501);
    }
};

TEST_F(AddressTest, DefaultConstructor) {
    Address address;
    
    EXPECT_EQ(address.getStreet(), "");
    EXPECT_EQ(address.getCity(), "");
    EXPECT_EQ(address.getState(), "");
    EXPECT_EQ(address.getZipCode(), "");
    EXPECT_EQ(address.getCountry(), "");
    EXPECT_DOUBLE_EQ(address.getLatitude(), 0.0);
    EXPECT_DOUBLE_EQ(address.getLongitude(), 0.0);
}

TEST_F(AddressTest, FullAddressConstructor) {
    Address address("123 Main St, Springfield, IL 62704, USA");
    
    EXPECT_EQ(address.getStreet(), "123 Main St, Springfield, IL 62704, USA");
    EXPECT_EQ(address.getCity(), "");
    EXPECT_EQ(address.getState(), "");
    EXPECT_EQ(address.getZipCode(), "");
    EXPECT_EQ(address.getCountry(), "");
    EXPECT_DOUBLE_EQ(address.getLatitude(), 0.0);
    EXPECT_DOUBLE_EQ(address.getLongitude(), 0.0);
}

TEST_F(AddressTest, ConstructorSixParamsNoCoords) {
    Address address("123 Main St", "Springfield", "IL", "62704", "USA");
    
    EXPECT_EQ(address.getStreet(), "123 Main St");
    EXPECT_EQ(address.getCity(), "Springfield");
    EXPECT_EQ(address.getState(), "IL");
    EXPECT_EQ(address.getZipCode(), "62704");
    EXPECT_EQ(address.getCountry(), "USA");
    EXPECT_DOUBLE_EQ(address.getLatitude(), 0.0);
    EXPECT_DOUBLE_EQ(address.getLongitude(), 0.0);
}

TEST_F(AddressTest, ConstructorFourParams) {
    Address address("123 Main St", "Springfield", "IL", "62704");
    
    EXPECT_EQ(address.getStreet(), "123 Main St");
    EXPECT_EQ(address.getCity(), "Springfield");
    EXPECT_EQ(address.getState(), "IL");
    EXPECT_EQ(address.getZipCode(), "62704");
    EXPECT_EQ(address.getCountry(), "USA");
    EXPECT_DOUBLE_EQ(address.getLatitude(), 0.0);
    EXPECT_DOUBLE_EQ(address.getLongitude(), 0.0);
}

TEST_F(AddressTest, FullConstructorEdgeCases) {
    Address address("", "", "", "", "", -100.0, 200.0);
    
    EXPECT_EQ(address.getStreet(), "");
    EXPECT_EQ(address.getCity(), "");
    EXPECT_EQ(address.getState(), "");
    EXPECT_EQ(address.getZipCode(), "");
    EXPECT_EQ(address.getCountry(), "");
    EXPECT_DOUBLE_EQ(address.getLatitude(), -100.0);
    EXPECT_DOUBLE_EQ(address.getLongitude(), 200.0);
}

TEST_F(AddressTest, CopyConstructor) {
    auto address1 = createBasicAddress();
    address1->setStreet("456 Oak St");
    address1->setCoordinates(40.0, -90.0);
    
    Address address2(*address1);
    
    EXPECT_EQ(address2.getStreet(), "456 Oak St");
    EXPECT_EQ(address2.getCity(), "Springfield");
    EXPECT_EQ(address2.getState(), "IL");
    EXPECT_EQ(address2.getZipCode(), "62704");
    EXPECT_EQ(address2.getCountry(), "USA");
    EXPECT_DOUBLE_EQ(address2.getLatitude(), 40.0);
    EXPECT_DOUBLE_EQ(address2.getLongitude(), -90.0);
}

TEST_F(AddressTest, CopyAssignment) {
    auto address1 = createBasicAddress();
    address1->setStreet("456 Oak St");
    address1->setCoordinates(40.0, -90.0);
    
    auto address2 = std::make_unique<Address>("789 Pine St", "Chicago", "IL", 
                                             "60601", "USA", 41.8781, -87.6298);
    
    *address2 = *address1;
    
    EXPECT_EQ(address2->getStreet(), "456 Oak St");
    EXPECT_EQ(address2->getCity(), "Springfield");
    EXPECT_EQ(address2->getState(), "IL");
    EXPECT_EQ(address2->getZipCode(), "62704");
    EXPECT_EQ(address2->getCountry(), "USA");
    EXPECT_DOUBLE_EQ(address2->getLatitude(), 40.0);
    EXPECT_DOUBLE_EQ(address2->getLongitude(), -90.0);
}

TEST_F(AddressTest, MoveConstructor) {
    auto address1 = createBasicAddress();
    Address address2(std::move(*address1));
    
    EXPECT_EQ(address2.getStreet(), "123 Main St");
    EXPECT_EQ(address2.getCity(), "Springfield");
    EXPECT_EQ(address2.getState(), "IL");
    EXPECT_EQ(address2.getZipCode(), "62704");
    EXPECT_EQ(address2.getCountry(), "USA");
    EXPECT_DOUBLE_EQ(address2.getLatitude(), 39.7817);
    EXPECT_DOUBLE_EQ(address2.getLongitude(), -89.6501);
}

TEST_F(AddressTest, MoveAssignment) {
    auto address1 = createBasicAddress();
    auto address2 = std::make_unique<Address>("789 Pine St", "Chicago", "IL", 
                                             "60601", "USA", 41.8781, -87.6298);
    
    *address2 = std::move(*address1);
    
    EXPECT_EQ(address2->getStreet(), "123 Main St");
    EXPECT_EQ(address2->getCity(), "Springfield");
    EXPECT_EQ(address2->getState(), "IL");
    EXPECT_EQ(address2->getZipCode(), "62704");
    EXPECT_EQ(address2->getCountry(), "USA");
    EXPECT_DOUBLE_EQ(address2->getLatitude(), 39.7817);
    EXPECT_DOUBLE_EQ(address2->getLongitude(), -89.6501);
}

TEST_F(AddressTest, SetCoordinatesEdgeCases) {
    auto address = createBasicAddress();
    
    address->setCoordinates(0.0, 0.0);
    EXPECT_DOUBLE_EQ(address->getLatitude(), 0.0);
    EXPECT_DOUBLE_EQ(address->getLongitude(), 0.0);
    
    address->setCoordinates(-90.0, -180.0);
    EXPECT_DOUBLE_EQ(address->getLatitude(), -90.0);
    EXPECT_DOUBLE_EQ(address->getLongitude(), -180.0);
    
    address->setCoordinates(90.0, 180.0);
    EXPECT_DOUBLE_EQ(address->getLatitude(), 90.0);
    EXPECT_DOUBLE_EQ(address->getLongitude(), 180.0);
    
    address->setCoordinates(100.0, 200.0);
    EXPECT_DOUBLE_EQ(address->getLatitude(), 100.0);
    EXPECT_DOUBLE_EQ(address->getLongitude(), 200.0);
}

TEST_F(AddressTest, GetFullAddressEdgeCases) {
    Address address1("", "", "", "", "");
    EXPECT_EQ(address1.getFullAddress(), ", ,  , ");
    
    Address address2("123 Main St", "", "IL", "62704", "");
    EXPECT_EQ(address2.getFullAddress(), "123 Main St, , IL 62704, ");
    
    Address address3("", "Springfield", "", "", "USA");
    EXPECT_EQ(address3.getFullAddress(), ", Springfield,  , USA");
    
    Address address4("  ", "  ", "  ", "  ", "  ");
    EXPECT_EQ(address4.getFullAddress(), "  ,   ,      ,   ");
    
    Address address5("123 Main St", "Springfield", "IL", "62704", "USA");
    EXPECT_EQ(address5.getFullAddress(), "123 Main St, Springfield, IL 62704, USA");
}

TEST_F(AddressTest, IsValidUSZipCodeAllCases) {
    Address address1("123 Main St", "Springfield", "IL", "62704", "USA");
    EXPECT_TRUE(address1.isValidUSZipCode());
    
    Address address2("123 Main St", "Springfield", "IL", "62704-1234", "USA");
    EXPECT_TRUE(address2.isValidUSZipCode());
    
    Address address3("123 Main St", "Springfield", "IL", "62704", "US");
    EXPECT_TRUE(address3.isValidUSZipCode());
    
    Address address4("123 Main St", "Springfield", "IL", "6270", "USA");
    EXPECT_FALSE(address4.isValidUSZipCode());
    
    Address address5("123 Main St", "Springfield", "IL", "62704-12", "USA");
    EXPECT_FALSE(address5.isValidUSZipCode());
    
    Address address6("123 Main St", "Springfield", "IL", "62704-12345", "USA");
    EXPECT_FALSE(address6.isValidUSZipCode());
    
    Address address7("123 Main St", "Springfield", "IL", "ABCDE", "USA");
    EXPECT_FALSE(address7.isValidUSZipCode());
    
    Address address8("123 Main St", "Springfield", "IL", "62704-ABCD", "USA");
    EXPECT_FALSE(address8.isValidUSZipCode());
    
    Address address9("123 Main St", "Springfield", "IL", "62704-12-34", "USA");
    EXPECT_FALSE(address9.isValidUSZipCode());
    
    Address address10("123 Main St", "Springfield", "IL", "62704", "Canada");
    EXPECT_FALSE(address10.isValidUSZipCode());
    
    Address address11("123 Main St", "Springfield", "IL", "", "USA");
    EXPECT_FALSE(address11.isValidUSZipCode());
    
    Address address12("123 Main St", "Springfield", "IL", "  ", "USA");
    EXPECT_FALSE(address12.isValidUSZipCode());
    
    Address address13("123 Main St", "Springfield", "IL", "62704-1234", "US");
    EXPECT_TRUE(address13.isValidUSZipCode());
}

TEST_F(AddressTest, IsInSameCityAllCases) {
    Address address1("123 Main St", "Springfield", "IL", "62704", "USA");
    Address address2("456 Oak St", "Springfield", "IL", "62705", "USA");
    EXPECT_TRUE(address1.isInSameCity(address2));
    
    Address address3("123 Main St", "Chicago", "IL", "60601", "USA");
    EXPECT_FALSE(address1.isInSameCity(address3));
    
    Address address4("123 Main St", "Springfield", "MO", "65804", "USA");
    EXPECT_FALSE(address1.isInSameCity(address4));
    
    Address address5("123 Main St", "Springfield", "IL", "62704", "Canada");
    EXPECT_FALSE(address1.isInSameCity(address5));
    
    Address address6("", "", "", "", "");
    EXPECT_TRUE(address6.isInSameCity(address6));
    
    Address address7("123 Main St", "", "IL", "62704", "USA");
    Address address8("456 Oak St", "", "IL", "62705", "USA");
    EXPECT_TRUE(address7.isInSameCity(address8));
    
    Address address9("123 Main St", "Springfield", "", "62704", "USA");
    Address address10("456 Oak St", "Springfield", "", "62705", "USA");
    EXPECT_TRUE(address9.isInSameCity(address10));
    
    Address address11("123 Main St", "Springfield", "IL", "62704", "");
    Address address12("456 Oak St", "Springfield", "IL", "62705", "");
    EXPECT_TRUE(address11.isInSameCity(address12));
}

TEST_F(AddressTest, CalculateDistanceEdgeCases) {
    Address address1("Location 1", "City", "State", "12345", "USA", 0.0, 0.0);
    Address address2("Location 2", "City", "State", "12345", "USA", 0.0, 0.0);
    
    double distance1 = address1.calculateDistance(address2);
    EXPECT_DOUBLE_EQ(distance1, 0.0);
    
    Address address3("Location 3", "City", "State", "12345", "USA", 0.0, 0.0);
    Address address4("Location 4", "City", "State", "12345", "USA", 0.0, 180.0);
    
    double distance2 = address3.calculateDistance(address4);
    EXPECT_GT(distance2, 0.0);
    
    Address address5("Location 5", "City", "State", "12345", "USA", 90.0, 0.0);
    Address address6("Location 6", "City", "State", "12345", "USA", -90.0, 0.0);
    
    double distance3 = address5.calculateDistance(address6);
    double earthCircumference = 40075.0;
    EXPECT_NEAR(distance3, earthCircumference / 2, 100.0);
    
    Address address7("Location 7", "City", "State", "12345", "USA", 0.0, 0.0);
    Address address8("Location 8", "City", "State", "12345", "USA", 0.0, 90.0);
    
    double distance4 = address7.calculateDistance(address8);
    EXPECT_GT(distance4, 0.0);
    
    Address address9("Location 9", "City", "State", "12345", "USA", 45.0, 45.0);
    Address address10("Location 10", "City", "State", "12345", "USA", -45.0, -45.0);
    
    double distance5 = address9.calculateDistance(address10);
    EXPECT_GT(distance5, 0.0);
    
    Address address11("Location 11", "City", "State", "12345", "USA", 100.0, 200.0);
    Address address12("Location 12", "City", "State", "12345", "USA", -100.0, -200.0);
    
    double distance6 = address11.calculateDistance(address12);
    EXPECT_GT(distance6, 0.0);
}

TEST_F(AddressTest, CalculateDistanceSamePoint) {
    Address address1("123 Main St", "Springfield", "IL", "62704", "USA", 39.7817, -89.6501);
    
    double distance = address1.calculateDistance(address1);
    EXPECT_DOUBLE_EQ(distance, 0.0);
}

TEST_F(AddressTest, CalculateDistanceAccuracy) {
    Address nyc("NYC", "New York", "NY", "10001", "USA", 40.7128, -74.0060);
    Address la("LA", "Los Angeles", "CA", "90001", "USA", 34.0522, -118.2437);
    
    double distance = nyc.calculateDistance(la);
    
    double expectedDistanceNYCtoLA = 3935.0;
    double tolerance = 100.0;
    
    EXPECT_NEAR(distance, expectedDistanceNYCtoLA, tolerance);
}

TEST_F(AddressTest, SetStreetAndCityEdgeCases) {
    auto address = createBasicAddress();
    
    address->setStreet("");
    EXPECT_EQ(address->getStreet(), "");
    
    address->setStreet("  ");
    EXPECT_EQ(address->getStreet(), "  ");
    
    address->setStreet("123 Main St, Apt 4B");
    EXPECT_EQ(address->getStreet(), "123 Main St, Apt 4B");
    
    address->setCity("");
    EXPECT_EQ(address->getCity(), "");
    
    address->setCity("  ");
    EXPECT_EQ(address->getCity(), "  ");
    
    address->setCity("St. Louis");
    EXPECT_EQ(address->getCity(), "St. Louis");
}

TEST_F(AddressTest, ConstructorWithSpecialCharacters) {
    Address address("123 Main St, Apt #4B", "St. Louis", "MO", "63101-1234", 
                   "U.S.A.", 38.6270, -90.1994);
    
    EXPECT_EQ(address.getStreet(), "123 Main St, Apt #4B");
    EXPECT_EQ(address.getCity(), "St. Louis");
    EXPECT_EQ(address.getState(), "MO");
    EXPECT_EQ(address.getZipCode(), "63101-1234");
    EXPECT_EQ(address.getCountry(), "U.S.A.");
    EXPECT_DOUBLE_EQ(address.getLatitude(), 38.6270);
    EXPECT_DOUBLE_EQ(address.getLongitude(), -90.1994);
}

TEST_F(AddressTest, IsValidUSZipCodeWithSpaces) {
    Address address1("123 Main St", "Springfield", "IL", "62704 1234", "USA");
    EXPECT_FALSE(address1.isValidUSZipCode());
    
    Address address2("123 Main St", "Springfield", "IL", "62704 -1234", "USA");
    EXPECT_FALSE(address2.isValidUSZipCode());
    
    Address address3("123 Main St", "Springfield", "IL", " 62704", "USA");
    EXPECT_FALSE(address3.isValidUSZipCode());
    
    Address address4("123 Main St", "Springfield", "IL", "62704 ", "USA");
    EXPECT_FALSE(address4.isValidUSZipCode());
}