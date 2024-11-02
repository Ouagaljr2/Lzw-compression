#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <math.h>

#include "new.h"

unsigned long hex2dec(char* hex) {
    return strtoul(hex, NULL, 16);
}

char *dec2hex(unsigned decimal, char* hex, unsigned size) {
    char *tmp = (char *) malloc((size) * sizeof(char));

    if (hex == NULL) {
        // size + 1 to take into account the '\0' string code
        hex= (char *) malloc((size + 1) * sizeof(char));
        hex[size] = '\0';
    }
    
    long quotient, remainder;
    int hex_size = 0;
    quotient = decimal;
 
    while (quotient != 0) {
        remainder = quotient % 16;
        if (remainder < 10) {
            tmp[hex_size] = 48 + remainder;
        }
        else {
            tmp[hex_size] = 55 + remainder;
        }
        hex_size++;
        quotient = quotient / 16;
    }

    // nb of zero to output
    int zero_left = size - hex_size;

    // write hex value to the end
    while(hex_size >= 0) {
        hex_size--;
        hex[size - hex_size - 1] = tmp[hex_size];
        
    }

    // complete hex with 0
    while (zero_left > 0) {
        hex[zero_left - 1] = '0';
        zero_left--;
    }

    free(tmp);

    return hex;
}

/**
 * Read next bytes of file stream as a short (most of the time 2 bytes on modern system)
 * and write it in a file
 */
char* rb_next_short_as_hex(FILE *fp) {
    unsigned short input;
    if(fread(&input, sizeof(unsigned short), 1, fp) == 0) {
        return NULL;
    }
    else {
        return dec2hex(input, NULL, sizeof(unsigned short));
    }
}

/**
 *  Convert hex string to short (most of the time 2 bytes on modern system)
 *  and write it in a file 
 */
bool wb_hex_as_short(FILE *fp, char* hex) {
    unsigned short bin = (unsigned short) hex2dec(hex);
    // true if fwrite wrote one elt, false otherwise
    return fwrite(&bin, sizeof(unsigned short), 1, fp) == 1;
}

/**
 *  Convert hex string to int (most of the time 4 bytes on modern system)
 *  and write it in a file 
 */
bool wb_hex_as_int(FILE *fp, char* hex) {
    unsigned int bin = (unsigned int) hex2dec(hex);
    // true if fwrite wrote one elt, false otherwise
    return fwrite(&bin, sizeof(unsigned int), 1, fp) == 1;
}

/**
 * Read next bytes of file stream as a int (most of the time 4 bytes on modern system)
 * and write it in a file
 */
char* rb_next_int_as_hex(FILE *fp) {
    unsigned int input;
    if(fread(&input, sizeof(unsigned short), 1, fp) == 0) {
        return NULL;
    }
    else {
        
        return dec2hex(input, NULL, sizeof(int));
    }
}

/**
 *  Convert hex string to long (most of the time 8 bytes on modern system)
 *  and write it in a file 
 */
bool wb_hex_as_long(FILE *fp, char* hex) {
    unsigned long bin = (unsigned long) hex2dec(hex);
    // true if fwrite wrote one elt, false otherwise
    return fwrite(&bin, sizeof(unsigned long), 1, fp) == 1;
}

/**
 * Read next bytes of file stream as a long (most of the time 8 bytes on modern system)
 * and write it in a file
 */
char* rb_next_long_as_hex(FILE *fp) {
    unsigned int input;
    if(fread(&input, sizeof(unsigned short), 1, fp) == 0) {
        return NULL;
    }
    else {
        return dec2hex(input, NULL, sizeof(unsigned long));
    }
}

