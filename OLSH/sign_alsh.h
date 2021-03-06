#ifndef __SIGN_ALSH_H
#define __SIGN_ALSH_H

class SRP_LSH;

// -----------------------------------------------------------------------------
//  Sign-LSH is used to solve the problem of c-Approximate Maximum Inner 
//  Product (c-AMIP) search
//
//  the idea was introduced by Anshumali Shrivastava and Ping Li in their paper 
//  "Improved asymmetric locality sensitive hashing (ALSH) for Maximum Inner 
//  Product Search (MIPS)", In Proceedings of the Thirty-First Conference on 
//  Uncertainty in Artificial Intelligence (UAI), pages 812–821, 2015.
// -----------------------------------------------------------------------------
class Sign_ALSH {
public:
	Sign_ALSH();					// default constructor
	~Sign_ALSH();					// destrcutor

	// -------------------------------------------------------------------------
	void build(						// build index
		int   n,						// number of data objects
		int   d,						// dimension of data objects
		int   K,						// number of hash tables
		int   m,						// additional dimension of data
		float U,						// scale factor for data
		float ratio,					// approximation ratio for AMC search
		const float** data);	 		// data objects

	// -------------------------------------------------------------------------
	int kmip(						// c-k-AMIP search
		int   top_k,					// top-k value
		const float* query,				// input query
		MaxK_List* list);				// top-k mip results

protected:
	int   n_pts_;					// number of data points
	int   dim_;						// dimension of data
	int   K_;						// number of hash tables
	int   m_;						// additional dimension of data
	float U_;						// scale factor
	float appr_ratio_;				// approximation ratio
	const float **data_;			// data objects

	float M_;						// max norm of data objects
	int   sign_alsh_dim_;			// dimension of sign_alsh data
	float **sign_alsh_data_;		// sign_alsh data
	SRP_LSH *lsh_;					// SRP_LSH

	// -------------------------------------------------------------------------
	int bulkload();					// bulkloading

	// -------------------------------------------------------------------------
	void display();					// display parameters
};

#endif // __SIGN_ALSH_H
