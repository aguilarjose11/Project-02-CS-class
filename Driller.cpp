/*
	Lab02 - Driller.cpp
	author: Jose E. Aguilar Escamilla
	version: 19.9.12_01

	Description: This program reads from a redirected input stream using a csv file
	containing drilling information. This program checks three things:
	  1. the datestamps are all the same (based on the first date stamp).
		- Error message: Non-matchingdatestampdat line n.
	  2. the timestamps are unique.
		- Error message: Duplicate time stamp at line n.
	  3. the data in all 17 columns is non-zero and positive.
		- Error message: Invalid floating-point data at line n.
	The data will be printed out using semicolons to separate it as it is being processed.
	Data with errors will not be outputer, and instead it will be ignored and replaced by
	the respective error message.
	The data will be printed in reverse. Last row in the file comes printed first.

	Disclamer: Parts of the code found here comes from the Lab01 Driller.cpp file
	by Jose E. Aguilar Escamilla.
*/


#include <iostream>
#include <string>
//#include <vector>
#include <sstream>
//#include <iomanip>
#include <fstream>
#include "ResizableArray.h"
#include "DrillingRecord.h"
#include "Exceptions.h"
#include "Sorter.h"
#include "DrillingRecordComparator.h"
#include "Search.h"

using namespace std;

// void skip_line();
// bool valid_time(vector<string>&);
// bool valid_date(string);
// bool valid_data(vector<double>&);
// void test_empty_istream();

// Outputs all data in the global data_frame
// depending on the user's input, it outputs to a file or the std::output
void output(ResizableArray<DrillingRecord>& driller_data_frame, int& valid_line_counter, int& memory_entry_counter, int& read_data_counter);

// sorts data depending on the user's input for column.
void sort(ResizableArray<DrillingRecord>& driller_data_frame, int& sorted_column);

// searches the global data_frame and outputs the search finds based on the user's input
void find(ResizableArray<DrillingRecord>& driller_data_frame, int& sorted_column);


// processes the data, changes the counters, valid data is saved, invalid data is announced.
void process_data_file(ResizableArray<DrillingRecord>& driller_data_frame, ifstream& file, int& valid_line_counter, int& memory_entry_counter, int& read_data_counter, string& valid_date);

// reads a single line and puts the data in the output variable.
bool read_single_line(ifstream& file, DrillingRecord* output, int& read_data_counter);

// checks if the given date is valid.
bool date_is_valid(string valid_date, string questionable_date);

// checks if the time is not in the given ResizableArray
bool time_not_in(string questionable_time, ResizableArray<DrillingRecord>& data_frame);

// overloading of the above so that we can know where the found time stamp is at!
bool time_not_in(string questionable_time, ResizableArray<DrillingRecord>& data_frame, int& found_at_index);

// checks to see if the given data is valide: greater than 0.0!
bool data_is_valid(DrillingRecord& data_line);

// linearly searches for the elements in the ResizableArray and returns the number of occurences. 0 if non were found.
template <typename T>
int linearSearch(const T& item, const ResizableArray<T>& array, const Comparator<T>& comparator);

// global variables declarations.


int line = 0;
bool flag = false;


// new main!
int main()
{
	// ResizableArray<DrillingRecord>* data_array = new ResizableArray<DrillingRecord>(); // make sure to delete!
	int valid_line_counter = 0, memory_entry_counter = 0, read_data_counter = 0;
	string valid_date = "";
	
	ResizableArray<DrillingRecord>* driller_data_frame = new ResizableArray<DrillingRecord>();
	if (driller_data_frame == NULL)
	{
		throw new ExceptionMemoryNotAvailable();
	}

	// Data loop.
	while (true)
	{
		string usr_in = "";
		::line = 0;
		cout << "Enter data file name: ";
		getline(cin, usr_in, '\n');
		if (usr_in.compare("") == 0)
		{
			break;
		}

		ifstream file(usr_in);
		if (file.good())
		{
			// read in file
			process_data_file(*driller_data_frame, file, valid_line_counter, memory_entry_counter, read_data_counter, valid_date);
			continue;
		}
		cout << "File is not available." << endl;
	}

	if (driller_data_frame->getSize() > 0)
	{
		Sorter<DrillingRecord>::sort(*driller_data_frame, DrillingRecordComparator(1));
		int sorted_column = -1;
		// Data Manipulation loop
		while (true)
		{
			// return 0;
			string usr_in = "";

			cout << "Enter (o)utput, (s)ort, (f)ind, or (q)uit: ";
			getline(cin, usr_in, '\n');
			cout << endl;

			if (usr_in.compare("o") == 0)
			{
				// output 
				output(*driller_data_frame, valid_line_counter, memory_entry_counter, read_data_counter);

			}
			else if (usr_in.compare("s") == 0)
			{
				// sort
				sort(*driller_data_frame, sorted_column);
			}
			else if (usr_in.compare("f") == 0)
			{
				// find
				find(*driller_data_frame, sorted_column);

			}
			else if (usr_in.compare("q") == 0)
			{
				cout << "Thanks for using Driller." << endl;
				break;
			}
			else if (usr_in.compare("") == 0)
			{
				cout << endl;
				break;
			}
		}
	}
	delete driller_data_frame;
	return 0;
}

