#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

//Vedere se e' meglio controllare prima i 128 byte alla fine e poi i TAG all'inizio
//redirigere stderr con freopen()

#define SIZE               5000
#define NON_PRINTABLE(c)   ((c >= 0 && c <= 31) || (c == 127))            
#define NOT_ALLOWED(c)     ( c == '\\' || c == '/' || c == '*' || \
                             c == '?'  || c == '<' || c == '>' || \
                             c == ':'  || c == '"' || c == '|' )
#define TAG_ARTISTA(str)   ( strcmp(str, "TP1")  == 0  || strcmp(str, "TP2")  == 0  || \
                             strcmp(str, "TP3")  == 0  || strcmp(str, "TP4")  == 0  || \
                             strcmp(str, "TPE1") == 0  || strcmp(str, "TPE2") == 0  || \
                             strcmp(str, "TPE3") == 0  || strcmp(str, "TPE4") == 0 )
#define TAG_TITOLO(str)    ( strcmp(str, "TT1")  == 0  || strcmp(str, "TT2")  == 0  || \
                             strcmp(str, "TT3")  == 0  || strcmp(str, "TIT1") == 0  || \
                             strcmp(str, "TIT2") == 0  || strcmp(str, "TIT3") == 0 )
#define TAG_ALBUM(str)     ( strcmp(str, "TALB") == 0  || strcmp(str, "TAL")  == 0 )
#define NON_VALIDO(str)    ( strstr(str, "www.") != NULL && \
                            (strstr(str, ".com") != NULL || strstr(str, ".it") != NULL || \
                             strstr(str, ".am")  != NULL) )

char * TAG[] = { "AENC", "APIC", "COMM", "COMR", "ENCR", "EQUA", "ETCO", "GEOB",
                 "GRID", "IPLS", "LINK", "MCDI", "MLLT", "OWNE", "PRIV", "PCNT",
                 "POPM", "POSS", "RBUF", "RVAD", "RVRB", "SYLT", "SYTC", "TALB",
                 "TBPM", "TCOM", "TCON", "TCOP", "TDAT", "TDLY", "TENC", "TEXT",
                 "TFLT", "TIME", "TIT1", "TIT2", "TIT3", "TKEY", "TLAN", "TLEN",
                 "TMED", "TOAL", "TOFN", "TOLY", "TOPE", "TORY", "TOWN", "TPE1",
                 "TPE2", "TPE3", "TPE4", "TPOS", "TPUB", "TRCK", "TRDA", "TRSN",
                 "TRSO", "TSIZ", "TSRC", "TSSE", "TYER", "TXXX", "UFID", "USER",
                 "USLT", "WCOM", "WCOP", "WOAF", "WOAR", "WOAS", "WORS", "WPAY",
                 "WPUB", "WXXX", "ASPI", "EQU2", "RVA2", "SEEK", "SIGN", "TDEN",
                 "TDOR", "TDRC", "TDRL", "TDTG", "TIPL", "TMCL", "TMOO", "TPRO",
                 "TSOA", "TSOP", "TSOT", "TSST", "TT1",  "TT2",  "TT3",  "TP1",
                 "TP2",  "TP3",  "TP4",  "TAL",  "TCO",  "TRK",  "TYE",  "TCP",
                 "PIC" };

char * TAG_Artista_Titolo_Album [] = { "TP1", "TP2", "TP3", "TP4",  "TPE1", "TPE2", "TPE3", "TPE4",
                                       "TT1", "TT2", "TT3", "TIT1", "TIT2", "TIT3",
                                       "TAL", "TALB" }; 

int  prog = 0;                    
char artista[128];
char titolo[128];
char album[128];
char mp3_dir[SIZE];
char err_log_file[SIZE];

int  my_strlen(const char *);
void get_tag_fine_file(char *, char *, FILE *, int, int);
void parse_mp3(const char *, int, int);
void get_tag_lettera_per_lettera(const char *, int, char *);
void fix_name(char *, int);
void final_string(char *, int, int, time_t);
void copy_file(const char *, const char *, const char *, int *, int *, FILE *);
void get_tagID3(const char *,  FILE *, int *, int *, int);

