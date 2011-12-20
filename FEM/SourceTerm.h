/**
 * \file SourceTerm.h
 * 2011/08/30 KR inital implementation
 *
 */

#ifndef SOURCETERM_H
#define SOURCETERM_H

#include "FEMCondition.h"

/**
 * \brief Adapter class for handling boundary conditions in the user Interface
 * \sa FEMCondition
 */
class SourceTerm : public FEMCondition
{
public:
	SourceTerm(const std::string &geometry_name)
		: FEMCondition(geometry_name, FEMCondition::SOURCE_TERM), _tim_type(0) {}
	SourceTerm(const CSourceTerm &st, const std::string &geometry_name);
	SourceTerm(const FEMCondition &cond)
		: FEMCondition(cond, FEMCondition::SOURCE_TERM) {};
	~SourceTerm() {}

	size_t getTimType() const {return _tim_type; }
	void setTimType(size_t value) { _tim_type = value; }

	/**
	 * Reads DIRECT source terms and creates conditions located on points that coincide with mesh nodes.
	 * \param st_vector Vector of source terms
	 * \param geo_name Name of the geometry the sourceterms are referring to
	 * \param file_path Path to the node-value files (necessary since st-files only give the filename but not the path)
	 */
	static std::vector<FEMCondition*> createDirectSourceTerms(
	        const std::vector<CSourceTerm*> &st_vector,
	        const std::string &geo_name,
			const std::string &file_path);

private:
	static void getDirectNodeValues(const std::string &filename,
	                                std::vector< std::pair<size_t, double> > &nodes_values);
	size_t _tim_type;
};

#endif //SOURCETERM_H
