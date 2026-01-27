
-- Boat
create table if not exists boat (
    id integer primary key,
    name text not null default '',
    boat_rego text not null default ''
);

-- Competitor
create table if not exists competitor (
    id integer primary key,
    first_name text not null default '',
    last_name text not null default '',
    email text not null default '',
    mobile text not null default '',
    phone text not null default '',
    address1 text not null default '',
    address2 text not null default '',
    suburb text not null default '',
    state text not null default '',
    postcode text not null default '',
    boat_id integer references boat(id)
);

-- Species
create table if not exists species (
    id integer primary key,
    name text not null,
    slug text not null,
    scientific_name text not null default '',
    common_names text not null default '',
    photo_url text not null default '',
    type text check (type in ('native', 'introduced')) not null default 'native',
    unique(name),
    unique(slug)
);

-- Catch
create table if not exists catch (
    id integer primary key,
    competitor_id integer not null references competitor(id),
    species_id integer not null references species(id),
    size integer not null check (size > 0),
    caught_at timestamptz not null,
    bait text not null default '',
    location_name text not null default '',
    location point
);

-- create type catch_review_status as enum (
--     'submitted',
--     'approved',
--     'rejected'
-- );
--
-- -- Catch
-- create table catch_review (
--   id integer primary key,
--   competitor_id integer not null references competitor(id),
--   species_id integer not null references species(id),
--   size integer not null,
--   caught_at timestamptz not null,
--   bait text not null default '',
--   location_name text not null default '',
--   location point,
--   photo_url text,
--   submitter_user_id uuid,  -- references auth.users(id),
--   submitted_at timestamptz default now(),
--   reviewer_user_id uuid, -- references auth.users(id),
--   reviewer_comment text not null default '',
--   reviewed_at timestamptz,
--   status catch_review_status not null default 'submitted' check (
--       (status = 'submitted' and catch_id is null) or
--       (status = 'approved' and catch_id is not null and reviewer_user_id is not null) or
--       (status = 'rejected' and catch_id is null and reviewer_user_id is not null)
--                                                                         ),
--   catch_id integer references catch(id) check (status = 'approved')
-- );
--
-- -- Catch Photo
-- create table catch_photo (
--     id integer primary key,
--     catch_id integer references catch(id),
--     storage_id text not null check (storage_id <> ''),
--     url text not null default '',
--     is_feature timestamptz
-- );
