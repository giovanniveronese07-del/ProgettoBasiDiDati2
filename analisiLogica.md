# 4.1 Analisi delle ridondanze

L'attributo **Num_Tappe** dell'entità **Sentiero**, che memorizza il numero di tappe appartenenti a un sentiero, rappresenta una ridondanza. Tale informazione può infatti essere ricavata contando il numero di tappe associate al sentiero tramite la relazione **Attraversa**.

Le operazioni considerate nell'analisi sono:

- **Operazione 1 (10 volte):** inserimento di una nuova tappa in un sentiero.
- **Operazione 2 (100 volte):** visualizzazione del numero di tappe di un sentiero.

Assumendo i seguenti volumi nella base di dati:

| Concetto | Costrutto | Volume |
|----------|-----------|-------:|
| SENTIERO | E | 100 |
| TAPPA | E | 400 |
| ATTRAVERSA | R | 1000 |

Essendo **Attraversa** una relazione molti-a-molti, è plausibile che diverse tappe siano condivise da più sentieri. Per questo motivo il numero di istanze della relazione è superiore al numero delle tappe.

Si assume inoltre che il costo di una scrittura sia pari a **2**, mentre quello di una lettura sia pari a **1**.

---

## Con ridondanza

### Operazione 1

| Concetto | Costrutto | Accessi | Tipo |
|----------|-----------|:-------:|:----:|
| TAPPA | E | 1 | S |
| ATTRAVERSA | R | 1 | S |
| SENTIERO | E | 1 | L |
| SENTIERO | E | 1 | S |

Operazione eseguita **10 volte**.

### Operazione 2

| Concetto | Costrutto | Accessi | Tipo |
|----------|-----------|:-------:|:----:|
| SENTIERO | E | 1 | L |

Operazione eseguita **100 volte**.

### Costo totale

```
10 × (3 × 2 + 1) + 100 = 170
```

---

## Senza ridondanza

### Operazione 1

| Concetto | Costrutto | Accessi | Tipo |
|----------|-----------|:-------:|:----:|
| TAPPA | E | 1 | S |
| ATTRAVERSA | R | 1 | S |

Operazione eseguita **10 volte**.

### Operazione 2

Poiché il numero di tappe non è memorizzato, è necessario contare le istanze della relazione **Attraversa** associate al sentiero.

Considerando:

```
1000 / 100 = 10
```

in media ogni sentiero attraversa **10 tappe**.

| Concetto | Costrutto | Accessi | Tipo |
|----------|-----------|:-------:|:----:|
| SENTIERO | E | 1 | L |
| ATTRAVERSA | R | 10 | L |

Operazione eseguita **100 volte**.

### Costo totale

```
10 × (2 × 2) + 100 × (10 + 1)
= 40 + 1100
= 1140
```

---

## Conclusioni

L'analisi mostra che mantenere l'attributo ridondante **Num_Tappe** comporta un costo maggiore nelle operazioni di aggiornamento, ma riduce notevolmente il costo delle operazioni di consultazione, che risultano molto più frequenti. Per questo motivo è conveniente mantenere l'attributo ridondante nello schema.

---
---

# 4.2 Eliminazione delle generalizzazioni

Nello schema E-R sono presenti due generalizzazioni, entrambe di tipo **parziale e disgiunta**:

- **Persona**, specializzata nelle entità **Guida** ed **Escursionista**;
- **Tappa**, specializzata nelle entità **Rifugio** e **Cima**.

Per entrambe le generalizzazioni è stata adottata la strategia di mantenere la superclasse e le relative sottoclassi.

## Generalizzazione Persona

La generalizzazione dell'entità **Persona** è stata mantenuta poiché le sottoclassi **Guida** ed **Escursionista** possiedono attributi specifici (`NumeroTesserino` e `ScadenzaIscrizione`) e partecipano a relazioni differenti con l'entità **Escursione** (`Dirige` e `Partecipa`).

L'eliminazione della superclasse avrebbe comportato la duplicazione degli attributi comuni (`CF`, `Nome` e `Cognome`) nelle due sottoclassi, introducendo una ridondanza non necessaria.

## Generalizzazione Tappa

Anche la generalizzazione dell'entità **Tappa** è stata mantenuta.

Le sottoclassi **Rifugio** e **Cima** possiedono attributi specifici (`PostiLetto` e `GruppoMontuoso`) che non sono condivisi. Inoltre, essendo la generalizzazione **parziale**, una tappa può anche non appartenere ad alcuna delle due sottoclassi, rappresentando ad esempio un punto panoramico, un bivio o un'altra tappa significativa del percorso.

Il mantenimento della superclasse consente quindi di rappresentare correttamente tutte le tipologie di tappa, evitando sia la duplicazione degli attributi comuni sia la presenza di valori nulli negli attributi specifici delle sottoclassi.

