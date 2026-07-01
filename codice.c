#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dependencies/include/libpq-fe.h"

#define PG_HOST "127.0.0.1"
#define PG_USER "postgres"
#define PG_DB "Escursioni"
#define PG_PASS "GiovaVero04."
#define PG_PORT 5432

#define COL_WIDTH 40


void checkResult(PGresult *res, const PGconn *conn);
void do_exit(PGconn *conn);
void printResult(PGresult *res);
void printMenu();

void query1(PGconn *conn);
void query2(PGconn *conn);
void query3(PGconn *conn);
void query4(PGconn *conn);
void query5(PGconn *conn);


int main(int argc, char **argv){
/*
    //connessione al server
    char PG_host[50];
    char PG_user[50];
    char PG_pw[100];
    char PG_db[50];
    int PG_port;

    printf("Host: ");
    scanf("%49s", PG_host);

    printf("Database: ");
    scanf("%49s", PG_db);

    printf("Utente: ");
    scanf("%49s", PG_user);

    printf("password: ");
    scanf("%99s", PG_pw);
    
    printf("Porta: ");
    scanf("%d", PG_port);

    char conninfo[512];
    sprintf(conninfo,"user=%s password=%s dbname=%s hostaddr=%s port=%d", PG_user, PG_pw, PG_db, PG_host, PG_port);*/
    
    //per verifica in locale
    char conninfo[512];
    sprintf(conninfo,"user=%s password=%s dbname=%s hostaddr=%s port=%d", PG_USER, PG_PASS, PG_DB, PG_HOST, PG_PORT);


    PGconn *conn;
    conn = PQconnectdb(conninfo);

    //controllo della connessione
    if(PQstatus(conn) != CONNECTION_OK){
        printf("errore di connesione: %s\n", PQerrorMessage(conn));
        do_exit(conn);
    }

    // scelta query

    int scelta;

    while(1){
        printMenu();
        printf("Inserire la scelta desiderata : ");
        scanf("%d", &scelta);
        switch(scelta){

            case 1:
                query1(conn);
                break;

            case 2:
                query2(conn);
                break;

            case 3:
                query3(conn);
                break;

            case 4:
                query4(conn);
                break;

            case 5:
                query5(conn);
                break;

            case 0:
                PQfinish(conn);
                printf("Connessione chiusa\n");
                return 0;

            default:
                printf("scelta non valida\n");
        }
    }


    
}

//controlla la valididta' della query
void checkResult(PGresult *res, const PGconn *conn){
    if(PQresultStatus(res) != PGRES_TUPLES_OK){
        printf("Risultati inconsistenti %s\n", PQerrorMessage(conn));
        PQclear(res);
        exit(1);
    }
}

void do_exit(PGconn *conn){
    PQfinish(conn);
    exit(1);
}

//visualizza il menu'
void printMenu(){
    printf("\n====== MENU QUERY ======\n");
    printf("1. Numero di escursioni organizzate da ogni guida\n");
    printf("2. Sentieri con il massimo numero di tappe visitabili\n");
    printf("3. Escursioni comprese in un intervallo di date\n");
    printf("4. Escursionisti che hanno partecipato almeno N escursioni\n");
    printf("5. Costo medio delle escursioni organizzate da ogni guida\n");
    printf("0. Esci\n");
}

//Query 1: Numero di escursioni organizzate da ogni guida
void query1(PGconn *conn) {
    PGresult *res;

    char *query = 
        "SELECT g.cf, p.nome, p.cognome, COUNT(e.id_escursione) AS numero_escursioni "
        "FROM GUIDA g "
        "JOIN PERSONA p ON g.cf = p.cf "
        "LEFT JOIN ESCURSIONE e ON g.cf = e.cf_guida "
        "GROUP BY g.cf, p.nome, p.cognome "
        "ORDER BY numero_escursioni DESC; ";

    res = PQexec(conn,query);
    checkResult(res, conn);
    
    printResult(res);

    PQclear(res);
}

//Query 2: Sentieri con il massimo numero di tappe visitabili
void query2(PGconn *conn){
    PGresult *res;

    char *query = 
        "DROP VIEW IF EXISTS V_NUMERO_TAPPE; "
        "CREATE VIEW V_NUMERO_TAPPE AS "
        "SELECT s.codice, s.nome, COUNT(a.id_tappa) AS numero_tappe "
        "FROM SENTIERO s "
        "JOIN attraversa a ON s.codice = a.codice_sentiero "
        "GROUP BY s.codice, s.nome; "
        "SELECT codice, nome, numero_tappe "
        "FROM V_NUMERO_TAPPE "
        "WHERE numero_tappe = ( "
        "SELECT MAX(numero_tappe) "
        "FROM V_NUMERO_TAPPE);";

    res = PQexec(conn,query);
    checkResult(res, conn);
    
    printResult(res);

    PQclear(res);
}

