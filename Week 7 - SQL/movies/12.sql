SELECT title FROM movies
JOIN stars ON movies.id = stars.movie_id
WHERE person_id = (SELECT id FROM people WHERE name = "Johnny Depp")
AND movie_id IN (SELECT movie_id FROM stars WHERE person_id = (SELECT id FROM people WHERE name = "Helena Bonham Carter"));