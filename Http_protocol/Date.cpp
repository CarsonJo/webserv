#include "Date.hpp"


static std::string	week_day(int a)
{
	if (a == 0)
		return ("Sun, ");
	if (a == 1)
		return ("Mon, ");
	if (a == 2)
		return ("Tue, ");
	if (a == 3)
		return ("Wed, ");
	if (a == 4)
		return ("Thu, ");
	if (a == 5)
		return ("Fri, ");
	if (a == 6)
		return ("Sat, ");
	return ("???, ");
}

static std::string	month(int a)
{
	if (a == 0)
		return (" Jan ");
	if (a == 1)
		return (" Feb ");
	if (a == 2)
		return (" Mar ");
	if (a == 3)
		return (" Apr ");
	if (a == 4)
		return (" May ");
	if (a == 5)
		return (" Jun ");
	if (a == 6)
		return (" Jul");
	if (a == 7)
		return (" Aug ");
	if (a == 8)
		return (" Sep ");
	if (a == 9)
		return (" Oct ");
	if (a == 10)
		return (" Nov ");
	if (a == 11)
		return (" Dec ");
	return (" ??? ");
}

static std::string year(int a)
{
	std::stringstream	s;

	s << a + 1900;
	return (s.str());
}

static std::string to_string(int a)
{
	std::stringstream	s;

	s << a;
	return (s.str());
}

static std::string hour_time(struct tm* ptm)
{
	std::stringstream s;

	s << " " << ptm->tm_hour << ":" << ptm->tm_min << ":" << ptm->tm_sec << " GMT\r\n";
	return (s.str());
}

Date::Date() : date("")
{
	time_t		rtime;
	struct tm	*ptm;

	time(&rtime);
	ptm = gmtime(&rtime);
	date.append("Date : ").append(week_day(ptm->tm_wday)).append(to_string(ptm->tm_mday))
	.append(month(ptm->tm_mon)).append(year(ptm->tm_year)).append(hour_time(ptm));
	std::cerr << "date :" << date << std::endl;
}

std::string Date::get_date()
{
	return (date);
}