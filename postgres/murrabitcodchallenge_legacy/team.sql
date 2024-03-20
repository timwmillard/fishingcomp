
-- Teams

-- 2009
with event as (
	select id from fishing.event where slug = '2009'
)
insert into fishing.team (
    event_id,
    team_no,
    name,
    boat_rego
)
select
    event.id,
    boat_id,
    coalesce(registration_no, ''),
    coalesce(registration_no, '')
from legacy_2009.boat, event;

 with event as (
 	select id from fishing.event where slug = '2009'
 )
 update fishing.competitor
 set team_id = team.id
 from fishing.team team, event
 where
     competitor.legacy_boat_id = team.team_no::int
     and team.event_id = event.id
     and competitor.event_id = event.id;


-- 2010
with event as (
	select id from fishing.event where slug = '2010'
)
insert into fishing.team (
    event_id,
    team_no,
    name,
    boat_rego
)
select
    event.id,
    boat_id,
    coalesce(registration_no, ''),
    coalesce(registration_no, '')
from legacy_2010.boat, event;

 with event as (
 	select id from fishing.event where slug = '2010'
 )
 update fishing.competitor
 set team_id = team.id
 from fishing.team team, event
 where
     competitor.legacy_boat_id = team.team_no::int
     and team.event_id = event.id
     and competitor.event_id = event.id;

-- 2011
with event as (
	select id from fishing.event where slug = '2011'
)
insert into fishing.team (
    event_id,
    team_no,
    name,
    boat_rego
)
select
    event.id,
    boat_id,
    coalesce(registration_no, ''),
    coalesce(registration_no, '')
from legacy_2011.boat, event;

 with event as (
 	select id from fishing.event where slug = '2011'
 )
 update fishing.competitor
 set team_id = team.id
 from fishing.team team, event
 where
     competitor.legacy_boat_id = team.team_no::int
     and team.event_id = event.id
     and competitor.event_id = event.id;


-- 2012
with event as (
	select id from fishing.event where slug = '2012'
)
insert into fishing.team (
    event_id,
    team_no,
    name,
    boat_rego
)
select
    event.id,
    boat_id,
    coalesce(registration_no, ''),
    coalesce(registration_no, '')
from legacy_2012.boat, event;

 with event as (
 	select id from fishing.event where slug = '2012'
 )
 update fishing.competitor
 set team_id = team.id
 from fishing.team team, event
 where
     competitor.legacy_boat_id = team.team_no::int
     and team.event_id = event.id
     and competitor.event_id = event.id;


-- 2013
with event as (
	select id from fishing.event where slug = '2013'
)
insert into fishing.team (
    event_id,
    team_no,
    name,
    boat_rego
)
select
    event.id,
    boat_id,
    coalesce(registration_no, ''),
    coalesce(registration_no, '')
from legacy_2013.boat, event;

 with event as (
 	select id from fishing.event where slug = '2013'
 )
 update fishing.competitor
 set team_id = team.id
 from fishing.team team, event
 where
     competitor.legacy_boat_id = team.team_no::int
     and team.event_id = event.id
     and competitor.event_id = event.id;


-- 2014
with event as (
	select id from fishing.event where slug = '2014'
)
insert into fishing.team (
    event_id,
    team_no,
    name,
    boat_rego
)
select
    event.id,
    boat_id,
    coalesce(registration_no, ''),
    coalesce(registration_no, '')
from legacy_2014.boat, event;

 with event as (
 	select id from fishing.event where slug = '2014'
 )
 update fishing.competitor
 set team_id = team.id
 from fishing.team team, event
 where
     competitor.legacy_boat_id = team.team_no::int
     and team.event_id = event.id
     and competitor.event_id = event.id;


-- 2015
with event as (
	select id from fishing.event where slug = '2015'
)
insert into fishing.team (
    event_id,
    team_no,
    name,
    boat_rego
)
select
    event.id,
    boat_id,
    coalesce(registration_no, ''),
    coalesce(registration_no, '')
