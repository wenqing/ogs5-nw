/**
 * \file SensorData.cpp
 * 2012/08/01 KR Initial implementation
 */

#include "SensorData.h"

#include "StringTools.h"
#include "DateTools.h"
#include <cstdlib>
#include <fstream>


SensorData::SensorData(const std::string &file_name)
: _start(0), _end(0), _step_size(0), _time_unit(TimeStepType::NONE)
{
	this->readDataFromFile(file_name);
}

SensorData::SensorData(std::vector<size_t> time_steps)
: _time_steps(time_steps), _start(time_steps[0]), _end(time_steps[time_steps.size()-1]), _step_size(0), _time_unit(TimeStepType::NONE)
{
	for (size_t i=1; i<time_steps.size(); i++)
	{
		if (time_steps[i-1]>=time_steps[i])
			std::cout << "Error in SensorData() - Time series has no order!" << std::endl;
	}
}

SensorData::SensorData(size_t first_timestep, size_t last_timestep, size_t step_size)
: _start(first_timestep), _end(last_timestep), _step_size(step_size), _time_unit(TimeStepType::NONE)
{
}

SensorData::~SensorData()
{
}


void SensorData::addTimeSeries( const std::string &data_name, std::vector<float> *data, const std::string &data_unit_string ) 
{
	this->addTimeSeries(SensorData::convertString2SensorDataType(data_name), data, data_unit_string);
}

void SensorData::addTimeSeries( SensorDataType::type data_name, std::vector<float> *data, const std::string &data_unit_string ) 
{ 
	if (_step_size>0)
	{
		if (((_end-_start)/_step_size) != data->size())
			std::cout << "Warning in SensorData::addTimeSeries() - Lengths of time series does not match number of time steps." << std::endl;
	}
	else if  (data->size() != _time_steps.size())
		std::cout << "Warning in SensorData::addTimeSeries() - Lengths of time series does not match number of time steps." << std::endl;
	else
	{
		_vec_names.push_back(data_name);
		_data_vecs.push_back(data);
		_data_unit_string.push_back(data_unit_string);
	}
}

const std::vector<float>* SensorData::getTimeSeries(SensorDataType::type time_series_name) const
{
	for (int i=0; i<_vec_names.size(); i++)
	{
		if (time_series_name == _vec_names[i])
			return _data_vecs[i];
	}
	std::cout << "Error in SensorData::getTimeSeries() - Time series \"" << time_series_name << "\" not found." << std::endl;
	return NULL;
}

const std::string SensorData::getDataUnit(SensorDataType::type time_series_name) const
{
	for (int i=0; i<_vec_names.size(); i++)
	{
		if (time_series_name == _vec_names[i])
			return _data_unit_string[i];
	}
	std::cout << "Error in SensorData::getDataUnit() - Time series \"" << time_series_name << "\" not found." << std::endl;
	return "";
}

int SensorData::readDataFromFile(const std::string &file_name)
{
	std::ifstream in( file_name.c_str() );

	if (!in.is_open())
	{
		std::cout << "SensorData::readDataFromFile() - Could not open file...\n";
		return 0;
	}

	std::string line("");

	/* first line contains field names */
	getline(in, line);
	std::list<std::string> fields = splitString(line, '\t');
	std::list<std::string>::const_iterator it (fields.begin());
	size_t nFields = fields.size();
		
	if (nFields<2)
		return 0;
	
	size_t nDataArrays(nFields-1);
	
	//create vectors necessary to hold the data
	for (size_t i=0; i<nDataArrays; i++)
	{
		this->_vec_names.push_back(SensorData::convertString2SensorDataType(*++it));
		this->_data_unit_string.push_back("");
		std::vector<float> *data = new std::vector<float>;
		this->_data_vecs.push_back(data);
	}

	while ( getline(in, line) )
	{
		fields = splitString(line, '\t');

		if (nFields == fields.size())
		{
			it = fields.begin();
			size_t pos(it->rfind("."));
			size_t current_time_step = (pos == std::string::npos) ? atoi((it++)->c_str()) : strDate2int(*it++);
			this->_time_steps.push_back(current_time_step);

			for (size_t i=0; i<nDataArrays; i++)
				this->_data_vecs[i]->push_back(static_cast<float>(strtod((it++)->c_str(), 0)));
		}
		else
			return 0;
	}

	in.close();

	this->_start = this->_time_steps[0];
	this->_end   = this->_time_steps[this->_time_steps.size()-1];

	return 1;
}

std::string SensorData::convertSensorDataType2String(SensorDataType::type t)
{
	if (SensorDataType::EVAPORATION == t) return "Evaporation";
	else if (SensorDataType::PRECIPITATION == t) return "Precipitation";
	else if (SensorDataType::TEMPERATURE == t) return "Temperature";
	// pls leave this as last choice
	else return "Unknown";
}

SensorDataType::type SensorData::convertString2SensorDataType(const std::string &s)
{
	if ((s.compare("Evaporation")==0) || (s.compare("EVAPORATION")==0)) return SensorDataType::EVAPORATION;
	else if ((s.compare("Precipitation")==0) || (s.compare("PRECIPITATION")==0)) return SensorDataType::PRECIPITATION;
	else if ((s.compare("Temperature")==0) || (s.compare("TEMPERATURE")==0)) return SensorDataType::TEMPERATURE;
	else return SensorDataType::OTHER;
}