int main(int argc, char *argv[])
{
    FILE * log_file;
    char str[SIZE];
    int  nFileConvertiti = 0, Errori = 0, mode;
    time_t TempoTrascorso;
    
    //Creo cartella MP3 e il file di log
    GetCurrentDirectory(SIZE, str);
    strcat(str, "\\MP3\\");
    strcpy(mp3_dir, str);
    if(CreateDirectory(mp3_dir, NULL) == 0){
        MessageBox(NULL, TEXT("Impossibile creare cartella MP3. Se già esiste rinominarla."), TEXT("Errore"), MB_ICONWARNING | MB_OK);
        return EXIT_FAILURE;
    }
    strcat(str, "\\Log.txt");
    if((log_file = fopen(str, "w")) == NULL){
        MessageBox(NULL, TEXT("Impossibile creare file di log."), TEXT("Errore"), MB_ICONWARNING | MB_OK);
        return EXIT_FAILURE;
    }
    GetCurrentDirectory(SIZE, err_log_file);
    strcat(err_log_file, "\\MP3\\Errori.txt");
    //Redirezione dello stderr su file
    if(freopen(err_log_file, "w", stderr) == NULL){
        MessageBox(NULL, TEXT("Errore nella redirezione dello stderr."), TEXT("Errore"), MB_ICONWARNING | MB_OK);
        return EXIT_FAILURE;
    }
    
    mode = MessageBox(NULL,
                      TEXT("Organizzare i titoli in cartelle in base ad artista ed album?"),
                      TEXT("Messaggio"),
                      MB_ICONINFORMATION | MB_YESNOCANCEL | MB_DEFBUTTON2);
    if(mode == IDYES)
       mode = 1;
    else if(mode == IDNO)
       mode = 0;
    else if(mode == IDCANCEL)
       goto exit;
    
    GetCurrentDirectory(SIZE, str);
    TempoTrascorso = time(NULL);
    get_tagID3(str, log_file, &nFileConvertiti, &Errori, mode);
    TempoTrascorso = time(NULL)-TempoTrascorso;
    
    ZeroMemory(str, SIZE);
    final_string(str, nFileConvertiti, Errori, TempoTrascorso);
    MessageBox(NULL, TEXT(str), TEXT("Messaggio"), MB_ICONINFORMATION | MB_OK);
    fprintf(log_file, "%s", str);
    fflush(log_file);
    
    exit:
    fclose(log_file);
    fclose(stderr);
    if(Errori == 0) DeleteFile(err_log_file);
    return EXIT_SUCCESS;
}

void
final_string(char * string,
             int n_file,
             int err,
             time_t secs)
{
    char temp[SIZE], temp2[100];
    if(secs < 60)
        sprintf(string, "Sono stati rinominati %d file in 00:%02d.\n", n_file, secs);
    else
        sprintf(string, "Sono stati rinominati %d file in %02d:%02d.\n", n_file, secs/60, secs%60);
    sprintf(temp, "I file sono stati memorizzati in %s.\n"
                  "Controllali poiché potrebbero esserci stati errori nella traduzione.\n", mp3_dir);
    if(err > 0){
        sprintf(temp2, "Ci sono stati %d errori nella copia.\n", err);
        strcat(temp, temp2);
    }
    strcat(string, temp);
}

