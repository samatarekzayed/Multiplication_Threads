#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int matrix1[1000][1000];
int matrix2[1000][1000];
int matrixout[1000][1000];
char file1[100];
char file2[100];
char file3[100];
char *txt=".txt";
int nrow1;
int nrow2;
int ncolumn1;
int ncolumn2;
struct row_col
{
    int row;
    int col;
};

void readFilea()
{
  //  printf("%s",file1);
  //   printf("%s",file2);
    FILE* ptr;
    char ch;
    char firstLine[14];
    ptr = fopen(file1, "r");
    if (NULL == ptr) {
        printf("file can't be opened \n");
    }

   while(1){
    fscanf(ptr,"%c",&ch);
    if (ch=='=')
    {
      break;
    }
   }
   fscanf(ptr,"%d",&nrow1);
    while(1){
    fscanf(ptr,"%c",&ch);
    if (ch=='=')
    {
      break;
    }
   }
   fscanf(ptr,"%d",&ncolumn1);
  //  printf("%d\n",ncolumn1);
  //   printf("%d\n",nrow1);

   //reading matrix
   for (int i=0;i<nrow1;i++)
   {
    for(int j=0;j<ncolumn1;j++)
    {
    fscanf(ptr,"%d",&matrix1[i][j]);
    }

   }
    // Closing the file
    fclose(ptr);
}
//////////////////////////////////////////////////////////////////////////////////
void readFileb()
{
  //  printf("%s",file1);
  //   printf("%s",file2);
    FILE* ptr;
    char ch;
    char firstLine[14];
    ptr = fopen(file2, "r");
    if (NULL == ptr) {
        printf("file can't be opened \n");
    }

   while(1){
    fscanf(ptr,"%c",&ch);
    if (ch=='=')
    {
      break;
    }
   }
   fscanf(ptr,"%d",&nrow2);
    while(1){
    fscanf(ptr,"%c",&ch);
    if (ch=='=')
    {
      break;
    }
   }
   fscanf(ptr,"%d",&ncolumn2);
    //  printf("%d\n",ncolumn2);
    // printf("%d\n",nrow2);
    // printf("%d",nrow2);
   //reading matrix
   for (int i=0;i<nrow2;i++)
   {
    for(int j=0;j<ncolumn2;j++)
    {
    fscanf(ptr,"%d",&matrix2[i][j]);
    }

   }
    // Closing the file
    fclose(ptr);

}
///////////////////////////////////////////////////////////////////////////////
void threadPerMatrix()
{
    for(int i=0;i<nrow1;i++)
    {
        for(int j=0;j<ncolumn2;j++)
        {
            matrixout[i][j]=0;
            for(int k=0;k<ncolumn1;k++){
                matrixout[i][j] += matrix1[i][k]*matrix2[k][j];
            }
        }
    }
////printing in the output file
       char tempfile[100];
      strcpy(tempfile,file3);
      strcat(tempfile,"_per_matrix.txt");
      FILE *out = fopen(tempfile,"w");

    fprintf(out,"Method: A thread per matrix\n");
    fprintf(out,"row=%d col=%d\n",nrow1,ncolumn2);
    for(int i=0;i<nrow1;i++){
        for(int j=0;j<ncolumn2;j++){
            fprintf(out,"%d ",matrixout[i][j]);
        }
        fprintf(out,"\n");
    }
}

