/* 
File: datafun.c
Class representing a plottable function z=f(x,y)
where f is interpolated from raw data.
There is a matrix storing a set of data values.
*/

#include <stdlib.h>
#include "datafun.h"

/* constructor */
DataFunction :: DataFunction()
{
  data = NULL;
  sizeX = sizeY = 0;
  numRow = numCol = 1;
  minX = minY = maxX = maxY = 0.0;
}

/* set the function, and also compute the limits of the function
   range based on the current grid */
int DataFunction :: readFunction(FILE * fd)
{
   int i, j;
   if (fscanf(fd, "%d %d", &sizeX, &sizeY)!=2) goto READING_ERROR;
   if ( (sizeX<=0) || (sizeY<=0) ) goto READING_ERROR;
   data = (double **) calloc(sizeY,sizeof(double *));
   if (!data) goto READING_ERROR;
   for (j=0; j<sizeY; j++)
   {  data[j] = (double *) calloc(sizeX,sizeof(double));
      if (!data[j]) goto READING_ERROR;
   }
   for (i=0; i<sizeY; i++)
   for (j=0; j<sizeX; j++)
   {  float f;
      if (fscanf(fd, "%f", &f)!=1) goto READING_ERROR;
      data[i][j] = (double)f;
   }
   computeRangeZ();
   return 1;
   
   READING_ERROR:
   data = NULL;
   sizeX = sizeY = 0;
   fprintf(stderr,"Error while reading function from file\n");
   return 0;
}

/* return the function value at a vertex (i,j) of the grid */
double DataFunction :: vertexZ(int i, int j)
{
  if (data) return pointZ(vertexX(i,j),vertexY(i,j));
  else return 0.0;
}

/* return the function value at a point (x,y) */
double DataFunction :: pointZ(double x, double y)
{
  /* interpolate by taking the average of the four neighbors of
     (x,y) in the data matrix */
  if (data)
  {
     int i,j;
     double val;
     int k;
     j = (int)( ((x-minX)/(maxX-minX))*(double)(sizeX-1) );
     i = (int)( ((y-minY)/(maxY-minY))*(double)(sizeY-1) );
     val = data[i][j]; k = 1;
     if (i<sizeY-1) { val += data[i+1][j]; k++;  }
     if (j<sizeX-1) { val += data[i][j+1]; k++;  }
     if ( (i<sizeY-1)&&(j<sizeX-1) ) {  val += data[i+1][j+1]; k++;  }
     return ( val / (double)k );
  }
  else return 0.0;
}

void DataFunction :: computeRangeZ(void)
{
  int i,j;
  float val;
  minZ = maxZ = 0.0;
  if (data)
  {  for (i=0; i<sizeY; i++)
     for (j=0; j<sizeX; j++)
     {
       val = data[i][j];
       if (val<minZ) minZ = val;
       else if (val>maxZ) maxZ = val;
      }
   }
}