void
get_tagID3 (const char * path,
            FILE * log,
            int * TotFileConvertiti,
            int * Errs,
            int extended)
{
    WIN32_FIND_DATA FindFileData;
    HANDLE hFind;
    FILE * file;
    char temp[SIZE], temp2[SIZE];
    char * buffer;
    int FileSize;
    
    strcpy(temp2, path);
    strcat(temp2, "\\*.mp3");
    if ((hFind = FindFirstFile(temp2, &FindFileData)) != INVALID_HANDLE_VALUE)
    {
        do
        {
            /*if(strcmp(FindFileData.cFileName, ".") == 0 || strcmp(FindFileData.cFileName, "..") == 0)
               continue;*/
            if((file = fopen(FindFileData.cFileName, "r+b")) == NULL){
                fprintf(stderr, "Errore apertura %s\n", FindFileData.cFileName);
                fflush(stderr);
                sprintf(temp, "Errore apertura %s", FindFileData.cFileName);
                MessageBox(NULL, TEXT(temp), TEXT("Errore"), MB_ICONWARNING | MB_OK);
            }
            else
            {
                fseek(file, 0, SEEK_END);
                FileSize = ftell(file)/3;
                rewind(file);
                
                buffer = (char *) calloc (FileSize, 1);
                ZeroMemory(temp, SIZE);
                ZeroMemory(artista, 128);
                ZeroMemory(titolo, 128);
                ZeroMemory(album, 128);
                fread(buffer, 1, FileSize, file);
                
                parse_mp3(buffer, FileSize, extended);

                if(strlen(artista) == 0)
                    get_tag_fine_file(buffer, artista, file, FileSize, 33);
                if(strlen(titolo) == 0)
                    get_tag_fine_file(buffer, titolo, file, FileSize, 3);
                if(strlen(album) == 0 && extended == 1)
                    get_tag_fine_file(buffer, album, file, FileSize, 63);
                    
                ZeroMemory(buffer, FileSize);

                if(strlen(artista) == 0 && strlen(album) == 0 && strlen(titolo) == 0)//000
                {
                    sprintf(buffer, "%s%s", mp3_dir, FindFileData.cFileName);
                    copy_file(FindFileData.cFileName, FindFileData.cFileName, buffer, TotFileConvertiti, Errs, log);
                }
                else if(strlen(artista) > 0 && strlen(album) > 0 && strlen(titolo) > 0)//111
                {
                     sprintf(temp, "%s - %s.mp3", artista, titolo);
                     if(extended)
                     {
                         sprintf(buffer, "%s%s\\", mp3_dir, artista);
                         CreateDirectory(buffer, NULL);
                         sprintf(buffer, "%s%s\\%s\\", mp3_dir, artista, album);
                         CreateDirectory(buffer, NULL);
                         sprintf(buffer, "%s%s\\%s\\%s", mp3_dir, artista, album, temp);
                     }
                     else
                         sprintf(buffer, "%s%s - %s.mp3", mp3_dir, artista, titolo);
                     copy_file(FindFileData.cFileName, temp, buffer, TotFileConvertiti, Errs, log);
                }
                else if(strlen(artista) == 0 && strlen(album) == 0 && strlen(titolo) > 0)//001
                {
                     sprintf(temp, "%s.mp3", titolo);
                     if(extended)
                         sprintf(buffer, "%s%s", mp3_dir, temp);
                     else
                         sprintf(buffer, "%s%s.mp3", mp3_dir, titolo);
                     copy_file(FindFileData.cFileName, temp, buffer, TotFileConvertiti, Errs, log);
                }
                else if(strlen(artista) == 0 && strlen(album) > 0 && strlen(titolo) == 0)//010
                {
                     if(extended)
                     {
                         sprintf(buffer, "%sArtistaSconosciuto\\", mp3_dir);
                         CreateDirectory(buffer, NULL);
                         sprintf(buffer, "%sArtista Sconosciuto\\%s\\", mp3_dir, album);
                         CreateDirectory(buffer, NULL);
                         strcat(buffer, FindFileData.cFileName);
                     }
                     else
                         sprintf(buffer, "%s%s", mp3_dir, FindFileData.cFileName);
                     copy_file(FindFileData.cFileName, FindFileData.cFileName, buffer, TotFileConvertiti, Errs, log);
                }
                else if(strlen(artista) == 0 && strlen(album) > 0 && strlen(titolo) > 0)//011
                {
                     sprintf(temp, "%s.mp3", titolo);
                     if(extended)
                     {
                         sprintf(buffer, "%sArtista Sconosciuto\\", mp3_dir);
                         CreateDirectory(buffer, NULL);
                         sprintf(buffer, "%sArtista Sconosciuto\\%s\\", mp3_dir, album);
                         CreateDirectory(buffer, NULL);
                         strcat(buffer, temp);
                     }
                     else
                         sprintf(buffer, "%s%s.mp3", mp3_dir, titolo);
                     copy_file(FindFileData.cFileName, temp, buffer, TotFileConvertiti, Errs, log);
                }
                else if(strlen(artista) > 0 && strlen(album) == 0 && strlen(titolo) == 0)//100
                {
                     if(extended)
                     {
                         sprintf(buffer, "%s%s\\", mp3_dir, artista);
                         CreateDirectory(buffer, NULL);
                         sprintf(buffer, "%s%s\\Album Sconosciuto\\", mp3_dir, artista);
                         CreateDirectory(buffer, NULL);
                         strcat(buffer, FindFileData.cFileName);
                     }
                     else
                         sprintf(buffer, "%s%s.mp3", mp3_dir, artista);
                     copy_file(FindFileData.cFileName, FindFileData.cFileName, buffer, TotFileConvertiti, Errs, log);
                }
                else if(strlen(artista) > 0 && strlen(album) == 0 && strlen(titolo) > 0)//101
                {
                     sprintf(temp, "%s - %s.mp3", artista, titolo);
                     if(extended)
                     {
                         sprintf(buffer, "%s%s\\", mp3_dir, artista);
                         CreateDirectory(buffer, NULL);
                         sprintf(buffer, "%s%s\\Album Sconosciuto\\", mp3_dir, artista);
                         CreateDirectory(buffer, NULL);
                         strcat(buffer, temp);
                     }
                     else
                         sprintf(buffer, "%s%s - %s.mp3", mp3_dir, artista, titolo);
                     copy_file(FindFileData.cFileName, temp, buffer, TotFileConvertiti, Errs, log);
                }
                else if(strlen(artista) > 0 && strlen(album) > 0 && strlen(titolo) == 0)//110
                {
                     if(extended)
                     {
                         sprintf(buffer, "%s%s\\", mp3_dir, artista);
                         CreateDirectory(buffer, NULL);
                         sprintf(buffer, "%s%s\\%s\\", mp3_dir, artista, album);
                         CreateDirectory(buffer, NULL);
                         strcat(buffer, FindFileData.cFileName);
                     }
                     else
                         sprintf(buffer, "%s%s.mp3", mp3_dir, artista);
                     copy_file(FindFileData.cFileName, FindFileData.cFileName, buffer, TotFileConvertiti, Errs, log);
                }
                fclose(file);
                free(buffer);
             }
        }
        while(FindNextFile(hFind, &FindFileData) != 0);
    }
    else
    {
        fprintf(stderr, "Nessun MP3 trovato nella cartella.\n");
        fflush(stderr);
        MessageBox(NULL, TEXT("Nessun MP3 trovato nella cartella."), TEXT("Errore"), MB_ICONWARNING | MB_OK);
        exit(EXIT_FAILURE);
    }
    FindClose(hFind);
}

