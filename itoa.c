#include <string.h>


void reverse(char *s)
{
   int c, i, j;

   for (i = 0, j = strlen(s) - 1; i < j; i++, j--) {
      c = s[i];
      s[i] = s[j];
      s[j] = c;
   }
}

char *itoa(int n, char *s)

{
   int i, sign;

   if ((sign = n) < 0)   /* tiene traccia del segno */
      n = -n;   /* rende n positivo */
   i = 0;
   do {   /* genera le cifre nell'ordine inverso */
      s[i++] = n % 10 + '0';   /* estrae la cifra seguente */
   } while ((n /= 10) > 0);   /* elimina la cifra da n */
   if (sign < 0)
      s[i++] = '-';
   s[i] = '\0';
   reverse(s);

   return s;
}


