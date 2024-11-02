typedef struct noeud{
    char *cle;
    char* hex; // en hexa
    struct noeud *suivant;
    struct noeud *precedent;
} noeud;

typedef noeud* dico;

dico creation_dico();
dico initialisation_dico(dico d);
dico insertion_dico(dico d, char *caractere, int i);
bool recherche(dico d, const char *str);
bool recherche_hex(dico d, const char *hex);
void free_dico(dico d);
dico compressLZW(dico d, FILE *stream, FILE *output);
char* valueOfKey(dico d, const char *key);
char* keyOfValue(dico d, const char *value);
dico decompressLZW(dico d, FILE *stream, FILE *output);
dico newCompressLZW(dico d, FILE *stream, FILE *output);