void
parse_mp3(const char * buf,
          int size,
          int ext)
{
     int tot = 0, j, check, artista_fatto = 0, titolo_fatto = 0, album_fatto = 0;
     register int i = 0, count, index;
     char * ptr;
     char temp[SIZE];
	 
	 check = (ext == 1) ? 3 : 2;
     
     while(i < size)
     {
         if(NON_PRINTABLE(buf[i]))
            i++;
         for(count = 0; count < SIZE && (buf[i] < 0 || (buf[i] != 0 && buf[i] != 3)); count++, i++)
             temp[count] = buf[i];
         temp[count] = 0;
         
         for(count = 0; count < sizeof(TAG_Artista_Titolo_Album)/sizeof(char *); count++)
         {
             if(TAG_ALBUM(TAG_Artista_Titolo_Album[count]) && ext == 0)
                continue;
             if(tot == check)
                break;
             ptr = strstr(temp, TAG_Artista_Titolo_Album[count]);
             //A volte il TAG si trova come parte dell'mp3, se dopo di esso non c'e' NUL, non e' un TAG
             if(ptr == NULL || (ptr != NULL && temp[ptr-temp+strlen(TAG_Artista_Titolo_Album[count])] != 0))
                continue;
                    
             //A volte dopo c'e' un carattere che rompe le palle, in quel caso
             //ritorniano la posizione successiva (NUL) ad esso
             index = i;
             for(; index < size && NON_PRINTABLE(buf[index]); index++);
             
             //A volte dopo i NUL dopo il TAG ci sono dei caratteri strani (decimali -1 e -2)
             //In quel caso l'artista e il titolo sono divisi lettera per lettera
             if(buf[index] == -1 && buf[index+1] == -2)
             {
                 if(TAG_ARTISTA(TAG_Artista_Titolo_Album[count])){
                     if(artista_fatto) continue;
                     get_tag_lettera_per_lettera(buf, index+2, artista);
                     fix_name(artista, 0);
                     if(NON_VALIDO(artista)) continue;
                     artista_fatto = 1;
                 }else if(TAG_TITOLO(TAG_Artista_Titolo_Album[count])){
                     if(titolo_fatto) continue;
                     get_tag_lettera_per_lettera(buf, index+2, titolo);
                     fix_name(titolo, 0);
                     if(NON_VALIDO(titolo)) continue;
                     titolo_fatto = 1;
                 }else if(TAG_ALBUM(TAG_Artista_Titolo_Album[count])){
                     if(album_fatto) continue;
                     get_tag_lettera_per_lettera(buf, index+2, album);
                     fix_name(album, 1);
                     if(NON_VALIDO(album)) continue;
                     album_fatto = 1;
                 }
                 tot++;
                 continue;
             }

             if(my_strlen(buf+index) == 1)
                 index++;
             else
                 index = i;
                 
             for(; index < size && NON_PRINTABLE(buf[index]); index++);
             if(buf[index] == -1 && buf[index+1] == -2)
             {
                 if(TAG_ARTISTA(TAG_Artista_Titolo_Album[count])){
                     if(artista_fatto) continue;
                     get_tag_lettera_per_lettera(buf, index+2, artista);
                     fix_name(artista, 0);
                     if(NON_VALIDO(artista)) continue;
                     artista_fatto = 1;
                 }else if(TAG_TITOLO(TAG_Artista_Titolo_Album[count])){
                     if(titolo_fatto) continue;
                     get_tag_lettera_per_lettera(buf, index+2, titolo);
                     fix_name(titolo, 0);
                     if(NON_VALIDO(titolo)) continue;
                     titolo_fatto = 1;
                 }else if(TAG_ALBUM(TAG_Artista_Titolo_Album[count])){
                     if(album_fatto) continue;
                     get_tag_lettera_per_lettera(buf, index+2, album);
                     fix_name(album, 1);
                     if(NON_VALIDO(album)) continue;
                     album_fatto = 1;
                 }
                 tot++;
                 continue;
             }
             else
             {
                 if(TAG_ARTISTA(TAG_Artista_Titolo_Album[count]))
                 {
                     if(artista_fatto)  continue;
                     //Ho beccato l'artista, salto eventuali caratteri di controllo
                     for(; index < size && NON_PRINTABLE(buf[index]); index++);
                     //Copio l'artista con eventuali TAG a seguire che vanno tolti
                     for(j = 0; index < size && buf[index] != 0; index++, j++)
                           artista[j] = buf[index];
                     artista[j] = 0;
                     fix_name(artista, 0);
                     if(NON_VALIDO(artista)) continue;
                     artista_fatto = 1;
                 }else if(TAG_TITOLO(TAG_Artista_Titolo_Album[count])){
                     if(titolo_fatto)  continue;
                     //Ho beccato il titolo, salto eventuali caratteri di controllo
                     for(; index < size && NON_PRINTABLE(buf[index]); index++);
                     //Copio il titolo con eventuali TAG a seguire che vanno tolti
                     for(j = 0; index < size && buf[index] != 0; index++, j++)
                           titolo[j] = buf[index];
                     titolo[j] = 0;
                     fix_name(titolo, 0);
                     if(NON_VALIDO(titolo)) continue;
                     titolo_fatto = 1;
                 }else if(TAG_ALBUM(TAG_Artista_Titolo_Album[count])){
                     if(album_fatto)  continue;
                     //Ho beccato il titolo, salto eventuali caratteri di controllo
                     for(; index < size && NON_PRINTABLE(buf[index]); index++);
                     //Copio il titolo con eventuali TAG a seguire che vanno tolti
                     for(j = 0; index < size && buf[index] != 0; index++, j++)
                           album[j] = buf[index];
                     album[j] = 0;
                     fix_name(album, 1);
                     if(NON_VALIDO(album)) continue;
                     album_fatto = 1;
                 }
                 tot++;
             }
         }
         //Ho gia' trovato artista e titolo (e album eventualmente),
		 //non c'e' motivo di continuare il while
         if(tot == check) break;
     }
}