//Query 3: Escursioni comprese in un intervallo di date
void query3(PGconn *conn){
    PGresult *res;
    
    char *query = 
        "SELECT e.id_escursione, e.data_programmata, SUBSTRING(s.nome FROM 1 FOR 30) AS sentiero, p.nome, p.cognome, e.costo "
        "FROM ESCURSIONE e "
        "JOIN SENTIERO s ON e.codice_sentiero = s.codice "
        "JOIN GUIDA g ON e.cf_guida = g.cf "
        "JOIN PERSONA p ON g.cf = p.cf "
        "WHERE e.data_programmata BETWEEN $1 AND $2 "
        "ORDER BY e.data_programmata; ";
          
    res = PQprepare(conn, "query3", query, 2, NULL);

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        printf("Errore nella preparazione della query: %s\n", PQerrorMessage(conn));
        PQclear(res);
        return;
    }

    PQclear(res);

    char dataInizio[11];
    char dataFine[11];

    printf("Inserisci la data iniziale (YYYY-MM-DD): ");
    scanf("%10s", dataInizio);

    printf("Inserisci la data finale (YYYY-MM-DD): ");
    scanf("%10s", dataFine);

    const char *paramValues[2] = {dataInizio, dataFine};

    res = PQexecPrepared(conn,"query3", 2, paramValues, NULL, NULL, 0);

    checkResult(res, conn);

    printResult(res);

    PQclear(res);
}

//Query 4: Escursionisti che hanno partecipato almeno N escursioni
void query4(PGconn *conn){
    PGresult *res;
    //creazione query
    char *query =
        "SELECT e.cf, p.nome, p.cognome, COUNT(pa.id_escursione) AS numero_escursioni "
        "FROM ESCURSIONISTA e " 
        "JOIN PERSONA p ON e.cf = p.cf "
        "JOIN partecipa pa ON e.cf = pa.cf_escursionista "
        "GROUP BY e.cf, p.nome, p.cognome "
        "HAVING COUNT(pa.id_escursione) >= $1 "
        "ORDER BY numero_escursioni DESC;";

    //preparo la query a Postgre
    res = PQprepare(conn, "query4", query, 1, NULL);

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        printf("Errore nella preparazione della query: %s\n", PQerrorMessage(conn));
        PQclear(res);
        return;
    }

    PQclear(res);

    //creo il parametro
    int limite;
    printf("Inserire il numero di escursioni (scelta consigliata -> 6): ");
    scanf("%d", &limite);

    char limite_str[16];
    snprintf(limite_str, sizeof(limite_str), "%d", limite);

    const char *paramValues[1] = { limite_str };

    //eseguo la query
    res = PQexecPrepared(conn, "query4", 1, paramValues, NULL, NULL, 0);

    checkResult(res, conn);

    //stampo il risultato
    printResult(res);
    PQclear(res);
}

//Query 5: Costo medio delle escursioni organizzate da ogni guida
void query5(PGconn *conn){
    PGresult *res;
    
    char *query =
        "SELECT g.cf, p.nome, p.cognome, ROUND(AVG(e.costo),2) AS costo_medio " 
        "FROM GUIDA g "
        "JOIN PERSONA p ON g.cf = p.cf "
        "JOIN ESCURSIONE e ON g.cf = e.cf_guida "
        "GROUP BY g.cf, p.nome, p.cognome "
        "ORDER BY costo_medio DESC;";
          
    res = PQexec(conn, query);
    checkResult(res, conn);
    
    printResult(res);
    
    PQclear(res);
}


//stampa il risultato
void printResult(PGresult *res){

    //trovo il numero di tuple e campi selezionati
    int tuple = PQntuples(res);
    int campi = PQnfields(res);

    //stampo le intestazioni delle colonne
    for(int i = 0; i < campi; i++){
        printf("%-*s",COL_WIDTH, PQfname(res, i));
    }

    printf("\n");

    // linea separatrice
    for (int i = 0; i < campi; i++) {
        printf("%-*s", COL_WIDTH, "-------------------");
    }
    printf("\n");

    //stampo i valori
    for(int i = 0; i < tuple; i++){
        for(int j = 0; j< campi; j++){
            printf("%-*s",COL_WIDTH, PQgetvalue(res, i, j));
        }
        printf("\n");
    }
}
