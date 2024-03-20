
-- Catch

-- TODO: caught_at time when NULL.  What should it be?

-- alter table fishing.catch add column legacy_competitor_id int;

-- 2009
with event as (
	select id from fishing.event where slug = '2009'
),
fish_species as (
	select *,
	case
		when slug = 'murraycod' then 'Murray Cod'
		when slug = 'yellowbelly' then 'Yellowbelly'
		when slug = 'carp' then 'Carp'
	end as legacy_name
	from fishing.species
)
insert into fishing.catch (
    event_id,
    competitor_id,
	species_id,
    size,
    caught_at,
    bait,
    location_name
)
select
    event.id as event_id,
    comp.id as new_competitor_id,
    fish_species.id,
    size * 100 as size,
    (date::date + coalesce(time::time, '00:00'::time))::timestamptz at time zone 'AEDT' as caught_at,
    coalesce(bait, '') as bait,
    coalesce(c.location, '') as location
from legacy_2009.catch as c
join event on true
left join fish_species on fish_species.legacy_name = c.fish_type
left join fishing.competitor comp on comp.competitor_no::int = c.competitor_id and comp.event_id = event.id;

-- 2010
with event as (
	select id from fishing.event where slug = '2010'
),
fish_species as (
	select *,
	case
		when slug = 'murraycod' then 'Murray Cod'
		when slug = 'yellowbelly' then 'Yellowbelly'
		when slug = 'carp' then 'Carp'
	end as legacy_name
	from fishing.species
)
insert into fishing.catch (
    event_id,
    competitor_id,
	species_id,
    size,
    caught_at,
    bait,
    location_name
)
select
    event.id as event_id,
    comp.id as new_competitor_id,
    fish_species.id,
    size * 100 as size,
    (date::date + coalesce(time::time, '00:00'::time))::timestamptz at time zone 'AEDT' as caught_at,
    coalesce(bait, '') as bait,
    coalesce(c.location, '') as location
from legacy_2010.catch as c
join event on true
left join fish_species on fish_species.legacy_name = c.fish_type
left join fishing.competitor comp on comp.competitor_no::int = c.competitor_id and comp.event_id = event.id;

-- 2011
with event as (
	select id from fishing.event where slug = '2011'
),
fish_species as (
	select *,
	case
		when slug = 'murraycod' then 'Murray Cod'
		when slug = 'yellowbelly' then 'Yellowbelly'
		when slug = 'carp' then 'Carp'
	end as legacy_name
	from fishing.species
)
insert into fishing.catch (
    event_id,
    competitor_id,
	species_id,
    size,
    caught_at,
    bait,
    location_name
)
select
    event.id as event_id,
    comp.id as new_competitor_id,
    fish_species.id,
    size * 100 as size,
    (date::date + coalesce(time::time, '00:00'::time))::timestamptz at time zone 'AEDT' as caught_at,
    coalesce(bait, '') as bait,
    coalesce(c.location, '') as location
from legacy_2011.catch as c
join event on true
left join fish_species on fish_species.legacy_name = c.fish_type
left join fishing.competitor comp on comp.competitor_no::int = c.competitor_id and comp.event_id = event.id;

-- 2012
with event as (
	select id from fishing.event where slug = '2012'
),
fish_species as (
	select *,
	case
		when slug = 'murraycod' then 'Murray Cod'
		when slug = 'yellowbelly' then 'Yellowbelly'
		when slug = 'carp' then 'Carp'
	end as legacy_name
	from fishing.species
)
insert into fishing.catch (
    event_id,
    competitor_id,
	species_id,
    size,
    caught_at,
    bait,
    location_name
)
select
    event.id as event_id,
    comp.id as new_competitor_id,
    fish_species.id,
    size * 100 as size,
    (date::date + coalesce(time::time, '00:00'::time))::timestamptz at time zone 'AEDT' as caught_at,
    coalesce(bait, '') as bait,
    coalesce(c.location, '') as location
from legacy_2012.catch as c
join event on true
left join fish_species on fish_species.legacy_name = c.fish_type
left join fishing.competitor comp on comp.competitor_no::int = c.competitor_id and comp.event_id = event.id;

