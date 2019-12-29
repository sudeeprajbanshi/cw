#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <crypt.h>
#include <time.h>
#include <stdio.h>
#include <mpi.h>
#include <unistd.h>
/***********************************************************************
*******
  Demonstrates how to crack an encrypted password using a simple
  "brute force" algorithm. Works on passwords that consist only of 2
uppercase
  letters and a 2 digit integer. Your personalised data set is included
in the
  code. 

  Compile with:
    mpicc -o pw_crack pw_crack.c -lcrypt

  If you want to analyse the results then use the redirection operator
to send
  output to a file that you can view using an editor or the less
utility:

   mpirun -n 3 ./pw_crack > pw_crack.txt

  Dr Kevan Buckley, University of Wolverhampton, 2018
************************************************************************
******/
int n_passwords = 4;

char *encrypted_passwords[] = {
"$6$KB$yPvAP5BWyF7oqOITVloN9mCAVdel.65miUZrEel72LcJy2KQDuYE6xccHS2ycoxqXDzW.lvbtDU5HuZ733K0X0",

"$6$KB$4dWyDaT7p8xMWfb6.1R4Q7hQMyezwOgLfAcZhdE4QkH3bylH4EmM.gOVhU7m7K9FHfL.kKhzK6bfHqfA.NDb/0",

"$6$KB$xELPxo7gwBciT/5FN/.kBwgSPG5RZ8TiW8N0hEER1gyYrQ9ErvSfTbGuDl4w9/QyIP5Lu6W25.G.C8jk6VTYL0",

"$6$KB$YsfzwjTmZTAEUfD/L/uEqVAmfKGAWSa0ZWg8tSPGT4DZnQNBzLT4vXg2nl7Yd2KZrs4dykykxVIZZLob5ijUb/"
};

/**
 Required by lack of standard function in C.   
*/

void substr(char *dest, char *src, int start, int length){
  memcpy(dest, src + start, length);
  *(dest + length) = '\0';
}

/**
 This function can crack the kind of password explained above. All
combinations
 that are tried are displayed and when the password is found, #, is put
at the 
 start of the line. Note that one of the most time consuming operations
that 
 it performs is the output of intermediate results, so performance
experiments 
 for this kind of program should not include this. i.e. comment out the
printfs.
*/

void function_1(char *salt_and_encrypted){
  int x, y, z;     // Loop counters
  char salt[7];    // String used in hashing the password. Need space

  char plain[7];   // The combination of letters currently being checked
  char *enc;       // Pointer to the encrypted password
  int count = 0;   // The number of combinations explored so far

  substr(salt, salt_and_encrypted, 0, 6);

  for(x='A'; x<='M'; x++){
    for(y='A'; y<='Z'; y++){
      for(z=0; z<=99; z++){
 printf("Instance 1");
        sprintf(plain, "%c%c%02d", x, y, z); 
        enc = (char *) crypt(plain, salt);
        count++;
        if(strcmp(salt_and_encrypted, enc) == 0){
          printf("#%-8d%s %s\n", count, plain, enc);
        } /*else {
          printf(" %-8d%s %s\n", count, plain, enc);
        }*/
      }
    }
  }
  printf("%d solutions explored\n", count);
}

void function_2(char *salt_and_encrypted){
  int a, b, c;     // Loop counters
  char salt[7];    // String used in hashing the password. Need space

  char plain[7];   // The combination of letters currently being checked
  char *enc;       // Pointer to the encrypted password
  int count = 0;   // The number of combinations explored so far

  substr(salt, salt_and_encrypted, 0, 6);

  for(a='N'; a<='Z'; a++){
    for(b='A'; b<='Z'; b++){
      for(c=0; c<=99; c++){
        printf("Instance 2");
        sprintf(plain, "%c%c%02d", a, b, c); 
        enc = (char *) crypt(plain, salt);
        count++;
        if(strcmp(salt_and_encrypted, enc) == 0){
          printf("#%-8d%s %s\n", count, plain, enc);
        } /*else {
          printf(" %-8d%s %s\n", count, plain, enc);
        }*/
      }
    }
  }
  printf("%d solutions explored\n", count);
}

int time_difference(struct timespec *start, struct timespec *finish,
                    long long int *difference) {
  long long int ds =  finish->tv_sec - start->tv_sec; 
  long long int dn =  finish->tv_nsec - start->tv_nsec; 

  if(dn < 0 ) {
    ds--;
    dn += 1000000000; 
  } 
  *difference = ds * 1000000000 + dn;
  return !(*difference > 0);
}


int main(int argc, char *argv[]){

  struct timespec start, finish;
  long long int difference;   
  int account = 0;
  clock_gettime(CLOCK_MONOTONIC, &start);

 

int size, rank;

  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if(size != 3) {
    if(rank == 0) {
      printf("This program needs to run on exactly 3 processes\n");
    }
  } else {
    if(rank ==0){
      int x;
      int y;
      int i;
	MPI_Send(&x, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);  
      MPI_Send(&y, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);
	
    } else {
      if(rank == 1){
	int i;
        int number = rank + 10;
      	MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
	for ( i = 0; i<n_passwords;i<i++){
		function_1(encrypted_passwords[i]);
	}
      }
	else if(rank == 2){
	int i;
      	int number = rank + 10;
      	MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
	for ( i = 0; i<n_passwords;i<i++){
		function_2(encrypted_passwords[i]);
	}
	}
    }
  }
  MPI_Finalize(); 

  clock_gettime(CLOCK_MONOTONIC, &finish);
  time_difference(&start, &finish, &difference);

  printf("Elapsed Time: %9.5lfs\n", difference/1000000000.0);
 

  return 0;
}

