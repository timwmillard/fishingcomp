-- name: GetCompetitor :one
select *
from competitor
where id = ?;

-- name: ListCompetitors :many
select *
from competitor;

-- name: CreateCompetitor :one
insert into competitor (
    first_name,
    last_name,
    email
) values ( ?, ?, ?)
returning id, first_name, last_name;

-- name: DeleteCompetitor :exec
delete from competitor
where id = ?;

