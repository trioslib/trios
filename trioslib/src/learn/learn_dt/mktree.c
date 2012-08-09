#include <trios.h>
#include "pacdt_local.h"	/* NINA */
#include <stdlib.h>

/* #define _DEBUG_ */
/* #define _DEBUG_1_ */
/* #define _DEBUG_2_ */

/*  #define _VERBOSE_
   #define _VERYVERBOSE_
*/

/* The next definition can be used under SunOs, or Solaris, also */

#define _mktreetime_

#ifdef _mktreetime_
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#endif

char dt_file[LINESIZE], train_data[LINESIZE];
char test_data[LINESIZE], misclassified_data[LINESIZE];
char log_file[LINESIZE];

int no_of_dimensions = 0, no_of_coeffs, no_of_categories = 0;
int no_of_restarts = 20, no_of_folds = 0;
int normalize = TRUE;
int order_of_perturbation = SEQUENTIAL;
int oblique = TRUE;
int axis_parallel = TRUE;
int cart_mode = FALSE;
int coeff_modified = FALSE;
int cycle_count = 0;
int *left_count = NULL, *right_count = NULL;
int max_no_of_random_perturbations = 5;
int no_of_stagnant_perturbations, no_of_missing_values = 0;
int no_of_train_points = 0, no_of_test_points = 0;
/*int stop_splitting();*/

/*float compute_impurity();*/
float *coeff_array, *modified_coeff_array, *best_coeff_array;
float ap_bias = 1.0;
float zeroing_tendency = 0.1;
float *attribute_min, *attribute_avg, *attribute_sdev;
double *temp_val;

struct unidim *candidates;
/*struct test_outcome estimate_accuracy();*/

FILE *perturb_file = NULL;
FILE *logfile;


POINT **train_points = NULL, **test_points = NULL;

int *map;			/* array to map the categories                */




/************************************************************************/
/* Module name : estimate_accuracy					*/
/* Functionality :	Uses the decision tree (pointed to by "root") to*/
/*			classify "points".				*/
/* Parameters :	points : array of pointers to POINT structures.		*/
/*		no_of_points : number of test samples			*/
/*		root : pointer to the root of the decision tree		*/
/* Returns :	a structure "test_outcome", containing the details of	*/
/*		classification (overall classification accuracy, 	*/
/*		accuracies for individual classes, decision tree leaf	*/
/*		counts and depths etc)					*/
/* Remarks :	This routine is to classify and estimate accuracy of 	*/
/*		a decision tree only on datasets in which the class of 	*/
/*		the objects is marked.					*/
/************************************************************************/
struct test_outcome estimate_accuracy(POINT ** points, int no_of_points,
				      struct tree_node *root)
{
	int i, j;
	int total_corrects, total_incorrects;
	/*
	   int leaf_count(), tree_depth();
	 */
	int *correct, *incorrect;
	struct tree_node *cur_node;
	struct test_outcome result;
	double sum;

	if (root == NULL)
		trios_fatal
		    ("Estimate_Accuracy : Called with empty decision tree.");

	correct = ivector(1, no_of_categories);
	incorrect = ivector(1, no_of_categories);

	for (i = 1; i <= no_of_categories; i++)
		correct[i] = incorrect[i] = 0;

	for (i = 1; i <= no_of_points; i++) {
		cur_node = root;
		while (cur_node != NULL) {
			sum = cur_node->coefficients[no_of_dimensions + 1];
			for (j = 1; j <= no_of_dimensions; j++)
				sum +=
				    cur_node->coefficients[j] *
				    points[i]->dimension[j];

			if (sum < 0) {
				if (cur_node->left != NULL)
					cur_node = cur_node->left;
				else {
					if (cur_node->left_cat ==
					    points[i]->category)
						correct[points[i]->category]++;
					else {
						incorrect[points
							  [i]->category]++;
					}
					break;
				}
			} else {
				if (cur_node->right != NULL)
					cur_node = cur_node->right;
				else {
					if (cur_node->right_cat ==
					    points[i]->category)
						correct[points[i]->category]++;
					else {
						incorrect[points
							  [i]->category]++;
					}
					break;
				}
			}
		}
	}

	result.leaf_count = leaf_count(root);
	result.tree_depth = tree_depth(root);
	result.class = ivector(1, 2 * no_of_categories);

	total_corrects = total_incorrects = 0;

	for (i = 1; i <= no_of_categories; i++) {
		total_corrects += correct[i];
		total_incorrects += incorrect[i];
	}

	result.accuracy = 100.0 * total_corrects / no_of_points;

	for (i = 1; i <= no_of_categories; i++) {
		j = correct[i] + incorrect[i];
		result.class[2 * i - 1] = correct[i];
		result.class[2 * i] = j;
	}

	free_ivector(correct, 1, no_of_categories);
	free_ivector(incorrect, 1, no_of_categories);

	return (result);
}

/*
     -------------------------------------------
     FUNCTION: ltrainDT 
     -------------------------------------------
*/

int				/*+ Purpose: Generates a decision tree from a mtm
				   file                                                + */ ltrainDT(
													    char *mtm_file,	/*+ In: mtm file                                    + */
													    int oblique_mode,	/*+ In: uses oblique cutting                        + */
													    int cart,	/*+ In: uses cart mode                              + */
													    int nrestarts,	/*+ In: number of restarts                          + */
													    int njumps,	/*+ In: number of jumps                             + */
													    char *log_file_name,	/*+ Out: name of the file to record the logs        + */
													    char *dt	/*+ Out: name of the file to storage the tree       + */
    )