void
fix_name(char * str,
         int is_album)
{
     int i, j;
     char * ptr;
     
     //Eliminiamo i caratteri non ammessi e quelli non stampabili
     for(i = 0; str[i] != 0; )
     {
         if(NOT_ALLOWED(str[i]) || NON_PRINTABLE(str[i]))
         {
            for(j = i; str[j+1] != 0; j++)
               str[j] = str[j+1];
            str[j] = 0;
         }
         else
            i++;
     }
     //Eliminiamo i TAG
     for(i = 0; i < (sizeof(TAG)/sizeof(char *)); i++)
         if((ptr = strstr(str, TAG[i])) != NULL)
             break;   
     if(ptr != NULL)
        *ptr = 0;
     /* Il nome di un album sara' quello di una cartella, ed il nome di una cartella
        non ammette il carattere "." alla fine del nome stesso, ne' uno, ne' piu' di uno*/
     if(is_album)
        for(i = strlen(str)-1; i >= 0 && str[i] == '.'; i--)
           str[i] = 0;
     //Eliminiamo i leading spaces, cioe' gli spazi finali senza caratteri a seguire, es. "Ciao     "
     for(i = strlen(str)-1; i >= 0 && str[i] == ' '; i--)
        str[i] = 0;
}

void
get_tag_lettera_per_lettera(const char * from,
                            int index,
                            char * to)
{
     int count = 0;
     for(; from[index] != 0 && from[index+1] == 0 && strlen(from+index+2) == 1; index += 2)
        to[count++] = from[index];
     to[count++] = from[index];
     to[count] = 0;   
}

