-- fishingcomp inital schema

create schema if not exists auth;

create table auth.user (
    id bigint generated always as identity primary key,
    username text not null unique check (username <> ''),
    password text not null check (password <> ''),
    email text unique,
    email_confirmed_at timestamptz,
    phone text unique,
    phone_confirmed_at timestamptz
);

create schema if not exists fishing;

create table fishing.address (
    id bigint generated always as identity primary key,
    line1 text not null default '',
    line2 text not null default '',
    suburb text not null default '',
    state text not null default '',
    postcode text not null default '',
    country text not null default ''
);

-- Club
create table if not exists fishing.club (
    id bigint generated always as identity primary key,
    name text not null,
    slug text not null,
    legal_name text not null default '',
    logo_url text not null default '',
    custom_domain text not null default '',
    billing_address_id bigint references fishing.address(id),
    settings json not null default '{}'
    unique(slug)
);

-- Event
create table if not exists fishing.event (
    id bigint generated always as identity primary key,
    name text not null,
    slug text not null,
    start_timestamp timestamptz,
    end_timestamp timestamptz,
    location text not null default '',
    settings json not null default '{}',
    club_id bigint not null references fishing.club(id),
    unique(id, slug)
);

alter table fishing.club add foreign key (current_event) references fishing.event(id);

-- Person
create table if not exists fishing.person (
    id bigint generated always as identity primary key,
    first_name text not null default '',
    last_name text not null default '',
    email text not null default '',
    mobile text not null default '',
    address_id bigint references fishing.address(id),
    meta json not null default '{}',
    settings json not null default '{}'
);

-- Boat
create table if not exists fishing.boat (
    id bigint generated always as identity primary key,
    boat_no text not null default '', -- TODO: make an int
    name text not null default '',
    boat_rego text not null default '',
    event_id bigint not null references fishing.event(id)
);

create unique index boat_event_id_boat_no_key on fishing.boat(event_id, boat_no) where boat_no <> '';

-- Competitor
create table if not exists fishing.competitor  (
    id bigint generated always as identity primary key,
    competitor_no text default '', -- TODO: make an int
    first_name text default '',
    last_name text default '',
    email text default '',
    mobile text default '',
    phone text default '',
    address1 text default '',
    address2 text default '',
    suburb text default '',
    state text default '',
    postcode text default '',
    event_id bigint not null references fishing.event(id),
    boat_id bigint references fishing.boat(id),
    person_id bigint references fishing.person(id)
);

create unique index competitor_event_id_competitor_no_key on fishing.competitor(event_id, competitor_no) where competitor_no <> '';

create type fishing.species_type as enum (
    'native',
    'introduced'
);
-- Species
create table fishing.species (
    id bigint generated always as identity primary key,
    name text not null,
    scientific_name text not null default '',
    common_names text[] not null default array[]::text[],
    slug text not null,
    photo_url text not null default '',
    type fishing.species_type not null default 'native',
    unique(name),
    unique(slug)
);

-- Catch
create table fishing.catch (
    id bigint generated always as identity primary key,
    competitor_id bigint not null references fishing.competitor(id),
    species_id bigint not null references fishing.species(id),
    size int not null check (size > 0),
    caught_at timestamptz not null,
    bait text not null default '',
    location_name text not null default '',
    location point,
    event_id bigint not null references fishing.event(id)
);

comment on column fishing.catch.size is 'fish length in millimetres';

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
    storage_id text not null check (storage_id <> ''),
    url text not null default '',
    is_feature timestamptz
);