// processes the data, changes the counters, valid data is saved, invalid data is announced.
void process_data_file(ResizableArray<DrillingRecord>& driller_data_frame, ifstream& file, int& valid_line_counter, int& memory_entry_counter, int& read_data_counter, string& valid_date)
{
	// variables
	ResizableArray<DrillingRecord>* current_file_data_frame = new ResizableArray<DrillingRecord>();
	if (current_file_data_frame == NULL)
	{
		throw new ExceptionMemoryNotAvailable();
	}

	// code.
	DrillingRecord* first_line = new DrillingRecord();
	if (first_line == NULL)
	{
		throw new ExceptionMemoryNotAvailable();
	}
	int trash;
	read_single_line(file, first_line, trash);
	::line = 0;
	delete first_line;
	first_line = NULL;
	// this is the first file to be processed.
	if (valid_date == "")
	{
		DrillingRecord* first_line = new DrillingRecord();
		if (current_file_data_frame == NULL)
		{
			throw new ExceptionMemoryNotAvailable();
		}

		if (read_single_line(file, first_line, read_data_counter)) // make sure the line was read in!
		{
			// stored valid date!
			valid_date = first_line->getString(0);
		}
		else 
		{
			return; // nothing read in?! bigger problem!
		}
		flag = false; // reset the flag so that messages keep being printed.
		while (!(date_is_valid(valid_date, first_line->getString(0)) && data_is_valid(*first_line))) // loop through the data until a valid row is found
		{
			flag = false; // reset the flag so that messages kep being printed.
			if (!read_single_line(file, first_line, read_data_counter))
			{
				return; // we ran out of lines, the whole file was invalid! we still count the first date!
			}
		}
		valid_line_counter++;
		// the data is ready to added in!
		current_file_data_frame->add(*first_line);
		memory_entry_counter++;

		delete first_line;
		first_line = NULL;
	}
	else // this is the first line in this file then! assume that current_file_data_frame is empty.
	{
		// reading when there is an official date
		DrillingRecord* first_line = new DrillingRecord();
		if (read_single_line(file, first_line, read_data_counter)) // make sure the line was read in!
		{
			flag = false; // reset the flag so that messages keep being printed.
			if (date_is_valid(valid_date, first_line->getString(0)))
			{
				flag = false; // reset the flag so that messages keep being printed.
				while (!(date_is_valid(valid_date, first_line->getString(0)) && data_is_valid(*first_line))) // no need to check timestamps since its the first line! to be added.
				{
					flag = false; // reset the flag so that messages keep being printed.
					if (!read_single_line(file, first_line, read_data_counter))
					{
						delete current_file_data_frame;
						return; // we ran out of lines, the whole file was invalid! we still count the first date!
					}
				} // this loops until a valid data row is found
				// if outside?

				valid_line_counter++;
				flag = true; // no message of error in case we find the actual duplicate outside this file!
				if (!time_not_in(first_line->getString(1), driller_data_frame))
				{
					int row = 0;
					time_not_in(first_line->getString(1), driller_data_frame, row);
					driller_data_frame.removeAt(row);
					memory_entry_counter--;
				}
				flag = false; // reset the flag so that messages keep being printed.

				// the data is ready to added in!
				current_file_data_frame->add(*first_line);
				memory_entry_counter++;
			}
			else // the first date of the file was a mismatch!
			{
				cout << "Date mismatch; file closed." << endl;
				delete current_file_data_frame;
				return;
			}
			delete first_line;
			first_line = NULL;
		}
		else
		{
			delete first_line;
			first_line = NULL;
		}
	}
	DrillingRecord* data_row = new DrillingRecord();


	// loop for the whole file!!! At this point there is at least an entry in the current_file_data_frame!
	while (read_single_line(file, data_row, read_data_counter))
	{
		flag = false;
		if (date_is_valid(valid_date, data_row->getString(0)) &&
			time_not_in(data_row->getString(1), *current_file_data_frame) &&
			data_is_valid(*data_row))
		{
			valid_line_counter++;
			flag = true; // no message of error in case we find the actual duplicate outside this file!
			if (!time_not_in(data_row->getString(1), driller_data_frame))
			{
				int row = 0;
				time_not_in(data_row->getString(1), driller_data_frame, row);
				driller_data_frame.removeAt(row);
				memory_entry_counter--;
			}
			memory_entry_counter++;
			flag = false; // reset the flag so that messages keep being printed.
			current_file_data_frame->add(*data_row);
		}
	}
	// there is at least a single element in the data frame so add it!
	delete data_row;
	data_row = NULL;

	// merging the data from this run!
	for (unsigned int row = 0; row < current_file_data_frame->getSize(); row++)
	{
		driller_data_frame.add(current_file_data_frame->get(row));
	}

	// release memory!
	delete current_file_data_frame;
	current_file_data_frame = NULL;
}

