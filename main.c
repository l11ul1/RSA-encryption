// This is a starting program for Problem2
//Mun Vladislav
//Student ID: 991531576
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#define FLUSH stdin=freopen(NULL,"r",stdin)
#define RMN(str) str[strcspn(str,"\n")]=0

// Set the maximum number of choices for e and d!
#define MAX_SIZE 30
// Needed for fgets(): max number of characters fgets() can get.
#define MAX_PLAINTEXT 1000


char str_from_ascii[MAX_SIZE];
long modinv(long, long);
void encrypt (char*, char);
long gcd(long, long);
char is_prime(long);
long encrypt_decrypt(long, long, long);
int countDigits(long);

// You'd need to modify this function: add extra parameter long* for arr_d (!)
void factorize (long, long*, long* );


// You must implement functions below!

// This function needs to get rid of characters with ASCII value less than 32 (SPACE) or greater than 122 ('z')
// Also, character ` not allowed (used for SPACES!)
void changePlainText(char*);


// This function  must perform the shift by 23: 'A' becomes 65-23= 42, 'z' (ASCII 122) becomes 99
// After the shift SPACE will have ASCII value 32-23=9. We need 2 digits! 
// Thus, replace "SPACE" with value '73' (that is reserved to `  (ASCII 96-21 = 73)
// Finally, this function must create long number based on shifted ASCII values: "ABz" => 424399 long
long str_to_ascii (char*);

// This function must do the opposite: convert 424399 to "ABz". 
// Don't forget to shift back by 23 characters and take care of SPACE (replace 73 with value 9 before shift).
char* ascii_to_str (long);