/*+ Return: 1 if OK, 0 if it fails                                            +*/
{
/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                    */
/*  date: Wed Jan 14 1998                                                      */

	window_t *win;
	apert_t *apt;
	mtm_t *mtm;

	char *pattern;
	int label_aux;

	int type;
	int i, j;

	struct tree_node *root = NULL;

	struct test_outcome result;

	freq_node *freqlist;	/* pointers to a frequency node */

#ifdef _mktreetime_
	unsigned long int tu0, tuf, ts0, tsf, time_usr, time_sys;
	struct rusage rusage;

	getrusage(RUSAGE_SELF, &rusage);
	tu0 = rusage.ru_utime.tv_sec;
	ts0 = rusage.ru_stime.tv_sec;
#endif


#ifdef _DEBUG_
	trios_debug("Entered ltrainDT()");
#endif

	if ((nrestarts >= 0) && (nrestarts != 20)) {
		no_of_restarts = nrestarts;
	}

	if ((njumps >= 0) && (njumps != 5)) {
		max_no_of_random_perturbations = njumps;
	}

	strcpy(dt_file, dt);

#ifdef _DEBUG_1_
	trios_debug("dt_file=%s", dt_file);
#endif

	if (log_file_name)
		strcpy(log_file, log_file_name);
	else
		strcpy(log_file, "dttrain.log");

	if ((logfile = fopen(log_file, "w")) == NULL) {
		(void) trios_error(1,
				   "Mktree: Log file cannot be written to.\n");
		exit(0);
	}
#ifdef _DEBUG_1_
	trios_debug("log_file=%s", log_file);
#endif

/* Nina: log_file pode ser um parametro char * */
/* Rob: concordo, mas antes preciso dele para nao dar problemas com
        os outros arquivos. Ele e' uma variavel global          */

	if (!oblique_mode) {	/*Axis parallel splits only */
		oblique = FALSE;
	}

	if (cart) {
		cart_mode = TRUE;
	}


	/* read classified examples file */
	if (!(mtm = mtm_read(mtm_file, &win /*, &apt */ ))) {
		return trios_error(MSG, "lpacdt: mtm_read() failed to read %s",
				   mtm_file);
	}
#ifdef _DEBUG_
	trios_debug("Reading classified examples file: done.");
#endif

#ifdef _DEBUG_2_
	for (i = 0; i < mtm_get_nmtm(mtm); i++) {
		trios_debug("pattern %d = %d", i, mtm_GX_get_label(mtm, i));
	}
#endif



	type = mtm_get_type(mtm);

	no_of_train_points = mtm_get_nmtm(mtm);

	no_of_dimensions = mtm_get_wsize(mtm);

	freqlist = (freq_node *) mtm->mtm_freq;

	while (freqlist) {
		no_of_categories++;
		freqlist = freqlist->next;
	}

#ifdef _DEBUG_1_
	trios_debug("type = %d", type);
	trios_debug("no_of_train_points = %d", no_of_train_points);
	trios_debug("no_of_dimensions = %d", no_of_dimensions);
	trios_debug("no_of_categories = %d", no_of_categories);
#endif



	/* map actual labels to virtual lables ... */
	if (NULL ==
	    (map = (int *) malloc(sizeof(int) * (no_of_categories + 1)))) {
		return trios_error(MSG, "ltrainDT: malloc failed for map");
	}

	freqlist = (freq_node *) mtm->mtm_freq;

	i = 0;
	map[i] = 0;

	while (freqlist) {
		i++;
		map[i] = freqlist->label;
		freqlist = freqlist->next;
	}

    printf("map map %p\n", map);

#ifdef _DEBUG_2_
	for (i = 1; i <= no_of_categories; i++) {
		trios_debug("map[%d] = %d", i, map[i]);
	}
#endif



	if (NULL ==
	    (train_points =
	     (struct point **) malloc(sizeof(int) * (no_of_train_points)))) {
		return trios_error(MSG,
				   "ltrainDT: malloc failed for train_points");
	}

	for (i = 0; i < no_of_train_points; i++) {
		if (NULL ==
		    (train_points[i] =
		     (struct point *) malloc(sizeof(struct point)))) {
			return (int) trios_error(MSG,
						 "ltrainDT: malloc failed for each train_point");
		}
	}

	for (i = 0; i < no_of_train_points; i++) {
		train_points[i]->dimension = fvector(1, no_of_dimensions);
	}



	/* updates the structure point */

	for (i = 0; i < no_of_train_points; i++) {

		pattern = mtm_GX_get_pattern(mtm, i);
		label_aux = mtm_GX_get_label(mtm, i);

#ifdef _DEBUG_2_
		trios_debug("pattern %d = %d", i, label_aux);
#endif

		for (j = 1; j <= no_of_categories; j++) {
			if (map[j] == label_aux) {
				train_points[i]->category = j;

#ifdef _DEBUG_2_
				trios_debug("train_points[%d]->category = %d",
					    i, train_points[i]->category);
#endif

				break;
			}
		}

		train_points[i]->val = (double) 0.0;

		for (j = 1; j <= no_of_dimensions; j++) {
			train_points[i]->dimension[j] = (float) pattern[j - 1];
		}

	}

#ifdef _DEBUG_1_
	for (i = 0; i < no_of_train_points; i++) {
		if (train_points[i]->category != 0) {
			/*      for (j=1; j <= no_of_dimensions ; j++) { 
			   trios_debug("%d ", pattern[j-1]) ;
			   } */
			trios_debug("label(%d) = %d", i,
				    train_points[i]->category);
			trios_debug("(train_points[%d]->category = %d)", i,
				    train_points[i]->category);
		}
	}
#endif


#ifdef _DEBUG_
	trios_debug("Before shuffle");
#endif

	/* All vectors goes from 1 to n */
	train_points -= 1;

	shuffle_points(train_points, no_of_train_points);

#ifdef _DEBUG_1_
	trios_debug("After shuffle");
	for (i = 1; i <= no_of_train_points; i++) {
		if (train_points[i]->category != 0) {
			trios_debug("(train_points[%d]->category = %d)", i,
				    train_points[i]->category);
		}
	}
#endif



#ifdef _VERBOSE_
	fprintf(logfile, "%d training examples loaded from %s.\n",
		no_of_train_points, train_data);
	if (no_of_folds == 0 && no_of_test_points != 0)
		fprintf(logfile, "%d testing examples loaded from %s.\n",
			no_of_test_points, train_data);
	if (no_of_missing_values)
		fprintf(logfile,
			"%d missing values filled with respective attribute means.\n",
			no_of_missing_values);
	fprintf(logfile, "Attributes = %d, Classes = %d\n", no_of_dimensions,
		no_of_categories);
#endif


#ifdef _DEBUG_
	trios_debug("Before allocate_structures");
#endif

	allocate_structures(no_of_train_points);

#ifdef _DEBUG_1_
	trios_debug("After allocate_structures");
#endif

	/* write dt_file */
	dt_write(dt_file, type, win, apt);


	sprintf(dt_file, "%s.dt", dt_file);

	/* */
#ifdef _DEBUG_
	trios_debug("It will call build_tree().");
#endif
	root = build_tree(train_points, no_of_train_points, dt_file);

#ifdef _DEBUG_
	trios_debug("build_tree() done.");
#endif

	deallocate_structures(no_of_train_points);

#ifdef _DEBUG_
	trios_debug("deallocate_structures() done");
#endif

	if (root != NULL) {
		result =
		    estimate_accuracy(train_points, no_of_train_points, root);
		printf
		    ("acc. on training set = %.2f\t#leaves = %.0f\tmax depth = %.0f\n",
		     result.accuracy, result.leaf_count, result.tree_depth);
	} else {
		printf("No tree was written\n");
	}

#ifdef _mktreetime_
	getrusage(RUSAGE_SELF, &rusage);
	tuf = rusage.ru_utime.tv_sec;
	tsf = rusage.ru_stime.tv_sec;

	time_usr = (tuf - tu0);
	time_sys = (tsf - ts0);

	trios_debug("user: %2d:%2d:%2d", (time_usr / 3600),
		    (time_usr % 3600) / 60, (time_usr % 3600) % 60);
	trios_debug("system: %2d:%2d:%2d", (time_sys / 3600),
		    (time_sys % 3600) / 60, (time_sys % 3600) % 60);
	trios_debug("Total: %2d:%2d:%2d", (time_sys + time_usr) / 3600,
		    ((time_sys + time_usr) % 3600) / 60,
		    ((time_sys + time_usr) % 3600) % 60);
#endif

#ifdef _VERBOSE_
	for (i = 1; i <= no_of_categories; i++) {
		if (result.class[2 * i] != 0) {
			fprintf(logfile,
				"Category %d : accuracy = %.2f (%d/%d)\n", i,
				100.0 * result.class[2 * i -
						     1] / result.class[2 * i],
				result.class[2 * i - 1], result.class[2 * i]);
		}
	}
#endif

	print_log_and_exit();

	return (1);

}


