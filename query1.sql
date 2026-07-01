--numero di escursioni organizzate da ogni guida
SELECT g.cf, p.nome, p.cognome, COUNT(e.id_escursione) AS numero_escursioni
FROM GUIDA g
JOIN PERSONA p ON g.cf = p.cf
LEFT JOIN ESCURSIONE e ON g.cf = e.cf_guida
GROUP BY g.cf, p.nome, p.cognome
ORDER BY numero_escursioni DESC;