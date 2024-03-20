create type fishing.catch_review_status as enum (
    'submitted',
    'approved',
    'rejected'
);

-- Catch
create table fishing.catch_review (
  id bigint generated always as identity primary key,
  competitor_id bigint not null references fishing.competitor(id),
  species_id bigint not null references fishing.species(id),
  size int not null,
  caught_at timestamptz not null,
  bait text not null default '',
  location_name text not null default '',
  location point,
  photo_url text,
  event_id bigint not null references fishing.event(id),
  submitter_user_id uuid,  -- references auth.users(id),
  submitted_at timestamptz default now(),
  reviewer_user_id uuid, -- references auth.users(id),
  reviewer_comment text not null default '',
  reviewed_at timestamptz,
  status fishing.catch_review_status not null default 'submitted' check (
                                                                          (status = 'submitted' and catch_id is null) or
                                                                          (status = 'approved' and catch_id is not null and reviewer_user_id is not null) or
                                                                          (status = 'rejected' and catch_id is null and reviewer_user_id is not null)
                                                                        ),
  catch_id bigint references fishing.catch(id) check (status = 'approved')
);

-- Catch Photo
create table catch_photo (
    id bigint generated always as identity primary key,
    catch_id bigint references fishing.catch(id),
    storage_object_id uuid not null, -- references storage.buckets(id),
    is_feature timestamptz
);