/************************************************************************/
/* Module name : allocate_point_array					*/
/* Functionality :	Allocates or reallocates "array_name" to be an	*/
/*			array of pointers (to POINT structures), of	*/
/*			size "size". Fully allocates all the POINT	*/
/*			structures also.				*/
/* Parameters :	array_name : name of the array to be (re)allocated.	*/
/*		size	   : number of points to be allocated.		*/
/*		prev_size  : 0 if array_name doesn't exist already	*/
/*			     current size otherwise.			*/
/* Returns :	pointer to the allocated array.				*/
/* Calls modules :	error (util.c)					*/
/*			vector (util.c)					*/
/* Is called by modules : 	load_points				*/
/************************************************************************/
POINT **allocate_point_array(POINT ** array_name, int size, int prev_size)
{
	int i;

	if (prev_size == 0) {
		if (array_name != NULL)
			free((char *) (array_name + 1));

		array_name = (struct point **) malloc
		    ((unsigned) size * sizeof(struct point *));
		if (!array_name)
			trios_fatal
			    ("Allocate_Point_Array: Memory Allocation Failure 1.");

		array_name -= 1;	/* All indices start from 1 */

		for (i = 1; i <= size; i++) {
			array_name[i] = (struct point *) malloc((unsigned)
								sizeof(struct
								       point));
			if (!array_name[i])
				trios_fatal
				    ("Allocate_Point_Array : Memory Allocation failure 2.");
		}

		for (i = 1; i <= size; i++)
			array_name[i]->dimension = fvector(1, no_of_dimensions);
	} else {
		array_name += 1;
		array_name = (struct point **) realloc
		    (array_name, (unsigned) size * sizeof(struct point *));
		if (!array_name)
			trios_fatal
			    ("Allocate_Point_Array: Memory Allocation Failure 3.");

		array_name -= 1;	/* All indices start from 1 */

		if (prev_size >= size)
			return (array_name);

		for (i = prev_size + 1; i <= size; i++) {
			array_name[i] = (struct point *) malloc((unsigned)
								sizeof(struct
								       point));
			if (!array_name[i])
				trios_fatal
				    ("Allocate_Point_Array : Memory Allocation failure 4.");
		}

		for (i = prev_size + 1; i <= size; i++) {
			array_name[i]->dimension = fvector(1, no_of_dimensions);
		}
	}

	return (array_name);
}


/************************************************************************/
/* Module name : allocate_structures					*/
/* Functionality :	Allocates space for some global data structures.*/
/* Parameters : no_of_points : size of the training dataset.		*/
/* Returns :	nothing.						*/
/* Calls modules :	vector (util.c)					*/
/*			ivector (util.c)				*/
/*			dvector (util.c)				*/
/* Is called by modules :	main					*/
/************************************************************************/
int allocate_structures(int no_of_points)
{

	no_of_coeffs = no_of_dimensions + 1;

	coeff_array = fvector(1, no_of_coeffs);

	modified_coeff_array = fvector(1, no_of_coeffs);

	best_coeff_array = fvector(1, no_of_coeffs);

	left_count = ivector(1, no_of_categories);

	right_count = ivector(1, no_of_categories);

	candidates = (struct unidim *) malloc((unsigned) no_of_points *
					      sizeof(struct unidim));

	candidates -= 1;

	attribute_min = fvector(1, no_of_dimensions);

	attribute_avg = fvector(1, no_of_dimensions);

	attribute_sdev = fvector(1, no_of_dimensions);

	temp_val = dvector(1, no_of_points);

	return (1);		/* NINA */
}

/************************************************************************/
/* Module name : deallocate_structures					*/
/* Functionality : Frees space allocated to some global data structures.*/
/* Parameters : no_of_points : size of the training dataset.		*/
/* Returns :	nothing.						*/
/* Calls modules :	free_fvector (util.c)				*/
/*			free_ivector (util.c)				*/
/*			free_dvector (util.c)				*/
/* Is called by modules :	main					*/
/************************************************************************/
int deallocate_structures(int no_of_points)
{

	free_fvector(coeff_array, 1, no_of_coeffs);

	free_fvector(modified_coeff_array, 1, no_of_coeffs);

	free_ivector(left_count, 1, no_of_categories);

	free_ivector(right_count, 1, no_of_categories);

#ifdef _DEBUG_
	trios_debug("First four free done");
#endif

	free_fvector(best_coeff_array, 1, no_of_coeffs);

	free((char *) (candidates + 1));

	free_fvector(attribute_min, 1, no_of_dimensions);

	free_fvector(attribute_avg, 1, no_of_dimensions);

#ifdef _DEBUG_
	trios_debug("First seven free done");
#endif

	free_fvector(attribute_sdev, 1, no_of_dimensions);

	free_dvector(temp_val, 1, no_of_points);

	return (1);		/* NINA */
}


float myrandom(double a, double b);


/************************************************************************/
/* Module name :	shuffle_points					*/
/* Functionality :	Pseudo-randomly shuffles the points in the	*/
/*			array "array_name". 				*/
/*			for i = 1 to n, do				*/
/*			  swap point i with the point at a random 	*/
/*			  position between 1 and n.			*/
/* Parameters :	array_name : Point array which is to be shuffled.	*/
/*		count	: Number of entries in the array.		*/
/* Returns : Nothing.							*/
/* Calls modules :	myrandom (util.c)				*/
/* Is called by modules :	load_points				*/
/* Remarks :	Achieves shuffling just by swapping pointers, thus 	*/
/*		not spending time on allocation/deallocation.		*/
/************************************************************************/
int shuffle_points(POINT ** array_name, int count)
{
	int i, newposition;
	POINT *temp_point;
    for (i = 1; i <= count; i++) {
        float r = myrandom(1.0, (double) count);
        newposition = (int) r;
		temp_point = array_name[i];
		array_name[i] = array_name[newposition];
		array_name[newposition] = temp_point;
	}

    return 1;
}



