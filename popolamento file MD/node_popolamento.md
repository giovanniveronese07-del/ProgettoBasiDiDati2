# PERSONA
 
Generazione di 450 persone con codice fiscale coerente, dal 2008 al 1961 con distribuzione gaussiana

# GUIDA e ESCURESIONISTA

Popolamento con Generalizzazione Sovrapposta per GUIDA e ESCURSIONISTA
Per implementare la tua richiesta sui 450 record (400 escursionisti, 30 guide, con una parte di guide che sono anche escursionisti), ho preparato uno script Python.
Lo script legge il file dati_persona.sql che hai creato prima, estrae i Codici Fiscali e li distribuisce secondo queste regole matematiche per garantire la sovrapposizione:

400 CF assegnati a ESCURSIONISTA.
20 CF pescati tra gli escursionisti e assegnati anche a GUIDA (sovrapposizione).
10 CF pescati tra i 50 rimanenti e assegnati solo a GUIDA (guide pure).
20 CF rimarranno senza specializzazione (utenti generici).

modifica entità rifugio e cima, struttura entità tappa

eliminazione entità luogo d'interesse?

