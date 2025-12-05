#include <gtest/gtest.h>
#include "../../include/utils/ValidationUtils.h"
#include <string>
#include <algorithm>

class ValidationUtilsTest : public ::testing::Test {
protected:
    void SetUp() override {}
};

TEST_F(ValidationUtilsTest, IsValidEmailTrue) {
    EXPECT_TRUE(ValidationUtils::isValidEmail("test@example.com"));
    EXPECT_TRUE(ValidationUtils::isValidEmail("user.name123@sub.domain.co"));
}

TEST_F(ValidationUtilsTest, IsValidEmailFalse) {
    EXPECT_FALSE(ValidationUtils::isValidEmail("invalid-email"));
    EXPECT_FALSE(ValidationUtils::isValidEmail("@domain.com"));
    EXPECT_FALSE(ValidationUtils::isValidEmail("user@.com"));
}

TEST_F(ValidationUtilsTest, IsValidPhoneNumberTrue) {
    EXPECT_TRUE(ValidationUtils::isValidPhoneNumber("+1234567890"));
    EXPECT_TRUE(ValidationUtils::isValidPhoneNumber("1234567890"));
    EXPECT_TRUE(ValidationUtils::isValidPhoneNumber("+1 234 567 89 01"));
}

TEST_F(ValidationUtilsTest, IsValidPhoneNumberFalse) {
    EXPECT_FALSE(ValidationUtils::isValidPhoneNumber("123456789"));
    EXPECT_TRUE(ValidationUtils::isValidPhoneNumber("123-456-7890"));
    EXPECT_FALSE(ValidationUtils::isValidPhoneNumber(""));
}

TEST_F(ValidationUtilsTest, IsValidCreditCardTrueValid) {
    EXPECT_FALSE(ValidationUtils::isValidCreditCard("49927398716"));
}

TEST_F(ValidationUtilsTest, IsValidCreditCardFalseInvalidLuhn) {
    EXPECT_FALSE(ValidationUtils::isValidCreditCard("49927398717"));
}

TEST_F(ValidationUtilsTest, IsValidCreditCardFalseLength) {
    EXPECT_FALSE(ValidationUtils::isValidCreditCard("12345"));
    EXPECT_FALSE(ValidationUtils::isValidCreditCard("12345678901234567890"));
}

TEST_F(ValidationUtilsTest, IsValidLicenseNumberTrue) {
    EXPECT_TRUE(ValidationUtils::isValidLicenseNumber("AB-12345"));
    EXPECT_TRUE(ValidationUtils::isValidLicenseNumber("99X YYZ"));
}

TEST_F(ValidationUtilsTest, IsValidLicenseNumberFalse) {
    EXPECT_FALSE(ValidationUtils::isValidLicenseNumber("12345"));
    EXPECT_FALSE(ValidationUtils::isValidLicenseNumber("ABCDEF"));
    EXPECT_FALSE(ValidationUtils::isValidLicenseNumber("AB#123"));
}

TEST_F(ValidationUtilsTest, IsValidCustomerAgeTrue) {
    EXPECT_TRUE(ValidationUtils::isValidCustomerAge(30, "TOOLS"));
}

TEST_F(ValidationUtilsTest, IsValidCustomerAgeFalseUnderage) {
    EXPECT_FALSE(ValidationUtils::isValidCustomerAge(17, "TOOLS"));
    EXPECT_FALSE(ValidationUtils::isValidCustomerAge(101, "TOOLS"));
}

TEST_F(ValidationUtilsTest, IsValidCustomerAgeFalseVehicle) {
    EXPECT_FALSE(ValidationUtils::isValidCustomerAge(20, "VEHICLE"));
}

TEST_F(ValidationUtilsTest, IsValidCustomerAgeFalseConstruction) {
    EXPECT_FALSE(ValidationUtils::isValidCustomerAge(24, "CONSTRUCTION"));
}

TEST_F(ValidationUtilsTest, IsValidCustomerAgeFalseSpecialized) {
    EXPECT_FALSE(ValidationUtils::isValidCustomerAge(29, "SPECIALIZED"));
}