/************************************************************************/
/* Module name : Build_Tree                                             */
/* Functionality : Top level tree to induce and write a decision        */
/*                 tree to a file.                                      */
/* Parameters : points = array of training instances                    */
/*              no_of_points = instance count                           */
/*              dt_file = file into which the decision tree is to be    */
/*                        written.                                      */
/* Returns :    Pointer to the root of the tree induced.                */
/* Calls modules :  build_subtree                                       */
/*                  write_tree (train_util.c)                           */
/*                  allocate_point_array (load_data.c)                  */
/* Is called by modules : main                                          */
/*                        cross_validate                                */
/* Important Variables used :                                           */
/* Remarks :                                                            */
/************************************************************************/
struct tree_node *build_tree(struct point **points, int no_of_training_points,
			     char *dt_file_local)
{
	/*
	   struct tree_node *root, *build_subtree();

	   struct point **allocate_point_array() ;
	 */
	struct tree_node *root;

	struct point **train_points = NULL;

	struct tree_node *proot;

	int no_of_points;


#ifdef _DEBUG_
	trios_debug("Entered build_tree()");
#endif
#ifdef _DEBUG_1_
	trios_debug("dt_file=%s\n", dt_file_local);
#endif

	train_points = points;
	no_of_points = no_of_training_points;

	/* Build the tree recursively. */
	root = build_subtree("\0", train_points, no_of_points);

	if (root == NULL) {
		fprintf(stderr,
			"No split could be found with the current parameter settings.\n");
		fprintf(stderr,
			"Try increasing the values of restarts and random jumps.\n");
		print_log_and_exit();
	} else
		root->parent = NULL;

	proot = root;

	/* Write the trees to files. */
	if (strlen(dt_file_local)) {
		write_tree(proot, dt_file_local);
		if (proot == root)
			printf("Unpruned decision tree written to %s.\n",
			       dt_file_local);
	}

	root = proot;
	return (root);

}



/************************************************************************/
/* Module name : build_subtree						*/
/* Functionality :	Recursively builds a decision tree. i.e., finds	*/
/*			the best (heuristic) hyperplane separating the 	*/
/*			given set of points, and recurses on both sides	*/
/*			of the hyperplane. The best axis-parallel split	*/
/*			is considered if -o option is not chosen, 	*/
/*			before computing oblique splits.		*/
/* Parameters :	node_str : Label to be assigned to the decision tree	*/
/*		           node to be created. 				*/
/*		cur_points : array of pointers to the points under	*/
/*		             consideration.			       	*/
/*		cur_no_of_points : Number of points.	                */
/* Returns :	pointer to the decision tree node created.		*/
/*		NULL, if a node couldn't be created.			*/
/* Calls modules :	set_counts (compute_impurity.c)			*/
/*			compute_impurity (compute_impurity.c)		*/
/*			axis_parallel_split				*/
/*			vector (util.c)					*/
/*			oblique_split					*/
/*			free_fvector (util.c)				*/
/*			error (util.c)					*/
/*			find_values (perturb.c)				*/
/*			largest_element (compute_impurity.c)		*/
/*			build_subtree					*/
/* Is called by modules : 	main					*/
/*				build_tree				*/
/*				build_subtree				*/
/*				cross_validate				*/
/* Important Variables used : 	initial_impurity: "inherent" impurity in*/
/*				the point set under consideration. ie.,	*/
/*				impurity when the separating hyperplane	*/
/*				lies on one side of the point set.	*/
/*				If any amount of perturbations (bounded	*/
/*				by the parametric settings) can not	*/
/*				result in a hyperplane that has a lesser*/
/*				impurity than this value, no new tree	*/
/*				node is created.			*/
/************************************************************************/
struct tree_node *build_subtree(char *node_str, POINT ** cur_points,
				int cur_no_of_points)
{
	struct tree_node *cur_node;
	/*
	   struct tree_node *build_subtree(),
	   *create_tree_node() ;
	 */

	POINT **lpoints = NULL, **rpoints = NULL;

	int i, lindex, rindex, lpt, rpt;
	/*
	   float oblique_split(),
	   axis_parallel_split(),
	   cart_split() ;
	 */
	float initial_impurity, cur_impurity;

	char lnode_str[MAX_DT_DEPTH], rnode_str[MAX_DT_DEPTH];


#ifdef _DEBUG_
	trios_debug("Entered build_subtree()");
#endif

	/* Validation checks */
	if (cur_no_of_points <= TOO_SMALL_FOR_ANY_SPLIT)
		return (NULL);
	if (strlen(node_str) + 1 > MAX_DT_DEPTH) {
		fprintf(stderr, "Tree growing aborted along this branch. \n");
		fprintf(stderr,
			"Depth cannot be more than MAX_DT_DEPTH, set in pacdt.h.\n");
		return (NULL);
	}


	set_counts(cur_points, cur_no_of_points, 0);

#ifdef _DEBUG_
	trios_debug("set_counts() : done");
#endif

	cur_impurity = initial_impurity = compute_impurity(cur_no_of_points);

#ifdef _DEBUG_
	trios_debug("compute_impurity() : done");
#endif

	if (cur_impurity == 0.0)
		return (NULL);

	if (cart_mode) {

#ifdef _DEBUG_
		trios_debug("build_subtree: entering cart_mode");
#endif
		cur_impurity =
		    axis_parallel_split(cur_points, cur_no_of_points);

		if (cur_impurity && (strlen(node_str) == 0 ||
				     cur_no_of_points >
				     TOO_SMALL_FOR_OBLIQUE_SPLIT))
			cur_impurity =
			    cart_split(cur_points, cur_no_of_points, node_str);
	} else {

#ifdef _DEBUG_
		trios_debug("build_subtree: entering !cart_mode");
#endif

		if (axis_parallel) {
#ifdef _DEBUG_
			trios_debug("build_subtree: it is axis_parallel");
#endif
			cur_impurity =
			    axis_parallel_split(cur_points, cur_no_of_points);
#ifdef _DEBUG_
			trios_debug
			    ("build_subtree: axis_parallel_split() done");
#endif
		}

		if (cur_impurity && oblique
		    && cur_no_of_points > TOO_SMALL_FOR_OBLIQUE_SPLIT) {
			float *ap_coeff_array, oblique_impurity;

#ifdef _DEBUG_
			trios_debug("build_subtree: it is oblique");
#endif

			ap_coeff_array = fvector(1, no_of_coeffs);

			for (i = 1; i <= no_of_coeffs; i++)
				ap_coeff_array[i] = coeff_array[i];

			if (normalize)
				normalize_data(cur_points, cur_no_of_points);

			oblique_impurity =
			    oblique_split(cur_points, cur_no_of_points,
					  node_str);

#ifdef _DEBUG_
			trios_debug("build_subtree: oblique_split() done");
#endif

			if (normalize) {
				unnormalize_data(cur_points, cur_no_of_points);
				unnormalize_hyperplane();
				for (i = 1; i <= no_of_dimensions; i++)
					attribute_min[i] = 0;
			}

			if (ap_bias * oblique_impurity >= cur_impurity) {
				for (i = 1; i <= no_of_coeffs; i++)
					coeff_array[i] = ap_coeff_array[i];
				coeff_modified = TRUE;
			} else
				cur_impurity = oblique_impurity;

			free_fvector(ap_coeff_array, 1, no_of_coeffs);
		}
	}

	if (cur_impurity >= initial_impurity)
		return (NULL);

	/*Can not find any split given current parameter settings. */

#ifdef _DEBUG_
	trios_debug
	    ("build_subtree: Can not find any split given current parameter settings");
#endif

	find_values(cur_points, cur_no_of_points);
	set_counts(cur_points, cur_no_of_points, 1);

#ifdef _VERBOSE_
	if (strlen(node_str))
		fprintf(logfile, "** \"%s\": ", node_str);
	else
		fprintf(logfile, "** Root: ");
	fprintf(logfile, "Left:[");
	for (i = 1; i < no_of_categories; i++)
		fprintf(logfile, "%d,", left_count[i]);
	fprintf(logfile, "%d] Right:[", left_count[no_of_categories]);
	for (i = 1; i < no_of_categories; i++)
		fprintf(logfile, "%d,", right_count[i]);
	fprintf(logfile, "%d]\n", right_count[no_of_categories]);
#endif

	for (i = 1, lpt = 0, rpt = 0; i <= no_of_categories; i++) {
		lpt += left_count[i];
		rpt += right_count[i];
	}

	cur_node = create_tree_node();
	cur_node->no_of_points = cur_no_of_points;

	strcpy(cur_node->label, node_str);

	if (cur_impurity == 0)
		return (cur_node);

	lpoints = rpoints = NULL;

	if (left_count[cur_node->left_cat] != lpt) {
		/* Left region is not homogeneous. */
		if ((lpoints =
		     (POINT **) malloc((unsigned) lpt * sizeof(POINT *)))
		    == NULL)
			trios_fatal("BUILD_DT : Memory allocation failure.");
		lpoints--;
		lindex = 0;
	}

	if (right_count[cur_node->right_cat] != rpt) {
		/* Right region is not homogeneous. */

		if ((rpoints =
		     (POINT **) malloc((unsigned) rpt * sizeof(POINT *)))
		    == NULL)
			trios_fatal("BUILD_DT : Memory allocation failure.");
		rpoints--;
		rindex = 0;
	}

	for (i = 1; i <= cur_no_of_points; i++)
		if (cur_points[i]->val < 0) {
			if (lpoints != NULL)
				lpoints[++lindex] = cur_points[i];
		} else {
			if (rpoints != NULL)
				rpoints[++rindex] = cur_points[i];
		}


	if (lpoints != NULL) {
		strcpy(lnode_str, node_str);
		strcat(lnode_str, "l");
		cur_node->left = build_subtree(lnode_str, lpoints, lpt);
		if (cur_node->left != NULL)
			(cur_node->left)->parent = cur_node;
		free((char *) (lpoints + 1));
	}

	if (rpoints != NULL) {
		strcpy(rnode_str, node_str);
		strcat(rnode_str, "r");
		cur_node->right = build_subtree(rnode_str, rpoints, rpt);
		if (cur_node->right != NULL)
			(cur_node->right)->parent = cur_node;
		free((char *) (rpoints + 1));
	}

	return (cur_node);

}


