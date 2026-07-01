--QUERY 5 Costo medio delle escursioni organizzate da ogni guida
SELECT g.cf, p.nome, p.cognome, ROUND(AVG(e.costo),2) AS costo_medio
FROM GUIDA g
JOIN PERSONA p ON g.cf = p.cf
JOIN ESCURSIONE e ON g.cf = e.cf_guida
GROUP BY g.cf, p.nome, p.cognome
ORDER BY costo_medio DESC;