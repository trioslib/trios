#include "trios_misc.h"
#include "trios_win.h"

/* Macro that computes the bit mask for the POSITION-th bit. */
#define BITMSK(POSITION) ( 1 << (POSITION) )

unsigned int bitmsk[NB] =
    { 1, 2, 4, 8, 0x10, 0x20, 0x40, 0x80, 0x100, 0x200, 0x400,
	0x800, 0x1000, 0x2000, 0x4000, 0x8000, 0x10000, 0x20000,
	0x40000, 0x80000, 0x100000, 0x200000, 0x400000, 0x800000,
	0x1000000, 0x2000000, 0x4000000, 0x8000000, 0x10000000,
	0x20000000, 0x40000000, 0x80000000
};

/* The vector DIM is used to compute the number of bits ON
 in a sequence of bytes (typically a vector of int)
 DIM[i] gives the number of bits ON the number i contains
 (0 <= i <= 255)                                          */
int DIM[256] = {
	/*   0 */ 0,
	/*   1 */ 1,
	/*   2 */ 1,
	/*   3 */ 2,
	/*   4 */ 1,
	/*   5 */ 2,
	/*   6 */ 2,
	/*   7 */ 3,
	/*   8 */ 1,
	/*   9 */ 2,
	/*  10 */ 2,
	/*  11 */ 3,
	/*  12 */ 2,
	/*  13 */ 3,
	/*  14 */ 3,
	/*  15 */ 4,
	/*  16 */ 1,
	/*  17 */ 2,
	/*  18 */ 2,
	/*  19 */ 3,
	/*  20 */ 2,
	/*  21 */ 3,
	/*  22 */ 3,
	/*  23 */ 4,
	/*  24 */ 2,
	/*  25 */ 3,
	/*  26 */ 3,
	/*  27 */ 4,
	/*  28 */ 3,
	/*  29 */ 4,
	/*  30 */ 4,
	/*  31 */ 5,
	/*  32 */ 1,
	/*  33 */ 2,
	/*  34 */ 2,
	/*  35 */ 3,
	/*  36 */ 2,
	/*  37 */ 3,
	/*  38 */ 3,
	/*  39 */ 4,
	/*  40 */ 2,
	/*  41 */ 3,
	/*  42 */ 3,
	/*  43 */ 4,
	/*  44 */ 3,
	/*  45 */ 4,
	/*  46 */ 4,
	/*  47 */ 5,
	/*  48 */ 2,
	/*  49 */ 3,
	/*  50 */ 3,
	/*  51 */ 4,
	/*  52 */ 3,
	/*  53 */ 4,
	/*  54 */ 4,
	/*  55 */ 5,
	/*  56 */ 3,
	/*  57 */ 4,
	/*  58 */ 4,
	/*  59 */ 5,
	/*  60 */ 4,
	/*  61 */ 5,
	/*  62 */ 5,
	/*  63 */ 6,
	/*  64 */ 1,
	/*  65 */ 2,
	/*  66 */ 2,
	/*  67 */ 3,
	/*  68 */ 2,
	/*  69 */ 3,
	/*  70 */ 3,
	/*  71 */ 4,
	/*  72 */ 2,
	/*  73 */ 3,
	/*  74 */ 3,
	/*  75 */ 4,
	/*  76 */ 3,
	/*  77 */ 4,
	/*  78 */ 4,
	/*  79 */ 5,
	/*  80 */ 2,
	/*  81 */ 3,
	/*  82 */ 3,
	/*  83 */ 4,
	/*  84 */ 3,
	/*  85 */ 4,
	/*  86 */ 4,
	/*  87 */ 5,
	/*  88 */ 3,
	/*  89 */ 4,
	/*  90 */ 4,
	/*  91 */ 5,
	/*  92 */ 4,
	/*  93 */ 5,
	/*  94 */ 5,
	/*  95 */ 6,
	/*  96 */ 2,
	/*  97 */ 3,
	/*  98 */ 3,
	/*  99 */ 4,
	/* 100 */ 3,
	/* 101 */ 4,
	/* 102 */ 4,
	/* 103 */ 5,
	/* 104 */ 3,
	/* 105 */ 4,
	/* 106 */ 4,
	/* 107 */ 5,
	/* 108 */ 4,
	/* 109 */ 5,
	/* 110 */ 5,
	/* 111 */ 6,
	/* 112 */ 3,
	/* 113 */ 4,
	/* 114 */ 4,
	/* 115 */ 5,
	/* 116 */ 4,
	/* 117 */ 5,
	/* 118 */ 5,
	/* 119 */ 6,
	/* 120 */ 4,
	/* 121 */ 5,
	/* 122 */ 5,
	/* 123 */ 6,
	/* 124 */ 5,
	/* 125 */ 6,
	/* 126 */ 6,
	/* 127 */ 7,
	/* 128 */ 1,
	/* 129 */ 2,
	/* 130 */ 2,
	/* 131 */ 3,
	/* 132 */ 2,
	/* 133 */ 3,
	/* 134 */ 3,
	/* 135 */ 4,
	/* 136 */ 2,
	/* 137 */ 3,
	/* 138 */ 3,
	/* 139 */ 4,
	/* 140 */ 3,
	/* 141 */ 4,
	/* 142 */ 4,
	/* 143 */ 5,
	/* 144 */ 2,
	/* 145 */ 3,
	/* 146 */ 3,
	/* 147 */ 4,
	/* 148 */ 3,
	/* 149 */ 4,
	/* 150 */ 4,
	/* 151 */ 5,
	/* 152 */ 3,
	/* 153 */ 4,
	/* 154 */ 4,
	/* 155 */ 5,
	/* 156 */ 4,
	/* 157 */ 5,
	/* 158 */ 5,
	/* 159 */ 6,
	/* 160 */ 2,
	/* 161 */ 3,
	/* 162 */ 3,
	/* 163 */ 4,
	/* 164 */ 3,
	/* 165 */ 4,
	/* 166 */ 4,
	/* 167 */ 5,
	/* 168 */ 3,
	/* 169 */ 4,
	/* 170 */ 4,
	/* 171 */ 5,
	/* 172 */ 4,
	/* 173 */ 5,
	/* 174 */ 5,
	/* 175 */ 6,
	/* 176 */ 3,
	/* 177 */ 4,
	/* 178 */ 4,
	/* 179 */ 5,
	/* 180 */ 4,
	/* 181 */ 5,
	/* 182 */ 5,
	/* 183 */ 6,
	/* 184 */ 4,
	/* 185 */ 5,
	/* 186 */ 5,
	/* 187 */ 6,
	/* 188 */ 5,
	/* 189 */ 6,
	/* 190 */ 6,
	/* 191 */ 7,
	/* 192 */ 2,
	/* 193 */ 3,
	/* 194 */ 3,
	/* 195 */ 4,
	/* 196 */ 3,
	/* 197 */ 4,
	/* 198 */ 4,
	/* 199 */ 5,
	/* 200 */ 3,
	/* 201 */ 4,
	/* 202 */ 4,
	/* 203 */ 5,
	/* 204 */ 4,
	/* 205 */ 5,
	/* 206 */ 5,
	/* 207 */ 6,
	/* 208 */ 3,
	/* 209 */ 4,
	/* 210 */ 4,
	/* 211 */ 5,
	/* 212 */ 4,
	/* 213 */ 5,
	/* 214 */ 5,
	/* 215 */ 6,
	/* 216 */ 4,
	/* 217 */ 5,
	/* 218 */ 5,
	/* 219 */ 6,
	/* 220 */ 5,
	/* 221 */ 6,
	/* 222 */ 6,
	/* 223 */ 7,
	/* 224 */ 3,
	/* 225 */ 4,
	/* 226 */ 4,
	/* 227 */ 5,
	/* 228 */ 4,
	/* 229 */ 5,
	/* 230 */ 5,
	/* 231 */ 6,
	/* 232 */ 4,
	/* 233 */ 5,
	/* 234 */ 5,
	/* 235 */ 6,
	/* 236 */ 5,
	/* 237 */ 6,
	/* 238 */ 6,
	/* 239 */ 7,
	/* 240 */ 4,
	/* 241 */ 5,
	/* 242 */ 5,
	/* 243 */ 6,
	/* 244 */ 5,
	/* 245 */ 6,
	/* 246 */ 6,
	/* 247 */ 7,
	/* 248 */ 5,
	/* 249 */ 6,
	/* 250 */ 6,
	/* 251 */ 7,
	/* 252 */ 6,
	/* 253 */ 7,
	/* 254 */ 7,
	/* 255 */ 8
};