/************************************************************************/
/* Module name : Cart_Split                                             */
/* Functionality : Implements the CART-Linear Combinations (Breiman et  */
/*                 al, 1984, Chapter 5) hill climbing coefficient       */
/*                 perturbation algorithm.                              */
/* Parameters : cur_points: Array of pointers to the current points.    */
/*              cur_no_of_points:                                       */
/*              cur_label: Label of the tree node for which current     */
/*                         split is being induced.                      */
/* Returns : impurity of the induced hyperplane.                        */
/* Calls modules : cart_perturb (perturb.c)                             */
/*                 cart_perturb_constant (perturb.c)                    */
/*                 find_values (compute_impurity.c)                     */
/*                 set_counts (compute_impurity.c)                      */
/* Is called by modules : build_subtree                                 */
/* Remarks : See the CART book for a description of the algorithm.      */
/************************************************************************/
float cart_split(POINT ** cur_points, int cur_no_of_points, char *cur_label)
{
	int cur_coeff;
	float cur_error, new_error, prev_impurity;
	/*
	   float cart_perturb(), 
	   cart_perturb_constant() ;
	 */

	/*Starts with the best axis parallel hyperplane. */
	find_values(cur_points, cur_no_of_points);
	set_counts(cur_points, cur_no_of_points, 1);
	cur_error = compute_impurity(cur_no_of_points);
	cycle_count = 0;

	while (TRUE) {
		if (cur_error == 0.0)
			break;
		cycle_count++;
		if (cycle_count != 1)
			prev_impurity = cur_error;

		for (cur_coeff = 1; cur_coeff < no_of_coeffs; cur_coeff++) {
			new_error =
			    cart_perturb(cur_points, cur_no_of_points,
					 cur_coeff, cur_error);
			if (alter_coefficients(cur_points, cur_no_of_points)) {

#ifdef _VERYVERBOSE_
				fprintf(logfile,
					"\tCART hill climbing for coeff. %d. impurity %.3f -> %.3f\n",
					cur_coeff, cur_error, new_error);
#endif

				cur_error = new_error;
				if (cur_error == 0)
					break;
			}
		}
		if (cur_error != 0) {
			new_error =
			    cart_perturb_constant(cur_points, cur_no_of_points,
						  cur_error);
			if (alter_coefficients(cur_points, cur_no_of_points)) {

#ifdef _VERYVERBOSE_
				fprintf(logfile,
					"\tCART hill climbing for coeff. %d. impurity %.3f -> %.3f\n",
					no_of_coeffs, cur_error, new_error);
#endif
				cur_error = new_error;
			}
		}
		if (cycle_count > MAX_CART_CYCLES)
			/* Cart multivariate algorithm can get stuck in some domains.
			   Arbitrary tie breaker. */
			break;

		if (cycle_count != 1
		    && fabs(prev_impurity - cur_error) < TOLERANCE)
			break;
	}

	return (cur_error);

}

