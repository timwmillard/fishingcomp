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

--name: UpdateCompetitor :one
update competitor
    set first_name = ?,
        last_name = ? 
where id = ? 
returning *;

-- name: DeleteCompetitor :exec
delete from competitor
where id = ?;

