--QUERY 4 Escursionisti che ahanno partecipato almeno a N escursioni
SELECT e.cf, p.nome, p.cognome, COUNT(pa.id_escursione) AS numero_escursioni
FROM ESCURSIONISTA e
JOIN PERSONA p ON e.cf = p.cf
JOIN partecipa pa ON e.cf = pa.cf_escursionista
GROUP BY e.cf, p.nome, p.cognome
HAVING COUNT(pa.id_escursione) >= 6
ORDER BY numero_escursioni DESC;