/*! Computes the number of bits ON of an integer (4 bytes)
  m_dim : the size vector
 m_d : the size of the integer
 m_X : the integer
 */
#define Comp_Dim(m_X, m_dim, m_d)    \
        {                            \
          int m_k;                   \
                                     \
          m_d = 0 ;                  \
          m_k = m_X & 0xff ;         \
          m_d = m_d + m_dim[m_k] ;   \
          m_k = (m_X>>8)&0xff ;      \
          m_d = m_d + m_dim[m_k] ;   \
          m_k = (m_X>>16)&0xff ;     \
          m_d = m_d + m_dim[m_k] ;   \
          m_k = (m_X>>24)&0xff ;     \
          m_d = m_d + m_dim[m_k] ;   \
        }

/*
     -------------------------------------------
     FUNCTION: wpat_dimension
     -------------------------------------------
*/
/*!
  Computes the number of bits ON in a W-pattern.

  \param wpat Binary W-pattern.
  \param wzip W-pattern compacted size.
  \return Number of bits ON in a W-pattern
*/
int wpat_dimension(unsigned int *wpat, int wzip)
{
	int i, dimension, aux;

	dimension = 0;
	for (i = 0; i < wzip; i++) {
		Comp_Dim(wpat[i], DIM, aux);

		dimension += aux;
	}

	return (dimension);
}

