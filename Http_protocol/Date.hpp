#ifndef DATE_HPP
#define DATE_HPP
#include <string>
#include <time.h>
#include <sstream>
#include <iostream>

class Date
{
	public :
		Date();
		std::string get_date();
		
	private:
		std::string	date;
};
#endif