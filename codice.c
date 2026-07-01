#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dependencies/include/libpq-fe.h"

#define PG_HOST "127.0.0.1"
#define PG_USER "postgres"
#define PG_DB "BancaDelSangue"
#define PG_PASS "GiovaVero04."
#define PG_PORt 5432

#define COL_WIDTH 30


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
    sprintf(conninfo,"user=%s password=%s dbname=%s hostaddr=%s port=%d", PG_USER, PG_PASS, PG_DB, PG_HOST, PG_PORt);


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
    printf("1. Gruppi sanguigni sotto soglia minima per ospedale\n");
    printf("2. Distribuzione delle scorte per provincia\n");
    printf("3. Visualizza il reparto che ha utilizzato piu' sacche\n");
    printf("4. Donatori con piu' donazioni per gruppo sanguigno\n");
    printf("5. Ospedali che hanno piu' trasferimenti in entrata che in uscita\n");
    printf("0. Esci\n");
}

//Controllare gli ospedali con il numero di sacche inferiore ad un limite imposto da utente
void query1(PGconn *conn) {
    PGresult *res;
    //creazione query
    char *query =
        "SELECT o.nome, s.gruppo_sanguigno, s.fattore_rh, COUNT(*) AS numero_sacche "
        "FROM SACCA s JOIN OSPEDALE o ON s.id_ospedale = o.id "
        "WHERE s.stato_sacca = 'Disponibile' "
        "GROUP BY o.nome, s.gruppo_sanguigno, s.fattore_rh "
        "HAVING COUNT(*) < $1 "
        "ORDER BY o.nome, s.gruppo_sanguigno, s.fattore_rh;";

    //preparo la query a Postgre
    res = PQprepare(conn, "query1", query, 1, NULL);

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        printf("Errore nella preparazione della query: %s\n", PQerrorMessage(conn));
        PQclear(res);
        return;
    }

    PQclear(res);

    //creo il parametro
    int limite;
    printf("Inserire la soglia minima di sacche: ");
    scanf("%d", &limite);

    char limite_str[16];
    snprintf(limite_str, sizeof(limite_str), "%d", limite);

    const char *paramValues[1] = { limite_str };

    //eseguo la query
    res = PQexecPrepared(conn, "query1", 1, paramValues, NULL, NULL, 0);

    checkResult(res, conn);

    //stampo il risultato
    printResult(res);
    PQclear(res);
}

//Seleziona la quantita' di sacche per provincia divise per gruppo
void query2(PGconn *conn){
    PGresult *res;

    char *query = 
        "SELECT o.provincia, s.gruppo_sanguigno, fattore_rh, COUNT(*) AS num_scorte "
        "FROM OSPEDALE o JOIN SACCA s ON o.id = s.id_ospedale "
        "WHERE s.stato_sacca = 'Disponibile' "
        "GROUP BY o.provincia, s.gruppo_sanguigno, fattore_rh "
        "ORDER BY o.provincia, s.gruppo_sanguigno, fattore_rh; ";

    res = PQexec(conn,query);
    checkResult(res, conn);
    
    printResult(res);

    PQclear(res);
}

//restituisce l'id il nome ed il reparto che ha utilizzato piu' sangue
void query3(PGconn *conn){
    PGresult *res;
    
    char *query =
          "DROP VIEW IF EXISTS num_sacche_reparto; "
          "CREATE VIEW num_sacche_reparto AS "
          "SELECT o.id, o.nome, rs.reparto_destinazione, SUM(rs.quantita_sacche) AS Num_sacche "
          "FROM RICHIESTA_SANGUE rs JOIN OSPEDALE o ON rs.id_ospedale_ricovero = o.id "
          "GROUP BY o.id, o.nome, rs.reparto_destinazione "
          "ORDER BY Num_sacche DESC; "
          "SELECT id, nome, reparto_destinazione, Num_sacche "
          "FROM num_sacche_reparto "
          "WHERE Num_sacche = (SELECT MAX(Num_sacche) FROM num_sacche_reparto); ";
          
    res = PQexec(conn, query);
    checkResult(res, conn);
    
    printResult(res);
    
    PQclear(res);
}

//trova tutti i donatori dui un determinato gruppo sanguigno che hanno donato di piu'
void query4(PGconn *conn){
    PGresult *res;
    
    char *query =
        "DROP VIEW IF EXISTS donazioni_per_donatore; "
        "CREATE VIEW donazioni_per_donatore AS "
        "SELECT d.cf, d.nome, d.cognome, d.gruppo_sanguigno, d.fattore_rh, COUNT(p.id) AS numero_prelievi "
        "FROM DONATORE d JOIN PRELIEVO p ON d.cf = p.cf_donatore "
        "GROUP BY d.cf, d.nome, d.cognome, d.gruppo_sanguigno, d.fattore_rh; "
	    "SELECT * "
        "FROM donazioni_per_donatore d1 " 
        "WHERE numero_prelievi =(SELECT MAX(d2.numero_prelievi) "
        "FROM donazioni_per_donatore d2 "
        "WHERE d2.gruppo_sanguigno = d1.gruppo_sanguigno "
        "AND d2.fattore_rh = d1.fattore_rh) "
        "ORDER BY gruppo_sanguigno, fattore_rh; ";
          
    res = PQexec(conn, query);
    checkResult(res, conn);
    
    printResult(res);
    
    PQclear(res);
}

//Trovare gli ospedali che hanno piu' trasferimenti in entrata che in uscita
void query5(PGconn *conn){
    PGresult *res;
    
    char *query =
        "SELECT o.id, o.nome, COUNT(t_in.id_sacca) AS sacche_ricevute, COUNT(t_out.id_sacca) AS sacche_spedite, COUNT(t_in.id_sacca) - COUNT(t_out.id_sacca) AS differenza "
        "FROM ospedale o LEFT JOIN TRASFERIMENTO t_in ON o.id = t_in.id_ospedale_mittente "
        "LEFT JOIN TRASFERIMENTO t_out ON o.id = t_out.id_ospedale_destinatario "
        "GROUP BY o.id, o.nome "
        "HAVING COUNT(t_in.id_sacca) > COUNT(t_out.id_sacca) "
        "ORDER BY differenza DESC; ";
          
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
