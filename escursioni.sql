CREATE TABLE PERSONA (
    cf
    tipo_persona ('GUIDA', 'ESCURSIONISTA')),
    nome
    cognome 
    PRIMARY KEY (cf)
);

CREATE TABLE GUIDA (
    tipo_persona  (tipo_persona = 'GUIDA'),
    numero_tesserino 
    FOREIGN KEY (cf, tipo_persona) REFERENCES PERSONA(cf, tipo_persona)
);

CREATE TABLE ESCURSIONISTA (
    tipo_persona  (tipo_persona = 'ESCURSIONISTA'),
    livello_esperienza
    FOREIGN KEY (cf, tipo_persona) REFERENCES PERSONA(cf, tipo_persona)
);

CREATE TABLE PARCO (
    id
    nome
    regione
);

CREATE TABLE SENTIERO (
    codice
    nome
    tipologia_percorso('ANELLO', 'LINEARE')),
    difficolta
    lunghezza_km 
    dislivello
);

CREATE TABLE compone (
    id_sentiero  REFERENCES SENTIERO(codice),
    id_parco  REFERENCES PARCO(id),
    PRIMARY KEY (id_sentiero, id_parco)
);

CREATE TABLE richiede (
    id_sentiero REFERENCES SENTIERO(codice),
    id_qualifica REFERENCES QUALIFICA(id),
    PRIMARY KEY (id_sentiero, id_qualifica)
);
==

CREATE TABLE TAPPA (
    id SERIAL,
    tipo_tappa ('LUOGO_INTERESSE', 'CIMA', 'RIFUGIO')),
    nome
    altitudine_m 
    coordinate_gps 
    PRIMARY KEY (id, tipo_tappa)
);

CREATE TABLE attraversa (
    id_sentiero REFERENCES SENTIERO(id),
    id_tappa
    tipo_tappa
    PRIMARY KEY (id_sentiero, id_tappa),
    FOREIGN KEY (id_tappa, tipo_tappa) REFERENCES TAPPA(id, tipo_tappa)
);

CREATE TABLE LUOGO_INTERESSE (
    id_tappa PRIMARY KEY,
    tipo_tappa (tipo_tappa = 'LUOGO_INTERESSE'),
    tipologia 
);

CREATE TABLE CIMA (
    id_tappa PRIMARY KEY,
    tipo_tappa (tipo_tappa = 'CIMA'),
    gruppo_montuoso 
);

CREATE TABLE GESTORE (
    id 
    nome 
    cognome 
    telefono 
);

CREATE TABLE RIFUGIO (
    id_tappa PRIMARY KEY,
    tipo_tappa  (tipo_tappa = 'RIFUGIO'),
    id_gestore REFERENCES GESTORE(id),
    posti_letto (posti_letto >= 0),
    FOREIGN KEY (id_tappa, tipo_tappa) REFERENCES TAPPA(id, tipo_tappa)
);

CREATE TABLE ESCURSIONE (
    id PRIMARY KEY,
    titolo 
    data_programmata,
    costo,
    guida,
    codice_sentiero REFERENCES SENTIERO(codice)
    FOREIGN KEY (cf, tipo_persona) REFERENCES PERSONA(cf, tipo_persona)

);

CREATE TABLE dirige (
    id_escursione  REFERENCES ESCURSIONE(id),
    id_guida  REFERENCES GUIDA(id_persona),
    PRIMARY KEY (id_escursione, id_guida)
);

CREATE TABLE partecipa (
    id_escursione  REFERENCES ESCURSIONE(id),
    id_escursionista  REFERENCES ESCURSIONISTA(id_persona),
    PRIMARY KEY (id_escursione, id_escursionista)
);