-- 2013
with event as (
	select id from fishing.event where slug = '2013'
),
fish_species as (
	select *,
	case
		when slug = 'murraycod' then 'Murray Cod'
		when slug = 'yellowbelly' then 'Yellowbelly'
		when slug = 'carp' then 'Carp'
	end as legacy_name
	from fishing.species
)
insert into fishing.catch (
    event_id,
    competitor_id,
	species_id,
    size,
    caught_at,
    bait,
    location_name
)
select
    event.id as event_id,
    comp.id as new_competitor_id,
    fish_species.id,
    size * 100 as size,
    (date::date + coalesce(time::time, '00:00'::time))::timestamptz at time zone 'AEDT' as caught_at,
    coalesce(bait, '') as bait,
    coalesce(c.location, '') as location
from legacy_2013.catch as c
join event on true
left join fish_species on fish_species.legacy_name = c.fish_type
left join fishing.competitor comp on comp.competitor_no::int = c.competitor_id and comp.event_id = event.id;

-- 2014
with event as (
	select id from fishing.event where slug = '2014'
),
fish_species as (
	select *,
	case
		when slug = 'murraycod' then 'Murray Cod'
		when slug = 'yellowbelly' then 'Yellowbelly'
		when slug = 'carp' then 'Carp'
	end as legacy_name
	from fishing.species
)
insert into fishing.catch (
    event_id,
    competitor_id,
	species_id,
    size,
    caught_at,
    bait,
    location_name
)
select
    event.id as event_id,
    comp.id as new_competitor_id,
    fish_species.id,
    size * 100 as size,
    (date::date + coalesce(time::time, '00:00'::time))::timestamptz at time zone 'AEDT' as caught_at,
    coalesce(bait, '') as bait,
    coalesce(c.location::text, '') as location
from legacy_2014.catch as c
join event on true
left join fish_species on fish_species.legacy_name = c.fish_type
left join fishing.competitor comp on comp.competitor_no::int = c.competitor_id and comp.event_id = event.id;

-- 2015
with event as (
	select id from fishing.event where slug = '2015'
),
fish_species as (
	select *,
	case
		when slug = 'murraycod' then 'Murray Cod'
		when slug = 'yellowbelly' then 'Yellowbelly'
		when slug = 'carp' then 'Carp'
	end as legacy_name
	from fishing.species
)
insert into fishing.catch (
    event_id,
    competitor_id,
	species_id,
    size,
    caught_at,
    bait,
    location_name
)
select
    event.id as event_id,
    comp.id as new_competitor_id,
    fish_species.id,
    size * 100 as size,
    (date::date + coalesce(time::time, '00:00'::time))::timestamptz at time zone 'AEDT' as caught_at,
    coalesce(bait, '') as bait,
    coalesce(c.location::text, '') as location
from legacy_2015.catch as c
join event on true
left join fish_species on fish_species.legacy_name = c.fish_type
left join fishing.competitor comp on comp.competitor_no::int = c.competitor_id and comp.event_id = event.id;

-- 2016
with event as (
	select id from fishing.event where slug = '2016'
),
fish_species as (
	select *,
	case
		when slug = 'murraycod' then 'Murray Cod'
		when slug = 'yellowbelly' then 'Yellowbelly'
		when slug = 'carp' then 'Carp'
	end as legacy_name
	from fishing.species
)
insert into fishing.catch (
    event_id,
    competitor_id,
	species_id,
    size,
    caught_at,
    bait,
    location_name
)
select
    event.id as event_id,
    comp.id as new_competitor_id,
    fish_species.id,
    size * 100 as size,
    (date::date + coalesce(time::time, '00:00'::time))::timestamptz at time zone 'AEDT' as caught_at,
    coalesce(bait, '') as bait,
    coalesce(c.location::text, '') as location
from legacy_2016.catch as c
join event on true
left join fish_species on fish_species.legacy_name = c.fish_type
left join fishing.competitor comp on comp.competitor_no::int = c.competitor_id and comp.event_id = event.id;

-- 2017
with event as (
	select id from fishing.event where slug = '2017'
),
fish_species as (
	select *,
	case
		when slug = 'murraycod' then 'Murray Cod'
		when slug = 'yellowbelly' then 'Yellowbelly'
		when slug = 'carp' then 'Carp'
	end as legacy_name
	from fishing.species
)
insert into fishing.catch (
    event_id,
    competitor_id,
	species_id,
    size,
    caught_at,
    bait,
    location_name
)
select
    event.id as event_id,
    comp.id as new_competitor_id,
    fish_species.id,
    size * 100 as size,
    (date::date + coalesce(time::time, '00:00'::time))::timestamptz at time zone 'AEDT' as caught_at,
    coalesce(bait, '') as bait,
    coalesce(c.location::text, '') as location