int main() {
    
   
   int MAX_PRIME=2;
   int MIN_PRIME=2;
   int flag;
   int p, q;
   
    // Public  constants (for encryption)
   long e[MAX_SIZE];
   long d[MAX_SIZE];
   char str[MAX_SIZE];
   long modulus=0;  // Modules is p * q;
   long totient=0;  // Totient is (p-1)*(q-1)
   
   int i=1;
   
   printf("INFO23431: Assignment N1\n");
   
   
   // MIN Prime Number
   do {
        FLUSH;
        printf("Please enter MINIMUM Prime Number (2-%d): ", INT_MAX);
   }
   while (!scanf("%d", &MIN_PRIME) || (MIN_PRIME < 2) );  
   
   // MAX Prime
   do {
       FLUSH;
        printf("Please enter MAXIMUM Prime Number (%d-%d): ", MIN_PRIME+1, INT_MAX);
   }
   while (!scanf("%d", &MAX_PRIME) || (MAX_PRIME <= MIN_PRIME) );  
   
   // Generate the List of Prime Numbers:
   printf("\nList of Prime numbers:\n");
   int count = 0;
   for (i=MIN_PRIME; i<= MAX_PRIME; i++) {
       if (is_prime(i)) {
           printf("%d ", i);
           count++;
       }
   }
   printf("\n"); // end list
   
   
   // Do we have enough prime numbers? If only one is printed, it's not enough!
   if (count <2) {
       printf("\nYou need at least two prime numbers!\n");
       exit (0);
   }
   
   
   // Get Prime Numbers p != q:
    do {
        
        FLUSH;
        printf("\nPlease input two different prime numbers %d <= p, q <= %d: ", MIN_PRIME, MAX_PRIME);
        scanf("%d %d", &p, &q );

        if ( (p<MIN_PRIME) || (p>MAX_PRIME) || (q<MIN_PRIME) || (q> MAX_PRIME) || (p==q) || !is_prime(p) || !is_prime(q) ) {
            printf("Incorrect input!\n"); 
        }
        else {
                modulus = p*q;
                totient = (p-1)*(q-1); // must be more than 99!
         
                // Can we even factorize the totient?? Example: p=3 and q=5 can't be factorized!
                // You need to check here!!! 
                factorize (totient, e, d);
                
                // Do we have at least one non-zero e[0] -> check here!
   
        }
               
   } while (totient <1 && totient < 99);
   
                   
   printf("Prime numbers: p=%d, q=%d\n", p, q);
   printf("Modulus p*q         = %ld\n", modulus);
   printf("Totient (p-1)*(q-1) = %ld\n", totient);
   
   
   i=0;

   printf("\nPossible choices for e (public) constant:\n");
   printf("0. Exit the program\n");

   while( (i<MAX_SIZE) && (e[i]>0)  ) {
       printf("%d. %ld %ld\n", (i+1), e[i], d[i]);
       ++i;
   }

   
   int choice=0;
   printf("Please make a choice (0-%d): ", i );
   FLUSH;
   scanf("%d", &choice);
   
   if (choice <1 || choice > i) {
       printf("Exiting the program");
       exit(0);
   }
   choice--; // because choice==1 is index[0]
   
   printf("Your choice: e=%ld d=%ld\n", e[choice], d[choice]);
   printf("Public  key: (%ld, %ld)\n", e[choice], modulus);
   printf("Private key: (%ld, %ld)\n", d[choice], modulus);
   
      

   printf("\n---------- Part 1: Working with numbers: ----------\n");
   
   long plaintext_long;

   do {
	FLUSH;
        printf("Please input Numerical Plaintext less than modulus (1- %ld): ", modulus-1);
   } while ( !scanf("%ld", &plaintext_long ) || (plaintext_long<0) || (plaintext_long > modulus-1));

   long ciphertext_long =encrypt_decrypt(plaintext_long, e[choice], modulus);
   long decrypted_long =encrypt_decrypt(ciphertext_long, d[choice], modulus);
   
   printf("Plaintext:  %ld\n", plaintext_long);
   printf("Ciphertext: %ld\n", ciphertext_long);
   
   // Here you need to add correct decryption
   printf("Decrypted:  %ld\n", decrypted_long);
 
   printf("\n---------- Part 2: Encrypting a string: ----------\n");
   printf("Based on modulus(%ld), you can encrypt up to 3 characters!\n", modulus);
   FLUSH;
    do {
        
        flag=0;
        printf("\nPlease input String Plain text(up to %d chars): ", 3);
        fgets(str, sizeof(str), stdin);
        RMN(str);
        if ((strlen(str)>3) || (strlen(str)<1)) {
            printf("Incorrect Input! Please try again!\n");
            flag=1;
        }
    }while (flag);
    changePlainText(str);
    long ascii_val = str_to_ascii(str);
    long ascii_cipher = encrypt_decrypt(ascii_val, e[choice], modulus);

    printf("\nNumerical Plaintext:  %ld", ascii_val);
    printf("\nNumerical Ciphertext: %ld", ascii_cipher);

    
   printf("\n---------- Part 3: Decrypting a string: ----------\n");
   long dec_numerical_ciphertext;
    do {
	FLUSH;
        printf("Deciphering Numerical Ciphertext : ");
    } while ( !scanf("%ld", &dec_numerical_ciphertext ) || (dec_numerical_ciphertext<0) || (dec_numerical_ciphertext > ascii_cipher));
   long ascii_decipher = encrypt_decrypt(dec_numerical_ciphertext, d[choice], modulus);
   printf("\nNumerical Plaintext:  %ld", ascii_decipher);
   strcpy(str_from_ascii, ascii_to_str(ascii_decipher));
   printf("\nNumerical Plaintext:  %s", str_from_ascii);
   
   return 0;
}

// Here arr_e is the array of private constants
// You must also add  arr_d is the array of public constants
void factorize (long tot, long* arr_e, long* arr_d) {
    
    long e=1;
    int inx=0; // Used for arr_e and arr_d
    
    while (++e < tot) { 
        // e must be co-prime to tot and e<tot.
        // Note: e doesn't have to be prime itself!
        
        if (gcd(e, tot)==1) {
             arr_e[inx]=e;
             arr_d[inx]=modinv(e, tot)%tot;
             inx++;
             if (inx>= MAX_SIZE) {
                e=tot+1; // to break from outer while
             }
              
	} // end if gcd(e, tot) == 1;
    } // end while
    
}
  
