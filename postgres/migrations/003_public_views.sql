
create or replace view competitors as
select *
from fishing.competitor;

create or replace view boats as
select *
from fishing.boat;

create or replace view catches as
select *
from fishing.catch;

create or replace view catch_reviews as
select *
from fishing.catch_review;

create or replace view species as
select *
from fishing.species;

create or replace view events as
select *
from fishing.event;