bool  read_single_line(ifstream& file, DrillingRecord* output, int& read_data_counter)
{
	string line, data_cell;
	if (getline(file, line, '\n'))
	{
		istringstream iss(line);
		int column = 0;
		for (; column < 2; column++)
		{
			if (getline(iss, data_cell, ','))
			{
				output->setString(data_cell, column);
			}
		}
		double double_data;
		for (column = 0; column < 15; column++)
		{
			double_data = -999.0;
			if (getline(iss, data_cell, ','))
			{
				try
				{
					double_data = stod(data_cell);
					output->setNum(double_data, column);
				}
				catch (invalid_argument &)
				{
					output->setNum(double_data, column);
				}
			}
		}

		double_data = -999.0;
		if (getline(iss, data_cell, '\n'))
		{
			try
			{
				double_data = stod(data_cell);
				output->setNum(double_data, column);
			}
			catch (invalid_argument &)
			{
				output->setNum(double_data, column);
			}
		}

		::line++;
		read_data_counter++;
		return true;
	}
	else 
	{
		// no data? error!
		return false;
	}
}


// checks if the given date is valid.
bool date_is_valid(string valid_date, string questionable_date)
{
	if (valid_date.compare(questionable_date) != 0)
	{
		if (!flag)
		{
			flag = true;
			cout << "Non-matching datestamp " << questionable_date << " at line " << ::line << "." << endl;
		}
		return false;
	}
	return true;
}

// checks if the time is not in the given ResizableArray
// make flag true to avoid message printing!
bool time_not_in(string questionable_time, ResizableArray<DrillingRecord>& data_frame)
{
	for (unsigned int row = 0; row < data_frame.getSize(); row++)
	{
		if (data_frame.get(row).getString(1).compare(questionable_time) == 0)
		{
			if (!flag)
			{
				cout << "Duplicate timestamp " << questionable_time << " at line " << line << "." << endl;
				flag = true;
			}
			return false;
		}
	}
	return true;
}

// overloading of the above so that we can know where the found time stamp is at!
bool time_not_in(string questionable_time, ResizableArray<DrillingRecord>& data_frame, int& found_at_index)
{
	for (unsigned int row = 0; row < data_frame.getSize(); row++)
	{
		if (data_frame.get(row).getString(1).compare(questionable_time) == 0)
		{
			if (!flag)
			{
				cout << "Duplicate timestamp " << questionable_time << " at line " << line << "." << endl;
				flag = true;
			}
			found_at_index = row;
			return false;
		}
	}
	found_at_index = -1;
	return true;
}

// checks to see if the given data is valide: greater than 0.0!
bool data_is_valid(DrillingRecord& data_line)
{
	for (int col = 0; col < 16; col++)
	{
		if (data_line.getNum(col) <= 0.0)
		{
			if (!flag)
			{
				cout << "Invalid floating-point data at line " << line << "." << endl;
				flag = false;
			}
			return false;
		}
	}
	return true;
}

// depending on the user's input, it outputs to a file or the std::output
void output(ResizableArray<DrillingRecord>& driller_data_frame, int& valid_line_counter, int& memory_entry_counter, int& read_data_counter)
{
	while (true)
	{
		cout << "Enter output file name: ";
		string input = "";
		getline(cin, input, '\n');
		if (input.compare("") == 0)
		{
			// print all the data to cout...
			for (unsigned int row = 0; row < driller_data_frame.getSize(); row++)
			{
				cout << driller_data_frame[row] << endl;
			}
			cout << "Data lines read: " << read_data_counter << "; Valid drilling records read: " << valid_line_counter << "; Drilling records in memory: " << memory_entry_counter << endl;
			break;
		}
		ofstream file(input);
		if (file.good())
		{

			// print to the file
			for (unsigned int row = 0; row < driller_data_frame.getSize(); row++)
			{
				file << driller_data_frame[row] << endl;
			}
			file << "Data lines read: " << read_data_counter << "; Valid drilling records read: " << valid_line_counter << "; Drilling records in memory: " << memory_entry_counter << endl;
			break;
		}
		else 
		{
			cout << "File is not available." << endl;
		}
	}
}

