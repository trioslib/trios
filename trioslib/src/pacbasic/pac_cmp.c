#include <pacbasic.h>

/* #define _DEBUG_ */

/*!
  Compare two files in ASCII format and evaluate if they are equal or not. It ignores tabs, '\n' and spaces.

  \param i1 Name of the first file to be compared.
  \param i2 Name of the second file to be compared.
  \return Return: -1 if there was any error while opening the files, 0 if the files are different or 1 if they are equal.
  */
int lpaccmp(char *i1, char *i2) {
  FILE *f1, *f2;
  int  a, igual;
  char ch1, ch2, *palavra1, *palavra2;
  

  palavra1 = (char *)malloc(sizeof (char)*256);
  palavra2 = (char *)malloc(sizeof (char)*256);


#ifdef _DEBUG_
pac_debug("Entering lpaccmp()\n");
#endif

  if ((f1=fopen(i1,"r"))==NULL) {
    (void) pac_error(1, "The program was unable to open the first input file.");
    return(-1);    
  }


  if ((f2=fopen(i2,"r"))==NULL) {
    (void) pac_error(1, "The program was unable to open the second input file.");
    return(-1);    
  }


  igual=1;
  
  while ( ( (!feof(f1)) && (!feof(f2)) ) && igual ) {
    
    /* ignore STDIN while it's spaces, tabs, form feed, new line or vertical tabs */
    while (isspace(ch1=getc(f1)));

    while (isspace(ch2=getc(f2)));

    /* read one word from the first file */
    if (!isspace(ch1)) {
      a=1;
      *palavra1=ch1;
      while (isalpha(ch1=getc(f1))) {
	*(palavra1+a)=ch1;
	a++;
      }
      *(palavra1+a)='\0';

#ifdef _DEBUG_
pac_debug("%s\n",palavra1);
#endif

      ungetc(ch1, f1);
    }
    
    /* read one word from the second file */
    if (!isspace(ch2)) {
      a=1;
      *palavra2=ch2;
      while (isalpha(ch2=getc(f2))) {
	*(palavra2+a)=ch2;
	a++;
      }
      *(palavra2+a)='\0';

#ifdef _DEBUG_
pac_debug("%s\n",palavra2);
#endif

      ungetc(ch2, f2);
    }
    
    a=0;
    igual=1;
    
    /* compare the two words */
    while ((*(palavra1+a)!='\0') && (*(palavra2+a)!='\0') && igual) {
      if (*(palavra1+a)!= *(palavra2+a)) igual=0;
      a++;
    }
      
  }
  
#ifdef _DEBUG_
pac_debug("Leaving lpaccmp()\n");
#endif
  
  if ( (igual) && feof(f1) && feof(f2) ) {
    fclose(f1);
    fclose(f2);
    return(1);
  }
  else {
    fclose(f1);
    fclose(f2);
    return(0);
  }
  
}
