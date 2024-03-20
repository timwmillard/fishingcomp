-- fishingcomp inital schema

create schema if not exists fishing;

-- Club
create table if not exists fishing.club (
    id bigint generated always as identity primary key,
    name text not null,
    slug text not null,
    legal_name text not null default '',
    logo_url text not null default '',
    custom_domain text not null default '',
    billing_address1 text not null default '',
    billing_address2 text not null default '',
    billing_suburb text not null default '',
    billing_state text not null default '',
    billing_postcode text not null default '',
    settings json not null default '{}',
    current_event bigint default null,
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
    address1 text not null default '',
    address2 text not null default '',
    suburb text not null default '',
    state text not null default '',
    postcode text not null default '',
    meta json not null default '{}',
    settings json not null default '{}'
);

-- Team
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
    photo_url text not null default '',
    event_id bigint not null references fishing.event(id)
);

comment on column fishing.catch.size is 'fish length in millimetres';