/*!
  Verify the order relation between two w-patterns.

  \param wpat1 First W-pattern.
  \param wpat2 Second W-pattern.
  \param wzip Compacted W-pattern size.
  \return 3 if they cannot be compared, 0 if they are equal, 1 if wpat1 < wpat2, and 2 if wpat1 > wpat2.
  */
int wpat_order(unsigned int *wpat1, unsigned int *wpat2, int wzip)
{
	int i;
	int retcode;

	retcode = 0;

	for (i = wzip - 1; i >= 0; i--) {
		if (wpat1[i] != wpat2[i]) {
			if ((wpat1[i] & wpat2[i]) == wpat1[i]) {
				retcode = retcode | 1;
			} else {
				if ((wpat1[i] & wpat2[i]) == wpat2[i]) {
					retcode = retcode | 2;
				} else
					retcode = retcode | 3;
			}

		}
	}

	return (retcode);
}

/*!
  Verify whether a given w-pattern is greater than another one.

  \param wpat1 First W-pattern.
  \param wpat2 Second W-pattern.
  \param wzip Compacted W-pattern size.
  \return 1 if wpat1 > wpat2, 0 otherwise.
  */
int wpat_greater_than(unsigned int *wpat1, unsigned int *wpat2, int wzip)
{
	int i;

	for (i = 0; i < wzip; i++) {
		if ((wpat1[i] & wpat2[i]) != wpat2[i])
			return (0);
	}

	return (1);
}

/*!
  Create the offset vector according to window size.

  \param wsize Window size.
  \return Pointer to offset vector if successful or NULL in case of failure.
 */
int *offset_create(int wsize)
{
	int *offset;
	offset = (int *)malloc(sizeof(int) * wsize);
	return offset;
}

/*!
  Set the offset vector according to a window and an image.

  \param offset Offset vector.
  \param win Pointer to the window.
  \param width Width of the image.
  \param band Band of the image.
*/

void offset_set(int *offset, window_t * win, int width, int band)
{
	int h_low, h_high, w_low, w_high;
	int i, j, k, l;
	char *windata;

	h_high = (win_get_height(win)) / 2;
	h_low = (win_get_height(win) - 1) / 2;

	w_high = (win_get_width(win)) / 2;
	w_low = (win_get_width(win) - 1) / 2;

	/*
	   Calculate the relative position of pixels associated to each point
	   of the window in relation to the pixel in the center of the
	   window.
	 */

	k = win_get_height(win) * win_get_width(win) * (band - 1);
	l = 0;
	windata = win_get_data(win);

	for (i = -h_low; i <= h_high; i++) {
		for (j = -w_low; j <= w_high; j++) {
			if (windata[k] != 0) {
				offset[l] = i * width + j;
				l++;
			}
			k++;
		}
	}
}

