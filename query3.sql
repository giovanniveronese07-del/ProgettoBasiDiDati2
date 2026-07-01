--QUERY 3 Elenco delle escursioni comprese in un intervallo di date, con guida e sentiero, ordinate cronologicamente
SELECT e.id_escursione, e.data_programmata, SUBSTRING(s.nome FROM 1 FOR 30) AS sentiero, p.nome, p.cognome, e.costo
FROM ESCURSIONE e
JOIN SENTIERO s
ON e.codice_sentiero = s.codice
JOIN GUIDA g
ON e.cf_guida = g.cf
JOIN PERSONA p
ON g.cf = p.cf
WHERE e.data_programmata BETWEEN '2026-06-13' AND '2026-12-24'
ORDER BY e.data_programmata;