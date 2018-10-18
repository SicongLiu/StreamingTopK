#include "headers.h"

// -----------------------------------------------------------------------------
Simple_LSH::Simple_LSH()			// default constructor
{
	n_pts_           = -1;
	dim_             = -1;
	K_               = -1;
	L_               = -1;
	S_               = -1.0f;
	appr_ratio_      = -1.0f;
	data_            = NULL;
	M_               = -1.0f;
	simple_lsh_dim_  = -1;
	simple_lsh_data_ = NULL;
	lsh_             = NULL;
}

// -----------------------------------------------------------------------------
Simple_LSH::~Simple_LSH()			// destructor
{
	if (simple_lsh_data_ != NULL) {
		for (int i = 0; i < n_pts_; ++i) {
			delete[] simple_lsh_data_[i]; simple_lsh_data_[i] = NULL;
		}
		delete[] simple_lsh_data_; simple_lsh_data_ = NULL;
	}

	if (lsh_ != NULL) {
		delete lsh_; lsh_ = NULL;
	}
}

// -----------------------------------------------------------------------------
void Simple_LSH::build(				// build index
	int   n,							// number of data
	int   d,							// dimension of data
	int   K,							// number of hash tables
	int   L,							// number of hash layers
	float   S,							// similarity threshold
	float ratio,						// approximation ratio
	const float** data)					// data objects
{
	// -------------------------------------------------------------------------
	//  init parameters
	// -------------------------------------------------------------------------
	n_pts_          = n;
	dim_            = d;
	K_              = K;
	L_              = L;
	S_				= S;
	appr_ratio_     = ratio;
	data_           = data;
	simple_lsh_dim_ = d + 1;

	// -------------------------------------------------------------------------
	//  build index
	// -------------------------------------------------------------------------
	bulkload();
	display();
}

// -----------------------------------------------------------------------------
int Simple_LSH::bulkload()			// bulkloading
{
	// -------------------------------------------------------------------------
	//  calculate the Euclidean norm of data and find the maximum norm of data
	// -------------------------------------------------------------------------
	M_ = MINREAL;
	vector<float> norm(n_pts_, 0.0f);

	for (int i = 0; i < n_pts_; ++i) {
		norm[i] = sqrt(calc_inner_product(dim_, data_[i], data_[i]));
		if (norm[i] > M_) M_ = norm[i];
	}

	// -------------------------------------------------------------------------
	//  construct new format of data
	// -------------------------------------------------------------------------
	float scale = 1.0f / M_;
	int exponent = -1;

	printf("Construct Simple_LSH Data\n\n");

	simple_lsh_data_ = new float*[n_pts_];
	for (int i = 0; i < n_pts_; ++i) {
		simple_lsh_data_[i] = new float[simple_lsh_dim_];

		norm[i] = norm[i] * scale;
		for (int j = 0; j < simple_lsh_dim_; ++j)
		{
			if (j < dim_)
			{
				simple_lsh_data_[i][j] = data_[i][j] * scale;
			}
			else {
				simple_lsh_data_[i][j] = sqrt(1.0f - norm[i] * norm[i]);
			}
		}
	}

	// -------------------------------------------------------------------------
	//  indexing the new data using SRP-LSH
	// -------------------------------------------------------------------------
	lsh_ = new SRP_LSH(n_pts_, simple_lsh_dim_, K_, L_, S_,
		(const float **) simple_lsh_data_);

	return 0;
}

// -----------------------------------------------------------------------------
void Simple_LSH::display()			// display parameters
{
	printf("Parameters of Simple_LSH:\n");
	printf("    n = %d\n", n_pts_);
	printf("    d = %d\n", dim_);
	printf("    K = %d\n", K_);
	printf("    L = %d\n", L_);
	printf("    c = %.2f\n", appr_ratio_);
	printf("    M = %.2f\n", M_);
	printf("\n");
}

// -----------------------------------------------------------------------------
int Simple_LSH::kmip(				// c-k-AMIP search
	int   top_k,						// top-k value
	const float *query,				// input query
	MaxK_List *list,					// top-k MIP results (return)
	float& sim_threshold)
{
	// -------------------------------------------------------------------------
	//  construct Simple_LSH query
	// -------------------------------------------------------------------------
	float norm_q = sqrt(calc_inner_product(dim_, query, query));
	float *simple_lsh_query = new float[simple_lsh_dim_];

	for (int i = 0; i < simple_lsh_dim_; ++i)
	{
		if (i < dim_)
			simple_lsh_query[i] = query[i] / norm_q;
		else
			simple_lsh_query[i] = 0.0f;
	}

	// -------------------------------------------------------------------------
	//  conduct c-k-AMC search by SRP-LSH
	// -------------------------------------------------------------------------
	MaxK_List *mcs_list = new MaxK_List(top_k);
	// lsh_->kmc(top_k, (const float *) simple_lsh_query, mcs_list, query);
	// vector<int> candidates = lsh_->mykmc(top_k, (const float *) simple_lsh_query, mcs_list, query);
	unordered_set<int> candidates = lsh_->mykmc(top_k, (const float *) simple_lsh_query, mcs_list, query, sim_threshold);

	// -------------------------------------------------------------------------
	//  calc inner product for candidates returned by SRP-LSH
	// -------------------------------------------------------------------------
	int candidate_size = 0;
	for(auto it : candidates)
	{
		int id = (int)it;
		float ip = calc_inner_product(dim_, data_[id], query);

		if( ip > S_)
		{
			++candidate_size;
			// list structure -- priority queue using resorted distance of inner prouct as similarity
			list->insert(ip, id + 1);
		}
	}

	// then update the sim_min_threshold
	int temp_index_size = min(top_k, list->size());

	if(temp_index_size > 0)
	{
		int current_data_idx = list->ith_id(temp_index_size - 1) - 1;
		sim_threshold = min(calc_inner_product(dim_, data_[current_data_idx], query), sim_threshold);
	}
	else
	{
		sim_threshold = MAXREAL;
	}

	if(list->size() < top_k)
	{
		for(int i = top_k - list->size(); i >=0;  i--)
		{
			int id = -1;
			float ip = FLT_MIN;

			// list structure -- priority queue using resorted distance of inner prouct as similarity
			list->insert(ip, id);
		}
	}

	if(list->size() != top_k)
	{
		printf("Something is wrong.... \n");
	}
	// -------------------------------------------------------------------------
	//  release space
	// -------------------------------------------------------------------------
	delete[] simple_lsh_query;
	simple_lsh_query = NULL;

	delete mcs_list;
	mcs_list = NULL;

	// return 0;
	return candidate_size;
}

// -----------------------------------------------------------------------------
void Simple_LSH::persistHashTable(const char *fname)			// persist HashTables on file
{
	lsh_->persistHashTable(fname);
}

// -----------------------------------------------------------------------------
void Simple_LSH::loadHashTable(const char *fname)			// load HashTables on file
{
	lsh_->loadHashTable(fname);
}