///////////////////////////////////////////////////////////////////////////////////
void* threadPerRow(void* indexOfRow)
{
    // int index=(int)indexOfRow; 3lshan lma b cast mn int l pointer byb2o msh nfs el size 
    long long index=(long long)indexOfRow;
    for(int i=0;i<ncolumn2;i++){
        matrixout[index][i]=0;
        for(int j=0;j<nrow2;j++){
            matrixout[index][i] += matrix1[index][j]*matrix2[j][i];
        }
    }
    //Terminate  thread
    pthread_exit(NULL);
    
}
////////////////////////////////////////////////////////////////////////////////////
void threadSPerRow()
{

    int thread_id;
    pthread_t thread[nrow1];

    for(long i=0;i<nrow1;i++) {
        thread_id = pthread_create(&thread[i], NULL, threadPerRow,(void *) i);
        if (thread_id != 0) {//Error Happened.
            printf("something went wrong!\n");
            exit(-1);
        }
    }

    for(int i=0;i<nrow1;i++) {
        pthread_join(thread[i], NULL);//Waiting for each thread to terminate.
    }

    //////////printing in the output file
      char tempfile[100];
      strcpy(tempfile,file3);
      strcat(tempfile,"_per_row.txt");
      FILE *out = fopen(tempfile,"w");

    fprintf(out,"Method: A thread per row\n");
    fprintf(out,"row=%d col=%d\n",nrow1,ncolumn2);
    for(int i=0;i<nrow1;i++){
        for(int j=0;j<ncolumn2;j++){
            fprintf(out,"%d ",matrixout[i][j]);
        }
        fprintf(out,"\n");
    }
}
//////////////////////////////////////////////////////////////
void* threadPerElement(void* ind)
{
    struct row_col* index;
    index = (struct row_col* ) ind;
    int ro=index->row;
    int co=index->col;
    matrixout[ro][co]=0;
    for(int i=0;i<ncolumn1;i++){
       matrixout[ro][co] += matrix1[ro][i]*matrix2[i][co];
    }
  //Terminate thread.
  pthread_exit(NULL);
}
////////////////////////////////////////////////////////////////////////////////////
void threadSPerElement(){
 
 int zayed=0;
 int thread_id;
  pthread_t thread[nrow1*ncolumn2];

     for(int i=0;i<nrow1;i++) 
     {
        for (int j=0;j<ncolumn2; j++) 
        {
           struct row_col* sama=malloc(sizeof(struct row_col));
           (*sama).col=ncolumn2;
           (*sama).row=nrow1;
           thread_id = pthread_create(&thread[zayed], NULL,  *threadPerElement, (void*) sama);
           zayed++;
           
        if (thread_id != 0) {
            printf("Something went wrong!\n");
            exit(-1);
        }

        }
     }
    // waiting the threads to terminate 
    
     for(int i=0;i<zayed;i++) 
     { 
        pthread_join(thread[i], NULL);
     }

////////////printing in file
      char tempfile[100];
      strcpy(tempfile,file3);
      strcat(tempfile,"_per_element.txt");
      FILE *out = fopen(tempfile,"w");

    fprintf(out,"Method: A thread per element\n");
    fprintf(out,"row=%d col=%d\n",nrow1,ncolumn2);
    for(int i=0;i<nrow1;i++){
        for(int j=0;j<ncolumn2;j++){
            fprintf(out,"%d ",matrixout[i][j]);
        }
        fprintf(out,"\n");
    }
}
///////////////////////////////////////////////////////////////////////////////////
void running(){
  
 struct timeval stop, start;
  gettimeofday(&start, NULL);
   threadPerMatrix();
    gettimeofday(&stop, NULL); 
printf("\nFirst method : thread per matrix\n");
    for (int i=0;i<nrow1;i++)
   {
    for(int j=0;j<ncolumn2;j++)
    {
    printf("%d ",matrixout[i][j]);
    }
     printf("\n");
   }
    printf("Number of threads created: 1 Thread\n");
    printf("Seconds taken %lu\n", stop.tv_sec - start.tv_sec);
    printf("Microseconds taken: %lu\n", stop.tv_usec - start.tv_usec);
//--------------------------------------------------------------------------------//
   gettimeofday(&start, NULL);
   threadSPerRow();
   gettimeofday(&stop, NULL);
printf("\nSecond method : one thread per row\n");
    for (int i=0;i<nrow1;i++)
   {
    for(int j=0;j<ncolumn2;j++)
    {
    printf("%d ",matrixout[i][j]);
    }
     printf("\n");
   }
    printf("Number of threads created: %d Thread\n",nrow1);
    printf("Seconds taken %lu\n", stop.tv_sec - start.tv_sec);
    printf("Microseconds taken: %lu\n", stop.tv_usec - start.tv_usec);
//--------------------------------------------------------------------------------//  
   gettimeofday(&start, NULL);  
   threadSPerElement();
   gettimeofday(&stop, NULL);
   printf("\nThird method : one thread per element\n");
    for (int i=0;i<nrow1;i++)
   {
    for(int j=0;j<ncolumn2;j++)
    {
    printf("%d ",matrixout[i][j]);
    }
     printf("\n");
   }
    printf("Number of threads created: %d Thread\n",nrow1*ncolumn2);
    printf("Seconds taken %lu\n", stop.tv_sec - start.tv_sec);
    printf("Microseconds taken: %lu\n", stop.tv_usec - start.tv_usec);
}
/////////////////////////////////////////////////////////////////////////////
int main(int args,char *arg[]) {
  if(args<4)
  {
    strcpy(file1,"a");
    strcpy(file2,"b");
    strcpy(file3,"c");
  }
  else
  {
    strcpy(file1,arg[1]);
    strcpy(file2,arg[2]);
    strcpy(file3,arg[3]);
  }
  strcat(file1,txt);
  strcat(file2,txt);
//  printf("%s",file1);
//   printf("%s",file2);
//   strcat(file3,txt);
//  printf("%s",file1);
readFilea();
readFileb();
if(nrow2!=ncolumn1){//the matrices cannot be multiplied because dimensions are not valid
        printf("Invalid Dimensions.\n");
        exit(-1);
    }
running();

return 0;
}
///////////////////////////////////////////////////////////////////////////////