char* concat(const char *s1, const char *s2) {
    char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
    // in production-ready code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

char* char2str(const char c) {
    char *str = malloc(2 * sizeof(char));
    str[0] = c;
    str[1] = '\0';

    return str;
}

// "bonjour bobo" => 

bool useShort = false;
bool useInt = false;
bool useLong = false;

int main(int argc, char *argv[]) {

    char *s = dec2hex( 77, NULL, sizeof(long) );
    printf("%s\n", s);

    if(argc != 4) {
        printf("Usage: %s <filename> <output> <decompressFile>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    FILE *f = NULL;
    FILE *out = NULL;
    FILE *in = NULL;

    f = fopen(argv[1], "r");
    out = fopen(argv[2], "wb+");
    in = fopen(argv[3], "w");

    if( f == NULL ){
        printf("Error opening\n");
        exit(EXIT_FAILURE);    
    }

    if( out == NULL ){
        printf("Error opening\n");
        exit(EXIT_FAILURE);    
    }

    if( in == NULL ){
        printf("Error opening\n");
        exit(EXIT_FAILURE);    
    }

    struct stat statbuf;

    if ( stat( argv[1], &statbuf ) == 1){
        perror( "" );
        exit( EXIT_FAILURE );
    }

    int size = statbuf.st_size;
    long unsigned st = 0x7F + size;

    

    if ( ( st ) < ( pow( 0xF, 1 ) * (2 - 1) )  * ( 0xF + 1 ) ){
        printf("Use short\n");
        useShort = true;
    }

    else if ( ( st ) < ( pow( 0xF, 3 ) * (4 - 1) )  * ( 0xF + 1 ) ){
        printf("Use int\n");
        useInt = true;
    }

    else if ( ( st ) < ( pow( 0xF, 7 ) * (8 - 1) )  * ( 0xF + 1 ) ){
        printf("Use long\n");
        useLong = true;
    }

    else{
        printf("Impossible to compress the input file\n");
        exit( EXIT_FAILURE );
    }

    dico d = creation_dico();
    printf("Created\n");
    d = compressLZW(d, f, out);

    // dico i = d; 

    // for( ; i != NULL; i = i->suivant)
    //     printf("%s   %s\n", i->cle, i->hex);

    free_dico(d);

    rewind( out );
    
    d = creation_dico();

    d = decompressLZW( d, out, in );

    
    free_dico(d);

    if( fclose( f ) == EOF ){
        perror("");
        exit( EXIT_FAILURE );
    }

    if( fclose( out ) ){
        perror("");
        exit( EXIT_FAILURE );
    }

    if( fclose( in ) ){
        perror("");
        exit( EXIT_FAILURE );
    }

    // FILE *f = fopen("output.lzw", "r");

    // decompressLZW( NULL, f, NULL );

    // if( fclose( f ) == EOF ){
    //     perror("");
    //     exit( EXIT_FAILURE );
    // }
  
    // utilisation de short pour optimissation de mla place memoir
    if(useShort)
        printf("short\n");

    else if (useInt)
        printf("int\n");

    else if (useLong)
        printf("long");
    

    return 0;

}


dico creation_dico(){
    return NULL;
}

void free_dico(dico d){

    if(d != NULL){

        if(d->suivant)
            free_dico(d->suivant);

    
        free( d->cle );
        free( d->hex );
        free( d );
    }

    return;
}

/// init the fucking dico rhooooh ;(

dico initialisation_dico(dico d) {

    char *str = NULL;

    for (int i = 0; i < 128; i++) {

        str = char2str( i );
    
        d = insertion_dico(d, str, i+1);

        free(str);
    }

    
    return d;

}

/// insert the concatenation or the single char in the string table :)

// done

dico insertion_dico( dico d, char *caractere, int i ){
    
    dico tmp = (dico) malloc( sizeof( noeud ) );

    tmp->cle = (char *) malloc( ( strlen( caractere ) + 1) * sizeof( char ) ) ;

    strcpy( tmp->cle, caractere );

    tmp->cle[strlen( caractere )] = '\0';


    if(useShort)
        tmp->hex = dec2hex( i, NULL, sizeof( short ) );
    
    if( useInt )
        tmp->hex = dec2hex( i, NULL, sizeof( int ) );

    if( useLong )
        tmp->hex = dec2hex( i, NULL, sizeof( long ) );

    

    if( d == NULL ){

        tmp->suivant = NULL;

        return tmp;

    }

    tmp->suivant = d;

    return tmp;

}


bool recherche(dico d, const char *str){
    noeud *i = d;

    for(  ; i != NULL; i = i->suivant)
        if( ! ( strcmp ( i->cle, str ) ) )
            return true;


    return false;
}

bool recherche_hex(dico d, const char *hex){
    noeud *i = d;

    for(  ; i != NULL; i = i->suivant)
        if( ! ( strcmp ( i->hex, hex ) ) )
            return true;


    return false;
}


///  i have to optimize this shit for more performance

// indice decomposition de fonction 
// the starting indice point apres  voir mis tous les caractres ascii

dico newCompressLZW(dico d, FILE *stream, FILE *output){
    printf("compressing");
    int i = 95; 
    d = initialisation_dico( d );


}


dico compressLZW(dico d, FILE *stream, FILE *output) {
    printf("compressing\n");
    int i = 129; 

    char P = fgetc( stream );

    char C;

    char *strP = NULL;
    char *strP2 = NULL;
    char *strC = NULL;
    char *concatPC = NULL;

    char *hex = NULL;

    d = initialisation_dico( d );


    while (C != EOF){
        
        C = fgetc( stream );


        if(strP != NULL && strC != NULL){
            free( strP ); free( strC );
        }

        // string P
        strP = char2str( P );

        // string C
        strC = char2str( C );


        if( concatPC != NULL)
            free(concatPC);
        
        // string P+C
        if( strP2 != NULL )
            concatPC = concat( strP2, strC );

        else
            concatPC = concat( strP, strC );




        if( recherche( d, concatPC ) ) {
            if(strP2 != NULL)
                strP2 = concat( strP2, strC );
            else
                strP2 = concat( strP, strC );
            
        }
        else {

            
            
            d = insertion_dico( d, concatPC, i);

            if( useShort ){

                hex = dec2hex( i, NULL, sizeof( unsigned short ) );

                if( strP2 != NULL ){
                    
                    wb_hex_as_short( output, valueOfKey( d, strP2 ) );
                    free( strP2 );
                    strP2 = NULL;
                }else{
                    wb_hex_as_short( output, valueOfKey( d, strP ) );
                }

                free( hex );

                

            }
            else if ( useInt ){

                hex = dec2hex( i, NULL, sizeof( unsigned int ) );

                if( strP2 != NULL ){
                    wb_hex_as_short( output, valueOfKey( d, strP2 ) );
                    free( strP2 );
                    strP2 = NULL;
                }else{
                    wb_hex_as_short( output, valueOfKey( d, strP ) );
                }

                free( hex );

                

            }
            else if ( useLong ){
                
                hex = dec2hex( i, NULL, sizeof( unsigned long ) );

                

                if( strP2 != NULL ){
                    printf("%s %s\n", strP2, concatPC );
                    wb_hex_as_short( output, valueOfKey( d, strP2 ) );
                    free( strP2 );
                    strP2 = NULL;
                }else{
                    printf("\n");
                    wb_hex_as_short( output, valueOfKey( d, strP ) );
                }

                free( hex );

                

            }
            
            

            

            P = C;
            i++;
        }
    }
    
        free( concatPC );

    
        free( strP );

    
        free( strC );

    return d;

}


// dico newCompressLZW(dico d, FILE *stream, FILE *output) {
//     printf("compressing");

//     d = initialisation_dico( d );

//     int i = 95; 

//     char *P = NULL;
//     char *P_tmp = NULL;
//     fgets( P, 2, stream);

//     char C;
//     char *strC = NULL;

//     // do {

//     //     // C = fgetc( stream );
//     //     if( recherche( d, concat( P, strC = char2str( C ) ) ) ){
            
//     //         P = concat( P, strC );
//     //         free( strC );

//     //     }else{
            
//     //         printf(" %s\n ", P);
//     //         d = insertion_dico( d, concat( P, strC ), i );
//     //         i++;
            
//     //         P = strC; 
            
//     //     }


//     // } while( C != EOF );
    

//     return d;

// }


dico decompressLZW( dico d, FILE *stream, FILE *output ) {
    printf(" on est dans la decompressLZW\n");
    d = initialisation_dico( d );
    int i = 129;

    (void) stream;
    (void) output;

    char *old = NULL;
    char *new = NULL;
    char *s = NULL;
    char c;

    if( useShort ){
        printf("debut\n");
        old = rb_next_short_as_hex( stream );
        printf("fin%s\n", old);
        fputs( keyOfValue( d, old ), output );
        new = rb_next_short_as_hex( stream );
    }

    else if( useInt ){
        old = rb_next_int_as_hex( stream );
        fputs( keyOfValue( d, old ), output );
        new = rb_next_int_as_hex( stream );
    }
    
    else if ( useLong ){
        printf("%s\n", old);
        old = rb_next_long_as_hex( stream );
        
        fputs( keyOfValue( d, old ), output );
        new = rb_next_long_as_hex( stream );
    }

    do {
        
        
        
        if( ! recherche_hex( d, new ) ){
            s = keyOfValue( d, old );

            s = concat( s,  char2str(c) );
        }else {

            s = keyOfValue( d, new );

        }

        fputs( s, output );
        c = s[0];

        d = insertion_dico(d, concat( keyOfValue( d, old ), char2str( c ) ) , i);
        i++;

        
        old = new;
        if( useShort ){
            new = rb_next_short_as_hex( stream );
        }

        else if( useInt ){
            new = rb_next_int_as_hex( stream );
        }
        
        else if ( useLong ){
            new = rb_next_long_as_hex( stream );
        }

    } while(new != NULL);
    

    return d;

}

char* keyOfValue(dico d, const char *value) {

    dico i = d;

    for( ; i != NULL; i = i->suivant){


        if( strcmp(value, i->hex) == 0 ){
            return i->cle;
        }

    }

    i = d; 

    for( ; i != NULL; i = i->suivant)
        printf("%s   %s\n", i->cle, i->hex);

    printf("P : %s pas dans le dico\n", value); 
    exit(EXIT_FAILURE);

}

char* valueOfKey(dico d, const char *key) {
    for(dico i = d; i != NULL; i = i->suivant){
        
        if( strcmp( key, i->cle ) ){
        }else
            return i->hex;

    }
    
    for(dico i = d; i != NULL; i = i->suivant){
        
        printf("clé : %s valeur : %s\n", i->cle, i->hex);

    }

    printf("P :%s pas dans le dico %ld\n", key, strlen( key )); 
    exit(EXIT_FAILURE);

}