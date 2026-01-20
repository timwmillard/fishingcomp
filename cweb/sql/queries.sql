-- name: GetCompetitor :one
select *
from competitor
where id = ?;

