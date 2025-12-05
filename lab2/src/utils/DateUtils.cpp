#include "../../include/utils/DateUtils.h"
#include <sstream>
#include <iomanip>
#include <cmath>

std::string DateUtils::getCurrentDate() {
    std::time_t now = std::time(nullptr);
    std::tm* timeinfo = std::localtime(&now);
    
    std::ostringstream oss;
    oss << (timeinfo->tm_year + 1900) << "-" 
        << std::setw(2) << std::setfill('0') << (timeinfo->tm_mon + 1) << "-"
        << std::setw(2) << std::setfill('0') << timeinfo->tm_mday;
    
    return oss.str();
}

std::string DateUtils::addDaysToDate(const std::string& date, int days) {
    int year, month, day;
    char dash;
    std::istringstream iss(date);
    iss >> year >> dash >> month >> dash >> day;
    
    day += days;
    
    while (day > 30) {
        day -= 30;
        month++;
        if (month > 12) {
            month = 1;
            year++;
        }
    }
    
    std::ostringstream oss;
    oss << year << "-" 
        << std::setw(2) << std::setfill('0') << month << "-"
        << std::setw(2) << std::setfill('0') << day;
    
    return oss.str();
}

int DateUtils::calculateDateDifference(const std::string& startDate, const std::string& endDate) {
    int startYear, startMonth, startDay;
    int endYear, endMonth, endDay;
    char dash;
    
    std::istringstream startIss(startDate);
    startIss >> startYear >> dash >> startMonth >> dash >> startDay;
    
    std::istringstream endIss(endDate);
    endIss >> endYear >> dash >> endMonth >> dash >> endDay;
    
    int startTotalDays = startYear * 365 + startMonth * 30 + startDay;
    int endTotalDays = endYear * 365 + endMonth * 30 + endDay;
    
    return endTotalDays - startTotalDays;
}

bool DateUtils::isDateInFuture(const std::string& date) {
    std::string currentDate = getCurrentDate();
    return calculateDateDifference(currentDate, date) > 0;
}

bool DateUtils::isWeekend(const std::string& date) {
    int year, month, day;
    char dash;
    std::istringstream iss(date);
    iss >> year >> dash >> month >> dash >> day;
    
    if (month < 3) {
        month += 12;
        year--;
    }
    int k = year % 100;
    int j = year / 100;
    int dayOfWeek = (day + 13*(month+1)/5 + k + k/4 + j/4 + 5*j) % 7;
    
    return dayOfWeek == 0 || dayOfWeek == 1;
}

std::string DateUtils::getSeason(const std::string& date) {
    int year, month, day;
    char dash;
    std::istringstream iss(date);
    iss >> year >> dash >> month >> dash >> day;
    
    if (month >= 3 && month <= 5) return "SPRING";
    else if (month >= 6 && month <= 8) return "SUMMER";
    else if (month >= 9 && month <= 11) return "AUTUMN";
    else return "WINTER";
}

bool DateUtils::isHoliday(const std::string& date) {
    int year, month, day;
    char dash;
    std::istringstream iss(date);
    iss >> year >> dash >> month >> dash >> day;
    
    if (month == 1 && day == 1) return true;  
    if (month == 12 && day == 25) return true; 
    if (month == 7 && day == 4) return true;  
    
    return false;
}

int DateUtils::getDaysUntilDate(const std::string& targetDate) {
    std::string currentDate = getCurrentDate();
    return calculateDateDifference(currentDate, targetDate);
}

bool DateUtils::validateDateFormat(const std::string& date) {
    if (date.length() != 10) return false;
    if (date[4] != '-' || date[7] != '-') return false;
    
    for (int i = 0; i < 10; i++) {
        if (i == 4 || i == 7) continue;
        if (!std::isdigit(date[i])) return false;
    }
    
    int year, month, day;
    char dash;
    std::istringstream iss(date);
    iss >> year >> dash >> month >> dash >> day;
    
    if (year < 2000 || year > 2100) return false;
    if (month < 1 || month > 12) return false;
    if (day < 1 || day > 31) return false;
    
    return true;
}

std::string DateUtils::formatDateForDisplay(const std::string& date) {
    int year, month, day;
    char dash;
    std::istringstream iss(date);
    iss >> year >> dash >> month >> dash >> day;
    
    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << day << "/"
        << std::setw(2) << std::setfill('0') << month << "/"
        << year;
    
    return oss.str();
}