In entrambe le generalizzazioni, le sottoclassi ereditano l'identificatore della rispettiva superclasse.

---
---

## 4.3 Partizionamento e accorpamento di entità e relazioni

Durante la ristrutturazione dello schema E-R è stata valutata la possibilità di accorpare alcune entità e relazioni.

Non sono stati effettuati ulteriori accorpamenti, poiché le entità presenti rappresentano concetti distinti del dominio applicativo e possiedono attributi o relazioni specifiche che ne giustificano il mantenimento.

Anche le relazioni **Attraversa**, **ParteDa** e **ArrivaA** sono state mantenute separate, in quanto descrivono vincoli semantici differenti. In particolare, **ParteDa** individua la tappa iniziale di un sentiero, **ArrivaA** la tappa finale, mentre **Attraversa** rappresenta l'insieme delle tappe appartenenti al percorso. Un loro accorpamento avrebbe richiesto l'introduzione di attributi discriminanti e avrebbe reso il modello meno chiaro e più complesso da gestire.

Si è pertanto scelto di mantenere inalterata la struttura delle entità e delle relazioni, preservando la chiarezza dello schema concettuale e la corretta rappresentazione del dominio.

---
---

# 4.4 Scelta degli identificatori primari

La scelta degli identificatori primari è stata effettuata privilegiando, ove possibile, attributi naturali univoci e, negli altri casi, identificatori artificiali che garantiscono l'univocità delle istanze.

## Persona

L'entità **Persona** è identificata dal **Codice Fiscale (CF)**, che rappresenta un identificatore naturale univoco e stabile nel tempo.

## Guida ed Escursionista

Le entità **Guida** ed **Escursionista** ereditano l'identificatore della superclasse **Persona**. Di conseguenza, il **Codice Fiscale** identifica univocamente anche le istanze delle due sottoclassi.

## Parco

L'entità **Parco** è identificata dalla coppia **(Nome, Regione)**. La scelta di una chiave composta consente di distinguere univocamente i parchi, evitando possibili ambiguità nel caso di parchi con lo stesso nome situati in regioni diverse.

## Sentiero

L'entità **Sentiero** è identificata da un **Codice** univoco assegnato a ciascun percorso, evitando possibili ambiguità dovute alla presenza di sentieri con lo stesso nome.

## Escursione

L'entità Escursione è identificata da un ID univoco. L'utilizzo di un identificatore artificiale evita la definizione di chiavi composte e consente di distinguere univocamente più escursioni organizzate sullo stesso sentiero.

## Tappa

L'entità **Tappa** è identificata da un **ID** univoco. L'utilizzo di un identificatore artificiale evita possibili duplicazioni dovute a tappe con lo stesso nome o situate alla medesima altitudine.

## Rifugio e Cima

Le entità **Rifugio** e **Cima** ereditano l'identificatore dell'entità **Tappa**, garantendo l'univocità delle rispettive istanze senza introdurre ulteriori identificatori.

---
---

# 4.5 Schema Relazionale

Lo schema E-R ristrutturato contiene esclusivamente costrutti direttamente traducibili nel modello relazionale. Lo schema logico risultante è il seguente.

- Persona(**CF**, Nome, Cognome)\

- Guida(**CF**, NumTesserino)\
    - Guida.Persona → Persona.CF

- Escursionista(**CF**, scadenza_certificato_medico)\
    - Escursionista.Persona → Persona.CF

- Parco(**Nome**, **Regione**, area_km^2)\

- Sentiero(**Codice**, Nome, Difficolta, Lunghezza, tempo_percorrenza, Dislivello, NumTappe, ParcoNome, ParcoRegione, partenza, arrivo)\
    - Sentiero.(ParcoNome, ParcoRegione) → Parco.(Nome, Regione)
    - Sentiero.partenza → Tappa.id
    - Sentiero.arrivo → Tappa.id

- Tappa(**ID**, Nome, altitudine_m, provincia, copertura_rete_mobile)\

- Rifugio(**id_Tappa**, PostiLetto, gestore, proprieta')\\
    - Rifugio.Tappa → Tappa.ID

- Cima(**id_Tappa**, GruppoMontuoso, libro_di_vetta)\
    - Cima.Tappa → Tappa.ID

- Escursione(**ID**,nome, DataProgrammata, Costo, Sentiero, Guida)\\
    - Escursione.Sentiero → Sentiero.Codice
    - Escursione.Guida → Guida.Persona

- Attraversa(**Sentiero**, **Tappa**)\
    - Attraversa.Sentiero → Sentiero.Codice
    - Attraversa.Tappa → Tappa.ID


- Partecipa(**Escursionista**, **Escursione**)\
    - Partecipa.Escursionista → Escursionista.Persona
    - Partecipa.Escursione → Escursione.ID