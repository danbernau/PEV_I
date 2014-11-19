// --- G. Rubino, oct. 2014

/* PICOSIMULATOR, simplified version --- ps.c */
/* gcc -c -o ps.o ps.c */


#include  <stdio.h>
#include  <stdlib.h>
#include  "ps.h"

/* --- this will point to the first event in the list */
EVENT *First;

/* --- event insertion */
void InsertEv(EVENT *ev)
{
   EVENT *x, *y;
   double time;

   time = ev->Time;
   if (time < First->Time)
   {
      /* --- ev will become the first one */
      ev->Next = First;
      First = ev;
      return;
   }

   /* here, ev goes at least at 2nd position */
   x = First;
   y = x->Next;
   while (y != NULL)
   {
      if (time < y->Time)
      {  /* --- ev goes after x end before y */
         ev->Next = y;
         x->Next = ev;
         return;
      }
      x = y;
      y = y->Next;
   }
   /* ev goes after the whole list */
   x->Next = ev;
   ev->Next = NULL;
}


/* --- event destruction */
void DeleteEv(EVENT *ev)
{
   EVENT *x, *y;

   x = First;

   if (ev == First)
   {
      First = First->Next;
      free(x);
      return;
   }

   /* ev is not the first element */
   y = x->Next;
   while (y != NULL)
   {
      if (y == ev)
      {
         x->Next = y->Next;
         free(y);
         return;
      }
      x = y;
      y = y->Next;
   }
}

/* accessing the first event */
void FirstEv(int *c, double *t)
{
   EVENT *x;

   x = First;
   First = First->Next;
   *t = x->Time;
   *c = x->Class;
   free(x);
}

/* initializing the event-list */
void InitEvList(double t)
{
   EVENT *ev;

   CREATE_EV(ev, END_SIM, t);
   First = ev;
}

/* print event list */
void PrintEventList()
{
	EVENT *x;
	int n = 0;
	
	printf(" --- Event list:\n");
	x = First;
	while(x != NULL)
	{
		n++;
		printf(" --- event %d: class %d, time %lf\n",n,x->Class,x->Time);
		x = x->Next;
	}
	if (n == 0) printf(" --- event list empty.\n");
	free(x);
}
