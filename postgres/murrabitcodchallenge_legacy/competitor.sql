-- Competitors

alter table fishing.competitor add column legacy_boat_id int;

-- 2009
with event as (
	select id from fishing.event where slug = '2009'
)
insert into fishing.competitor (
	event_id,
	competitor_no,
	first_name,
	last_name,
    address1,
    suburb,
    state,
    postcode,
    email,
    mobile,
    phone,
    legacy_boat_id
)
select
    event.id,
    competitor_id,
    firstname,
    coalesce(surname, ''),
    coalesce(address, ''),
    coalesce(suburb, ''),
    coalesce(state, ''),
    coalesce(postcode, ''),
    coalesce(email, ''),
    coalesce(mobile, ''),
    coalesce(phone, ''),
    boat_id
from legacy_2009.competitor, event;

-- 2010
with event as (
	select id from fishing.event where slug = '2010'
)
insert into fishing.competitor (
	event_id,
	competitor_no,
	first_name,
	last_name,
    address1,
    suburb,
    state,
    postcode,
    email,
    mobile,
    phone,
    legacy_boat_id
)
select
    event.id,
    competitor_id,
    firstname,
    coalesce(surname, ''),
    coalesce(address, ''),
    coalesce(suburb, ''),
    coalesce(state, ''),
    coalesce(postcode, ''),
    coalesce(email, ''),
    coalesce(mobile, ''),
    coalesce(phone, ''),
    boat_id
from legacy_2010.competitor, event;

-- 2011
with event as (
	select id from fishing.event where slug = '2011'
)
insert into fishing.competitor (
	event_id,
	competitor_no,
	first_name,
	last_name,
    address1,
    suburb,
    state,
    postcode,
    email,
    mobile,
    phone,
    legacy_boat_id
)
select
    event.id,
    competitor_id,
    firstname,
    coalesce(surname, ''),
    coalesce(address, ''),
    coalesce(suburb, ''),
    coalesce(state, ''),
    coalesce(postcode, ''),
    coalesce(email, ''),
    coalesce(mobile, ''),
    coalesce(phone, ''),
    boat_id
from legacy_2011.competitor, event;

-- 2012
with event as (
	select id from fishing.event where slug = '2012'
)
insert into fishing.competitor (
	event_id,
	competitor_no,
	first_name,
	last_name,
    address1,
    suburb,
    state,
    postcode,
    email,
    mobile,
    phone,
    legacy_boat_id
)
select
    event.id,
    competitor_id,
    firstname,
    coalesce(surname, ''),
    coalesce(address, ''),
    coalesce(suburb, ''),
    coalesce(state, ''),
    coalesce(postcode, ''),
    coalesce(email, ''),
    coalesce(mobile, ''),
    coalesce(phone, ''),
    boat_id
from legacy_2012.competitor, event;

-- 2013
with event as (
	select id from fishing.event where slug = '2013'
)
insert into fishing.competitor (
	event_id,
	competitor_no,
	first_name,
	last_name,
    address1,
    suburb,
    state,
    postcode,
    email,
    mobile,
    phone,
    legacy_boat_id
)
select
    event.id,
    competitor_id,
    firstname,
    coalesce(surname, ''),
    coalesce(address, ''),
    coalesce(suburb, ''),
    coalesce(state, ''),
    coalesce(postcode, ''),
    coalesce(email, ''),
    coalesce(mobile, ''),
    coalesce(phone, ''),
    boat_id
from legacy_2013.competitor, event;

-- 2014
with event as (
	select id from fishing.event where slug = '2014'
)
insert into fishing.competitor (
	event_id,
	competitor_no,
	first_name,
	last_name,
    address1,
    suburb,
    state,
    postcode,
    email,
    mobile,
    phone,
    legacy_boat_id
)
select
    event.id,
    competitor_id,
    firstname,
    coalesce(surname, ''),
    coalesce(address, ''),
    coalesce(suburb, ''),
    coalesce(state, ''),
    coalesce(postcode, ''),
    coalesce(email, ''),
    coalesce(mobile, ''),
    coalesce(phone, ''),
    boat_id
from legacy_2014.competitor, event;

-- 2015
with event as (
	select id from fishing.event where slug = '2015'
)
insert into fishing.competitor (
	event_id,
	competitor_no,
	first_name,
	last_name,
    address1,
    suburb,
    state,
    postcode,
    email,
    mobile,
    phone,
    legacy_boat_id
)
select
    event.id,
    competitor_id,
    firstname,
    coalesce(surname, ''),
    coalesce(address, ''),
    coalesce(suburb, ''),
    coalesce(state, ''),
    coalesce(postcode, ''),
    coalesce(email, ''),
    coalesce(mobile, ''),
    coalesce(phone, ''),
    boat_id