/*!
  Compate two W-patterns.

  \param wpat1 First W-pattern.
  \param wpat2 Second W-pattern.
  \param wzip Compacted W-pattern size.
  \return 0 if they are equal, -1 if the first is less and 1 if the first is greater than the second.
*/

int wpat_compare(unsigned int *wpat1, unsigned int *wpat2, int wzip)
{
	int i;

	for (i = wzip - 1; i >= 0; i--) {
		if (wpat1[i] > wpat2[i])
			return (1);
		if (wpat1[i] < wpat2[i])
			return (-1);
	}

	return (0);
}

/*!
 * Compares two XG W-patterns.
 * \param wpat1 First W-pattern.
 * \param wpat2 Second  W-pattern.
 * \param wsize W-pattern size.
 * \return 0 if they are equal, -1 if the first is less and 1 if the first is greater than the second.
 */
int wpat_compareXG(int *wpat1, int *wpat2, int wsize)
{
	int i;

	for (i = wsize - 1; i >= 0; i--) {
		if (wpat1[i] > wpat2[i])
			return (1);
		if (wpat1[i] < wpat2[i])
			return (-1);
	}

	return (0);
}

/*!
  Computes the Hamming distance between two binary w-patterns.

  \param wpat1 First W-pattern.
  \param wpat2 Second W-pattern.
  \param wzip Compacted W-pattern size.
  \return An integer between 0 and n (where n stands for wsize).
*/

int wpat_hamming(unsigned int *wpat1, unsigned int *wpat2, int wzip)
{
	int wpat_xor, dist;
	int i, j;

	dist = 0;
	for (i = 0; i < wzip; i++) {
		wpat_xor = (wpat1[i] ^ wpat2[i]);	/* exclusive OR */
		for (j = 0; j < NB; j++) {
			dist = dist + ((wpat_xor >> j) & 1);
		}
	}

	return (dist);
}

/*!
  Calculate the size of compacted w-pattern.

  \param wsize Size of W-pattern.
  \return The compacted W-pattern size.
*/

int size_of_zpat(int wsize)
{
	return ((wsize + 31) / 32);
}

/*======================================================================== */
/*        Routines to do the treatment of the list of frequencies          */

/*!
  Creates a node for the frequency list.

  \param label Label of the pattern.
  \param freq Frequency of the pattern.
  \return Created freq_node.
*/
freq_node *freq_node_create(int label, unsigned int freq)
{
	freq_node *p;

	p = (freq_node *) malloc(sizeof(freq_node));
	if (p == NULL) {
		return (freq_node *) trios_error(1,
						 "Memory allocation failed.");
	}

	p->freq = freq;
	p->label = label;
	p->next = NULL;

	return (p);

}

/*!
  Free the memory of a freq_node.

  \param freq_list freq_node to free.
*/

void freqlist_free(freq_node * freq_list)
{
	/* A non-recursive algorithm will be used here */
	freq_node *p, *paux;
	p = freq_list;

	while (p) {

		paux = p->next;
		free(p);
		p = paux;

	}

}

/*!
  Searches for each label of the input freq list in the frequency list and adjusts its frequency if it exists, if it doesn't exist, puts the label into the list.

  \param freqlstin Pointer to the input frequency list.
  \param freqlist Pointer to another frequency list.
  \return 1 on succes, 0 on failure.
*/

int set_freq(freq_node * freqlstin, freq_node ** freqlist)
{
	int label;
	int freq;
	freq_node *pfreq, *pfreqprev, *qfreq, *qfreqnext;

	if (*freqlist == NULL) {
		*freqlist = freqlstin;

#ifdef _DEBUG_
		trios_debug("Entrou no xpl_set_freq com freqlist = null");
		trios_debug("label1=%d , freq1=%d\n", freqlstin->label,
			    freqlstin->freq);
#endif

	}

	else {

#ifdef _DEBUG_
		trios_debug("Entrou no xpl_set_freq");
		trios_debug("label1=%d , freq1=%d\n", freqlstin->label,
			    freqlstin->freq);
#endif

		qfreq = freqlstin;
		pfreqprev = NULL;
		pfreq = *freqlist;

		while (qfreq) {

			label = qfreq->label;
			freq = qfreq->freq;

			qfreqnext = qfreq->next;

			while ((pfreq) && (pfreq->label < label)) {
				pfreqprev = pfreq;
				pfreq = pfreq->next;
			}

			/* Has reached the end of the current list of frequencies? YES,  */
			/* then insert the remaining of the input list to the end of the */
			/* current one                                                   */

			if (pfreq == NULL) {

				pfreqprev->next = qfreq;
				qfreq = NULL;

			}

			else {	/* No, has the node the searched label ? */

				if (pfreq->label == label) {	/* YES, then update information  */

					pfreq->freq += freq;
					free(qfreq);
				}

				else {	/* NO, then insert the node. */

					if (pfreqprev == NULL) {

						qfreq->next = pfreq;
						*freqlist = qfreq;

					}

					else {

						qfreq->next = pfreq;
						pfreqprev->next = qfreq;

					}

				}

			}

			qfreq = qfreqnext;

		}

	}

	return (1);

}