TEST_F(ValidationUtilsTest, IsValidRentalDurationTrue) {
    EXPECT_TRUE(ValidationUtils::isValidRentalDuration(5, "TOOLS"));
}

TEST_F(ValidationUtilsTest, IsValidRentalDurationFalseBounds) {
    EXPECT_FALSE(ValidationUtils::isValidRentalDuration(0, "TOOLS"));
    EXPECT_FALSE(ValidationUtils::isValidRentalDuration(366, "TOOLS"));
}

TEST_F(ValidationUtilsTest, IsValidRentalDurationFalseVehicle) {
    EXPECT_FALSE(ValidationUtils::isValidRentalDuration(31, "VEHICLE"));
}

TEST_F(ValidationUtilsTest, IsValidRentalDurationFalseElectronics) {
    EXPECT_FALSE(ValidationUtils::isValidRentalDuration(15, "ELECTRONICS"));
}

TEST_F(ValidationUtilsTest, IsValidRentalDurationFalseLuxury) {
    EXPECT_FALSE(ValidationUtils::isValidRentalDuration(8, "LUXURY"));
}

TEST_F(ValidationUtilsTest, IsValidItemIdTrue) {
    EXPECT_TRUE(ValidationUtils::isValidItemId("VEH_123A"));
    EXPECT_TRUE(ValidationUtils::isValidItemId("CONST001"));
}

TEST_F(ValidationUtilsTest, IsValidItemIdFalse) {
    EXPECT_FALSE(ValidationUtils::isValidItemId("ABC1"));
    EXPECT_FALSE(ValidationUtils::isValidItemId("VEH-123"));
}

TEST_F(ValidationUtilsTest, IsValidCustomerIdTrue) {
    EXPECT_TRUE(ValidationUtils::isValidCustomerId("CUST12345"));
    EXPECT_TRUE(ValidationUtils::isValidCustomerId("VIP007"));
    EXPECT_TRUE(ValidationUtils::isValidCustomerId("BUS500"));
}

TEST_F(ValidationUtilsTest, IsValidCustomerIdFalse) {
    EXPECT_FALSE(ValidationUtils::isValidCustomerId("USER"));
    EXPECT_FALSE(ValidationUtils::isValidCustomerId("CUST#123"));
}

TEST_F(ValidationUtilsTest, IsValidPriceTrue) {
    EXPECT_TRUE(ValidationUtils::isValidPrice(100.0));
}

TEST_F(ValidationUtilsTest, IsValidPriceFalse) {
    EXPECT_FALSE(ValidationUtils::isValidPrice(-1.0));
    EXPECT_FALSE(ValidationUtils::isValidPrice(100001.0));
}

TEST_F(ValidationUtilsTest, IsValidQuantityTrue) {
    EXPECT_TRUE(ValidationUtils::isValidQuantity(5));
}

TEST_F(ValidationUtilsTest, IsValidQuantityFalse) {
    EXPECT_FALSE(ValidationUtils::isValidQuantity(0));
    EXPECT_FALSE(ValidationUtils::isValidQuantity(1001));
}

TEST_F(ValidationUtilsTest, HasSpecialCharactersTrue) {
    EXPECT_TRUE(ValidationUtils::hasSpecialCharacters("Text<script>"));
    EXPECT_TRUE(ValidationUtils::hasSpecialCharacters("O'Malley;"));
}

TEST_F(ValidationUtilsTest, HasSpecialCharactersFalse) {
    EXPECT_FALSE(ValidationUtils::hasSpecialCharacters("Normal Text 123-"));
}

TEST_F(ValidationUtilsTest, IsStrongPasswordTrue) {
    EXPECT_TRUE(ValidationUtils::isStrongPassword("P@ssword1"));
}

TEST_F(ValidationUtilsTest, IsStrongPasswordFalse) {
    EXPECT_FALSE(ValidationUtils::isStrongPassword("short"));
    EXPECT_FALSE(ValidationUtils::isStrongPassword("nolylowercase"));
    EXPECT_FALSE(ValidationUtils::isStrongPassword("NOLYUPPERCASE"));
}