from legacy_2015.competitor, event;

-- 2016
with event as (
	select id from fishing.event where slug = '2016'
)
insert into fishing.competitor (
	event_id,
	competitor_no,
	first_name,
	last_name,
    address1,
    suburb,
    state,
    postcode,
    email,
    mobile,
    phone,
    legacy_boat_id
)
select
    event.id,
    competitor_id,
    firstname,
    coalesce(surname, ''),
    coalesce(address, ''),
    coalesce(suburb, ''),
    coalesce(state, ''),
    coalesce(postcode, ''),
    coalesce(email, ''),
    coalesce(mobile, ''),
    coalesce(phone, ''),
    boat_id
from legacy_2016.competitor, event;

-- 2017
with event as (
	select id from fishing.event where slug = '2017'
)
insert into fishing.competitor (
	event_id,
	competitor_no,
	first_name,
	last_name,
    address1,
    suburb,
    state,
    postcode,
    email,
    mobile,
    phone,
    legacy_boat_id
)
select
    event.id,
    competitor_id,
    firstname,
    coalesce(surname, ''),
    coalesce(address, ''),
    coalesce(suburb, ''),
    coalesce(state, ''),
    coalesce(postcode, ''),
    coalesce(email, ''),
    coalesce(mobile, ''),
    coalesce(phone, ''),
    boat_id
from legacy_2017.competitor, event;

-- 2018
with event as (
	select id from fishing.event where slug = '2018'
)
insert into fishing.competitor (
	event_id,
	competitor_no,
	first_name,
	last_name,
    address1,
    suburb,
    state,
    postcode,
    email,
    mobile,
    phone,
    legacy_boat_id
)
select
    event.id,
    competitor_id,
    firstname,
    coalesce(surname, ''),
    coalesce(address, ''),
    coalesce(suburb, ''),
    coalesce(state, ''),
    coalesce(postcode, ''),
    coalesce(email, ''),
    coalesce(mobile, ''),
    coalesce(phone, ''),
    boat_id
from legacy_2018.competitor, event;

-- 2019
with event as (
	select id from fishing.event where slug = '2019'
)
insert into fishing.competitor (
	event_id,
	competitor_no,
	first_name,
	last_name,
    address1,
    suburb,
    state,
    postcode,
    email,
    mobile,
    phone,
    legacy_boat_id
)
select
    event.id,
    competitor_id,
    firstname,
    coalesce(surname, ''),
    coalesce(address, ''),
    coalesce(suburb, ''),
    coalesce(state, ''),
    coalesce(postcode, ''),
    coalesce(email, ''),
    coalesce(mobile, ''),
    coalesce(phone, ''),
    boat_id
from legacy_2019.competitor, event;

-- 2021
with event as (
	select id from fishing.event where slug = '2021'
)
insert into fishing.competitor (
	event_id,
	competitor_no,
	first_name,
	last_name,
    address1,
    suburb,
    state,
    postcode,
    email,
    mobile,
    phone,
    legacy_boat_id
)
select
    event.id,
    competitor_id,
    firstname,
    coalesce(surname, ''),
    coalesce(address, ''),
    coalesce(suburb, ''),
    coalesce(state, ''),
    coalesce(postcode, ''),
    coalesce(email, ''),
    coalesce(mobile, ''),
    coalesce(phone, ''),
    boat_id
from legacy_2021.competitor, event;


-- 2022
with event as (
	select id from fishing.event where slug = '2022'
)
insert into fishing.competitor (
	event_id,
	competitor_no,
	first_name,
	last_name,
    address1,
    suburb,
    state,
    postcode,
    email,
    mobile,
    phone,
    legacy_boat_id
)
select
    event.id,
    competitor_id,
    firstname,
    coalesce(surname, ''),
    coalesce(address, ''),
    coalesce(suburb, ''),
    coalesce(state, ''),
    coalesce(postcode, ''),
    coalesce(email, ''),
    coalesce(mobile, ''),
    coalesce(phone, ''),
    boat_id
from legacy_2022.competitor, event;


-- Clean data

update fishing.competitor
set email = ''
where email = 'timwmillard@gmail.com' and last_name <> 'Millard';
