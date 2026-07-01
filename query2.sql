--Sentieri con il massimo numero di tappe visitabili
DROP VIEW IF EXISTS V_NUMERO_TAPPE;

CREATE VIEW V_NUMERO_TAPPE AS
SELECT s.codice, s.nome, COUNT(a.id_tappa) AS numero_tappe
FROM SENTIERO s
JOIN attraversa a
    ON s.codice = a.codice_sentiero
GROUP BY s.codice, s.nome;

SELECT codice, nome, numero_tappe
FROM V_NUMERO_TAPPE
WHERE numero_tappe = (
    SELECT MAX(numero_tappe)
    FROM V_NUMERO_TAPPE
);