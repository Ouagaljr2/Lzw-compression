typedef struct Noeud {

    struct Noeud *papa;
    struct Noeud **fils;
    char *c;
    bool isWord;
    char *value;

} strTrie, *Trie;


Trie createTrie( );
Trie insertAux( Trie t, Trie papa, char *c, char *s, int i, bool last, int j, int hex );
Trie insertTrie( Trie t, char *s, int hex );
void freeListe( Trie t );
char * valueOfKey(Trie tmp,char* kef);
Trie descendre(Trie t,char i);
Trie init(Trie t);

void newCompressLZW(Trie tr, FILE *stream, FILE *output, char *filename);


