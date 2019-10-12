#include "DrillingRecord.h"
#include "Exceptions.h"
#include <iostream>
#include <iomanip>

 

std::ostream& operator<<(std::ostream& os, const DrillingRecord& record)
{
	os << record.getString(0) << ';' << record.getString(1); // possible error!
	for (int index = 0; index < 16; index++)
	{
		os << std::fixed << std::setprecision(2) << ';' << record.getNum(index);
	}
	return os;
}

void DrillingRecord::addNum(double num)
{
	this->nums[this->numCtr++] = num;
}

void DrillingRecord::addString(std::string string)
{
	this->strings[this->strCtr++] = string;
}

double DrillingRecord::getNum(unsigned int index) const
{
	if (index >= MAX_NUMS)
	{
		throw new ExceptionIndexOutOfRange();
	}
	return this->nums[index];
}

std::string  DrillingRecord::getString(unsigned int index) const
{
	if (index > 1)
	{
		throw new ExceptionIndexOutOfRange();
	}
	return this->strings[index];
}

// Driller v1.0 TODO.

void DrillingRecord::setNum(double num, unsigned int index)
{
	this->nums[index] = num;
}

void DrillingRecord::setString(std::string string, unsigned int index)
{
	this->strings[index] = string;
}

DrillingRecord::DrillingRecord()
{
	nums[0] = 0.0;
	nums[1] = 0.0;
	nums[2] = 0.0;
	nums[3] = 0.0;
	nums[4] = 0.0;
	nums[5] = 0.0;
	nums[6] = 0.0;
	nums[7] = 0.0;
	nums[8] = 0.0;
	nums[9] = 0.0;
	nums[10] = 0.0;
	nums[11] = 0.0;
	nums[12] = 0.0;
	nums[13] = 0.0;
	nums[14] = 0.0;
	nums[15] = 0.0;
	strings[0] = "";
	strings[1] = "";

}