/*!
  Sums the frequency of a W-pattern.

  \param freqlist Frequency list of a W-pattern.
  \return The sum of the frequencies of the labels that appeared in the W-pattern.
*/
unsigned int freq_sum(freq_node * freqlist)
{
	freq_node *p;
	unsigned int sum;

	p = freqlist;
	sum = 0;

	while (p) {
		sum += p->freq;
		p = p->next;

	}

	return (sum);

}

/*
     -------------------------------------------
     FUNCTION: freq_moda
     -------------------------------------------
*/

int /*+ Purpose: Finds the moda of the labels                    + */ freq_moda(
										       freq_node * pqfreq,	/*+ In: pointer to a frequency node               + */
										       unsigned int sum,	/*+ In: Total number of examples                  + */
										       double freqmin,	/*+ In: Minimal frequency to consider an example  + */
										       int undflabel,	/*+ In: Value assigned to undecidable cases       + */
										       int comptype,	/*+ In: Type of computation to be done for
													   assigning the label of the undecidable
													   pattern                                   + */
										       int *wpat_label,	/*+ Out: Moda                                     + */
										       unsigned int *wpat_freq	/*+ Out: The frequency of a pattern              + */
    )
/*+ Return: 1 on success, -1 on criteria not satisfied and 0 on failure      +*/
{
/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                   */
/*  date: Wed Feb 16 2000                                                     */

	unsigned int maximum_freq,	/* an auxiliar variable to keep the maximum frequency   */
	 label_freq,		/* the frequency of a given label                       */
	 nmoda,			/* the number of conflicting labels                     */
	 label_sum;		/* the total sum of conflicting labels                  */

	int most_frequent_label;	/* the labels of a pattern                     */

	float verify;		/* used to verify if the pattern was sufficiently
				   observed to be taken in consideration             */
	freq_node *qfreq;

#ifdef _DEBUG_
	trios_debug("Starting freq moda");
#endif

	qfreq = pqfreq;
	*wpat_freq = 0;
	maximum_freq = 0;
	nmoda = 1;

	/* scans the linked list and finds the most frequent label */

	while (qfreq) {
		most_frequent_label = qfreq->label;
		label_freq = qfreq->freq;

		if (maximum_freq < label_freq) {
			nmoda = 1;
			*wpat_label = most_frequent_label;
			maximum_freq = label_freq;
		} else if (maximum_freq == label_freq) {
			nmoda++;
		}

		*wpat_freq = *wpat_freq + label_freq;
		qfreq = qfreq->next;

	}

	verify = ((float)*wpat_freq / sum) * 100;

	if (verify >= freqmin) {

		if (nmoda > 1) {

			switch (comptype) {

			case 1:	/* Assign undflabel to most_frequent_label */
				*wpat_label = undflabel;
				break;

			case 2:	/* most_frequent_label will receive the mean of the
					   moda's labels                                      */
				qfreq = pqfreq;

				/* scans the linked list again to compute the mean */

				while (qfreq) {
					most_frequent_label = qfreq->label;
					label_freq = qfreq->freq;
					if (label_freq == maximum_freq) {
						label_sum += *wpat_label;
					}

					qfreq = qfreq->next;

				}

				*wpat_label = (int)((float)label_sum / nmoda);

				break;

			case 3:{	/* most_frequent_label will receive the median of the
					   moda's labels                                      */
					int contador;

					qfreq = pqfreq;

					/* scans the linked list again to compute the mean */
					contador = 0;
					while ((qfreq)
					       && (contador < nmoda / 2)) {
						*wpat_label = qfreq->label;
						label_freq = qfreq->freq;
						if (label_freq == maximum_freq) {
							contador++;
						}

						qfreq = qfreq->next;

					}

					*wpat_label = most_frequent_label;
					break;
				}
			default:
				return (0);
			}

		}

		return (1);
	}

	else
		return (-1);	/* Criteria not satisfied (verify < freqmin) */

}

