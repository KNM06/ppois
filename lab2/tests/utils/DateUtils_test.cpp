#include <gtest/gtest.h>
#include "../../include/utils/DateUtils.h"
#include <string>
#include <algorithm>

class DateUtilsTest : public ::testing::Test {
protected:
    void SetUp() override {}
};

TEST_F(DateUtilsTest, GetCurrentDate) {
    std::string date = DateUtils::getCurrentDate();
    EXPECT_FALSE(date.empty());
    EXPECT_EQ(date.length(), 10);
    EXPECT_EQ(date[4], '-');
    EXPECT_EQ(date[7], '-');
}

TEST_F(DateUtilsTest, AddDaysToDateWithinMonth) {
    std::string newDate = DateUtils::addDaysToDate("2024-01-01", 7);
    EXPECT_EQ(newDate, "2024-01-08");
}

TEST_F(DateUtilsTest, AddDaysToDateAcrossMonth) {
    std::string newDate = DateUtils::addDaysToDate("2024-01-20", 15);
    EXPECT_EQ(newDate, "2024-02-05");
}

TEST_F(DateUtilsTest, AddDaysToDateAcrossYear) {
    std::string newDate = DateUtils::addDaysToDate("2024-12-15", 30);
    EXPECT_EQ(newDate, "2025-01-15");
}

TEST_F(DateUtilsTest, CalculateDateDifferencePositive) {
    int difference = DateUtils::calculateDateDifference("2024-01-01", "2024-01-08");
    EXPECT_EQ(difference, 7);
}

TEST_F(DateUtilsTest, CalculateDateDifferenceNegative) {
    int difference = DateUtils::calculateDateDifference("2024-01-10", "2024-01-01");
    EXPECT_EQ(difference, -9);
}

TEST_F(DateUtilsTest, CalculateDateDifferenceAcrossYear) {
    int difference = DateUtils::calculateDateDifference("2024-12-01", "2025-01-01");
    EXPECT_EQ(difference, 35);
}

TEST_F(DateUtilsTest, IsDateInFutureToday) {
    std::string today = DateUtils::getCurrentDate();
    bool isFuture = DateUtils::isDateInFuture(today);
    EXPECT_FALSE(isFuture);
}

TEST_F(DateUtilsTest, IsDateInFuturePast) {
    bool isFuture = DateUtils::isDateInFuture("2000-01-01");
    EXPECT_FALSE(isFuture);
}

TEST_F(DateUtilsTest, IsDateInFutureFuture) {
    bool isFuture = DateUtils::isDateInFuture("2040-01-01");
    EXPECT_TRUE(isFuture);
}

TEST_F(DateUtilsTest, IsWeekendSaturday) {
    bool isWeekend = DateUtils::isWeekend("2024-01-06");
    EXPECT_TRUE(isWeekend);
}

TEST_F(DateUtilsTest, IsWeekendSunday) {
    bool isWeekend = DateUtils::isWeekend("2024-01-07");
    EXPECT_TRUE(isWeekend);
}

TEST_F(DateUtilsTest, IsWeekendWeekday) {
    bool isWeekend = DateUtils::isWeekend("2024-01-08");
    EXPECT_FALSE(isWeekend);
}

TEST_F(DateUtilsTest, GetSeasonSpring) {
    EXPECT_EQ(DateUtils::getSeason("2024-04-15"), "SPRING");
}

TEST_F(DateUtilsTest, GetSeasonSummer) {
    EXPECT_EQ(DateUtils::getSeason("2024-07-20"), "SUMMER");
}

TEST_F(DateUtilsTest, GetSeasonAutumn) {
    EXPECT_EQ(DateUtils::getSeason("2024-10-05"), "AUTUMN");
}

TEST_F(DateUtilsTest, GetSeasonWinter) {
    EXPECT_EQ(DateUtils::getSeason("2024-02-10"), "WINTER");
}

TEST_F(DateUtilsTest, IsHolidayNewYears) {
    EXPECT_TRUE(DateUtils::isHoliday("2024-01-01"));
}

TEST_F(DateUtilsTest, IsHolidayChristmas) {
    EXPECT_TRUE(DateUtils::isHoliday("2024-12-25"));
}

TEST_F(DateUtilsTest, IsHolidayIndependenceDay) {
    EXPECT_TRUE(DateUtils::isHoliday("2024-07-04"));
}

TEST_F(DateUtilsTest, IsHolidayNotHoliday) {
    EXPECT_FALSE(DateUtils::isHoliday("2024-01-02"));
}

TEST_F(DateUtilsTest, GetDaysUntilDateFuture) {
    int days = DateUtils::getDaysUntilDate("2040-01-01");
    EXPECT_GT(days, 0);
}

TEST_F(DateUtilsTest, GetDaysUntilDatePast) {
    int days = DateUtils::getDaysUntilDate("2000-01-01");
    EXPECT_LT(days, 0);
}

TEST_F(DateUtilsTest, ValidateDateFormatCorrect) {
    EXPECT_TRUE(DateUtils::validateDateFormat("2024-10-01"));
}

TEST_F(DateUtilsTest, ValidateDateFormatWrongLength) {
    EXPECT_FALSE(DateUtils::validateDateFormat("2024-10-1"));
}

TEST_F(DateUtilsTest, ValidateDateFormatWrongDelimiter) {
    EXPECT_FALSE(DateUtils::validateDateFormat("2024/10/01"));
}

TEST_F(DateUtilsTest, ValidateDateFormatWrongCharacter) {
    EXPECT_FALSE(DateUtils::validateDateFormat("2024-1O-01"));
}

TEST_F(DateUtilsTest, ValidateDateFormatWrongMonth) {
    EXPECT_FALSE(DateUtils::validateDateFormat("2024-13-01"));
}

TEST_F(DateUtilsTest, FormatDateForDisplay) {
    std::string formatted = DateUtils::formatDateForDisplay("2024-05-09");
    EXPECT_EQ(formatted, "09/05/2024");
}