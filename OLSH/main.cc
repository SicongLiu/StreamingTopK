#include "headers.h"

// int MAX_DIMENSION = 0;


// -----------------------------------------------------------------------------
void usage() 						// display the usage of this package
{
	printf("\n"
		"-------------------------------------------------------------------\n"
		" Usage of the package for c-Approximate MIP (c-AMIP) search\n"
		"-------------------------------------------------------------------\n"
		"    -alg  {integer}  options of algorithms (0 - 13)\n"
		"    -n    {integer}  cardinality of the dataset\n"
		"    -d    {integer}  dimensionality of the dataset\n"
		"    -qn   {integer}  number of queries\n"
		"    -K    {integer}  #hash functions for Sign_ALSH and Simple_LSH\n"
		"    -L    {integer}  #hash layers for Sign_ALSH and Simple_LSH\n"
		"    -c0   {real}     approximation ratio of ANN search (c0 > 1)\n"
		"    -c    {real}     approximation ratio of AMIP search (0 < c < 1)\n"
		"    -ds   {string}   address of the data  set\n"
		"    -qs   {string}   address of the query set\n"
		"    -ts   {string}   address of the truth set\n"
		"    -of   {string}   output folder\n"
		"\n"
		"-------------------------------------------------------------------\n"
		" The options of algorithms are:\n"
		"-------------------------------------------------------------------\n"
		"    0  - Ground-Truth\n"
		"         Parameters: -alg 0 -n -qn -d -ds -qs -ts\n"
		"\n"
		"    1  - AMIP search by H2_ALSH\n"
		"         Parameters: -alg 1 -n -qn -d -c0 -c -ds -qs -ts -of\n"
		"\n"
		"    2  - AMIP search by L2_ALSH\n"
		"         Parameters: -alg 2 -n -qn -d -m -U -c0 -ds -qs -ts -of\n"
		"\n"
		"    3  - AMIP search by L2_ALSH2\n"
		"         Parameters: -alg 3 -n -qn -d -m -U -c0 -ds -qs -ts -of\n"
		"\n"
		"    4  - AMIP search by XBOX and H2-ALSH-\n"
		"         Parameters: -alg 4 -n -qn -d -c0 -ds -qs -ts -of\n"
		"\n"
		"    5  - AMIP search by Sign_ALSH\n"
		"         Parameters: -alg 5 -n -qn -d -K -m -U -c0 -ds -qs -ts -of\n"
		"\n"
		"    6  - AMIP search by Simple_LSH\n"
		"         Parameters: -alg 6 -n -qn -d -K -c0 -ds -qs -ts -of\n"
		"\n"
		"    7  - MIP search by Linear_Scan\n"
		"         Parameters: -alg 7 -n -qn -d -B -qs -ts -df -of\n"
		"\n"
		"    8  - Precision-Recall Curve of AMIP search by H2_ALSH\n"
		"         Parameters: -alg 8 -n -qn -d -c0 -c -ds -qs -ts -of\n"
		"\n"
		"    9  - Precision-Recall Curve of AMIP search by Sign_ALSH\n"
		"         Parameters: -alg 9 -n -qn -d -K -m -U -c0 -ds -qs -ts -of\n"
		"\n"
		"    10 - Precision-Recall Curve of AMIP search by Simple_LSH\n"
		"         Parameters: -alg 10 -n -qn -d -K -c0 -ds -qs -ts -of\n"
		"\n"
		"    11 - Norm Distributiuon\n"
		"         Parameters: -alg 11 -n -qn -d -ds -qs -of\n"
		"    12 - overall_performance \n"
		"         Parameters: -alg 12 -d -qn -L1 -it -ts -of\n"
		"\n"
		"-------------------------------------------------------------------\n"
		" Authors: Sicong Liu (s.liu@asu.edu)                               \n"
		"-------------------------------------------------------------------\n"
		"\n\n\n");
}

