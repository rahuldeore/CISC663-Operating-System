/* 
 * Test Program #3 - Semaphore
 */

#include <stdio.h>
#include <stdlib.h>
#include "ud_thread.h"

sem_t *s;
int resource = 0;

void read_function(int val) 
{
   long i, j ;
   int res_before, res_after;

   for (i = 0; i < 5; i++) {
      printf("I am READ thread %d (%d)\n", val, i);

      sem_wait(s);
            
      res_before = resource;
      printf("  [%d READ %d] resource = %d\n", val, i, resource);

      t_yield();
      printf("  ** [%d READ %d] returns from t_yield()\n", val, i);
      res_after = resource;

      if (res_before != res_after) {

         fprintf(stderr, "[THREAD %d] error in the semaphore program \n", val);
         exit(-1);
      }

      sem_signal(s);
   }

   t_terminate () ;
}

void write_function(int val) 
{
   long i, j ;

   for (i = 0; i < 5; i++) {
      printf("I am WRITE thread %d (%d)\n", val, i);
	
      sem_wait(s);
            
      resource = rand() % 100;      
      printf("  [%d WRITE %d] resource = %d\n", val, i, resource);

      sem_signal(s);

      t_yield();
   }

   t_terminate();
}

int main(void) {

   int i;

   t_init();
   sem_init(&s, 1);

   t_create(write_function, 1, 1);
   t_create(write_function, 2, 1);
   t_create(read_function, 11, 1);
   t_create(read_function, 22, 1);
   t_create(write_function, 3, 1);
   t_create(write_function, 4, 1);
   t_create(read_function, 33, 1);
   t_create(read_function, 44, 1);
  
   for (i = 0; i < 60; i++) {
      printf("I am main thread (%d)...\n", i);
      t_yield();
   }

   sem_destroy(&s);
   t_shutdown();

   return 0;
}
/*
I am main thread (0)...
I am WRITE thread 1 (0)
  [1 WRITE 0] resource = 38
I am WRITE thread 2 (0)
  [2 WRITE 0] resource = 58
I am READ thread 11 (0)
  [11 READ 0] resource = 58
I am READ thread 22 (0)
I am WRITE thread 3 (0)
I am WRITE thread 4 (0)
I am READ thread 33 (0)
I am READ thread 44 (0)
I am main thread (1)...
I am WRITE thread 1 (1)
I am WRITE thread 2 (1)
  ** [11 READ 0] returns from t_yield()
I am main thread (2)...
  [22 READ 0] resource = 58
I am READ thread 11 (1)
I am main thread (3)...
  ** [22 READ 0] returns from t_yield()
I am main thread (4)...
  [3 WRITE 0] resource = 13
I am READ thread 22 (1)
I am main thread (5)...
  [4 WRITE 0] resource = 15
I am main thread (6)...
  [33 READ 0] resource = 15
I am WRITE thread 3 (1)
I am main thread (7)...
  ** [33 READ 0] returns from t_yield()
I am WRITE thread 4 (1)
I am main thread (8)...
  [44 READ 0] resource = 15
I am READ thread 33 (1)
I am main thread (9)...
  ** [44 READ 0] returns from t_yield()
I am main thread (10)...
  [1 WRITE 1] resource = 51
I am READ thread 44 (1)
I am main thread (11)...
  [2 WRITE 1] resource = 27
I am main thread (12)...
  [11 READ 1] resource = 27
I am WRITE thread 1 (2)
I am main thread (13)...
  ** [11 READ 1] returns from t_yield()
I am WRITE thread 2 (2)
I am main thread (14)...
  [22 READ 1] resource = 27
I am READ thread 11 (2)
I am main thread (15)...
  ** [22 READ 1] returns from t_yield()
I am main thread (16)...
  [3 WRITE 1] resource = 10
I am READ thread 22 (2)
I am main thread (17)...
  [4 WRITE 1] resource = 19
I am main thread (18)...
  [33 READ 1] resource = 19
I am WRITE thread 3 (2)
I am main thread (19)...
  ** [33 READ 1] returns from t_yield()
I am WRITE thread 4 (2)
I am main thread (20)...
  [44 READ 1] resource = 19
I am READ thread 33 (2)
I am main thread (21)...
  ** [44 READ 1] returns from t_yield()
I am main thread (22)...
  [1 WRITE 2] resource = 12
I am READ thread 44 (2)
I am main thread (23)...
  [2 WRITE 2] resource = 86
I am main thread (24)...
  [11 READ 2] resource = 86
I am WRITE thread 1 (3)
I am main thread (25)...
  ** [11 READ 2] returns from t_yield()
I am WRITE thread 2 (3)
I am main thread (26)...
  [22 READ 2] resource = 86
I am READ thread 11 (3)
I am main thread (27)...
  ** [22 READ 2] returns from t_yield()
I am main thread (28)...
  [3 WRITE 2] resource = 49
I am READ thread 22 (3)
I am main thread (29)...
  [4 WRITE 2] resource = 67
I am main thread (30)...
  [33 READ 2] resource = 67
I am WRITE thread 3 (3)
I am main thread (31)...
  ** [33 READ 2] returns from t_yield()
I am WRITE thread 4 (3)
I am main thread (32)...
  [44 READ 2] resource = 67
I am READ thread 33 (3)
I am main thread (33)...
  ** [44 READ 2] returns from t_yield()
I am main thread (34)...
  [1 WRITE 3] resource = 84
I am READ thread 44 (3)
I am main thread (35)...
  [2 WRITE 3] resource = 60
I am main thread (36)...
  [11 READ 3] resource = 60
I am WRITE thread 1 (4)
I am main thread (37)...
  ** [11 READ 3] returns from t_yield()
I am WRITE thread 2 (4)
I am main thread (38)...
  [22 READ 3] resource = 60
I am READ thread 11 (4)
I am main thread (39)...
  ** [22 READ 3] returns from t_yield()
I am main thread (40)...
  [3 WRITE 3] resource = 25
I am READ thread 22 (4)
I am main thread (41)...
  [4 WRITE 3] resource = 43
I am main thread (42)...
  [33 READ 3] resource = 43
I am WRITE thread 3 (4)
I am main thread (43)...
  ** [33 READ 3] returns from t_yield()
I am WRITE thread 4 (4)
I am main thread (44)...
  [44 READ 3] resource = 43
I am READ thread 33 (4)
I am main thread (45)...
  ** [44 READ 3] returns from t_yield()
I am main thread (46)...
  [1 WRITE 4] resource = 89
I am READ thread 44 (4)
I am main thread (47)...
  [2 WRITE 4] resource = 83
I am main thread (48)...
  [11 READ 4] resource = 83
I am main thread (49)...
  ** [11 READ 4] returns from t_yield()
I am main thread (50)...
  [22 READ 4] resource = 83
I am main thread (51)...
  ** [22 READ 4] returns from t_yield()
I am main thread (52)...
  [3 WRITE 4] resource = 37
I am main thread (53)...
  [4 WRITE 4] resource = 66
I am main thread (54)...
  [33 READ 4] resource = 66
I am main thread (55)...
  ** [33 READ 4] returns from t_yield()
I am main thread (56)...
  [44 READ 4] resource = 66
I am main thread (57)...
  ** [44 READ 4] returns from t_yield()
I am main thread (58)...
I am main thread (59)...
*/