/************************************************************************/
/* Module name : Create_Tree_Node                                       */
/* Functionality : Creates a tree node structure, and sets some fields. */
/* Parameters : None.                                                   */
/* Returns : Pointer to the tree node created.                          */
/* Calls modules : error (util.c)                                       */
/*                 vector (util.c)                                      */
/*                 ivector (util.c)                                     */
/*                 largest_element (compute_impurity.c)                 */
/* Is called by modules : build_subtree                                 */
/* Remarks : Assumes that the left_count, right_count arrays and the    */
/*           coeff_array are set correctly.                             */
/************************************************************************/
struct tree_node *create_tree_node(void)
{
	struct tree_node *cur_node;

	/*
	   int i, 
	   largest_element() ;
	 */
	int i;

	cur_node = (struct tree_node *) malloc(sizeof(struct tree_node));
	if (cur_node == NULL)
		trios_fatal("Create_Tree_Node : Memory allocation failure.");

	cur_node->coefficients = fvector(1, no_of_coeffs);
	for (i = 1; i <= no_of_coeffs; i++)
		cur_node->coefficients[i] = coeff_array[i];

	cur_node->left_count = ivector(1, no_of_categories);
	cur_node->right_count = ivector(1, no_of_categories);
	for (i = 1; i <= no_of_categories; i++) {
		cur_node->left_count[i] = left_count[i];
		cur_node->right_count[i] = right_count[i];
	}

	cur_node->parent = cur_node->left = cur_node->right = NULL;
	cur_node->left_cat = largest_element(left_count, no_of_categories);
	cur_node->right_cat = largest_element(right_count, no_of_categories);

	return (cur_node);
}

/************************************************************************/
/* Module name : oblique_split						*/
/* Functionality : 	Attempts to find the hyperplane, at an unrestri-*/
/*			cted orientation, that best separates 		*/
/*			"cur_points" (minimizing the current impurity	*/
/*			measure), using hill climbing and randomization.*/
/* Parameters :	cur_points : array of pointers to the points (samples)	*/
/*			     under consideration.			*/
/*		cur_no_of_points : number of points under consideration.*/
/* Returns :	the impurity measure of the best hyperplane found.	*/
/*		The hyperplane itself is returned through the global	*/
/*		array "coeff_array".					*/
/* Calls modules :	generate_random_hyperplane			*/
/*			find_values (perturb.c)				*/
/*			set_counts (compute_impurity.c)			*/
/*			compute_impurity (compute_impurity.c)		*/
/*			myrandom (util.c)				*/
/*			suggest_perturbation (perturb.c)		*/
/*			perturb_randomly (perturb.c)			*/
/* Is called by modules :	build_subtree				*/
/************************************************************************/
float oblique_split(POINT ** cur_points, int cur_no_of_points, char *cur_label)
{
	int i, j, old_nsp, restart_count = 1;

	/*
	   int alter_coefficients() ;
	 */

	int cur_coeff, improved_in_this_cycle, best_coeff_to_improve;

	/*
	   float perturb_randomly() ;
	 */

	float cur_error, best_cur_error, least_error;
	/*
	   float new_error,
	   suggest_perturbation() ;
	 */
	float new_error;


	/*Start with the best axis parallel hyperplane if axis_parallel is true. 
	   Otherwise start with a random hyperplane. */
	if (axis_parallel != TRUE) {
		generate_random_hyperplane(coeff_array, no_of_coeffs,
					   MAX_COEFFICIENT);
		coeff_modified = TRUE;
	}

	find_values(cur_points, cur_no_of_points);
	set_counts(cur_points, cur_no_of_points, 1);
	least_error = cur_error = compute_impurity(cur_no_of_points);
	for (i = 1; i <= no_of_coeffs; i++)
		best_coeff_array[i] = coeff_array[i];
	/* Repeat this loop once for every restart */
	while (least_error != 0.0 && restart_count <= no_of_restarts) {

#ifdef _VERYVERBOSE_
		fprintf(logfile, " Restart %d: Initial Impurity = %.3f\n",
			restart_count, cur_error);
#endif

		no_of_stagnant_perturbations = 0;
		if (order_of_perturbation == RANDOM) {
			if (cycle_count <= 0)
				cycle_count = 10 * no_of_coeffs;
			for (i = 1; i <= cycle_count; i++) {
				if (cur_error == 0.0)
					break;
				cur_coeff = 0;
				while (!cur_coeff) {
					cur_coeff =
					    (int) myrandom(1.0,
							   (float) (no_of_coeffs
								    + 1));
				}
				new_error =
				    suggest_perturbation(cur_points,
							 cur_no_of_points,
							 cur_coeff, cur_error);
				if (new_error <= cur_error
				    && alter_coefficients(cur_points,
							  cur_no_of_points)) {

#ifdef _VERYVERBOSE_
					fprintf(logfile,
						"\thill climbing for coeff. %d. impurity %.3f -> %.3f\n",
						cur_coeff, cur_error,
						new_error);
#endif

					cur_error = new_error;
					improved_in_this_cycle = TRUE;
					if (cur_error == 0)
						break;
				} else {	/*Try improving in a random direction */
					improved_in_this_cycle = FALSE;
					j = 0;
					while (cur_error != 0 &&
					       !improved_in_this_cycle &&
					       ++j <=
					       max_no_of_random_perturbations) {
						new_error =
						    perturb_randomly(cur_points,
								     cur_no_of_points,
								     cur_error,
								     0);
						if (alter_coefficients
						    (cur_points,
						     cur_no_of_points)) {

#ifdef _VERYVERBOSE_
							fprintf(logfile,
								"\trandom jump. impurity %.3f -> %.3f\n",
								cur_error,
								new_error);
#endif

							cur_error = new_error;
							improved_in_this_cycle =
							    TRUE;
						}
					}
				}
			}
		}

		else {		/* best_first or sequential orders of perturbation. */

			improved_in_this_cycle = TRUE;
			cycle_count = 0;

			while (improved_in_this_cycle) {
				if (cur_error == 0.0)
					break;
				cycle_count++;
				improved_in_this_cycle = FALSE;

				if (order_of_perturbation == BEST_FIRST) {
					best_cur_error = HUGE;
					best_coeff_to_improve = 1;
					old_nsp = no_of_stagnant_perturbations;
				}

				for (cur_coeff = 1; cur_coeff < no_of_coeffs;
				     cur_coeff++) {
					new_error =
					    suggest_perturbation(cur_points,
								 cur_no_of_points,
								 cur_coeff,
								 cur_error);
					if (order_of_perturbation == BEST_FIRST) {
						if (new_error < best_cur_error) {
							best_cur_error =
							    new_error;
							best_coeff_to_improve =
							    cur_coeff;
						}
						no_of_stagnant_perturbations =
						    old_nsp;
						if (best_cur_error == 0)
							break;
					} else if (new_error <= cur_error &&
						   alter_coefficients
						   (cur_points,
						    cur_no_of_points)) {

#ifdef _VERYVERBOSE_
						fprintf(logfile,
							"\thill climbing for coeff. %d. impurity %.3f -> %.3f\n",
							cur_coeff, cur_error,
							new_error);
#endif

						cur_error = new_error;
						improved_in_this_cycle = TRUE;
						if (cur_error == 0)
							break;
					}
				}

				if (order_of_perturbation == BEST_FIRST
				    && best_cur_error <= cur_error) {
					cur_coeff = best_coeff_to_improve;
					new_error =
					    suggest_perturbation(cur_points,
								 cur_no_of_points,
								 cur_coeff,
								 cur_error);
					if (alter_coefficients
					    (cur_points, cur_no_of_points)) {

#ifdef _VERYVERBOSE_
						fprintf(logfile,
							"\thill climbing for coeff. %d. impurity %.3f -> %.3f\n",
							cur_coeff, cur_error,
							new_error);
#endif

						cur_error = new_error;
						improved_in_this_cycle = TRUE;
					}
				}

				if (cur_error != 0 && !improved_in_this_cycle) {
					/*Try improving along a random direction */
					i = 0;
					while (cur_error != 0 &&
					       !improved_in_this_cycle &&
					       ++i <=
					       max_no_of_random_perturbations) {
						new_error =
						    perturb_randomly(cur_points,
								     cur_no_of_points,
								     cur_error,
								     cur_label);
						if (alter_coefficients
						    (cur_points,
						     cur_no_of_points)) {

#ifdef _VERYVERBOSE_
							fprintf(logfile,
								"\trandom jump. impurity %.3f -> %.3f\n",
								cur_error,
								new_error);
#endif

							cur_error = new_error;
							improved_in_this_cycle =
							    TRUE;
						}
					}
				}
			}
		}

		if (cur_error < least_error ||
		    (cur_error == least_error && myrandom(0.0, 1.0) > 0.5)) {
			least_error = cur_error;
			for (i = 1; i <= no_of_coeffs; i++)
				best_coeff_array[i] = coeff_array[i];
		}

		if (least_error != 0 && ++restart_count <= no_of_restarts) {
			generate_random_hyperplane(coeff_array, no_of_coeffs,
						   MAX_COEFFICIENT);
			coeff_modified = TRUE;
			find_values(cur_points, cur_no_of_points);
			set_counts(cur_points, cur_no_of_points, 1);
			cur_error = compute_impurity(cur_no_of_points);
		}
	}

	for (i = 1; i <= no_of_coeffs; i++) {
		coeff_array[i] = best_coeff_array[i];
	}
	coeff_modified = TRUE;
	find_values(cur_points, cur_no_of_points);
	set_counts(cur_points, cur_no_of_points, 1);
	return (least_error);

}

