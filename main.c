///
//  main.c
//  lab1
//
//  Created by Liaolily on 10/9/14.
//  Copyright (c) 2014 Liaolily. All rights reserved.
//

#include <stdio.h>

#include <math.h>


void to_binary  ( int  n, int w, int *x, int *o ) ;
void from_binary( int *x, int w, int *n ) ;
void adder( int *x, int *y, int *z, int *o, int w ) ;
void mult ( int *x, int *y, int *z, int *o, int w ) ;
void print_binary(int *x, int w);

void main()
{
    int a,b,c,d,w,n,o ;
    int x[32]={0}, y[32]={0}, z[32]={0} ;
    
    o = 0 ;
    w = 9 ; /* word size in bits */
    /* note that x, y, and z must be at least as large as w and may be larger */
    
    b = 1 ; /* add one to each */
    for( c=250;c<260;c++ )
    {
        if ( c > 255 )  /* exceeds 8 bits */
            d = c-512 ;
        else
            d =  c ;
        printf( "%d %d ", d,b ) ;
        to_binary( d,w,x,&o ) ;
        //print_binary(x, w);
        if ( o )
            printf( "%d Overflow in to_binary! ",d ) ;
        to_binary( b,w,y,&o ) ;
        if ( o )
            printf( "%d Overflow in to_binary! ",b ) ;
        adder( x,y,z,&o,w ) ;
        from_binary( z,w,&a ) ;
        printf( "%d ", a ) ;
        if ( o )
            printf( "Overflow in adder! " ) ;
        printf( "\n" ) ;
    }
    
    b = -1 ; /* add -1 to each */
    for( c=250;c<260;c++ )
    {
        if ( c > 255 )
            d = c-512 ;
        else
            d =  c ;
        printf( "%d %d ", d,b ) ;
        to_binary( d,w,x,&o ) ;
        if ( o )
            printf( "%d Overflow in to_binary! ",d ) ;
        to_binary( b,w,y,&o ) ;
        if ( o )
            printf( "%d Overflow in to_binary! ",d ) ;
        adder( x,y,z,&o,w ) ;
        from_binary( z,w,&a ) ;
        printf( "%d ", a ) ;
        if ( o )
            printf( "Overflow in adder! " ) ;
        printf( "\n" ) ;
    }
    
    
    for( w=8; w<13; w=w+4 )          /* word sizes 8 and 12 bits */
        for( a=99; a >-100; a=a-99 )  /* 99, 0, -99 */
            for( b=4; b>-5; b=b-4 )    /* 4, 0, -4 */
            {
                to_binary( a,w,x,&o ) ;
                if ( o )
                    printf( "%d Overflow in to_binary! ",a ) ;
                to_binary( b,w,y,&o ) ;
                if ( o )
                    printf( "%d Overflow in to_binary! ",b ) ;
                mult( x,y,z,&o,w ) ;
                printf( "%d %d ", a,b ) ;
                from_binary( z,w,&n ) ;
                printf( "%d ", n ) ;
                if ( o )
                    printf( "Overflow in multiplier! " ) ;
                printf( "\n" ) ;
            }
}

void adder( int *x, int *y, int *z, int *o, int w )
{
    /* x is an input array of w ints, either 0 or 1 representing the first addend */
    /* y is an input array of w ints, either 0 or 1 representing the second addend */
    /* z is an output array of w ints, either 0 or 1 representing the sum of x and y */
    /* o is an output = 1 if an overflow occurred */
    /* w is an input = to the width in bits of x, y, z */
    
    int s = 0;
    int c = 0;
    int i;
    
    // for each bit
    for(i=0;i<w;i++)
    {
        // add bits
        s = !(x[i] ^ (!(y[i] ^ c)));
        z[i] = s;
        
        // carry?
        c =  (y[i] | c) & (x[i] | c)  & (x[i] | y[i]);
        
    }
    
    *o = c;
}
void mult ( int *x, int *y, int *z, int *o, int w )
{
    /* x is an input array of w ints, either 0 or 1 representing the first multiplicand */
    /* y is an input array of w ints, either 0 or 1 representing the second multicand */
    /* z is an output array of w ints, either 0 or 1 representing the product of x and y */
    /* o is an output = 1 if an overflow occurred */
    /* w is an input = to the width in bits of x, y, z */
    
    int c = 0;
    int xx[32]={0};
    int i,j,k;
    int negx=x[w-1];
    int negy=y[w-1];
    int yy[32]={0};
    int ovl = 0;
    
    // make this a 1
    yy[0]=1;
    
    
    
    // clear z
    for(i=0;i<w;i++)
        z[i] = 0;
    
    // for each bit
    for(i=0;i<w;i++)
    {
        if(y[i] == 1)
        {
            for(j=w-i-1, k=w-1;j>=0;j--,k--)
                xx[k] = x[j];
            // clear rest of xx
            while(k>=0)
                xx[k--] = 0;
            
            adder( xx, z, z, o, w );
            ovl = ovl | *o;
        }
        
    }
    
    *o = ovl;
    
}


void to_binary( int n, int w, int *x, int *o )
{
    /* n is an input and is the number to be converted to binary */
    /* w is an input = to the width in bits of x */
    /* x is an output array of w ints, either 0 or 1: the binary representation of n */
    /*           x[0] is the least significant bit   */
    /*           x[w-1] is the most signifcant bit, for a signed number, it is the sign */
    /* o is an output = 1 if an overflow occurred ( n is too large for w bits ) */
    
    int i;
    int neg = 0;
    
    int yy[32]={0};
    
    // make this a 1
    yy[0]=1;
    
    if(n < 0)
    {
        neg=1;
        n = -n;
    }
    
    for(i=0;i<w;i++)
    {
        x[i] = n % 2;
        n = n >> 1;
    }
    
    if(n > 0)
        *o = 1;
    else
        *o = 0;
    
    // if neg take two's complement
    if(neg)
    {
        // invert bits
        for(i=0;i<w;i++)
        {
            if(x[i] == 0) x[i] = 1;
            else x[i] = 0;
        }
        
        // add 1
        adder(x,yy,x,o,w);
    }
    
}
void from_binary( int *x, int w, int *n )
{
    /* x is an input array of w ints, either 0 or 1: the binary representation of n */
    /*           x[0] is the least significant bit   */
    /*           x[w-1] is the most signifcant bit, for a signed number, it is the sign */
    /* w is an input = to the width in bits of x */
    /* n is an output and will be equal to the value stored in binary in x */
    int m = 1;
    int i = 0;
    int yy[32]={0};
    int z[32]={0};
    int o;
    
    yy[0]=1;
    
    // negative
    if(x[w-1]==1)
    {
        // invert bits
        for(i=0;i<w;i++)
        {
            if(x[i] == 0) z[i] = 1;
            else z[i] = 0;
        }
        
        // add 1
        adder(z,yy,z,&o,w);
        
        // calculate
        *n = 0;
        
        for(i=0;i<w;i++)
        {
            *n = *n + z[i] * m;
            m = m << 1;
        }
        
        *n = -*n;
    }
    
    // positive
    else
    {
        *n = 0;
        
        for(i=0;i<w;i++)
        {
            *n = * n + x[i] * m;
            m = m << 1;
        }
        
    }
    
}


void print_binary(int *x, int w)
{
    int i;
    
    for(i=w-1;i>=0;i--)
    {
        printf("%d",x[i]);
    }
    
    printf("\n");
    
}

