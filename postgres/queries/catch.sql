
-- approve catch
with new_catch as (
	insert into fishing.catch (competitor_id, species_id, size, caught_at, bait, location_name, location, photo_url, event_id)
	select competitor_id, species_id, size, caught_at, bait, location_name, location, photo_url, event_id
	from fishing.catch_review
	where id = $1 
	returning *
)
update fishing.catch_review
set reviewer_user_id = NULL,
	reviewer_comment = $2,
	reviewed_at = now(),
	status = 'approved',
	catch_id = new_catch.id
from new_catch
where catch_review.id = $1;



-- reject catch
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
set reviewer_user_id = NULL,
	reviewer_comment = $2,
	reviewed_at = now(),
	status = 'rejected',
	catch_id = NULL
where catch_review.id = $1;

