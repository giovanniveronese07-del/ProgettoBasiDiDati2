# Progetto Basi di Dati

## Valutazione del progetto

| Area di valutazione | Punteggio ottenuto | Punteggio massimo |
|----------------------|:------------------:|:-----------------:|
| Descrizione dei requisiti e qualità della relazione | **2.0** | 3.0 |
| Progettazione concettuale | **7.5** | 7.5 |
| Ristrutturazione dello schema concettuale | **2.0** | 2.0 |
| Schema relazionale | **3.0** | 4.0 |
| Implementazione SQL | **2.0** | 2.0 |
| Query | **7.5** | 7.5 |
| Indici | **2.0** | 2.0 |
| Accesso al database tramite C | **2.0** | 2.0 |

**Punteggio totale:** **28 / 30**

### Osservazioni del docente

- Le generalizzazioni parziali non sono sufficientemente giustificate nell'analisi dei requisiti.
- Nello schema relazionale l'attributo `superficie` del `Parco` può essere nullo, ma questo non è indicato con la cardinalità `(0,1)` nel diagramma E-R.
---
## Tecnologie utilizzate

- C
- SQL
- PostgreSQL
- LaTeX
- Draw.io

## Contenuto della repository

- `Escursioni.pdf` – documentazione completa del progetto.
- `escursioni.sql` – script completo contenente creazione del database, popolamento e query.
- `codice.c` – applicazione C per l'interazione con il database.
- `dependencies` compilazione file c da Windows per altri OS consultare il pdf del laboratorio