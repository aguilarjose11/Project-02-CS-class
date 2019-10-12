#ifndef DRILLING_RECORD_COMPARATOR_H
#define DRILLING_RECORD_COMPARATOR_H

#include "Comparator.h"
#include "DrillingRecord.h"

class DrillingRecordComparator : public Comparator<DrillingRecord> {
private:
	unsigned int column = 0;
public:
	DrillingRecordComparator(unsigned int column);
	int compare(const DrillingRecord& item1, const DrillingRecord& item2) const;
};

#endif