/*
     -------------------------------------------
     FUNCTION: freq_average
     -------------------------------------------
*/

int /*+ Purpose: Finds the average of the labels                 + */
freq_average(
			   freq_node * qfreq,	/*+ In: pointer to a frequency node               + */
			   unsigned int sum,	/*+ In: Total number of examples                  + */
			   double freqmin,	/*+ In: Minimal frequency to consider an example  + */
			   int *wpat_label,	/*+ Out: Average                                 + */
			   unsigned int *wpat_freq	/*+ Out: The frequency of a pattern              + */
    )
/*+ Return: 1 on success, -1 on criteria not satisfied                       +*/
{

/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                 */
/*  date: Wed Feb 16 2000                                      */

	int pre_average;	/* the sum of the products of the labels times their */
	/* frequency                                         */

	float verify;		/* used to verify if the pattern was sufficiently */
	/* observed to be taken in consideration          */

#ifdef _DEBUG_
	trios_debug("Starting freq average");
#endif

	*wpat_freq = 0;
	pre_average = 0;

	/* scans the linked list and calculates some values  */

	while (qfreq) {
		pre_average += qfreq->label * qfreq->freq;
		*wpat_freq += qfreq->freq;
		qfreq = qfreq->next;
	}

	verify = ((float)*wpat_freq / sum) * 100;

	if (verify >= freqmin) {

		/* Take the average and trunkate */

		if (pre_average >= 0) {
			*wpat_label =
			    (int)((float)pre_average / (*wpat_freq) + 0.5);
		} else
			*wpat_label =
			    (int)((float)pre_average / (*wpat_freq) - 0.5);

		return (1);

	}

	return (-1);

}

/*
     -------------------------------------------
     FUNCTION: freq_median
     -------------------------------------------
*/

int /*+ Purpose: Finds the median of the labels                 + */ freq_median(
											freq_node * qfreq,	/*+ In: pointer to a frequency node               + */
											unsigned int sum,	/*+ In: Total number of examples                  + */
											double freqmin,	/*+ In: Minimal frequency to consider an example  + */
											int *wpat_label,	/*+ Out: Average                                  + */
											unsigned int *wpat_freq	/*+ Out: The frequency of a pattern            + */
    )
/*+ Return: 1 on success, -1 on criteria not satisfied                      +*/
{
/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                  */
/*  date: Wed Feb 16 2000                                                    */

	unsigned int wpat_sub_freq;	/* the subtotal frequency of the pattern     */
	int wpat_freq_median;	/* the median of the frequencies of the pattern */

	float verify;

	/* computes the total frequence of the w-pat and the mean */
	*wpat_freq = freq_sum(qfreq);
	wpat_freq_median = (int)((float)*wpat_freq / 2 + 0.5);
	/* scans the linked list and computes the median  */

	wpat_sub_freq = 0;

	while (wpat_sub_freq < wpat_freq_median) {
		*wpat_label = qfreq->label;

		wpat_sub_freq += qfreq->freq;

		qfreq = qfreq->next;
	}

	verify = ((float)*wpat_freq / sum) * 100;

	if (verify >= freqmin)
		return (1);

	return (-1);

}

/*
     -------------------------------------------
     FUNCTION: freq_absolute
     -------------------------------------------
*/

int /*+ Purpose: Finds the non-conflicting labels               + */
freq_absolute(
			    freq_node * pqfreq,	/*+ In: pointer to a frequency node               + */
			    unsigned int sum,	/*+ In: Total number of examples                  + */
			    double freqmin,	/*+ In: Minimal frequency to consider an example  + */
			    int undflabel,	/*+ In: Value assigned to undecidable cases       + */
			    int *wpat_label,	/*+ Out: Average                                 + */
			    unsigned int *wpat_freq	/*+ Out: The frequency of a pattern              + */
    )
