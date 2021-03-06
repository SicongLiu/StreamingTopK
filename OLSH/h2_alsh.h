#ifndef __H2_ALSH_H
#define __H2_ALSH_H

#include <vector>
using namespace std;

class QALSH;
class MaxK_List;

// -----------------------------------------------------------------------------
//  Assistant Data Structure for H2-ALSH
// -----------------------------------------------------------------------------
struct Block {
	int   n_pts_;
	float M_;
	int   *index_;
	QALSH *lsh_;

	Block() { n_pts_ = 0; M_ = 0; index_ = NULL; lsh_ = NULL; }
	~Block() {
		if (index_ != NULL) { delete[] index_; index_ = NULL; }
		if (lsh_ != NULL) { delete lsh_; lsh_ = NULL; }
	}
};

// -----------------------------------------------------------------------------
//  H2_ALSH is used to solve the problem of c-Approximate Maximum Inner Product 
//  (c-AMIP) search
// -----------------------------------------------------------------------------
class H2_ALSH {
public:
	H2_ALSH();						// default constructor
	~H2_ALSH();						// destructor

	// -------------------------------------------------------------------------
	void build(						// build index
		int   n,						// number of data objects
		int   d,						// dimension of data objects
		float nn_ratio,					// approximation ratio for NN
		float mip_ratio,				// approximation ratio for MIP
		const float **data);			// input data

	// -------------------------------------------------------------------------
	int kmip(						// c-k-AMIP search
		int   top_k,					// top-k value
		const float *query,				// input query
		MaxK_List *list);				// top-k MIP results (return) 

protected:
	int   n_pts_;					// number of data objects
	int   dim_;						// dimension of data objects
	float nn_ratio_;				// approximation ratio for NN
	float mip_ratio_;				// approximation ratio for MIP
	const float **data_;			// original data objects

	float b_;						// compression ratio
	float M_;						// max norm of the data objects
	int   h2_alsh_dim_;				// dimension of h2_alsh data (dim_ + 1)
	float **h2_alsh_data_;			// h2_alsh data
	int   num_blocks_;				// number of blocks
	vector<Block*> blocks_;			// blocks
	
	// -------------------------------------------------------------------------
	int bulkload();					// bulkloading

	// -------------------------------------------------------------------------
	void display();					// display parameters
};

#endif // __H2_ALSH_H