/************************************************************************/
/* Module name : Alter_Coefficients                                     */
/* Functionality : First checks if any coefficient values are changed   */
/*                 considerably (> TOLERANCE) in the last perturbation. */
/*                 If they are, updates the "val" fields of the points  */
/*                 to correspond to the new hyperplane. Sets the left_  */
/*                 count and right_count arrays.                        */
/* Parameters : cur_points : Array of pointers to the points            */
/*              cur_no_of_points.                                       */
/* Returns : 1  if any coefficient values are altered,                  */
/*           0  otherwise                                               */
/* Calls modules : set_counts (compute_impurity.c)                      */
/* Is called by modules : oblique_split                                 */
/* Remarks : Assumes that the arrays coeff_array, modified_coeff_array  */
/*           are set. Assumes that the "val" fields of the points       */
/*           correspond to the coefficient values in coeff_array.       */
/************************************************************************/
int alter_coefficients(struct point **cur_points, int cur_no_of_points)
{
	int i, j = 0;

	for (i = 1; i <= no_of_coeffs; i++) {
		if (fabs(coeff_array[i] - modified_coeff_array[i]) > TOLERANCE) {
			if (i != no_of_coeffs) {
				for (j = 1; j <= cur_no_of_points; j++) {
					cur_points[j]->val +=
					    (modified_coeff_array[i] -
					     coeff_array[i]) *
					    cur_points[j]->dimension[i];
				}
			} else {
				for (j = 1; j <= cur_no_of_points; j++) {
					cur_points[j]->val +=
					    (modified_coeff_array[i] -
					     coeff_array[i]);
				}
			}

			coeff_array[i] = modified_coeff_array[i];
		}
	}
	if (j != 0) {
		set_counts(cur_points, cur_no_of_points, 1);
		return (1);
	} else
		return (0);
}


/************************************************************************/
/* Module name : 	axis_parallel_split				*/
/* Functionality : 	Attempts to find the hyperplane, at an axis-	*/
/*			parallel orientation, that best separates	*/
/*			"cur_points" (minimizing the current impurity	*/
/*			measure). 					*/
/* Parameters :	cur_points : array of pointers to the points (samples)	*/
/*			     under consideration.			*/
/*		cur_no_of_points : number of points under consideration.*/
/* Returns :	the impurity of the best hyperplane found.	        */
/*		The hyperplane itself is returned through the global	*/
/*		array "coeff_array".					*/
/* Calls modules :	linear_split (perturb.c)			*/
/*			find_values (perturb.c)				*/
/*			set_counts (compute_impurity.c)			*/
/*			compute_impurity (compute_impurity.c)		*/
/* Is called by modules :	build_subtree				*/
/************************************************************************/
float axis_parallel_split(POINT ** cur_points, int cur_no_of_points)
{
	int i, j, cur_coeff, best_coeff;

	float cur_error, best_error, best_coeff_split_at;

	/*  
	   float linear_split();
	 */

	for (i = 1; i <= no_of_coeffs; i++)
		coeff_array[i] = 0;

	for (cur_coeff = 1; cur_coeff <= no_of_dimensions; cur_coeff++) {
		coeff_array[cur_coeff] = 1;
		for (j = 1; j <= cur_no_of_points; j++) {
			candidates[j].value =
			    cur_points[j]->dimension[cur_coeff];
			candidates[j].cat = cur_points[j]->category;
		}
		coeff_array[no_of_coeffs] =
		    -1.0 * (float) linear_split(cur_no_of_points);

		coeff_modified = TRUE;
		find_values(cur_points, cur_no_of_points);
		set_counts(cur_points, cur_no_of_points, 1);
		cur_error = compute_impurity(cur_no_of_points);

		if (cur_coeff == 1 || cur_error < best_error) {
			best_coeff = cur_coeff;
			best_coeff_split_at = coeff_array[no_of_coeffs];
			best_error = cur_error;
		}

		coeff_array[cur_coeff] = 0;
		coeff_array[no_of_coeffs] = 0;

		if (best_error == 0)
			break;
	}

	coeff_array[best_coeff] = 1;
	coeff_array[no_of_coeffs] = best_coeff_split_at;
	coeff_modified = TRUE;

	return (best_error);
}