/*+ Return: 1 on success, -1 on criteria not satisfied                       +*/
{
/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                 */
/*  date: Wed Feb 16 2000                                      */

	float verify;		/* used to verify if the pattern was sufficiently */
	/* observed to be taken in consideration          */
	freq_node *qfreq;

	qfreq = pqfreq;

	*wpat_freq = 0;

	/* scans the linked list and calculates some values  */

	*wpat_label = qfreq->label;
	*wpat_freq = qfreq->freq;
	qfreq = qfreq->next;

	if (qfreq) {
		*wpat_label = undflabel;
		*wpat_freq = freq_sum(pqfreq);
	}

	verify = ((float)*wpat_freq / sum) * 100;

	if (verify >= freqmin) {
		return (1);
	}

	return (-1);

}

/*
     -------------------------------------------
     FUNCTION: freq_general
     -------------------------------------------
*/

int				/*+ Purpose: Finds the moda of the labels and decides what to
				   do based on the estimator                       + */ freq_general(
													    freq_node * pqfreq,	/*+ In: pointer to a frequency node               + */
													    unsigned int sum,	/*+ In: Total number of examples                  + */
													    double freqmin,	/*+ In: Minimal frequency to consider an example  + */
													    double estimator,	/*+ In: Decision Estimator                        + */
													    int undflabel,	/*+ In: Value assigned to undecidable cases       + */
													    int comptype,	/*+ In: Type of computation to be done for
																   assigning the label of the undecidable
																   pattern                                   + */
													    int *wpat_label,	/*+ Out: Moda                                    + */
													    unsigned int *wpat_freq	/*+ Out: The frequency of a pattern              + */
    )
/*+ Return: 1 on success, -1 on criteria not satisfied and 0 on failure      +*/
{
/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                   */
/*  date: Wed Feb 16 2000                                                     */

	unsigned int maximum_freq,	/* an auxiliar variable to keep the maximum frequency   */
	 label_freq,		/* the frequency of a given label                       */
	 nmoda,			/* the number of conflicting labels                     */
	 label_sum;		/* the total sum of conflicting labels                  */

	int most_frequent_label;	/* the labels of a pattern                     */

	float verify;		/* used to verify if the pattern was sufficiently
				   observed to be taken in consideration             */
	freq_node *qfreq;

	qfreq = pqfreq;
	*wpat_freq = 0;
	maximum_freq = 0;
	nmoda = 1;

	/* scans the linked list and finds the most frequent label */

	while (qfreq) {
		most_frequent_label = qfreq->label;
		label_freq = qfreq->freq;

		if (maximum_freq < label_freq) {
			nmoda = 1;
			*wpat_label = most_frequent_label;
			maximum_freq = label_freq;
		} else if (maximum_freq == label_freq) {
			nmoda++;
		}

		*wpat_freq = *wpat_freq + label_freq;
		qfreq = qfreq->next;

	}

	verify = ((float)*wpat_freq / sum) * 100;

	if (verify >= freqmin) {

		if (((float)maximum_freq / *wpat_freq) >= estimator) {

			if (nmoda > 1) {

				switch (comptype) {

				case 1:	/* Assign undflabel to most_frequent_label */
					*wpat_label = undflabel;
					break;

				case 2:	/* most_frequent_label will receive the mean of the
						   moda's labels                                      */
					qfreq = pqfreq;

					/* scans the linked list again to compute the mean */

					while (qfreq) {
						most_frequent_label =
						    qfreq->label;
						label_freq = qfreq->freq;
						if (label_freq == maximum_freq) {
							label_sum +=
							    *wpat_label;
						}

						qfreq = qfreq->next;

					}

					*wpat_label =
					    (int)((float)label_sum / nmoda);

					break;

				case 3:{	/* most_frequent_label will receive the median of the
						   moda's labels                                      */
						int contador;

						qfreq = pqfreq;

						/* scans the linked list again to compute the mean */
						contador = 0;
						while ((qfreq)
						       && (contador <
							   nmoda / 2)) {
							most_frequent_label =
							    qfreq->label;
							label_freq =
							    qfreq->freq;
							if (label_freq ==
							    maximum_freq) {
								contador++;
							}

							qfreq = qfreq->next;

						}

						*wpat_label =
						    most_frequent_label;
						break;
					}
				default:
					return (0);
				}

			}

		}

		return (1);

	}

	else
		return (-1);	/* Criteria not satisfied (verify < freqmin) */

}
