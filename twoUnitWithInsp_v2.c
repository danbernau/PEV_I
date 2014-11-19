// --- G. Rubino, Feb 2014
//      status:  working
//      quick and dirty (and no confidence intervals)

/* This is oneUnitWithInsp-v1.c; compile for instance with
   gcc  -o  oneUnitWithInsp-v1  ps.o  oneUnitWithInsp-v1.c  -lm
*/

#include  <stdio.h>
#include  <stdlib.h>
#include  "./ps.h"

// --- event classes
#define failure 1
#define repairFailure 2
#define failureFailure 3   
#define repair  4
#define inspection  5
char *cl[6] = {"","failure","repairFailure","failureAfterFailure","repair","inspection"};

// --- model states
#define up 1
#define downRepairing1 2
#define downRepairing2 3
#define downWaiting1 4
#define downWaiting2 5
char *st[6] = {"","up","downRepairing1","downRepairing2","downWaiting1","downWaiting2"};

#define initGen(s) srand48((double)s)
#define expo(mean) (-mean*log(drand48()))
#define detected (drand48() < d)

double drand48();
void srand48(long);
double log(double);

#define printBefore
#define printAfter

int main(int nbOfPars, char **pars)
{
   double mlt;  /* mean life-time */
   double mrt;  /* mean repair time */
   double d;   /* probability of failure detection */
   double Delta;   /* inspection period */
   double T;    /* total simulation time */
   long seed;   /* seed for the pseudo-r.n. */
 
   double totalDown1 = 0.0;       /* total down time for system 1*/
   double totalDown2 = 0.0;       /* total down time for system 2*/
   
   double time;    /* current sim time */
   int class;      /* current event class */
   
   int state;
   double t;       /* aux */
   double tPrec = 0.0;   /* last epoch */
   double tStartDown1;
   double tStartDown2;

   
   EVENT *ev;

   nbOfPars--;   /* after this, mbOfPars = the effective \# of pars */
   if (nbOfPars != 6 ||
      sscanf(pars[1],"%lf",&mlt) != 1 ||
      sscanf(pars[2],"%lf",&mrt) != 1 ||
      sscanf(pars[3],"%lf",&d) != 1 ||
      sscanf(pars[4],"%lf",&Delta) != 1 ||
      sscanf(pars[5],"%lf",&T) != 1 ||
      sscanf(pars[6],"%ld",&seed) != 1)
   {
      fprintf(stderr,"\nUsage: %s  mlt  mrt  d   Delta  T  seed\n\n",pars[0]);
      exit(-1);
   }

   initGen(seed);
   InitEvList(T);

  // TODO: Write Mail about initial lambda and second lambda
  // t = 2* expo(mlt);
  // t = expo(mlt/2);
  // t = expo(mlt)+ expo(mlt);

   t = expo(mlt)+ expo(mlt);

   CREATE_EV(ev,failure,t);
   InsertEv(ev);

   CREATE_EV(ev,inspection,Delta);
   InsertEv(ev);
                     printf("\nPrevious state: %s \n", st[state]);
   state = up;
                   printf("\nNew state: %s \n", st[state]);

   do
   {
      FirstEv(&class, &time);
      switch(class)
      {
         case failure:
                    printBefore;
                    if (state==up){
                    tStartDown1 = time;
                    if (detected)
                    {
                                        printf("\nPrevious state: %s \n", st[state]);
                    state = downRepairing1;
                                    printf("\nNew state: %s \n", st[state]);
                    t = time + expo(mrt);
                        CREATE_EV(ev,repair,t)
                        InsertEv(ev);
                    }
                    else
                    {
                                        printf("\nPrevious state: %s \n", st[state]);
                        state = downWaiting1;
                                        printf("\nNew state: %s \n", st[state]);
                        t = time + expo(mlt);
                        CREATE_EV(ev,failureFailure,t)
                        InsertEv(ev);
                    }
                    printAfter;
                    break;
                  }
                  
            case failureFailure:
                    if(state==downWaiting1)
                    {
                        tStartDown2 = time;
                       if (detected)
                      {
                                          printf("\nPrevious state: %s \n", st[state]);
                        state = downRepairing2;
                                        printf("\nNew state: %s \n", st[state]);
                        t = time + expo(mrt);
                        CREATE_EV(ev,repair,t)
                        InsertEv(ev);
                      }
                      else
                      {
                                          printf("\nPrevious state: %s \n", st[state]);
                        state = downWaiting2;
                                        printf("\nNew state: %s \n", st[state]);
                      }
                    }
                    printAfter;
                    break;

            case repairFailure:
                    if(state==downRepairing1)
                    {
                                        printf("\nPrevious state: %s \n", st[state]);
                      state = downRepairing2;
                                      printf("\nNew state: %s \n", st[state]);
                      tStartDown2 = time;
                      t = time + expo(mrt);
                      CREATE_EV(ev,repair,t)
                      InsertEv(ev);
                      printAfter;
                      break;
                    }

         case repair:
         			printBefore;         			

              if (state == downRepairing1)
              {
                totalDown1 += time - tStartDown1;
                t = time + (expo(mlt)+ expo(mlt));
                CREATE_EV(ev,failure,t);
                InsertEv(ev);
                printf("\nPrevious state: %s \n", st[state]);
                state = up;
                                printf("\nNew state: %s \n", st[state]);
                printAfter;
                break;
              }

              if (state == downRepairing2)
              {
                totalDown2 += time - tStartDown2;
                t = time + expo(mlt);
                CREATE_EV(ev,repairFailure,t);
                InsertEv(ev);
                // schedule next repair
                t = time + expo(mrt);
                CREATE_EV(ev,repair,t);
                InsertEv(ev);
                printf("\nPrevious state: %s \n", st[state]);
                state = downRepairing1;
                printf("\nNew state: %s \n", st[state]);
                printAfter;
                break;
              }

         case inspection:
         			printBefore;
         			if (state == downWaiting1)
         			{
         					t = time + expo(mrt);
         					CREATE_EV(ev,repair,t);
         					InsertEv(ev);
                  printf("\nPrevious state: %s", st[state]);
                  state = downRepairing1;
                                  printf("\nNew state: %s", st[state]);
              }

              if (state == downWaiting2)
              {
                  t = time + expo(mrt);
                  CREATE_EV(ev,repair,t);
                  InsertEv(ev);
                  printf("\nPrevious state: %s", st[state]);
                  state = downRepairing2;
                                  printf("\nNew state: %s", st[state]);
              }
              
              t = time + Delta;
              CREATE_EV(ev,inspection,t);
              InsertEv(ev);
              printAfter;
              //PrintEventList();
              break;
                    
         case END_SIM:
 				   break;	
      };
      tPrec = time;
   } while(class != END_SIM);
   printf("\n --- asymptotic unavailability = %lf\n\n",totalDown1/T);
}

