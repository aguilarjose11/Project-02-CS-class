/*
	Definition of the DrillingRecordComparator class.
	Author: Dr. Hougen, Jose E. Aguilar Escamilla
	Version: 19.10.1_01
*/

#include "DrillingRecordComparator.h"

DrillingRecordComparator::DrillingRecordComparator(unsigned int column)
{	
	this->column = column;
}

int DrillingRecordComparator::compare(const DrillingRecord& item1, const DrillingRecord& item2) const
{
	if (this->column <= 1)
	{
		// return item1.getString(this->column).compare(item2.getString(this->column));
		if (item1.getString(this->column) < item2.getString(this->column))
		{
			return -1;
		}
		else if (item1.getString(this->column) > item2.getString(this->column))
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return (item1.getNum(this->column - 2) < item2.getNum(this->column - 2)) ? -1 : (item1.getNum(this->column - 2) > item2.getNum(this->column - 2) ? 1 : 0);
	}
}