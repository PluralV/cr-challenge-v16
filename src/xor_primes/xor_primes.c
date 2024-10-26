#include "xor_primes.h"

void print_mob(int n){
    int *mob_2n = mobius_2_sieve(n);
    for (int i = 0; i <= n; i++){
        fprintf(stderr,"%c",(i==0)?'[':',');
        fprintf(stderr,"%d",mob_2n[i]);
    }
    fprintf(stderr,"]\n");
    free(mob_2n);
}

void print_elst(int *elist,int len){
    int i = 0;
    while (i < len){
        fprintf(stderr,"%c",(i==0)?'[':',');
        fprintf(stderr,"%d",elist[i]);
        i++;
    }
    fprintf(stderr,"]\n");
}

void insert(list_node *head, int data){
    list_node *node = malloc(sizeof(list_node));
    node->data = data;
    node->next = head->next;
    head->next = node;
}

void free_list_nodes(list_node *head){
    if (head == NULL) return;
    else{
        list_node *newhead = head->next;
        free(head);
        free(newhead);
    } 
}

int get(list_node *head, int index){
    while (index > 0){
        if ((head = head->next) == NULL) return -1;
        index--;
    }
    return head->data;
}

//length n array where all but the first x entries are 1 (first x are 0)
int *emplst(int n, int x){
    int *ret = malloc(n*sizeof(int));
    for (int i = 0; i < x; i++) ret[i] = 0;
    for (int i = x; i < n; i++) ret[i] = 1;
    return ret;
}

unsigned long xor_product(unsigned int xi, unsigned int yi){//works for any ints
    unsigned long x = (unsigned long) xi, y = (unsigned long)yi;
    unsigned long product = 0;
    while (y != 0){
        if (y & 1) product ^= x;
        x = x << 1;
        y = y >> 1;
    }
    return product;
}

list_node *divisors_of(int x){
    list_node *head = malloc(sizeof(list_node));
    head->data = x;
    head->next = NULL;
    int sq_rt = (int)sqrt((double)x);
    for (int i = 1; i <= sq_rt; i++){
        if (x % i == 0){
            insert(head,i);
            if (i ^ 1 && i ^ sq_rt)insert(head,x/i);
        } 
    }
    return head;
}


// c transposition of python code: def mobius_k_sieve(n, k = 2):
//     '''
//     I redefined the the Mobius function:
//                     1 if n is k-free positive integer with even number of prime factors
//         μ_{k}(n) = -1 if n is k-free positive integer with odd number of prime factors
//                     0 if n has a k power factor
//     '''
//     prime = [1]*(n + 1)
//     prime[0] = prime[1] = 0
//     mob = [0] + [1]*(n)
//     for p in range(2, n + 1):
//         if prime[p]:
//             mob[p] *= -1
//             for i in range(2*p, n + 1, p):
//                 prime[i] = 0
//                 mob[i] *= -1
//             sq = pow(p, k)
//             if sq <= n:
//                 for j in range(sq, n + 1, sq):
//                     mob[j] = 0
//     return mob

int *mobius_2_sieve(int n){
    int *prime = emplst(n+1, 2);
    int *mob = emplst(n+1,1);
    for (int i = 2; i < n+1; i++){
        if (*(prime+i)){
            *(mob+i) = *(mob+i) * -1;
            for (int j = 2*i; j < n+1; j += i){
                *(prime+j) = 0;
                *(mob+j) = *(mob+j) * -1;
            }
            int sq = i*i;
            if (sq <= n){
                for (int j = sq; j < n+1; j+=sq){
                    *(mob+j) = 0;
                }
            }
        }
    }
    free(prime);
    return mob;
}

/*transposition of python code: 
def numOfIrred(n):
    tot = 0
    mob = mobius_k_sieve(n)
    div = Divisors_of(n)
    div.add(n)
    for d in div:
        tot += mob[d]*pow(2, n//d)
    return tot//n
*/
long two_to_the(unsigned int n){
    return (long)1 << n;
}

long count_irreducible(unsigned int degree){
    long tot = 0;
    int *mob = mobius_2_sieve(degree);
    print_mob(degree);
    list_node *divs = divisors_of(degree);
    list_node *div_iter = divs;
    while (div_iter != NULL){
        fprintf(stderr,"TOT: %ld\n",tot);
        fprintf(stderr,"DIVISOR: %ld\n",div_iter->data);
        fprintf(stderr,"MOB[]: %ld\n",mob[div_iter->data]);
        fprintf(stderr,"POW2: %ld\n",two_to_the(degree/(div_iter->data)));
        long add = mob[div_iter->data] * two_to_the(degree/(div_iter->data));
        fprintf(stderr,"ADD: %ld\n",add);
        tot = tot+add;
        div_iter = div_iter->next;
    }
    free(mob);
    free_list_nodes(divs);
    return tot/degree;
}

/*
def sieve(lim):
    tot = 0
    d = 1
    while True:
        t = numOfIrred(d)
        if tot + t > lim:
            break
        tot += t
        d += 1
        
    N = pow(2, d + 1)
    result = [True]*N
    result[0] = result[1] = False
    for x in range(4, N, 2):
        result[x] = False
    
    count = 1
    for i in range(3, N, 2):
        if result[i]:
            count += 1
            if count == lim:
                return i
            
            j = i
            while True:
                x = xorProduct(j, i)
                if x >= N:
                    break
                result[x] = False
                j += 2
*/

long sieve(int n){
    if (n == 1) return 2;
    long tot = 0;
    int d = 1;
    while (1){
        long t = count_irreducible(d);
        if ((tot + t) > n) break;
        tot += t;
        d++;
    }
    unsigned int N = (unsigned int)two_to_the(d+1);
    int *results = emplst(N,2);
    for (int i = 4; i < N; i+=2) *(results+i) = 0;
    int count = 1;
    for (int i = 3; i < N; i+=2){
        if (*(results+i)){
            count++;
            if (count == n){
                free(results); 
                return i;
            }
            int j = i;
            while (1){
                long x = xor_product(j,i);
                if (x >= (long)N) break;
                *(results+x) = 0;
                j+=2;
            }
        }
    }
}
/*
while True:
    A = int(input("Input an integer: "))
    start_time = time.time()
    print(sieve(A))
    break
*/


int main(int argc, char **argv){
    print_mob(2);
    print_mob(3);
    int nth = 0;
    while (nth != -1){
        printf("Enter integer n: \n");
        if (scanf("%d",&nth) == 1){
                if (nth > 0) printf("XOR Prime #%d is: %ld\n",nth,sieve(nth));
        } else return -1;
    }
    return 0;
}

/*
def xorProduct(x, y):
    prod = 0
    #Note that 0 ^ x = x, ^ = XOR
    while y != 0:
        if y % 2 == 1:
            #If the first bit of y is a 1, prod = prod ^ x
            prod ^= x
        #Then we shift x one bit to the left, essentially adds a 0 behind x
        x <<= 1
        #And we push y one bit to the right, essentially removes the first bit
        y >>= 1
    return prod
*/