//Il carattere di fine stringa e', in questo caso, un qualsiasi carattere non stampabile
int
my_strlen(const char * str)
{
    int count;
    for(count = 0; !NON_PRINTABLE(str[count]); count++);
    return count;
}

void
get_tag_fine_file(char * buf,
                  char * to,
                  FILE * f,
                  int size,
                  int offset)
{
    int i, count;
    ZeroMemory(buf, size);
    fseek(f, -128, SEEK_END);
    fread(buf, 1, 128, f);
    rewind(f);
    
    if(buf[0] == 'T' && buf[1] == 'A' && buf[2] == 'G')
    {
       for(count = 0, i = offset; buf[i] != 0 && count < 30; i++, count++)
          to[count] = buf[i];
       to[count] = 0;
       fix_name(to, 0);
    }
}

void
copy_file(const char * file_name,
          const char * file_renamed,
          const char * new_path,
          int * nFile,
          int * err,
          FILE * log)
{
    FILE * file;
    char temp1[SIZE], temp2[5];
    int i, index, count;
    char * ptr;
    
    if(CopyFile(file_name, new_path, TRUE) == 0)
    {
         for(i = 2; ; i++)
         {
             itoa(i, temp2, 10);
             strcpy(temp1, new_path);
             ptr = strstr(temp1, ".mp3");
             *ptr = ' ';
             *(ptr + 1) = '(';
             *(ptr + 2) = 0;
             strcat(temp1, temp2);
             strcat(temp1, ").mp3");
             if((file = fopen(temp1, "r")) == NULL)
             {
                 if(CopyFile(file_name, temp1, TRUE) == 0)
                 {
                     printf("%s: Impossibile salvare \"%s\"\n", file_name, file_renamed);
                     fprintf(stderr, "%s: Impossibile salvare \"%s\"\n", file_name, file_renamed);
                     fflush(stderr);
                     (*err)++;
                 }
                 else
                 {
                     /*temp3 contiene il path assoluto del file, nella stampa
                       a video e su file vogliamo invece solo il nome del file,
                       accorciamo quindi temp3*/
                       index = 0;
                       for(count = strlen(temp1)-1; count >= 0 && temp1[count] != '\\'; count--);
                       count++;
                       for(; temp1[count] != 0; count++)
                           temp1[index++] = temp1[count];
                       temp1[index] = 0;
                       printf("%s: %s\n", file_name, temp1);
                       fprintf(log, "%4d - %s: %s\n", ++prog, file_name, temp1);
                       fflush(log);
                       (*nFile)++;
                 }
                 break;
             }  
             else
                 fclose(file);
         }
     }
     else
     {
         printf("%s: %s\n", file_name, file_renamed);
         fprintf(log, "%4d - %s: %s\n", ++prog, file_name, file_renamed);
         fflush(log);
         (*nFile)++;
     }
}