from legacy_2017.catch as c
join event on true
left join fish_species on fish_species.legacy_name = c.fish_type
left join fishing.competitor comp on comp.competitor_no::int = c.competitor_id and comp.event_id = event.id;

-- 2018
with event as (
	select id from fishing.event where slug = '2018'
),
fish_species as (
	select *,
	case
		when slug = 'murraycod' then 'Murray Cod'
		when slug = 'yellowbelly' then 'Yellowbelly'
		when slug = 'carp' then 'Carp'
	end as legacy_name
	from fishing.species
)
insert into fishing.catch (
    event_id,
    competitor_id,
	species_id,
    size,
    caught_at,
    bait,
    location_name
)
select
    event.id as event_id,
    comp.id as new_competitor_id,
    fish_species.id,
    size * 100 as size,
    (date::date + coalesce(time::time, '00:00'::time))::timestamptz at time zone 'AEDT' as caught_at,
    coalesce(bait, '') as bait,
    coalesce(c.location::text, '') as location
from legacy_2018.catch as c
join event on true
left join fish_species on fish_species.legacy_name = c.fish_type
left join fishing.competitor comp on comp.competitor_no::int = c.competitor_id and comp.event_id = event.id;

-- 2019
with event as (
	select id from fishing.event where slug = '2019'
),
fish_species as (
	select *,
	case
		when slug = 'murraycod' then 'Murray Cod'
		when slug = 'yellowbelly' then 'Yellowbelly'
		when slug = 'carp' then 'Carp'
	end as legacy_name
	from fishing.species
)
insert into fishing.catch (
    event_id,
    competitor_id,
	species_id,
    size,
    caught_at,
    bait,
    location_name
)
select
    event.id as event_id,
    comp.id as new_competitor_id,
    fish_species.id,
    size * 100 as size,
    (date::date + coalesce(time::time, '00:00'::time))::timestamptz at time zone 'AEDT' as caught_at,
    coalesce(bait, '') as bait,
    coalesce(c.location::text, '') as location
from legacy_2019.catch as c
join event on true
left join fish_species on fish_species.legacy_name = c.fish_type
left join fishing.competitor comp on comp.competitor_no::int = c.competitor_id and comp.event_id = event.id;

-- 2021
with event as (
	select id from fishing.event where slug = '2021'
),
fish_species as (
	select *,
	case
		when slug = 'murraycod' then 'Murray Cod'
		when slug = 'yellowbelly' then 'Yellowbelly'
		when slug = 'carp' then 'Carp'
	end as legacy_name
	from fishing.species
)
insert into fishing.catch (
    event_id,
    competitor_id,
	species_id,
    size,
    caught_at,
    bait,
    location_name
)
select
    event.id as event_id,
    comp.id as new_competitor_id,
    fish_species.id,
    size * 100 as size,
    (date::date + coalesce(time::time, '00:00'::time))::timestamptz at time zone 'AEDT' as caught_at,
    coalesce(bait, '') as bait,
    coalesce(c.location::text, '') as location
from legacy_2021.catch as c
join event on true
left join fish_species on fish_species.legacy_name = c.fish_type
left join fishing.competitor comp on comp.competitor_no::int = c.competitor_id and comp.event_id = event.id;

-- 2022
with event as (
	select id from fishing.event where slug = '2022'
),
fish_species as (
	select *,
	case
		when slug = 'murraycod' then 'Murray Cod'
		when slug = 'yellowbelly' then 'Yellowbelly'
		when slug = 'carp' then 'Carp'
	end as legacy_name
	from fishing.species
)
insert into fishing.catch (
    event_id,
    competitor_id,
	species_id,
    size,
    caught_at,
    bait,
    location_name
)
select
    event.id as event_id,
    comp.id as new_competitor_id,
    fish_species.id,
    size * 100 as size,
    (date::date + coalesce(time::time, '00:00'::time))::timestamptz at time zone 'AEDT' as caught_at,
    coalesce(bait, '') as bait,
    coalesce(c.location::text, '') as location
from legacy_2022.catch as c
join event on true
left join fish_species on fish_species.legacy_name = c.fish_type
left join fishing.competitor comp on comp.competitor_no::int = c.competitor_id and comp.event_id = event.id;