// -------------------------
char is_prime(long pr) {
 
    long i, j=(long)sqrt(pr);

    for (i=2;i<=j;i++) {

            if(pr%i==0){ 
                return 0;
            }
    }

    return 1;
}

// -------------------------
long gcd(long a, long b) { 
    long temp; 
    while (1)    { 
        temp = a%b; 
        if (temp == 0) 
          return b; 
        a = b; 
        b = temp; 
    } 
} 

long modinv(long u, long v){
    // refernce one of the hints https://www.di-mgt.com.au/euclidean.html
    unsigned int inv, u1, u3, v1, v3, t1, t3, q;
    int iter;
    /* Step X1. Initialise */
    u1 = 1;
    u3 = u;
    v1 = 0;
    v3 = v;
    /* Remember odd/even iterations */
    iter = 1;
    /* Step X2. Loop while v3 != 0 */
    while (v3 != 0)
    {
        /* Step X3. Divide and "Subtract" */
        q = u3 / v3;
        t3 = u3 % v3;
        t1 = u1 + q * v1;
        /* Swap */
        u1 = v1; v1 = t1; u3 = v3; v3 = t3;
        iter = -iter;
    }
    /* Make sure u3 = gcd(u,v) == 1 */
    if (u3 != 1)
        return 0;   /* Error: No inverse exists */
    /* Ensure a positive result */
    if (iter < 0)
        inv = v - u1;
    else
        inv = u1;
    return inv;
}

/* Iterative Function to calculate (msg^exp)% n in O(log exp) */
// See: https://www.geeksforgeeks.org/modular-exponentiation-power-in-modular-arithmetic/
long encrypt_decrypt(long msg, long exp, long n) { 
    long res = 1;      // Initialize result 
  
    msg = msg % n;  // Update msg if it is more than or equal to n 
  
    while (exp > 0)  { 
        // If exp is odd, multiply msg with result 
        if (exp & 1) {
            res = (res*msg) % n; 
        }
  
        // exp must be even now 
        exp = exp >>1; // exp = exp/2 
        msg = (msg*msg) % n;   
    } 
    return res; 
}

// Count digits in a number
//https://math.stackexchange.com/questions/231742/proof-how-many-digits-does-a-number-have-lfloor-log-10-n-rfloor-1
int countDigits(long num) {
 return (num>0) ? (int) log10 ((double)num) + 1 : 1;
}

void changePlainText(char* str){
    for(int i = 0; i < strlen(str); i++){
        if((long)str[i]<32){
            str[i]=' ';
        }
        if((long)str[i]>122){
            str[i]=' ';
        }
        if(str[i] == '`'){
            str[i] = ' ';
        }
    }
}

long str_to_ascii (char *str){
    long ascii = 0;
    long *ptr = &ascii;
    int mul = 10000;
    for(int i = 0; i<strlen(str); i++){
        long cur_char = (long)str[i];
        if(str[i] == ' '){
             cur_char= 96;
        }
        *ptr = ascii + mul * ( cur_char - 23 );
        mul = mul/100;
    }
    return ascii;
}

char* ascii_to_str (long dec_msg){ 
    int devisor = 10000, reminder, quotient;
    long msg = dec_msg;
    char *str_from_ascii = malloc(3);
    for(int i = 0; i < 3; i++){
        quotient = msg / devisor;
        reminder = msg % devisor;
        if(quotient + 23 == 96){
            quotient = 9;
        }
        str_from_ascii[i] = quotient + 23;
        msg = reminder;
        devisor /= 100;
    }
    return str_from_ascii; 
}