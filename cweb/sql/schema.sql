create table if not exists business (
    id integer primary key check (id = 1),
    name text not null,
);

