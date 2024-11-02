#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <math.h>

#include "trie.h"
#include "hex.h"
#include "utils.h"


int useShort_liste= 0;
int useInt_liste= 0;
int useLong_liste= 0;


Trie createTrie( ) {

    return NULL;

}

Trie init(Trie t){

    t = (Trie ) malloc(sizeof(strTrie));

    t->fils=calloc( 256, sizeof( strTrie));

    for (int i = 0; i < 256; i++){

        t->fils[i]=malloc(sizeof(strTrie));
        t->fils[i]->c=char2str(i);

        t->fils[i]->value=dec2hex(i,NULL,sizeof(short));

    }

    return t;
}



Trie insrtAux( Trie t, Trie papa, char *c, char *s, int i, bool last, int j, int hex ) {
    char* val=dec2hex( hex, NULL, sizeof( short ) );
    
    if( !i ) {
        Trie tr = ( Trie ) malloc( sizeof( strTrie ));
        tr->papa = papa;
        tr->c = s; 
        tr->fils = calloc( 256, sizeof( strTrie ) );
        if( last ) 
            tr->value = val;
        else 
            tr->value = "0";

        tr->isWord = last;
            
        return tr;

    }

    

    return insertAux( (t->fils[(int) s[ j+1 ]  ]), t, c, s, i-1, last, j+1, hex );


}

void freeListe( Trie t ) {

    free( t->fils );

    free( t ); 

    return;

}

Trie insertTrie( Trie t, char *s, int hex ) {
    
    if( t == NULL ){
        t = malloc( sizeof(strTrie));
        t->fils = calloc( 256, sizeof( strTrie ) );
        t->papa = NULL;
    }

    Trie tr = t;
    Trie tr_tmp = t;
    Trie tr_t = t;
    char *str = "";

    for ( int i = 0; i < (int) strlen( s ); i++ ){
        int b = (i == (int)(strlen(s) - 1));

        str = concat( str, char2str( s[i] ) );
        
        tr = insertAux( t, t->papa, str, str, i+1, b, -1, hex );    

        for ( int j = 0; j < i; j++ ){

            t = t->fils[(int) s[j]];

        }

        t->fils[(int) s[i]] = tr;

        for ( int j = 0; j < i; j++ ){

            t = t->papa;

        }
        
    }
    
    return t;
}


bool rechercheRec(Trie t, char *caract, int i ){
    if (caract[i]=='\0') return true;
    
    if(t==NULL) {
        return false;
    }
    else if( i == (int) (strlen(caract) - 1)){
        return t->isWord;
    }
    
    i++;
    return rechercheRec(t->fils[(int) caract[i]],caract,i);


}

bool recherche( Trie t, char * caractere){
    
    return rechercheRec(t,caractere,0);
}



Trie descendre(Trie t,char i){
    Trie tmp;
    tmp=t->fils[i];
    return tmp;

}

char * valueOfKey(Trie tr,char* key){

    int keylen=strlen(key);
    Trie tmp=tr;

    for (int i = 0; i < keylen; i++){
        tmp=descendre(tmp,key[i]);

    }
    return tmp->value;
   
}


void newCompressLZW(Trie tr, FILE *stream, FILE *output, char *filename) {

    taille_dico( &useShort_liste, &useInt_liste, &useLong_liste, filename );

    tr = createTrie();
    tr=init(tr);

    char strP[2048];
    char strC[2048];

    fgets( strP, 2, stream );

    int i = 257;


    while( fgets( strC, 2, stream ) != NULL) {

        if( recherche(tr, concat( strP, strC ) ) ){

            strcpy( strP, concat( strP, strC ) );

        }else{

            if( useShort_liste ){
                    

                char *hex = dec2hex( i, NULL, sizeof( unsigned short ) );


                wb_hex_as_short( output, valueOfKey( tr, strP ) );


                free( hex );

            }
            else if ( useInt_liste ){


                char *hex = dec2hex( i, NULL, sizeof( unsigned int ) );


                wb_hex_as_int( output, valueOfKey( tr, strP ) );


                free( hex );

            }
            else if ( useLong_liste ){


                char *hex = dec2hex( i, NULL, sizeof( unsigned long ) );


                wb_hex_as_long( output, valueOfKey( tr, strP ) );


                free( hex );
            }



            tr = insertTrie( tr, concat( strP, strC ), i );

            strcpy( strP, strC );
            i++;

        } 

    } 

    wb_hex_as_long( output, valueOfKey( tr, strP ) );

}

    
