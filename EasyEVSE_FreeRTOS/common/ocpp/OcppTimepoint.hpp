/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef OCPPTIMEPOINT_HPP_
#define OCPPTIMEPOINT_HPP_

#define SECONDS_IN_DAY 86400
#define SECONDS_IN_HOUR 3600
#define SECONDS_IN_MINUTE 60

extern "C" {
extern uint32_t EVSE_Connectivity_GetUnixTime(void);
}

static bool is_leap_year(int year) {
    // Leap year if divisible by 4 and (not divisible by 100 or divisible by 400)
    if ((year % 4 == 0) && ((year % 100 != 0) || (year % 400 == 0))) {
        return true;
    } else {
        return false;
    }
}

class OcppTimepoint
{
public:
	int year;
	char month;
	char day;
	char hours;
	char minutes;
	char seconds;

	unsigned long timestamp_s;

	OcppTimepoint() : OcppTimepoint(EVSE_Connectivity_GetUnixTime()) {
	}

	OcppTimepoint(unsigned long timestamp){
		    timestamp_s = timestamp;

		    // Start from Unix epoch
		    year = 1970;
		    unsigned long _day = timestamp / SECONDS_IN_DAY;
		    unsigned long seconds_remaining = timestamp % SECONDS_IN_DAY;

		    // Calculate year
		    while (1) {
		        unsigned int days_in_year = 365;
		        if (is_leap_year(year))
		        {
		        	days_in_year = 366;
		        }

		        if (_day >= days_in_year)
		        {
		            _day -= days_in_year;
		            year++;
		        } else {
		            break;
		        }
		    }

		    // Days in each month
		    unsigned int month_days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
		    if (is_leap_year(year)) {
		        month_days[1] = 29;
		    }

		    // Calculate month
		    unsigned int _month = 0;
		    while (_day > month_days[_month]) {
		        _day -= month_days[_month];
		        _month++;
		    }
		    _month++;
		    _day = _day + 1;

		    day = _day;
		    month = _month;
		    hours = seconds_remaining / SECONDS_IN_HOUR;
		    seconds_remaining %= SECONDS_IN_HOUR;
		    minutes = seconds_remaining / SECONDS_IN_MINUTE;
		    seconds = seconds_remaining % SECONDS_IN_MINUTE;

	}

	OcppTimepoint(int year, char month, char day, char hours, char minutes, char seconds): year(year), month(month), day(day), hours(hours), minutes(minutes), seconds(seconds){
	}

	~OcppTimepoint(){
	}

	unsigned long int to_time_point_s(void) const
	{
		return this->timestamp_s;
	}

	// Addition operator
	OcppTimepoint operator+(const OcppTimepoint& other) const {
		int totalMonths = month + other.month;
		int totalDays = day + other.day;
		int totalSeconds = seconds + other.seconds;
		int totalMinutes = minutes + other.minutes + totalSeconds / 60;
		int totalHours = hours + other.hours + totalMinutes / 60;
		int totalYear = year;
		if (other.year > year)
		{
			totalYear = other.year;
		}

		return OcppTimepoint(totalYear,
				totalDays % 30,
				totalMonths % 12,
				totalHours % 24,       // wrap around 24 hours if needed
				totalMinutes % 60,
				totalSeconds % 60
		);
	}

	OcppTimepoint operator+(int additionalSeconds) const {
//		int totalSeconds = hours * 3600 + minutes * 60 + seconds + additionalSeconds;
//
//		int newHours = (totalSeconds / 3600) % 24;
//		int newMinutes = (totalSeconds / 60) % 60;
//		int newSeconds = totalSeconds % 60;
//
//		unsigned long totalSeconds =
//
//		return OcppTimepoint(newHours, newMinutes, newSeconds);

		OcppTimepoint result = *this;

		int totalSeconds = result.seconds + additionalSeconds;
		result.seconds = totalSeconds % 60;
		if (result.seconds == 0)
		{
			result.minutes += 1;
		}

		int totalMinutes = result.minutes + (totalSeconds / SECONDS_IN_MINUTE);
		result.minutes = totalMinutes % 60;
		if (result.minutes == 0)
		{
			result.hours += 1;
		}

		int totalHours = result.hours + (totalMinutes / 60);
		result.hours = totalHours % 60;

		if (totalHours > 24)
		{
			result.day += 1;
		}

	    // Days in each month
	    unsigned int month_days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	    if (is_leap_year(result.year)) {
	        month_days[1] = 29;
	    }
		if (result.day > month_days[static_cast<int>(result.month)])
		{
			result.day = 1;
			result.month += 1;
		}

		if (result.month > 12)
		{
			result.year += 1;
			result.month = result.month % 12;
		}

		return result;
	}

	// Substraction operator
	OcppTimepoint operator-(const OcppTimepoint& other) const {
		 OcppTimepoint result;
		 return result;
	}

	bool operator<(const OcppTimepoint& other) const {
		if (year != other.year)
			return year < other.year;
		if (month != other.month)
			return month < other.month;
		if (day != other.day)
			return day < other.day;
		if (hours != other.hours)
			return hours < other.hours;
		if (minutes != other.minutes)
			return minutes < other.minutes;
		return seconds < other.seconds;
	}

	bool operator>(const OcppTimepoint& other) const {
		if (year != other.year)
			return year > other.year;
		if (month != other.month)
			return month > other.month;
		if (day != other.day)
			return day > other.day;
		if (hours != other.hours)
			return hours > other.hours;
		if (minutes != other.minutes)
			return minutes > other.minutes;
		return seconds > other.seconds;
	}

private:

};

#endif /* OCPPTIMEPOINT_HPP_ */