// sorts data depending on the user's input for column.
void sort(ResizableArray<DrillingRecord>& driller_data_frame, int& sorted_column)
{
	cout << "Enter sort field (0-17): ";
	string input = "";
	getline(cin, input, '\n');
	int column = 0; 
	try
	{
		column = stoi(input);
	}
	catch (invalid_argument &)
	{
		return;
	}
	if (column >= 0 && column <= 17)
	{
		Sorter<DrillingRecord>::sort(driller_data_frame, DrillingRecordComparator(column));
		sorted_column = column;
	}
	else
	{
		return;
	}
}

// searches the global data_frame and outputs the search finds based on the user's input
void find(ResizableArray<DrillingRecord>& driller_data_frame, int& sorted_column)
{

	cout << "Enter search field (0-17): ";
	string input = "";
	getline(cin, input, '\n');
	int column = 0;
	try
	{
		column = stoi(input);
	}
	catch (invalid_argument &)
	{
		return;
	}
	if (column >= 0 && column <= 17)
	{
		if (column <= 1)
		{
			cout << "Enter exact text on which to search: ";
			string text_to_search = "";
			getline(cin, text_to_search, '\n');
			DrillingRecord value_to_find = DrillingRecord();
			value_to_find.setString(text_to_search, column);
			if (sorted_column == column)
			{
				// copying in a deep way the whole array.
				ResizableArray<DrillingRecord> copy_driller_data_frame;
				for (unsigned int row = 0; row < driller_data_frame.getSize(); row++)
				{
					DrillingRecord* temp = new DrillingRecord();;
					for (int i = 0; i < 2; i++)
					{
						temp->addString(driller_data_frame.get(row).getString(i));
					}
					for (int i = 0; i < 16; i++)
					{
						temp->addNum(driller_data_frame.get(row).getNum(i));
					}
					copy_driller_data_frame.add(*temp);
					delete temp;
				}
				int counter = 0;
				while (true)
				{
					if (copy_driller_data_frame.getSize() == 0)
					{
						break;
					}
					int index_of_find = (int)binarySearch(value_to_find, copy_driller_data_frame, DrillingRecordComparator(column));
					if (index_of_find >= 0)
					{
						counter++;
						cout << copy_driller_data_frame[index_of_find]<< endl;
						copy_driller_data_frame.removeAt(index_of_find);
					}
					else
					{
						break;
					}
				}
				cout << "Drilling records found: " << counter << "." << endl;
				// we got the number
			}
			else
			{
				cout << "Drilling records found: " << linearSearch(value_to_find, driller_data_frame, DrillingRecordComparator(column)) << "." << endl;
			}
		}
		else
		{
			cout << "Enter positive field value: ";
			string to_search = "";
			double value_to_search = -1;
			getline(cin, to_search, '\n');
			try
			{
				value_to_search = stod(to_search);
			}
			catch (invalid_argument &)
			{
				return; // 
			}
			if (value_to_search > 0.0)
			{
				DrillingRecord value_to_find = DrillingRecord();
				value_to_find.setNum(value_to_search, column-2);
				if (sorted_column == column)
				{
					// copying in a deep way the whole array.
					ResizableArray<DrillingRecord> copy_driller_data_frame;
					for (unsigned int row = 0; row < driller_data_frame.getSize(); row++)
					{
						DrillingRecord* temp = new DrillingRecord();;
						for (int i = 0; i < 2; i++)
						{
							temp->addString(driller_data_frame.get(row).getString(i));
						}
						for (int i = 0; i < 16; i++)
						{
							temp->addNum(driller_data_frame.get(row).getNum(i));
						}
						copy_driller_data_frame.add(*temp);
						delete temp;
					}
					int counter = 0;
					while (true)
					{
						if (copy_driller_data_frame.getSize() == 0)
						{
							break;
						}
						signed int index_of_find = (signed int)binarySearch(value_to_find, copy_driller_data_frame, DrillingRecordComparator(column));
						if (index_of_find < 0)
						{
							break;
						}
						counter++;
						cout << copy_driller_data_frame[index_of_find] << endl;
						copy_driller_data_frame.removeAt(index_of_find);
					}
					cout << "Drilling records found: " << counter << "." << endl;
					// we got the number
				}
				else
				{
					cout << "Drilling records found: " << linearSearch(value_to_find, driller_data_frame, DrillingRecordComparator(column)) << "." << endl;
				}
			}
			else
			{
				return; // wrong data
			}
		}
	}
	else
	{
		return;
	}

}

// linearly searches for the elements in the ResizableArray and returns the number of occurences. 0 if non were found.
template <typename T>
int linearSearch(const T& item, const ResizableArray<T>& array, const Comparator<T>& comparator)
{
	int found_cases = 0;
	for (unsigned int row = 0; row < array.getSize(); row++)
	{
		if (comparator.compare(item, array[row]) == 0)
		{
			cout << array[row] << endl;
			found_cases++;
		}
	}
	return found_cases;
}