from legacy_2015.boat, event;

 with event as (
 	select id from fishing.event where slug = '2015'
 )
 update fishing.competitor
 set team_id = team.id
 from fishing.team team, event
 where
     competitor.legacy_boat_id = team.team_no::int
     and team.event_id = event.id
     and competitor.event_id = event.id;

-- 2016
with event as (
	select id from fishing.event where slug = '2016'
)
insert into fishing.team (
    event_id,
    team_no,
    name,
    boat_rego
)
select
    event.id,
    boat_id,
    coalesce(registration_no, ''),
    coalesce(registration_no, '')
from legacy_2016.boat, event;

 with event as (
 	select id from fishing.event where slug = '2016'
 )
 update fishing.competitor
 set team_id = team.id
 from fishing.team team, event
 where
     competitor.legacy_boat_id = team.team_no::int
     and team.event_id = event.id
     and competitor.event_id = event.id;

-- 2017
with event as (
	select id from fishing.event where slug = '2017'
)
insert into fishing.team (
    event_id,
    team_no,
    name,
    boat_rego
)
select
    event.id,
    boat_id,
    coalesce(registration_no, ''),
    coalesce(registration_no, '')
from legacy_2017.boat, event;

 with event as (
 	select id from fishing.event where slug = '2017'
 )
 update fishing.competitor
 set team_id = team.id
 from fishing.team team, event
 where
     competitor.legacy_boat_id = team.team_no::int
     and team.event_id = event.id
     and competitor.event_id = event.id;


-- 2018
with event as (
	select id from fishing.event where slug = '2018'
)
insert into fishing.team (
    event_id,
    team_no,
    name,
    boat_rego
)
select
    event.id,
    boat_id,
    coalesce(registration_no, ''),
    coalesce(registration_no, '')
from legacy_2018.boat, event;

 with event as (
 	select id from fishing.event where slug = '2018'
 )
 update fishing.competitor
 set team_id = team.id
 from fishing.team team, event
 where
     competitor.legacy_boat_id = team.team_no::int
     and team.event_id = event.id
     and competitor.event_id = event.id;


-- 2019
with event as (
	select id from fishing.event where slug = '2019'
)
insert into fishing.team (
    event_id,
    team_no,
    name,
    boat_rego
)
select
    event.id,
    boat_id,
    coalesce(registration_no, ''),
    coalesce(registration_no, '')
from legacy_2019.boat, event;

 with event as (
 	select id from fishing.event where slug = '2019'
 )
 update fishing.competitor
 set team_id = team.id
 from fishing.team team, event
 where
     competitor.legacy_boat_id = team.team_no::int
     and team.event_id = event.id
     and competitor.event_id = event.id;


-- 2021
with event as (
	select id from fishing.event where slug = '2021'
)
insert into fishing.team (
    event_id,
    team_no,
    name,
    boat_rego
)
select
    event.id,
    boat_id,
    coalesce(registration_no, ''),
    coalesce(registration_no, '')
from legacy_2021.boat, event;

 with event as (
 	select id from fishing.event where slug = '2021'
 )
 update fishing.competitor
 set team_id = team.id
 from fishing.team team, event
 where
     competitor.legacy_boat_id = team.team_no::int
     and team.event_id = event.id
     and competitor.event_id = event.id;



-- 2022
with event as (
	select id from fishing.event where slug = '2022'
)
insert into fishing.team (
    event_id,
    team_no,
    name,
    boat_rego
)
select
    event.id,
    boat_id,
    coalesce(registration_no, ''),
    coalesce(registration_no, '')
from legacy_2022.boat, event;

 with event as (
 	select id from fishing.event where slug = '2022'
 )
 update fishing.competitor
 set team_id = team.id
 from fishing.team team, event
 where
     competitor.legacy_boat_id = team.team_no::int
     and team.event_id = event.id
     and competitor.event_id = event.id;


-- drop lecacy_boat_id
alter table fishing.competitor drop column legacy_boat_id;
