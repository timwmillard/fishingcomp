
-- Competitors at event
select
	event.slug as year,
	count(case when competitor.competitor_no::int <= 300 then competitor_no end) as adults,
	count(case when competitor.competitor_no::int > 300 then competitor_no end) as juniors,
	count(competitor_no) as total
from fishing.competitor competitor
join fishing.event event on event.id = competitor.event_id
group by event_id, event.slug, event.start_timestamp
order by event.start_timestamp, slug;

-- Total Adult Competitors by Event
select event.slug, count(competitor_no)
from fishing.competitor competitor
join fishing.event event on event.id = competitor.event_id
where competitor.competitor_no::int <= 300
group by event_id, event.slug, event.start_timestamp
order by event.start_timestamp, slug;


-- Total Junior Competitors by Event
select event.slug, count(competitor_no)
from fishing.competitor competitor
join fishing.event event on event.id = competitor.event_id
where
    (competitor.competitor_no::int > 300 and event.slug != '2014')
    or
    (competitor.competitor_no::int > 300 and competitor.competitor_no::int < 1000 and event.slug = '2014')
group by event_id, event.slug, event.start_timestamp
order by event.start_timestamp, slug;


-- Catch List
select
    comp.competitor_no,
    comp.first_name || ' ' || comp.last_name as competitor,
    fish.name,
    size / 100 as size_cm,
    caught_at,
    bait,
    catch.location_name
from fishing.catch catch
left join fishing.competitor comp on comp.id = catch.competitor_id
left join fishing.species fish on fish.id = catch.species_id
left join fishing.event event on event.id = catch.event_id
where event.slug = '2019'
order by catch.caught_at;

-- name: longest-murray-cod
select competitor_id,
	first_name,
	last_name,
	catch.size / 100 as size,
	species.name as fish,
	caught_at,
	location_name,
	bait
from fishing.catch catch
join fishing.competitor comp on comp.id = catch.competitor_id
join fishing.event event on event.id = catch.event_id
join fishing.species species on species.id = catch.species_id
where event.slug = '2009' and species.slug = 'murraycod'
order by size desc;

-- name: longest-yellowbelly
select competitor_id,
	first_name,
	last_name,
	catch.size / 100 as size,
	species.name as fish,
	caught_at,
	location_name,
	bait
from fishing.catch catch
join fishing.competitor comp on comp.id = catch.competitor_id
join fishing.event event on event.id = catch.event_id
join fishing.species species on species.id = catch.species_id
where event.slug = '2009' and species.slug = 'yellowbelly'
order by size desc;

-- approve catch
prepare approve_catch as
with new_catch as (
	insert into fishing.catch (competitor_id, species_id, size, caught_at, bait, location_name, location, photo_url, event_id)
	select competitor_id, species_id, size, caught_at, bait, location_name, location, photo_url, event_id
	from fishing.catch_review
	where id = $1
	returning *
)
update fishing.catch_review
set reviewer_user_id = $2,
	reviewer_comment = $3,
	reviewed_at = now(),
	status = 'approved',
	catch_id = new_catch.id
from new_catch
where catch_review.id = $1;

execute approve_catch(6, 1, 'Great fish');


-- reject catch
prepare reject_catch as
with delete_catch as (
	delete
	from fishing.catch
	where id IN (
		select catch_id
		from fishing.catch_review
		where id = $1
	)
)
update fishing.catch_review
set reviewer_user_id = $2,
	reviewer_comment = $3,
	reviewed_at = now(),
	status = 'rejected',
	catch_id = NULL
where catch_review.id = $1;

execute reject_catch(7, 1, 'Invalid');
