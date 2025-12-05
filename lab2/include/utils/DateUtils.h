#ifndef DATEUTILS_H
#define DATEUTILS_H

#include <string>
#include <ctime>

class DateUtils {
public:
    static std::string getCurrentDate();
    static std::string addDaysToDate(const std::string& date, int days);
    static int calculateDateDifference(const std::string& startDate, const std::string& endDate);
    static bool isDateInFuture(const std::string& date);
    static bool isWeekend(const std::string& date);
    static std::string getSeason(const std::string& date);
    static bool isHoliday(const std::string& date);
    static int getDaysUntilDate(const std::string& targetDate);
    static bool validateDateFormat(const std::string& date);
    static std::string formatDateForDisplay(const std::string& date);
    
private:
    DateUtils() = delete; 
};

#endif