
pragma foreign_keys = 1;
pragma journal_mode = 'wal';

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
    id text not null primary key,
    name text not null,
    scientific_name text not null default '',
    common_names text not null default '',
    photo_url text not null default '',
    type text check (type in ('native', 'introduced')) not null default 'native',
    unique(name)
);

insert into species (id, name, scientific_name, common_names, type) values
('murraycod', 'Murray Cod', 'Maccullochella peelii', 'Murray cod', 'native'),
('yellowbelly', 'Yellowbelly', 'Macquaria ambigua', 'Golden perch,Yellowbelly', 'native'),
('carp', 'Carp', 'Cyprinidae', 'Carp', 'introduced') on conflict do nothing;

-- Catch
create table if not exists catch (
    id integer primary key,
    competitor_id integer not null references competitor(id),
    species_id text not null references species(id),
    size integer not null check (size > 0),
    caught_at timestamptz not null,
    bait text not null default '',
    location_name text not null default '',
    location point
);

-- create type catch_review_status as enum (
--     'pending',
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
--   photo text, -- or blob?
--   submitter text,
--   submitted_at timestamptz default now(),
--   reviewer text,
--   reviewer_comment text,
--   reviewed_at timestamptz,
--   status catch_review_status not null default 'submitted' check (
--       (status = 'pending' and catch_id is null) or
--       (status = 'approved' and catch_id is not null and reviewer_user_id is not null) or
--       (status = 'rejected' and catch_id is null and reviewer_user_id is not null)
--                                                                         ),
--   catch_id integer references catch(id) check (status = 'approved'),
--   created_at timestamptz default now()
-- );