// -----------------------------------------------------------------------------
int main(int nargs, char **args)
{
	srand((unsigned) time(NULL));	// set the random seed
	// usage();

	int   alg       = -1;			// which algorithm?
	int   n         = -1;			// cardinality
	int   qn        = -1;			// query number
	int   d         = -1;			// dimensionality
	int   K         = -1;			// # tables for sign-alsh and simple-lsh
	int   L         = -1;			// # of hash layers
	int   layer_index = -1;			// current onion layer index
	int 	  top_k = -1;
	int   L1         = -1;			// # of onion layers
	float S 		= -1.0f;				// similarity threshold
	float nn_ratio  = -1.0f;			// approximation ratio of ANN search
	float mip_ratio = -1.0f;			// approximation ratio of AMIP search
	bool post_opt = false;

	char  data_set[200];				// address of data set
	char  query_set[200];			// address of query set
	char  truth_set[200];			// address of ground truth file
	char  temp_set[200];				// address of temporary results per query per onion layer
	char  output_folder[200];		// output folder
	char  sim_angle[200];			// address of sim-angle output
	char  temp_hash[200];			// address of temporal data hash code

	bool  failed = false;
	int   cnt = 1;
	
	while (cnt < nargs && !failed) {
		if (strcmp(args[cnt], "-alg") == 0) {
			alg = atoi(args[++cnt]);
			printf("alg           = %d\n", alg);
			if (alg < 0 || alg > 12) {
				failed = true;
				break;
			}
		}
		else if (strcmp(args[cnt], "-n") == 0) {
			n = atoi(args[++cnt]);
			printf("n             = %d\n", n);
			if (n <= 0) {
				failed = true;
				break;
			}
		}
		else if (strcmp(args[cnt], "-d") == 0) {
			d = atoi(args[++cnt]);
			printf("d             = %d\n", d);
			// MAX_DIMENSION = d;
			if (d <= 0) {
				failed = true;
				break;
			}
		}
		else if (strcmp(args[cnt], "-qn") == 0) {
			qn = atoi(args[++cnt]);
			printf("qn            = %d\n", qn);
			if (qn <= 0) {
				failed = true;
				break;
			}
		}
		else if (strcmp(args[cnt], "-L1") == 0) {
			L1 = atoi(args[++cnt]);
			printf("L1            = %d\n", L1);
			if (L1 <= 0) {
				failed = true;
				break;
			}
		}
		else if (strcmp(args[cnt], "-K") == 0) {
			K = atoi(args[++cnt]);
			printf("K             = %d\n", K);
			if (K <= 0) {
				failed = true;
				break;
			}
		}
		else if (strcmp(args[cnt], "-L") == 0) {
			L = atoi(args[++cnt]);
			printf("L             = %d\n", L);
			if (L <= 0) {
				failed = true;
				break;
			}
		}
		else if (strcmp(args[cnt], "-LI") == 0) {
			layer_index = atoi(args[++cnt]);
			printf("layer_index             = %d\n", layer_index);
			if (layer_index <= 0) {
				failed = true;
				break;
			}
		}
		else if (strcmp(args[cnt], "-tk") == 0) {
			top_k = atoi(args[++cnt]);
			printf("top_k             = %d\n", top_k);
			if (top_k <= 0) {
				failed = true;
				break;
			}
		}
		else if (strcmp(args[cnt], "-S") == 0) {
			S = (float) atof(args[++cnt]);
			printf("S            = %.2f\n", S);
			/*if (S <= 0.0f) {
				failed = true;
				break;
			}*/
		}
		else if (strcmp(args[cnt], "-c0") == 0) {
			nn_ratio = (float) atof(args[++cnt]);
			printf("c0            = %.2f\n", nn_ratio);
			if (nn_ratio <= 1.0f) {
				failed = true;
				break;
			}
		}
		else if (strcmp(args[cnt], "-c") == 0) {
			mip_ratio = (float) atof(args[++cnt]);
			printf("c             = %.2f\n", mip_ratio);
			if (mip_ratio <= 0.0f || mip_ratio >= 1.0f) {
				failed = true;
				break;
			}
		}
		else if (strcmp(args[cnt], "-ds") == 0) {
			strncpy(data_set, args[++cnt], sizeof(data_set));
			printf("data_set      = %s\n", data_set);
		}
		else if (strcmp(args[cnt], "-qs") == 0) {
			strncpy(query_set, args[++cnt], sizeof(query_set));
			printf("query_set     = %s\n", query_set);
		}
		else if (strcmp(args[cnt], "-ts") == 0) {
			strncpy(truth_set, args[++cnt], sizeof(truth_set));
			printf("truth_set     = %s\n", truth_set);
		}
		else if (strcmp(args[cnt], "-it") == 0) {
			strncpy(temp_set, args[++cnt], sizeof(temp_set));
			printf("temp_set     = %s\n", temp_set);
		}
		else if (strcmp(args[cnt], "-sa") == 0) {
			strncpy(sim_angle, args[++cnt], sizeof(sim_angle));
			printf("sim_angle     = %s\n", sim_angle);
		}
		else if (strcmp(args[cnt], "-of") == 0) {
			strncpy(output_folder, args[++cnt], sizeof(output_folder));
			printf("output_folder = %s\n", output_folder);

			if(alg != 12)
			{
				int len = (int) strlen(output_folder);
				if (output_folder[len - 1] != '/') {
					output_folder[len] = '/';
					output_folder[len + 1] = '\0';
				}
				create_dir(output_folder);
			}
		}
		else if (strcmp(args[cnt], "-hr") == 0) {
			strncpy(temp_hash, args[++cnt], sizeof(temp_hash));
			printf("temp_hash     = %s\n", temp_hash);
		}
		else if (strcmp(args[cnt], "-pot") == 0) {
			int post_opt_int = atoi(args[++cnt]);
			printf("post_opt_int             = %d\n", post_opt_int);
			if(post_opt_int == 0)
			{
				post_opt = false;
			}
			else if(post_opt_int == 1)
			{
				post_opt = true;
			}
			if (post_opt_int < 0)
			{
				failed = true;
				break;
			}
		}
		else {
			failed = true;
			usage();
			break;
		}
		cnt++;
	}
	printf("\n");

	// -------------------------------------------------------------------------
	//  read data set and query set
	// -------------------------------------------------------------------------

	if(alg == 12) // compute overall performance, as a separate option
	{
		overall_performance(d, qn, L1, top_k, temp_set, truth_set, output_folder);
	}
	else
	{
		timeval start_time, end_time;

		gettimeofday(&start_time, NULL);
		float** data = new float*[n];
		for (int i = 0; i < n; ++i)
		{
			data[i] = new float[d];
		}
		if (read_data(n, d, data_set, data) == 1)
		{
			printf("Reading dataset error!\n");
			return 1;
		}

		float** query = new float*[qn];
		for (int i = 0; i < qn; ++i)
		{
			query[i] = new float[d];
		}
		if (read_data(qn, d, query_set, query) == 1)
		{
			printf("Reading query set error!\n");
			return 1;
		}
		gettimeofday(&end_time, NULL);
		float read_file_time = end_time.tv_sec - start_time.tv_sec +
				(end_time.tv_usec - start_time.tv_usec) / 1000000.0f;
		printf("Read Data and Query: %f Seconds\n\n", read_file_time);


		// -------------------------------------------------------------------------
		//  methods
		// -------------------------------------------------------------------------
		switch (alg)
		{
		case 0:
			ground_truth(n, qn, d, (const float **) data, (const float **) query,
					truth_set);
			break;
		case 1:
			h2_alsh(n, qn, d, nn_ratio, mip_ratio, (const float **) data,
					(const float **) query, truth_set, output_folder);
			break;
		case 4:
			xbox(n, qn, d, nn_ratio, (const float **) data, (const float **) query,
					truth_set, output_folder);
			break;
		case 6:
			// simple_lsh(n, qn, d, K, L, S, nn_ratio, (const float **) data,
			//		(const float **) query, truth_set, output_folder);
			break;
		case 7:
			linear_scan(n, qn, d, (const float **) data, (const float **) query,
					truth_set, output_folder);
			break;
		case 8:
			h2_alsh_precision_recall(n, qn, d, nn_ratio, mip_ratio, (const float **) data,
					(const float **) query, truth_set, output_folder);
			break;
		case 10:
			// simple_lsh_recall(n, qn, d, K, L, layer_index, S, nn_ratio, (const float **) data,
			//								(const float **) query, truth_set, temp_set, output_folder);
			simple_lsh_recall(n, qn, d, K, L, layer_index, top_k, S, nn_ratio, (const float **) data,
											(const float **) query, truth_set, temp_set, sim_angle, output_folder,
											temp_hash, post_opt);
			break;
		case 11:
			norm_distribution(n, d, (const float **) data, output_folder);
			break;
		default:
			printf("Parameters error!\n");
			usage();
			break;
		}

		// -------------------------------------------------------------------------
		//  release space
		// -------------------------------------------------------------------------
		for (int i = 0; i < n; ++i)
		{
			delete[] data[i];
			data[i] = NULL;
		}
		delete[] data;
		data  = NULL;

		for (int i = 0; i < qn; ++i)
		{
			delete[] query[i];
			query[i] = NULL;
		}
		delete[] query;
		query = NULL;
	}

	return 0;
}