/************************************************************************/
/* Module name :	print_log_and_exit				*/
/* Functionality :	prints the log of a run of OC1 into the user-	*/
/*			specified "log_file" (default : oc1.log). Log	*/
/*			mainly consists of the parameter settings for   */
/*                      the particular run.                             */
/* Returns :	Nothing.						*/
/* Calls modules :	none.						*/
/* Is called by modules :	main					*/
/************************************************************************/
int print_log_and_exit(void)
{
	if (strlen(train_data))
		fprintf(logfile, "Training data : %s\n", train_data);

	if (no_of_folds)
		fprintf(logfile,
			"%d-fold cross validation used to estimate accuracy.\n",
			no_of_folds);
	else if (strlen(test_data))
		fprintf(logfile, "Testing data : %s\n", test_data);

	fprintf(logfile, "Data is %d-dimensional, having %d classes.\n",
		no_of_dimensions, no_of_categories);

	if (axis_parallel == FALSE)
		fprintf(logfile, "No axis-parallel splits considered.\n");
	if (oblique == FALSE)
		fprintf(logfile, "No oblique splits considered.\n");
	else {
		fprintf(logfile,
			"Parameters for finding oblique splits at each node :\n");
		fprintf(logfile, "\tNumber of restarts = %d\n", no_of_restarts);
		if (order_of_perturbation == BEST_FIRST)
			fprintf(logfile,
				"\tOrder of coefficient perturbation = Best First\n");
		else if (order_of_perturbation == RANDOM)
			fprintf(logfile,
				"\tOrder of coefficient perturbation = Random-%d\n",
				cycle_count);
		else
			fprintf(logfile,
				"\tOrder of coefficient perturbation = Sequential\n");
		fprintf(logfile,
			"\tMaximum number of random perturbations tried at each ");
		fprintf(logfile, "local minimum = %d\n",
			max_no_of_random_perturbations);
		if (normalize == FALSE)
			fprintf(logfile, "No normalization was used.\n");
	}

	if (strlen(train_data)) {
		if (no_of_folds == 0) {
			fprintf(logfile,
				"Unpruned decision tree written to %s.\n",
				dt_file);
		} else {
			fprintf(logfile,
				"Unpruned tree of the first fold written to %s.\n",
				dt_file);
		}
	} else if (strlen(test_data))
		fprintf(logfile, "Decision tree read from %s.\n", dt_file);

	if (strlen(misclassified_data) && no_of_folds != 0)
		fprintf(logfile, "Misclassified points written to %s.\n",
			misclassified_data);

	fprintf(logfile, "\n");

	fclose(logfile);
	return (1);

}


/************************************************************************/
/* Module name : Normalize_Data                                         */
/* Functionality : Translates all points to lie in the positive         */
/*                 quadrant (a requirement for OC1's algorithm).        */
/* Parameters :    points: array of points to be normalized.            */
/*                 no_of_points: number of points.                      */
/* Returns : Nothing.                                                   */
/* Calls modules :  vector (util.c)                                     */
/*                  free_fvector (util.c)                                */
/*                  average (util.c)                                    */
/*                  sdev (util.c)                                       */
/* Is called by modules :  build_subtree (mktree.c)                     */
/* Important Variables used : attribute_avg,attribute_sdev,attribute_min:*/
/*           Global arrays that maintain the average, standard deviation*/
/*           and minimum of the instance set seen most recently.        */
/* Remarks : This normalization is done at every tree node. The         */
/*           hyperplane induced is subsequently modified (in the module */
/*           unnormalize_hyperplane) to correspond to the original data.*/
/************************************************************************/
int normalize_data(struct point **points, int no_of_points)
{
	int i, j;

	float *temp;
	/*
	   average(),
	   sdev(),
	   min() ;
	 */

	temp = fvector(1, no_of_points);

	for (j = 1; j <= no_of_dimensions; j++) {
		for (i = 1; i <= no_of_points; i++) {
			temp[i] = points[i]->dimension[j];
		}
		attribute_min[j] = min(temp, no_of_points);
		if (attribute_min[j] < 0)
			for (i = 1; i <= no_of_points; i++)
				points[i]->dimension[j] -= attribute_min[j];
	}
	free_fvector(temp, 1, no_of_points);

	return (1);		/* NINA */
}

/************************************************************************/
/* Module name : Unnormalize_Data                                       */
/* Functionality : Removes the effects of normalization on data.        */
/* Parameters : points: Array of pointers to the point structures.      */
/*              no_of_points.                                           */
/* Returns : Nothing.                                                   */
/* Calls modules :                                                      */
/* Is called by modules : build_subtree                                 */
/* Important Variables used : attribute_min, attribute_avg,             */
/*                            attribute_sdev: global arrays that        */
/*                            maintain the minimum, average and standard*/
/*                            deviation along all attributes of the     */
/*                            data subset last seen.                    */
/* Remarks : Assumes that the data is normalized.                       */
/************************************************************************/
int unnormalize_data(struct point **points, int no_of_points)
{
	int i, j;

	for (j = 1; j <= no_of_dimensions; j++) {
		if (attribute_min[j] < 0)
			for (i = 1; i <= no_of_points; i++)
				points[i]->dimension[j] += attribute_min[j];
	}

	return (1);		/* NINA */
}

/************************************************************************/
/* Module name : Unnormalize_Hyperplane                                 */
/* Functionality : Removes the effects of normalization on the oblique  */
/*                 hyperplane induced.                                  */
/* Parameters : None.                                                   */
/* Returns : Nothing.                                                   */
/* Calls modules : None.                                                */
/* Is called by modules : build_subtree                                 */
/* Important Variables used : attribute_min, attribute_avg,             */
/*                            attribute_sdev: global arrays that        */
/*                            maintain the minimum, average and standard*/
/*                            deviation along all attributes of the     */
/*                            data subset last seen.                    */
/* Remarks : Assumes that the data is normalized.                       */
/************************************************************************/
int unnormalize_hyperplane(void)
{
	int i;

	for (i = 1; i <= no_of_dimensions; i++)
		if (attribute_min[i] < 0)
			coeff_array[no_of_coeffs] -=
			    coeff_array[i] * attribute_min[i];

	return (1);		/* NINA */
}
